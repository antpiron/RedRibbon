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
  double A[m][n], means[n];
  double res, exp, delta;
  double eps = 0.000001;

  ALG_INIT_M(m, n, A, j);
  stats_colmeans(m, n, A, means);
  for (size_t i = 0 ; i < n ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(i, means[i], eps), "FAIL: means[%zu] = %f != %zu\n", i, means[i], i);
    }
  
  
  return EXIT_SUCCESS;
}
