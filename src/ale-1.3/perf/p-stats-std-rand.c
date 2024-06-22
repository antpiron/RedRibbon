#include <stdlib.h>
#include <time.h>

#include "ale/error.h"
#include "ale/stats.h"
#ifdef _OPENMP
#include <omp.h>
#endif

#define MAX_N (1 << 26)

int
main(int argc, char *argv[argc])
{
  double res = 0;
  double diff, rate;
  struct timespec st, et;
  size_t n = MAX_N;


  printf("%s: ", argv[0]);
  
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (size_t i = 0 ; i < n ; i++)
    {
      res += stats_unif_std_rand();
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = n / (diff * 1000 * 1000);
        
  printf("\nGenerated %12.2F MNumbers/sec, mean = %6.3f\n", rate, res / n);

  res = 0;
  int n_threads = omp_get_max_threads();
  n = MAX_N * n_threads;

#ifdef _OPENMP
  clock_gettime(CLOCK_MONOTONIC, &st);
#pragma omp parallel for reduction (+:res)
  for (size_t i = 0 ; i < n ; i++)
    {
      res += stats_unif_std_rand();
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = n / (diff * 1000 * 1000);
        
  printf("Generated %12.2F MNumbers/sec in parallel (n = %d), total = %.2F MNumbers/sec, mean = %6.3f\n", rate / n_threads, n_threads, rate, res / n);
#endif

  
  
  return EXIT_SUCCESS;
}
