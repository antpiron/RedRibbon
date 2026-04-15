#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/stats-normalize.h"

int
main(int argc, char *argv[argc])
{
#define N (100)
#define M (3)
  double (*mat)[M][N] = malloc(N*M * sizeof(double));
  double *beta = malloc(N * sizeof(double));
  size_t ref[M] = {0, 1, 2};
  double eps = 0.0001;
  double res, delta;
  int ret;

  for (int i = 0 ; i < M ; i++)
    (*mat)[i][0] = 2*(i + 100);

  for (int i = 0 ; i < M ; i++)
    for (int j = 1 ; j < N ; j++)
    {
      (*mat)[i][j] =  (*mat)[i][0] * (j+1)  ;
    }

  ret = stats_normalize_beta_ls(M, N, M, *mat, ref, beta, STATS_NORM_LS_MEAN);
  ERROR_FATAL_FMT(0 != ret, "FAIL: stats_normalize_beta_ls() = %d != 0\n", ret);
  
  for (int i = 0 ; i < M ; i++)
    for (int j = 0 ; j < N ; j++)
      {
	double exp =  stats_mean(N, (*mat)[i] );
	double corrected = beta[j] * (*mat)[i][j];
	double delta = fabs(corrected - exp) / exp;
	ERROR_UNDEF_FATAL_FMT(delta > eps,
			      "FAIL: stats_normalize_beta_ls() mat[%d,%d] = %f != %f = expected (delta = %f)\n",
			      i, j, corrected, exp, delta); 
      }

  free(mat);
  free(beta);

  return EXIT_SUCCESS;
}
