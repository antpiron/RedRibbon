#include <stdlib.h>
#include <string.h>
#include <ale/algebra.h>

#include "correlation.h"

int
rrho_expression_prediction(size_t m, size_t n, const double mat[m][n], ssize_t index[m], double beta[2][m])
{
  int ret = 0;
  struct alg_ols ols;
  struct mem_pool pool;
  mem_init(&pool);
  
  size_t mat_size = sizeof(double) * m * n;
  double (*Y)[m] = mem_malloc(&pool, mat_size);
  double (*D)[2] = mem_malloc(&pool, sizeof(double) * n * 2);
  double (*betas)[m] = mem_malloc(&pool, sizeof(double) * m * 2);
  double *loocv_cur = mem_malloc(&pool, sizeof(double) * m );

  for (size_t i = 0 ; i < m ; i++)
    index[i] = -1;
  
  for (size_t i = 0 ; i < m ; i++)
    {
      alg_transpose(m, n, mat, Y);
      
      ALG_INIT_M(n, 2, D, (0 == j) ? 1  : mat[i][j] );
      
      ret = alg_AX_B_OLS_init(&ols, 2, n, m, D, Y, betas);
      if ( ret < 0)
	continue;
      // ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_init ret = %d\n != 0", name, ret);
      
      ret = alg_AX_B_OLS_statistics(&ols, 1);
      if (ret < 0)
	continue;
      // ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_statistics() ret = %d\n != 0", name, ret);

      double *pvalue = ols.pvalue;
      for ( size_t j = 0 ; j < m ; j++ )
	if ( pvalue[j] < 0.05 )
	  {
	    double *loocv = ols.loocv;
	    if ( index[j] < 0 || loocv[j] < loocv_cur[j] )
	      {
		loocv_cur[j] =  loocv[j];
		index[j] = i;
		beta[0][j] = betas[0][j];
		beta[1][j] = betas[1][j];
	      }
	  }
    }
  
  mem_destroy(&pool);
    
  return ret;
}
