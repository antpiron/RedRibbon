#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  size_t								\
  stats_categorical_rand##SUFFIX(size_t n, TYPE cumul_p[n])		\
  {									\
    TYPE u = stats_unif_std_rand##SUFFIX();				\
    ssize_t min = 0;							\
    ssize_t max = n-1;							\
    const ssize_t iter = 32;						\
									\
    while (min < max - iter)						\
      {									\
	ssize_t mid = (min + max) / 2;					\
									\
	if (cumul_p[mid] <= u)						\
	  min = mid;							\
	else /* if (cumul_p[mid] > u) */				\
	  max = mid;							\
      }									\
									\
    for ( ; cumul_p[min] <= u ; min++) ;				\
									\
    return min;								\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
