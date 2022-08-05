#include <config.h>

// #ifdef HAVE_R_H

#include "RedRibbon.h"

SEXP
rrho_r_rectangle_min(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP direction)
{
  struct rrho rrho;
  int length_a = length(a);
  int length_b = length(b);
  struct rrho_coord coord;
  SEXP ret;
  int err;

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

  if ( ! isString(direction) )
    error("Direction is not a string.");


  
  struct rrho_c c =
    {
     .i = INTEGER(i)[0] - 1, .j = INTEGER(j)[0] - 1,
     .ilen = INTEGER(ilen)[0], .jlen = INTEGER(jlen)[0],
     .m = INTEGER(m)[0], .n = INTEGER(n)[0],
     .a = REAL(a), .b = REAL(b),
     .strmode = CHAR(STRING_PTR(mode)[0]),
     .mode = RRHO_HYPER,
     .strdirection = CHAR(STRING_PTR(direction)[0]),
     .direction = 1
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
  if ( 0 == strcmp(c.strmode, "hyper-two-tailed") )
    c.mode = RRHO_HYPER_TWO_TAILED;
  else if ( 0 == strcmp(c.strmode, "hyper-two-tailed-old") )
    c.mode = RRHO_HYPER_TWO_TAILED_R_MODULE;
  
  if ( 0 !=  strcmp(c.strdirection, "enrichment") )
    c.direction = -1;

 

  rrho_init(&rrho, length_a, c.a, c.b);
  struct rrho_rectangle_params params = { .m = c.m, .n = c.n };
  err = rrho_rectangle_min(&rrho, c.i, c.j, c.ilen, c.jlen, &params, c.mode, c.direction, &coord);
  rrho_destroy(&rrho);

  if ( 0 == err)
    {
      ret =  PROTECT(allocVector(INTSXP, 2));
      INTEGER(ret)[0] = coord.i + 1;
      INTEGER(ret)[1] = coord.j + 1;
    }
  else
    {
      ret =  PROTECT(allocVector(INTSXP, 1));
      INTEGER(ret)[0] = NA_INTEGER;
    }
 
  UNPROTECT(1);
  
  return ret;
}
