#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  int res;
  double eps = 0.01;
  double val1 = 4.0;
  double val2 = val1 + eps / 2;

  res = ale_cmp_double(3, 4, eps);
  ERROR_UNDEF_FATAL_FMT(-1 !=  res, "FAIL: ale_cmp_double(3, 4, %f) == %d != -1\n", eps, res);

  res = ale_cmp_double(4, 3, eps);
  ERROR_UNDEF_FATAL_FMT(1 !=  res, "FAIL: ale_cmp_double(4, 3, %f) == %d != -1\n", eps, res);

  res = ale_cmp_double(val1, val2, eps);
  ERROR_UNDEF_FATAL_FMT(0 !=  res, "FAIL: ale_cmp_double(%f, %f, %f) == %d != -1\n", val1, val2, eps, res);


  return EXIT_SUCCESS;
}
