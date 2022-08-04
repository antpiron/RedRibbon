#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

const double eps = 0.00001d;

int
main(int argc, char *argv[argc])
{

  for (double alpha = 0.5 ; alpha < 5 ; alpha += 0.1)
    for (double beta = 0.5 ; beta < 5 ; beta += 0.1)
      for (double x = 0 ; x <= 1 ; x += 0.1)
      {
	double res = stats_beta_F_inv(x, alpha, beta);
	double xres = stats_beta_F(res, alpha, beta);
	
	ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x, xres, eps),
			      "FAIL: stats_beta_F(stats_beta_F_inv(%f, %f, %f) == %f != %f\n",
			      x, alpha, beta, xres, x);
      }
 
 
  return EXIT_SUCCESS;
}
