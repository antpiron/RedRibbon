#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/matrix.h"


int
main(int argc, char *argv[argc])
{
  struct matrix mat;

  matrix_init_size(&mat, 1024);
  ERROR_FATAL_FMT(0 != mat.m || 0 != mat.n, "FATAL: [%zu, %zu] != 0\n",  mat.m, mat.n);
  ERROR_FATAL_FMT(1024 != mat.alloc_size_double, "FATAL: alloc_size_double =%zu != 0\n",  mat.alloc_size_double);
 
  matrix_destroy(&mat);
  
  
  return EXIT_SUCCESS;
}
