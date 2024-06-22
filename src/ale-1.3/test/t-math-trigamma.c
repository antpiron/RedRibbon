#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.000000000001, res, expect;

  res = ale_trigamma(2);
  expect = 0.6449340668482264060656916626612655818462371826171875;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: ale_trigamma(2) = %.50f != %.50f\n",
			res, expect);

  res = ale_trigamma(100);
  expect = 0.01005016666333356616103156255803696694783866405487060546875;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: ale_trigamma(2) = %.50f != %.50f\n",
			res, expect);

  res = ale_trigamma(0.1);
  expect = 101.433299150792748832827783189713954925537109375;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: ale_trigamma(2) = %.50f != %.50f\n",
			res, expect);

  return EXIT_SUCCESS;
}
