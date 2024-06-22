#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/stack.h"
#include "ale/stack-scalars.h"


int
main(int argc, char *argv[argc])
{
  struct stack_int stack;
  struct stack_int stack_dup;
  int val;

  /* test stack as a vector */
  stack_int_init(&stack);
  ERROR_UNDEF_FATAL_FMT(stack.vector.size != VECTOR_DEFAULT_SIZE,
			"FATAL: stack_int_init() size %zu != %d\n",
			stack.vector.size, VECTOR_DEFAULT_SIZE);

  stack_int_set(&stack, 1, 1);
  ERROR_UNDEF_FATAL_FMT(stack.vector.data[1] != 1, "FATAL: stack_int_set() size %d != 1\n", stack.vector.data[1]);

  stack_int_set(&stack, 512, 512);
  ERROR_UNDEF_FATAL_FMT(stack.vector.data[512] != 512, "FATAL: stack_int_set() size %d != 512\n", stack.vector.data[512]);

  val = stack_int_get(&stack, 1);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: stack_int_get() size %d != 1\n", val);
  val = stack_int_get(&stack, 512);
  ERROR_UNDEF_FATAL_FMT(512 != val, "FATAL: stack_int_get() size %d != 512\n", val);

  stack_int_init_copy(&stack_dup, &stack);
  ERROR_UNDEF_FATAL_FMT(stack_dup.vector.size != stack.vector.size,
			"FATAL: stack_int_init_copy() size = %zu != %zu = dup_size\n",
			stack.vector.size, stack_dup.vector.size);
  for (size_t i = 0 ; i < stack.vector.size ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(stack.vector.data[i] != stack_dup.vector.data[i],
			    "FATAL: stack_int_init_copy() data[%zu] = %d != %d = dup_data[%zu]\n",
			    i, stack.vector.data[i], stack_dup.vector.data[i], i);
    }
  stack_int_destroy(&stack_dup);

  stack_int_init_size(&stack_dup, 1);
  stack_int_copy(&stack_dup, &stack);
  ERROR_UNDEF_FATAL_FMT(stack_dup.vector.size < stack.vector.size,
			"FATAL: stack_int_init_copy() size = %zu > %zu = dup_size\n",
			stack.vector.size, stack_dup.vector.size);
  for (size_t i = 0 ; i < stack.vector.size ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(stack.vector.data[i] != stack_dup.vector.data[i],
			    "FATAL: stack_int_init_copy() data[%zu] = %d != %d = dup_data[%zu]\n",
			    i, stack.vector.data[i], stack_dup.vector.data[i], i);
    }

  
  stack_int_destroy(&stack_dup);
  stack_int_destroy(&stack);

  /* real stack tests */
  stack_int_init(&stack);
  ERROR_UNDEF_FATAL_FMT(stack.n != 0,
			"FATAL: stack_int_init() n = %zu != 0\n",
			stack.n);

  stack_int_push(&stack, 1);
  ERROR_UNDEF_FATAL_FMT(stack.n != 1,
			"FATAL: stack_int_push(1) n = %zu != 1\n",
			stack.n);

  val = stack_int_top(&stack);
  ERROR_UNDEF_FATAL_FMT(stack.n != 1,
			"FATAL: stack_int_top() n = %zu != 1\n",
			stack.n);
  ERROR_UNDEF_FATAL_FMT(val != 1,
			"FATAL: stack_int_top() = %d != 1\n",
			val);

  val = stack_int_pop(&stack);
  ERROR_UNDEF_FATAL_FMT(stack.n != 0,
			"FATAL: stack_int_pop() n = %zu != 0\n",
			stack.n);
  ERROR_UNDEF_FATAL_FMT(val != 1,
			"FATAL: stack_int_pop() = %d != 1\n",
			val);


  stack_int_destroy(&stack);
  
  return EXIT_SUCCESS;
}
