#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/stringutils.h"

int
main(int argc, char *argv[argc])
{
  char str[] = "PloplP";
  char exp[] = "ploplp";

  strtolower(str, str);
  ERROR_FATAL_FMT(0 != strcmp(str, exp), "FAIL: strtolower() %s != %s\n", str, exp);
  
  return EXIT_SUCCESS;
}
