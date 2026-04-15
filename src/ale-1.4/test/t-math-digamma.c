#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.000000000001, res, expect;

  res = ale_digamma(2);
  expect = 0.4227843350984674675174801450339145958423614501953125;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: ale_digamma(2) = %.50f != %.50f\n",
			res, expect);

  res = ale_digamma(100);
  expect = 4.6001618527380880863120182766579091548919677734375000;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: ale_digamma(2) = %.50f != %.50f\n",
			res, expect);

  res = ale_digamma(0.1);
  expect = -10.4237549404110758644037559861317276954650878906250000;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expect, eps),
			"FAIL: ale_digamma(2) = %.50f != %.50f\n",
			res, expect);

  return EXIT_SUCCESS;
}
