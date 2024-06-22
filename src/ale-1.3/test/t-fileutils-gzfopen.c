#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/fileutils.h"
#include "ale/stringutils.h"

#define GZFILE "plop.gz"

int
main(int argc, char *argv[argc])
{
  FILE *out;
  struct string string;

  out = gzfopen(GZFILE, "r");
  ERROR_FATAL(NULL == out, "FAIL: process_popenvp()");

  string_init(&string);
  string_readline(&string, out);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("plop!", string.str),
			 "FATAL: process_popenvp() %s != 'plop!'\n", string.str);

  fclose(out);
  return EXIT_SUCCESS;
}
