#ifndef __STATS_H
#define __STATS_H

#include <stddef.h>
#include <stdint.h>
#include <float.h>
#include <sys/types.h>

#include <ale/memory.h>
#include <ale/interpolation.h>
#include <ale/stats-normalize.h>

#define STATS_LOWER (0)
#define STATS_UPPER (1)

#define STATS_EPS (DBL_EPSILON)
#define STATS_EPSl (LDBL_EPSILON)


void stats_shuffle(void *vec, size_t nmemb, size_t size);
void shuffle_n_size_t(size_t n, size_t *vec);

#define STATS_GENERIC_HEADERS(SUFFIX,TYPE)				\
  TYPE stats_mean##SUFFIX(size_t n, const TYPE x[n]);			\
  TYPE stats_geom_mean##SUFFIX(size_t n, const TYPE x[n]);		\
  TYPE stats_var##SUFFIX(size_t n, const TYPE x[n]);			\
  TYPE stats_sd##SUFFIX(size_t n, const TYPE x[n]);			\
  TYPE stats_diff_mean##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n]); \
  TYPE stats_diff_var##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n]); \
  TYPE stats_diff_sd##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n]); \
  TYPE stats_cov##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n]);	\
									\
  void stats_rowmeans##SUFFIX(size_t m, size_t n,			\
			      const TYPE A[m][n], TYPE means[m]);	\
  void stats_colmeans##SUFFIX(size_t m, size_t n,			\
			      const TYPE A[m][n], TYPE means[n]);	\
									\
  int stats_pearson_corr##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n], \
				 TYPE *rho, TYPE *pvalue);		\
  void stats_mat_cov##SUFFIX(size_t m, size_t n, TYPE cov[m][m], const TYPE x[m][n]); \
  									\
  TYPE stats_rss##SUFFIX(size_t m, size_t n, const TYPE y[m], const TYPE x[m][n], \
			 TYPE (*predict)(const TYPE x[n], void *cls), void *cls); \
  int stats_rsquared##SUFFIX(size_t m, size_t n, const TYPE y[m], const TYPE x[m][n], \
			     TYPE (*predict)(const TYPE x[n], void *cls), void *cls, \
			     TYPE *rsquared);				\
  int stats_p_adjust_fdr_bh##SUFFIX(size_t n,  const TYPE p[n], TYPE padj[n]); \
  int stats_p_adjust_fwer_bonf##SUFFIX(size_t n,  const TYPE p[n], TYPE padj[n]); \
  									\
  TYPE stats_binom_f##SUFFIX(long k, long n, TYPE p);			\
  TYPE stats_binom_F##SUFFIX(long k, long n, TYPE p);			\
									\
  size_t stats_categorical_rand##SUFFIX(size_t n, TYPE cumul_p[n]);	\
  									\
  TYPE stats_unif_std_rand##SUFFIX();					\
  TYPE stats_unif_rand##SUFFIX(TYPE min, TYPE max);			\
  TYPE stats_unif_std_F##SUFFIX(TYPE x);				\
  TYPE stats_unif_F##SUFFIX(TYPE x, TYPE min, TYPE max);		\
									\
  TYPE stats_norm_std_rand##SUFFIX();					\
  TYPE stats_norm_rand##SUFFIX(TYPE mu, TYPE sig);			\
  TYPE stats_norm_std_F##SUFFIX(TYPE x);				\
  TYPE stats_norm_F##SUFFIX(TYPE x, TYPE mu, TYPE sig);			\
									\
  TYPE stats_poisson_f##SUFFIX(long k, TYPE lambda);			\
  TYPE stats_poisson_F##SUFFIX(long k, TYPE lambda);			\
  unsigned long stats_poisson_rand_its##SUFFIX(TYPE lambda);		\
  unsigned long stats_poisson_rand_pa##SUFFIX(TYPE lambda);		\
  unsigned long stats_poisson_rand##SUFFIX(TYPE lambda);		\
									\
  TYPE stats_student_rand##SUFFIX(TYPE df);				\
  TYPE stats_student_F##SUFFIX(TYPE x, TYPE df);			\
									\
  TYPE stats_F_F##SUFFIX(TYPE x, TYPE d1, TYPE d2);			\
									\
  struct stats_t_test##SUFFIX						\
  {									\
    TYPE pvalue, df, t;							\
  };									\
  TYPE stats_t_test##SUFFIX(size_t n, const TYPE x[n], TYPE mu, int H0,	\
			    struct stats_t_test##SUFFIX *data);		\
  TYPE stats_t_test_paired##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n], \
				   TYPE mu, int H0,			\
				   struct stats_t_test##SUFFIX *data);	\
  TYPE stats_t_test_welch##SUFFIX(size_t nx, const TYPE x[nx], size_t ny, const TYPE y[ny], \
				  TYPE mu, int H0,			\
				  struct stats_t_test##SUFFIX *data);	\
  									\
  TYPE stats_hyper_f##SUFFIX(long k, long K, long n, long N);		\
  TYPE stats_hyper_tail##SUFFIX(long k, long K, long n, long N, int upper); \
  TYPE stats_hyper_F##SUFFIX(long k, long K, long n, long N);		\
  									\
  TYPE stats_beta_rand##SUFFIX(TYPE alpha, TYPE beta);			\
  TYPE stats_beta_F##SUFFIX(TYPE x, TYPE alpha, TYPE beta);		\
  TYPE stats_beta_F_inv##SUFFIX(TYPE x, TYPE alpha, TYPE beta);		\
  void stats_beta_fit_mm##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta); \
  int stats_beta_fit_mle_newton##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta); \
  int stats_beta_fit_mle##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta); \
  int stats_beta_fit##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta); \
  									\
  TYPE stats_gamma_rand##SUFFIX(TYPE alpha, TYPE beta);			\
  TYPE stats_gamma_rand_k_theta##SUFFIX(TYPE k, TYPE theta);		\
  TYPE stats_gamma_F##SUFFIX(TYPE x, TYPE alpha, TYPE beta);		\
  void stats_gamma_fit_mm##SUFFIX(size_t n, const TYPE x[n], TYPE *alpha, TYPE *beta); \
									\
  									\
  struct stats_ecdf##SUFFIX						\
  {									\
    size_t n, no_dup_n;							\
    TYPE *x;								\
    size_t *index;							\
    TYPE *no_dup_F;							\
    TYPE *no_dup_x;							\
    struct interpolation_linear##SUFFIX inter_inv;			\
    struct interpolation_linear##SUFFIX inter;				\
    struct mem_pool pool;						\
  };									\
									\
  void stats_ecdf_init##SUFFIX(struct stats_ecdf##SUFFIX *ecdf,		\
			       size_t n, TYPE x[n]);			\
  void stats_ecdf_destroy##SUFFIX(struct stats_ecdf##SUFFIX *ecdf);	\
  TYPE stats_ecdf_F##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x);	\
  TYPE stats_ecdf_F_linear##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x); \
  TYPE stats_ecdf_F_inv##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE p); \
  TYPE stats_ecdf_rand##SUFFIX(struct stats_ecdf##SUFFIX *ecdf);	\
  									\
  TYPE stats_kolmo_F_approx##SUFFIX(TYPE d, unsigned long n);		\
  TYPE stats_kolmo_F_carvalho##SUFFIX(TYPE d, unsigned long n);		\
  TYPE stats_kolmo_F##SUFFIX(TYPE d, unsigned long n);			\
  int stats_ks_test##SUFFIX(size_t n, TYPE a[n],			\
			    TYPE (*cdf)(TYPE x, void *cls), void *cls,	\
			    TYPE *pval, TYPE *stat);			\
  									\
  									\
  struct stats_permutation##SUFFIX					\
  {									\
    size_t n;								\
    TYPE *vec;								\
    ssize_t *deps;							\
    TYPE (*predict)(size_t i, TYPE x, void *cls);			\
    void *cls;								\
  };									\
  									\
  void stats_permutation_init##SUFFIX(struct stats_permutation##SUFFIX *p, \
  				      size_t n, TYPE vec[n]);		\
  void stats_permutation_destroy##SUFFIX(struct stats_permutation##SUFFIX *p); \
  void stats_permutation_dependencies##SUFFIX(struct stats_permutation##SUFFIX *p, \
  					      ssize_t *deps,		\
  					      TYPE (*predict)(size_t i, \
  							      TYPE x,	\
  							      void *cls), \
  					      void *cls);		\
  int stats_permutation##SUFFIX(struct stats_permutation##SUFFIX *p, TYPE res[p->n]);


  
STATS_GENERIC_HEADERS(,double)
STATS_GENERIC_HEADERS(l,long double)



#endif
