#ifndef __MATRIX_H
#define __MATRIX_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <ale/error.h>
#include <ale/index.h>

#define MATRIX_DEFAULT_SIZE (1lu << 8)
#define MATRIX_MAX_SIZE (1lu << 24)

enum {
      MATRIX_ECHAR = 1,
      MATRIX_ECOLS,
      MATRIX_EEMPTY
};

enum {
      MATRIX_FHEADER = 1,
      MATRIX_FROWNAMES = 1 << 1,
      MATRIX_FFILL = 1 << 2,
      MATRIX_FHEADERONELESS = 1 << 3,     
      MATRIX_FSKIPEMPTY = 1 << 4
};

struct matrix_parameters
{
  char sep;
  char comment;
  uint32_t flags;
};

struct matrix
{
  size_t m, n;
  size_t alloc_size_double, alloc_size_step;
  // uint32_t flags;
  struct index rownames;
  struct index colnames;
  double *data;
};

int matrix_init(struct matrix *mat);
int matrix_init_size(struct matrix *mat, size_t size);
void matrix_destroy(struct matrix *mat);
int matrix_read_full(struct matrix *mat, FILE *file, struct matrix_parameters *params);
int matrix_filter_rows(struct matrix *dst, const struct matrix *src,
		       int (*filter)(size_t n, double row[n], void *cls),
		       void *cls);
ssize_t matrix_fprint_c(FILE *file, size_t m, size_t n, double *mat, struct index *rownames,
			struct index *colnames, struct matrix_parameters *params);
ssize_t matrix_fprint(FILE *file, struct matrix *mat, struct matrix_parameters *params);

// ===================
// INTERNAL USAGE ONLY
// ===================

static inline void
matrix_resize(struct matrix *mat, size_t size)
{
  if (size > mat->alloc_size_double)
    {
      size_t new_size = ((size + mat->alloc_size_step) / mat->alloc_size_step) * mat->alloc_size_step;
      size_t alloc_size_step = mat->alloc_size_step << 1;

      if (alloc_size_step <= mat->alloc_size_step)
	mat->alloc_size_step = alloc_size_step;

      if (mat->alloc_size_double > 0)
        mat->data = realloc(mat->data, new_size * sizeof(double));
      else
        mat->data = malloc(new_size * sizeof(double));
      ERROR_UNDEF_FATAL(NULL ==  mat->data, "matrix_resize() unable to allocate memory!\n");
      mat->alloc_size_double = new_size;      
    }
}

#endif
