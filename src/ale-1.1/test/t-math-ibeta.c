#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  // https://keisan.casio.com/exec/system/1180573396
  double eps = 0.0001, res, delta;

  
  res = ale_ibeta(0, 1, 1);
  delta = fabs(res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_ibeta(0,1,1) == %f != 0\n",
			res);

  res = ale_ibeta(1, 1, 1);
  delta = fabs(res-1);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_ibeta(1,1,1) == %f != 1\n",
			res);

  res = ale_ibeta(0.99999, 1, 3);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: ale_ibeta(0.99999,1,3) == NaN\n");
  delta = fabs(res-1);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_ibeta(0.99999,1,3) == %f != 1\n",
			res);

  res = ale_ibeta(0.000001, 1, 3);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: ale_ibeta(0.000001,1,3) == NaN\n");
  delta = fabs(res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_ibeta(0.000001,1,3) == %f != 0\n",
			res);

  
  res = ale_ibeta(0.5, 1, 3);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: ale_ibeta(0.5,1,3) == NaN\n");
  delta = fabs(res-0.875);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_ibeta(0.5,1,3) == %f != 0.875\n",
			res);

  return EXIT_SUCCESS;
}
