#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.0001, res, delta;

  
  res = ale_beta(1, 1);
  delta = fabs(res - 1);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(1,1) == %f != 1\n",
			res);
  res = ale_beta(2, 2);
  delta = fabs(res - 0.1666667);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(2,2) == %f != 0.1666667\n",
			res);

  res = ale_beta(0.1, 0.1);
  delta = fabs(res - 19.71464);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_beta(2,2) == %f != 19.71464\n",
			res);


  return EXIT_SUCCESS;
}
