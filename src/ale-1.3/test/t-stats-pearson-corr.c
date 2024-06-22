#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"


int
main(int argc, char *argv[argc])
{
#define LEN (100000)
  double *x = malloc(sizeof(double) * LEN);
  double *y = malloc(sizeof(double) * LEN);
  double res, exp, delta, pvalue;
  double eps = 0.000001, pval_eps = 0.001;
  int ret;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = y[i] = 0.0;

  ret = stats_pearson_corr(LEN, x, y, &res, &pvalue);
  ERROR_UNDEF_FATAL_FMT(-1 != ret, "FAIL: stats_stats_pearson_corr() == %d != -1\n", ret);

  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = i;
      y[i] = 2*i;
    }

  ret = stats_pearson_corr(LEN, x, y, &res, &pvalue);
  ERROR_UNDEF_FATAL_FMT(-1 == ret,
			"FAIL 1: stats_stats_pearson_corr() == %d != 0\n", ret);
  exp = 1.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL 2: stats_pearson_corr() == %f != %f. delta = %f\n",
			res, exp, delta);
  exp = 0.0;
  delta = fabs(exp - pvalue);
  ERROR_UNDEF_FATAL_FMT(delta >= pval_eps,
			"FAIL 3: stats_pearson_corr().pval == %f != 0\n", pvalue);
  
  for (long i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_unif_std_rand();
      y[i] = stats_unif_std_rand();
    }

  ret = stats_pearson_corr(LEN, x, y, &res, &pvalue);
  ERROR_UNDEF_FATAL_FMT(-1 == ret, "FAIL 4: stats_stats_pearson_corr() == %d != 0\n", ret);
  exp = 0.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(pvalue < pval_eps && delta >= eps,
			"FAIL 5: stats_pearson_corr() == %f != %f. pval = %f\n",
			res, exp, pvalue);
  ERROR_UNDEF_FATAL_FMT(pvalue < pval_eps,
			"FAIL 6: stats_pearson_corr().pval == %f < 0.01\n", pvalue);
  
  free(x);
  free(y);
  
  return EXIT_SUCCESS;
}
