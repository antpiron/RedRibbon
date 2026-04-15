#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
  struct btree bt;

  ERROR_UNDEF_FATAL(-1 != bt_init(&bt, 3, btfuncs_intptr_t), "FAIL: bt_init() order too small and succeed ?!?");

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");

  ERROR_UNDEF_FATAL_FMT(ORDER != bt.order, "FAIL: order %d != %d", bt.order, ORDER);
  ERROR_UNDEF_FATAL(NULL != bt.root, "FAIL: bt.root != NULL");
  

  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
