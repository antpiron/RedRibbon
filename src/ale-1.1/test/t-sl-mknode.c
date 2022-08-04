#include <stdlib.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/sl_list.h"

#define ORDER 16

int
main(int argc, char *argv[argc])
{
  struct sl_node *node;
  int val1 = 1, val2 = 2, res;

  node = sl_mknode(&val1, NULL);
  ERROR_FATAL(NULL == node, "FAIL: sl_mknode()\n");
  res = *(int*) node->data;
  ERROR_UNDEF_FATAL_FMT( 1 != res, "FAIL: %d != 1\n", res);
  ERROR_UNDEF_FATAL( NULL != node->next, "FAIL: node->next != NULL\n");

  node = sl_mknode(&val2, node);
  ERROR_FATAL(NULL == node, "FAIL: sl_mknode()\n");
  res = *(int*) node->data;
  ERROR_UNDEF_FATAL_FMT( 2 != res, "FAIL: %d != 2\n", res);
  res = *(int*) node->next->data;
  ERROR_UNDEF_FATAL_FMT( 1 != res, "FAIL: %d != 1\n", res);

  
  return EXIT_SUCCESS;
}
