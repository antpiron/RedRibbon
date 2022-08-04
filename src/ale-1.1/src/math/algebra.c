#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ale/algebra.h"
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/error.h"
#include "ale/memory.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  alg_sum_of_squares##SUFFIX(size_t n, const TYPE vec[n])		\
  {									\
    TYPE sum = 0;							\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      sum += vec[i] * vec[i];						\
    									\
    return sum;								\
  }									\
  									\
  TYPE									\
  alg_norm##SUFFIX(size_t n, const TYPE vec[n])				\
  {									\
    return sqrt##SUFFIX(alg_sum_of_squares##SUFFIX(n, vec));		\
  }									\
  									\
  TYPE*									\
  alg_normalize##SUFFIX(size_t n, const TYPE vec[n], TYPE res[n])	\
  {									\
    TYPE norm = alg_norm##SUFFIX(n, vec);				\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = vec[i] / norm;						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_identity_init##SUFFIX(size_t m, size_t n, TYPE A[m][n])		\
  {									\
    ALG_INIT_M(m, n, A, ( i == j )?1:0);				\
    									\
    return &A[0][0];							\
  }									\
  									\
  TYPE									\
  alg_dot##SUFFIX(size_t n, const TYPE v1[n], const TYPE v2[n])		\
  {									\
    TYPE res = 0;							\
    for (size_t i = 0 ; i < n ; i++)					\
      res += v1[i] * v2[i];						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_lin_comb2##SUFFIX(size_t n, TYPE a1, const TYPE v1[n],		\
			TYPE a2, const TYPE v2[n], TYPE res[n])		\
  {									\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = a1*v1[i] + a2*v2[i];					\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_sub_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n])	\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = v1[i] - v2[i];						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_add_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n])	\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = v1[i] + v2[i];						\
    									\
    return res;								\
  }									\
  									\
  TYPE*									\
  alg_opposite_v##SUFFIX(size_t n, TYPE v[n], TYPE res[n])		\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = -v[i];							\
    									\
    return res;								\
  }									\
   									\
  TYPE*									\
  alg_div_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = vec[i] / d;						\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[i] = vec[i] * d;						\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_m_v##SUFFIX(size_t m, size_t n, const TYPE A[m][n], const TYPE vec[n], TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < m ; i++)					\
      {									\
	res[i] = 0;							\
	for (size_t j = 0 ; j < n ; j++)				\
	  res[i] += A[i][j] * vec[j];					\
      }									\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_vt_m##SUFFIX(size_t m, size_t n, const TYPE vec[m], const TYPE A[m][n], TYPE res[n]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      res[n] = 0;							\
									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	res[j] += A[i][j] * vec[i];					\
									\
    return res;								\
  }									\
									\
  TYPE*									\
  alg_mul_m_m##SUFFIX(size_t m, size_t n, size_t p, const TYPE A[m][n], const TYPE B[n][p], TYPE res[m][p]) \
  {									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = 0 ; j < p ; j++)					\
	{								\
	  res[i][j] = 0;						\
	  for (size_t k = 0 ; k < n ; k++)				\
	    res[i][j] += A[i][k] * B[k][j];				\
	}								\
									\
    return res[0];							\
  }									\
									\
  TYPE*									\
  alg_mul_L_A##SUFFIX(size_t n, size_t p, const TYPE L[n][n], const TYPE A[n][p], TYPE LA[n][p]) \
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      for (size_t j = 0 ; j < p ; j++)					\
	{								\
	  LA[i][j] = 0;							\
	  for (size_t k = 0 ; k < i+1 ; k++)				\
	    LA[i][j] += L[i][k] * A[k][j];				\
	}								\
									\
    return LA[0];							\
  }									\
									\
  TYPE*									\
  alg_transpose##SUFFIX(size_t m, size_t n, const TYPE A[m][n], TYPE res[n][m]) \
  {									\
    size_t min_m_n = (m < n)?m:n;					\
									\
    for (size_t i = 0 ; i < min_m_n ; i++)				\
      res[i][i] = A[i][i];						\
									\
    for (size_t i = 0 ; i < min_m_n ; i++)				\
      {									\
	for (size_t j = i + 1 ; j < min_m_n ; j++)			\
	  {								\
	    TYPE tmp = A[i][j];						\
	    res[i][j] = A[j][i];					\
	    res[j][i] = tmp;						\
	    res[j][i] = A[i][j];					\
	  }								\
      }									\
									\
    for (size_t i = n ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	res[j][i] = A[i][j];						\
    									\
    if (m < n)								\
      {									\
	for (size_t i = 0 ; i < m ; i++)				\
	  for (size_t j = m ; j < n ; j++)				\
	    res[j][i] = A[i][j];					\
      }									\
									\
    return (TYPE*) res;							\
  }									\
									\
  TYPE*									\
  alg_U_transpose##SUFFIX(size_t n, const TYPE U[n][n], TYPE L[n][n])	\
  {									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	L[i][i] = U[i][i];						\
	for (size_t j = i+1 ; j < n ; j++)				\
	  {								\
	    L[j][i] = U[i][j];						\
	    L[i][j] = 0;						\
	  }								\
      }									\
									\
    return L[0];							\
  }									\
									\
  int									\
  alg_UX_B_solve##SUFFIX(size_t n, size_t p, const TYPE U[n][n], const TYPE B[n][p], TYPE X[n][p]) \
  {									\
    TYPE *sum = NULL;							\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      ERROR_CUSTOM_RET(0 == U[i][i], 1, -1);				\
									\
    sum = malloc(sizeof(TYPE) * p);					\
									\
    for (ssize_t i = n - 1 ; i >= 0 ; i--)				\
      {									\
	for (size_t k = 0 ; k < p ; k++)				\
	  sum[k] = 0;							\
	for (size_t j = i+1 ; j < n ; j++)				\
	  for (size_t k = 0 ; k < p ; k++)				\
	    sum[k] += U[i][j] * X[j][k];				\
									\
	for (size_t k = 0 ; k < p ; k++)				\
	  X[i][k] = (B[i][k] - sum[k]) / U[i][i] ;			\
      }									\
									\
    free(sum);								\
									\
    return 0;								\
  }									\
									\
  int									\
  alg_LX_B_solve##SUFFIX(size_t n, size_t p, const TYPE L[n][n], const TYPE B[n][p], TYPE X[n][p]) \
  {									\
    TYPE *sum = NULL;							\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      ERROR_CUSTOM_RET(0 == L[i][i], 1, -1);				\
									\
    sum = malloc(sizeof(TYPE) * p);					\
									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	for (size_t k = 0 ; k < p ; k++)				\
	  sum[k] = 0;							\
	for (size_t j = 0 ; j < i ; j++)				\
	  for (size_t k = 0 ; k < p ; k++)				\
	    sum[k] += L[i][j] * X[j][k];				\
									\
	for (size_t k = 0 ; k < p ; k++)				\
	  X[i][k] = (B[i][k] - sum[k]) / L[i][i] ;			\
      }									\
									\
    free(sum);								\
									\
    return 0;								\
  }									\
									\
  int									\
  alg_U_inverse##SUFFIX(size_t n, const TYPE U[n][n], TYPE X[n][n])	\
  {									\
    int ret = 0;							\
    TYPE (*I)[n] = malloc(sizeof(TYPE) * n * n);			\
									\
    alg_identity_init##SUFFIX(n, n, I);					\
									\
    ret = alg_UX_B_solve##SUFFIX(n, n, U, I, X);			\
									\
    free(I);								\
									\
    return ret;								\
  }									\
									\
  int									\
  alg_L_inverse##SUFFIX(size_t n, const TYPE L[n][n], TYPE X[n][n])	\
  {									\
    int ret = 0;							\
    TYPE (*I)[n] = malloc(sizeof(TYPE) * n * n);			\
									\
    alg_identity_init##SUFFIX(n, n, I);					\
									\
    ret = alg_LX_B_solve##SUFFIX(n, n, L, I, X);			\
									\
    free(I);								\
									\
    return ret;								\
  }									\
									\
  int									\
  alg_AX_B_solve##SUFFIX(size_t n, size_t p, TYPE A[n][n], TYPE B[n][p], TYPE X[n][p]) \
  {									\
    struct mem_pool pool;					        \
    mem_init_size(&pool, sizeof(TYPE) * n * n * 2);			\
    TYPE (*V)[n][n] = mem_malloc(&pool, sizeof(*V));			\
    TYPE (*R)[n][n] = mem_malloc(&pool, sizeof(*R));			\
    int ret;								\
									\
    memcpy(*R, A, sizeof(*R));						\
    alg_QR_householder##SUFFIX(n, n, *R, *V);				\
    householder_proj_QtB##SUFFIX(n, n, p, *V, B);			\
									\
    ret = alg_UX_B_solve##SUFFIX(n, p, *R, B, X);			\
									\
    mem_destroy(&pool);							\
									\
    return ret;								\
  }									\
									\
  int									\
  alg_AX_B_OLS_init##SUFFIX(struct alg_ols##SUFFIX *ols,		\
			    size_t m, size_t n, size_t p,		\
			    TYPE A[m][n], TYPE B[m][p], TYPE (*X)[p]) \
  {									\
    int ret = 0;							\
									\
    mem_init(&ols->pool);						\
    ols->m = m;								\
    ols->n = n;								\
    ols->p = p;								\
    ols->A = A;								\
    ols->B = B;								\
    ols->X = (NULL == X)?						\
      mem_malloc(&ols->pool, sizeof(TYPE) * n * p) : X;			\
									\
    ols->V = mem_malloc(&ols->pool, sizeof(TYPE) * n * m);		\
    ols->R = mem_malloc(&ols->pool, sizeof(TYPE) * n * m);		\
    ols->Rt = mem_malloc(&ols->pool, sizeof(TYPE) * n * n);		\
    ols->QtB = mem_malloc(&ols->pool, sizeof(TYPE) * m * p);		\
    ols->RtQtB = mem_malloc(&ols->pool, sizeof(TYPE) * n * p);		\
    ols->Y =  mem_malloc(&ols->pool, sizeof(TYPE) * n * p);		\
    ols->r_squared = ols->score = ols->pvalue = ols->mse = NULL;	\
    ols->beta_pvalue = ols->beta_score = NULL;				\
    									\
    memcpy(ols->R, A, sizeof(TYPE) * n * m);				\
    ret = alg_QR_householder##SUFFIX(m, n, ols->R, ols->V);		\
    ERROR_CUSTOM_GOTO(ret < 0, -1, OLS_INIT_ERROR##SUFFIX);		\
    									\
    memcpy(ols->QtB, B, sizeof(TYPE) * m * p);				\
    householder_proj_QtB##SUFFIX(m, n, p, ols->V, ols->QtB);		\
    									\
    alg_U_transpose##SUFFIX(n, ols->R, ols->Rt);			\
									\
    alg_mul_L_A##SUFFIX(n, p,  ols->Rt, ols->QtB, ols->RtQtB);		\
    									\
    ret = alg_LX_B_solve##SUFFIX(n, p, ols->Rt, ols->RtQtB, ols->Y);	\
    ERROR_CUSTOM_GOTO(ret < 0, -2, OLS_INIT_ERROR##SUFFIX);		\
    									\
    ret = alg_UX_B_solve##SUFFIX(n, p, ols->R, ols->Y, ols->X);		\
    ERROR_CUSTOM_GOTO(ret < 0, -3, OLS_INIT_ERROR##SUFFIX);		\
    									\
    return ret;								\
									\
OLS_INIT_ERROR##SUFFIX:							\
									\
    mem_destroy(&ols->pool);						\
									\
    return ret;								\
  }									\
									\
  void								        \
  alg_AX_B_OLS_destroy##SUFFIX(struct alg_ols##SUFFIX *ols)		\
  {									\
    mem_destroy(&ols->pool);						\
  }									\
  									\
  int									\
  alg_AX_B_OLS_statistics##SUFFIX(struct alg_ols##SUFFIX *ols, size_t intercept) \
  {									\
    if ( NULL != ols->r_squared )					\
      {									\
	if (0 == ols->ret_statistics)					\
	  return 0;							\
	else								\
	  ERROR_CUSTOM_RET(1, ols->ret_statistics, (ols->custom_ret_statistics)); \
      }									\
									\
    size_t m = ols->m;							\
    size_t n = ols->n;							\
    size_t p = ols->p;							\
    TYPE (*A)[n] = ols->A;						\
    TYPE (*B)[p] = ols->B;						\
    TYPE (*X)[p] = ols->X;						\
    TYPE (*R)[n] = ols->R;						\
    TYPE (*Rt)[n] = ols->Rt;						\
    									\
    ols->ret_statistics = ols->custom_ret_statistics = 0;		\
									\
    TYPE *r_squared = ols->r_squared = mem_malloc(&ols->pool, sizeof(TYPE) * p); \
    TYPE *score = ols->score = mem_malloc(&ols->pool, sizeof(TYPE) * p); \
    TYPE *pvalue = ols->pvalue = mem_malloc(&ols->pool, sizeof(TYPE) * p); \
    /* TODO: add cross-validation */						\
    /* TYPE *mse = */ ols->mse = mem_malloc(&ols->pool, sizeof(TYPE) * p); \
    TYPE (*beta_pvalue)[p] = ols->beta_pvalue = mem_malloc(&ols->pool, sizeof(TYPE) * n * p); \
    TYPE (*beta_score)[p] = ols->beta_score = mem_malloc(&ols->pool, sizeof(TYPE) * n * p); \
    									\
    TYPE df = m - n;							\
    TYPE (*AX)[p] = ols->AX = mem_malloc(&ols->pool, sizeof(TYPE) * m * p);	\
    TYPE (*Q)[n] = ols->Q = mem_malloc(&ols->pool, sizeof(TYPE) * m * n);	\
    TYPE *Hii = ols->Q = mem_malloc(&ols->pool, sizeof(TYPE) * m);	\
    TYPE *loocv = ols->loocv = mem_malloc(&ols->pool, sizeof(TYPE) * p); \
    TYPE *rss = ols->rss = mem_malloc(&ols->pool, sizeof(TYPE) * p);	\
    TYPE *means = ols->means = mem_malloc(&ols->pool, sizeof(TYPE) * p); \
    TYPE *mss = ols->mss = mem_malloc(&ols->pool, sizeof(TYPE) * p);	\
    TYPE (*Rt_inv)[n] = ols->Rt_inv= mem_malloc(&ols->pool, sizeof(TYPE) * n * n); \
    TYPE (*AtA_inv)[n] = ols->AtA_inv = mem_malloc(&ols->pool, sizeof(TYPE) * n * n);	\
    									\
    /* compute p-values for coefficients https://stats.stackexchange.com/a/344008 */ \
    /* https://en.wikipedia.org/wiki/Residual_sum_of_squares#Relation_with_Pearson's_product-moment_correlation */ \
    									\
    alg_mul_m_m##SUFFIX(m, n, p, A, X, AX);				\
    if (intercept)							\
      stats_colmeans##SUFFIX(m, p, AX, means);				\
    									\
    /* LOO https://robjhyndman.com/hyndsight/loocv-linear-models/ */	\
    alg_identity_init##SUFFIX(m, n, Q);					\
    householder_proj_QX##SUFFIX(m, n, n, ols->V, Q);			\
    for (size_t i = 0 ; i < m ; i++)					\
      Hii[i] = alg_sum_of_squares##SUFFIX(n, Q[i]);			\
    									\
    for (size_t i = 0 ; i < p ; i++)					\
      mss[i] = rss[i] = loocv[i] = 0;					\
    									\
    for (size_t i = 0 ; i < m ; i++)					\
      for (size_t j = 0 ; j < p ; j++)					\
	{								\
	  TYPE e_ij = B[i][j] - AX[i][j];				\
	  TYPE e_ij_loo = e_ij / (1 - Hii[j]);				\
	  TYPE f_ij = (intercept) ? AX[i][j] - means[j] : AX[i][j];	\
	  rss[j] += e_ij * e_ij;					\
	  loocv[j] += e_ij_loo * e_ij_loo;				\
	  mss[j] += f_ij * f_ij;					\
	}								\
									\
									\
    for (size_t i = 0 ; i < p ; i++)					\
      {									\
	TYPE denom =  mss[i] + rss[i];					\
	/* TODO: https://github.com/SurajGupta/r-source/blob/a28e609e72ed7c47f6ddfbb86c85279a0750f0b7/src/library/stats/R/lm.R#L335 */ \
	/* R module do not check for zero denom */			\
	r_squared[i] = mss[i] / denom;					\
	/* http://facweb.cs.depaul.edu/sjost/csc423/documents/f-test-reg.htm */	\
	score[i] = (mss[i] / (n - intercept)) / (rss[i] / (df));	\
	pvalue[i] = isinf(score[i]) ?					\
	  0 :								\
	  1 - stats_F_F(score[i],(n - intercept), df);			\
	loocv[i] /= m;							\
      }									\
    									\
									\
    ols->ret_statistics = alg_L_inverse##SUFFIX(n, Rt, Rt_inv);		\
    ERROR_CUSTOM_RET(ols->ret_statistics < 0, ols->ret_statistics,	\
		     (ols->custom_ret_statistics = -2));		\
    									\
    ols->ret_statistics= alg_UX_B_solve##SUFFIX(n, n,			\
						R, Rt_inv, AtA_inv);	\
    ERROR_CUSTOM_RET(ols->ret_statistics < 0, ols->ret_statistics,	\
		     (ols->custom_ret_statistics = -3));		\
    for (size_t i = 0 ; i < n ; i++)					\
      for (size_t j = 0 ; j < p ; j++)					\
	{								\
	  TYPE mse = rss[j] / df; 					\
	  TYPE Si2 = AtA_inv[i][i];					\
	  if ( Si2 < 0) Si2 = 0;					\
	  TYPE denom = sqrt##SUFFIX(Si2 * mse);				\
	  if (0 != denom)						\
	    {								\
	      TYPE t = X[i][j] / denom;					\
	      beta_pvalue[i][j] = 2 *					\
		(1 - stats_student_F##SUFFIX(fabs##SUFFIX(t), df));	\
	      beta_score[i][j] = t;					\
	    }								\
	  else								\
	    {								\
	      beta_pvalue [i][j] = 0;					\
	      beta_score[i][j] = INFINITY;				\
	    }								\
	}								\
    									\
    									\
    return ols->ret_statistics;						\
  }									\
  									\
  									\
  int									\
  alg_AX_B_OLS_solve##SUFFIX(size_t m, size_t n, size_t p, TYPE A[m][n], TYPE B[m][p], TYPE X[n][p]) \
  {									\
    struct alg_ols##SUFFIX ols;						\
    int ret = alg_AX_B_OLS_init##SUFFIX(&ols, m, n, p, A, B, X);	\
    ERROR_RET(ret < 0, ret);						\
									\
    alg_AX_B_OLS_destroy##SUFFIX(&ols);					\
									\
    return ret;								\
  }									\
									\
  int									\
  alg_QtR_mgs##SUFFIX(size_t m, size_t n, const TYPE A[m][n], TYPE Qt[n][m], TYPE R[n][n]) \
  {									\
    /* http://www.math.iit.edu/~fass/477577_Chapter_4.pdf */		\
    if (m < n)								\
      return -1;							\
									\
    alg_transpose##SUFFIX(m,n,A,Qt);					\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	R[i][i] = alg_norm##SUFFIX(m, Qt[i]);				\
	alg_div_v_c##SUFFIX(m, Qt[i], R[i][i], Qt[i]);			\
	for (size_t j = i+1 ; j < n ; j++)				\
	  {								\
	    R[i][j] = alg_dot##SUFFIX(m, Qt[i], Qt[i]);			\
	    alg_lin_comb2##SUFFIX(m, 1.0, Qt[j], -R[i][j], Qt[i], Qt[j]); \
	  }								\
      }									\
									\
    return 0;								\
  }									\
									\
  void									\
  householder_proj##SUFFIX(size_t m, size_t n, size_t k, TYPE v[k], TYPE A[m][n], TYPE vA[n]) \
  {									\
    /* dynamic alloc. or param */					\
    /* TYPE vA[n]; */							\
									\
    /* v^t * A_k:m,k:n */						\
    for (size_t i = 0 ; i < n ; i++)					\
      vA[i] = 0;							\
    for (size_t i = k ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	vA[j] += v[i-k] * A[i][j];					\
									\
    /* A_k:m,k:n = A_k:m,k:n - 2 * v * v^t * A_k:m,k:n */		\
    for (size_t i = k ; i < m ; i++)					\
      for (size_t j = 0 ; j < n ; j++)					\
	A[i][j] += - 2 * v[i-k] * vA[j];				\
  }									\
									\
  int									\
  householder_proj_QtB##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE B[m][p]) \
  {									\
    TYPE (*vB)[p] = malloc(sizeof(*vB));				\
									\
    for (size_t k = 0 ; k < n ; k++)					\
      householder_proj##SUFFIX(m, p, k, V[k], B, *vB);				\
									\
    free(vB);								\
									\
    return 0;								\
  }									\
									\
  int									\
  householder_proj_QX##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE X[m][p]) \
  {									\
    TYPE (*vX)[p] = malloc(sizeof(*vX));				\
									\
    for (ssize_t k = n-1 ; k >= 0 ; k--)				\
      householder_proj##SUFFIX(m, p, k, V[k], X, *vX);				\
									\
    free(vX);								\
									\
    return 0;								\
  }									\
									\
									\
									\
  /* destroy A and B */							\
  int									\
  alg_QR_householder##SUFFIX(size_t m, size_t n, TYPE A[m][n], TYPE V[n][m]) \
  {									\
    int ret = 0;							\
    TYPE (*vA)[n];							\
									\
    if (m < n)								\
      return -1;							\
									\
    vA = malloc(sizeof(*vA));						\
    if ( NULL == vA)							\
      return -1;							\
									\
    /* https://math.dartmouth.edu/~m116w17/Householder.pdf */		\
									\
    for (size_t k = 0 ; k < n ; k++)					\
      {									\
	size_t mv = m - k;						\
	TYPE ss, norm;							\
									\
	for (size_t i = 0 ; i < mv ; i++)				\
	  V[k][i] = A[i+k][k];						\
	/* ||v_2:m-k|| */						\
	ss = alg_sum_of_squares##SUFFIX(mv-1, V[k]+1);			\
	/* ||v|| */							\
	norm = sqrt##SUFFIX(V[k][0]*V[k][0] + ss);			\
	/* v = v + sign(v1) * e1 */					\
	V[k][0] += copysign##SUFFIX(1.0, V[k][0]) * norm;		\
	/* ||v|| */							\
	norm = sqrt##SUFFIX(V[k][0]*V[k][0] + ss);			\
	if (0 == norm)							\
	  {								\
	    ret = -1;							\
	    break;							\
	  }								\
	alg_div_v_c##SUFFIX(mv, V[k], norm, V[k]);			\
									\
	/* A_k:m,k:n = A_k:m,k:n - 2 * v * v^t * A_k:m,k:n */		\
	householder_proj##SUFFIX(m, n, k, V[k], A, *vA);		\
      }									\
									\
    free(vA);								\
									\
    return ret;								\
  }

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)



void	
print_m(size_t m, size_t n, const double A[m][n])
{
  for (size_t i = 0 ; i < m ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	printf("%f\t", A[i][j]);
      printf("\n");
    }	
}

void	
print_ml(size_t m, size_t n, const long double A[m][n])
{
  for (size_t i = 0 ; i < m ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	printf("%Lf\t", A[i][j]);
      printf("\n");
    }	
}

void	
print_v(size_t n, const double v[n])
{
  for (size_t i = 0 ; i < n ; i++)
    printf("%f\t", v[i]);
  printf("\n");
}

void	
print_vl(size_t n, const long double v[n])
{
  for (size_t i = 0 ; i < n ; i++)
    printf("%Lf\t", v[i]);
  printf("\n");
}
