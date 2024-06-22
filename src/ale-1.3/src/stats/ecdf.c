#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"
#include "ale/sort.h"
#include "ale/interpolation.h"


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  void									\
  stats_ecdf_init_no_dup##SUFFIX(struct stats_ecdf##SUFFIX *ecdf)	\
  {									\
    size_t n = ecdf->n;							\
    TYPE *x = ecdf->x;							\
									\
    ecdf->no_dup_F = mem_malloc(&ecdf->pool, n * sizeof(TYPE) );	\
    ecdf->no_dup_x = mem_malloc(&ecdf->pool, n * sizeof(TYPE) );	\
									\
    TYPE last = x[ ecdf->index[0] ];					\
    ecdf->no_dup_x[0] = last;						\
    ecdf->no_dup_F[0] = (TYPE) 1 / (TYPE) ecdf->n;			\
									\
    size_t no_dup_i = 0;						\
									\
    for (size_t i = 1 ; i < n ; i++)					\
      {									\
	TYPE current = x[ ecdf->index[i] ];				\
									\
	if ( last != current )						\
	  {								\
	    ecdf->no_dup_x[ ++no_dup_i ] = current;			\
	  }								\
									\
	ecdf->no_dup_F[ no_dup_i ] = (TYPE) (i+1) / (TYPE)ecdf->n;	\
      }									\
									\
    ecdf->no_dup_n = no_dup_i + 1;					\
  }									\
									\
  void									\
  stats_ecdf_init##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, size_t n,	\
			  TYPE x[n])					\
  {									\
    ecdf->n= n;								\
    ecdf->x = x;							\
    mem_init(&ecdf->pool);						\
    ecdf->index = mem_malloc(&ecdf->pool, n * sizeof(size_t) );		\
									\
    sort_q_indirect(ecdf->index, x, n, sizeof(TYPE),			\
		    sort_compar_double##SUFFIX, NULL);			\
									\
    stats_ecdf_init_no_dup##SUFFIX(ecdf);				\
    interpolation_init_full##SUFFIX(&ecdf->inter_inv, ecdf->no_dup_n,	\
				    ecdf->no_dup_x, ecdf->no_dup_F,	\
				    NULL);				\
    mem_callback(&ecdf->pool, &ecdf->inter_inv,				\
      (void*) interpolation_destroy##SUFFIX);				\
    interpolation_init_full##SUFFIX(&ecdf->inter, ecdf->no_dup_n,	\
				    ecdf->no_dup_F, ecdf->no_dup_x,	\
				    NULL);				\
    mem_callback(&ecdf->pool, &ecdf->inter,				\
		 (void*) interpolation_destroy##SUFFIX);		\
  }									\
  									\
  void									\
  stats_ecdf_destroy##SUFFIX(struct stats_ecdf##SUFFIX *ecdf)		\
  {									\
    mem_destroy(&ecdf->pool);						\
  }									\
  									\
  TYPE									\
  stats_ecdf_f##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x)		\
  {									\
    TYPE x0 = x;							\
    TYPE x1 = x + ALE_EPS##SUFFIX;					\
    TYPE y0 = stats_ecdf_F_linear##SUFFIX(ecdf, x0);			\
    TYPE y1 = stats_ecdf_F_linear##SUFFIX(ecdf, x1);			\
									\
    return (y1 - y0) / ALE_EPS##SUFFIX;					\
  }									\
  									\
  TYPE									\
  stats_ecdf_F##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x)		\
  {									\
    ssize_t l = 0, r = ecdf->n;						\
    									\
    if ( x < ecdf->x[ ecdf->index[0] ])					\
      return 0;								\
    									\
    if ( x >= ecdf->x[ ecdf->index[r - 1] ])				\
      return 1;								\
    									\
    while (1)								\
      {									\
	ssize_t mid = (r + l) / 2;					\
									\
	if ( x < ecdf->x[ ecdf->index[mid] ] )				\
	  r = mid;							\
	else if ( l != mid )						\
	  l = mid;							\
	else								\
	  break;							\
      }									\
    									\
    return (TYPE) (l+1) / (TYPE) ecdf->n;				\
  }									\
  									\
  TYPE									\
  stats_ecdf_F_linear##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE x)	\
  {									\
    TYPE res;								\
									\
    res = interpolation_linear_f##SUFFIX(&ecdf->inter, x,		\
					 NULL,				\
					 &(struct interpolation_min_max##SUFFIX) {.min = 0, .max = 1} ); \
									\
    return res;								\
  }  									\
  									\
  TYPE									\
  stats_ecdf_F_inv##SUFFIX(struct stats_ecdf##SUFFIX *ecdf, TYPE p)	\
  {									\
    TYPE res;								\
    const size_t n = ecdf->n;						\
    const TYPE *x = ecdf->x;						\
    const TYPE pos_float = p * n - 1.5;					\
    size_t ceil_pos = (pos_float < 0.0) ? 0 : ceil##SUFFIX(pos_float);	\
									\
    if (ceil_pos >= n)							\
      ceil_pos = n - 1;							\
									\
    res = x[ecdf->index[ceil_pos]];					\
    									\
    return res;								\
  }									\
									\
  TYPE									\
  stats_ecdf_rand##SUFFIX(struct stats_ecdf##SUFFIX *ecdf)		\
  {									\
    TYPE unif = stats_unif_std_rand##SUFFIX();				\
									\
    return stats_ecdf_F_inv##SUFFIX(ecdf, unif);			\
  }
 

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
