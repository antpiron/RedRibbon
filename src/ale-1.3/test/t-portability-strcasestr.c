#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/portability.h"

#define NUM_ITER (1l << 22)
#define EPS (0.0001)
#define BITS (8)
#define SIZE (1 << BITS)
#define SHIFT (32 - BITS)

int
main(int argc, char *argv[argc])
{

  char *needle = portability_strcasestr("plopAlT", "alt");
  ERROR_UNDEF_FATAL(NULL == needle, "FAIL: needle == NULL\n");
  ERROR_UNDEF_FATAL_FMT('A' != needle[0], "FAIL: needle[0] = %c != 'A'\n", needle[0]);

  needle = portability_strcasestr("plopAlT", "altA");
  ERROR_UNDEF_FATAL_FMT(NULL != needle, "FAIL: needle != NULL = %s\n", needle);

 

  return EXIT_SUCCESS;
}
