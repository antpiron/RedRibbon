#define _GNU_SOURCE
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_poisson_f##SUFFIX(long k, TYPE lambda)				\
  {									\
    if (k < 0)								\
      return 0.0;							\
									\
    return exp##SUFFIX(-ale_lgamma##SUFFIX(k+1) + k *			\
		       log##SUFFIX(lambda) - lambda);			\
  }									\
									\
  TYPE									\
  stats_poisson_F##SUFFIX(long k, TYPE lambda)				\
  {									\
    long mode = (long) floor##SUFFIX(lambda);				\
    TYPE sum = 0;							\
									\
    if (k <= mode)							\
      {									\
	TYPE pk = stats_poisson_f##SUFFIX(k, lambda);			\
	sum = pk;							\
	for (long i = k ; i > 0 && pk > STATS_EPS##SUFFIX ; i--)	\
	  {								\
	    pk *= i / lambda;						\
	    sum += pk;							\
	  }								\
      }									\
    else								\
      {									\
	TYPE pk = stats_poisson_f##SUFFIX(k+1, lambda);			\
	sum = pk;							\
	for (long i = k+1 ; pk > STATS_EPS##SUFFIX ; i++)		\
	  {								\
	    pk *= lambda / (TYPE)(i + 1);				\
	    sum += pk;							\
	  }								\
	sum = 1 - sum;							\
      }									\
									\
    return sum;								\
  }									\
									\
  /* Inverse transform sampling */					\
  /* Ok for small lambda */						\
  /* O(lambda) */							\
  inline								\
  unsigned long								\
  stats_poisson_rand_its##SUFFIX(TYPE lambda)				\
  {									\
    unsigned long res = 0;						\
    TYPE p = exp##SUFFIX(-lambda);					\
    TYPE sump = p;							\
    TYPE unif = stats_unif_std_rand##SUFFIX();				\
									\
    while (unif > sump)							\
      {									\
	res++;								\
	p = p * lambda / res;						\
	sump += p;							\
      }									\
									\
    return res;								\
  }									\
									\
  /* From "The Computer Generation of Poisson Random Variables" -- A. C. Atkinson */ \
  /* and for constant c https://www.johndcook.com/blog/2010/06/14/generating-poisson-random-values/ */ \
  inline								\
  unsigned long								\
  stats_poisson_rand_pa##SUFFIX(TYPE lambda)				\
  {									\
    const TYPE c = 0.767 - 3.36 / lambda;				\
    const TYPE beta = M_PI / sqrt##SUFFIX(3 * lambda);			\
    const TYPE alpha = beta * lambda;					\
    const TYPE k = log##SUFFIX(c) - lambda - log##SUFFIX(beta);		\
    const TYPE llambda = log##SUFFIX(lambda);				\
									\
    while (1)								\
      {									\
	/* generate logistic variate */					\
	TYPE u1 = stats_unif_std_rand##SUFFIX();			\
	TYPE x = (alpha - log( (1 - u1) / u1 ) ) / beta;		\
									\
	if ( x < -0.5 )							\
	  continue;							\
									\
	TYPE n = x + 0.5;						\
	TYPE u2 = stats_unif_std_rand##SUFFIX();			\
									\
	TYPE alpha_beta_x = alpha - beta * x;				\
	TYPE sq =  1 + exp##SUFFIX(alpha_beta_x);			\
	sq *= sq;							\
									\
	if ( alpha_beta_x + log##SUFFIX( u2 / sq ) <=			\
	     k + n * llambda - ale_lgamma##SUFFIX(n+1) )		\
	  return n;							\
      }									\
    /* never here */							\
    return 0;								\
  }									\
									\
  unsigned long								\
  stats_poisson_rand##SUFFIX(TYPE lambda)				\
  {									\
    if (lambda > 30)							\
      return stats_poisson_rand_pa##SUFFIX(lambda);			\
									\
    return stats_poisson_rand_its##SUFFIX(lambda);			\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
