#include <stdlib.h>
#include "ale/memory.h"

void
mem_block_init_size(struct mem_block *block, size_t size)
{
  block->block = malloc(size);
  block->usage = 0;
  block->alloc_size = size;
}

void
mem_block_destroy(struct mem_block *block)
{
  free(block->block);
}

struct mem_block *
mem_block_new(size_t size)
{
  struct mem_block *block = malloc(sizeof(struct mem_block));
  mem_block_init_size(block, size);

  return block;
}

void
mem_block_free(struct mem_block *block)
{
  mem_block_destroy(block);
  free(block);
}

void
mem_init_size(struct mem_pool *pool, size_t size)
{
  pool->block_size = size;
  sl_init(&pool->stack);
  sl_init(&pool->stack_callback);
}

void
mem_callback_init(struct mem_callback *callback, void *ptr, void (*destroy)(void *ptr))
{
  callback->ptr = ptr;
  callback->destroy = destroy;
}

void
mem_callback_destroy(struct mem_callback *callback)
{
  callback->destroy(callback->ptr);
}



struct mem_callback *
mem_callback_new(void *ptr, void (*destroy)(void *ptr))
{
  struct mem_callback *callback = malloc(sizeof(struct mem_callback));
  mem_callback_init(callback, ptr, destroy);

  return callback;
}
  
void
mem_callback_free(struct mem_callback *callback)
{
  mem_callback_destroy(callback);
  free(callback);
}


void
mem_init(struct mem_pool *pool)
{
  mem_init_size(pool, MEM_DEFAULT_BLOCK_SIZE);
}

void
mem_destroy(struct mem_pool *pool)
{
  sl_destroy_full(&pool->stack,
		  (void (*)( void * )) mem_block_free);
  sl_destroy_full(&pool->stack_callback,
		  (void (*)( void * )) mem_callback_free);
}


void *
mem_malloc(struct mem_pool *pool, size_t size)
{
  struct mem_block *block;
  void *ptr;
  
  if ( -1 == sl_top(&pool->stack, (void **) &block) ||
       (block->alloc_size - block->usage) < size )
    {
      
      block = mem_block_new( (size > pool->block_size)?size:pool->block_size );
      sl_push(&pool->stack,  block);
    }

  ptr = block->block + block->usage;
  block->usage += size;
  
  return ptr;
}

void
mem_callback(struct mem_pool *pool, void *ptr, void (*destroy)(void *ptr))
{
   struct mem_callback *callback = mem_callback_new(ptr, destroy);
   sl_push(&pool->stack_callback, callback);
}
