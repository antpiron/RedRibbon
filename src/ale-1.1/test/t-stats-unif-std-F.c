#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.01, res, delta;

  res = stats_unif_std_F(-1000);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_unif_std_F(-1000)) == NaN\n");
  delta = fabs(res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_std_F(-1000)) == %f != 0\n", res);

  res = stats_unif_std_F(1000);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_unif_std_F(1000)) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_std_F(1000)) == %f != 0\n", res);

  res = stats_unif_std_F(0.123);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_unif_std_F(0.123)) == NaN\n");
  delta = fabs(0.123 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_std_F(0.123)) == %f != 0.123\n", res);


  return EXIT_SUCCESS;
}
