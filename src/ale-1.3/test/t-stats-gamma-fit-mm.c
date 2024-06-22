#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (200000)
  double x[LEN] = {0.0};
  double eps = 0.1, res, delta;
  double count = 0;
  double alpha = 10, beta = 4;
  double ralpha, rbeta;

  for (int i = 0 ; i < LEN ; i++)
    res = x[i] = stats_gamma_rand(alpha, beta);

  stats_gamma_fit_mm(LEN, x, &ralpha, &rbeta);
  delta = fabs(ralpha - alpha);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_gamma_fit_mm(), alpha == %f != %f\n",
			ralpha, alpha);
  delta = fabs(rbeta - beta);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_gamma_fit_mm(), beta == %f != %f\n",
			rbeta, beta);


  return EXIT_SUCCESS;
}
