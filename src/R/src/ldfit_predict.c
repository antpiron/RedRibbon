
#include "config.h"

// #ifdef HAVE_R_H
#include <ale/sort.h>

#include "RedRibbon.h"

SEXP
rrho_r_ldfit_prediction(SEXP half, SEXP pval, SEXP position)
{

  SEXP ret;
  size_t m = length(pval);
  if (length(position) != m)
    error("length(pval) != length(position)");
  
  double half_c = REAL(half)[0];
  double *rpval = (double *) REAL(pval);
  int *rpos = (int*) INTEGER(position);

  struct mem_pool pool;
  mem_init(&pool);

  
  ssize_t *index = mem_malloc(&pool, sizeof(ssize_t) * m);
  size_t *position_c = mem_malloc(&pool, sizeof(ssize_t) * m);
  for (size_t i = 0 ; i < m ; i++)
    position_c[i] = rpos[i];
  
  int err = rrho_ldfit_prediction(m, half_c, rpval, position_c, index);

  ret = PROTECT(allocVector(INTSXP, m));
  int *index_c = INTEGER(ret);
  for (size_t i = 0 ; i < m ; i++)
    index_c[i] = (index[i] < 0) ? -1 : index[i] + 1;
  
  mem_destroy(&pool);
  UNPROTECT(1);
 
  return ret;
}
