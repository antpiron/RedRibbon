#ifndef __SORT_H
#define __SORT_H

#include <stddef.h>

int sort_compar_double(const void *p1, const void *p2, void *arg);
int sort_compar_doublel(const void *p1, const void *p2, void *arg);
int sort_compar_double_decreasing(const void *p1, const void *p2, void *arg);
int sort_compar_double_decreasingl(const void *p1, const void *p2, void *arg);
int sort_compar_size_t(const void *p1, const void *p2, void *arg);
int sort_compar_ssize_t(const void *p1, const void *p2, void *arg);

void sort_q_indirect(size_t *index, void *base, size_t nmemb, size_t size,
		     int (*compar)(const void *, const void *, void *),
		     void *arg);



#endif
