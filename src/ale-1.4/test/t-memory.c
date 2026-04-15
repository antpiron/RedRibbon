#include <stdlib.h>

#include <string.h>

#include "ale/error.h"
#include "ale/memory.h"

#define ALLOC_SIZE ((1ul << 10) + 1)
#define NUM_ITER (1ul << 8)
#define ARRAY_LEN (1ul << 8)

void
destroy(void *ptr)
{
  int *intp = ptr;

  *intp = 1;
}

int
main(int argc, char *argv[argc])
{
  struct mem_pool pool;
  int array[ARRAY_LEN] = {0};

  mem_init(&pool);
  ERROR_UNDEF_FATAL(MEM_DEFAULT_BLOCK_SIZE != pool.block_size, "FAIL: mem_init()\n" );

  for (int i = 0 ; i < NUM_ITER ; i++)
    {
      char *ptr = NULL;
      ptr = mem_malloc(&pool, ALLOC_SIZE);
      memset(ptr, i, ALLOC_SIZE);
      ERROR_UNDEF_FATAL(NULL == pool.stack.next, "FAIL: mem_malloc()\n" );
    }

  size_t sum = 0;
  for (struct sl_node *node = sl_first(&pool.stack) ;
       node ; node = sl_next(node) )
    {
      struct mem_block* block = node->data;
      ERROR_UNDEF_FATAL_FMT(block->usage > block->alloc_size,
			    "FAIL: usage = %zu > %zu = alloc_size\n", block->usage, block->alloc_size );  
      sum += block->usage;
    }
  size_t exp =  ALLOC_SIZE * NUM_ITER;
  ERROR_UNDEF_FATAL_FMT(exp != sum, "FAIL: mem_malloc() usage = %zu != %zu\n", sum, exp );  

  for (int i = 0 ; i < ARRAY_LEN ; i++)
    {
      mem_callback(&pool, array + i, destroy);
    }
  
  
  mem_destroy(&pool);

  for (int i = 0 ; i < ARRAY_LEN ; i++)
    ERROR_UNDEF_FATAL_FMT(1 != array[i], "FAIL: mem_destroy() 1 !=  %d = array[%d]\n", array[i], i);

  
  return EXIT_SUCCESS;
}
