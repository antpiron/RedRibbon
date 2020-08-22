#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  const double eps = 0.00000001;
  struct rrho rrho;
  size_t n = 1ll << 14;
  size_t mres = n/2;
  size_t nres = n/2;
  struct rrho_coord res;
  double *a = malloc(n * sizeof(double));
  double *b = malloc(n * sizeof(double));
  size_t exp_i = 80;
  size_t exp_j = 80;

  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = stats_unif_std_rand();
      b[i] = stats_unif_std_rand();
    }
  for (ssize_t i = 0 ; i < exp_i+1 ; i++)
    a[i] = -i-1 ; // stats_unif_std_rand();
  for (ssize_t j = 0 ; j < exp_j+1 ; j++)
    b[j] = -j-1; //stats_unif_std_rand();
 
  rrho_init(&rrho, n, a, b);
  
  rrho_rectangle_min(&rrho, 0, 0, mres, nres, &res, RRHO_HYPER);
  ERROR_UNDEF_FATAL_FMT(exp_i != res.i,
			"FAIL: rrho_rectangle_min() i = %zu != %zu\n", res.i, exp_i);
  ERROR_UNDEF_FATAL_FMT(exp_j != res.j,
			"FAIL: rrho_rectangle_min() j = %zu != %zu\n", res.j, exp_j);

  // TODO: check 0 <= p-value <= 1
  
  rrho_destroy(&rrho);
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
