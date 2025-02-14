#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/stringutils.h"

int
main(int argc, char *argv[argc])
{
  char *res = strjoin("/:/", (const char *[]) {"one", "two", "three", NULL});

  ERROR_FATAL_FMT(0 != strcmp("one/:/two/:/three", res), "FAIL: strjoin() %s != %s\n", res, "one/:/two/:/three");

  free(res);
  
  return EXIT_SUCCESS;
}
