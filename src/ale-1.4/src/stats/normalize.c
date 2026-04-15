
#include "ale/stats-normalize.h"
#include "ale/stats.h"
#include "ale/error.h"

#include <stdlib.h>
#include <math.h>

int
stats_normalize_beta_ls(size_t m, size_t n, size_t r,
			   const double mat[m][n], const size_t ref[r], double beta[n],
			   unsigned int options)
{
  if ( ! ((STATS_NORM_LS_VARIANCE | STATS_NORM_LS_MEAN) & options) )
    ERROR_CUSTOM_RET(1, -1, NORMALIZE_INVALID_MODE);
  
  double *w = malloc( r * sizeof(double) );
  double *mu = malloc( r * sizeof(double) );
  int ret = -1;
  double (*mean_func)(size_t n, const double v[n]) = (STATS_PARAM_GEOM_MEAN & options) ? stats_geom_mean : stats_mean;

  for (size_t i = 0 ; i < r ; i++)
    mu[i] = mean_func(n, mat[ref[i]]);

  if ( STATS_NORM_LS_VARIANCE & options )
    {
      for (size_t i = 0 ; i < r ; i++)
	{
	  double var = stats_var(n, mat[ref[i]]);
	  ERROR_CUSTOM_GOTO(0 == var, NORMALIZE_EVARZERO, ERROR_LS);
	  w[i] = 1.0 / var;
	  // printf("ref[%zu] = %zu ; var = %f\n", i, ref[i], var);
	}
    }
  else if ( STATS_NORM_LS_MEAN & options )
    {
      for (size_t i = 0 ; i < r ; i++)
	{
	  ERROR_CUSTOM_GOTO(0 == mu[i], NORMALIZE_EMEANZERO, ERROR_LS);
	  w[i] = 1.0 / mu[i];
	}
    }
  else
    ERROR_CUSTOM_GOTO(1, NORMALIZE_INVALID_MODE, ERROR_LS);

  // TODO: switch order of loop for cache
  for (size_t s = 0 ; s < n ; s++)
    {
      double denom = 0;
      double num = 0;
      for (size_t i = 0 ; i < r ; i++)
	{
	  double x =  mat[ref[i]][s];
	  num +=  w[i] * x * mu[i];
	  denom += w[i] * x * x ;
	}
      
      ERROR_CUSTOM_GOTO(0 == denom, NORMALIZE_EDIVZERO, ERROR_LS);

      // ERROR_UNDEF_RET(0 == denom, -1);
      beta[s] = num / denom;
    }
  ret = 0;
  
 ERROR_LS:
  free(w);
  free(mu);

  return ret;
}

int
stats_normalize_beta_poisson(size_t m, size_t n, size_t r,
			     const double mat[m][n], const size_t ref[r], double beta[n],
			     int options)
{
  double *mumu = malloc( r * sizeof(double) );
  int ret = -1;
  double (*mean_func)(size_t n, const double v[n]) = (STATS_PARAM_GEOM_MEAN & options) ? stats_geom_mean : stats_mean;

    
  for (size_t i = 0 ; i < r ; i++)
    {
      double mu = mean_func(n, mat[ref[i]]);
      mumu[i] = mu * mu;
    }

  // TODO: switch order of loop for cache
  for (size_t s = 0 ; s < n ; s++)
    {
      double denom = 0;
      double num = 0;
      for (size_t i = 0 ; i < r ; i++)
	{
	  double x = mat[ref[i]][s];
	  ERROR_CUSTOM_GOTO(0 == x, NORMALIZE_EDIVZERO, ERROR_POISSON);
	  
	  num += mumu[i] / x;
	  denom += x;
	}
      beta[s] = sqrt(num / denom);
    }
  ret = 0;

 ERROR_POISSON:
  free(mumu);
  
  return ret;
}


int
stats_normalize_geometric_mean(size_t m, size_t n, size_t r,
			    const double mat[m][n], const size_t ref[r], double beta[n])
{
  for (size_t i = 0 ; i < n ; i++)
    beta[i] = 0;

  for (size_t i = 0 ; i < r ; i++)
    {
      for (size_t j = 0 ; j < n ; j++)
	{
	  double x = mat[ ref[i] ] [j];
	  ERROR_CUSTOM_RET(x <= 0, NORMALIZE_ERANGE, -1);
	  
	  beta[j] +=  log(x);
	}
    }

  double frac = 1.0 / (double) r;
  double log_first =  frac * beta[0];
  beta[0] = 1.0;
  for (size_t i = 1 ; i < n ; i++)
    {
      double log_next = frac * beta[i];
      beta[i] = exp(log_first - log_next);
    }

  return 0;
}


// TODO: add test
int
stats_normalize_beta(size_t m, size_t n, size_t r,
		     const double mat[m][n], const size_t ref[r], double beta[n],
		     unsigned int mode)
{
  if ( (STATS_NORM_LS_MEAN & mode) || (STATS_NORM_LS_VARIANCE & mode) )
    return stats_normalize_beta_ls(m, n, r, mat, ref, beta, mode);
  else if (STATS_NORM_POISSON & mode)
    return stats_normalize_beta_poisson(m, n, r, mat, ref, beta, mode);
  else if (STATS_NORM_GEOM_MEAN & mode)
    return stats_normalize_geometric_mean(m, n, r, mat, ref, beta);

  ERROR_CUSTOM_RET(1, NORMALIZE_INVALID_MODE, -1);
  return 0;
}

// TODO: add test
void
stats_normalize_samples(size_t m, size_t n, double dst[m][n],
			const double src[m][n], const double beta[n])
{
#pragma omp parallel for
  for (size_t i = 0 ; i < m ; i++)
#pragma omp simd
    for (size_t j = 0 ;  j < n ; j++)
      dst[i][j] = beta[j] * src[i][j];     
}
