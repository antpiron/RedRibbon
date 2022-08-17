#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/stringutils.h"


int
main(int argc, char *argv[argc])
{
  struct string dst;
  
  string_init(&dst);
    
  char *next = string_split(&dst, "aa,bb,cc", ",", 0);
  ERROR_UNDEF_FATAL(next == NULL, "FATAL: string_split() returned NULL!\n");
  ERROR_UNDEF_FATAL_FMT(*next != 'b', "FATAL: string_split() *next == %c != 'b' !\n", *next);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(dst.str, "aa"),
			"FATAL: string_split() str == %s != 'aa' \n", dst.str);

  next = string_split(&dst, next, ",", 0);
  ERROR_UNDEF_FATAL(next == NULL, "FATAL: string_split() returned NULL!\n");
  ERROR_UNDEF_FATAL_FMT(*next != 'c', "FATAL: string_split() *next == %c != 'c' !\n", *next);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(dst.str, "bb"),
			"FATAL: string_split() str == %s != 'bb' \n", dst.str);

  next = string_split(&dst, next, ",", 0);
  ERROR_UNDEF_FATAL(next == NULL, "FATAL: string_split() returned NULL!\n");
  ERROR_UNDEF_FATAL_FMT(*next != '\0', "FATAL: string_split() *next == %c != '\\0' !\n", *next);
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(dst.str, "cc"),
			"FATAL: string_split() str == %s != 'cc' \n", dst.str);

  next = string_split(&dst, next, ",", 0);
  ERROR_UNDEF_FATAL(next != NULL, "FATAL: string_split() returned not NULL!\n");

  string_destroy(&dst);
  
  return EXIT_SUCCESS;
}
