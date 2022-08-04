#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

const double eps = 0.0001;

void
test(double x, double alpha, double beta, double exp_res)
{
  double res = stats_beta_F(x, alpha, beta);
  ERROR_UNDEF_FATAL_FMT(isnan(res), "FAIL: count(stats_beta_F(%f, %f, %f) == NaN\n",
			x, alpha, beta);
  double delta = fabs(exp_res - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_beta_F(%f, %f, %f)) == %f != %f\n",
			x, alpha, beta, res, exp_res);

}

int
main(int argc, char *argv[argc])
{

  test(0.5, 1, 1, 0.5);
  test(0.5, 1, 2, 0.75);
  test(0.7, 1, 2, 0.91);
  test(0.1, 1, 2, 0.19);
  test(0.4, 300, 500, 0.9270784);
 
 
  return EXIT_SUCCESS;
}
