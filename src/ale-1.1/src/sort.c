#define _GNU_SOURCE
#include <stdlib.h>

#include "ale/sort.h"
#include <stdio.h>

struct indirect_closure
{
  size_t size;
  void *base;
  void *arg;
  int (*compar)(const void *, const void *, void *);
};

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  inline int								\
  sort_compar_double##SUFFIX(const void *p1, const void *p2, void *arg)	\
  {									\
    const TYPE *a = p1;							\
    const TYPE *b = p2;							\
    /* disable unused parameter warning */				\
    (void)(arg);							\
    									\
    if (*a < *b)							\
      return -1;							\
    									\
    if (*a > *b)							\
      return  1;							\
    									\
    return 0;								\
  }									\
  									\
  int									\
  sort_compar_double_decreasing##SUFFIX(const void *p1, const void *p2, void *arg) \
  {									\
    return - sort_compar_double##SUFFIX(p1, p2, arg);			\
  }									

GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
  
inline int
sort_compar_size_t(const void *p1, const void *p2, void *arg)
{
  const size_t *a = p1;
  const size_t *b = p2;
  // disable unused parameter warning
  (void)(arg);

  if (*a < *b)
    return -1;
  
  if (*a > *b)
    return  1;
  
  return 0;
}

inline int
sort_compar_ssize_t(const void *p1, const void *p2, void *arg)
{
  const ssize_t *a = p1;
  const ssize_t *b = p2;
  // disable unused parameter warning
  (void)(arg);

  if (*a < *b)
    return -1;
  
  if (*a > *b)
    return  1;
  
  return 0;
}


static int
indirect_compar(const void *p1, const void *p2, void *arg)
{
  struct indirect_closure *closure = arg;
  const size_t *a = p1;
  const size_t *b = p2;
  const char (*base)[closure->size] = closure->base;

  return closure->compar(base + *a , base + *b, closure->arg);
}

void
sort_q_indirect(size_t *index, void *base, size_t nmemb, size_t size,
		int (*compar)(const void *, const void *, void *),
		void *arg)
{
  struct indirect_closure closure = {.size = size, .base = base, .arg = arg, .compar = compar};
  
  for (size_t i = 0 ; i < nmemb ; i++)
    index[i] = i;
  
  qsort_r(index, nmemb, sizeof(size_t),
          indirect_compar, (void*) &closure);

}
