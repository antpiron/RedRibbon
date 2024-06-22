#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  struct stats_ecdf ecdf;
  size_t n = 10;
  double x[n];
  double res, expected;
  double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    x[i] = i+1;

  stats_ecdf_init(&ecdf, n, x);
  ERROR_UNDEF_FATAL_FMT(ecdf.n != n,
			"FAIL: stats_ecdf_init() %zu != %zu\n", ecdf.n, n);

  res = stats_ecdf_F_inv(&ecdf, 1);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, n, eps) ,
			"FAIL: stats_ecdf_F_inv(1) = %f != %zu\n", res, n);

  res = stats_ecdf_F_inv(&ecdf, 0);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, 1.0, eps) ,
			"FAIL: stats_ecdf_F_inv(0) = %f != %d\n", res, 1.0);

  for (size_t expected = 1 ; expected < (n+1) ; expected++)
    {
      double val = expected / (double) n;
      res = stats_ecdf_F_inv(&ecdf, val);
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expected, eps),
			    "FAIL: stats_ecdf_F_inv(%f) = %f != %zu\n",
			    val, res, expected);
    }

  stats_ecdf_destroy(&ecdf);
  
  
  return EXIT_SUCCESS;
}
