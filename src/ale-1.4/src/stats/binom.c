#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  static inline TYPE							\
  stats_binom_lf_lgamma##SUFFIX(long k, long n, TYPE p)			\
  {									\
    return ale_lgamma##SUFFIX(n+1) - ale_lgamma##SUFFIX(k+1) - ale_lgamma##SUFFIX(n-k+1) + \
      k * log##SUFFIX(p) + (n-k) * log##SUFFIX(1-p);			\
  }									\
									\
  static inline TYPE							\
  stats_binom_f_lgamma##SUFFIX(long k, long n, TYPE p)			\
  {									\
    return exp##SUFFIX(stats_binom_lf_lgamma##SUFFIX(k, n, p));		\
  }									\
									\
									\
  /* Fast and Accurate Computation of Binomial Probabilities. Catherine Loader. */ \
  /* see https://www.r-project.org/doc/reports/CLoader-dbinom-2002.pdf */ \
  static inline TYPE							\
  stats_npd0##SUFFIX(TYPE x, TYPE np)					\
  {									\
    TYPE x_np = x - np;							\
    TYPE xnp = x + np;							\
    TYPE v = x_np / xnp;						\
    TYPE sum, prod;							\
									\
    if ( fabs##SUFFIX(x_np) < 0.1 * xnp )				\
      {									\
	TYPE vv = v*v;							\
	TYPE last_sum = -1;						\
	prod = sum = vv*v / 3.0;					\
	for (int i = 2 ; i < 1024 && last_sum != sum ; i++)		\
	  {								\
	    prod *= vv / (2*i);						\
	    last_sum = sum;						\
	    sum += prod;						\
	  }								\
									\
	return v * x_np + 2 * x * sum;					\
      }									\
									\
    return x * log##SUFFIX(x/np) + x_np;				\
  }									\
									\
  static inline TYPE							\
  stats_stirling_error##SUFFIX(TYPE n)					\
  {									\
    TYPE sq = n*n;							\
    return (1.0 / 12.0 - (1.0 / 360.0 - (1.0 / 1260.0) / sq ) / sq  ) / n; \
  }									\
									\
  static inline TYPE							\
  stats_binom_lf_saddle_point##SUFFIX(long k, long n, TYPE p)		\
  {									\
    if ( 0 == k )							\
      return n*log##SUFFIX(1-p);					\
    if ( n == k )							\
      return n*log##SUFFIX(p);						\
									\
    TYPE q = 1 - p;							\
    TYPE np = n * p;							\
    TYPE nq = n * q;							\
    TYPE d = stats_npd0##SUFFIX(k, np) + stats_npd0##SUFFIX(n-k, nq);	\
    TYPE stirling_err = stats_stirling_error##SUFFIX(n) -		\
      stats_stirling_error##SUFFIX(k) - stats_stirling_error##SUFFIX(n-k); \
    TYPE lroot = 0.5 * ( log##SUFFIX(2.0 * M_PI) + log##SUFFIX(k) +	\
			 log1p##SUFFIX(- (TYPE) k / (TYPE) n) );	\
									\
    return stirling_err - lroot - d;					\
  }									\
									\
  static inline TYPE							\
  stats_binom_f_saddle_point##SUFFIX(long k, long n, TYPE p)		\
  {									\
    return exp##SUFFIX(stats_binom_lf_saddle_point##SUFFIX(k, n, p));	\
  }									\
  									\
  TYPE									\
  stats_binom_f##SUFFIX(long k, long n, TYPE p)				\
  {									\
    if (k < 0 || k > n) return 0.0;					\
    									\
    if ( n < 30 )							\
      return stats_binom_f_lgamma##SUFFIX(k, n,  p);			\
    									\
    return stats_binom_f_saddle_point##SUFFIX(k, n,  p);		\
  }									\
  									\
  TYPE									\
  stats_binom_F##SUFFIX(long k, long n, TYPE p)				\
  {									\
    long mode = (long)(n*p);						\
    TYPE sum = 0;							\
    									\
    if (k <= mode)							\
      {									\
	TYPE cons = (1 - p) / p;					\
	TYPE pk = stats_binom_f##SUFFIX(k, n, p);			\
									\
	sum = pk;							\
	for (long i = k ; i > 0 && pk > STATS_EPS##SUFFIX ; i--)	\
	  {								\
	    pk *= (TYPE)(i*cons) / (TYPE)(n - i + 1);			\
	    sum += pk;							\
	  }								\
      }									\
    else								\
      {									\
	TYPE cons = p / (1-p);						\
	TYPE pk = stats_binom_f(k+1, n, p);				\
	sum = pk;							\
	for (long i = k+1 ; i < n && pk > STATS_EPS##SUFFIX ; i++)	\
	  {								\
	    pk *= (TYPE)((n-i)*cons) / (TYPE)(i + 1);			\
	    sum += pk;							\
	  }								\
									\
	sum = 1 - sum;							\
      }									\
    									\
    return sum;								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
