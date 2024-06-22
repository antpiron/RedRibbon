#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.000001, res, delta;


  
  res = stats_poisson_f(-1, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_poisson_f(-1, 10) == NaN\n");
  delta = fabs(0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_poisson_f(-1, 10) == %f != 0\n", res);

  res = stats_poisson_f(10, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_poisson_f(10, 10) == NaN\n");
  delta = fabs(0.12511 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_poisson_f(10, 10) == %f != 0.12511\n", res);

  
  res = 0;
  for (long k = 0 ; k < 1000 ; k++)
    {
      res += stats_poisson_f(k, 10);
    }
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: sum(stats_poisson_f(k, 10)) == NaN\n");
  delta = fabs(1 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: sum(stats_poisson_f(k, 10)) == %f != 1\n", res);

  return EXIT_SUCCESS;
}
