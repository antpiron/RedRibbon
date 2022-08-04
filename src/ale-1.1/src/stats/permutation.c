#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ale/stats.h"
#include "ale/error.h"
#include "ale/matrix.h"
#include "ale/bitset.h"



#define GENERIC_FUNC(SUFFIX,TYPE)					\
  void									\
  stats_permutation_init##SUFFIX(struct stats_permutation##SUFFIX *p,	\
				      size_t n, TYPE vec[n])		\
  {									\
    p->n = n;								\
    p->vec = vec;							\
    p->deps = NULL;							\
  }									\
									\
  void stats_permutation_destroy##SUFFIX(struct stats_permutation##SUFFIX *p) \
  {									\
    (void)p;								\
  }									\
  									\
  void									\
  stats_permutation_dependencies##SUFFIX(struct stats_permutation##SUFFIX *p, \
					 ssize_t *deps,			\
					 TYPE (*predict)(size_t i,	\
							 TYPE x,	\
							 void *cls),	\
					 void *cls)			\
  { 									\
    p->deps = deps;							\
    p->predict = predict;						\
    p->cls = cls;							\
  }									\
									\
  static void								\
  recurse_predict##SUFFIX(struct stats_permutation##SUFFIX *p,		\
			  TYPE res[p->n], struct bitset *bs,		\
			  size_t i)					\
  {									\
    ssize_t dep = p->deps[i];						\
    if ( dep < 0 || bitset_isset(bs, dep) )				\
      return;								\
									\
    if ( ! bitset_isset(bs, dep) )					\
      recurse_predict##SUFFIX(p, res, bs, dep);				\
									\
    res[i] = p->predict(i, res[dep], p->cls);				\
    bitset_set(bs, i);							\
  }									\
									\
  int									\
  stats_permutation##SUFFIX(struct stats_permutation##SUFFIX *p,	\
			    TYPE res[p->n])				\
  {									\
    size_t n = p->n;							\
    									\
    memcpy(res, p->vec, sizeof(TYPE) * n);				\
    stats_shuffle(res, n, sizeof(TYPE));				\
    									\
    if (NULL != p->deps)						\
      {									\
	struct bitset bs;						\
	bitset_init(&bs, n);						\
									\
	for (size_t i = 0 ; i < n ; i++)				\
	  {								\
	    if ( bitset_isset(&bs, i) || p->deps[i] < 0 )		\
	      continue;							\
									\
	    recurse_predict##SUFFIX(p, res, &bs, i);			\
	  }								\
									\
	bitset_destroy(&bs);						\
      }									\
									\
    return 0;								\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
