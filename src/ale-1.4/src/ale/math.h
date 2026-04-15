#ifndef __ALE_MATH_H
#define __ALE_MATH_H

#include <math.h>
#include <float.h>

#define ALE_EPS (DBL_EPSILON * 4)
#define ALE_EPSl (LDBL_EPSILON * 4)

#define ALE_TINY_DOUBLE (DBL_EPSILON * 4)
#define ALE_TINY_DOUBLEl (LDBL_EPSILON * 4)


#define MATH_GENERIC_HEADERS(SUFFIX,TYPE)				\
  static inline TYPE ale_beta##SUFFIX(TYPE a, TYPE b);			\
  static inline TYPE ale_lbeta##SUFFIX(TYPE a, TYPE b);			\
  static inline TYPE ale_lgamma##SUFFIX(TYPE x);			\
  static inline TYPE ale_gamma##SUFFIX(TYPE x);				\
  TYPE ale_rilgamma##SUFFIX(TYPE x, TYPE a);				\
  TYPE ale_riugamma##SUFFIX(TYPE x, TYPE a);				\
  TYPE ale_ibeta##SUFFIX(TYPE x, TYPE a, TYPE b);			\
  TYPE  ale_digamma##SUFFIX(TYPE x);					\
  TYPE  ale_trigamma##SUFFIX(TYPE x);					\
  static inline int ale_cmp_double##SUFFIX(TYPE a, TYPE b, TYPE eps);


MATH_GENERIC_HEADERS(,double)
MATH_GENERIC_HEADERS(l,long double)


// Inline body
#define MATH_GENERIC_FUNC(SUFFIX,TYPE)					\
  static inline								\
  int									\
  ale_cmp_double##SUFFIX(TYPE a, TYPE b, TYPE eps)			\
  {									\
    if ( isinf(a) && isinf(b) )						\
      return isinf(a) - isinf(b);					\
    if ( isinf(a) )							\
      return 1;								\
    if ( isinf(b) )							\
      return -1;							\
									\
    TYPE diff = a - b;							\
    TYPE max = (fabs##SUFFIX(a) >= fabs##SUFFIX(b)) ?			\
      fabs##SUFFIX(a) :							\
      fabs##SUFFIX(b);							\
    									\
    if ( max < 1)							\
      max = 1;								\
									\
    if ( fabs##SUFFIX(diff) <= max * eps )				\
      return 0;								\
    									\
    if ( diff < 0.0 )							\
      return -1;							\
    									\
    return 1;								\
  }									\
									\
  static inline								\
  TYPE									\
  ale_gamma##SUFFIX(TYPE x)						\
  {									\
    return tgamma##SUFFIX(x);						\
  }									\
									\
  static inline								\
  TYPE									\
  ale_lgamma##SUFFIX(TYPE x)						\
  {									\
    return lgamma##SUFFIX(x);						\
  }									\
									\
  static inline								\
  TYPE									\
  ale_beta##SUFFIX(TYPE a, TYPE b)					\
  {									\
    return exp(ale_lbeta##SUFFIX(a, b));				\
  }									\
									\
  static inline								\
  TYPE									\
  ale_lbeta##SUFFIX(TYPE a, TYPE b)					\
  {									\
    return ale_lgamma##SUFFIX(a) + ale_lgamma##SUFFIX(b) -		\
      ale_lgamma##SUFFIX(a + b);					\
  }									

MATH_GENERIC_FUNC(,double)
MATH_GENERIC_FUNC(l,long double)




#endif
