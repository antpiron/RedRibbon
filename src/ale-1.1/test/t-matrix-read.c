#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "ale/error.h"
#include "ale/matrix.h"

FILE*
create_file(size_t m, size_t n, double mat[m][n])
{
  FILE *file = tmpfile();

  fprintf(file, "%s%zu", "col", 0);
  for (size_t j = 1 ; j < n ; j++)
    fprintf(file, "\t%s%zu", "col", j);
  fprintf(file, "\n");
  
  for (size_t i = 0 ; i < m ; i++)
    {
      fprintf(file, "%s%zu", "row", i);
      for (size_t j = 0 ; j < n ; j++)
	{
	  fprintf(file, "\t%f", mat[i][j]);
	}
      fprintf(file, "\n");
    }
  
  rewind(file);
  
  return file;
}

int
main(int argc, char *argv[argc])
{
  struct matrix mat;
  const size_t n = 3, m = 3;
  double exp[3][3] =  {{ 1.0, 2.0, 3.0},
		       { 1.0, 2.0, 3.0},
		       { 1.0, 2.0, 3.0}};
  double eps = 0.000000000001;
  FILE *file = create_file(m, n, exp);
  int ret;
  
  matrix_init_size(&mat, 6);

  ret = matrix_read_full(&mat, file,
			 &(struct matrix_parameters)
			 {
			   .sep = '\t', .comment = '#',
			    .flags = MATRIX_FHEADER | MATRIX_FROWNAMES | MATRIX_FHEADERONELESS
			    } );
  ERROR_FATAL_FMT(0 != ret, "FATAL: %d != ret\n",  ret);
  ERROR_FATAL_FMT(m != mat.m || n != mat.n, "FATAL: [%zu, %zu] != [3, 3]\n",  mat.m, mat.n);
  ERROR_FATAL_FMT(mat.alloc_size_double < m*n, "FATAL: alloc_size_double = %zu < %zu\n",
		  mat.alloc_size_double, mat.m * mat.n);

  ssize_t index = index_get(&mat.colnames, "col0");
  ERROR_FATAL(-1 == index, "FATAL: index_get(col0) -1 == index\n");
  ERROR_UNDEF_FATAL_FMT(0 != index, "FATAL: index_get(col0) %zd != index\n",  index);

  index = index_get(&mat.colnames, "col2");
  ERROR_FATAL(-1 == index, "FATAL: index_get(col2) -1 == index\n");
  ERROR_UNDEF_FATAL_FMT(2 != index, "FATAL: index_get(col2) %zd != index\n",  index);

  index = index_get(&mat.colnames, "col3");
  ERROR_UNDEF_FATAL(-1 != index, "FATAL: index_get(col2) -1 == index\n");

  index = index_get(&mat.rownames, "row2");
  ERROR_FATAL(-1 == index, "FATAL: index_get(row2) -1 == index\n");
  ERROR_UNDEF_FATAL_FMT(2 != index, "FATAL: index_get(row2) %zd != index\n",  index);

  for (size_t i = 0 ; i < m ; i++)
    for (size_t j = 0 ; j < n ; j++)
      {
	double (*out)[m][n] = (void*) mat.data;
	double diff = fabs((*out)[i][j] - exp[i][j]);
	ERROR_FATAL_FMT(diff > eps, "FATAL: mat[%zu, %zu] = %f != %f\n",  i, j, (*out)[i][j], exp[i][j]);
      }

  
  
  matrix_destroy(&mat);

  fclose(file);
  
  return EXIT_SUCCESS;
}
