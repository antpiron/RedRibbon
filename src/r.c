#include "config.h"

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

static const R_CallMethodDef callMethods[]  = {
  {"rrho_r_rectangle", (DL_FUNC) &rrho_r_rectangle, 10},
  {"rrho_r_rectangle_min", (DL_FUNC) &rrho_r_rectangle_min, 10},
  {"rrho_r_rectangle_min_ea", (DL_FUNC) &rrho_r_rectangle_min_ea, 8},
  {"rrho_r_permutation", (DL_FUNC) &rrho_r_permutation, 14},
  {"rrho_r_rrho", (DL_FUNC) &rrho_r_rrho, 5},
  {"rrho_r_intersect", (DL_FUNC) &rrho_r_intersect, 5},
  {NULL, NULL, 0}
};

void
R_init_RedRibbon(DllInfo *info)
{
   R_registerRoutines(info, NULL, callMethods, NULL, NULL);
}

// #endif
