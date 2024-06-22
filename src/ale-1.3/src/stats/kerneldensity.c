#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"
#include "ale/sort.h"
#include "ale/interpolation.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  void									\
  stats_kd_init_full##SUFFIX(struct stats_kd##SUFFIX *kd, size_t n,	\
			     TYPE x[n], TYPE h,				\
			     TYPE (*K)(TYPE),				\
			     TYPE K_eps)				\
  {									\
    kd->n= n;								\
    kd->x = x;								\
    kd->h = h;								\
    kd->K = K;								\
    kd->K_eps = K_eps;							\
    mem_init(&kd->pool);						\
									\
    kd->index = mem_malloc(&kd->pool, n * sizeof(size_t) );		\
									\
    sort_q_indirect(kd->index, x, n, sizeof(TYPE),			\
		    sort_compar_double##SUFFIX, NULL);			\
  }									\
									\
  void									\
  stats_kd_init##SUFFIX(struct stats_kd##SUFFIX *kd, size_t n,		\
			TYPE x[n])					\
  {									\
    /* https://en.wikipedia.org/wiki/Kernel_density_estimation#A_rule-of-thumb_bandwidth_estimator */ \
    TYPE IQR = stats_IQR##SUFFIX(n, x);					\
    TYPE sd = stats_sd##SUFFIX(n, x);					\
    TYPE A = fmin##SUFFIX(sd, IQR / 1.23);				\
    TYPE h = 0.9 * A * pow##SUFFIX((TYPE) n, -0.2);			\
									\
    stats_kd_init_full##SUFFIX(kd, n, x, h,				\
			       &stats_norm_std_f##SUFFIX,		\
			       0.001);					\
  }									\
									\
  int									\
  stats_kd_multi_init_full##SUFFIX(struct stats_kd##SUFFIX *kd,		\
				   size_t d, size_t n,			\
				   TYPE x[n][d], TYPE H[d][d],		\
				   int mem_pool_init)			\
  {									\
    int ret = 0;							\
    kd->n = n;								\
    kd->d = d;								\
    kd->x = &x[n][d];							\
    kd->H = &H[0][0];							\
    if (mem_pool_init)							\
      mem_init(&kd->pool);						\
									\
    kd->H_inv = mem_malloc(&kd->pool, d * d * sizeof(TYPE) );		\
    ret = alg_symmetric_definite_inv##SUFFIX(d, H,			\
					     (TYPE(*)[d]) kd->H_inv);	\
    if (0 != ret)							\
      {									\
	if (mem_pool_init)						\
	  mem_destroy(&kd->pool);					\
	ERROR_RET(1, -1);						\
      }									\
    kd->det = alg_symmetric_definite_det##SUFFIX(d, H);			\
									\
    return ret;								\
  }									\
									\
  int									\
  stats_kd_multi_init##SUFFIX(struct stats_kd##SUFFIX *kd,		\
			      size_t n,	size_t d,			\
			      TYPE x[n][d])				\
  {									\
    /* https://en.wikipedia.org/wiki/Multivariate_kernel_density_estimation#Rule_of_thumb */ \
    int ret = 0;							\
    mem_init(&kd->pool);						\
    TYPE (*H)[d] = mem_malloc(&kd->pool, d * d * sizeof(TYPE));		\
    TYPE *vars = malloc(sizeof(TYPE) * d);				\
									\
    /* TODO: test */							\
    stats_colvars##SUFFIX(n, d, x, vars);				\
    for (size_t i = 0 ; i < kd->d ; i++)				\
      for (size_t j = 0 ; j < kd->d ; j++)				\
	{								\
	  if (i == j)							\
	    {								\
	      TYPE var = vars[i];					\
	      H[i][j] = pow##SUFFIX(4.0 / (TYPE) (n * (d + 2)),		\
				    2.0 / (TYPE) (d + 4)) * var;	\
	    }								\
	  else								\
	    H[i][j] = 0;						\
	}								\
									\
    free(vars);								\
    									\
    ret = stats_kd_multi_init_full##SUFFIX(kd, n, n, x, H, 0); 		\
    if (0 != ret)							\
      mem_destroy(&kd->pool);						\
									\
    return ret;								\
  }									\
									\
  void									\
  stats_kd_destroy##SUFFIX(struct stats_kd##SUFFIX *kd)			\
  {									\
    mem_destroy(&kd->pool);						\
  }									\
  									\
  TYPE									\
  stats_kd_f##SUFFIX(struct stats_kd##SUFFIX *kd, TYPE x)		\
  {									\
    TYPE sum = 0;							\
    ssize_t below = 0, above = kd->n - 1;				\
									\
    while ( below < above - 1)						\
      {									\
	size_t half = (below + above) / 2;				\
	TYPE val = kd->x[kd->index[half]];				\
									\
	if ( x < val  )							\
	  above = half;							\
	else if (x > val)						\
	  below = half;							\
	else								\
	  { below = half ; above = half + 1;}				\
									\
      }									\
									\
    for (size_t i = above ; i < kd->n ; i++)				\
      {									\
	TYPE param_x = (x - kd->x[kd->index[i]] ) / kd->h;		\
	TYPE K_val =  kd->K(param_x);					\
									\
	sum += K_val;							\
									\
	if (fabs##SUFFIX(K_val) < kd->K_eps)				\
	  break;							\
      }									\
									\
    for (ssize_t i = below ; i >= 0 ; i--)				\
      {									\
	TYPE param_x = (x - kd->x[kd->index[i]] ) / kd->h;		\
	TYPE K_val =  kd->K(param_x);					\
									\
	sum += K_val;							\
									\
	if (fabs##SUFFIX(K_val) < kd->K_eps)				\
	  break;							\
      }									\
									\
    return 1.0 / (kd->n * kd->h) * sum;					\
  }  									\
									\
  static inline								\
  TYPE									\
  Kh_multi_norm_f##SUFFIX(struct stats_kd##SUFFIX *kd, TYPE x[kd->d])	\
  {									\
    TYPE *prod = malloc(sizeof(TYPE) * kd->d);				\
    TYPE res;								\
									\
    alg_mul_m_v##SUFFIX(kd->d, kd->d,					\
			(TYPE (*)[kd->d]) kd->H_inv, x, prod);		\
    res = pow##SUFFIX(2*M_PI, -(TYPE) kd->d / (TYPE) 2.0) *		\
      1.0 / sqrt##SUFFIX(kd->det) *					\
      exp##SUFFIX(-0.5 * alg_dot##SUFFIX(kd->d, x, prod));		\
									\
    free(prod);								\
									\
    return res;								\
  }									\
									\
  TYPE									\
  stats_kd_multi_f##SUFFIX(struct stats_kd##SUFFIX *kd, TYPE x[kd->d])	\
  {									\
    TYPE sum = 0;							\
    TYPE *param = malloc(sizeof(TYPE) * kd->d);				\
									\
     for (size_t i = 0 ; i < kd->n ; i++)				\
      {									\
	alg_sub_v_v##SUFFIX(kd->d, x, kd->H + (i * kd->d), param);	\
									\
	sum += Kh_multi_norm_f##SUFFIX(kd, param);			\
      }									\
									\
    free(param);							\
									\
    return 1.0 / kd->n * sum;						\
  }



GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
