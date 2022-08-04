#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (100000)
  double x[LEN] = {0.0};
  double eps = 0.01, res, delta;

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_unif_std_rand();
      ERROR_UNDEF_FATAL_FMT(0 > res, "FAIL: mean(stats_unif_rand()) == %f < 0\n", res);
      ERROR_UNDEF_FATAL_FMT(1 < res, "FAIL: mean(stats_unif_rand()) == %f > 1\n", res);
    }
  res = stats_mean(LEN, x);
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_unif_rand_std()) == %f != 0.0\n", res);
  res = stats_var(LEN, x);
  delta = fabs(1.0 / 12.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: var(stats_unif_rand_std()) == %f != 0.0\n", res);


  return EXIT_SUCCESS;
}
