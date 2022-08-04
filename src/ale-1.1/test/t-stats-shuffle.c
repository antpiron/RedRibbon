#define _GNU_SOURCE
#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/sort.h"
#include <math.h>

#define ORDER 16

int
main(int argc, char *argv[argc])
{
#define LEN (100)
  double x[LEN];
  size_t count;
  size_t count_min = (size_t) sqrt(LEN);

  for (size_t i = 0 ; i < LEN ; i++)
    x[i] = i;

  stats_shuffle(x, LEN, sizeof(double));
  count = 0;
  for (size_t i = 0 ; i < LEN ; i++)
    count += (x[i] == i);
  ERROR_UNDEF_FATAL_FMT(count >  count_min, "FAIL: count = %zu > %zu\n", count, count_min);

  qsort_r(x, LEN, sizeof(double),
          sort_compar_double, NULL);
  for (size_t i = 0 ; i < LEN ; i++)
    ERROR_UNDEF_FATAL_FMT(x[i] != i, "FAIL: x.sorted[%zu] = %f != %zu", i, x[i], i);

  return EXIT_SUCCESS;
}
