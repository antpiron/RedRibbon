#include "config.h"

#ifdef HAVE_R_H

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include "rrho.h"

struct rrho_c
{
  int i, j, ilen, jlen, mres, nres;
  double *a, *b;
  const char *mode;
};

SEXP
rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP mres, SEXP nres, SEXP a, SEXP b, SEXP mode)
{
  struct rrho rrho;
  int length_a = length(a);
  int length_b = length(b);
  SEXP ret;

  if ( length_a != length_b )
    error("The vectors a and b should be of equal size.");
  if ( ! isReal(a) )
     error("a is not a real.");
  if ( ! isReal(jlen) )
     error("b is not a real.");

  if ( ! isString(mode) )
    error("Mode is not a string.");
  
  if ( ! isInteger(i) )
     error("i is not an integer.");
  if ( ! isInteger(j) )
     error("j is not an integer.");

  if ( ! isInteger(ilen) )
     error("ilen is not an integer.");
  if ( ! isInteger(jlen) )
     error("jlen is not an integer.");

  
  struct rrho_c c =
    {
     .i = INTEGER(i)[0], .j = INTEGER(j)[0],
     .ilen = INTEGER(ilen)[0], .jlen = INTEGER(jlen)[0],
     .mres = INTEGER(mres)[0], .nres = INTEGER(nres)[0],
     .a = REAL(a), .b = REAL(b),
     .mode = CHAR(STRING_PTR(mode)[0])
    };
  
  
  ret = allocMatrix(REALSXP, c.mres, c.nres);
  PROTECT(ret);
  double (*array)[c.nres] = (void*) REAL(ret);

  rrho_init(&rrho, length_a, c.a, c.b);

  rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.mres, c.nres, array, RRHO_HYPER);

  rrho_destroy(&rrho);
  
  UNPROTECT(1);
  
  return ret;
}


#endif
