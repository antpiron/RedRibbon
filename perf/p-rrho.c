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
  double diff, rate;
  struct timeb st, et;

  printf("%s: \n", argv[0]);

  for ( size_t n = (1llu << 10)  ; n < (1llu << 17) + 1 ; n = n << 1)
    {
      double *a = malloc(n * sizeof(double));
      double *b = malloc(n * sizeof(double));
      double root = cbrt(n);
      long step = ceil(n / root * 0.2);
 
      printf("RRHO for vectors of n = %6zu elements, step = %4ld: ", n, step);
      
      for (size_t i = 0 ; i < n ; i++)
	{
	  a[i] = i; // stats_norm_rand(i, sqrt(n));
	  b[i] = i; // stats_norm_rand(i, sqrt(n));
	}
      
      rrho_init(&rrho, n, a, b);
      
      
      ftime(&st);
      
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
  
      printf("%12.2F sec, %12.0F rrho/sec\n", diff, rate);
  
       
      rrho_destroy(&rrho);
      free(a);
      free(b);
    }
  
  return EXIT_SUCCESS;
}
