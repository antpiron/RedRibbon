#include "config.h"

#ifdef HAVE_R_H

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <string.h>

#include "rrho.h"

struct rrho_c
{
  int i, j, ilen, jlen, m, n;
  double *a, *b;
  const char *strmode;
  int mode;
};

SEXP
rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode)
{
  struct rrho rrho;
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

  if ( ! isInteger(ilen) )
     error("ilen is not an integer.");
  if ( ! isInteger(jlen) )
     error("jlen is not an integer.");

  
  struct rrho_c c =
    {
     .i = INTEGER(i)[0] - 1, .j = INTEGER(j)[0] - 1,
     .ilen = INTEGER(ilen)[0], .jlen = INTEGER(jlen)[0],
     .m = INTEGER(m)[0], .n = INTEGER(n)[0],
     .a = REAL(a), .b = REAL(b),
     .strmode = CHAR(STRING_PTR(mode)[0]),
     .mode = RRHO_HYPER
    };

  if ( c.i < 0 )
    error("i should be  greater or equal of 1.");
  if ( c.j < 0 )
    error("j should be  greater or equal of 1.");
  if ( c.m > c.ilen )
    error("m should be less than ilen.");
  if ( c.n > c.jlen )
    error("n should be less than jlen.");
  /* enum {
      RRHO_HYPER = 0,
      RRHO_HYPER_TWO_TAILED,
      RRHO_HYPER_TWO_TAILED_R_MODULE
      }; */
  if ( strcmp(c.strmode, "hyper-two-tailed") )
    c.mode = RRHO_HYPER_TWO_TAILED;
  else if ( strcmp(c.strmode, "hyper-two-tailed-old") )
    c.mode = RRHO_HYPER_TWO_TAILED_R_MODULE;
  
  
 
  ret = PROTECT(allocMatrix(REALSXP, c.m, c.n));
  double (*array)[c.n] = (void*) REAL(ret);

  rrho_init(&rrho, length_a, c.a, c.b);

  rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, array, RRHO_HYPER);

  rrho_destroy(&rrho);
  
  UNPROTECT(1);
  
  return ret;
}


#endif
