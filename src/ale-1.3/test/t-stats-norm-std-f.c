#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.01, res, delta;

  res = stats_norm_std_f(0);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_norm_std_f(0) == NaN\n");
  delta = fabs(0.3989423 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_norm_std_f(0) == %f != 0.3989423\n", res);

  res = stats_norm_std_f(1);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_norm_std_F(1) == NaN\n");
  delta = fabs(0.2419707 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_norm_std_f(1) == %f != 0.2419707\n", res);

  res = stats_norm_std_f(-1);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_norm_std_f(-1) == NaN\n");
  delta = fabs(0.2419707 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_norm_std_f(-1) == %f != 0.2419707\n", res);

  return EXIT_SUCCESS;
}
