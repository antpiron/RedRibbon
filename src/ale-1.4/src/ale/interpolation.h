#ifndef __ALE_INTERPOLATION_H
#define __ALE_INTERPOLATION_H


#define INTERPOLATION_GENERIC_HEADERS(SUFFIX,TYPE)			\
    struct interpolation_linear##SUFFIX					\
    {									\
      size_t n;								\
      TYPE *x;								\
      TYPE *y;								\
      int index_allocated;						\
      size_t *index;							\
    };									\
									\
    struct interpolation_min_max##SUFFIX				\
    {									\
      TYPE min, max;							\
    };									\
									\
    void interpolation_init_full##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
					 size_t n, TYPE y[n], TYPE x[n], size_t index[n]); \
    void interpolation_init##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
				    size_t n, TYPE y[n], TYPE x[n]);	\
    void interpolation_destroy##SUFFIX(struct interpolation_linear##SUFFIX *inter); \
    TYPE interpolation_out_of_domain_linear##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
						    TYPE x, void *cls); \
    TYPE interpolation_linear_f##SUFFIX(struct interpolation_linear##SUFFIX *inter, \
					TYPE x,				\
					TYPE (*out_of_domain)(struct interpolation_linear##SUFFIX *inter, TYPE x, void *cls), \
					void *cls);

    

INTERPOLATION_GENERIC_HEADERS(,double)
INTERPOLATION_GENERIC_HEADERS(l,long double)


#endif
