#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN] = {0.0};
  double res, expect;
  double eps = 0.0001;

  for (size_t i = 0 ; i < LEN ; i++)
    x[i] = 1;
  
  res = stats_geom_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, 1.0, eps), "FAIL: stats_geom_mean(1) == %f != 0.0\n", res);

  for (int i = 0 ; i < LEN ; i++)
    x[i] = exp(i + 1);

  res = stats_geom_mean(LEN, x);
  expect = exp(LEN * (LEN + 1.0) / 2.0 / LEN);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: stats_geom_mean(i++) == %f != %f\n", res, expect);
  
  
  return EXIT_SUCCESS;
}
