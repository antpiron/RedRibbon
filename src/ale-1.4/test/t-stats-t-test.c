#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100000)
  double *x = malloc(sizeof(double) * LEN);
  double res, exp, delta;
  struct stats_t_test data;
  double eps = 0.0000001;

  for (int i = 0 ; i < LEN ; i++)
    x[i] = stats_student_rand(LEN-1);

  res = stats_t_test(LEN, x, 0, 0, &data); 
  double m = stats_mean(LEN, x);
  ERROR_UNDEF_FATAL_FMT(res < eps, "FAIL: stats_t_test() == %f < 0.5. Mean = %f \n",
			res, m);
  
  free(x);
  
  return EXIT_SUCCESS;
}
