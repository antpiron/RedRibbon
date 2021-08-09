#ifndef __CORRELATION_H
#define __CORRELATION_H

#include <stddef.h>
#include <unistd.h>

int rrho_expression_prediction(size_t m, size_t n, const double mat[m][n], ssize_t index[m], double beta[2][m]);

#endif
