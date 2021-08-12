#ifndef __REDRIBBON_H
#define __REDRIBBON_H

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <string.h>

#include <rrho.h>

// TODO: clean this R shit

struct rrho_c
{
  int i, j, ilen, jlen, m, n;
  double *a, *b;
  const char *strmode;
  const char *strdirection;
  const char *stralgorithm;
  int mode;
  int direction;
  int algorithm;
  int log;
  int niter;
  long double pvalue;
  int pvalue_i, pvalue_j;
};

enum { RRHO_PERMUTATION_SHUFFLE = 0, RRHO_PERMUTATION_FC, RRHO_PERMUTATION_LD_FIT,  RRHO_PERMUTATION_LD};

struct rrho_predict_fc_cls
{
  double *beta;
};

struct rrho_predict_ld_cls
{
  struct stats_permutation *permutation;
  double *r;
};

struct rrho_predict_ld_fit_cls
{
  struct stats_permutation *permutation;
  double half;
  ssize_t *position;
};



SEXP rrho_r_rectangle(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP log_flag);
SEXP rrho_r_rectangle_min(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP m, SEXP n, SEXP a, SEXP b, SEXP mode, SEXP direction);
SEXP rrho_r_rectangle_min_ea(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP a, SEXP b, SEXP mode, SEXP direction);


SEXP rrho_r_permutation(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP a, SEXP b, SEXP algo_params,
			SEXP mode, SEXP direction, SEXP algorithm,
			SEXP correlation,
			SEXP niter, SEXP pvalue_i, SEXP pvalue_j);
SEXP rrho_r_rrho(SEXP i, SEXP j, SEXP a, SEXP b, SEXP mode);
SEXP rrho_r_intersect(SEXP i, SEXP j, SEXP a, SEXP b, SEXP directions);


// Helper functions
SEXP rrho_getListElement(SEXP list, const char *str);
void R_init_RedRibbon(DllInfo *info);
int rrho_initial_population_func(struct rrho_coord *coord, size_t index,
				 struct rrho_rectangle_params_ea *params, void *cls);


#endif
