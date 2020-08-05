#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>


#include "rrho.h"
#include "ale.h"

static int
indirect_compar_double(const void *p1, const void *p2, void *arg)
{
  const double *d = arg;
  const size_t *a = p1;
  const size_t *b = p2;

  if (d[*a] < d[*b])
    return -1;
  else if (d[*a] == d[*b])
    return 0;
  
  return 1;
}



int
rrho_init(struct rrho *rrho, size_t n, double a[n], double b[n])
{
  rrho->n = n;
  rrho->a = a;
  rrho->b = b;
  rrho->n_a = rrho->n_b = 0;

  rrho->index_a = malloc(sizeof(size_t) * n);
  rrho->index_b = malloc(sizeof(size_t) * n);
  for (size_t i = 0 ; i < n ; i++)
    rrho->index_a[i] = rrho->index_b[i] = i;

  qsort_r(rrho->index_a, n, sizeof(rrho->index_a[0]),
	  indirect_compar_double, rrho->a);

  qsort_r(rrho->index_b, n, sizeof(rrho->index_b[0]),
	  indirect_compar_double, rrho->b);

  bitset_init(&rrho->bs_a,n);
  bitset_init(&rrho->bs_b, n);
  bitset_init(&rrho->bs_and, n);
 
  return 0;
}

int
rrho_destroy(struct rrho *rrho)
{

  free(rrho->index_a);
  free(rrho->index_b);

  bitset_destroy(&rrho->bs_a);
  bitset_destroy(&rrho->bs_b);
  bitset_destroy(&rrho->bs_and);
 
  return 0;
}

static size_t
count_intersect(struct rrho *rrho, size_t i, size_t j)
{
  // TODO: improve performance
  size_t count = 0;
  
  // bitset_reset(&rrho->bs_a);
  // bitset_reset(&rrho->bs_b);
  // bitset_reset(&rrho->bs_and);

  if (rrho->n_a < i+1)
    {
      for (size_t ii = rrho->n_a ; ii < i+1 ; ii++)
	bitset_set(&rrho->bs_a,  rrho->index_a[ii]);
    }
  else
    {
      for (size_t ii = i+1 ; ii < rrho->n_a ; ii++)
	bitset_unset(&rrho->bs_a,  rrho->index_a[ii]);
    }

  if (rrho->n_b < j+1)
    {
      for (size_t jj = rrho->n_b ; jj < j+1 ; jj++)
	bitset_set(&rrho->bs_b,  rrho->index_b[jj]);
    }
  else
    {
      for (size_t jj = j+1 ; jj < rrho->n_b ; jj++)
	bitset_unset(&rrho->bs_b,  rrho->index_b[jj]);	    
    }
   
  bitset_and(&rrho->bs_and, &rrho->bs_a, &rrho->bs_b);
  
  count = bitset_ones(&rrho->bs_and);

  rrho->n_a = i+1;
  rrho->n_b = j+1;
  
  return count;
}

int
rrho_hyper_two_tails(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res)
{
  // try http://showmethevotes.org/2016/12/11/hypergeometric-p-value-computation-for-exit-poll-results-using-excel/
  //    2*min(stats_hyper_F(count, i+1, j+1, rrho->n), 1.0 - stats_hyper_F(count-1, i+1, j+1, rrho->n),  0.5)
  // double stats_hyper_F(long k, long K, long n, long N)
  size_t count = count_intersect(rrho, i, j);
  double mean = (double) (i+1) * (double) (j+1) / rrho->n;
  double symmetric = 2*mean - count;
  long lower = (count < symmetric)?count:ceil(symmetric);
  long upper = (count < symmetric)?floor(symmetric):count;

  if ( (double) count <= mean )
    res->direction = -1;
  else
    res->direction = 1;
  
  res->pvalue = stats_hyper_F(lower, i+1, j+1, rrho->n) +
    (1.0 - stats_hyper_F(upper, i+1, j+1, rrho->n));
  // res->fdr = (0 == count)?-1:mean / count;
  res->count = count;
  
  return 0;
}

int
rrho_hyper(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res)
{
  // https://www.ncbi.nlm.nih.gov/pmc/articles/PMC2943622/
  // double stats_hyper_F(long k, long K, long n, long N)
  size_t count = count_intersect(rrho, i, j);
  double mean = (double) (i+1) * (double) (j+1) / rrho->n;

  if ( (double) count <= mean )
    {
      res->pvalue = stats_hyper_F(count, i+1, j+1, rrho->n);
      res->direction = -1;
    }
  else
    {
      // count-1 because we want to include from count to n
      res->pvalue = 1.0 - stats_hyper_F(count-1, i+1, j+1, rrho->n);
      res->direction = 1;
    }
  // res->fdr = (0 == count)?-1:mean / count;
  res->count = count;
  
  return 0;
}

