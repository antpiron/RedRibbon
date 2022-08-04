#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/ea.h"
#include "ale/stats.h"

struct params
{
  double prob_mutation;
  double sigma;
};

double
fitness(double x,  struct params *param)
{
  (void)param;
  double f = (x-1)*(x-2)*(x-3);
  return 1/(1+f*f);
}

void
mutate(double *x, struct params *param)
{
  double u = stats_unif_std_rand();
  if (u < param->prob_mutation)
    *x = *x + stats_norm_rand(0,param->sigma);
}


void
mate(double *x, double m1, double m2, struct params *param)
{
  (void)param;
  *x = (m1+m2)/2;
}
  
EA_INIT(optim,double,mate,mutate,fitness,struct params *);

void
check_fitness(struct ea_optim ea)
{
  size_t last_index = ea.fitness_index[0];
  for (size_t i = 1 ; i < ea.pop_max_size ; i++)
    {
      size_t index = ea.fitness_index[i];
      ERROR_UNDEF_FATAL_FMT(ea.fitness[index] > ea.fitness[last_index],
			    "FAIL: fitness is not sorted %f > %f\n",
			    ea.fitness[index], ea.fitness[last_index]);
      last_index = index;
    }
}

void
test(int parallel, size_t niter, size_t min_pop_size, size_t max_pop_size)
{
  struct params param = {.prob_mutation = 0.1, .sigma = 2.0 };
  struct ea_optim ea;
  double *population = malloc(max_pop_size * sizeof(double));
  double eps = 0.0001;

  for (size_t i = 0 ; i < max_pop_size ; i++)
    population[i] = stats_unif_rand(-1000, 1000);
  
  if (parallel)
    ea_optim_init_parallel(&ea, min_pop_size, max_pop_size, population, &param);
  else
    ea_optim_init(&ea, min_pop_size, max_pop_size, population, &param);
  ERROR_UNDEF_FATAL_FMT(min_pop_size != ea.pop_min_size,
			"FAIL: pop_min_size = %zu != %zu\n", ea.pop_min_size, min_pop_size);
  ERROR_UNDEF_FATAL_FMT(max_pop_size != ea.pop_max_size,
			"FAIL: pop_min_size = %zu != %zu\n", ea.pop_max_size, max_pop_size);
  check_fitness(ea);

  for (size_t iter = 0 ; iter < niter ; iter++)
    {
      ea_optim_next_generation(&ea, &param);
      check_fitness(ea);
      /* for (size_t i = 0 ; i < min_pop_size / 10 ; i++) */
      /* 	{ */
      /* 	  size_t index = ea.fitness_index[i]; */
      /* 	  printf("%.4f:%.4f ",  ea.fitness[index], ea.population[index]); */
      /* 	} */
      /* printf("\n"); */
    }

  size_t index0 = ea.fitness_index[0];
  double diff = fabs(ea.fitness[index0] - 1);
  ERROR_UNDEF_FATAL_FMT(diff > eps, "FAIL: fitness[0] = %f != 1", ea.fitness[index0]);
  ERROR_UNDEF_FATAL_FMT(fabs(ea.population[index0] - 1) > eps &&
			fabs(ea.population[index0] - 2) > eps &&
			fabs(ea.population[index0] - 3) > eps,
			"FAIL: fitness[0] = %f != 1|2|3", ea.population[index0]);
  
  free(population);
}


int
main(int argc, char *argv[argc])
{
  test(0, 5000, 50, 500);
  test(1, 5000, 50, 1000);
  
  return EXIT_SUCCESS;
}
