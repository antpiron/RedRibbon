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

  rrho->index_a = malloc(sizeof(size_t) * n);
  rrho->index_b = malloc(sizeof(size_t) * n);
  for (size_t i = 0 ; i < n ; i++)
    rrho->index_a[i] = rrho->index_b[i] = i;

  qsort_r(rrho->index_a, n, sizeof(rrho->index_a[0]),
	  indirect_compar_double, rrho->a);

  qsort_r(rrho->index_b, n, sizeof(rrho->index_b[0]),
	  indirect_compar_double, rrho->b);
  
  return 0;
}


static size_t
count_intersect(struct rrho *rrho, size_t i, size_t j)
{
  // TODO: improve performance
  size_t count = 0;
  struct bitset bs;
  
  bitset_init(&bs, rrho->n);
  
  for (size_t jj = 0 ; jj < j+1 ; jj++)
    bitset_set(&bs,  rrho->index_b[jj]);

  for (size_t ii = 0 ; ii < i+1 ; ii++)
    count += bitset_isset(&bs,  rrho->index_a[ii]);
  
  bitset_destroy(&bs);
  
  return count;
}

int
rrho_hyper(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res)
{
  // double stats_hyper_F(long k, long K, long n, long N)
  size_t count = count_intersect(rrho, i, j);

  res->pvalue = stats_hyper_F(count, i, j, rrho->n);
  
  return 0;
}

int
rrho_destroy(struct rrho *rrho)
{

  free(rrho->index_a);
  free(rrho->index_b);
  
  return 0;
}
