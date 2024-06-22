#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_student_rand##SUFFIX(TYPE df)					\
  {									\
    /* Num. recipes */							\
    TYPE u = stats_unif_rand##SUFFIX(0,1);				\
    TYPE v = stats_unif_rand##SUFFIX(0,1);				\
									\
    return sqrt##SUFFIX(df * ( pow##SUFFIX(u, -2/df) - 1 ) ) * cos##SUFFIX(2*M_PI*v); \
  }									\
									\
  TYPE									\
  stats_student_F##SUFFIX(TYPE x, TYPE df)				\
  {									\
    if (x < 0)								\
      return 1 - stats_student_F##SUFFIX(-x, df);			\
									\
    return 1 - 0.5 * ale_ibeta##SUFFIX(df / (df + x*x), 0.5*df, 0.5);	\
  }									\
									\
  static TYPE								\
  H0_student_pvalue##SUFFIX(int H0, TYPE t, TYPE df)			\
  {									\
    if (0 == H0)							\
      return 2 * (1-stats_student_F##SUFFIX(fabs##SUFFIX(t), df)); /* two-sided */ \
    else if (0 < H0)							\
      return 1 - stats_student_F##SUFFIX(t, df); /* greater */		\
									\
    return stats_student_F##SUFFIX(t, df); /* smaller */		\
  }									\
									\
  static inline TYPE							\
  t_test##SUFFIX(size_t n, TYPE m, TYPE s, TYPE mu, int H0,		\
		 struct stats_t_test##SUFFIX *data)			\
  {									\
    TYPE t, pvalue;							\
									\
    if (s == 0)								\
      return -1;							\
    									\
    t = (m - mu) / (s / sqrt##SUFFIX(n));				\
    pvalue = H0_student_pvalue##SUFFIX(H0, t, n-1);			\
    									\
    if (data)								\
      {									\
	data->pvalue = pvalue;						\
	data->t = t;							\
	data->df = n-1;							\
      }									\
    									\
    return pvalue;							\
  }									\
  									\
  TYPE									\
  stats_t_test##SUFFIX(size_t n, const TYPE x[n], TYPE mu, int H0,	\
		       struct stats_t_test##SUFFIX *data)		\
  {									\
    TYPE m = stats_mean##SUFFIX(n, x);					\
    TYPE s = stats_sd##SUFFIX(n, x);					\
    return t_test##SUFFIX(n, m, s, mu, H0, data);			\
  }									\
  									\
  TYPE									\
  stats_t_test_paired##SUFFIX(size_t n, const TYPE x[n], const TYPE y[n], \
			      TYPE mu, int H0,				\
			      struct stats_t_test##SUFFIX *data)	\
  {									\
    TYPE m = stats_diff_mean##SUFFIX(n, x, y);				\
    TYPE s = stats_diff_sd##SUFFIX(n, x, y);				\
    									\
    return t_test##SUFFIX(n, m, s, mu, H0, data);			\
  }									\
  									\
  TYPE									\
  stats_t_test_welch##SUFFIX(size_t nx, const TYPE x[nx], size_t ny, const TYPE y[ny], \
			     TYPE mu, int H0,				\
			     struct stats_t_test##SUFFIX *data)		\
  {									\
      /* https://en.wikipedia.org/wiki/Welch%27s_t-test */		\
    TYPE t, pvalue;							\
    TYPE mx = stats_mean##SUFFIX(nx, x);				\
    TYPE my = stats_mean##SUFFIX(ny, y);				\
    TYPE sx = stats_sd##SUFFIX(nx, x);					\
    TYPE sy = stats_sd##SUFFIX(ny, y);					\
    if (sx+sy == 0)							\
      return -1;							\
    TYPE num = sx*sx/nx + sy*sy/ny;					\
    TYPE df = num*num / (sx*sx*sx*sx / (nx*nx * (nx-1)) + sy*sy*sy*sy / (ny*ny * (ny-1)) ); \
    t = (my - mx - mu) / sqrt(num);					\
    pvalue = H0_student_pvalue##SUFFIX(H0, t, df);			\
    									\
    if (data)								\
      {									\
	data->pvalue = pvalue;						\
	  data->t = t;							\
	  data->df = df;						\
      }									\
    									\
    return pvalue;							\
  }
  

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
