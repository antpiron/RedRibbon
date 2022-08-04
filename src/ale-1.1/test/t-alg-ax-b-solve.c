#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double A[n][n],  X[n][n], B[n][n];
  double A_copy[n][n], B_copy[n][n];
  double delta;
  const double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	A_copy[i][j] = A[i][j] = (i == j)?2:1;
	B_copy[i][j] = B[i][j] = i*n+j;
      }
  
  printf("\nA=\n");
  print_m(n, n, A);
  printf("\nB=\n");
  print_m(n, n, B);
  alg_AX_B_solve(n, n, A, B, X);
  printf("\nX=\n");
  print_m(n, n, X);

  alg_mul_m_m(n,n,n, A_copy, X, B);
  printf("\nB=\n");
  print_m(n, n, B);


  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0; j < n ; j++)
    {
      
      delta = fabs(B[i][j] - B_copy[i][j]);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_AX_B_solve() delta != C[%ld]*C[%ld] = %f\n", i, j, delta);
    }



  return EXIT_SUCCESS;
}
