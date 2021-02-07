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
  struct rrho_result res2;
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
  
  rrho_hyper(&rrho, 0, 0, &res);
  exp = 1.0 / (double) n;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res.pvalue, exp, eps),
			"FAIL: rrho_hyper(0,0) pval = %.20e != %.20e\n", res.pvalue, exp);
  ERROR_UNDEF_FATAL_FMT(1 != res.count,
			"FAIL: rrho_hyper(0,0) count = %d != 1\n", res.count);

  rrho_hyper(&rrho, n-1, n-1, &res);
  exp = 1.0;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res.pvalue, exp, eps),
			"FAIL: rrho_hyper(%zu,%zu) pval = %f != %f\n", n-1, n-1, res.pvalue, exp);

  // TODO: check 0 <= p-value <= 1
  
  rrho_destroy(&rrho);
  
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = i; //stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);

  for (size_t j = 0 ; j < n ; j++)
    for (size_t i = j ; i < n ; i++)
      {
	rrho_hyper(&rrho, i, j, &res);
	rrho_hyper(&rrho, j, i, &res2);
	exp = 1.0 / (double) n;
	ERROR_UNDEF_FATAL_FMT(res.count != res2.count || res.count != j+1,
			      "FAIL: rrho_hyper(%d,%d) count = %d != %.d = rrho_hyper(%d,%d)\n", i, j, 
			      res.count, res2.count, j, i);
	ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res.pvalue, res2.pvalue, eps), 
	 		      "FAIL: rrho_hyper(%d,%d) pval = %.20e != %.20e = rrho_hyper(%d,%d)\n", i, j,  
	 		      res.pvalue, res2.pvalue, j, i); 
      }

 
  
  rrho_destroy(&rrho);

  
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
