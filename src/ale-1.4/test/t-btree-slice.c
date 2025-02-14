#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"

#define ORDER 128

int
main(int argc, char *argv[argc])
{
  struct btnode *node1 = bt_mknode(ORDER, BT_NODE_LEAF);
  struct btnode *node2 = bt_mknode(ORDER, BT_NODE_LEAF);

  ERROR_FATAL(NULL == node1 || NULL == node2, "FAIL: bt_mknode()");
 
  for (int i = 0 ; i < ORDER - 1 ; i++)
    {
      node1->key[i] =  (void*) (intptr_t) i;
      node1->childs.data[i] = (void*) (intptr_t) (i+ORDER);
      node2->key[i] = NULL;
      node2->childs.data[i] = NULL;
    }

  bt_slice(node2, node1, 10, ORDER - 20);
  
  for (int i = 0 ; i < ORDER - 20 ; i++)
    {
      ERROR_FATAL_FMT(node2->key[i] != node1->key[10+i], 
		      "FAIL: bt_slice() keys %"PRIdPTR" != %"PRIdPTR, 
		      (intptr_t) node2->key[i], (intptr_t) node1->key[10+i]);
      ERROR_FATAL_FMT(node2->childs.data[i] != node1->childs.data[10+i], 
		      "FAIL: bt_slice() data %"PRIdPTR" != %"PRIdPTR, 
		      (intptr_t) node2->childs.data[i], (intptr_t) node1->childs.data[10+i]);
    }

  for (int i = ORDER - 20 ; i < ORDER - 1 ; i++)
    {
      ERROR_FATAL_FMT(NULL != node2->key[i], "FAIL: bt_slice() key [%d] != NULL == %"PRIdPTR, 
		      i, (intptr_t)node2->key[i]);
      ERROR_FATAL_FMT(NULL != node2->childs.data[i], "FAIL: bt_slice() data [%d] != NULL == %"PRIdPTR, 
		      i, (intptr_t)node2->childs.data[i]);
    }
    

  bt_freenode(node1);
  bt_freenode(node2);

  return EXIT_SUCCESS;
}
