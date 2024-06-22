#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.00001, res, delta;

  res = stats_poisson_F(-1, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_poisson_F(-1, 10) == NaN\n");
  delta = fabs(0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_poisson_F(-1, 10) == %f != 0 \n", res);

  res = stats_poisson_F(10, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_poisson_F(10, 10) == NaN\n");
  delta = fabs(0.5830398 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_poisson_F(10, 10) == %f != 0.5830398 \n", res);


  res = stats_poisson_F(1000, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_poisson_F(1000, 10) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_poisson_F(1000, 10) == %f != 1 \n", res);
  

  return EXIT_SUCCESS;
}
