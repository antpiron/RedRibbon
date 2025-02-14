#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/stats.h"



int
main(int argc, char *argv[argc])
{
  size_t n = 10, p = 8;;
  int ret;
  double diff;
  struct timespec st, et;

  printf("%s:\n", argv[0]);

  for (size_t m = (1l << 16)  ; m < (1l << 21) ; m <<= 1)
    {
      double (*A)[m][n] = malloc(m*n*sizeof(double));
      double (*X)[n][p] = malloc(n*p*sizeof(double));
      double (*B)[m][p] = malloc(m*p*sizeof(double));

      
      for (size_t i = 0 ; i < m ; i++)
	for (size_t j = 0 ; j < n ; j++)
	  {
	    (*A)[i][j] = (i == j)?2:1;
	  }
      for (size_t i = 0 ; i < m ; i++)
	for (size_t j = 0 ; j < p; j++)
	  {
	    for (size_t k = 0 ; k < n ; k++)
	      (*B)[i][j] += (j+1) * (*A)[i][k];
	  }
      
      clock_gettime(CLOCK_MONOTONIC, &st);
      
      ret = alg_AX_B_OLS_solve(m, n, p, *A, *B, *X);
      ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve() ret = %d\n != 0", ret);
      
      clock_gettime(CLOCK_MONOTONIC, &et);
      diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  
      printf("Solved A[%7zu, %3zu] X[%7zu, %3zu] = B[%7zu, %3zu] in %6.3f seconds\n", m, n, n, p, m, p, diff);

      free(A);
      free(X);
      free(B);
    }
  
  return EXIT_SUCCESS;
}
