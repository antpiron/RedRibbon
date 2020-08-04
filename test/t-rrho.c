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
      a[i] = stats_unif_std_rand();
      b[i] = stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  printf("\na = ");
  for (size_t i = 0 ; i < n ; i++)
    printf("%f\t", rrho.a[i]);
  printf("\nb = ");
  for (size_t i = 0 ; i < n ; i++)
    printf("%f\t", rrho.b[i]);
  printf("\nindex_a = ");
  for (size_t i = 0 ; i < n ; i++)
    printf("%ld\t", rrho.index_a[i]);
  printf("\nindex_b = ");
  for (size_t i = 0 ; i < n ; i++)
    printf("%ld\t", rrho.index_b[i]);
  printf("\n\n");
  
  for (size_t i = 0 ; i < n ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	{
	  rrho_hyper(&rrho, i, j, &res);
	  printf("%.3f,%2ld  ", res.pvalue, res.count);
	}
      printf("\n");
    }
  //  printf("FDR: %f\n", res.fdr);
  // ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_AX_B_solve() delta != C[%ld]*C[%ld] = %f\n", i, j, delta);

  // TODO: check 0 <= p-value <= 1
  
  rrho_destroy(&rrho);

  return EXIT_SUCCESS;
}
