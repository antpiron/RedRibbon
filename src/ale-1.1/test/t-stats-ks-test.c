#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

double
cdf(double x, void *cls)
{
  (void)(cls);
  return stats_norm_F(x, 2, 1);
}

int
main(int argc, char *argv[argc])
{
#define LEN (2500)
  double *x = malloc(sizeof(double) * LEN);
  double pval, stat;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_norm_rand(2, 1);

  stats_ks_test(LEN, x, cdf, NULL, &pval, &stat); 
  printf("%.8f %.8f\n", pval, stat);
  ERROR_UNDEF_FATAL_FMT(pval <= 0.01, "FAIL: norm_rand stats_ks_test() pval = %.4f <= 0.01\n",
			pval);

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_unif_std_rand();

  stats_ks_test(LEN, x, cdf, NULL, &pval, &stat); 
  printf("%.8f %.8f\n", pval, stat);
  ERROR_UNDEF_FATAL_FMT(pval > 0.01, "FAIL: unif_rand stats_ks_test() pval = %.4f > 0.01\n",
			pval);

  
  free(x);
  
  return EXIT_SUCCESS;
}
