#include "config.h"

// #ifdef HAVE_R_H

#include "RedRibbon.h"





// static inline int
// rrho_generic(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res, int mode)
SEXP
rrho_r_rrho(SEXP i, SEXP j, SEXP a, SEXP b, SEXP mode)
{
  struct rrho rrho;
  struct rrho_result res;
  int length_a = length(a);
  int length_b = length(b);
  SEXP ret;

  if ( length_a != length_b )
    error("The vectors a and b should be of equal size.");
  if ( ! isReal(a) )
     error("a is not a real.");
  if ( ! isReal(b) )
     error("b is not a real.");

  if ( ! isString(mode) )
    error("Mode is not a string.");
  
  if ( ! isInteger(i) )
     error("i is not an integer.");
  if ( ! isInteger(j) )
     error("j is not an integer.");

  struct rrho_c c =
    {
     .i = INTEGER(i)[0] - 1, .j = INTEGER(j)[0] - 1,
     .a = REAL(a), .b = REAL(b),
     .strmode = CHAR(STRING_PTR(mode)[0]),
      .mode = RRHO_HYPER
    };

  if ( c.i < 0 )
    error("i should be  greater or equal of 1.");
  if ( c.j < 0 )
    error("j should be  greater or equal of 1.");
  /* enum {
      RRHO_HYPER = 0,
      RRHO_HYPER_TWO_TAILED,
      RRHO_HYPER_TWO_TAILED_R_MODULE
      }; */
  if ( 0 == strcmp(c.strmode, "hyper-two-tailed") )
    c.mode = RRHO_HYPER_TWO_TAILED;
  else if ( 0 == strcmp(c.strmode, "hyper-two-tailed-old") )
    c.mode = RRHO_HYPER_TWO_TAILED_R_MODULE;



  const char *names[] = {"pvalue", "log_pvalue", "direction", "count", ""};
  ret = PROTECT(Rf_mkNamed(VECSXP, names));

  rrho_init(&rrho, length_a, c.a, c.b);

  rrho_generic(&rrho, c.i, c.j, c.mode, &res);
  // rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, array, RRHO_HYPER);

  SEXP pvalue = PROTECT(Rf_ScalarReal(res.pvalue));
  SET_VECTOR_ELT(ret, 0, pvalue);
  
  long double log_pvalue = fabsl(-logl(res.pvalue));
  SEXP Slog_pvalue = PROTECT(Rf_ScalarReal(log_pvalue));
  SET_VECTOR_ELT(ret, 1, Slog_pvalue);
  
  SEXP direction = PROTECT(Rf_ScalarInteger(res.direction));
  SET_VECTOR_ELT(ret, 2, direction);
  
  SEXP count = PROTECT(Rf_ScalarInteger(res.count));
  SET_VECTOR_ELT(ret, 3, count);

  rrho_destroy(&rrho);
  
  UNPROTECT(5);

  return ret;
}
