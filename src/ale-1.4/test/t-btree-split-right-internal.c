#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"

#define ORDER 128
#define SPLIT_POS (ORDER / 3)
#define RIGHT_ORDER (ORDER - SPLIT_POS)

int
main(int argc, char *argv[argc])
{
  struct btree bt;
  struct btnode *node = bt_mknode(ORDER, BT_NODE_INTERNAL);
  struct btnode *res;

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");
  ERROR_FATAL(NULL == node, "FAIL: bt_mknode()");

  for (int i = 0 ; i < ORDER - 1 ; i++)
    {
      node->key[i] =  (void*) (intptr_t) i;
      node->childs.nodes[i] = (void*) (intptr_t) (i+ORDER);
    }
  node->childs.nodes[ORDER-1] = (void*) (intptr_t) (2 * ORDER - 1);
  node->order = ORDER;
    
  ERROR_FATAL(NULL == (res = bt_split_right_internal(&bt, node, SPLIT_POS)), 
	      "FAIL: bt_split_right_internal() returned NULL");

  ERROR_FATAL_FMT(RIGHT_ORDER != res->order, "FAIL: order %d != %d", res->order, RIGHT_ORDER);

  for (int i = 0 ; i < res->order-1 ; i++)
    {
      ERROR_FATAL_FMT(res->key[i] != node->key[SPLIT_POS+i], 
		      "FAIL: keys [%d] is %"PRIdPTR" != %"PRIdPTR, i, 
		      (intptr_t) res->key[i], (intptr_t) node->key[SPLIT_POS+i]);
      ERROR_FATAL_FMT(res->childs.nodes[i] != node->childs.nodes[SPLIT_POS+i], 
		      "FAIL: data [%d] is %"PRIdPTR" != %"PRIdPTR, i,
		      (intptr_t) res->childs.nodes[i], (intptr_t) node->childs.nodes[SPLIT_POS+i]);
    }
  ERROR_FATAL_FMT(res->childs.nodes[res->order-1] != node->childs.nodes[ORDER-1], 
		  "FAIL: data [%d] is %"PRIdPTR" != %"PRIdPTR, res->order-1,
		  (intptr_t) res->childs.nodes[res->order-1], (intptr_t) node->childs.nodes[ORDER-1]);


  bt_freenode(node);
  bt_freenode(res);
  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
