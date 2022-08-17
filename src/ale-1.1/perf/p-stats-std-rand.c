#include <stdlib.h>
#include <time.h>

#include "ale/error.h"
#include "ale/stats.h"

#define MAX_INSERT (1 << 22)

int
main(int argc, char *argv[argc])
{
  double res = 0;
  double diff, rate;
  struct timespec st, et;


  printf("%s: ", argv[0]);
  
  clock_gettime(CLOCK_MONOTONIC, &st);
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      res += stats_unif_std_rand();
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = MAX_INSERT / (diff * 1000 * 1000);
        
  printf("\nGenerated %12.2F MNumbers/sec, mean = %6.3f\n", rate, res / MAX_INSERT);

  
  return EXIT_SUCCESS;
}
