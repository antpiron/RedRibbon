#include "ale/math.h"
#include "ale/stats.h"

#define GENERIC_FUNC(SUFFIX,TYPE)                                       \
  TYPE                                                                  \
  stats_F_F##SUFFIX(TYPE x, TYPE d1, TYPE d2)				\
  {                                                                     \
    TYPE h1 = d1 / 2, h2 = d2 / 2;					\
    TYPE d1x = d1 * x;							\
    TYPE px = d1x / (d1x + d2);						\
    /* already regularized */						\
    return ale_ibeta##SUFFIX(px, h1, h2);				\
  }                                                                     \

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
