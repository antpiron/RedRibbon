#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"
#include "ale/optimisation.h"



double
f(double *x, void *cls)
{
  double *shift = cls;
  double xx = x[0] - *shift;
  double yy = x[1] - *shift;
  
  return xx*xx + yy*yy;
}

void
gradf(double *y, double *x, void *cls)
{
  double *shift = cls;
  double xx = x[0] - *shift;
  double yy = x[1] - *shift;
  
  y[0] = 2 * xx ;
  y[1] = 2 * yy;
}

void
invgradf(double *y, double *x, void *cls)
{
  double *shift = cls;
  double xx = x[0] - *shift;
  double yy = x[1] - *shift;
  
  y[0] = - 2 * xx ;
  y[1] = - 2 * yy;
}

int
main(int argc, char *argv[argc])
{
  const size_t n = 2;
  double x[] =  {1, 1};
  const double eps = ALE_EPS;
  double shift = 0;
  int ret;

  
  ret = optimisation_gradient_descent(n, x, OPTIM_MIN, f, gradf, &shift);
  printf("shift = %e\n", shift);
  ERROR_FATAL(0 != ret, "FATAL:  optimisation_gradient_descent() != 0\n");
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x[0], 0.0, eps), "FATAL:  optimisation_gradient_descent() x[0] = %e != %e\n", x[0], shift);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x[1], 0.0, eps), "FATAL:  optimisation_gradient_descent() x[1] = %e != %e\n", x[1], shift);
  printf("Solution: %e, %e\n", x[0], x[1]);

  shift = 1;
  x[0] = x[1] = 0;
  ret = optimisation_gradient_descent(n, x, OPTIM_MIN, f, gradf, &shift);
  ERROR_FATAL(0 != ret, "FATAL:  optimisation_gradient_descents() != 0\n");
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x[0], shift, eps), "FATAL:  optimisation_gradient_descent() x[0] = %e != %e\n", x[0], shift);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x[1], shift, eps), "FATAL:  optimisation_gradient_descent() x[1] = %e != %e\n", x[1], shift);
  printf("Solution: %e, %e\n", x[0], x[1]);

  shift = 1;
  x[0] = x[1] = 0;
  ret = optimisation_gradient_descent(n, x, OPTIM_MAX, f, gradf, &shift);
  ERROR_FATAL(0 == ret, "FATAL:  optimisation_gradient_descent() == 0\n");
 
    shift = 1;
  x[0] = x[1] = 0;
  ret = optimisation_gradient_descent(n, x, OPTIM_MAX, f, invgradf, &shift);
  ERROR_FATAL(0 != ret, "FATAL:  optimisation_gradient_descents() != 0\n");
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x[0], shift, eps), "FATAL:  optimisation_gradient_descent() x[0] = %e != %e\n", x[0], shift);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(x[1], shift, eps), "FATAL:  optimisation_gradient_descent() x[1] = %e != %e\n", x[1], shift);
  printf("Solution: %e, %e\n", x[0], x[1]);

  return EXIT_SUCCESS;
}
