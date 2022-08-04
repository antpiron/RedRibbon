#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN] = {0.0};
  double y[LEN] = {0.0};
  double res, exp, delta;
  double eps = 0.0001;

  res = stats_diff_mean(LEN, x, y);
  ERROR_UNDEF_FATAL_FMT(0.0 != res, "FAIL: stats_diff_mean() == %f != 0.0\n", res);

  for (int i = 0 ; i < LEN ; i++)
    y[i] = i + 1;

  res = stats_diff_mean(LEN, x, y);
  exp = LEN * (LEN + 1.0) / 2.0 / LEN;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_diff_mean() == %f != %f. delta = %f\n", res, exp, delta);
  
  
  return EXIT_SUCCESS;
}
