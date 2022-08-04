#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

const double eps = 0.0001;

void
test(double x, double alpha, double beta, double exp_res)
{
  double res = stats_gamma_F(x, alpha, beta);
  ERROR_UNDEF_FATAL_FMT(isnan(res), "FAIL: count(stats_gamma_F(%f, %f, %f) == NaN\n",
			x, alpha, beta);
  double delta = fabs(exp_res - res);
  ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: count(stats_gamma_F(%f, %f, %f)) == %f != %f\n",
			x, alpha, beta, res, exp_res);

}

int
main(int argc, char *argv[argc])
{
  double eps = 0.0001, res, exp_res,
    delta, alpha = 1, beta = 1;


  test(2, 1, 1, 0.8646647);
  test(2, 2, 1, 0.5939942);
  test(2, 2, 2, 0.9084218);
  test(2, 20, 5, 0.003454342);
 
 
  return EXIT_SUCCESS;
}
