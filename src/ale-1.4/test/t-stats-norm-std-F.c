#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.01, res, delta;

  res = stats_norm_std_F(0);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_norm_std_F(0)) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_std_F(0)) == %f != 0.5\n", res);

  res = stats_norm_std_F(1.96);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_norm_std_F(1.96)) == NaN\n");
  delta = fabs(0.975 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_std_F(1.96)) == %f != 0.975\n", res);

  res = stats_norm_std_F(-1.96);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_norm_std_F(-1.96)) == NaN\n");
  delta = fabs(0.025 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_std_F(-1.96)) == %f != 0.025\n", res);

  return EXIT_SUCCESS;
}
