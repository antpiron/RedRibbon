#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

int
main(int argc, char *argv[argc])
{
#define COUNT (1000000)
#define LEN (10)
  double cumul_p[LEN];
  double exp[LEN];
  double sum = 0;
  size_t count[LEN] = {0};
  double eps = 0.005;

  for (size_t i = 0 ; i < LEN ; i++)
    {
      double u = stats_unif_std_rand();
      exp[i] = u;
      sum += u;
    }
  printf("p = ");
  for (size_t i = 0 ; i < LEN ; i++)
    {
      exp[i] /= sum;
      printf("%.3f ", exp[i]);
    }
  printf("\n");

  cumul_p[0] = exp[0];
  printf("c = %.3f ", cumul_p[0]);
  for (size_t i = 1 ; i < LEN ; i++)
    {
      cumul_p[i] = cumul_p[i-1] + exp[i];
      printf("%.3f ", cumul_p[i]);
    }
  printf("\n");
  
  
  for (size_t i = 0 ; i < COUNT ; i++)
    {
      size_t k = stats_categorical_rand(LEN, cumul_p);
      ERROR_UNDEF_FATAL_FMT(k >= LEN, "FAIL: stats_categorical_rand() = %zu >= %zu \n", k, LEN);
      count[k]++;
      // printf("\n");
    }

  
  for (size_t i = 0 ; i < LEN ; i++)
    {
      double res = (double) count[i] / (double) COUNT;
      double diff = fabs(res - exp[i]);
      ERROR_UNDEF_FATAL_FMT(diff > eps, "FAIL: stats_categorical_rand() freq[%zu] = %f != %f \n", i, res, exp[i]);
    }


  return EXIT_SUCCESS;
}
