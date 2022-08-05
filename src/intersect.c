#include <config.h>

// #ifdef HAVE_R_H

#include "RedRibbon.h"



// void rrho_intersect(struct rrho *rrho, size_t i, size_t j, int directions, struct bitset *bs_res);
SEXP
rrho_r_intersect(SEXP i, SEXP j, SEXP a, SEXP b, SEXP directions)
{
  struct rrho rrho;
  struct bitset bs_res;
  int length_a = length(a);
  int length_b = length(b);
  SEXP ret;

  if ( length_a != length_b )
    error("The vectors a and b should be of equal size.");
  if ( ! isReal(a) )
     error("a is not a real.");
  if ( ! isReal(b) )
     error("b is not a real.");

  if ( ! isString(directions) )
    error("Direction is not a string.");
  
  if ( ! isInteger(i) )
     error("i is not an integer.");
  if ( ! isInteger(j) )
     error("j is not an integer.");

  struct rrho_c c =
    {
     .i = INTEGER(i)[0] - 1, .j = INTEGER(j)[0] - 1,
     .a = REAL(a), .b = REAL(b),
     .strdirection = CHAR(STRING_PTR(directions)[0]),
     .direction = RRHO_DOWN_DOWN
    };

  if ( c.i < 0 )
    error("i should be  greater or equal of 1.");
  if ( c.j < 0 )
    error("j should be  greater or equal of 1.");

  /* enum {
     RRHO_DOWN_DOWN = 0,
     RRHO_UP_UP,
     RRHO_DOWN_UP,
     RRHO_UP_DOWN
     }; */

  if ( 0 ==  strcmp(c.strdirection, "upup") )
    c.direction = RRHO_UP_UP;
  else if ( 0 ==  strcmp(c.strdirection, "downup") )
    c.direction = RRHO_DOWN_UP;
  else if ( 0 ==  strcmp(c.strdirection, "updown") )
    c.direction = RRHO_UP_DOWN;
  else
    c.direction = RRHO_DOWN_DOWN;

  


  rrho_init(&rrho, length_a, c.a, c.b);
  bitset_init(&bs_res, length_a);
  bitset_reset(&bs_res);
  
  rrho_intersect(&rrho, c.i, c.j, c.direction, &bs_res);
  size_t count = bitset_ones(&bs_res);
  // Rprintf("count ones = %zu ; n = %zu ; buf[last] = %" PRIx64 " \n", count, bs_res.n, bs_res.buf[(1000 + 63) / 64 - 1]);
  ret = PROTECT(allocVector(INTSXP, count));

  ssize_t value = -1;
  for (size_t i = 0 ; ; i++) 
    {
      value = bitset_iterate(&bs_res, value);
      if (value < 0)
	break;
      INTEGER(ret)[i] = value + 1;
      // Rprintf("%d\n", value);
    }

  bitset_destroy(&bs_res);
  rrho_destroy(&rrho);
  
  UNPROTECT(1);

  return ret;
}
