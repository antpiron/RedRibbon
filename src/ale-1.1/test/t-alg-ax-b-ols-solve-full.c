#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#include "ale/error.h"
#include "ale/algebra.h"
#include "ale/stats.h"
#include "ale/math.h"

#define MAX_ITER (10)

const double alpha = 0.01;
const double eps = 0.0000001;

enum {
      B_MODE_RANDOM,
      B_MODE_PERFECT,
      B_MODE_CORRELATED
};

enum {
      ARG_ERROR_SD,
      ARG_A,
      ARG_B,
      ARG_INTERCEPT,
      ARG_B_MODE,
      ARG_TEST_COEF,
      ARG_TEST_R2,
      ARG_TEST_FSTAT,
      ARG_TEST_FSTAT_PVALUE,
      ARG_TEST_BETA_STAT,
      ARG_TEST_BETA_PVALUE,
      ARG_TEST_LOOCV_LT,
      ARG_END
};

void
gen_model(char *name, size_t m, size_t n, size_t p, ...)
{
  va_list ap;
  int ret;
  struct alg_ols ols;
  double A[m][n],  X[n][p], B[m][p], X_res[n][p];
  size_t intercept = 1;
  int rand_B = B_MODE_RANDOM;
  double error_sd = 10;
  void *A_param = NULL;
  void *B_param = NULL;
  double (*coef_exp)[p] = NULL;
  double *r2_exp = NULL;
  double *fstat_exp = NULL;
  double *pvalue_exp = NULL;
  double (*beta_stat_exp)[p] = NULL;
  double (*beta_pvalue_exp)[p] = NULL;
  double *loocv_exp = NULL;

  va_start(ap, p); 
  for ( int cont = 1 ; cont ; )
    {
      int arg_type = va_arg(ap, int);
      
      if (ARG_END == arg_type)
	break;
      
      switch (arg_type)
	{
	case ARG_INTERCEPT:
	  intercept = va_arg(ap, size_t);
	  break;
	  
	case ARG_B_MODE:
	  rand_B = va_arg(ap, int);
	  break;
	  
	case ARG_ERROR_SD:
	  error_sd = va_arg(ap, double);
	  break;

	case ARG_A:
	  A_param = va_arg(ap, void*);
	  break;

	case ARG_B:
	  B_param = va_arg(ap, void*);
	  break;

	case ARG_TEST_COEF:
	  coef_exp = va_arg(ap, void*);
	  break;
	  
	case ARG_TEST_R2:
	  r2_exp = va_arg(ap, void*);
	  break;
	  
	case ARG_TEST_FSTAT:
	  fstat_exp = va_arg(ap, void*);
	  break;

	case ARG_TEST_FSTAT_PVALUE:
	  pvalue_exp = va_arg(ap, void*);
	  break;
	  
	case ARG_TEST_BETA_STAT:
	  beta_stat_exp = va_arg(ap, void*);
	  break;

	case ARG_TEST_BETA_PVALUE:
	  beta_pvalue_exp = va_arg(ap, void*);
	  break;
	  
	case ARG_TEST_LOOCV_LT:
	  loocv_exp = va_arg(ap, void*);
	  break;
	  
	default:
	  cont = 0;
	  break;
	}
    }
  va_end(ap);

  printf("\n\n%s\n", name);

  if (A_param)
    memcpy(A, A_param, sizeof(double) * m * n);
  else
    ALG_INIT_M(m, n, A, (intercept && (0 == j))? 1  : stats_unif_rand(0, 1000) );

  // generate B
  if ( B_param )
    memcpy(B, B_param, sizeof(double) * m * p);
  else
    {
      if ( B_MODE_RANDOM == rand_B )
	ALG_INIT_M(m, p, B, stats_unif_rand(0, 1000) );
      else
	{
	  ALG_INIT_M(n, p, X_res, stats_unif_rand(0, 1000) );
	  
	  alg_mul_m_m(m, n, p, A, X_res, B);
	  
	  if ( B_MODE_CORRELATED  == rand_B)
	    {
	      ALG_ADD_M(m, p, B, stats_norm_rand(0, error_sd) );
	    }
	}
    }

  printf("\nA=\n");
  print_m(m, n, A);
  printf("\nB=\n");
  print_m(m, p, B);
  if ( B_MODE_RANDOM != rand_B)
    {
      printf("\nX_res=\n");
      print_m(n, p, X_res);
    }

  ret = alg_AX_B_OLS_init(&ols, m, n, p, A, B, X);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_init ret = %d\n != 0", name, ret);

  printf("\nX=\n");
  print_m(n, p, X);

  if ( B_MODE_PERFECT  == rand_B )
    {
      for (size_t i = 0 ; i < n ; i++)
	for (size_t j = 0; j < p ; j++)
	  {
	    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(X[i][j], X_res[i][j], eps),
				  "FAIL: %s alg_AX_B_OLS_solve_full() X[%ld, %ld] = %f != %f = X_res[%ld, %ld]\n",
				  name, i, j, X[i][j], X_res[i][j], i, j);
	  }
    }

  if ( NULL != coef_exp )
    {
      for (size_t i = 0; i < n ; i++)
	for (size_t j = 0; j < p ; j++)
	  {
	    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(X[i][j], coef_exp[i][j], eps),
				  "FAIL: %s alg_AX_B_OLS_solve_full() X[%ld, %ld] = %f != %f = coef_exp[%ld, %ld]\n",
				  name, i, j, X[i][j], coef_exp[i][j], i, j);
	  }
    }

  ret = alg_AX_B_OLS_statistics(&ols, intercept);
  ERROR_UNDEF_FATAL_FMT(ret < 0, "FAIL: %s alg_AX_B_OLS_statistics() ret = %d\n != 0", name, ret);

  printf("\nAX=\n");
  print_m(m, p, ols.AX);
  printf("\nmeans=\n");
  print_v(p, ols.means);

  printf("\nrss=\n");
  print_v(p, ols.rss);
  printf("\nmss=\n");
  print_v(p, ols.mss);

  printf("\nr²=\n");
  print_v(p, ols.r_squared);

  printf("\nF-stat=\n");
  print_v(p, ols.score);

  printf("\nF-pval=\n");
  print_v(p, ols.pvalue);

  printf("\nbeta-stat=\n");
  print_m(n, p, ols.beta_score);

  printf("\nbeta-pvalue=\n");
  print_m(n, p, ols.beta_pvalue);

  printf("\nloocv=\n");
  print_v(p, ols.loocv);

  if ( NULL != r2_exp )
    {
      for (size_t i = 0; i < p ; i++)
	{
	  double *r2 = ols.r_squared;
	  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(r2[i], r2_exp[i], eps),
				"FAIL: %s alg_AX_B_OLS_solve_full() r2[%ld] = %f != %f = r2_exp[%ld]\n",
				name, i, r2[i], r2_exp[i], i);
	}
    }

  if ( NULL != fstat_exp )
    {
      for (size_t i = 0; i < p ; i++)
	{
	  double *score = ols.score;
	  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(score[i], fstat_exp[i], eps),
				"FAIL: %s alg_AX_B_OLS_solve_full() score[%ld] = %f != %f = fstat_exp[%ld]\n",
				name, i, score[i], fstat_exp[i], i);
	}
    }

  if ( NULL != pvalue_exp )
    {
      for (size_t i = 0; i < p ; i++)
	{
	  double *pvalue = ols.pvalue;
	  ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(pvalue[i], pvalue_exp[i], eps),
				"FAIL: %s alg_AX_B_OLS_solve_full() pvalue[%ld] = %f != %f = pvalue_exp[%ld]\n",
				name, i, pvalue[i], pvalue_exp[i], i);
	}
    }

  if ( NULL != beta_stat_exp )
    {
      for (size_t i = 0; i < n ; i++)
	for (size_t j = 0; j < p ; j++)
	  {
	    double (*score)[p] = ols.beta_score;
	    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(score[i][j], beta_stat_exp[i][j], eps),
				  "FAIL: %s alg_AX_B_OLS_solve_full() beta_stat[%ld, %ld] = %f != %f = beta_stat_exp[%ld, %ld]\n",
				  name, i, j, score[i][j], beta_stat_exp[i][j], i, j);
	  }
    }

  if ( NULL != beta_pvalue_exp )
    {
      for (size_t i = 0; i < n ; i++)
	for (size_t j = 0; j < p ; j++)
	  {
	    double (*pvalue)[p] = ols.beta_pvalue;
	    ERROR_UNDEF_FATAL_FMT(0 != ale_cmp_double(pvalue[i][j], beta_pvalue_exp[i][j], eps),
				  "FAIL: %s alg_AX_B_OLS_solve_full() beta_pvalue[%ld, %ld] = %f != %f = beta_pvalue_exp[%ld, %ld]\n",
				  name, i, j, pvalue[i][j], beta_pvalue_exp[i][j], i, j);
	  }
    }

    if ( NULL != loocv_exp )
    {
      for (size_t i = 0; i < p ; i++)
	{
	  double *loocv = ols.loocv;
	  ERROR_UNDEF_FATAL_FMT(loocv[i] >= loocv_exp[i],
				"FAIL: %s alg_AX_B_OLS_solve_full() loocv[%ld] = %e >= %e = loocv_exp[%ld]\n",
				name, i, loocv[i], loocv_exp[i], i);
	}
    }


  alg_AX_B_OLS_destroy(&ols);
}

int
main(int argc, char *argv[argc])
{
  /* 
     A <- rbind(c(751.411799, 53.406825),
                c(878.793326, 184.466027),
		c(934.970735, 400.979769),
		c(248.113489, 6.991620),
		c(969.135640, 443.855135),
		c(397.544328, 330.078718))
     
     B <- cbind(c(216.396249,
	          852.721703,
		  234.933271,
		  446.330890,
		  161.112500,
		  863.669808))

     model <- lm(B ~ A)
     summary(model)
     
Call:
lm(formula = B ~ A)

Residuals:
     1      2      3      4      5      6 
-173.2  494.3 -149.2 -183.5 -216.6  228.2 

Coefficients:
            Estimate Std. Error t value Pr(>|t|)
(Intercept) 752.2431   419.0238   1.795    0.170
A1           -0.5003     0.6670  -0.750    0.508
A2            0.2487     1.1021   0.226    0.836

Residual standard error: 378.3 on 3 degrees of freedom
Multiple R-squared:  0.1691,	Adjusted R-squared:  -0.3848 
F-statistic: 0.3053 on 2 and 3 DF,  p-value: 0.7574

     sum(model$residuals^2) 
     meanB <- mean(B)
     X  <- cbind(model$coefficients)
     AX  <- (cbind(1, A) %*% X)
     sum( (AX - meanB)^2 )
  */

  gen_model("fixed random with intercept", 6, 3, 1,
	    ARG_INTERCEPT, 1,
	    ARG_B_MODE, B_MODE_RANDOM, 
	    ARG_A, (double[6][3]) { {1.000000, 751.411799, 53.406825},	
				     {1.000000, 878.793326, 184.466027},
				       {1.000000, 934.970735, 400.979769},
					 {1.000000, 248.113489, 6.991620 },
					   {1.000000, 969.135640, 443.855135},
					     {1.000000, 397.544328, 330.078718 } },
	    ARG_B, (double[6]) {216.396249, 852.721703, 234.933271, 446.330890, 161.112500, 863.669808},
	    ARG_TEST_COEF, (double[3][1]) { {752.2431179}, {-0.5003319}, {0.2486852} }, 
	    ARG_TEST_R2, (double[1]) { 0.1691208 },
	    ARG_TEST_FSTAT, (double[1]) { 0.3053166 },
	    ARG_TEST_FSTAT_PVALUE, (double[1]) { 0.7573678 },
	    ARG_TEST_BETA_STAT, (double[3][1]) { {1.7952275}, {-0.7501315}, {0.2256522} },
	    ARG_TEST_BETA_PVALUE, (double[3][1]) { {0.1704931}, {0.5076460}, {0.8359707} },
	    ARG_END);

  gen_model("perfect with intercept", 6, 3, 2,
	    ARG_INTERCEPT, 1,
	    ARG_B_MODE, B_MODE_PERFECT,
	    ARG_TEST_R2, (double[2]) { 1, 1 },
	    ARG_TEST_FSTAT_PVALUE, (double[2]) { 0, 0 },
	    ARG_TEST_BETA_PVALUE, (double[3][2]) { {0, 0}, {0, 0}, {0, 0} },
	    ARG_TEST_LOOCV_LT, (double[2]) { eps, eps },
	    ARG_END);

  gen_model("correlation with intercept", 20, 3, 3,
	    ARG_INTERCEPT, 1,
	    ARG_B_MODE, B_MODE_CORRELATED,
	    ARG_ERROR_SD, 1.0,
	    ARG_TEST_R2, (double[3]) { 1, 1, 1},
	    ARG_TEST_FSTAT_PVALUE, (double[3]) { 0, 0, 0 },
	    ARG_END);

  gen_model("random with intercept", 30, 3, 1,
	    ARG_INTERCEPT, 1,
	    ARG_B_MODE, B_MODE_RANDOM, 
	    ARG_END);

  return EXIT_SUCCESS;
}
