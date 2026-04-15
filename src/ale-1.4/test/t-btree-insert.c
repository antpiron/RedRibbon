#include "config.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"

#define ORDER 24
#define LEAF_ORDER (ORDER - 1)
#define RIGHT_ORDER ((LEAF_ORDER+1) / 2 + 1)
#define LEFT_ORDER (LEAF_ORDER / 2)

void
cmp_key_rec(struct btnode *node, intptr_t lt, intptr_t ge)
{
  int order = BT_ISLEAF(*node)?node->order:node->order - 1;

  for (int i = 0 ; i < order ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(lt >= (intptr_t) node->key[i] || (intptr_t) node->key[i] > ge, 
			    "FAIL: Not %"PRIdPTR " <= %"PRIdPTR " < %"PRIdPTR, lt, (intptr_t) node->key[i], ge);
    }
  
  for (int i = 1 ; i < order ; i++)
    ERROR_UNDEF_FATAL_FMT(node->key[i-1] >= node->key[i], 
			  "FAIL: key [%d] %"PRIdPTR " >= [%d] %"PRIdPTR, 
			  i-1, (intptr_t) node->key[i-1], i, (intptr_t) node->key[i]);

  if (BT_ISINTERNAL(*node))
    {
      intptr_t nlt = lt;
      intptr_t nge;
      for (int i = 0 ; i < node->order ; i++)
	{
	  nge = (order == i)?ge:(intptr_t) node->key[i];

	  cmp_key_rec(node->childs.nodes[i], nlt, nge);

	  nlt = nge;
	}
    }
}

int
main(int argc, char *argv[argc])
{
  struct btree bt;

  ERROR_FATAL(-1 == bt_init(&bt, ORDER, btfuncs_intptr_t), "FAIL: bt_init()");

  ERROR_FATAL(-1 ==  bt_insert(&bt, (void*) (intptr_t) 0, (void*)(intptr_t) 0), "FAIL: unable to insert (0 => 0)");
  ERROR_UNDEF_FATAL_FMT(1 != bt.root->order, "FAIL: Order %d != 1", bt.root->order);

  for (int i = 1 ; i < LEAF_ORDER ; i++)
    {
      ERROR_FATAL_FMT(-1 ==  bt_insert(&bt, (void*) (intptr_t) i, (void*)(intptr_t) i), 
		      "FAIL: unable to insert (%d => %d)", i, i);
    }


  ERROR_UNDEF_FATAL_FMT(LEAF_ORDER != bt.root->order, "FAIL: Order %d != %d", bt.root->order, LEAF_ORDER);

  ERROR_FATAL_FMT(-1 ==  bt_insert(&bt, (void*) (intptr_t) LEAF_ORDER, (void*)(intptr_t) LEAF_ORDER), 
		  "FAIL: unable to insert (%d => %d)", LEAF_ORDER, LEAF_ORDER);

  ERROR_UNDEF_FATAL_FMT(2 != bt.root->order, "FAIL: Order %d != 2", bt.root->order);
  ERROR_UNDEF_FATAL_FMT(LEFT_ORDER != bt.root->childs.nodes[0]->order, "FAIL: Left order %d != %d", 
			bt.root->childs.nodes[0]->order, LEFT_ORDER);
  ERROR_UNDEF_FATAL_FMT(RIGHT_ORDER != bt.root->childs.nodes[1]->order, "FAIL: Right order %d != %d", 
			bt.root->childs.nodes[1]->order, RIGHT_ORDER);

  for (int i = 0 ; i < 100000 ; i++)
    {
      intptr_t val = random() % INTPTR_MAX;
      
      ERROR_FATAL_FMT(-1 ==  bt_insert(&bt, (void*) val, (void*) val), 
		    "FAIL: unable to insert (%"PRIdPTR " => %"PRIdPTR ")", val, val);
    }
  
  cmp_key_rec(bt.root, INTPTR_MIN, INTPTR_MAX);

  bt_destroy(&bt);

  return EXIT_SUCCESS;
}
