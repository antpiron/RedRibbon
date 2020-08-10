#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>


#include "rrho.h"
#include "ale.h"
#include "ale/ea.h"

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
rrho_hyper_two_tailed_as_r_module(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res)
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
rrho_hyper_two_tailed(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res)
{
  // http://showmethevotes.org/2016/12/11/hypergeometric-p-value-computation-for-exit-poll-results-using-excel/
  //    2*min(stats_hyper_F(count, i+1, j+1, rrho->n), 1.0 - stats_hyper_F(count-1, i+1, j+1, rrho->n),  0.5)
  // double stats_hyper_F(long k, long K, long n, long N)
  size_t count = count_intersect(rrho, i, j);
  double mean = (double) (i+1) * (double) (j+1) / rrho->n;
  double min_pval = 0.5;
  double pval;
  

  if ( (double) count <= mean )
    res->direction = -1;
  else
    res->direction = 1;

  pval = stats_hyper_F(count, i+1, j+1, rrho->n);
  if ( pval < min_pval )
    min_pval = pval;

  pval = 1.0 - stats_hyper_F(count-1, i+1, j+1, rrho->n);
  if  ( pval < min_pval )
    min_pval = pval;
  
  res->pvalue = 2 * min_pval;
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

  if ( (double) count < mean )
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

int
rrho_rectangle(struct rrho *rrho, size_t i, size_t j, size_t ilen, size_t jlen,
		   size_t m, size_t n, double dst[m][n],
		   int mode)
{
  struct rrho_result res;
  size_t istep = ilen / m;
  size_t jstep = jlen / n;

  for (size_t y = 0 ; y < m ; y++)
    {
      for (size_t x = 0 ; x < n ; x++)
	{
	  size_t ii = i + y * istep;
	  size_t jj = j + x * jstep;
	  rrho_generic(rrho, ii, jj, &res, mode);
	  dst[y][x] = copysign(res.pvalue, res.direction);
	}
    }
  
  return 0;
}



struct params
{
  double prob_mutation;
  double sigma;
  int mode;
  struct rrho *rrho;
};


static double
fitness(struct rrho_coord x,  struct params *param)
{
  struct rrho_result res;
  
  rrho_generic(param->rrho, x.i, x.j, &res, param->mode);
    
  return 1 / (res.pvalue + 1) ;
}

static void
mutate(struct rrho_coord *x, struct params *param)
{
  double u = stats_unif_std_rand();
  if (u < param->prob_mutation)
    {
      x->i += stats_norm_rand(0,param->sigma);
      x->j += stats_norm_rand(0,param->sigma);
    }
}


static void
mate(struct rrho_coord *x, struct rrho_coord m1, struct rrho_coord m2, struct params *param)
{
  (void)param;
  x->i = (m1.i + m2.i)/2;
  x->j = (m1.j + m2.j)/2;
}
  
EA_INIT(optim,struct rrho_coord,mate,mutate,fitness,struct params *);

int
rrho_rectangle_min(struct rrho *rrho, size_t i, size_t j, size_t ilen, size_t jlen,
		   struct rrho_coord *coord, int mode)
{
#define ITER (5000)
  const size_t min_pop_size = 50;
  const size_t max_pop_size = 500;
  struct params param = {.prob_mutation = 0.1, .sigma = 2.0, .mode = mode, .rrho = rrho};
  struct ea_optim ea;
  struct rrho_coord *population = malloc(max_pop_size * sizeof(struct rrho_coord));

  for (size_t c = 0 ; c < max_pop_size ; c++)
    {
      population[c].i = stats_unif_rand(0, ilen);
      population[c].j = stats_unif_rand(0, jlen);
    }
  
  ea_optim_init(&ea, min_pop_size, max_pop_size, population, &param);

  for (size_t iter = 0 ; iter < ITER ; iter++)
    {
      ea_optim_next_generation(&ea, &param);
    }

  size_t index0 = ea.fitness_index[0];
  *coord = ea.population[index0];
  
  free(population);

  return 0;
}
