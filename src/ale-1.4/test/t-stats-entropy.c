#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (30000)
  double *x = malloc(LEN * sizeof(double));
  double res, exp, delta;
  double eps = 0.01;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_norm_std_rand();

  res = stats_entropy(LEN, x);
  exp = 0.5 * log(2*M_PI) + 0.5;
  delta = fabs(exp - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_entropy() == %f != %f. delta = %f\n", res, exp, delta);

  free(x);
  
  return EXIT_SUCCESS;
}
