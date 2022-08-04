#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/math.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define LEN (1000000)
  double *x = malloc(sizeof(double)*LEN);
  double eps = 0.01, res;
  double count = 0;
  double alpha = 10, beta = 4;
  double ralpha, rbeta;
  int ret;

  for (int iter = 0 ; iter < 4 ; iter++)
    {
      alpha = stats_unif_std_rand() * 100;
      beta = stats_unif_std_rand() * 100;

      printf("stats_beta_fit_mle(%f, %f)\n", alpha, beta);
      
      for (int i = 0 ; i < LEN ; i++)
	res = x[i] = stats_beta_rand(alpha, beta);
      
      ret = stats_beta_fit_mle(LEN, x, &ralpha, &rbeta);
      ERROR_FATAL_FMT(0 != ret, "FAIL: stats_beta_fit_mle(%f, %f), alpha = %f, beta = %f\n", alpha, beta, ralpha, rbeta);
      printf("stats_beta_fit_mle(%f, %f) => alpha = %f, beta = %f\n", alpha, beta, ralpha, rbeta);
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(ralpha, alpha, eps),
			    "FAIL: stats_beta_fit_mle(%f, %f), alpha == %f != %f\n",
			    alpha, beta, ralpha, alpha);
      ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(rbeta, beta, eps),
			    "FAIL: stats_beta_fit_mle(%f, %f), beta == %f != %f\n",
			    alpha, beta, rbeta, beta);
     }
  free(x);

  return EXIT_SUCCESS;
}
