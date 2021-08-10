#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <ale/algebra.h>
#include <ale/vector.h>
#include <ale/error.h>
#include <ale/bitset.h>


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
	      // printf("Remove loop for %zu\n", j);
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

static int
has_loop(struct bitset *bs, size_t n, ssize_t index[n], ssize_t i)
{
  bitset_reset(bs);
  
  for ( ; i >= 0 ; i = index[i]  )
    {
      if ( bitset_isset(bs, i) )
	return 1;

      bitset_set(bs, i);
    }

  return 0;
}

int
rrho_expression_prediction(size_t m, size_t n, const double mat[m][n], ssize_t index[m], double beta[2][m])
{
  int ret = 0;
  struct mem_pool pool;
  mem_init(&pool);
  
  size_t mat_size = sizeof(double) * m * n;
  double *loocv_cur = mem_malloc(&pool, sizeof(double) * m );
 
 
  for (size_t i = 0 ; i < m ; i++)
    index[i] = -1;

  for ( size_t i = 0 ; i < m ; i++ )
    loocv_cur[i] = DBL_MAX;

#pragma omp parallel 
  {
    int ret = 0;
    struct mem_pool pool2;
    mem_init(&pool2);
    double (*Y)[m] = mem_malloc(&pool2, mat_size);
    double (*D)[2] = mem_malloc(&pool2, sizeof(double) * n * 2);
    double (*betas)[m] = mem_malloc(&pool2, sizeof(double) * m * 2);
    struct bitset bs;
    bitset_init(&bs, m);

#pragma omp for
    for (size_t i = 0 ; i < m ; i++)
      {
	struct alg_ols ols;
      
	alg_transpose(m, n, mat, Y);
	
	size_t ii = i;
	ALG_INIT_M(n, 2, D, (0 == j) ? 1  : mat[ii][i] );
	// print_m(m, n, mat);
	// print_m(n, 2, D);
	
	ret = alg_AX_B_OLS_init(&ols, n, 2, m, D, Y, betas);
	if ( ret < 0)
	  {
	    ERROR_MSG_FMT(ret < 0, "FAIL: rrho_expression_prediction(), alg_AX_B_OLS_init() ret = %d != 0\n", ret);
	    continue;
	  }
      
	ret = alg_AX_B_OLS_statistics(&ols, 1);
	if (ret < 0)
	  {
	    ERROR_MSG_FMT(ret < 0, "FAIL: rrho_expression_prediction(), alg_AX_B_OLS_statistics() ret = %d != 0\n", ret);
	    continue;
	  }
	// ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_statistics() ret = %d\n != 0", name, ret);
      
	double *pvalue = ols.pvalue;
	double *loocv = ols.loocv;
	// print_v(m, pvalue);
#pragma omp critical
	for ( size_t j = 0 ; j < m ; j++ )
	  {
	    if ( i != j  && pvalue[j] < (0.05d / ( m * (m-1) ) ) ) // Bonferroni corrected m * (m-1)
	      {
		if ( index[j] < 0 || loocv[j] < loocv_cur[j] )
		  {
		    ssize_t old = index[j];
		    
		    index[j] = i;
		    if ( has_loop(&bs, m, index, j) )
		      {
			index[j] = old;
			continue;
		      } 
		  
		    loocv_cur[j] =  loocv[j];
		    beta[0][j] = betas[0][j];
		    beta[1][j] = betas[1][j];
		  }
	      }
	  } 
	
	alg_AX_B_OLS_destroy(&ols);
      }

    bitset_destroy(&bs);
    mem_destroy(&pool2);
  }

  // remove_loop(m, index);

   mem_destroy(&pool);
    
  return 0;
}
