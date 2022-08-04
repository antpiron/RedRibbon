#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN] = {0.0};
  double eps = 0.01, res, delta;

  double lfact = 0;
  for (int i = 2 ; i < LEN ; lfact += log(i++))
    {
      res = ale_lgamma(i);
      delta = fabs(res - lfact);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: ale_lgamma(%d) == %f != %f\n",
			    i, res, lfact);
    }

  return EXIT_SUCCESS;
}
