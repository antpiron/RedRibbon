#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

#include "ale/error.h"
#include "ale/math.h"
#include "ale/random.h"

#define NUM_ITER 2000000
#define EPS 0.001
#define HALF ((UINT64_MAX >> 1) + 1)


int
main(int argc, char *argv[argc])
{
  uint64_t sum = 0;
  
  
  for (unsigned long i = 0 ; i < NUM_ITER; i++)
    {
      uint64_t rand = rd_rand_u64();
      // printf("%" PRIu64 "\n", rand);
      sum += (rand < HALF);
     }

  double frac = (double) sum / (double) NUM_ITER;
  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(0.5, frac, EPS),
			"frac = %f !=  0.5\n", frac);

  rd_setseed(57);
  uint64_t rand1 = rd_rand_u64();
  rd_setseed(57);
  uint64_t rand2 = rd_rand_u64();

  ERROR_UNDEF_FATAL_FMT(rand1 != rand2,
			"%" PRIu64 " = rand1 != rand2 = %" PRIu64 "\n", rand1, rand2);

  rd_setseed(57);
  rand1 = rd_rand_u64();
  rd_setseed(58);
  rand2 = rd_rand_u64();

  ERROR_UNDEF_FATAL_FMT(rand1 == rand2,
			"%" PRIu64 " = rand1 == rand2 = %" PRIu64 "\n", rand1, rand2);

  struct rd_state state;

  rd_setseed_r(&state, 57);
  
  ERROR_UNDEF_FATAL_FMT(1 != state.counter,
			"counter == %" PRIu64 " != 1\n", state.counter);

  rand1 = rd_rand_u64_r(&state);
  rd_setseed_r(&state, 57);
  rand2 = rd_rand_u64_r(&state);

  ERROR_UNDEF_FATAL_FMT(rand1 != rand2,
			"%" PRIu64 " = rand1 != rand2 = %" PRIu64 "\n", rand1, rand2);

  rd_setseed_r(&state, 57);
  rand1 = rd_rand_u64_r(&state);
  rd_setseed_r(&state, 58);
  rand2 = rd_rand_u64_r(&state);

  ERROR_UNDEF_FATAL_FMT(rand1 == rand2,
			"%" PRIu64 " = rand1 == rand2 = %" PRIu64 "\n", rand1, rand2);


  return EXIT_SUCCESS;
}
