#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (200000)
  double x[LEN] = {0.0};
  double eps = 0.01, res, delta;
  double count = 0;

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_norm_std_rand();
      if (res < 0)
	count++;
    }
  res = count / LEN;
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_unif_rand_std()) == %f != 0.5\n", res);
  res = stats_mean(LEN, x);
  delta = fabs(0.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_unif_rand_std()) == %f != 0.0\n", res);
  res = stats_sd(LEN, x);
  delta = fabs(1.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: sd(stats_unif_rand_std()) == %f != 1.0\n", res);


  return EXIT_SUCCESS;
}
