#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"


int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double A[n][n];
  double det, exp;
  const double eps = 0.0000001;
  
  ALG_INIT_M(n, n, A, (i == j)?2:0 );
	   
  det = alg_det(n, A);
  exp = pow(2, n);
 
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(det, exp ,eps), "FAIL: alg_det() == %f != %f\n", det, exp);

  ALG_INIT_M(n, n, A, (i == j)?2:0 );
  for (size_t i = 0 ; i < n ; i++)
    {
      double tmp = A[0][i];
      A[0][i] = A[1][i];
      A[1][i] = tmp;
    }

  printf("A =\n");
  print_m(n, n, A);
  
  det = alg_det(n, A);
  exp = -pow(2, n);
 
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(det, exp ,eps), "FAIL: negative alg_det() == %f != %f\n", det, exp);
 
 
  return EXIT_SUCCESS;
}
