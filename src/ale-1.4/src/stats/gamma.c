#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE								\
  stats_gamma_rand_k_theta##SUFFIX(TYPE k, TYPE theta)			\
  {									\
    /* A Simple Method for Generating Gamma -- GEORGE MARSAGLIA, WAI WAN TSANG */ \
    TYPE u, x, v;							\
    TYPE d = k - 1.0 / 3.0;						\
    TYPE c = (1.0 / 3.0) / sqrt##SUFFIX(d);				\
									\
    if (k < 1)								\
      {									\
	u = stats_unif_std_rand();					\
	return stats_gamma_rand##SUFFIX(1.0 + k, theta) * pow##SUFFIX(u, 1.0 / k); \
      }									\
									\
    do									\
      {									\
	do								\
	  {								\
	    x = stats_norm_std_rand##SUFFIX();				\
	    v = 1.0 + c * x;						\
	  }								\
	while (v <= 0);							\
									\
	v = v * v * v;							\
	u = stats_unif_std_rand##SUFFIX();				\
      }									\
    while (u >= 1 - 0.0331 * x * x * x * x &&				\
	   log(u) >= 0.5 * x * x + d * (1 - v + log (v)));		\
    									\
    return theta * d * v;						\
  }									\
									\
  TYPE									\
  stats_gamma_rand##SUFFIX(TYPE alpha, TYPE beta)			\
  {									\
    return stats_gamma_rand_k_theta##SUFFIX(alpha, 1/beta);		\
  }									\
  									\
  TYPE									\
  stats_gamma_F##SUFFIX(TYPE x, TYPE alpha, TYPE beta)			\
  {									\
    return ale_rilgamma##SUFFIX(beta * x, alpha);			\
  }									\
									\
  /* https://ocw.mit.edu/courses/mathematics/18-443-statistics-for-applications-spring-2015/lecture-notes/MIT18_443S15_LEC3.pdf */ \
  void									\
  stats_gamma_fit_mm##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    TYPE m = stats_mean##SUFFIX(n, x);					\
    TYPE v = stats_var##SUFFIX(n, x);					\
    									\
    *beta = m/v;							\
    *alpha =  m * *beta;						\
  }									\
  									\
  /* https://en.wikipedia.org/wiki/Likelihood_function#Example:_the_gamma_distribution */ \
   /*  https://tminka.github.io/papers/minka-gamma.pdf */		\
  /* not tested */							\
  void									\
  stats_gamma_fit_ml##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    TYPE m = stats_mean##SUFFIX(n, x);					\
    TYPE a;								\
    									\
    /* add computation of alpha */					\
    /* https://tminka.github.io/papers/minka-gamma.pdf */		\
     /* https://en.wikipedia.org/wiki/Digamma_function#Computation_and_approximation */	\
    a=1;								\
									\
    *beta = m/a;							\
    *alpha = a;								\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
