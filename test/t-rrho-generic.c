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
  double exp;

  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = i+1; //stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  rrho_generic(&rrho, 0, 0, RRHO_HYPER, &res);
  exp = 1.0 / (double) n;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res.pvalue, exp, eps),
			"FAIL: rrho_hyper(0,0) pval = %.20e != %.20e\n", res.pvalue, exp);

  rrho_generic(&rrho, n-1, n-1, RRHO_HYPER, &res);
  exp = 1.0;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res.pvalue, exp, eps),
			"FAIL: rrho_hyper(%zu,%zu) pval = %f != %s\f", n-1, n-1, res.pvalue, exp);

  // TODO: check 0 <= p-value <= 1
  
  rrho_destroy(&rrho);
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
