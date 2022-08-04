#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/process.h"
#include "ale/stringutils.h"


int
main(int argc, char *argv[argc])
{
  FILE *out;
  struct string string;

  out = process_popenvp("echo", (char* []){"echo", "-n", "plop!", NULL} );
  ERROR_FATAL(NULL == out, "FAIL: process_popenvp()");

  string_init(&string);
  string_readline(&string, out);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("plop!", string.str),
			 "FATAL: process_popenvp() %s != 'plop!'\n", string.str);

  fclose(out);

  out = process_popenp("echo", "echo", "-n", "plop!", NULL );
  ERROR_FATAL(NULL == out, "FAIL: process_popenp()");

  string_init(&string);
  string_readline(&string, out);
  ERROR_UNDEF_FATAL_FMT( 0 != strcmp("plop!", string.str),
			 "FATAL: process_popenp() %s != 'plop!'\n", string.str);

  fclose(out);
  return EXIT_SUCCESS;
}
