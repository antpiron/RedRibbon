#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 1e-14, res, res1, res2, delta;
  double exp;

  res = stats_hyper_F(5, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(5, 5, 5, 10) == NaN\n");
  exp = 1.0d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(5, 5, 5, 10) == %f != %f\n", res, exp);

  res = stats_hyper_F(6, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(6, 5, 5, 10) == NaN\n");
  exp = 1.0d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(6, 5, 5, 10) == %f != %f\n", res, exp);

  res = stats_hyper_F(-1, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(-1, 5, 5, 10) == NaN\n");
  exp = 0.0d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(-1, 5, 5, 10) == %f != %f\n", res, exp);

  res = stats_hyper_F(0, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(0, 5, 5, 10) == NaN\n");
  exp = 3.96825396825396803368590781246894039213657379150391e-03d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(0, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_F(1, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(1, 5, 5, 10) == NaN\n");
  exp = 1.03174603174603099486894564051908673718571662902832e-01d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(1, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_F(2, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(2, 5, 5, 10) == NaN\n");
  exp = 4.99999999999999833466546306226518936455249786376953e-01d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(2, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_F(4, 5, 5, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(2, 5, 5, 10) == NaN\n");
  exp = 9.96031746031746045844101900001987814903259277343750e-01d;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(4, 5, 5, 10) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_F(10000000, 5, 1, 10000000);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(10000000, 5, 1, 10000000) == NaN\n");
  delta = fabs(exp - res);
  eps = 1.0d;
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(10000000, 5, 1, 10000000) == %.20f != %.20f\n", res, exp);

  res = stats_hyper_F(0, 5, 1, 10000000);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_hyper_F(0, 5, 1, 10000000) == NaN\n");
  exp = 0.9999995d;
  delta = fabs(exp - res);
  eps = 1e-7;
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_hyper_F(0, 5, 1, 10000000) == %.20f != %.20f\n", res, exp);

  res1 = stats_hyper_F(0, 5, 2, 1000);
  res2 = stats_hyper_F(0, 2, 5, 1000);
  eps = 1e-7;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res1, res2, eps),
			"FAIL: stats_hyper_F(0, 5, 2, 1000) == %.20f != %.20f = stats_hyper_F(0, 2, 5, 1000)\n",
			res1, res2);
  
  
  
  return EXIT_SUCCESS;
}
