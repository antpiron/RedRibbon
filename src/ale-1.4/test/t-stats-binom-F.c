#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.00001, res, delta;

  res = stats_binom_F(0, 2, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_F(0, 2, 0.5) == NaN\n");
  delta = fabs(0.25 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_F(0, 2, 0.5) == %f != 0.25 \n", res);

  res = stats_binom_F(1, 2, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_F(1, 2, 0.5) == NaN\n");
  delta = fabs(0.75 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_F(1, 2, 0.5) == %f != 0.75 \n", res);

  res = stats_binom_F(2, 2, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_F(2, 2, 0.5) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_F(2, 2, 0.5) == %f != 1 \n", res);

  res = stats_binom_F(55, 100, 0.5);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_binom_F(55, 100, 0.5) == NaN\n");
  delta = fabs(0.8643735 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_binom_F(55, 100, 0.5) == %f != 1 \n", res);

  

  return EXIT_SUCCESS;
}
