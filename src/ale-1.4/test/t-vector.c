#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ale/error.h"
#include "ale/vector.h"


VECTOR_INIT(int,int)

int
main(int argc, char *argv[argc])
{
  struct vector_int vector;
  struct vector_int vector_dup;
  int val;


  vector_int_init(&vector);
  ERROR_UNDEF_FATAL_FMT(vector.size != VECTOR_DEFAULT_SIZE,
			"FATAL: vector_int_init() size %zu != %d\n",
			vector.size, VECTOR_DEFAULT_SIZE);

  vector_int_set(&vector, 1, 1);
  ERROR_UNDEF_FATAL_FMT(vector.data[1] != 1, "FATAL: vector_int_set() size %d != 1\n", vector.data[1]);

  vector_int_set(&vector, 512, 512);
  ERROR_UNDEF_FATAL_FMT(vector.data[512] != 512, "FATAL: vector_int_set() size %d != 512\n", vector.data[512]);

  val = vector_int_get(&vector, 1);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: vector_int_get() size %d != 1\n", val);
  val = vector_int_get(&vector, 512);
  ERROR_UNDEF_FATAL_FMT(512 != val, "FATAL: vector_int_get() size %d != 512\n", val);

  vector_int_init_copy(&vector_dup, &vector);
  ERROR_UNDEF_FATAL_FMT(vector_dup.size != vector.size,
			"FATAL: vector_int_init_copy() size = %zu != %zu = dup_size\n",
			vector.size, vector_dup.size);
  for (size_t i = 0 ; i < vector.size ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(vector.data[i] != vector_dup.data[i],
			    "FATAL: vector_int_init_copy() data[%zu] = %d != %d = dup_data[%zu]\n",
			    i, vector.data[i], vector_dup.data[i], i);
    }
  vector_int_destroy(&vector_dup);

  vector_int_init_size(&vector_dup, 1);
  vector_int_copy(&vector_dup, &vector);
  ERROR_UNDEF_FATAL_FMT(vector_dup.size < vector.size,
			"FATAL: vector_int_init_copy() size = %zu > %zu = dup_size\n",
			vector.size, vector_dup.size);
  for (size_t i = 0 ; i < vector.size ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(vector.data[i] != vector_dup.data[i],
			    "FATAL: vector_int_init_copy() data[%zu] = %d != %d = dup_data[%zu]\n",
			    i, vector.data[i], vector_dup.data[i], i);
    }

  
  vector_int_destroy(&vector_dup);
  vector_int_destroy(&vector);
  
  return EXIT_SUCCESS;
}
