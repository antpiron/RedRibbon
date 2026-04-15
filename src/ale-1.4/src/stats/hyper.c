#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"

// TODO: stats_hyper_f_binom seems inaccurate 
/* static inline double */
/* stats_hyper_f_binom(long k, long K, long n, long N) */
/* { */
/*   double p = (double) n / (double) N; */
/*   double b1 = stats_binom_f(k, K, p); */
/*   double b2 = stats_binom_f(n - k, N - K, p); */
/*   double b3 = stats_binom_f(n, N, p); */

/*   return b1 * b2 / b3; */
/* } */

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  static inline TYPE							\
  stats_hyper_f_lgamma##SUFFIX(long k, long K, long n, long N)		\
  {									\
    TYPE cb1 = ale_lgamma##SUFFIX(K+1) -				\
      ale_lgamma##SUFFIX(k+1) - ale_lgamma##SUFFIX(K-k+1);		\
    TYPE cb2 = ale_lgamma##SUFFIX(N-K+1) -				\
      ale_lgamma##SUFFIX(n-k+1) - ale_lgamma##SUFFIX(N-K-n+k+1);	\
    TYPE cb3 = ale_lgamma##SUFFIX(N+1) -				\
      ale_lgamma##SUFFIX(n+1) - ale_lgamma##SUFFIX(N-n+1);		\
    									\
    return exp##SUFFIX(cb1 + cb2 - cb3);				\
  }									\
									\
  TYPE									\
  stats_hyper_f##SUFFIX(long k, long K, long n, long N)			\
  {									\
    long upper = (k < K)? k: K;						\
    long lower = K-N+n;							\
									\
    if (lower < 0)							\
      lower = 0;							\
									\
    if (k > upper || k < lower)						\
      return 0.0;							\
    									\
    /* TODO: stats_hyper_f_binom seems inaccurate */			\
    /* return stats_hyper_f_binom(k,K,n,N); */				\
    return stats_hyper_f_lgamma##SUFFIX(k,K,n,N);			\
  }									\
									\
  static TYPE								\
  stats_hyper_upper##SUFFIX(long k, long K, long n, long N)		\
  {									\
    long upper = (n < K)? n : K;					\
    long lower = K-N+n;							\
    TYPE res = 0, fk;							\
									\
    if ( k > upper )							\
      return 0.0;							\
									\
    if ( k <  lower)							\
      return 1.0;							\
									\
    fk = res = stats_hyper_f##SUFFIX(k, K, n, N);			\
    for (long i = k ; i <= upper ; i++)					\
      {									\
	TYPE id = (TYPE) i;						\
	fk *=  (n-id) * (K-id) /  (id+1) / (N-K-n+id+1) ;		\
	res += fk;							\
      }									\
    return res;								\
  }									\
									\
  static TYPE								\
  stats_hyper_lower##SUFFIX(long k, long K, long n, long N)		\
  {									\
    long upper = (n < K)? n : K;					\
    long lower = K-N+n;							\
    TYPE res = 0, fk;							\
									\
    if ( k > upper )							\
      return 1.0;							\
									\
    if ( k <  lower)							\
      return 0.0;							\
									\
    lower = lower < 0?0:lower;						\
									\
    fk = res = stats_hyper_f##SUFFIX(k, K, n, N);			\
    for (long i = k  ; i > lower  ; i--)				\
      {									\
	TYPE id = (TYPE) i;						\
	fk *= (id * (N-K-n+id)) / (n-id+1) / (K-id+1);			\
	res += fk;							\
      }									\
									\
    return res;								\
    }									\
  									\
  TYPE									\
  stats_hyper_tail##SUFFIX(long k, long K, long n, long N, int upper)	\
  {									\
    long mode = (n + 1) * (K + 1) / (N + 2);				\
									\
    if (STATS_UPPER == upper)						\
      {									\
	if ( k <= mode )						\
	  return 1 - stats_hyper_lower##SUFFIX(k-1, K, n, N);		\
									\
	return stats_hyper_upper##SUFFIX(k, K, n, N);			\
      }									\
									\
    if ( k <= mode )							\
      return stats_hyper_lower##SUFFIX(k, K, n, N);			\
									\
    return 1 - stats_hyper_upper##SUFFIX(k+1, K, n, N);			\
    }									\
									\
  TYPE									\
  stats_hyper_F##SUFFIX(long k, long K, long n, long N)			\
  {									\
    return stats_hyper_tail##SUFFIX(k, K, n, N, STATS_LOWER);		\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
