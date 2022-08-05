#include <config.h>

// #ifdef HAVE_R_H

#include "RedRibbon.h"


SEXP
rrho_r_rectangle_min_ea(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP a, SEXP b, SEXP mode, SEXP direction)
// int
// rrho_r_rectangle_min_ea(struct rrho *rrho, size_t i, size_t j, size_t ilen, size_t jlen,
// 			struct rrho_coord *coord, int mode, int direction)
{
  struct rrho rrho;
  struct rrho_coord coord;
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
  if ( ! isString(direction) )
    error("Direction is not a string.");
  
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
     .a = REAL(a), .b = REAL(b),
     .strmode = CHAR(STRING_PTR(mode)[0]),
     .strdirection = CHAR(STRING_PTR(direction)[0]),
     .mode = RRHO_HYPER,
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
  
  ret = PROTECT(allocVector(INTSXP, 2));
  struct rrho_rectangle_params_ea params;
  
  rrho_init(&rrho, length_a, c.a, c.b);
  
  rrho_init_params_ea(&rrho, &params);
  params.initial_population_func = rrho_initial_population_func;
  params.initial_population_cls = &rrho;
  
  rrho_rectangle_min_ea(&rrho, c.i, c.j, c.ilen, c.jlen, &params, c.mode, c.direction, &coord);

  // rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, array, RRHO_HYPER);
  INTEGER(ret)[0] = coord.i + 1;
  INTEGER(ret)[1] = coord.j + 1;
  
  rrho_destroy(&rrho);
  
  UNPROTECT(1);
  
  return ret;
}
