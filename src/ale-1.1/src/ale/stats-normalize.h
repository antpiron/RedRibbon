#ifndef __ALE_STATS_NORMALIZE_H
#define __ALE_STATS_NORMALIZE_H

#include <stddef.h>
#include <inttypes.h>


enum {
      NORMALIZE_EDIVZERO = 1,
      NORMALIZE_EVARZERO,
      NORMALIZE_EMEANZERO,
      NORMALIZE_ERANGE,
      NORMALIZE_INVALID_MODE
};

enum {
      STATS_NORM_LS_MEAN = 1,
      STATS_NORM_LS_VARIANCE = 2,
      STATS_NORM_POISSON = 4,
      STATS_NORM_GEOM_MEAN = 8,
      
      STATS_PARAM_GEOM_MEAN = 128
};


int stats_normalize_beta(size_t m, size_t n, size_t r,
			 const double mat[m][n], const size_t ref[r], double beta[n],
			 unsigned int mode);
void stats_normalize_samples(size_t m, size_t n, double dst[m][n],
			     const double src[m][n], const double beta[n]);


/* *****************
 *  PRIVATE */

int stats_normalize_beta_ls(size_t m, size_t n, size_t r,
			    const double mat[m][n], const size_t ref[r], double beta[n],
			    unsigned int options);
int stats_normalize_beta_poisson(size_t m, size_t n, size_t r,
				 const double mat[m][n], const size_t ref[r], double beta[n],
				 int options);

int stats_normalize_geometric_mean(size_t m, size_t n, size_t r,
				   const double mat[m][n], const size_t ref[r], double beta[n]);

#endif
