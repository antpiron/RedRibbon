#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"

#define ORDER 24
#define NUMBER ((intptr_t) (1 << 17))

int
main(int argc, char *argv[argc])
{
  struct btree bt;
  
  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");

  for (intptr_t i = 0 ; i < NUMBER ; i++)
    {      
      ERROR_FATAL_FMT(-1 ==  bt_insert(&bt, (void*) i, (void*) i), 
		      "FAIL: unable to insert (%"PRIdPTR " => %"PRIdPTR ")", i, i);
    }  
  
  for (intptr_t i = 0 ; i < NUMBER ; i++)
    {
      intptr_t val = (intptr_t) bt_search(&bt, (void*) i);
      ERROR_FATAL_FMT( i != val, "FAIL: unable to search %"PRIdPTR " != %"PRIdPTR, i, val);
    }

  ERROR_FATAL_FMT( NULL != bt_search(&bt, (void*) NUMBER), "FAIL: found %"PRIdPTR, NUMBER);
  
  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
