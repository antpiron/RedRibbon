#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/parser.h"
#include "ale/stringutils.h"

/*
  https://lambda.uta.edu/cse5317/notes/node18.html
  0) S ::= R $
  1) R ::= R b
  2) R ::= a
  
  state          action 	        	goto
    	|        a 	b 	$ 	| 	S 	R
  0 	| 	 s3 	  	  	|	  	1
  1 	| 	  	s4 	s2	| 	  	 
  2 	| 	 a 	a 	a	| 	  	 
  3  	|	 r2 	r2 	r2	|	  	 
  4 	| 	 r1 	r1 	r1	|
*/

enum { NT_S = 0, NT_R };
ssize_t
goto_table(size_t state, size_t lhs, void *cls)
{
  ssize_t ret = -1;

  if (0 == state && NT_R == lhs)
    return 1;
  
  return ret;
}


enum { RULE_S_R = 0, RULE_R_Rb, RULE_R_a };

struct parser_action*
action_table(size_t state, size_t token, void *cls)
{
  static struct parser_action pa = { .type = PARSER_ACTION_ERROR  };
  
  if ( 0 == state && 'a' == token )
    {
      pa.type = PARSER_ACTION_SHIFT;
      pa.shift.next_state = 3;
    }

  if ( 1 == state )
    {
      if ( 'b' == token )
	{
	  pa.type = PARSER_ACTION_SHIFT;
	  pa.shift.next_state = 4;
	}
      if ( '\0' == token )
	{
	  pa.type = PARSER_ACTION_SHIFT;
	  pa.shift.next_state = 2;
	}
    }

  if ( 2 == state )
    pa.type = PARSER_ACTION_ACCEPT;

  if ('a' == token || 'b' == token || '\0' == token)
    {
      
      if ( 3 == state )
	{
	  pa.type = PARSER_ACTION_REDUCE;
	  pa.reduce.lhs = NT_R; // RULE_R_a;
	  pa.reduce.rhs_n = 1;
	  pa.reduce.callback = NULL;
	}
      
      if ( 4 == state )
	{
	  pa.type = PARSER_ACTION_REDUCE;
	  pa.reduce.lhs = NT_R; // RULE_R_R_b;
	  pa.reduce.rhs_n = 2;
	  pa.reduce.callback = NULL;
	}
    }
  
  return &pa;
}

struct input
{
  size_t pos;
  char *str;
};

struct lexer_token*
next_token(size_t state, void *cls)
{
  static char str[2] = "a";
  static struct lexer_token tok;
  struct input *in = cls;

  tok.str = str;

  // printf("%zu: %c\n", in->pos,  in->str[in->pos]);
  switch (in->str[in->pos])
    {
      
    case '\0':
      tok.id = 0;
      tok.str[0] = 0; 
      break;

    default:
      tok.id = tok.str[0] = in->str[in->pos++];
      break;      
    }

  return &tok;
}

int
main(int argc, char *argv[argc])
{
  struct input in = { .pos = 0, .str = "abbbbbbbbbbbbbb" };
  struct input in_error = { .pos = 0, .str = "abbbbbcbbbbbbbbb" };
  struct parser_shift_reduce sr;
  void *value;
  int ret;

  parser_shift_reduce_init(&sr, goto_table, action_table, next_token);

  ret = parser_shift_reduce(&sr, value, &in);
  ERROR_FATAL_FMT(0 != ret, "parser_shift_reduce() returned %d != 0\n", ret);

  ret = parser_shift_reduce(&sr, value, &in_error);
  ERROR_FATAL_FMT(0 == ret, "parser_shift_reduce() returned %d instead of error\n", ret);
 
  parser_shift_reduce_destroy(&sr);

  
  return EXIT_SUCCESS;
}
