#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  const double eps = 1e-3;
  struct rrho rrho;
  struct rrho_result res;
  size_t n = 1024;
  double *a = malloc(n * sizeof(double));
  double *b = malloc(n * sizeof(double));
  long double exp;

  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = i; //stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  rrho_permutation_generic(&rrho, 512, 512, RRHO_HYPER, 1000, &res);
  ERROR_UNDEF_FATAL_FMT(0.01 > res.pvalue_ks,
			"FAIL: rrho__permutation_generic(512,512) pval_ks = %.20Le <= 0.01\n", res.pvalue_ks);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_doublel(res.pvalue_perm, exp, eps),
			"FAIL: rrho_hyper(0,0) pval_perm = %.20Le != %.20Le\n", res.pvalue_perm, res.pvalue);

  
  rrho_destroy(&rrho);
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
