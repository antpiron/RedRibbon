#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include <ale.h>
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  int ret;
  size_t m = 15, n = 10000;
  double (*expr)[n] = malloc(sizeof(double) * m * n);
  ssize_t *index = malloc(sizeof(ssize_t) * m );
  double (*beta)[m] = malloc(sizeof(double) * m * 2);


  ALG_INIT_M(m, n, expr, stats_norm_std_rand() );
 
  ret = rrho_expression_prediction(m, n, expr, index, beta);
  ERROR_UNDEF_FATAL_FMT(0 != ret,  "FAIL: rrho_expression_prediction() ret = %du\n", ret);

  for (size_t i = 0 ; i < m ; i++)
    printf("%d\t", index[i]);
  printf("\n");

  int count = 0;
  for (size_t i = 0 ; i < m ; i++)
    {
      count += (-1 != index[i]);
      // ERROR_UNDEF_FATAL_FMT(-1 != index[i],  "FAIL: rrho_expression_prediction() index[%zu] = %d != -1\n", i, index[i]);
    }
  ERROR_UNDEF_FATAL_FMT(1 < count,  "FAIL: rrho_expression_prediction() count = %d > 1\n", count);

  // ====================
  ALG_INIT_M(m, n, expr, (i == m-1) ? expr[0][j] : stats_norm_std_rand() );

  ret = rrho_expression_prediction(m, n, expr, index, beta);
  ERROR_UNDEF_FATAL_FMT(0 != ret,  "FAIL: rrho_expression_prediction() ret = %du\n", ret);

  for (size_t i = 0 ; i < m ; i++)
    printf("%d\t", index[i]);
  printf("\n");

  count = 0;
  for (size_t i = 0 ; i < m-1 ; i++)
    {
      count += (-1 != index[i]);
      // ERROR_UNDEF_FATAL_FMT(-1 != index[i],  "FAIL: rrho_expression_prediction() index[%zu] = %d != -1\n", i, index[i]);
    }
  ERROR_UNDEF_FATAL_FMT(1 < count,  "FAIL: rrho_expression_prediction() count = %d > 1\n", count);
 
  ERROR_UNDEF_FATAL_FMT(0 != index[m-1] && m-1 != index[0],
			"FAIL: rrho_expression_prediction() index[%zu] = %d != 0\n", m-1, index[m-1]);
 

  // ====================
  ALG_INIT_M(m, n, expr, (i > 0 && stats_unif_std_rand() < 0.2 ) ? expr[i-1][j] + stats_norm_rand(0, 0.1) : stats_norm_std_rand() );
 
  ret = rrho_expression_prediction(m, n, expr, index, beta);
  ERROR_UNDEF_FATAL_FMT(0 != ret,  "FAIL: rrho_expression_prediction() ret = %du\n", ret);

  for (size_t i = 0 ; i < m ; i++)
    printf("%d\t", index[i]);
  printf("\n");

  count = 0;
  for (size_t i = 0 ; i < m ; i++)
    {
      count += ( index[i] == -1) ? 0 : labs(i - index[i]) - 1  ;
    }
  ERROR_UNDEF_FATAL_FMT(count > 5,  "FAIL: rrho_expression_prediction() count = %d > 5\n", count);

  free(expr);
  free(index);
  free(beta);
  
  return EXIT_SUCCESS;
}
