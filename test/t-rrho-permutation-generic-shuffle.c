#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include "rrho.h"

#define SIZE (256)
#define NITER (1 << 7)

int
main(int argc, char *argv[argc])
{
  const double eps = 1e-3;
  struct rrho rrho;
  struct rrho_result res;
  struct rrho_permutation_result res_perm;
  size_t n = SIZE;
  double *a = malloc(n * sizeof(double));
  double *b = malloc(n * sizeof(double));
  struct rrho_coord coord;
  long double exp;
  struct rrho_rectangle_params params = { .m = n, .n = n };
  struct stats_permutation permutation;
  
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = i; //stats_unif_std_rand();
    }
  
  stats_shuffle(b, SIZE, sizeof(double));

  rrho_init(&rrho, n, a, b);

  rrho_rectangle_min_generic(&rrho, 0, 0, n, n, &params, RRHO_HYPER, 1, RRHO_CLASSIC, &coord);
  rrho_generic(&rrho, coord.i, coord.j, RRHO_HYPER, &res);
  
  stats_permutation_init(&permutation, n, b);
  
  rrho_permutation_generic(&rrho, 0, 0, n, n, &params,  &permutation, RRHO_HYPER, 1, RRHO_CLASSIC, NITER, res.pvalue, &res_perm);
  printf("shuffled pvalue = %Le, pvalue_perm = %Le, pvalue_ks = %Le\n", res.pvalue, res_perm.pvalue, res_perm.pvalue_ks);
  ERROR_UNDEF_FATAL_FMT(res_perm.pvalue < res.pvalue,
			"FAIL: shuffled rrho_permutation_generic(%d,%d) pvalue_perm = %.20Lf < %Lf = pvalue (pvalue_ks = %Lf)\n",
			n, n, res_perm.pvalue, res.pvalue, res_perm.pvalue_ks);
  /* TODO: Fix failure in 5% of the time */
  ERROR_UNDEF_FATAL_FMT(res_perm.pvalue <= 0.05,
			"FAIL: shuffled rrho_permutation_generic(%d,%d) pval_perm = %.20Lf <= 0.05\n", n, n, res_perm.pvalue);
  
  stats_permutation_destroy(&permutation);
  rrho_destroy(&rrho);
  
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
