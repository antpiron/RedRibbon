
#include <config.h>

// #ifdef HAVE_R_H
#include <ale/sort.h>

#include "RedRibbon.h"

SEXP
rrho_r_expression_prediction(SEXP mat, SEXP nbr_tested)
{
  struct mem_pool pool;
  mem_init(&pool);

  SEXP ret;
  size_t m = nrows(mat);
  size_t n = ncols(mat);
  ssize_t rnbr_tested = INTEGER(nbr_tested)[0];
  double (*rmat)[m] = ( double (*)[m] ) REAL(mat);
  ssize_t *index = mem_malloc(&pool, sizeof(ssize_t) * m);
  double *r = mem_malloc(&pool, sizeof(double) * m);
  double (*beta)[m] = mem_malloc(&pool, sizeof(double) * 2 * m);

  
  int err = rrho_expression_prediction_col_major(m, n, rmat, rnbr_tested, index, beta, r);

  const char *names[] = {"index", "beta", "r", ""};
  ret = PROTECT(Rf_mkNamed(VECSXP, names));
  SEXP beta_sexp = PROTECT(allocMatrix(REALSXP, m, 2));
  SEXP index_sexp = PROTECT(allocVector(INTSXP, m));
  SEXP r_sexp = PROTECT(allocVector(REALSXP, m));

  int *index_c = INTEGER(index_sexp);
  for (size_t i = 0 ; i < m ; i++)
    index_c[i] = (index[i] < 0) ? -1 : index[i] + 1;
  SET_VECTOR_ELT(ret, 0, index_sexp);
 
  memcpy(REAL(beta_sexp), beta, sizeof(double) * 2 * m);
  SET_VECTOR_ELT(ret, 1, beta_sexp);

  memcpy(REAL(r_sexp), r, sizeof(double) * m);
  SET_VECTOR_ELT(ret, 2, r_sexp);
 
  mem_destroy(&pool);
  UNPROTECT(4);
 
  return ret;
}
