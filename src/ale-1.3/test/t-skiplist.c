#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/skiplist.h"

#define NUM_INSERT (1 << 17)

inline int
cmp_int(int a, int b)
{
  return a - b;
}

SKL_INIT(int,16,int,int,cmp_int)

int
main(int argc, char *argv[argc])
{
  struct skl_int skl;
  struct skl_int_node *node;
  int key, val, ret;

  skl_int_init(&skl);
  
  ret = skl_int_insert(&skl, 1, 1, NULL);
  ERROR_UNDEF_FATAL_FMT(ret != 0, "FATAL: skl_int_insert(1) ret == %d != 0\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != skl.header.forward[0]->value,
			"FATAL: 1 != %d\n", skl.header.forward[0]->value);
  ERROR_UNDEF_FATAL(NULL != skl.header.forward[0]->forward[0],
		    "FATAL: NULL != skl.header.forward[0]->forward[0]\n");
  
  ret = skl_int_search(&skl,1,&node);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: skl_int_search(1) %d != 1\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != node->value,
			"FATAL: skl_int_search() 1 != %d\n", node->value);

  for (int i = 0 ; i < NUM_INSERT ; i++)
    {
      int ret = skl_int_insert(&skl, i, i, NULL);
      ERROR_UNDEF_FATAL_FMT(1 == i && ret != 1, "FATAL: skl_int_insert(i) ret == %d != 1\n", ret);
      ERROR_UNDEF_FATAL_FMT(1 != i && ret != 0, "FATAL: skl_int_insert(i) ret == %d != 0\n", ret);
    }

  for (int i = 0 ; i < NUM_INSERT ; i++)
    {
      int ret = skl_int_search(&skl,i,&node);
      ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: skl_int_search(i) %d != 1\n", ret);
      ERROR_UNDEF_FATAL_FMT(i != node->value,
			    "FATAL: skl_int_search() %d != %d\n", i, node->value);
    }

  int prev = -1;
  node = skl.header.forward[0];
  for (int i = 0 ; i < NUM_INSERT ; i++, node = node->forward[0], prev++)
    {
      ERROR_UNDEF_FATAL_FMT(prev+1 != node->key, "FATAL: key disorder %d != %d\n",
			    prev+1, node->key);
      ERROR_UNDEF_FATAL_FMT(prev+1 != node->value, "FATAL: value disorder  %d != %d\n",
			    prev+1, node->value);
    }

  ret = skl_int_delete(&skl,10,&key,&val);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: skl_int_delete(10) ret == %d != 10\n", ret);
  ERROR_UNDEF_FATAL_FMT(10 != key, "FATAL: skl_int_delete(10) key == %d != 10\n", key);
  ERROR_UNDEF_FATAL_FMT(10 != val, "FATAL: skl_int_delete(10) key == %d != 10\n", val);

  for (int i = 0 ; i < NUM_INSERT ; i++)
    {
      int ret = skl_int_search(&skl,i,&node);
      ERROR_UNDEF_FATAL_FMT(10 == i && 0 != ret, "FATAL: skl_int_search(%d) %d != 0\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(10 != i && 1 != ret, "FATAL: skl_int_search(%d) %d != 1\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(10 != i && i != node->value,
			    "FATAL: skl_int_search(%d) %d != %d\n", i, i, node->value);
    }

  skl_int_destroy(&skl);
  
  return EXIT_SUCCESS;
}
