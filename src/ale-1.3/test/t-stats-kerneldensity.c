#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
#define LEN (100000)
  struct stats_kd kd;
  double *x = malloc(LEN * sizeof(double));
  double res, expected;
  double eps = 0.01;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_norm_std_rand();


  stats_kd_init(&kd, LEN, x);
  ERROR_UNDEF_FATAL_FMT(kd.n != LEN, "FAIL: stats_kd_init() n == %zu != %zu\n", kd.n, LEN);
  ERROR_UNDEF_FATAL(kd.x != x, "FAIL: stats_kd_init() pointer to data not set\n");

  res = stats_kd_f(&kd, 0);
  expected = stats_norm_std_f(0);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res, expected, eps), "FAIL: stats_kd_f(0) == %f != %f\n", res, expected);

  stats_kd_destroy(&kd);

  free(x);

  
  
  return EXIT_SUCCESS;
}
