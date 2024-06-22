#include <stdlib.h>
#include <math.h>

#include <string.h>
#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"

const double eps = 0.0000001;


void
test(size_t m, size_t n, double A[m][n], size_t exp_perm[m], size_t exp_s)
{
  double res_A[m][n];
  double exp_A[m][n];
  size_t perm[m], s;
  int ret;

  memcpy(exp_A, A, sizeof(exp_A));
  
  printf("\nA=\n");
  print_m(m, n, A);
  
  ret = alg_LUP_pivoting(m, n, A, perm, &s);
  ERROR_UNDEF_FATAL_FMT(ret != 0, "FAIL: alg_LUP_pivoting() returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(s != exp_s, "FAIL: alg_LUP_pivoting() %zu != s = %zu\n", exp_s, s);

  for (size_t i = 0 ; i < m ; i++)
    ERROR_UNDEF_FATAL_FMT(exp_perm[i] != perm[i], "FAIL: alg_LUP_pivoting() %zu != exp_perm[%zu] = %zu\n", perm[i], i, exp_perm[i]);
  
  printf("\nLU =\n");
  print_m(m, n, A);


  printf("\nperm = %2zu", perm[0]);
  for (size_t i = 1 ; i < m ; i++)
    printf(" %2zu", perm[i]);
  printf("\n");

  /* multiply L and U */
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	res_A[perm[i]][j] = 0;
	for (size_t k = 0 ; k < i+1 && k < j+1 ; k++)
	  {
	    double l = (i == k) ? 1 : A[perm[i]][k];
	    double u = A[perm[k]][j];
	    res_A[perm[i]][j] += l * u;
	  }
      }
  
  printf("\nres_A=\n");
  print_m(m, n, res_A);

  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(exp_A[i][j], res_A[i][j], eps),
			      "FAIL: alg_LUP_pivoting() %f != res_A[perm[%zu]][%zu] = %f\n",
			      exp_A[i][j], i, j, res_A[i][j]);
      }
}

int
main(int argc, char *argv[argc])
{
  const size_t m = 4, n = 3;
  double A[m][n];
  size_t exp_perm[m], exp_s, s, perm[m];
  int ret;

  ALG_INIT_V(m, exp_perm, i);
  ALG_INIT_M(m, n, A,  (i == j) ? 2 : 1);
  exp_s = 0;

  test(m, n, A, exp_perm, exp_s);

  printf("\n==================\n");

  ALG_INIT_V(m, exp_perm, m - i - 1);
  ALG_INIT_M(m, n, A, (exp_perm[i] == j) ? 2 : 1);
  exp_s = 2;

  test(m, n, A, exp_perm, exp_s);
  test(m, n, A, exp_perm, exp_s);

  printf("\n==================\n");

  ALG_INIT_M(n, n, A, (i == j) ? 2 : 1);
  for (size_t j = 0 ; j < n ; j++)
    A[1][j] = A[0][j];

  printf("\nA=\n");
  print_m(n, n, A);

  ret = alg_LUP_pivoting(n, n, A, perm, &s);
  ERROR_UNDEF_FATAL_FMT(ret == 0, "FAIL: alg_LUP_pivoting() returned %d for singular matrix\n", ret);


  return EXIT_SUCCESS;
}
