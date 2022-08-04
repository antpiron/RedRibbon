#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (2000000)
  double *x = malloc(sizeof(double)*LEN);
  double eps = 0.01, res, delta;
  double count = 0;
  double alpha = 10, beta = 4;
  double ralpha, rbeta;

  for (int i = 0 ; i < LEN ; i++)
    res = x[i] = stats_beta_rand(alpha, beta);

  stats_beta_fit_mm(LEN, x, &ralpha, &rbeta);
  delta = fabs(ralpha - alpha) / alpha;
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_beta_fit_mm(), alpha == %f != %f\n",
			ralpha, alpha);
  delta = fabs(rbeta - beta) / beta;
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_beta_fit_mm(), beta == %f != %f\n",
			rbeta, beta);

  free(x);

  return EXIT_SUCCESS;
}
