#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.01, res, delta;

  res = stats_unif_F(-1000, 10, 20);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_unif_F(-1000, 10, 20)) == NaN\n");
  delta = fabs(res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_F(-1000, 10, 20) == %f != 0\n", res);

  res = stats_unif_F(1000, 10, 20);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_unif_std_F(1000, 10, 20) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_F(1000, 10, 20) == %f != 0\n", res);

  res = stats_unif_F(11, 10, 20);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_unif_F(11, 10, 20) == NaN\n");
  delta = fabs(0.1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_F(11, 10, 20) == %f != 10.123\n", res);


  return EXIT_SUCCESS;
}
