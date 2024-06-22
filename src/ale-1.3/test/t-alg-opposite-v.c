#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/math.h"



int
main(int argc, char *argv[argc])
{
  const size_t n = 3;
  double x[] =  {-1, 2, -2};
  double res[n];
  const double eps = 0.0000001;


  
  alg_opposite_v(n, x, res);
  for (size_t i = 0 ; i < n ; i++)
    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res[i], -x[i], eps),
			  "FAIL: alg_opposite_v(), res[%zu] = %f != %f\n",
			  i, res[i], -x[i]);
  

  return EXIT_SUCCESS;
}
