#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  const double eps = 1e-10;
  struct rrho rrho;
  struct rrho_result res;
  size_t n = 1024;
  double *a = malloc(n * sizeof(double));
  double *b = malloc(n * sizeof(double));
  size_t count;
  struct bitset bs_res;

  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = i; //stats_unif_std_rand();
    }

  bitset_init(&bs_res, n);
    
  rrho_init(&rrho, n, a, b);
  
  rrho_intersect(&rrho, 9, 9, RRHO_DOWN_DOWN, &bs_res);
  count = bitset_ones(&bs_res);
  ERROR_UNDEF_FATAL_FMT(10 != count,
			"FAIL: rrho_intersect(10,10, DOWN_DOWN) = %zu != 10\n", count);

  bitset_reset(&bs_res);
  rrho_intersect(&rrho, n - 10, n - 10, RRHO_UP_UP, &bs_res);
  count = bitset_ones(&bs_res);
  ERROR_UNDEF_FATAL_FMT(10 != count,
			"FAIL: rrho_intersect(10,10, UP_UP) = %zu != 10\n", count);
  
   
  rrho_destroy(&rrho);
  bitset_destroy(&bs_res);
 
  
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
