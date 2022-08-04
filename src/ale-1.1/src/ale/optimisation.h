#ifndef __ALE_OPTIMISATION_H
#define __ALE_OPTIMISATION_H


enum {
      OPTIM_MIN = 1,
      OPTIM_MAX
};

enum {
      ERROR_GRADIENT_DESCENT_TOO_MANY_ITER = 1,
      ERROR_GRADIENT_DESCENT_DIVISION_BY_ZERO,
      ERROR_GRADIENT_DESCENT_NO_CONVERGENCE
};

#define OPTIMISATION_GENERIC_HEADERS(SUFFIX,TYPE)			\
  int optimisation_gradient_descent##SUFFIX(size_t n, TYPE x[n], int direction,	\
					    TYPE (*f)(TYPE *x, void *cls), \
					    void (*gradf)(TYPE *y, TYPE *x, void *cls), \
					    void *cls);


OPTIMISATION_GENERIC_HEADERS(,double)
OPTIMISATION_GENERIC_HEADERS(l,long double)


#endif
