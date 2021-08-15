#include "config.h"

// #ifdef HAVE_R_H
#include <ale/sort.h>
#include <ale/algebra.h>
#include <ale/error.h>
#include <ale/stats-normalize.h>

#include "RedRibbon.h"

// TODO: move to the refgene package
SEXP
rrho_r_normalize(SEXP mat, SEXP ref, SEXP mode)
{
  struct mem_pool pool;

  SEXP ret;

  size_t m = nrows(mat);
  size_t n = ncols(mat);
  size_t r = length(ref);

  if (r > m)
    error("Normalization error: ref length should be less or equal than mat row number.");
  
  mem_init(&pool);

  double (*mat_r)[m] = ( double (*)[m] ) REAL(mat);
  int *ref_r = INTEGER(ref);

  
  size_t *ref_c = mem_malloc(&pool, sizeof(size_t) * r);

  for (size_t i = 0 ; i < r; i++)
    ref_c[i] = ref_r[i] - 1; 
  
  double (*mat_c)[n] = mem_malloc(&pool, sizeof(double) * m * n);
  double *beta_c = mem_malloc(&pool, sizeof(double) * n );
  double (*ret_r)[m];

  alg_transpose(n, m, mat_r, mat_c);

  //  STATS_LS_MEAN = 0,
  //  STATS_LS_VARIANCE,
  //  STATS_POISSON
  // TODO: add other modes
  int err = stats_normalize_beta(m, n, r, mat_c, ref_c, beta_c, STATS_POISSON);
  if ( 0 <= err )
    {
      stats_normalize_samples(m, n, mat_c, mat_c, beta_c);
      ret = PROTECT(allocMatrix(REALSXP, m, n));

      ret_r = ( double (*)[m] ) REAL(ret);
      alg_transpose(m, n, mat_c, ret_r);

      mem_destroy(&pool);
      UNPROTECT(1);
  
      return ret;
    }

  mem_destroy(&pool);
  error("Normalization error.");

  return R_NilValue;
}
