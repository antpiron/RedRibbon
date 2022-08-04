#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <ale.h>
#include "cRedRibbon.h"

int
main(int argc, char *argv[argc])
{
  int ret;
  size_t m = 30;
  size_t *position = malloc(sizeof(size_t) * m);
  double *pval = malloc(sizeof(double) * m);
  ssize_t *index = malloc(sizeof(ssize_t) * m );
  double half = 2;


  for (size_t i = 0 ; i < m ; i++)
    {
       pval[i] = stats_unif_std_rand();
       position[i] = i;
    }

  print_v(m, pval);
 
  ret = rrho_ldfit_prediction(m, 1, pval, position, index);
  ERROR_FATAL_FMT(0 != ret,  "FAIL: rrho_ldfit_prediction() ret = %du\n", ret);

  for (size_t i = 0 ; i < m ; i++)
    printf("%zd\t", index[i]);
  printf("\n");

  size_t smallest = 0;
  for (size_t i = 0 ; i < m ; i++)
    {
      if ( pval[i] < pval[smallest] )
	smallest = i;
      
      ERROR_UNDEF_FATAL_FMT(-1 > index[i] || (ssize_t) m <= index[i], "FAIL: rrho_ldfit_prediction() index[%zu] = %zd\n", i, index[i]);

      ERROR_UNDEF_FATAL_FMT(0 < i && -1 == index[i] && i != index[i-1] ,
			    "FAIL: rrho_ldfit_prediction() index[%zu] = %zd != %zu\n", i-1, index[i-1], i);
      ERROR_UNDEF_FATAL_FMT(i < m - 1 && -1 == index[i] && i != index[i+1] ,
			    "FAIL: rrho_ldfit_prediction() index[%zu] = %zd != %zu\n", i+1, index[i+1], i);
    }

  ERROR_UNDEF_FATAL_FMT(-1 != index[smallest], "FAIL: rrho_ldfit_prediction() index[%zu] = %zd != -1\n", smallest, index[smallest]);


  free(position);
  free(pval);
  free(index);
  
  return EXIT_SUCCESS;
}
