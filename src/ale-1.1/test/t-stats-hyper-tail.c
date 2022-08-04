#include <stdlib.h>
#include <math.h>

#include "ale/error.h"
#include "ale/stats.h"
#include "ale/math.h"

int
main(int argc, char *argv[argc])
{
  double eps = 1e-5, res1, res2;

  res1 = stats_hyper_tail(1, 50, 20, 1000, STATS_UPPER);
  res2 = stats_hyper_tail(1, 20, 50, 1000, STATS_UPPER);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res1, res2, eps),
			"FAIL: UPPER stats_hyper_tail(1, 50, 20, 1000) == %.20f != %.20f = stats_hyper_F(1, 20, 50, 1000)\n",
			res1, res2);
 
  res1 = stats_hyper_tail(1, 50, 20, 1000, STATS_LOWER);
  res2 = stats_hyper_tail(1, 20, 50, 1000, STATS_LOWER);
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(res1, res2, eps),
			"FAIL: LOWER stats_hyper_tail(1, 50, 2, 1000) == %.20f != %.20f = stats_hyper_F(1, 2, 50, 1000)\n",
			res1, res2);
  
  
  
  return EXIT_SUCCESS;
}
