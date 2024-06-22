#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (40000)
  double *x = malloc(sizeof(double) * LEN);
  double *y = malloc(sizeof(double) * LEN);
  double res, exp, delta;
  struct stats_t_test data;
  double eps = 0.001;

  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_norm_std_rand();
      y[i] = stats_norm_std_rand();
    }

  res = stats_t_test_paired(LEN, x, y, 0, 0, &data); 
  double m = stats_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(res < eps, "FAIL: stats_t_test() == %f < %f. Mean = %f \n",
			res, eps, m);
  for (int i = 0 ; i < LEN ; i++)
    {
      x[i] = stats_norm_rand(1, 1);
      y[i] = stats_norm_rand(2, 1);
    }

  res = stats_t_test_paired(LEN, x, y, 0, 0, &data); 
  m = stats_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(res > eps, "FAIL: stats_t_test() == %f < %f. Mean = %f \n",
			res, eps, m);
  
  free(x);
  free(y);
    
  return EXIT_SUCCESS;
}
