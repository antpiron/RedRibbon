#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <ale.h>
#include "cRedRibbon.h"

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
  size_t value;
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
  /* for (size_t i = 0 ; i < (bs_res.n + 63) / 64 ; i++) */
  /*   printf("%zu: %" PRIx64 " %"  PRIx64 " %"  PRIx64 "\n", i, bs_res.buf[i], rrho.bs_a.buf[i], rrho.bs_b.buf[i]); */
  ERROR_UNDEF_FATAL_FMT(10 != count,
			"FAIL: rrho_intersect(10,10, DOWN_DOWN) = %zu != 10\n", count);
  value = -1;
  for (size_t i = 0 ; i < 10 ; i++)
    {
      ERROR_UNDEF_FATAL(bitset_iterate(&bs_res, &value) < 0,
			"FAIL: bitset_iterate() == -1");
      ERROR_UNDEF_FATAL_FMT(value != i,
			    "FAIL: rrho_intersect(10,10, DOWN_DOWN) vec[%zu] = %zd != %zu\n", i, value, i);
    }

  // UP UP
  rrho_intersect(&rrho, n - 10, n - 10, RRHO_UP_UP, &bs_res);
  count = bitset_ones(&bs_res);
  /* for (size_t i = 0 ; i < (bs_res.n + 63) / 64 ; i++) */
  /*   printf("%2zu: %#018" PRIx64 " %#018"  PRIx64 " %#018"  PRIx64 "\n", i, bs_res.buf[i], rrho.bs_a.buf[i], rrho.bs_b.buf[i]); */
  ERROR_UNDEF_FATAL_FMT(10 != count,
			"FAIL: rrho_intersect(n-10,n-10, UP_UP) = %zu != 10\n", count);
  value = -1;
  for (ssize_t i = 9 ; i >= 0 ; i--)
    {
      ERROR_UNDEF_FATAL(bitset_iterate(&bs_res, &value) < 0,
			"FAIL: bitset_iterate() == -1");
      ERROR_UNDEF_FATAL_FMT(value != n - i - 1,
			    "FAIL: rrho_intersect(n-10,n-10, UP_UP) vec[%zu] = %zd != %zu\n", i, value, n-i-1);
    }

  rrho_destroy(&rrho);
  bitset_destroy(&bs_res);

  // DOWN UP
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i ; // stats_unif_std_rand();
      b[i] = n - i - 1; //stats_unif_std_rand();
    }

  bitset_init(&bs_res, n);
  rrho_init(&rrho, n, a, b);
  
  rrho_intersect(&rrho, 9, n-10, RRHO_DOWN_UP, &bs_res);
  count = bitset_ones(&bs_res);
  ERROR_UNDEF_FATAL_FMT(10 != count,
			"FAIL: rrho_intersect(10,10, DOWN_UP) = %zu != 10\n", count);
 

  // UP DOWN
  rrho_intersect(&rrho, n-10, 9, RRHO_UP_DOWN, &bs_res);
  count = bitset_ones(&bs_res);
  ERROR_UNDEF_FATAL_FMT(10 != count,
			"FAIL: rrho_intersect(10,10, UP_DOWN) = %zu != 10\n", count);
 
   
  rrho_destroy(&rrho);

  
  bitset_destroy(&bs_res);
 
 
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
