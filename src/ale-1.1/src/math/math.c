#include <stddef.h>
#include "ale/math.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  /* Regularized Incomplete Lower gamma function */			\
  /* IG(x;a) in http://www.stat.tamu.edu/~jnewton/604/chap3.pdf */	\
  static TYPE								\
  ale_rilgamma_serie##SUFFIX(TYPE x, TYPE a)				\
  {									\
    TYPE lgamma = ale_lgamma##SUFFIX(a);				\
    TYPE ai = a;							\
    TYPE sum = 1.0 / a;							\
    TYPE delta = sum;							\
									\
    do									\
      {									\
	ai++;								\
	delta *=  x / ai;						\
	sum += delta;							\
      }									\
    while (fabs##SUFFIX(delta) >= fabs##SUFFIX(sum) * ALE_EPS);		\
									\
    return sum * exp##SUFFIX(-x + a*log##SUFFIX(x) - lgamma);		\
  }									\
									\
  TYPE									\
  ale_rilgamma##SUFFIX(TYPE x, TYPE a)					\
  {									\
    if (0.0 == x)							\
      return 0.0;							\
									\
    return ale_rilgamma_serie##SUFFIX(x, a);				\
  }									\
									\
  TYPE									\
  ale_riugamma##SUFFIX(TYPE x, TYPE a)					\
  {									\
    return 1 - ale_rilgamma##SUFFIX(x, a);				\
  }									\
									\
  TYPE									\
  ale_ibeta##SUFFIX(TYPE x, TYPE a, TYPE b)				\
  {									\
    /* 0 <= x <= 1, a,b > 0 */						\
    /* http://www.stat.tamu.edu/~jnewton/604/chap3.pdf */		\
    if (x == 0.0 || x == 1.0)						\
      return x;								\
    									\
    if ( x > (a+1.0) / (a+b+2.0) )					\
      return 1.0 - ale_ibeta##SUFFIX(1.0-x, b, a);			\
    									\
    TYPE factor = exp##SUFFIX( log##SUFFIX(x)*a +			\
			       log##SUFFIX(1.0-x)*b -			\
			       ale_lbeta##SUFFIX(a,b) ) / a;		\
    TYPE f = 1.0, c = 1.0, d = 0.0;					\
    									\
    TYPE frac_cont = 1.0;						\
    size_t m = 0, i = 0;						\
    									\
    while(1)								\
      {									\
	d = 1.0 + frac_cont * d;					\
	if (fabs##SUFFIX(d) < ALE_TINY_DOUBLE##SUFFIX)			\
	  d = ALE_TINY_DOUBLE##SUFFIX;					\
	d = 1.0 / d;							\
      									\
	c = 1.0 + frac_cont / c;					\
	if (fabs##SUFFIX(c) < ALE_TINY_DOUBLE##SUFFIX)			\
	  c = ALE_TINY_DOUBLE##SUFFIX;					\
      									\
	TYPE delta = c*d;						\
	f *= delta;							\
      									\
	if (fabs##SUFFIX(1.0 - delta) < ALE_EPS##SUFFIX)		\
	  break;							\
      									\
	if (++i >= 5000)						\
	  break;							\
      									\
	m = i/2;							\
	frac_cont = (i % 2)?-((a+m)*(a+b+m)*x)/((a+2.0*m)*(a+2.0*m+1)):	\
	  (m*(b-m)*x)/((a+2.0*m-1.0)*(a+2.0*m));			\
      }									\
    									\
    return factor * (f - 1.0);						\
  }									\
  									\
  TYPE									\
  ale_digamma##SUFFIX(TYPE x)						\
  {									\
    TYPE sum = 0;							\
    TYPE num[]   = { -1,   1,  -1,   1,  -1,   691, -1};		\
    TYPE denom[] = { 12, 120, 252, 240, 132, 32760, 12};		\
    									\
    for ( ; x < 10 ; x++)						\
      sum -= 1 / x;							\
    									\
    sum += log##SUFFIX(x) - 0.5##SUFFIX / x;				\
    									\
    const TYPE x2 = 1.0L / (x*x);					\
    TYPE fact = x2;							\
    for (size_t i = 0 ; i < 7 ; i++, fact *= x2)			\
       {								\
	 sum += num[i] / denom[i] * fact;				\
       }								\
       									\
    return sum;								\
  }									\
  									\
  TYPE									\
  ale_trigamma##SUFFIX(TYPE x)						\
  {									\
    TYPE sum = 0;							\
    TYPE num[]   = { 1, -1,  1, -1,  5, -691, 7};			\
    TYPE denom[] = { 6, 30, 42, 30, 66, 2730, 6};			\
    									\
    for ( ; x < 10 ; x++)						\
      sum += 1 / (x*x);							\
    									\
    const TYPE x2 = 1.0L / (x*x);					\
    sum += 1.0 / x + 0.5 * x2 ;						\
    									\
    TYPE fact = x2 / x;							\
    for (size_t i = 0 ; i < 7 ; i++, fact *= x2)			\
       {								\
	 sum += num[i] / denom[i] * fact;				\
       }								\
       									\
    return sum;								\
  }


  

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
