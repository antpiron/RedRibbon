#ifndef __VECTOR_H
#define __VECTOR_H

#include <stdlib.h>
#include <inttypes.h>
#include <ale/error.h>

#define VECTOR_DEFAULT_SIZE (256)
#define VECTOR_MAX_INCREMENT (1ll << 24)

#define VECTOR_INIT(name,type)				\
  VECTOR_INIT_FULL(name,type,VECTOR_DEFAULT_SIZE)

#define VECTOR_INIT_FULL(name,type,initsize)				\
  struct vector_##name							\
  {									\
    size_t size;							\
    type *data;								\
  };									\
  									\
  static inline int							\
  vector_##name##_init_size(struct vector_##name *vector, size_t size)	\
  {									\
    vector->data = malloc(sizeof(type) * size);				\
    ERROR_UNDEF_FATAL(NULL == vector->data,				\
  		      "Unable to allocate memory in vector_init()\n");	\
    									\
    vector->size = size;						\
    return 0;								\
  }								 	\
  static inline int							\
  vector_##name##_init(struct vector_##name *vector)			\
  {									\
    return vector_##name##_init_size(vector, (initsize));		\
  }									\
  static inline int							\
  vector_##name##_init_copy(struct vector_##name *dest,			\
			    const struct vector_##name *src)		\
  {									\
    ERROR_RET(0 != vector_##name##_init_size(dest, src->size), -1);	\
    memcpy(dest->data, src->data, sizeof(type) * dest->size);		\
    return 0;								\
  }									\
									\
  static inline void							\
  vector_##name##_destroy(struct vector_##name *vector)			\
  {									\
    free(vector->data);							\
  }									\
  									\
  static inline void							\
  vector_##name##_grow(struct vector_##name *vector, size_t pos)	\
  {									\
    if (vector->size <= pos)						\
      {									\
  	const size_t newsize = ((VECTOR_DEFAULT_SIZE + pos*2) / VECTOR_DEFAULT_SIZE) * VECTOR_DEFAULT_SIZE; \
  	ERROR_UNDEF_FATAL(NULL == (vector->data = realloc(vector->data, sizeof(type) * newsize)), \
  			  "Unable to allocate memory in vector_init()\n"); \
  	vector->size = newsize;						\
      }									\
  }									\
									\
  static inline void							\
  vector_##name##_set(struct vector_##name *vector, size_t pos, type value) \
  {									\
    vector_##name##_grow(vector, pos);					\
    vector->data[pos] = value;						\
  }									\
  									\
  static inline type							\
  vector_##name##_get(const struct vector_##name *vector, size_t pos)	\
  {									\
    return vector->data[pos];						\
  }									\
									\
  static inline type*							\
  vector_##name##_get_ptr(struct vector_##name *vector, size_t pos)	\
  {									\
    vector_##name##_grow(vector, pos);					\
    return vector->data + pos;						\
  }									\
  									\
  static inline void							\
  vector_##name##_copy(struct vector_##name *dest, const struct vector_##name *src) \
  {									\
    if (src->size > dest->size)						\
      {									\
	dest->data = realloc(dest->data, sizeof(type) * src->size);	\
	ERROR_UNDEF_FATAL(NULL == dest->data,				\
			  "Unable to allocate memory in vector_init()\n"); \
	dest->size = src->size;						\
      }									\
    memcpy(dest->data, src->data, sizeof(type) * dest->size);		\
  }

#endif
