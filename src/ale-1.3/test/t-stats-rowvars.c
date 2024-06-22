#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/algebra.h"
#include "ale/math.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
  size_t m = 5, n = 10;
  double A[m][n], vars[m];
  double res, exp, delta;
  double eps = 0.000001;

  ALG_INIT_M(m, n, A, 1);
  stats_rowvars(m, n, A, vars);
  for (size_t i = 0 ; i < m ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(0, vars[i], eps), "FAIL: vars[%zu] = %f != %zu\n", i, vars[i], 0);
    }
  
  
  return EXIT_SUCCESS;
}
