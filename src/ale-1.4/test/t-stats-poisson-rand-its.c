#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (400000)
  double *x = malloc(LEN * sizeof(double));
  double lambda = 10;
  double eps = 0.01;
  double res, delta;

  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_poisson_rand_its(lambda);
    }
  res = stats_mean(LEN, x);
  delta = fabs(lambda - res) / sqrt(lambda);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_poisson_rand_its()) == %f != %f\n", res, lambda);

  res = stats_var(LEN, x);
  delta = fabs(lambda - res) / lambda;
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: var(stats_poisson_rand_its()) == %f != %f\n", res, lambda);

  free(x);
  
  return EXIT_SUCCESS;
}
