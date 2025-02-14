#ifndef __STACK_H
#define __STACK_H

#include <ale/error.h>
#include <ale/vector.h>

enum { ERROR_STACK_IS_EMPTY = 1 };

#define STACK_INIT(name,type)				\
  STACK_INIT_FULL(name,type,VECTOR_DEFAULT_SIZE)

#define STACK_INIT_FULL(name,type,initsize)				\
  VECTOR_INIT_FULL(name##_stack,type,initsize)				\
									\
  struct stack_##name							\
  {									\
    size_t n;								\
    struct vector_##name##_stack vector;				\
  };									\
  									\
  static inline int							\
  stack_##name##_init_size(struct stack_##name *stack, size_t size)	\
  {									\
    stack->n = 0;							\
									\
    return vector_##name##_stack_init_size(&stack->vector, size);	\
  }								 	\
  static inline int							\
  stack_##name##_init(struct stack_##name *stack)			\
  {									\
    return stack_##name##_init_size(stack, (initsize));			\
  }									\
									\
  static inline int							\
  stack_##name##_init_copy(struct stack_##name *dest,			\
			   const struct stack_##name *src)		\
  {									\
    dest->n = src->n;							\
    									\
    return vector_##name##_stack_init_copy(&dest->vector, &src->vector); \
  }									\
									\
  static inline void							\
  stack_##name##_destroy(struct stack_##name *stack)			\
  {									\
    vector_##name##_stack_destroy(&stack->vector);			\
  }									\
									\
  static inline int							\
  stack_##name##_isempty(struct stack_##name *stack)			\
  {									\
    return 0 == stack->n;						\
  }									\
									\
  static inline size_t							\
  stack_##name##_n(struct stack_##name *stack)				\
  {									\
    return stack->n;							\
  }									\
  									\
  static inline void							\
  stack_##name##_set(struct stack_##name *stack, size_t pos, type value) \
  {									\
    vector_##name##_stack_set(&stack->vector, pos, value);		\
  }									\
  									\
  static inline type							\
  stack_##name##_get(const struct stack_##name *stack, size_t pos)	\
  {									\
    return vector_##name##_stack_get(&stack->vector, pos);		\
  }									\
									\
  static inline type*							\
  stack_##name##_get_ptr(struct stack_##name *stack, size_t pos)	\
  {									\
    return vector_##name##_stack_get_ptr(&stack->vector, pos);		\
  }									\
  									\
  static inline void							\
  stack_##name##_copy(struct stack_##name *dest,			\
		       const struct stack_##name *src)			\
  {									\
    dest->n = src->n;							\
    return vector_##name##_stack_copy(&dest->vector, &src->vector);	\
  }									\
									\
  static inline void							\
  stack_##name##_push(struct stack_##name *stack, type value)		\
  {									\
    vector_##name##_stack_set(&stack->vector, stack->n++, value);	\
  }									\
									\
  static inline type							\
  stack_##name##_top(struct stack_##name *stack)			\
  {									\
    ERROR_CUSTOM_FATAL(stack_##name##_isempty(stack),			\
		       ERROR_STACK_IS_EMPTY,				\
		       "stack_"#name"_top() called on an empty stack\n"); \
    return vector_##name##_stack_get(&stack->vector, stack->n - 1);	\
  }									\
									\
  static inline type*							\
  stack_##name##_top_ptr(struct stack_##name *stack)			\
  {									\
    ERROR_CUSTOM_RET(stack_##name##_isempty(stack),			\
		     ERROR_STACK_IS_EMPTY, NULL);			\
    return vector_##name##_stack_get_ptr(&stack->vector, stack->n - 1); \
  }									\
									\
  static inline type							\
  stack_##name##_pop(struct stack_##name *stack)			\
  {									\
    ERROR_CUSTOM_FATAL(stack_##name##_isempty(stack),			\
		       ERROR_STACK_IS_EMPTY,				\
		       "stack_"#name"_pop() called on an empty stack\n"); \
    return vector_##name##_stack_get(&stack->vector, --stack->n);	\
  }									\
									\
  static inline type*							\
  stack_##name##_pop_ptr(struct stack_##name *stack)			\
  {									\
    ERROR_CUSTOM_RET(stack_##name##_isempty(stack),			\
		     ERROR_STACK_IS_EMPTY, NULL);			\
    return vector_##name##_stack_get_ptr(&stack->vector, --stack->n);	\
  }



#endif
