#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 8, n = 3, p = 3;;
  double A[m][n],  X[n][p], B[m][p];
  double A_copy[m][n];
  double B_copy[m][p];
  double delta;
  int ret;
  const double eps = 0.0000001;

  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	A_copy[i][j] = A[i][j] = (i == j)?2:1;
      }
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < p; j++)
      {
	double x = 0;
	for (size_t k = 0 ; k < n ; k++)
	  B_copy[i][j] = B[i][j] += (j+1)*A[i][k];
      }
  
  printf("\nA=\n");
  print_m(m, n, A);
  printf("\nB=\n");
  print_m(m, p, B);
  ret = alg_AX_B_OLS_solve(m, n, p, A, B, X);
  printf("\nA=\n");
  print_m(m, n, A);
  printf("\nB=\n");
  print_m(m, p, B);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: alg_AX_B_OLS_solve() ret = %d\n != 0", ret);
  
  printf("\nX=\n");
  print_m(n, p, X);

  alg_mul_m_m(m,n,p, A_copy, X, B);
  printf("\nB=\n");
  print_m(m, p, B);


  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0; j < p ; j++)
    {
      
      delta = fabs(B[i][j] - B_copy[i][j]);
      ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_AX_B_solve() delta != C[%ld]*C[%ld] = %f\n", i, j, delta);
    }



  return EXIT_SUCCESS;
}
