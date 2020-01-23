#ifndef __RRHO_H
#define __RRHO_H

#include <stddef.h>
#include "ale.h"

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
  size_t count;
};

int rrho_init(struct rrho *rrho, size_t n, double a[n], double b[n]);
int rrho_hyper(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res);
int rrho_destroy(struct rrho *rrho);

#endif
