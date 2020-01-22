#ifndef __RRHO_H
#define __RRHO_H

#include <stddef.h>

struct rrho
{
  size_t n;
  double *a, *b;
  size_t *index_a, *index_b;
};

struct rrho_result
{
  double pvalue;
  double fdr;
};

int rrho_init(struct rrho *rrho, size_t n, double a[n], double b[n]);
int rrho_hyper(struct rrho *rrho, size_t i, size_t j, struct rrho_result *res);
int rrho_destroy(struct rrho *rrho);

#endif
