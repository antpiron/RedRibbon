#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include "ale/error.h"
#include "ale/exparser.h"

enum {
  T_INT = EXPR_TOK_LAST
};

struct ep_token 
get_token(void *cls)
{
  static char *str = "1+  2+ 3";

  if ('\0' == *str)
    return (struct ep_token) {.token = EXPR_TOK_EOF};

  if ( isdigit(*str) )
    {
      int val;
      for (val = 0 ; *str && isdigit(*str) ; str++)
	val=val*10+(*str-'0');
      
      return (struct ep_token) {.token = T_INT, .cint = val};
    }

  return (struct ep_token) { .token = *(str++) };
}


int
main(int argc, char *argv[argc])
{
  struct ep_parser ep;

  ERROR_UNDEF_FATAL(-1 == expr_parser_init(&ep), "FAIL: expr_parser_init()");

  

  ERROR_UNDEF_FATAL(-1 == expr_parser_destroy(&ep), "FAIL: expr_parser_destroy()");

  return EXIT_SUCCESS;
}
