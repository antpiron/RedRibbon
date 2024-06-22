#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (10000)
  double eps = 0.001, res, delta;

  res = stats_student_F(0, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_student_F(0, 10)) == NaN\n");
  delta = fabs(0.5 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_student_F(0, 10)) == %f != 0.5\n", res);

  res = stats_student_F(1.96, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_student_F(1.96, 10)) == NaN\n");
  delta = fabs(0.9607819 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_norm_F(1.96, 10)) == %f != 0.9607819\n", res);

  res = stats_student_F(-1.96, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: count(stats_student_F(-1.96, 10)) == NaN\n");
  delta = fabs(0.03921812 - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_student_F(-1.96, 10)) == %f != 0.03921812\n", res);

  for (double i = -10 ; i < 10 ; i += 0.01)
    {
      double exp = stats_norm_std_F(i);
      res = stats_student_F(i, LEN);
      ERROR_UNDEF_FATAL_FMT(isnan(res), "FAIL: stats_student_F(%f, %d) == NaN\n", i, LEN);
      delta = fabs(exp - res);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_student_F(%f, %d) == %f != %f\n", i, LEN, res, exp);
    }
  
  return EXIT_SUCCESS;
}
