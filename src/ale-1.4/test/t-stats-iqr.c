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
  double res, exp, delta;
  double eps = 0.0001;

  res = stats_IQR(LEN, x);
  ERROR_UNDEF_FATAL_FMT(0.0 != res, "FAIL: stats_IQR() == %f != 0.0\n", res);

  for (int i = 0 ; i < LEN ; i++)
    x[i] = i + 1;

  res = stats_IQR(LEN, x);
  exp = LEN / 2.0;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_IQR() == %f != %f. delta = %f\n",
			res, exp, delta);

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_norm_std_rand();

  res = stats_IQR(LEN, x);
  
  return EXIT_SUCCESS;
}
