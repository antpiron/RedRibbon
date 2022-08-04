#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"



int
main(int argc, char *argv[argc])
{
  const size_t m = 3, n = 2;
  double x[m][n];
  double res[n][m];
  const double eps = 0.0000001;

  size_t count = 0;
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      x[i][j] = count++;
      
  
  alg_transpose(m, n, x, res);
  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[j][i], x[i][j], eps),
			      "FAIL: alg_transpose(), res[%zu][%zu] = %f != %f\n",
			      j, i,  res[j][i], x[i][i]);
      }
  

  return EXIT_SUCCESS;
}
