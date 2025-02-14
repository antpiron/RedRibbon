#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#include "ale/error.h"
#include "ale/btree.h"


int
main(int argc, char *argv[argc])
{
  struct btnode *node = bt_mknode(128, BT_NODE_LEAF);

  ERROR_FATAL(NULL == node, "FAIL: bt_mknode()");
  ERROR_FATAL(! BT_ISLEAF(*node), "FAIL: node is not a leaf");
  ERROR_FATAL_FMT(0 != node->order, "FAIL: node order is %d != 0", node->order);
 
  bt_freenode(node);

  node = bt_mknode(256, BT_NODE_INTERNAL);

  ERROR_FATAL(NULL == node, "FAIL: bt_mknode()");
  ERROR_FATAL(! BT_ISINTERNAL(*node), "FAIL: node is not an internal node");
  ERROR_FATAL_FMT(0 != node->order, "FAIL: node order is %d != 0", node->order);
 
  bt_freenode(node); 

  return EXIT_SUCCESS;
}
