#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (100000)
  double x[LEN] = {0.0};
  double eps = 0.1, res, delta, exp;

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_unif_rand(10, 20);
      ERROR_UNDEF_FATAL_FMT(10 > res, "FAIL: mean(stats_unif_rand()) == %f < 10\n", res);
      ERROR_UNDEF_FATAL_FMT(20 < res, "FAIL: mean(stats_unif_rand()) == %f > 10\n", res);
    }
  res = stats_mean(LEN, x);
  delta = fabs(15.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_unif_rand()) == %f != 15\n", res);
  res = stats_var(LEN, x);
  exp = 1.0 / 12.0 * 100;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: var(stats_unif_rand()) == %f != %f\n", res, exp);


  return EXIT_SUCCESS;
}
