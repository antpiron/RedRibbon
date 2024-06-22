#include "config.h"

#include <stdlib.h>
#include <time.h>
#ifdef HAVE_GETRANDOM
#include <sys/random.h>
#endif

#include "ale/error.h"
#include "ale/portability.h"

#define MAX_INSERT (1 << 24)

int
main(int argc, char *argv[argc])
{
  uint64_t res = 0;
  double diff, rate;
  struct timespec st, et;
  size_t n = MAX_INSERT;


  printf("%s: ", argv[0]);
  
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (size_t i = 0 ; i < n ; i++)
    {
      portability_getrandom(&res, sizeof(res), 0);
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = MAX_INSERT / (diff * 1000 * 1000);
        
  printf("\nportability_getrandom() generated %12.2F MNumbers/sec, mean = %6.3f\n", rate, (double)res / n);

#ifdef HAVE_GETRANDOM
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (size_t i = 0 ; i < n ; i++)
    {
      getrandom(&res, sizeof(res), 0);
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = MAX_INSERT / (diff * 1000 * 1000);
        
  printf("getrandom() generated %12.2F MNumbers/sec, mean = %6.3f\n", rate, (double)res / n);
#endif

  
  return EXIT_SUCCESS;
}
