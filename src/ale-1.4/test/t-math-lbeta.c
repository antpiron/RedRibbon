#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.0001, res, delta;

  res = ale_lbeta(1, 1);
  delta = fabs(res - 0);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(1,1) == %f != 1\n",
			res);
  res = ale_lbeta(2, 2);
  delta = fabs(res - -1.791759);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(2,2) == %f != 0.1666667\n",
			res);

  res = ale_lbeta(0.1, 0.1);
  delta = fabs(res - 2.981361);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(0.1,0.1) == %f != 19.71464\n",
			res);

  return EXIT_SUCCESS;
}
