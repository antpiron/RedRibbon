#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
#define LEN (16)
  double x[LEN] = {0.0};
  double eps = 0.01, res, delta;

  double fact = 1;
  for (int i = 1 ; i < LEN ; fact *= i++)
    {
      res = ale_gamma(i);
      delta = fabs(res - fact);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_gamma(%d) == %f != %f\n",
			    i, res, fact);
    }

  return EXIT_SUCCESS;
}
