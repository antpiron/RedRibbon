#include <stdlib.h>
#include <string.h>
#include <ale/algebra.h>
#include <ale/vector.h>

#include "correlation.h"

VECTOR_INIT(ssize_t,ssize_t)

static void
remove_loop(size_t n, ssize_t index[n])
{
  struct vector_ssize_t stack;
  ssize_t len = 0;
  char *mark = malloc(sizeof(char) * n);

  vector_ssize_t_init(&stack);
  memset(mark, 0, sizeof(char) * n);

  for (size_t i = 0 ; i < n ; i++)
    {
      if ( 0 != mark[i] )
	continue;

      for (ssize_t j = i ; j >= 0 ; j = index[j] )
	{
	  if ( 1 == mark[j] )
	    break;

	  if ( 2 == mark[j] )
	    {
	      index[j] = -1;
	      break;
	    }
	  
	  mark[j] = 2;
	  vector_ssize_t_set(&stack, len++, j);
	}

      while ( len > 0 )
	mark[ stack.data[--len] ] = 1;
    }

  vector_ssize_t_destroy(&stack);
  free(mark);
}

int
rrho_expression_prediction(size_t m, size_t n, const double mat[m][n], ssize_t index[m], double beta[2][m])
{
  int ret = 0;
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
      struct alg_ols ols;
      
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
	if ( pvalue[j] < (0.05d / m) ) // Bonferroni corrected
	  {
	    double *loocv = ols.loocv;
	    if ( index[j] < 0 || loocv[j] < loocv_cur[j] )
	      {
		index[j] = i;
		loocv_cur[j] =  loocv[j];
		beta[0][j] = betas[0][j];
		beta[1][j] = betas[1][j];
	      }
	  }

      alg_AX_B_OLS_destroy(&ols);
    }

  remove_loop(m, index);
  
  mem_destroy(&pool);
    
  return 0;
}
