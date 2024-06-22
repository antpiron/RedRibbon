#ifndef __EVOLUTIONARY_ALGORITHM_H
#define __EVOLUTIONARY_ALGORITHM_H

#include <ale/vector.h>
#include <ale/stats.h>
#include <stdlib.h>
#include <math.h>
#include <ale/sort.h>

int ea_indirect_compar_double(const void *p1, const void *p2, void *arg);

#define EA_INIT(name,matetype,mate_func,mutate_func,fitness_func,clstype) \
  struct ea_##name							\
  {                                                                     \
    size_t parallel;							\
    size_t pop_min_size;						\
    size_t pop_max_size;						\
    size_t pop_size;							\
    size_t *fitness_index;						\
    double *fitness;							\
    matetype *population;						\
  };                                                                    \
									\
  static inline void                                                    \
  ea_##name##_init(struct ea_##name *ea,				\
		   size_t pop_min_size, size_t pop_max_size,		\
		   matetype initial_pop[pop_max_size], clstype cls)	\
  {                                                                     \
    ea->parallel = 0;							\
    ea->pop_min_size = pop_min_size;					\
    ea->pop_max_size = pop_max_size;					\
    ea->fitness_index = malloc(pop_max_size * sizeof(size_t));		\
    ea->fitness = malloc(pop_max_size * sizeof(double));		\
    ea->population = initial_pop;					\
    for (size_t i = 0 ; i < pop_max_size ; i++)				\
      {									\
	ea->fitness[i] = fitness_func(initial_pop[i], cls);		\
      }									\
    sort_q_indirect(ea->fitness_index, ea->fitness,			\
		    ea->pop_max_size, sizeof(double), 			\
		    sort_compar_double_decreasing, NULL);		\
  }									\
									\
  static inline void                                                    \
  ea_##name##_init_parallel(struct ea_##name *ea,			\
		   size_t pop_min_size, size_t pop_max_size,		\
		   matetype initial_pop[pop_max_size], clstype cls)	\
  {									\
    ea_##name##_init(ea, pop_min_size, pop_max_size, initial_pop, cls);	\
    ea->parallel = 1;							\
  }									\
  static inline void							\
  ea_##name##_destroy(struct ea_##name *ea)				\
  {                                                                     \
    free(ea->fitness_index);						\
    free(ea->fitness);							\
  }									\
  									\
  static void								\
  ea_##name##_next_generation(struct ea_##name *ea, clstype cls)	\
  {									\
    double *cumul_fitness = malloc(ea->pop_min_size *sizeof(double));	\
    int parallel = ea->parallel;					\
    									\
  									\
    cumul_fitness[0] = ea->fitness[ea->fitness_index[0]];		\
    for (size_t i = 1 ; i < ea->pop_min_size ; i++)			\
      cumul_fitness[i] = cumul_fitness[i-1] +				\
	ea->fitness[ ea->fitness_index[i] ];				\
    for (size_t i = 0 ; i < ea->pop_min_size ; i++)			\
      cumul_fitness[i] /= cumul_fitness[ea->pop_min_size-1];		\
    _Pragma("omp parallel for if(parallel)")				\
    for (size_t i = ea->pop_min_size ; i < ea->pop_max_size ; i++)	\
    {									\
      size_t index1 = stats_categorical_rand(ea->pop_min_size,		\
					     cumul_fitness);		\
      size_t index2 = stats_categorical_rand(ea->pop_min_size,		\
					     cumul_fitness);		\
      size_t dst = ea->fitness_index[i];				\
      mate_func(&ea->population[dst],					\
		ea->population[ea->fitness_index[index1]],		\
		ea->population[ea->fitness_index[index2]], cls);	\
      mutate_func(&ea->population[dst], cls);				\
      ea->fitness[dst] = fitness_func(ea->population[dst], cls);	\
    }									\
    free(cumul_fitness);						\
    sort_q_indirect(ea->fitness_index, ea->fitness,			\
		    ea->pop_max_size, sizeof(double),			\
		    sort_compar_double_decreasing, NULL);		\
  }

#endif
