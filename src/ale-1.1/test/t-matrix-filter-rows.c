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
filter(size_t n, double row[n], void *cls)
{
  for (size_t i = 0 ; i < n ; i++)
    if (0 == row[i])
      return 0;
  
  return 1;
}

int
main(int argc, char *argv[argc])
{
  struct matrix src, dst;
  const size_t n = 3, m = 3;
  double mat[3][3] =  {{ 1.0, 2.0, 3.0},
		       { 1.0, 0.0, 3.0},
		       { 1.0, 2.0, 3.0}};
  FILE *file = create_file(m, n, mat);
  int ret;
  
  matrix_init_size(&src, 6);
  matrix_init_size(&dst, 6);

  ret = matrix_read_full(&src, file,
			 &(struct matrix_parameters)
			 {
			   .sep = '\t', .comment = '#',
			    .flags = MATRIX_FHEADER | MATRIX_FROWNAMES | MATRIX_FHEADERONELESS
			    } );
  ERROR_FATAL(-1 == ret, "FAIL: matrix_read_full() returned -1\n");

  ret = matrix_filter_rows(&dst, &src, filter, NULL);
  ERROR_FATAL(-1 == ret, "FAIL: matrix_filter_rows() returned -1\n");
  ERROR_UNDEF_FATAL_FMT(2 != dst.m, "FAIL: matrix_filter_rows() dst.m = %zu != %zu\n", dst.m, 2);
  ERROR_UNDEF_FATAL_FMT(3 != dst.n, "FAIL: matrix_filter_rows() dst.n = %zu != %zu\n", dst.n, 3);

  char *rowname = index_rget(&dst.rownames, 0);
  ERROR_UNDEF_FATAL(NULL == rowname, "FAIL: index_rget(0) = NULL\n");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(rowname,"row0"), "FAIL: index_rget(0) = '%s' != 'row0'\n", rowname);

  rowname = index_rget(&dst.rownames, 1);
  ERROR_UNDEF_FATAL(NULL == rowname, "FAIL: index_rget(1) = NULL\n");
  ERROR_UNDEF_FATAL_FMT(0 != strcmp(rowname,"row2"), "FAIL: index_rget(1) = %s != 'row2'\n", rowname);

  ssize_t index = index_get(&dst.rownames, "row1");
  ERROR_UNDEF_FATAL_FMT(-1 != index, "FAIL: index_get(row1) = %z != -1\n", index);
  index = index_get(&dst.rownames, "row0");
  ERROR_UNDEF_FATAL_FMT(0 != index, "FAIL: index_get(row0) = %z != -1\n", index);
  index = index_get(&dst.rownames, "row2");
  ERROR_UNDEF_FATAL_FMT(1 != index, "FAIL: index_get(row2) = %z != -1\n", index);

  for (size_t i = 0 ; i  < n ; i++)
    {
      double x = dst.data[0*n + i];
      ERROR_UNDEF_FATAL_FMT(x != mat[0][i], "FAIL: dst[0][%zu] = %f != %f = mat[0][%zu]", i, x, mat[0][i], i);
    }
  
  for (size_t i = 0 ; i  < n ; i++)
    {
      double x = dst.data[1*n + i];
      ERROR_UNDEF_FATAL_FMT(x != mat[2][i], "FAIL: dst[1][%zu] = %f != %f = mat[2][%zu]", i, x, mat[2][i], i);
    }
  
  matrix_destroy(&dst);
  matrix_destroy(&src);

  fclose(file);
  
  return EXIT_SUCCESS;
}
