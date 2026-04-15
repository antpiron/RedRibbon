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
  struct btnode *child = bt_mknode(ORDER, BT_NODE_LEAF);
  struct btnode *dad = bt_mknode(ORDER, BT_NODE_INTERNAL);

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");
  ERROR_FATAL(NULL == child || NULL == dad, "FAIL: bt_mknode()");

  for (int i = 0 ; i < ORDER - 1 ; i++)
    {
      child->key[i] =  (void*) (intptr_t) i;
      child->childs.data[i] = (void*) (intptr_t) (i+ORDER);
    }
  child->order = LEAF_ORDER;
  
  for (int i = 0 ; i < ORDER-1 ; i++)
    {
      child->order = LEAF_ORDER;
      dad->order = ORDER-1;
      for (int j = 0 ; j < dad->order ; j++)
	dad->childs.nodes[j] = NULL;
      dad->childs.nodes[i] = child;
      
      ERROR_FATAL(-1 == bt_split_child(&bt, dad, i), "FAIL: bt_split_child");

      ERROR_UNDEF_FATAL_FMT(child != dad->childs.nodes[i], "FAIL: left child pointer changed [%d] %p != %p", 
			    i, (void*) child, (void*) dad->childs.nodes[i]);
      ERROR_UNDEF_FATAL_FMT(ORDER != dad->order, "FAIL: dad order %d != %d", dad->order, ORDER);
      ERROR_UNDEF_FATAL_FMT(LEFT_ORDER - 1 != (intptr_t) dad->key[i], "FAIL: dad key %"PRIdPTR" != %d\n",
			    (intptr_t) dad->key[i], LEFT_ORDER - 1);
      ERROR_UNDEF_FATAL_FMT(LEFT_ORDER != dad->childs.nodes[i]->order, "FAIL: left order %d != %d\n",
      			    dad->childs.nodes[i]->order, LEFT_ORDER);
      ERROR_UNDEF_FATAL_FMT(RIGHT_ORDER != dad->childs.nodes[i+1]->order, "FAIL: right order %d != %d\n",
      			    dad->childs.nodes[i+1]->order, RIGHT_ORDER);

      bt_freenode(dad->childs.nodes[i+1]);
    }
   
  bt_freenode(child);
  bt_freenode(dad);
  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
