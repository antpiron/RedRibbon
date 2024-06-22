#ifndef __ALGEBRA_H
#define __ALGEBRA_H

#include <stddef.h>
#include <ale/memory.h>

#define ALG_INIT_M(m, n, A, value) do {		\
    for (size_t i = 0 ; i < (m) ; i++)		\
      for (size_t j = 0 ; j < (n) ; j++)	\
	(A)[i][j] = (value);			\
  } while (0)

#define ALG_INIT_V(n, v, value) do {		\
    for (size_t i = 0 ; i < (n) ; i++)		\
	(v)[i] = (value);			\
  } while (0)

#define ALG_ADD_M(m, n, A, value) do {		\
    for (size_t i = 0 ; i < (m) ; i++)		\
      for (size_t j = 0 ; j < (n) ; j++)	\
	(A)[i][j] += (value);			\
  } while (0)

#define ALG_GENERIC_HEADERS(SUFFIX,TYPE)				\
									\
  TYPE* alg_add_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n]); \
  TYPE* alg_sub_v_v##SUFFIX(size_t n, TYPE v1[n], TYPE v2[n], TYPE res[n]); \
  TYPE* alg_mul_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]); \
  TYPE* alg_mul_m_v##SUFFIX(size_t m, size_t n, const TYPE A[m][n], const TYPE vec[n], TYPE res[n]); \
  TYPE*	alg_div_v_c##SUFFIX(size_t n, const TYPE vec[n], TYPE d,  TYPE res[n]); \
  TYPE* alg_opposite_v##SUFFIX(size_t n, TYPE v[n], TYPE res[n]);	\
  TYPE* alg_mul_m_v##SUFFIX(size_t m, size_t n, const TYPE A[m][n], const TYPE vec[n], TYPE res[n]); \
  									\
  TYPE* alg_identity_init##SUFFIX(size_t m, size_t n, TYPE A[m][n]);	\
  TYPE* alg_transpose##SUFFIX(size_t m, size_t n, const TYPE A[m][n], TYPE res[n][m]); \
  TYPE* alg_mul_m_m##SUFFIX(size_t m, size_t n, size_t p, const TYPE A[m][n], const TYPE B[n][p], TYPE res[m][p]); \
  int alg_UX_B_solve##SUFFIX(size_t n, size_t p, const TYPE U[n][n], const TYPE B[n][p], TYPE X[n][p]); \
  int alg_LX_B_solve##SUFFIX(size_t n, size_t p, const TYPE L[n][n], const TYPE B[n][p], TYPE X[n][p]); \
									\
  int alg_U_inverse##SUFFIX(size_t n, const TYPE U[n][n], TYPE X[n][n]); \
  int alg_L_inverse##SUFFIX(size_t n, const TYPE L[n][n], TYPE X[n][n]); \
  									\
  int alg_LUP_pivoting##SUFFIX(size_t m, size_t n,			\
			       TYPE A[m][n],				\
			       size_t perm[m], size_t *s);		\
  TYPE alg_det##SUFFIX(size_t n, const TYPE A[n][n]);			\
									\
  int alg_AX_B_solve##SUFFIX(size_t n, size_t p, TYPE A[n][n], TYPE B[n][p], TYPE X[n][p]); \
									\
  struct alg_ols##SUFFIX						\
  {									\
    struct mem_pool pool;						\
    size_t m, n, p;							\
    void *A;                                /* [m, n] */		\
    void *B;                                /* [m, p] */		\
    void *X;                                /* [n, p] */		\
    void *r_squared, *score, *pvalue, *mse; /* [p] */			\
    void *beta_pvalue, *beta_score;	    /* [n, p] */		\
									\
    									\
    void *V;                                /* [n][m] */		\
    void *R;                                /* [m][n] */		\
    void *Rt;                               /* [n][n] */		\
    void *QtB;                              /* [m][p] */		\
    void *RtQtB;                            /* [n][p] */		\
    void *Y;                                /* [n][p] */		\
    									\
    void *AX;                               /* [m][p] */		\
    void *Q;                                /* [m][n] */		\
    void *Hii;                              /* [m] */		\
    void *loocv, *rss, *means, *mss;                /* [p] */		\
    void *Rt_inv, *AtA_inv;                 /* [n][n] */		\
    int ret_statistics, custom_ret_statistics;				\
  };									\
									\
  int alg_AX_B_OLS_init##SUFFIX(struct alg_ols##SUFFIX *ols,		\
				size_t m, size_t n, size_t p,		\
				TYPE A[m][n],				\
				TYPE B[m][p],				\
				TYPE (*X)[p]);				\
  void alg_AX_B_OLS_destroy##SUFFIX(struct alg_ols##SUFFIX *ols);	\
  int alg_AX_B_OLS_statistics##SUFFIX(struct alg_ols##SUFFIX *ols,	\
				     size_t intercept);			\
  int alg_AX_B_OLS_solve##SUFFIX(size_t m, size_t n, size_t p,		\
				 TYPE A[m][n], TYPE B[m][p],		\
				 TYPE X[n][p]);				\
  									\
  TYPE alg_norm##SUFFIX(size_t n, const TYPE vec[n]);			\
  TYPE alg_dot##SUFFIX(size_t n, const TYPE v1[n], const TYPE v2[n]);	\
  									\
  void print_m##SUFFIX(size_t m, size_t n, const TYPE A[m][n]);		\
  void print_v##SUFFIX(size_t n, const TYPE v[n]);			\
  									\
  int alg_QR_householder##SUFFIX(size_t m, size_t n, TYPE A[m][n], TYPE V[n][m]); \
  int householder_proj_QtB##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE B[m][p]); \
  int householder_proj_QX##SUFFIX(size_t m, size_t n, size_t p, TYPE V[n][m], TYPE X[m][p]); \
									\
  int alg_LLt_cholesky##SUFFIX(size_t n, const TYPE A[n][n], TYPE L[n][n]); \
  TYPE alg_symmetric_definite_det##SUFFIX(size_t n, const TYPE A[n][n]); \
  int  alg_symmetric_definite_inv##SUFFIX(size_t n, const TYPE A[n][n],	\
					  TYPE X[n][n]);


ALG_GENERIC_HEADERS(,double)
ALG_GENERIC_HEADERS(l,long double)


#endif
