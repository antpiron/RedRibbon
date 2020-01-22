#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  struct rrho rrho;
  struct rrho_result res;
  size_t n = 15;
  double a[n], b[n];

  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i;// stats_norm_rand((double)i - n/2, sqrt(n));
      b[i] = i;// stats_norm_rand((double)i - n/2, sqrt(n));
    }
  
  rrho_init(&rrho, n, a, b);
  
  for (size_t i = 0 ; i < n ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	{
	  rrho_hyper(&rrho, i, j, &res);
	  printf("%f\t", res.pvalue);
	}
      printf("\n");
    }
  //  printf("FDR: %f\n", res.fdr);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_AX_B_solve() delta != C[%ld]*C[%ld] = %f\n", i, j, delta);
 
  rrho_destroy(&rrho);

  return EXIT_SUCCESS;
}
