#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"


int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double A[n][n], L[n][n], Lt[n][n], prod[n][n];
  int ret;
  const double eps = 0.0000001;

  /* https://en.wikipedia.org/wiki/Diagonally_dominant_matrix condition*/
  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	A[i][j] = (i == j)?3:1;
	L[i][j] = 0;
      }

  ret = alg_LLt_cholesky(n, A, L);
  ERROR_FATAL_FMT(0 != ret, "FAIL: alg_LLt_cholesky() == %d != 0", ret);

  alg_transpose(n, n, L, Lt);

  alg_mul_m_m(n, n, n, L, Lt, prod);

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0; j < n ; j++)
      {
	ERROR_UNDEF_FATAL_FMT(i == j && 0 != ale_cmp_double(prod[i][j], A[i][j], eps),
			      "FAIL: (L * L^t)[%zu, %zu] == %f != %f\n", i, j, prod[i][j], A[i][j]);
	ERROR_UNDEF_FATAL_FMT(i != j && 0 != ale_cmp_double(prod[i][j], A[i][j], eps),
			      "FAIL: (L * L^t)[%zu, %zu] == %f != %f\n", i, j, prod[i][j], A[i][j]);
      }
    
 
 return EXIT_SUCCESS;
}
