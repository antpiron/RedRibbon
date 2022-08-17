#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/sort.h"



#define SIZE (10)

int
main(int argc, char *argv[argc])
{
  int ret;
  double tosort[SIZE];
  size_t index[SIZE];


  for (ssize_t i = 0 ; i < SIZE ; i++)
    tosort[i] = (i-SIZE/2)*(i-SIZE/2);

  sort_q_indirect(index, tosort, SIZE, sizeof(double), sort_compar_double, NULL);


  for (ssize_t i = 0 ; i < SIZE ; i++)
    ERROR_UNDEF_FATAL_FMT(index[i] >= SIZE, "FAIL: index[%zd] = %zu >= %d\n", i, index[i], SIZE);

  for (ssize_t i = 1 ; i < SIZE ; i++)
    {
      size_t ia = index[i-1];
      size_t ib = index[i];
      double a = tosort[ia];
      double b = tosort[ib];
      
      ERROR_UNDEF_FATAL_FMT(a > b,
			    "FAIL: %f > %f", a, b);
    }

  sort_q_indirect(index, tosort, SIZE, sizeof(double), sort_compar_double_decreasing, NULL);


  for (ssize_t i = 0 ; i < SIZE ; i++)
    ERROR_UNDEF_FATAL_FMT(index[i] >= SIZE, "FAIL: index[%zu] = %zu >= %d\n", i, index[i], SIZE);

  for (ssize_t i = 1 ; i < SIZE ; i++)
    {
      size_t ia = index[i-1];
      size_t ib = index[i];
      double a = tosort[ia];
      double b = tosort[ib];
      
      ERROR_UNDEF_FATAL_FMT(a < b,
			    "FAIL: %f > %f\n", a, b);
    }

  return EXIT_SUCCESS;
}
