#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"



int
main(int argc, char *argv[argc])
{
  double eps = 0.000001, res, delta;

  res = stats_kolmo_F(1, 100);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_kolmo_F(1, 100) is NaN\n");
  ERROR_UNDEF_FATAL(isinf(res), "FAIL: stats_kolmo_F(1, 100) is infinite\n");
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(res, 1.0, eps), "FAIL: stats_kolmo_F(1, 100) = %e != 1.0\n", res);

  res = stats_kolmo_F(0.0, 10);
  ERROR_UNDEF_FATAL(isnan(res), "FAIL: stats_kolmo_F(0, 100) is NaN\n");
  ERROR_UNDEF_FATAL(isinf(res), "FAIL: stats_kolmo_F(0, 100) is infinite\n");
  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(res, 0.0, eps), "FAIL: stats_kolmo_F(0, 100) = %e != 0.0\n", res);



  for (unsigned long n = 100 ; n < 110 ; n++)
    {
      for (double d =  0.19 ; d <= 0.5 ; d += 0.01)
	{
	  double exp = stats_kolmo_F_approx(d, n);
	  res = stats_kolmo_F_carvalho(d, n);
	  
	  ERROR_UNDEF_FATAL_FMT(ale_cmp_double(res, exp, eps),
				"FAIL: stats_kolmo_F(%e, %ld) = %.20e != %.20e == approx()\n", d, n, res, exp);
 
	  // printf("d = %e ; n = %ld ; exp = %e ; %e\n", d, n, exp, res);
	  
	}
    }
  
  return EXIT_SUCCESS;
}
