#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/error.h"
#include "ale/optimisation.h"
#include "ale/algebra.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  /* https://en.wikipedia.org/wiki/Beta_distribution#Generating_beta-distributed_random_variates */ \
  TYPE									\
  stats_beta_rand##SUFFIX(TYPE alpha, TYPE beta)			\
  {									\
    TYPE x = stats_gamma_rand##SUFFIX(alpha, 1);			\
    TYPE y = stats_gamma_rand##SUFFIX(beta, 1);				\
    									\
    return x / (x + y) ;						\
    }									\
  									\
  TYPE									\
  stats_beta_F##SUFFIX(TYPE x, TYPE alpha, TYPE beta)			\
  {									\
    return ale_ibeta##SUFFIX(x, alpha, beta);				\
  }									\
									\
  TYPE stats_beta_F_inv##SUFFIX(TYPE x, TYPE alpha, TYPE beta)		\
  {									\
    TYPE l = 0, r = 1;							\
    TYPE half;								\
    									\
    for (size_t i = 0 ; i < 64 && r - l > ALE_EPS##SUFFIX ; i++ )	\
      {									\
	half = (r + l) / 2;						\
									\
	TYPE beta_inv = stats_beta_F##SUFFIX(half, alpha, beta);	\
	if (beta_inv < x)						\
	  l = half;							\
	else								\
	  r = half;							\
      }									\
									\
    return half;							\
  }									\
  									\
  void									\
  stats_beta_fit_mm##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    TYPE m = stats_mean##SUFFIX(n, x);					\
    TYPE v = stats_var##SUFFIX(n, x);					\
    TYPE t1_m = 1-m;							\
    TYPE tfrac = m * t1_m / v - 1;					\
									\
    *alpha = m*tfrac;							\
    *beta =  t1_m *tfrac;						\
  }									\
  									\
  struct grad_cls##SUFFIX						\
  {									\
    size_t n;								\
    TYPE sum_ln_x, sum_ln_1x;						\
  };									\
    									\
  static void								\
  gradf##SUFFIX(TYPE *y, TYPE *x, void *cls)				\
  {									\
    struct grad_cls##SUFFIX *c = cls;					\
    if ( x[0] < 0 || x[1] < 0 )						\
      {									\
	y[0] = y[1] = 0;						\
	return;								\
      }									\
    TYPE dg = ale_digamma(x[0] + x[1]);					\
									\
    y[0] = c->sum_ln_x  - c->n * ( ale_digamma(x[0]) - dg );		\
    y[1] = c->sum_ln_1x - c->n * ( ale_digamma(x[1]) - dg );		\
  }									\
									\
    /* https://www.real-statistics.com/distribution-fitting/distribution-fitting-via-maximum-likelihood/fitting-beta-distribution-parameters-mle/ */ \
  int									\
  stats_beta_fit_mle_newton##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    TYPE grad[2], H[2][2], H1[2][2], p[2], y[2];			\
    TYPE sum_ln_x = 0 /*, sum_ln_1x = 0 */;				\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	sum_ln_x += log(x[i]);						\
	/* sum_ln_1x += log(1 - x[i]); */				\
      }									\
    									\
    stats_beta_fit_mm##SUFFIX(n, x, alpha, beta);			\
    y[0] = *alpha; y[1] = *beta;					\
									\
    for (size_t i = 0 ; i < (1ll << 12) ; i++ )				\
      {									\
	TYPE dg = ale_digamma(y[0] + y[1]);				\
	grad[0] = sum_ln_x - n * (ale_digamma(y[0]) - dg);		\
	grad[1] = sum_ln_x - n * (ale_digamma(y[1]) - dg);		\
									\
	TYPE tg = ale_trigamma(y[0] + y[1]);				\
	H[0][0] = - n * (ale_trigamma(y[0]) - tg);			\
	H[1][1] = - n * (ale_trigamma(y[1]) - tg);			\
	H[0][1] = H[1][0] = n * tg;					\
									\
	TYPE det = H[0][0] * H[1][1] - H[0][1] * H[1][0];		\
	ERROR_CUSTOM_RET(0 == det, -1, 1);				\
									\
	TYPE inv_det = 1 / det;						\
	H1[0][0] = inv_det * H[1][1];					\
	H1[1][1] = inv_det * H[0][0];					\
	H1[1][0] = -inv_det * H[1][0];					\
	H1[0][1] = -inv_det * H[0][1];					\
									\
	alg_mul_m_v##SUFFIX(2, 2, H1, grad, p);				\
	alg_add_v_v##SUFFIX(2, y, p, y);				\
	TYPE norm =  alg_norm##SUFFIX(2, grad);				\
	if ( norm <= ALE_EPS##SUFFIX )					\
	  break;							\
      }									\
    									\
    *alpha = y[0]; *beta = y[1];					\
									\
    return 0;								\
  }									\
									\
  int									\
  stats_beta_fit_mle##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    int ret = 0;							\
    struct grad_cls##SUFFIX cls = { .n = n,  .sum_ln_x = 0, .sum_ln_1x = 0}; \
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	cls.sum_ln_x += log(x[i]);					\
	cls.sum_ln_1x += log(1 - x[i]);					\
      }									\
									\
    stats_beta_fit_mm##SUFFIX(n, x, alpha, beta);			\
    TYPE ab[2] = {*alpha, *beta};					\
    ret = optimisation_gradient_descent##SUFFIX(2, ab, OPTIM_MAX,	\
						NULL, gradf##SUFFIX, &cls); \
    /* if <= 0, return the mm estimates */				\
    if (ab[0] > 0 && ab[1] > 0)						\
      {									\
	*alpha = ab[0];							\
	*beta = ab[1];							\
      }									\
      									\
    ERROR_RET(0 != ret, -1);						\
    									\
    return 0;								\
  }									\
									\
  int									\
  stats_beta_fit##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta) \
  {									\
    stats_beta_fit_mm##SUFFIX(n, x, alpha, beta);				\
    return 0;								\
    /* return stats_beta_fit_mle_newton##SUFFIX(n, x, alpha, beta); */	\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)

