#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/error.h"
#include "ale/portability.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_unif_std_rand##SUFFIX()						\
  {									\
    /* generate (0,1) and not [0,1] */					\
    uint64_t a;								\
    TYPE r;								\
									\
    do									\
      {									\
	ERROR_ERRNO_MSG(-1 == portability_getrandom(&a, sizeof(a), 0),	\
			"stats_unif_rand_std(): failed");		\
	r = (TYPE) a / (TYPE) UINT64_MAX;				\
      }									\
    while ( r <= 0.0 || r >= 1.0 );					\
									\
    return r;								\
  }									\
  									\
  TYPE									\
  stats_unif_rand##SUFFIX(TYPE min, TYPE max)				\
  {									\
    return min + (max - min) * stats_unif_std_rand##SUFFIX();		\
  }									\
  									\
  TYPE									\
  stats_unif_std_F##SUFFIX(TYPE x)					\
  {									\
    if (x <= 0)								\
      return 0;								\
    if (x >= 1)								\
      return 1;								\
									\
    return x;								\
  }									\
  									\
  TYPE									\
  stats_unif_F##SUFFIX(TYPE x, TYPE min, TYPE max)			\
  {									\
    if (x <= min)							\
      return 0;								\
    if (x >= max)							\
      return 1;								\
									\
    return (x - min) / (max - min);					\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
