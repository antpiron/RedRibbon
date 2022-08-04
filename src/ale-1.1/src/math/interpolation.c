#include <string.h>

#include "ale/algebra.h"
#include "ale/optimisation.h"
#include "ale/memory.h"
#include "ale/math.h"
#include "ale/error.h"
#include "ale/sort.h"
#include "ale/interpolation.h"

#define I_INDEX(inter, i) ( (NULL == (inter)->index) ? i : (inter)->index[i] )


#define GENERIC_FUNC(SUFFIX,TYPE)					\
  void									\
  interpolation_init_full##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
				  size_t n, TYPE y[n], TYPE x[n],	\
				  size_t index[n])			\
  {									\
    inter->n = n;							\
    inter->y = y;							\
    inter->x = x;							\
    inter->index = index;						\
    inter->index_allocated = 0;						\
  }									\
									\
  void									\
  interpolation_init##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
			     size_t n, TYPE y[n], TYPE x[n])		\
  {									\
    size_t *index = malloc(n * sizeof(size_t));				\
    sort_q_indirect(index, x, n, sizeof(TYPE),				\
		    sort_compar_double##SUFFIX, NULL);			\
    interpolation_init_full##SUFFIX(inter, n, y, x, index);		\
    inter->index_allocated = 1;						\
  }									\
									\
  void									\
  interpolation_destroy##SUFFIX(struct interpolation_linear##SUFFIX *inter) \
  {									\
    (void)inter;							\
    if (inter->index_allocated)						\
      free(inter->index);						\
  }									\
									\
  TYPE									\
  interpolation_out_of_domain_linear##SUFFIX(struct interpolation_linear##SUFFIX *inter, TYPE x, void *cls) \
  {									\
    struct interpolation_min_max##SUFFIX *mm = cls;			\
    size_t l = 0, r = inter->n;						\
    TYPE x0, y0, x1, y1;						\
    TYPE result;							\
    									\
    if ( x < inter->x[ I_INDEX(inter, 0) ] )				\
      {									\
	l = 0;								\
	r = 1;								\
      }									\
    else if ( x >= inter->x[ I_INDEX(inter, inter->n - 1) ] )		\
      {									\
	l = inter->n - 2;						\
	r = inter->n - 1;						\
      }									\
    									\
    x0 = inter->x[ I_INDEX(inter, l) ];					\
    y0 = inter->y[ I_INDEX(inter, l) ];					\
    x1 = inter->x[ I_INDEX(inter, r) ];					\
    y1 = inter->y[ I_INDEX(inter, r) ];					\
    									\
    result = (y1 - y0) / (x1 - x0) * (x - x0) + y0;			\
									\
    if ( NULL != mm )							\
      {									\
	if (result < mm->min)						\
	  result = mm->min;						\
									\
	if (result > mm->max)						\
	  result = mm->max;						\
      }									\
									\
    return result;							\
  }									\
									\
  TYPE									\
  interpolation_linear_f##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
				 TYPE x,				\
				 TYPE (*out_of_domain)(struct interpolation_linear##SUFFIX *inter, TYPE x, void *cls), \
				 void *cls)				\
  {									\
    size_t l = 0, r = inter->n;						\
    TYPE x0, y0, x1, y1;						\
    TYPE smallest = inter->x[ I_INDEX(inter, 0) ];			\
    TYPE biggest = inter->x[ I_INDEX(inter, inter->n - 1) ];		\
    int lt =  (x < smallest);						\
    int gte = (x >= biggest);						\
    									\
    if ( lt || gte )							\
      {									\
	if (NULL == out_of_domain)					\
	  return interpolation_out_of_domain_linear##SUFFIX(inter, x, cls); \
	 								\
	return out_of_domain(inter, x, cls);				\
      }									\
									\
    while (1)								\
      {									\
	size_t mid = (l + r) / 2;					\
	    								\
	if ( x < inter->x[mid] )					\
	  r = mid;							\
	else if (l != mid)						\
	  l = mid;							\
	else								\
	  break;							\
      }									\
    									\
    x0 = inter->x[ I_INDEX(inter, l) ];					\
    y0 = inter->y[ I_INDEX(inter, l) ];					\
    x1 = inter->x[ I_INDEX(inter, r) ];					\
    y1 = inter->y[ I_INDEX(inter, r) ];					\
    									\
    return (y1 - y0) / (x1 - x0) * (x - x0) + y0;			\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
