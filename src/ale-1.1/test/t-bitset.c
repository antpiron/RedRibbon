#include <stdlib.h>
#include <stdio.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/bitset.h"
#include "ale/stats.h"

#define NBRBIT (2043)

int
main(int argc, char *argv[argc])
{
  struct bitset bs;
  struct bitset a, b;
  size_t ones;
  
  ERROR_FATAL(bitset_init(&bs,NBRBIT) < 0, "FAIL: Init failed\n");
  
  for (int i = 0 ; i < (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL: bitset_init() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);

  bitset_set(&bs, 57);
  ERROR_FATAL_FMT((1ull << 57) != bs.buf[0], "FAIL: bitset_set() %"PRIx64" != %llx\n", bs.buf[0], (1ull << 57));
  ERROR_FATAL(! bitset_isset(&bs, 57), "FAIL: bitset_set() isset(57) != 1");
  for (int i = 1 ; i < (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL:  bitset_set() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);

  bitset_unset(&bs, 57);
  ERROR_FATAL(bitset_isset(&bs, 57), "FAIL: bitset_unset() isset(57) != 0");
  for (int i = 0 ; i < (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL:  bitset_unset() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);

  bitset_setrange(&bs, 8, 48);
  ERROR_FATAL_FMT(0x00FFFFFFFFFFFF00ull != bs.buf[0], "FAIL: bitset_setrange() %"PRIx64" != 0x00FFFFFFFFFFFF00", bs.buf[0]);
  for (int i = 1 ; i < (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL: bitset_setrange() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);

  bitset_setrange(&bs, 73, 553);
  for (int i = 73 ; i < 73+553 ; i++)
    ERROR_FATAL_FMT(! bitset_isset(&bs, i), "FAIL: setrange() isset(%d) != 1", i);
  
  
  bitset_set(&bs, 73+553);
  bitset_set(&bs, 72);
  bitset_unsetrange(&bs, 73,553);
  for (int i = 73 ; i < 73+553 ; i++)
    ERROR_FATAL_FMT(bitset_isset(&bs, i), "FAIL: unsetrange() isset(%d) != 0\n", i);
  ERROR_FATAL_FMT(! bitset_isset(&bs, 73+553), "FAIL: unsetrange() isset(%d) != 1\n", 73+553);
  ERROR_FATAL_FMT(! bitset_isset(&bs, 72), "FAIL: unsetrange() isset(%d) != 1\n", 72);

  bitset_reset(&bs);
  for (int i = 0 ; i <  (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL:  bitset_reset() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);
    
  ERROR_FATAL(0 == bitset_isempty(&bs), "FAIL: bitset_isempty() do not say empty\n");
  bitset_set(&bs, 1201);
  ERROR_FATAL(bitset_isempty(&bs), "FAIL: bitset_isempty() say empty\n");

  bitset_reset(&bs);
  ERROR_FATAL(bitset_init(&a,NBRBIT) < 0, "FAIL: Init failed\n");
  ERROR_FATAL(bitset_init(&b,NBRBIT) < 0, "FAIL: Init failed\n");
  bitset_setrange(&a, 0,553);
  bitset_setrange(&b, 1,552);
  bitset_xor(&bs, &a, &b);
  ERROR_FATAL_FMT(0x1ull != bs.buf[0], "FAIL: bitset_xor() %"PRIx64" != 0x1", bs.buf[0]);
  bitset_unset(&bs, 0);
  for (int i = 0 ; i <  (NBRBIT + 63) / 64 ; i++)
    ERROR_FATAL_FMT(bs.buf[i], "FAIL:  bitset_xor() bs.buf[%d] = %"PRIx64" != 0\n", i, bs.buf[i]);
  ERROR_FATAL(0 == bitset_isempty(&bs), "FAIL: bitset_isempty() do not say empty\n");
  

  bitset_reset(&bs);
  bitset_setrange(&bs, 73,553);
  ones = bitset_ones(&bs);
  ERROR_UNDEF_FATAL_FMT(553 != ones, "FAIL: %zu != 553\n", ones);

  bitset_set(&bs, NBRBIT + 10);
  ERROR_FATAL_FMT(2054 != bs.n, "FAIL: bitset_set() n = %zu != 2054", bs.n);
  ERROR_FATAL_FMT(65 != bs.alloc_size,
		  "FAIL: bitset_set() alloc_size = %zu != %d",  bs.alloc_size, 65);
  ERROR_FATAL(! bitset_isset(&bs, NBRBIT + 10), "FAIL: bitset_set() isset(2052) != 1");

  bitset_reset(&bs);
  bitset_reset(&a);
  
  bitset_set(&a, 1201);

  bitset_cpy(&bs, &a);
  ERROR_FATAL_FMT(a.n != bs.n, "FAIL: bitset_cpy() a.n = %zu != %zu = bs.n", a.n, bs.n);
  for (int i = 0 ; i < (a.n + 63) / 64 ; i++)
    ERROR_FATAL_FMT(a.buf[i] != bs.buf[i], "FAIL: bitset_cpy() a.buf[%zu] = %" PRId64 " != %" PRId64 " = bs.buf[%zu]\n", i, a.buf[i], bs.buf[i], i);

  bitset_destroy(&bs);
  bitset_destroy(&a);
  bitset_destroy(&b);

  ERROR_FATAL(bitset_init(&bs,1024) < 0, "FAIL: Init failed\n");
  ERROR_FATAL(bitset_init(&a,1024) < 0, "FAIL: Init failed\n");

  bitset_setrange(&a, 1024-32, 32);
  size_t last = (a.n-1) / 64;
  // printf("a[last] = %#018" PRIx64 "\n", a.buf[last]);
  bitset_not(&bs, &a);
  for (int i = 0 ; i < (a.n + 63) / 64 - 1 ; i++)
    {
      ERROR_FATAL_FMT(a.buf[i] != ~ bs.buf[i],
		      "FAIL: bitset_not() a.buf[%zu] = %" PRIx64 " != ~ %" PRIx64 " = bs.buf[%zu] = %" PRIx64 "\n", i,
		      a.buf[i], bs.buf[i],i, ~ bs.buf[i]);
    }
  uint64_t exp = 0x00000000ffffffffull;
  ERROR_FATAL_FMT(bs.buf[last] != exp,
		  "FAIL: bitset_not() bs.buf[%zu] = %#018" PRIx64 " !=  %#018" PRIx64 "\n",
		  last, bs.buf[last], exp);
  
  
  bitset_reset(&bs);
  bitset_set(&bs, 1201);
  bitset_set(&bs, 1202);
  bitset_setrange(&bs, 62, 10);
  ssize_t value = -1;
  ones = 0;
  while (1)
    {
      ssize_t old_value = value;
      value = bitset_iterate(&bs, value);
      if (value < 0)
	break;
      ERROR_FATAL_FMT(! ( ( value >= 62 && value < 72) || 1201 == value || 1202 == value ), "FAIL: bitset_iterate(%zd) = %zd\n", old_value, value);
      ones++;
    }
  ERROR_FATAL_FMT(12 != ones, "FAIL: bitset_iterate() ones = %zu != 12\n", ones);

  bitset_reset(&bs);

  size_t *vec = malloc(sizeof(size_t) * NBRBIT);
  for (size_t i = 0 ; i < NBRBIT ; i++)
    vec[i] = i;

  stats_shuffle(vec, NBRBIT, sizeof(size_t));

#define NBRSET (NBRBIT / 2)
  for (size_t i = 0 ; i < NBRSET ; i++)
    {
      ERROR_FATAL_FMT( vec[i] >= NBRBIT, "FAIL: stats_shuffle() vec[%z] = %zu >= \n", i, vec[i], NBRBIT);
      ERROR_FATAL_FMT( bitset_isset(&bs, vec[i]), "FAIL: stats_shuffle() vec[%z] is set.\n", i);
      bitset_set(&bs, vec[i]);
      ERROR_FATAL_FMT( ! bitset_isset(&bs, vec[i]), "FAIL: stats_shuffle() vec[%z] is not set.\n", i);
    }
  ones = bitset_ones(&bs);
  ERROR_FATAL_FMT(ones != NBRSET, "FAIL: bitset_iterate() bitset_ones = %z != %d\n", ones, NBRSET);
  ones = 0;
  value = -1;
  while (1)
    {
      ssize_t old_value = value;
      value = bitset_iterate(&bs, value);
      if (value < 0)
	break;
      /* ERROR_FATAL_FMT(! ( ( value >= 62 && value < 72) || 1201 == value || 1202 == value ), */
      /* 		      "FAIL: bitset_iterate(%zd) = %zd\n", old_value, value); */
      ones++;
    }
  ERROR_FATAL_FMT(NBRSET != ones, "FAIL: bitset_iterate() ones = %zu != %d\n", ones, NBRSET);

  free(vec);

  bitset_destroy(&bs);
  bitset_destroy(&a);

  return EXIT_SUCCESS;
}
