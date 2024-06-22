#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/interpolation.h"

int
main(int argc, char *argv[argc])
{
  struct interpolation_linear inter;
  size_t n = 10;
  double x[n], y[n];
  double res, expected;
  double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    {
      x[i] = i+1;
      y[i] = i+1;
    }

  interpolation_init(&inter, n, y, x);
  ERROR_UNDEF_FATAL_FMT(inter.n != n, "FAIL: interpolation_init() %zu != %zu\n", inter.n, n);

  for (expected = -10 ; expected < n + 10 ; expected += 0.1)
    {
      res = interpolation_linear_f(&inter, expected, NULL, NULL);
      expected = expected;
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expected, eps),
			    "FAIL: interpolation_linear_f(%f) %f != %f\n",
			    expected, res, expected);
    }
  
  interpolation_destroy(&inter);
  
  
  return EXIT_SUCCESS;
}
