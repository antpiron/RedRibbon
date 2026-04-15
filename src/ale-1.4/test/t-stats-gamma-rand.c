#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (200000)
  double x[LEN] = {0.0};
  double eps = 0.03, res, delta;
  double count = 0;
  double alpha = 10, beta = 4;
  double exp_res;

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_gamma_rand(alpha, beta);
      if (res < 10)
	count++;
    }
  // res = count / LEN;
  // delta = fabs(0.5 - res);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_gamma_rand(10, 4)) == %f != 0.5\n", res);
  res = stats_mean(LEN, x);
  delta = fabs(alpha/beta - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: mean(stats_gamma_rand(%f, %f)) == %f != 10.0\n",
			alpha, beta, res);

  res = stats_var(LEN, x);
  exp_res = alpha / (beta*beta);
  delta = fabs(exp_res - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: var(stats_gamma_rand(%f, %f)) == %f != %f\n",
			alpha, beta, res, exp_res);


  return EXIT_SUCCESS;
}
