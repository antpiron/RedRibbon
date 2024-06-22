#include <string.h>

#include "ale/algebra.h"
#include "ale/optimisation.h"
#include "ale/memory.h"
#include "ale/math.h"
#include "ale/error.h"


#define MAX_ITER (1ll << 12)

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  /* https://en.wikipedia.org/wiki/Gradient_descent */			\
  int optimisation_gradient_descent##SUFFIX(size_t n, TYPE x[n], int direction,	\
					    TYPE (*f)(TYPE *x, void *cls), \
					    void (*gradf)(TYPE *y, TYPE *x, void *cls), \
					    void *cls)			\
  {									\
    (void) (f);								\
    struct mem_pool pool;						\
    mem_init(&pool);							\
    TYPE *p = mem_malloc(&pool, sizeof(TYPE) * n);			\
    TYPE *g = mem_malloc(&pool, sizeof(TYPE) * n);			\
    TYPE *delta_x = mem_malloc(&pool, sizeof(TYPE) * n);		\
    TYPE *delta_grad = mem_malloc(&pool, sizeof(TYPE) * n);		\
    TYPE alpha = 0.5;							\
    TYPE denom;								\
    									\
    gradf(g, x, cls);							\
    									\
    for (size_t iter = 0 ; ; iter++)					\
      {									\
	if (OPTIM_MIN == direction)					\
	  alg_opposite_v##SUFFIX(n, g, p);				\
	else								\
	  memcpy(p, g, sizeof(TYPE) * n);				\
	alg_mul_v_c##SUFFIX(n, p, alpha, p);				\
									\
	if ( alg_norm##SUFFIX(n, p) <= ALE_EPS##SUFFIX * 100 )		\
	  break;							\
	memcpy(delta_x, x, sizeof(TYPE) * n);				\
	alg_add_v_v##SUFFIX(n, x, p, x);				\
	alg_sub_v_v##SUFFIX(n, x, delta_x, delta_x);			\
									\
	memcpy(delta_grad, g, sizeof(TYPE) * n);			\
	gradf(g, x, cls);						\
	alg_sub_v_v##SUFFIX(n, g, delta_grad, delta_grad);		\
	denom = alg_dot##SUFFIX(n, delta_grad, delta_grad);		\
	if (0 != denom)							\
	  alpha = fabs##SUFFIX(alg_dot##SUFFIX(n, delta_x, delta_grad)) / denom; \
	ERROR_CUSTOM_GOTO(iter >= MAX_ITER, ERROR_GRADIENT_DESCENT_TOO_MANY_ITER, ERROR##SUFFIX); \
      }									\
    									\
    for (size_t i = 0 ; i < n ; i++)					\
      {									\
	ERROR_CUSTOM_GOTO(isnan(x[i]), ERROR_GRADIENT_DESCENT_NO_CONVERGENCE, ERROR##SUFFIX); \
      }									\
									\
    mem_destroy(&pool);							\
    return 0;								\
  ERROR##SUFFIX:							\
    mem_destroy(&pool);							\
    return -1;								\
}



GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)

