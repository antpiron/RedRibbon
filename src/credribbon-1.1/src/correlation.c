#include "cRedRibbon.h"
#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <ale/algebra.h>
#include <ale/vector.h>
#include <ale/error.h>
#include <ale/bitset.h>
#include <ale/stats.h>
#include <ale/sort.h>


VECTOR_INIT(ssize_t,ssize_t)

/* static void
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
  } */

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
rrho_expression_prediction_col_major(size_t m, size_t n, double mat[n][m], ssize_t nbr_tested,
				     ssize_t index[m], double beta[2][m], double r[m])
{
  struct mem_pool pool;
  mem_init(&pool);
  
  double *loocv_cur = mem_malloc(&pool, sizeof(double) * m );
  size_t *permutation = mem_malloc(&pool, sizeof(size_t) * m );
  double (*Y)[m] = mat;

  if ( nbr_tested > (ssize_t) m || nbr_tested < 0)
    nbr_tested = m;
  
  shuffle_n_size_t(m, permutation);

  for (size_t i = 0 ; i < m ; i++)
    index[i] = -1;

  for ( size_t i = 0 ; i < m ; i++ )
    loocv_cur[i] = DBL_MAX;

  for (size_t i = 0 ; i < 2 ; i++)
    for (size_t j = 0 ; j < m ; j++)
      beta[i][j] = 0;

  // alg_transpose(m, n, mat, Y);

#pragma omp parallel 
  {
    int ret = 0;
    struct mem_pool pool2;
    mem_init(&pool2);
    double (*D)[2] = mem_malloc(&pool2, sizeof(double) * n * 2);
    double (*betas)[m] = mem_malloc(&pool2, sizeof(double) * m * 2);
    struct bitset bs;
    bitset_init(&bs, m);

    
#pragma omp for
    for (size_t iter = 0 ; iter < (size_t) nbr_tested ; iter++)
      {
	struct alg_ols ols;
	size_t i = permutation[iter];
      
	size_t ii = i;
	ALG_INIT_M(n, 2, D, (0 == j) ? 1  : mat[i][ii] );
	// print_m(m, n, mat);
	// print_m(n, 2, D);
	
	ret = alg_AX_B_OLS_init(&ols, n, 2, m, D, Y, betas);
	if ( ret < 0)
	  {
	    // ERROR_MSG_FMT(ret < 0, "FAIL: rrho_expression_prediction(), alg_AX_B_OLS_init() ret = %d != 0\n", ret);
	    continue;
	  }
      
	ret = alg_AX_B_OLS_statistics(&ols, 1);
	if (ret < 0)
	  {
	    // ERROR_MSG_FMT(ret < 0, "FAIL: rrho_expression_prediction(), alg_AX_B_OLS_statistics() ret = %d != 0\n", ret);
	    continue;
	  }
	// ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_statistics() ret = %d\n != 0", name, ret);
      
	double *pvalue = ols.pvalue;
	double *loocv = ols.loocv;
	double *r_squared = ols.r_squared;
	// print_v(m, pvalue);
#pragma omp critical
	for ( size_t j = 0 ; j < m ; j++ )
	  {
	    if ( i != j  && pvalue[j] < ((double) 0.05 / ( nbr_tested * (m-1) ) ) ) // Bonferroni corrected m * (m-1)
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
		    r[j] = copysign(sqrt(r_squared[m]), betas[1][j]);
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

int
rrho_expression_prediction(size_t m, size_t n, double mat[m][n], ssize_t nbr_tested,
			   ssize_t index[m], double beta[2][m], double r[m])
{
  int ret = 0;
  size_t mat_size = sizeof(double) * m * n;
  double (*Y)[m] = malloc(mat_size);

  alg_transpose(m, n, mat, Y);
  ret = rrho_expression_prediction_col_major(m, n, Y, nbr_tested, index, beta, r);

  free(Y);
  return ret;
}

int
rrho_ldfit_prediction(size_t m, double half, double pval[m], size_t position[m], ssize_t index[m])
{
  struct mem_pool pool;
  mem_init(&pool);

  size_t *pval_index = mem_malloc(&pool, sizeof(size_t) * m);
  size_t *position_index = mem_malloc(&pool, sizeof(size_t) * m);
  size_t *reverse_position_index = mem_malloc(&pool, sizeof(size_t) * m);
  double *best_r = mem_malloc(&pool, sizeof(double) * m);
  struct bitset bs;
  bitset_init(&bs, m);

  sort_q_indirect(pval_index, pval, m, sizeof(double), sort_compar_double, NULL);
  sort_q_indirect(reverse_position_index, position, m, sizeof(size_t), sort_compar_ssize_t, NULL);
  for (size_t i = 0 ; i < m ; i++)
    position_index[reverse_position_index[i]] = i;

  for (size_t i = 0 ; i < m ; i++)
    best_r[i] = -1;

  for (size_t i = 0 ; i < m ; i++)
    index[i] = -1;
  
  for (size_t i = 0 ; i < m ; i++)
    {
      size_t current = pval_index[i];
      ssize_t current_position = position[current];

      // printf("current = %zu ; current_position = %zd\n", current, current_position);

      for (ssize_t sign = -1 ; sign <= 1 ; sign +=2)
	{
	  ssize_t last_adj = current;
	  
	  for (ssize_t j = 1 ; j < (ssize_t) m ; j++)
	    {
	      ssize_t next = (ssize_t) position_index[current] + sign * j;
	      if ( next >= (ssize_t) m || next < 0)
		break;

	      size_t adj = reverse_position_index[next];
	      ssize_t position_adj = position[adj];
	      ssize_t position_last_adj = position[last_adj];
	      double distance = fabs( (double) (current_position - position_adj) );
	      double distance_adj = fabs( (double) (position_last_adj - position_adj) );
	      double r = half / (half + distance);
	      double r_adj = half / (half + distance_adj);
	      if ( r > 0.2 && r_adj > best_r[adj])
		{
		  ssize_t old = index[adj];
		  index[adj] = last_adj;
		  if ( has_loop(&bs, m, index, adj) )
		    {
		      index[adj] = old;
		      continue;
		    } 
		  best_r[adj] = r_adj;
		  last_adj = adj;
		}
	      else
		break;
	    }
	}
    }
  
  bitset_destroy(&bs);
  mem_destroy(&pool);
  return 0;
}
