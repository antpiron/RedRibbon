#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"



int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double U[n][n], X[n][n], I[n][n];
  const double eps = 0.0000001;

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = i ; j < n ; j++)
      U[i][j] = (i == j)?2:1;

  printf("U=\n");
  print_m(n,n,U);
  
  alg_U_inverse(n, U, X);
  printf("X=\n");
  print_m(n,n,X);
  
  alg_mul_m_m(n, n, n, U, X, I);
  printf("UX=\n");
  print_m(n, n, I);
  

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0; j < n ; j++)
      {
	double delta = fabs(((i == j)?1:0) - I[i][j]);
	ERROR_UNDEF_FATAL_FMT(delta >= eps, "FAIL: alg_UX_B_solve() delta[%ld, %ld] != 0", i, j, delta);
      }

  return EXIT_SUCCESS;
}
