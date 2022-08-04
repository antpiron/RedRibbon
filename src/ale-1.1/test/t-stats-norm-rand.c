#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (200000)
  double x[LEN] = {0.0};
  double eps = 0.03, res, delta;
  double count = 0;

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_norm_rand(10, 4);
      if (res < 10)
	count++;
    }
  res = count / LEN;
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_rand(10, 4)) == %f != 0.5\n", res);
  res = stats_mean(LEN, x);
  delta = fabs(10 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_norm_rand(10, 4)) == %f != 10.0\n", res);
  res = stats_sd(LEN, x);
  delta = fabs(4.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: sd(stats_norm_rand(10, 4)) == %f != 4.0\n", res);


  return EXIT_SUCCESS;
}
