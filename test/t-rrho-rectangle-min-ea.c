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
  struct rrho_result rrho_res, rrho_res_exp;
  size_t mres = n/2;
  size_t nres = n/2;
  struct rrho_coord coord;
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
  
  rrho_rectangle_min_ea(&rrho, 0, 0, mres, nres, NULL, RRHO_HYPER, 1, &coord);
  rrho_hyper(&rrho, coord.i, coord.j, &rrho_res);
  rrho_hyper(&rrho, exp_i, exp_j, &rrho_res_exp);
  ERROR_UNDEF_FATAL_FMT(exp_i != coord.i,
			"FAIL: rrho_rectangle_min_ea() i = %zu != %zu (pvalue: %Le, expected = %Le)\n",
			coord.i, exp_i, rrho_res.pvalue, rrho_res_exp.pvalue);
  ERROR_UNDEF_FATAL_FMT(exp_j != coord.j,
			"FAIL: rrho_rectangle_min_ea() j = %zu != %zu (pvalue: %Le, expected = %Le)\n",
			coord.j, exp_j, rrho_res.pvalue, rrho_res_exp.pvalue);

  // TODO: check 0 <= p-value <= 1
  
  rrho_destroy(&rrho);
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
