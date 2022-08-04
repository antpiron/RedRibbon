#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include <ale.h>
#include "cRedRibbon.h"

void
test_count(size_t n, size_t index[n])
{
  struct rrho rrho;
  struct rrho_result res;
  double a[n], b[n];
  
  for (size_t i = 0 ; i < n ; i++)
    {
      a[index[i]] = i; // stats_norm_rand(i, 1) ; // stats_unif_std_rand();
      b[index[i]] = i; //stats_norm_rand(i, 1) ; //stats_unif_std_rand();
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
	  if (i <= j)
	    {
	      ERROR_UNDEF_FATAL_FMT(res.count != i+1,
				    "FAIL: rrho_hyper(%zu, %zu) count = %zu != %zu\n",  i, j, res.count, i+1);
	    }
	  else
	     ERROR_UNDEF_FATAL_FMT(res.count != j+1,
				   "FAIL: rrho_hyper(%zu, %zu) count = %zu != %zu\n",  i, j, res.count, j+1);
	}
      printf("\n");
    }

  printf("\n");
  for (ssize_t i = n-1 ; i >= 0 ; i--)
    {
      for (ssize_t j = n-1 ; j >= 0 ; j--)
	{
	  rrho_hyper(&rrho, i, j, &res);
	  printf("%.3f,%2ld  ", res.pvalue, res.count);
	  if (i <= j)
	    {
	      ERROR_UNDEF_FATAL_FMT(res.count != i+1,
				    "FAIL: rrho_hyper(%zu, %zu) count = %zu != %zu\n",  i, j, res.count, i+1);
	    }
	  else
	    ERROR_UNDEF_FATAL_FMT(res.count != j+1,
				  "FAIL: rrho_hyper(%zu, %zu) count = %zu != %zu\n",  i, j, res.count, j+1);
	}
      printf("\n");
    }

  for (size_t l = 0 ; l < n*n ; l++)
    {
      size_t i = floor(stats_unif_rand(0, n));
      size_t j = floor(stats_unif_rand(0, n));
      rrho_hyper(&rrho, i, j, &res);
      printf("(%2zu, %2zu) = %.3f,%2ld\n", i, j, res.pvalue, res.count);
      if (i <= j)
	{
	  ERROR_UNDEF_FATAL_FMT(res.count != i+1,
				"FAIL: rrho_hyper(%zu, %zu) count = %zu != %zu\n",  i, j, res.count, i+1);
	}
      else
	ERROR_UNDEF_FATAL_FMT(res.count != j+1,
			      "FAIL: rrho_hyper(%zu, %zu) count = %zu != %zu\n",  i, j, res.count, j+1);
    }
  
  rrho_destroy(&rrho);
}

int
main(int argc, char *argv[argc])
{
  size_t n = 15;
  size_t index[n];

  for (size_t i = 0 ; i < n ; i++)
      index[i] = i;
  
  test_count(n, index);

  stats_shuffle(index, n, sizeof(size_t));
  test_count(n, index);

  return EXIT_SUCCESS;
}
