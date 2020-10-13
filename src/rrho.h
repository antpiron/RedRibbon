#ifndef __RRHO_H
#define __RRHO_H

#include <stddef.h>
#include "ale.h"

enum {
      RRHO_HYPER = 0,
      RRHO_HYPER_TWO_TAILED,
      RRHO_HYPER_TWO_TAILED_R_MODULE
};

struct rrho
{
  size_t n;
  double *a, *b;
  size_t *index_a, *index_b;
  struct bitset bs_a;
  struct bitset bs_b;
  struct bitset bs_and;
  size_t n_a, n_b;
};

struct rrho_result
{
  double pvalue;
  double fdr;
  int direction;
  size_t count;
};

struct rrho_coord
{
  size_t i, j;
};

int rrho_init(struct rrho *rrho, size_t n, double a[n], double b[n]);
int rrho_destroy(struct rrho *rrho);

int rrho_hyper_two_tailed_as_r_module(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res);
int rrho_hyper_two_tailed(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res);
int rrho_hyper(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res);
static inline int rrho_generic(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res, int mode);

int rrho_rectangle(struct rrho *rrho, size_t i, size_t j, size_t ilen, size_t jlen,
		   size_t m, size_t n, double dst[m][n],
		   int mode);
int rrho_rectangle_min_ea(struct rrho *rrho, size_t i, size_t j, size_t ilen, size_t jlen,
			  struct rrho_coord *coord, int mode, int direction);

// inline

static inline int
rrho_generic(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res, int mode)
{
  if (RRHO_HYPER_TWO_TAILED == mode)
    return rrho_hyper_two_tailed(rrho, i, j, res);

  if (RRHO_HYPER_TWO_TAILED_R_MODULE == mode)
    return rrho_hyper_two_tailed_as_r_module(rrho, i, j, res);

  return rrho_hyper(rrho, i, j, res);
}

#endif
