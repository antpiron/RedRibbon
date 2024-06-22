#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <float.h>

#include <ale.h>
#include "cRedRibbon.h"

int
main(int argc, char *argv[argc])
{
#define ITER (100000000)
  double res = 0;
  long double resl = 0;
  double diff, rate;
  struct timespec st, et;

  printf("%s: \n", argv[0]);
  printf("sizeof(double)      = %2lu,  DBL_MIN = %e\n", sizeof(double), DBL_MIN);
  printf("sizeof(long double) = %2lu, LDBL_MIN = %Le \n", sizeof(long double), LDBL_MIN);

  clock_gettime(CLOCK_REALTIME, &st);

  res = 0;
  for (size_t i = 0 ; i < ITER ; i++)
    {
      res += 11.0;
      res /= 3.0;
    }
      
  clock_gettime(CLOCK_REALTIME, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9d;
  rate = ITER / diff ;
  
  printf("     double %12.2F sec, %12.0F operation/sec (%e)\n", diff, rate, res);
  
  clock_gettime(CLOCK_REALTIME, &st);

  resl = 0;
  for (size_t i = 0 ; i < ITER ; i++)
    {
      resl += 11.0;
      resl /= 3.0;
    }
      
  clock_gettime(CLOCK_REALTIME, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = ITER / diff ;
  
  printf("long double %12.2F sec, %12.0F operation/sec (%Le)\n", diff, rate, resl);

 
  return EXIT_SUCCESS;
}
