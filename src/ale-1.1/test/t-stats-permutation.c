#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/sort.h"
#include "ale/math.h"
#include <math.h>

#define EPS (0.0000000001d)


int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN], res[LEN];
  ssize_t corr[LEN];
  struct stats_permutation permutation;

  for (size_t i = 0 ; i < LEN ; i++) 
    x[i] = i;

  stats_permutation_init(&permutation, LEN, x);

  stats_permutation(&permutation, res);

  qsort_r(res, LEN, sizeof(double), 
	  sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++) 
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], i, EPS), "FAIL: res.sorted[%zu] = %f != %zu", i, res[i], i); 

  stats_permutation_destroy(&permutation);

   
  return EXIT_SUCCESS;
}
