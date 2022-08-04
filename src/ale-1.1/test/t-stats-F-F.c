#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
#define LEN (10000)
  double eps = 0.000001, res, exp_res;

  res = stats_F_F(1, 1, 1);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_F_F(1, 1, 1) == NaN\n");
  exp_res = 0.5;
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(res, exp_res, eps), "FAIL: stats_F_F(1, 1, 1) == %f != %f\n", res, exp_res);

  res = stats_F_F(2, 3, 4);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_F_F(2, 3, 4) == NaN\n");
  exp_res = 0.7436128;
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(res, exp_res, eps), "FAIL: stats_F_F(2, 3, 4) == %f != %f\n", res, exp_res);

  res = stats_F_F(0.1,30.5, 1.2);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_F_F(0.1, 30.5, 1.2) == NaN\n");
  exp_res = 0.002234316;
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(res, exp_res, eps), "FAIL: stats_F_F(0.1, 30.5, 1.2) == %f != %f\n", res, exp_res);
  printf("stats_F_F(0.1, 30.5, 1.2) = %f\n", res);

 
  return EXIT_SUCCESS;
}
