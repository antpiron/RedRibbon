#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"


int
main(int argc, char *argv[argc])
{
#define LEN (1000)
#define VAR (5)
  double x[VAR][LEN] = {0};
  double res[VAR][VAR];
  double eps = 0.01, delta;

  stats_mat_cov(VAR, LEN, res, x);
  for (int i = 0 ; i < VAR ; i++)
    for (int j = 0 ; j < VAR ; j++)
      {
	delta = fabs(res[i][j]);
	ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_mat_cov() == %f != 0.0\n", res[i][j]);
      }

  for (int i = 0 ; i < VAR ; i++)
    for (int j = 0 ; j < LEN ; j++)
      x[i][j] = (j < LEN/2)?-1:1;
  stats_mat_cov(VAR, LEN, res, x);
  for (int i = 0 ; i < VAR ; i++)
    for (int j = 0 ; j < VAR ; j++)
      {
	delta = fabs(1.0 - res[i][j]);
	ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: stats_mat_cov() == %f != 0.0\n", res[i][j]);
      }

  
  
  return EXIT_SUCCESS;
}
