#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 0.0001, res, delta;

  
  res = ale_riugamma(1, 1);
  delta = fabs(res - (1-0.6321206));
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_riugamma(1,1) == %f != 0.6321206\n",
			res);
  
  res = ale_riugamma(0.5, 1);
  delta = fabs(res - (1-0.3934693));
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_riugamma(0.5,1) == %f != 0.3934693\n",
			res);

  res = ale_riugamma(7, 5);
  delta = fabs(res - (1-0.8270084));
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_riugamma(7,5) == %f != 0.8270084\n",
			res);

  res = ale_riugamma(100, 5);
  delta = fabs(res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps,
			"FAIL: ale_riugamma(100,5) == %f != 1\n",
			res);
  

  return EXIT_SUCCESS;
}
