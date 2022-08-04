#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/stats-normalize.h"
#include "ale/algebra.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
#define N ((size_t) 20000)
#define M ((size_t) 30000)
  double (*dst)[N] = malloc(N*M * sizeof(double));
  double (*mat)[N] = malloc(N*M * sizeof(double));
  double *beta = malloc(N * sizeof(double));
  double eps = 0.0001;
  double res, delta;

  #pragma omp parallel for
  for (int i = 0 ; i < M ; i++)
    for (int j = 0 ; j < N ; j++)
    {
      mat[i][j] = 1;
    }

  for (int j = 0 ; j < N ; j++)
    beta[j] = j;

  stats_normalize_samples(M, N, dst, mat, beta);
  // print_v(N, beta);
  #pragma omp parallel for
  for (int i = 0 ; i < M ; i++)
    for (int j = 0 ; j < N ; j++)
      {
	double exp =  j;
	ERROR_UNDEF_FATAL_FMT(ale_cmp_double(exp, dst[i][j], eps) != 0,
			      "FAIL: stats_normalize_samples() dst[%d,%d] = %f != %f = expected\n",
			      i, j, dst[i][j], exp); 
      }

  free(mat);
  free(dst);
  free(beta);

  return EXIT_SUCCESS;
}
