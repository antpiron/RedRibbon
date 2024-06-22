#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"

#define ORDER 17
#define LEAF_ORDER (ORDER - 1)
#define RIGHT_ORDER ((LEAF_ORDER+1) / 2)
#define LEFT_ORDER (LEAF_ORDER / 2)

int
main(int argc, char *argv[argc])
{
  struct btree bt;
  struct btnode *node = bt_mknode(ORDER, BT_NODE_LEAF);

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");
  ERROR_FATAL(NULL == node, "FAIL: bt_mknode()");
  node->order = LEAF_ORDER;
  bt.root = node;

  ERROR_FATAL(-1 == bt_split_root(&bt), "FAIL: bt_split_root()");
  ERROR_UNDEF_FATAL_FMT(1 != bt.root->order, "FAIL: order %d != 1", bt.root->order);
  ERROR_UNDEF_FATAL(node != bt.root->childs.nodes[0], "FAIL: child of root is not the node");
     
  bt_freenode(node);
  bt_freenode(bt.root);
  // bt_destroy(&bt); // freed manually

  return EXIT_SUCCESS;
}
