#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/sort.h"
#include "ale/math.h"
#include "ale/algebra.h"
#include <math.h>

#define ORDER 16
#define EPS (0.0000000001d)

double
predict_1(size_t i, double x, void *cls)
{
  return -1;
}

double
predict_2(size_t i, double x, void *cls)
{
  return x * 2;
}


int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN], res[LEN];
  ssize_t deps[LEN];
  int ret;
  struct stats_permutation permutation;

  for (size_t i = 0 ; i < LEN ; i++) 
    x[i] = i;

  stats_permutation_init(&permutation, LEN, x);
  // ERROR_FATAL(0 != ret, "FAIL: stats_permutation_init() != 0\n");
  
  for (size_t i = 0 ; i < LEN ; i++) 
    deps[i] = -1;


  stats_permutation_dependencies(&permutation, deps, predict_1, NULL);

  stats_permutation(&permutation, res);
  qsort_r(res, LEN, sizeof(double), 
          sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], i, EPS), "FAIL: 0, res.sorted[%zu] = %f != %zu\n", i, res[i], i); 

  for (size_t i = 0 ; i < 4 ; i++) 
    deps[i] = i+1;

  stats_permutation(&permutation, res);
  qsort_r(res, LEN, sizeof(double), 
          sort_compar_double, NULL);
  // print_v(LEN, res);
  for (size_t i = 0 ; i < 4 ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], -1, EPS), "FAIL: 1, res.sorted[%zu] = %f != %f\n", i, res[i], -1); 
  for (size_t i = 4 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(res[i] < 0 || res[i] >= LEN, "FAIL: 2, res.sorted[%zu] = %f\n", i, res[i]); 


  for (size_t i = 0 ; i < LEN ; i++) 
    deps[i] = (0 == (i % 2) ) ? -1 : i-1;

  stats_permutation(&permutation, res);
  for (size_t i = 1 ; i < LEN ; i += 2) 
    ERROR_UNDEF_FATAL_FMT(-1 != res[i], "FAIL: 3, res[%zu] = %f != -1\n", i, res[i]); 

  stats_permutation_destroy(&permutation);

  // =================================
  stats_permutation_init(&permutation, LEN, x);
  
  for (size_t i = 0 ; i < LEN ; i++) 
    deps[i] = (0 == (i % 2) ) ? -1 : i-1;


  stats_permutation_dependencies(&permutation, deps, predict_2, NULL);

  stats_permutation(&permutation, res);
  for (size_t i = 1 ; i < LEN ; i += 2) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(2*res[i-1], res[i], EPS), "FAIL: 4, res[%zu] = %f != 2*%f\n", i, res[i], res[i-1]); 

  stats_permutation_destroy(&permutation);
  
   
  return EXIT_SUCCESS;
}
