#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  struct stats_ecdf ecdf;
#define LEN (200000)
  double x[LEN] = {0.0};
  double eps = 0.01, res;
  double count = 0;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_norm_std_rand();

  stats_ecdf_init(&ecdf, LEN, x);

  for (int i = 0 ; i < LEN ; i++)
    {
      res = x[i] = stats_ecdf_rand(&ecdf);
      if (res < 0)
	count++;
    }

  res = count / LEN;
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(0.5, res, eps), "FAIL: count(stats_ecdf_rand()) == %f != 0.5\n", res);
  res = stats_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(0, res, eps), "FAIL: mean(stats_ecdf_rand()) == %f != 0.0\n", res);
  res = stats_sd(LEN, x);
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(1.0, res, eps), "FAIL: sd(stats_ecdf_rand()) == %f != 1.0\n", res);

  stats_ecdf_destroy(&ecdf);


  return EXIT_SUCCESS;
}
