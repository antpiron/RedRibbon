#include "config.h"

// #ifdef HAVE_R_H

#include "RedRibbon.h"

static void rrho_prediction_init_distance(struct rrho_prediction *pred, SEXP distance, double *vec);
static void rrho_prediction_destroy(struct rrho_prediction *pred);
static int predict_ld(size_t i, size_t j, int flags, double x,
		      struct stats_predict_results *res, void *cls);
static int predict_ld_fit(size_t i, size_t j, int flags, double x,
			  struct stats_predict_results *res, void *cls);


static
int
predict_ld(size_t i, size_t j, int flags, double x,
		  struct stats_predict_results *res, void *cls)
{
  struct rrho_prediction *p = cls;

  if (PERMUTATION_LD == p->tag)
    {
      double r = fabs(p->linear.r[i]);
      res->pvalue = 0;
      res->mse = 0;
      res->r = p->linear.r[i];
      res->y = r * (p->linear.beta0[i] + p->linear.beta1[i] * x) + (1 - r) * stats_ecdf_rand(& p->ecdf);

      return 0;
    }

  return -1;
}

static
int
predict_ld_fit(size_t i, size_t j, int flags, double x,
		  struct stats_predict_results *res, void *cls)
{
  struct rrho_prediction *p = cls;

  if (PERMUTATION_LD_FIT == p->tag)
    {
      const double half = p->dist.half; // 6480.306
      double distance = fabs(p->dist.pos[i] - p->dist.pos[j]);
      double r = half / (half + distance);
      res->pvalue = 0;
      res->mse = 0;
      res->r = r;
      res->y = r * x + (1 - r) * stats_ecdf_rand(& p->ecdf);

      return 0;
    }
  
  return -1;
}

void
rrho_prediction_init_distance(struct rrho_prediction *pred, SEXP distance, double *vec)
{
  size_t n = length(distance);
  ssize_t *corr;
  int ret;

  pred->tag = PERMUTATION_LD_FIT;
  
  ret = stats_permutation_correlated_init(&pred->permutation, n, vec, -1, predict_ld_fit, pred);
  if ( 0 != ret )
    error("Unable to initialize permutation.");

  stats_ecdf_init(&pred->ecdf, n, vec);
  
  corr = malloc(sizeof(ssize_t) * n);

  for (size_t i = 0 ; i < n ; i++)
    corr[i] = INTEGER(distance)[i];

  stats_permutation_correlated_set(&pred->permutation, corr);
  free(corr);
}

void
rrho_prediction_destroy(struct rrho_prediction *pred)
{
  if ( PERMUTATION_LD_FIT == pred->tag )
    {
      stats_permutation_destroy(&pred->permutation);
      stats_ecdf_destroy(&pred->ecdf);
    }
}


// int rrho_permutation_generic(struct rrho *rrho, size_t i, size_t j, size_t ilen, size_t jlen,
//			     void *params, int mode, int direction, int algorithm,
//			     size_t niter, long double pvalue, struct rrho_permutation_result *res);
SEXP
rrho_r_permutation(SEXP i, SEXP j, SEXP ilen, SEXP jlen, SEXP a, SEXP b, SEXP algo_params,
		   SEXP mode, SEXP direction, SEXP algorithm,
		   SEXP correlation,
		   SEXP niter, SEXP pvalue_i, SEXP pvalue_j)
{
  struct rrho rrho;
  struct rrho_result res;
  int length_a = length(a);
  int length_b = length(b);
  struct rrho_rectangle_params_ea params_ea;
  struct rrho_rectangle_params params_classic;
  struct rrho_permutation_result perm_res;
  struct stats_permutation permutation;
  SEXP ret;

  if ( length_a != length_b )
    error("The vectors a and b should be of equal size.");
  if ( ! isReal(a) )
     error("a is not a real.");
  if ( ! isReal(b) )
     error("b is not a real.");

  if ( ! isNull(correlation) )
    {
      printf("HERE %d\n", length(correlation));
      if ( length(correlation) != length_a )
	error("The vectors correlation and a should be of equal size.");

      if ( ! isInteger(correlation))
	error("correlation is not an integer.");
    }

  if ( ! isInteger(pvalue_i) )
     error("pvalue_i is not an integer.");
  if ( ! isInteger(pvalue_j) )
     error("pvalue_j is not an integer.");

  if ( ! isString(mode) )
    error("Mode is not a string.");
  if ( ! isString(direction) )
    error("Direction is not a string.");
  if ( ! isString(algorithm) )
    error("algorithm is not a string.");
  
  if ( ! isInteger(i) )
     error("i is not an integer.");
  if ( ! isInteger(j) )
     error("j is not an integer.");

  if ( ! isInteger(ilen) )
     error("ilen is not an integer.");
  if ( ! isInteger(jlen) )
     error("jlen is not an integer.");

  if ( ! isNewList(algo_params) )
     error("algo_params is not a (new)List.");

  if ( ! isInteger(niter) )
     error("niter is not an integer.");

  
  struct rrho_c c =
    {
     .i = INTEGER(i)[0] - 1, .j = INTEGER(j)[0] - 1,
     .ilen = INTEGER(ilen)[0], .jlen = INTEGER(jlen)[0],
     .a = REAL(a), .b = REAL(b),
     .pvalue_i = INTEGER(pvalue_i)[0], .pvalue_j = INTEGER(pvalue_j)[0],
     .niter = INTEGER(niter)[0],
     .strmode = CHAR(STRING_PTR(mode)[0]),
     .mode = RRHO_HYPER,
     .strdirection = CHAR(STRING_PTR(direction)[0]),
     .direction = 1,
     .stralgorithm = CHAR(STRING_PTR(algorithm)[0]),
     .algorithm = RRHO_CLASSIC
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

  /* enum {
      RRHO_CLASSIC = 0,
      RRHO_EA,
      }; */
  if ( 0 !=  strcmp(c.stralgorithm, "classic") )
    c.algorithm = RRHO_EA;

  
  rrho_init(&rrho, length_a, c.a, c.b);

  
  rrho_generic(&rrho, c.pvalue_i, c.pvalue_j, c.mode, &res);
  c.pvalue = res.pvalue;
  
  void *ptr_params;
  if (RRHO_CLASSIC == c.algorithm)
    {
      SEXP val;

      val = rrho_getListElement(algo_params, "m");
      params_classic.m = (R_NilValue == val) ? c.ilen : INTEGER(val)[0];
      val = rrho_getListElement(algo_params, "n");
      params_classic.n = (R_NilValue == val) ? c.jlen : INTEGER(val)[0];

      ptr_params = &params_classic;
    }
  else
    {
      rrho_init_params_ea(&rrho, &params_ea);
      ptr_params = &params_ea;
    }

  if ( isNull(correlation) )
    {
      stats_permutation_init(&permutation, length_a, c.b);
    }
  else if ( ! isInteger(correlation) )
    {
      ssize_t *corr = malloc(sizeof(ssize_t) * length_a);
      struct rrho_prediction pred;

      stats_ecdf_init(&pred.ecdf, length_a, c.b);
      for (size_t i = 0 ; i < length_a ; i++)
	{
	  corr[i] = INTEGER(correlation)[i];
	  pred.linear.r[i] = 0;
	  pred.linear.beta0[i] = 0;
	  pred.linear.beta1[i] = 1;
	}

      ret = stats_permutation_correlated_init(&permutation, length_a, c.b, -1, predict_ld, &pred);
      if ( 0 != ret )
	error("Unable to initialize permutation.");

      
      stats_permutation_correlated_set(&permutation, corr);
      free(corr);
      stats_ecdf_destroy(&pred.ecdf);
    }
  else
    {
      // should never reach here, already tested previously
      error("correlation is not an integer.");
    }
 
  rrho_permutation_generic(&rrho, c.i, c.j, c.ilen, c.jlen, ptr_params, &permutation, c.mode, c.direction, c.algorithm,
			   c.niter, c.pvalue, &perm_res);
  rrho_destroy(&rrho);
  stats_permutation_destroy(&permutation);


  const char *names[] = {"pvalue", "log_pvalue", "pvalue_ks", "stat_ks", ""};
  ret = PROTECT(Rf_mkNamed(VECSXP, names));

  SEXP Spvalue = PROTECT(Rf_ScalarReal(perm_res.pvalue));
  SET_VECTOR_ELT(ret, 0, Spvalue);
  
  long double log_pvalue = fabsl(-logl(perm_res.pvalue));
  /* Rprintf("pval = %Le, perm pval = %Le, log_pval = %Lf\n", c.pvalue,  perm_res.pvalue, log_pvalue); */
  SEXP Slog_pvalue = PROTECT(Rf_ScalarReal(log_pvalue));
  SET_VECTOR_ELT(ret, 1, Slog_pvalue);
  
  SEXP Spvalue_ks = PROTECT(Rf_ScalarReal(perm_res.pvalue_ks));
  SET_VECTOR_ELT(ret, 2, Spvalue_ks);
  
  SEXP Sstat_ks = PROTECT(Rf_ScalarReal(perm_res.stat_ks));
  SET_VECTOR_ELT(ret, 3, Sstat_ks);
  
  UNPROTECT(5);

  return ret;
}

