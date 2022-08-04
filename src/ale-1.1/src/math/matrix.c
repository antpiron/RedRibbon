#include "ale/matrix.h"
#include "ale/stringutils.h"


int
matrix_init(struct matrix *mat)
{
  return matrix_init_size(mat, MATRIX_DEFAULT_SIZE);
}

int
matrix_init_size(struct matrix *mat, size_t size)
{
  mat->m = mat->n = 0;

  if (size > 0)
    {
      mat->data = malloc(size * sizeof(double));
      ERROR_UNDEF_FATAL(NULL == mat->data,
                        "matrix_init_size() unable to allocate memory!\n");
    }
  
  mat->alloc_size_step = MATRIX_DEFAULT_SIZE;
  mat->alloc_size_double = size;

  index_init(&mat->rownames);
  index_init(&mat->colnames);
  
  return 0;
}

void
matrix_destroy(struct matrix *mat)
{
  index_destroy(&mat->rownames);
  index_destroy(&mat->colnames);
  free(mat->data);
}

int
matrix_read_full(struct matrix *mat, FILE *file, struct matrix_parameters *params)
{
  struct string str;
  size_t nrows = 0, ncols = 0, nlines = 0;
  
  string_init(&str);
  
  for ( ; 0 == feof(file) ; nlines++ )
    {
      int ret = string_readline(&str, file);
      if (-1 == ret && feof(file))
	break;
      ERROR_GOTO(-1 == ret, ERROR);
      char *current = str.str;
      size_t count = 0;

      
      string_chomp(&str);
        
      if (0 == str.len)
	{
	  ERROR_CUSTOM_GOTO(0 == (MATRIX_FSKIPEMPTY & params->flags), MATRIX_EEMPTY, ERROR);
	  continue;
	}
      
      if (params->comment == str.str[0])
	continue;

      if ( 0 != (MATRIX_FHEADER & params->flags) && 0 == ncols )
	{
	  // ncols = 0;
	  
	  if ( 0 != (MATRIX_FROWNAMES & params->flags) &&
	       0 ==  (MATRIX_FHEADERONELESS & params->flags) )
	    {
	      for ( ; *current && params->sep != *current ; current++ );
	      if (params->sep == *current)
		current++;
	    }

	  while (*current)
	    {
	      char *ptr = current;
	      for ( ; *current && params->sep != *current ; current++ );
	      if ( params->sep == *current )
		{
		  *current = '\0';
		  current++;
		}
	      index_set(&mat->colnames, ptr, ncols);
	      ptr = current;
	      ncols++;
	    }

	  continue;
	}

      if ( 0 != (MATRIX_FROWNAMES & params->flags) )
	{
	  for ( ; *current && params->sep != *current ; current++ ) ;

	  if (*current)
	    {
	      *current = '\0';
	      current++;
	    }
	  index_set(&mat->rownames, str.str, nrows);
	}

      for ( count = 0 ; *current ; current++, count++ )
	{
	  errno = 0;
	  double val = strtod(current, &current);
	  ERROR_ERRNO_GOTO(0 != errno, ERROR);
	  int is_error = params->sep != *current && '\0' != *current;
	  ERROR_CUSTOM_MSG_FMT(is_error, MATRIX_ECHAR,
			       "Invalid number of columns at line %zu: sep = %d\n",
			       nlines, *current);
	  ERROR_CUSTOM_GOTO(is_error, MATRIX_ECHAR, ERROR);

	  matrix_resize(mat, mat->alloc_size_double+1);
	  mat->data[nrows * ncols + count] = val;
	}

      if ( 0 == count )
	ncols = count;

      if ( count != ncols )
	{
	  ERROR_CUSTOM_MSG_FMT(1, MATRIX_ECOLS,
			       "Invalid number of columns at line %zu: %zu != %zu\n",
			       nlines, count, ncols);
	  ERROR_CUSTOM_GOTO(1, MATRIX_ECOLS, ERROR);
	}

      nrows++;
    }

  mat->m = nrows;
  mat->n = ncols;
  
  string_destroy(&str);

  return 0;

 ERROR:
  string_destroy(&str);
  return -1;
}

int
matrix_filter_rows(struct matrix *dst, const struct matrix *src,
		   int (*filter)(size_t n, double row[n], void *cls),
		   void *cls)
{
  size_t m = 0;
  size_t n = src->n;

  for (size_t i = 0 ; i < src->n ; i++)
    {
      char *colname = index_rget(&src->colnames, i);
      if (NULL != colname)
	index_set(&dst->colnames, colname, i);
    }

  for (size_t i = 0 ; i < src->m ; i++)
    {
      if ( filter(n, &src->data[i * n], cls) )
	{
	  char *rowname;
	  matrix_resize(dst, (m+1) * n);
	  memcpy(&dst->data[m * n], &src->data[i * n], n*sizeof(double) );
	  rowname = index_rget(&src->rownames, i);
	  if ( NULL != rowname )
	    index_set(&dst->rownames, rowname, m);
	  m++;
	}
    }
  dst->m = m;
  dst->n = n;
  
  return 0;
}

static void
print_index(FILE *file, struct index *names, size_t i)
{
  char *str = index_rget(names, i);
  if (NULL == str)
    fprintf(file, "%zu", i);
  else
    fprintf(file, "%s", str);
}


ssize_t
matrix_fprint_c(FILE *file, size_t m, size_t n, double *mat, struct index *rownames,
		struct index *colnames, struct matrix_parameters *params)
{
  ERROR_CUSTOM_RET(0 == n, 2, -1);
  ERROR_CUSTOM_RET(0 == m, 1, -1);
  
  if ( 0 != (MATRIX_FHEADER & params->flags) )
    {
      size_t j = 0;
       
      if ( 0 != (MATRIX_FROWNAMES & params->flags) )
	fprintf(file, "%c", params->sep);
      
      while (1)
	{
	  print_index(file, colnames, j);

	  if ( ++j  >= n )
	    break;
	  
	  fprintf(file, "%c", params->sep);
	}
      fprintf(file, "\n");
    }
  for (size_t i = 0 ; i  < m ; i++)
    {
      size_t j = 0;
      
      if ( 0 != (MATRIX_FROWNAMES & params->flags) )
	{
	  print_index(file, rownames, i);
	  fprintf(file, "%c", params->sep);
	}
      
      while (1)
	{
	  fprintf(file, "%f", mat[i*n + j] );
	  
	  if ( ++j >= n)
	    break;
	  
	  fprintf(file, "%c", params->sep);
	}
      fprintf(file, "\n");
    }

  return 0;
}

ssize_t
matrix_fprint(FILE *file, struct matrix *mat, struct matrix_parameters *params)
{

  return matrix_fprint_c(file, mat->m, mat->n, mat->data, &mat->rownames, &mat->colnames, params);
}
