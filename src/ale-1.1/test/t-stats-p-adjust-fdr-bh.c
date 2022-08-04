#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double p[LEN], padj[LEN];
  double res, exp;
  double eps = 0.000000001;

  p[0] = 0.001;
  for (int i = 1 ; i < LEN ; i++)
    p[i] = p[i-1] + 0.001;

  res = stats_p_adjust_fdr_bh(LEN, p, padj);
  ERROR_UNDEF_FATAL_FMT(res < 0, "FAIL: stats_p_adjust_fdr_bh()  res = %d != 0\n", res);
  for (int i = 0 ; i < LEN ; i++)
    {
      printf("%f ", padj[i]);
      ERROR_UNDEF_FATAL_FMT(p[i] > padj[i] , "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f < %f\n", i, padj[i], p[i]);
      ERROR_UNDEF_FATAL_FMT(padj[i] > 1 + eps, "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f > 1\n", i, padj[i]);
      ERROR_UNDEF_FATAL_FMT(fabs(padj[i] - 0.1) > eps, "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f != 0.1\n", i, padj[i]);
    }
  printf("\n");
 
  p[0] = 0.5;
  for (int i = 1 ; i < LEN ; i++)
    p[i] = 0.1;

  res = stats_p_adjust_fdr_bh(LEN, p, padj);
  ERROR_UNDEF_FATAL_FMT(res < 0, "FAIL: stats_p_adjust_fdr_bh()  res = %d != 0\n", res);

  ERROR_UNDEF_FATAL_FMT(fabs(padj[0] - 0.5) > eps, "FAIL: stats_p_adjust_fdr_bh()  padj[0] = %f != \n", padj[0]);
  for (int i = 1 ; i < LEN ; i++)
    {
      printf("%f ", padj[i]);
      ERROR_UNDEF_FATAL_FMT(p[i] > padj[i] , "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f < %f\n", i, padj[i], p[i]);
      ERROR_UNDEF_FATAL_FMT(padj[i] > 1 + eps, "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f > 1\n", i, padj[i]);
      exp = 0.1d * LEN / 99.0d;
      ERROR_UNDEF_FATAL_FMT(fabs(padj[i] - exp) > eps,
			    "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f != %f\n", i, padj[i], exp);
    }

  
  double exp_val[] = {0.001, 0.0055, 0.007, 0.00775, 0.0082, 0.0085, 0.008714286, 0.008875, 0.009, 0.0091};
  int n = sizeof(exp_val) / sizeof(double);
  p[0] = 0.0001;
  for (int i = 1 ; i < n ; i++)
    p[i] = p[i-1] + 0.001;

  res = stats_p_adjust_fdr_bh(n, p, padj);
  ERROR_UNDEF_FATAL_FMT(res < 0, "FAIL: stats_p_adjust_fdr_bh()  res = %d != 0\n", res);

  for (int i = 0 ; i < n && exp_val[i] >= 0 ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(fabs(padj[i] - exp_val[i]) > eps, "FAIL: stats_p_adjust_fdr_bh()  padj[%d] = %f != \n", i, padj[i]);
    }
  
  return EXIT_SUCCESS;
}
