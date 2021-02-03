#include "config.h"

// #ifdef HAVE_R_H

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <string.h>

#include "rrho.h"

// TODO: clean this R shit

struct rrho_c
{
  int i, j, ilen, jlen, m, n;
  double *a, *b;
  const char *strmode;
  const char *strdirection;
  int mode;
  int direction;
  int log;
};

SEXP
rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP log_flag)
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

  if ( ! isLogical(log_flag) )
     error("log_flag is not a boolean.");

  
  struct rrho_c c =
    {
     .i = INTEGER(i)[0] - 1, .j = INTEGER(j)[0] - 1,
     .ilen = INTEGER(ilen)[0], .jlen = INTEGER(jlen)[0],
     .m = INTEGER(m)[0], .n = INTEGER(n)[0],
     .a = REAL(a), .b = REAL(b),
     .strmode = CHAR(STRING_PTR(mode)[0]),
     .mode = RRHO_HYPER,
     .log = LOGICAL(log_flag)[0]
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

  double (*c_array)[c.n] = malloc(sizeof(double) * c.m * c.n);
  rrho_init(&rrho, length_a, c.a, c.b);
  rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, c_array, c.mode, c.log);
  rrho_destroy(&rrho);

  
  // matrix are column-wise in R, so transpose needed
  ret = PROTECT(allocMatrix(REALSXP, c.m, c.n));
  double (*array)[c.m] = (void*) REAL(ret);

  alg_transpose(c.m, c.n, c_array, array);
  
  free(c_array);

  UNPROTECT(1);
  
  return ret;
}

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
  err = rrho_rectangle_min(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, &coord, c.mode, c.direction);
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

  rrho_init(&rrho, length_a, c.a, c.b);

  rrho_rectangle_min_ea(&rrho, c.i, c.j, c.ilen, c.jlen, &coord, c.mode, c.direction);
  // rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, array, RRHO_HYPER);
  INTEGER(ret)[0] = coord.i + 1;
  INTEGER(ret)[1] = coord.j + 1;
  
  rrho_destroy(&rrho);
  
  UNPROTECT(1);
  
  return ret;
}

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



  const char *names[] = {"pvalue", "direction", "fdr", "count", ""};
  ret = PROTECT(Rf_mkNamed(VECSXP, names));

  rrho_init(&rrho, length_a, c.a, c.b);

  rrho_generic(&rrho, c.i, c.j, &res, c.mode);
  // rrho_rectangle(&rrho, c.i, c.j, c.ilen, c.jlen, c.m, c.n, array, RRHO_HYPER);

  SEXP pvalue = PROTECT(Rf_ScalarReal(res.pvalue));
  SET_VECTOR_ELT(ret, 0, pvalue);
  SEXP direction = PROTECT(Rf_ScalarInteger(res.direction));
  SET_VECTOR_ELT(ret, 1, direction);
  SEXP fdr = PROTECT(Rf_ScalarReal(res.fdr));
  SET_VECTOR_ELT(ret, 2, fdr);
  SEXP count = PROTECT(Rf_ScalarInteger(res.count));
  SET_VECTOR_ELT(ret, 3, count);
  
  rrho_destroy(&rrho);
  
  UNPROTECT(5);

  return ret;
}

static const R_CallMethodDef callMethods[]  = {
  {"rrho_r_rectangle", (DL_FUNC) &rrho_r_rectangle, 10},
  {"rrho_r_rectangle_min", (DL_FUNC) &rrho_r_rectangle_min, 10},
  {"rrho_r_rectangle_min_ea", (DL_FUNC) &rrho_r_rectangle_min_ea, 8},
  {"rrho_r_rrho", (DL_FUNC) &rrho_r_rrho, 5},
  {NULL, NULL, 0}
};

void
R_init_RedRibbon(DllInfo *info)
{
   R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}

// #endif
