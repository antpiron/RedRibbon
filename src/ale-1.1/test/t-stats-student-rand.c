#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (200000)
#define DF (10.0)
  double x[LEN] = {0.0};
  double eps = 0.01, res, delta;
  double count = 0;

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_student_rand(DF);
      ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_student_rand() == NaN\n");
      if (res < 0)
	count++;
    }
  res = count / LEN;
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_student_rand()) == %f != 0.5\n", res);
  
  res = stats_mean(LEN, x);
  delta = fabs(0.0 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_student_rand()) == %f != 0.0\n", res);
  
  res = stats_sd(LEN, x);
  delta = fabs(sqrt(DF/(DF-2)) - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: sd(stats_student_rand()) == %f != 1.0\n", res);


  return EXIT_SUCCESS;
}
