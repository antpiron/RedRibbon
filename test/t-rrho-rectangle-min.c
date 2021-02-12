#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  const double eps = 1e-30;
  struct rrho rrho;
  struct rrho_coord coord;
  struct rrho_result res;
  size_t n = 512;
  size_t n10perc = (n * 10) / 100;
  double *a = malloc(n * sizeof(double));
  double *b = malloc(n * sizeof(double));
  size_t exp_i = n / 2 - 1;
  size_t exp_j = n / 2 - 1;
  size_t exp_n10perc = (n10perc + 1) / 2;
  size_t exp_n10perc_i =  n - exp_n10perc;
  size_t exp_n10perc_j =  n - exp_n10perc;
  int ret;


  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i; // stats_unif_std_rand();
      b[i] = i; // stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  rrho_rectangle_min(&rrho, 0, 0, n, n, n, n, RRHO_HYPER, 1, &coord);
  rrho_hyper(&rrho, coord.i, coord.j, &res);
  
  ERROR_UNDEF_FATAL_FMT(exp_i != coord.i || exp_j != coord.j, 
  			"FAIL: rrho_rectangle_min(0,0,%zu,%zu) coord = (%zu,%zu) != (%zu, %zu)\n",
			n, n, coord.i, coord.j, exp_i, exp_j);
  ERROR_UNDEF_FATAL_FMT(res.pvalue >= eps, 
  			"FAIL: rrho_rectangle_min(0,0,%zu,%zu) pvalue = %Le >= %e\n",
			n, n, res.pvalue, eps);

  size_t n_i = 2;
  size_t n_j = 2; 
  rrho_rectangle_min(&rrho, 0, 0, n, n, n/n_i, n/n_j, RRHO_HYPER, 1, &coord);
  rrho_hyper(&rrho, coord.i, coord.j, &res);
  
  ERROR_UNDEF_FATAL_FMT( labs(exp_i - coord.i) >= n_i || labs(exp_j - coord.j) >= n_j, 
  			"FAIL: rrho_rectangle_min(0, 0, %zu, %zu, %zu, %zu) coord = (%zu,%zu) != (%zu, %zu)\n",
			 n, n, n_i, n_j, coord.i, coord.j, exp_i, exp_j);
  ERROR_UNDEF_FATAL_FMT(res.pvalue >= eps, 
  			"FAIL: rrho_rectangle_min(0, 0, %zu, %zu, %zu, %zu) pvalue = %Le >= %e\n",
			n, n, n_i, n_j, res.pvalue, eps);

  
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i; // stats_unif_std_rand();
      b[i] = (i + n - n10perc) % n; // stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  rrho_rectangle_min(&rrho, 0, 0, n, n, n, n, RRHO_HYPER, 1, &coord);
  rrho_hyper(&rrho, coord.i, coord.j, &res);


  ERROR_UNDEF_FATAL_FMT(labs(exp_i - coord.i) >= exp_n10perc_i  || labs(exp_j - coord.j) >= exp_n10perc_j, 
  			"FAIL: rrho_rectangle_min(0,0,%zu,%zu) coord = (%zu,%zu) != (%zu, %zu)\n",
			n, n, coord.i, coord.j, exp_i, exp_j);
  ERROR_UNDEF_FATAL_FMT(res.pvalue >= eps, 
  			"FAIL: rrho_rectangle_min(0,0,%zu,%zu) pvalue = %Le >= %e\n",
			n, n, res.pvalue, eps);

  rrho_destroy(&rrho);


  // check imbalance
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i; // stats_unif_std_rand();
      b[i] = (0 == i % 2) ? i : n/2 * (1 + stats_unif_std_rand()); // stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  rrho_rectangle_min(&rrho, 0, 0, n, n, n, n, RRHO_HYPER, 1, &coord);
  rrho_hyper(&rrho, coord.i, coord.j, &res);

  ERROR_UNDEF_FATAL_FMT(coord.i <= coord.j + n10perc, 
  			"FAIL: b[i] alternated - rrho_rectangle_min(0,0,%zu,%zu) coord = (%zu <= %zu)\n",
			n, n, coord.i, coord.j);
  ERROR_UNDEF_FATAL_FMT(res.pvalue >= eps, 
  			"FAIL: rrho_rectangle_min(0,0,%zu,%zu) pvalue = %Le >= %e\n",
			n, n, res.pvalue, eps);

  rrho_destroy(&rrho);

  // check reversed direction
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i; // stats_unif_std_rand();
      b[i] = n - i - 1; // stats_unif_std_rand();
    }
  
  rrho_init(&rrho, n, a, b);
  
  ret = rrho_rectangle_min(&rrho, 0, 0, n, n, n, n, RRHO_HYPER_TWO_TAILED, -1, &coord);
  ERROR_UNDEF_FATAL_FMT(0 != ret, 
  			"FAIL: inverted rrho_rectangle_min(0,0,%zu,%zu) ret = %d\n",
			n, n, ret);
  
  rrho_destroy(&rrho);

  
  free(a);
  free(b);

  return EXIT_SUCCESS;
}
