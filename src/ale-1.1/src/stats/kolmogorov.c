#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/error.h"
#include "ale/portability.h"
#include "ale/algebra.h"
#include "ale/sort.h"

/* 
   An Improved Evaluation of Kolmogorov’s Distribution. Luis Carvalho 
   https://www.jstatsoft.org/article/view/v065c03/v65c03.pdf
   https://github.com/cran/kolmim/blob/master/src/kolmim.c
   Evaluating Kolmogorov’s Distribution. George Marsaglia, Wai Wan Tsang, Jingbo Wang.
   https://www.jstatsoft.org/article/view/v008i18/kolmo.pdf
*/
#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_kolmo_F_approx##SUFFIX(TYPE d, unsigned long n)			\
  {									\
    TYPE ret = 1 - 2 *							\
      exp##SUFFIX( - (((TYPE) 2.000071) +				\
		      ((TYPE) 0.331) / sqrt##SUFFIX(n) +		\
		      ((TYPE) 1.409) / (TYPE) n) *			\
		   n * d * d );						\
    									\
    if (ret < 0)							\
      return 0;								\
    									\
    return ret;								\
  }									\
  									\
  TYPE									\
  stats_kolmo_F_carvalho##SUFFIX(TYPE d, unsigned long n)		\
  {									\
    TYPE nd = n*d;							\
    TYPE k_dbl = ceil##SUFFIX(nd);					\
    size_t k = k_dbl;							\
    TYPE h = k_dbl - nd;						\
    size_t m = 2*k - 1;							\
    TYPE *v = malloc(sizeof(TYPE) * m);					\
    TYPE *w = malloc(sizeof(TYPE) * (m - 2));				\
    TYPE *q = malloc(sizeof(TYPE) * m);					\
    TYPE qk = -1;							\
    									\
    for (size_t i = 0 ; i < m ; i++)					\
      q[i] = 0;								\
    q[k - 1] = 1.0;							\
									\
    TYPE hj = 1, lsj = 0;						\
    for (size_t j = 0 ; j < m - 1 ; j++)				\
      {									\
	hj *= h;							\
	lsj += log##SUFFIX(j+1);					\
	v[j] = exp##SUFFIX( log1p##SUFFIX(- hj) - lsj );		\
	if (j < m - 2)							\
	  w[j] = exp##SUFFIX(-lsj);					\
      }									\
    hj *= h;								\
    lsj += log##SUFFIX(m);						\
    TYPE sub = (h > 0.5) ? pow##SUFFIX(2 * h - 1, m) : 0;		\
    v[m - 1] = exp##SUFFIX( log##SUFFIX(1 - 2 * hj + sub) - lsj );	\
    									\
    for (size_t i = 1 ; i <= n ; i++)					\
      {									\
	TYPE u = q[0];							\
	TYPE s = ( (TYPE) i ) / (TYPE) n;				\
									\
	q[0] = alg_dot##SUFFIX(m, v, q) * s;				\
									\
	for (size_t j = 2 ; j < m ; j++)				\
	  {								\
	    TYPE a = u;							\
	    u = q[j - 1];						\
	    a += v[m - j] * q[m - 1] + alg_dot##SUFFIX(m - j, w, q + j - 1); \
	    q[j - 1] = a * s;						\
	  }								\
	q[m - 1] = (v[0] * q[m - 1] + u) * s;				\
      }									\
    qk = q[k - 1];							\
    									\
    free(v);								\
    free(w);								\
    free(q);								\
    									\
    return(qk);								\
  }									\
									\
  int									\
  stats_ks_test##SUFFIX(size_t n, TYPE a[n],				\
			    TYPE (*cdf)(TYPE x, void *cls), void *cls,	\
			    TYPE *pval, TYPE *stat)			\
  {									\
    size_t *index = malloc(sizeof(size_t) * n);				\
    TYPE best = -2;							\
									\
    sort_q_indirect(index, a, n, sizeof(TYPE),				\
		    sort_compar_double##SUFFIX, NULL);			\
		    							\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	TYPE stat1 = (*cdf)(a[index[i]], cls) - ((TYPE) i) / (TYPE) n;	\
	TYPE stat2 = ((TYPE) 1) / (TYPE) n - stat1;			\
									\
	if (stat2 > stat1)						\
	  stat1 = stat2;						\
									\
	if (stat1 > best)						\
	  best = stat1;							\
      }									\
									\
    if ( NULL != stat)							\
      *stat = best;							\
    *pval = 1 - stats_kolmo_F##SUFFIX(*stat, n);			\
    if (*pval < 0)							\
      *pval = 0.0;							\
    									\
    free(index);							\
									\
    return 0;								\
  }									\
  									\
  TYPE									\
  stats_kolmo_F##SUFFIX(TYPE d, unsigned long n)			\
  {									\
    if ( d <= 0.0 )							\
      return (TYPE) 0.0;						\
    if ( d >= 1.0 )							\
      return (TYPE) 1.0;						\
									\
    TYPE ndd = n * d * d;						\
    if ( ndd > 7.24 || ( n > 99 && ndd > 3.76 ) )			\
      return stats_kolmo_F_approx##SUFFIX(d, n);			\
  									\
    return  stats_kolmo_F_carvalho##SUFFIX(d, n);			\
  }
     
GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
