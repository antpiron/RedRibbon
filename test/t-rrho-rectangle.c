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
  size_t mres = 10;
  size_t nres = 10;
  double (*res)[nres] = malloc(nres * mres * sizeof(double));
  size_t n = 1024;
  double *a = malloc(n * sizeof(double));
  double *b = malloc(n * sizeof(double));
  double exp;

  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = 1; //stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  rrho_rectangle(&rrho, 0, 0, mres, nres,  mres, nres, res, RRHO_HYPER);
  exp = 1.0 / (double) n;
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(res[0][0], exp, eps),
			"FAIL: rrho_rectangle(0,0) pval = %f != %s\n", res[0][0], exp);

  rrho_rectangle(&rrho, n-mres, n-nres, mres, nres, mres, nres, res, RRHO_HYPER);
  exp = 1.0;
  ERROR_UNDEF_FATAL_FMT(0 != ale_doublecmp(res[mres-1][nres-1], exp, eps),
			"FAIL: rrho_rectangle(0,0) pval = %f != %s\n", res[mres-1][nres-1], exp);


  // TODO: check 0 <= p-value <= 1
  
  rrho_destroy(&rrho);
  free(res);
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
