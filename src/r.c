#include <config.h>

#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

// #ifdef HAVE_R_H

#include "RedRibbon.h"

/* from https://cran.r-project.org/doc/manuals/r-release/R-exts.html#Handling-lists */
/* get the list element named str, or return NULL */
SEXP
rrho_getListElement(SEXP list, const char *str)
{
    SEXP elmt = R_NilValue, names = getAttrib(list, R_NamesSymbol);


    for (int i = 0; i < length(list); i++)
        if(strcmp(CHAR(STRING_ELT(names, i)), str) == 0) {
           elmt = VECTOR_ELT(list, i);
           break;
        }
    return elmt;
}

extern SEXP rrho_r_expression_prediction(SEXP, SEXP);
extern SEXP rrho_r_intersect(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rrho_r_ldfit_prediction(SEXP, SEXP, SEXP);
extern SEXP rrho_r_normalize(SEXP, SEXP, SEXP);
extern SEXP rrho_r_permutation(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rrho_r_rectangle(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rrho_r_rectangle_min(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rrho_r_rectangle_min_ea(SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rrho_r_rrho(SEXP, SEXP, SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
    {"rrho_r_expression_prediction", (DL_FUNC) &rrho_r_expression_prediction,  2},
    {"rrho_r_intersect",             (DL_FUNC) &rrho_r_intersect,              5},
    {"rrho_r_ldfit_prediction",      (DL_FUNC) &rrho_r_ldfit_prediction,       3},
    {"rrho_r_normalize",             (DL_FUNC) &rrho_r_normalize,              3},
    {"rrho_r_permutation",           (DL_FUNC) &rrho_r_permutation,           14},
    {"rrho_r_rectangle",             (DL_FUNC) &rrho_r_rectangle,             10},
    {"rrho_r_rectangle_min",         (DL_FUNC) &rrho_r_rectangle_min,         10},
    {"rrho_r_rectangle_min_ea",      (DL_FUNC) &rrho_r_rectangle_min_ea,       8},
    {"rrho_r_rrho",                  (DL_FUNC) &rrho_r_rrho,                   5},
    {NULL, NULL, 0}
};

void R_init_RedRibbon(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}

// #endif
