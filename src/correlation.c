#include <stdlib.h>
#include <string.h>
#include <ale/algebra.h>

#include "correlation.h"

int
rrho_expression_prediction(size_t m, size_t n, const double mat[m][n], ssize_t index[m], double beta[m][2])
{
  int ret = 0;
  size_t mat_size = sizeof(double) * m * n;
  double (*Y)[m] = malloc(mat_size);
  double (*D)[2] = malloc(sizeof(double) * n * 2);
  double (*betas)[m] = malloc(sizeof(double) * m * 2);

  for (size_t i = 0 ; i < m ; i++)
    {
      alg_transpose(m, n, mat, Y);

      for (size_t j = 0 ; j < n ; j++)
	{
	  D[j][0] = 1;
	  D[j][1] = mat[i][j] ;
	}
      
      ret = alg_AX_B_OLS_solve(2, n, m, D, Y, betas);
    }


  free(Y);
  free(D);
  free(beta);

  return ret;
}
