#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/timeb.h>

#include "ale.h"
#include "rrho.h"

int
main(int argc, char *argv[argc])
{
  struct rrho rrho;
  struct rrho_result res;
  size_t n = 50000;
  double a[n], b[n];
  double diff, rate;
  struct timeb st, et;

  printf("%s: ", argv[0]);
  
  for (size_t i = 0 ; i < n ; i++)
    {
      a[i] = i; // stats_norm_rand(i, sqrt(n));
      b[i] = i; // stats_norm_rand(i, sqrt(n));
    }
  
  rrho_init(&rrho, n, a, b);
  
  
  ftime(&st);

  long step = floor(sqrt(n));
  for (size_t i = 0 ; i < n ; i += step)
    {
      for (size_t j = 0 ; j < n ; j += step)
	{
	  rrho_hyper(&rrho, i, j, &res);
	  // printf("%f,%ld\t", res.pvalue, res.count);
	}
      // printf("\n");
    }
 
  ftime(&et);
  diff = (et.time - st.time) + (et.millitm - st.millitm) / 1000.0;
  rate = n*n / diff / step / step;
  
  printf("\nrate = %12.0F rrho/sec", rate);
  
  printf("\n");
  return EXIT_SUCCESS;
}
