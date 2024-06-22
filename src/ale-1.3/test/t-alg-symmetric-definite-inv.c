#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"


int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double A[n][n], A_inv[n][n], prod[n][n];
  int ret;
  const double eps = 0.0000001;

  /* https://en.wikipedia.org/wiki/Diagonally_dominant_matrix condition*/
  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0 ; j < n ; j++)
      A[i][j] = (i == j)?3:1;

  ret = alg_symmetric_definite_inv(n, A, A_inv);
  ERROR_FATAL_FMT(0 != ret, "FAIL: alg_det_symmetric_inv() == %d != 0", ret);

  alg_mul_m_m(n, n, n, A, A_inv, prod);

  for (size_t i = 0 ; i < n ; i++)
    for (size_t j = 0; j < n ; j++)
      {
	ERROR_UNDEF_FATAL_FMT(i == j && 0 != ale_cmp_double(prod[i][j], 1, eps),
			      "FAIL: (A * A^-1)[%zu, %zu] == %f != 1\n", i, j, prod[i][j]);
	ERROR_UNDEF_FATAL_FMT(i != j && 0 != ale_cmp_double(prod[i][j], 0, eps),
			      "FAIL: (A * A^-1)[%zu, %zu] == %f != 1\n", i, j, prod[i][j]);
      }
    
 
 return EXIT_SUCCESS;
}
