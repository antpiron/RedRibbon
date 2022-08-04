#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (1000)
  double x[LEN] = {0.0};
  double y[LEN] = {0.0};
  double res, exp, delta;
  double eps = 0.01;

  res = stats_diff_var(LEN, x, y);
  delta = fabs(res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_diff_var() == %f != 0.0\n", res);

  for (int i = 0 ; i < LEN ; i++)
    y[i] = (i < LEN / 2)?-1:1;

  res = stats_diff_var(LEN, x,y);
  exp = 1.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_diff_var() == %f != %f. delta = %f\n", res, exp, delta);
  
  
  return EXIT_SUCCESS;
}
