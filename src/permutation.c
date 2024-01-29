#include <config.h>

// #ifdef HAVE_R_H
#include <ale/sort.h>

#include "RedRibbon.h"

/* static void rrho_prediction_init_shuffle(struct rrho_prediction *pred, size_t n, double *pvalues_or_fc); */
/* static void rrho_prediction_init_distance(struct rrho_prediction *pred, double half, size_t n, size_t *pos, double *pvalues_or_fc); */
/* static void rrho_prediction_destroy(struct rrho_prediction *pred); */

static double predict_fc(size_t i, double x, void *cls);
static double predict_ld(size_t i, double x, void *cls);
static double predict_ld_fit(size_t i, double x, void *cls);


static
double
predict_fc(size_t i, double x, void *cls)
{
  struct rrho_predict_fc_cls *p = cls;

  return p->beta[i] * x;
}

static
double
predict_ld(size_t i, double x, void *cls)
{
  struct rrho_predict_ld_cls *p = cls;
  const double r = fabs(p->r[i]);
  const size_t rand = floor( stats_unif_rand(0, p->permutation->n) );
  
  return r * x + (1 - r) * p->permutation->vec[rand];
}

static
double
predict_ld_fit(size_t i, double x, void *cls)
{
  struct rrho_predict_ld_fit_cls *p = cls;
  const double half = p->half; // 6480.306
  double distance = fabs( p->position[i] - p->position[ p->permutation->deps[i] ] );
  double r = half / (half + distance);
  const size_t rand = floor( stats_unif_rand(0, p->permutation->n) );
  
  return r * x + (1 - r) * p->permutation->vec[rand];
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
  struct mem_pool pool;
  mem_init(&pool);
  struct rrho rrho;
  struct rrho_result res;
  int length_a = length(a);
  int length_b = length(b);
  struct rrho_rectangle_params_ea params_ea;
  struct rrho_rectangle_params params_classic;
  struct rrho_permutation_result perm_res;
  struct stats_permutation permutation;
  // union
  // {
  struct rrho_predict_fc_cls predict_fc_cls;
  struct rrho_predict_ld_cls predict_ld_cls;
  struct rrho_predict_ld_fit_cls predict_ld_fit_cls;
    // };
  SEXP ret;

  if ( length_a != length_b )
    error("The vectors a and b should be of equal size.");
  if ( ! isReal(a) )
     error("a is not a real.");
  if ( ! isReal(b) )
     error("b is not a real.");

  if ( ! isNull(correlation) && ! isNewList(correlation))
    {
      error("The vectors correlation and a should be NULL or a list.");
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
     .pvalue_i = INTEGER(pvalue_i)[0] - 1, .pvalue_j = INTEGER(pvalue_j)[0] - 1,
     .niter = INTEGER(niter)[0],
     .strmode = CHAR(STRING_PTR(mode)[0]),
     .mode = RRHO_HYPER,
     .strdirection = CHAR(STRING_PTR(direction)[0]),
     .direction = 1,
     .stralgorithm = CHAR(STRING_PTR(algorithm)[0]),
     .algorithm = RRHO_CLASSIC
    };

  if ( c.i < 0 )
    error("i should be greater or equal than 1.");
  if ( c.j < 0 )
    error("j should be greater or equal than 1.");
  
  if ( c.m < c.ilen + c.i )
    error("m should be >= ilen + i - 1.");
  if ( c.n < c.jlen + c.j )
    error("n should be >= jlen + j - 1.");

  if ( c.pvalue_i < c.i )
     error("pvalue_i should be >= i.");
  if ( c.pvalue_j < c.j )
     error("pvalue_j should be >= j.");
  
  if ( c.pvalue_i >= c.i + c.ilen )
    error("%d == pvalue_i should be < i + ilen - 1 == %d.", c.pvalue_i, c.i + c.ilen);
  if ( c.pvalue_j >= c.j + c.jlen )
    error("%d == pvalue_j should < j + ilen - 1 == %d.",  c.pvalue_j, c.j + c.jlen);
  
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
      params_ea.initial_population_func = rrho_initial_population_func;
      params_ea.initial_population_cls = &rrho;
      ptr_params = &params_ea;
    }

  
  stats_permutation_init(&permutation, length_b, c.b);
  if ( ! isNull(correlation)  )
    {
      if (isNewList(correlation) )
	{
	  ssize_t *deps = mem_malloc(&pool, sizeof(ssize_t) * length_b);
	  SEXP deps_sexp = rrho_getListElement(correlation, "deps");
	  
	  for (size_t i = 0 ; i < length_b ; i++)
	    {
	      deps[i] = INTEGER(deps_sexp)[i];
	      deps[i] = (deps[i] <= 0) ? -1 : deps[i] - 1;
	    }

	  if ( Rf_inherits(correlation, "ld_fit") )
	    {
	      SEXP half_sexp, positions_sexp;
	      
	      // half = 6480.306
	      half_sexp = rrho_getListElement(correlation, "half");
	      positions_sexp = rrho_getListElement(correlation, "pos");
	      
	      predict_ld_fit_cls.permutation = &permutation;
	      predict_ld_fit_cls.half = REAL(half_sexp)[0];
	      
	      predict_ld_fit_cls.position = mem_malloc(&pool, sizeof(ssize_t) * length_b);
	      for (size_t i = 0 ; i < length_b ; i++)
		predict_ld_fit_cls.position[i] = INTEGER(positions_sexp)[i];
	      
	      
	      // rrho_prediction_init_distance(&pred, REAL(shalf)[0], length_b, pos, c.b);
	      stats_permutation_dependencies(&permutation,  deps,  predict_ld_fit, &predict_ld_fit_cls);
	    }
	  else if ( Rf_inherits(correlation, "ld") )
	    {
	      SEXP r_sexp = rrho_getListElement(correlation, "r");
	      
	      predict_ld_cls.r = mem_malloc(&pool, sizeof(double) * length_b);
	      
	      for (size_t i = 0 ; i < length_b ; i++)
		predict_ld_cls.r[i] = REAL(r_sexp)[i];
	      predict_ld_cls.permutation = &permutation;

	      stats_permutation_dependencies(&permutation, deps,  predict_fc, &predict_ld_cls);
	    }
	  else if ( Rf_inherits(correlation, "fc") )
	    {
	      SEXP beta_sexp = rrho_getListElement(correlation, "beta");

	      predict_fc_cls.beta = mem_malloc(&pool, sizeof(double) * length_b);
	      
	      for (size_t i = 0 ; i < length_b ; i++)
		predict_fc_cls.beta[i] = REAL(beta_sexp)[i];

	      stats_permutation_dependencies(&permutation, deps,  predict_fc, &predict_fc_cls);
	    }
	  else
	    error("correlation is not if class `ld_fit` or `fc`.");
	}
      else
	{
	  // should never reach here, already tested previously
	  error("correlation is not an list.");
	}
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

  mem_destroy(&pool);
  
  return ret;
}

