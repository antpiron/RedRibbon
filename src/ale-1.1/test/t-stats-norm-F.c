#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.005, res, delta;

  res = stats_norm_F(2, 2, 4);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_norm_F(2, 2, 4) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_F(2, 2, 4)) == %f != 0.5\n", res);

  res = stats_norm_F(3, 2, 4);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_norm_F(3, 2, 4)) == NaN\n");
  delta = fabs(0.599 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_F(3, 2, 4)) == %f != 0.599\n", res);

  res = stats_norm_F(1, 2, 4);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_norm_F(1, 2, 4)) == NaN\n");
  delta = fabs(0.401 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_F(1, 2, 4)) == %f != 0.401\n", res);

  return EXIT_SUCCESS;
}
