#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "ale/stats.h"
#include "ale/math.h"

#define GENERIC_FUNC(SUFFIX,TYPE)					\
  TYPE									\
  stats_norm_std_rand##SUFFIX()						\
  {									\
    /* https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform */	\
    static int flag = 1;						\
    static TYPE val;							\
    TYPE ret;								\
									\
    if (flag)								\
      {									\
	TYPE s,u,v;							\
	do								\
	  {								\
	    u = stats_unif_rand##SUFFIX(-1,1);				\
	    v = stats_unif_rand##SUFFIX(-1,1);				\
	    s = u*u+v*v;						\
	  }								\
	while (s >= 1.0 || s == 0.0);					\
									\
	TYPE root = sqrt##SUFFIX(-2.0*log(s)/s);			\
	val = u*root;							\
	ret = v*root;							\
	flag = 0;							\
      }									\
    else								\
      {									\
	flag = 1;							\
	ret = val;							\
      }									\
									\
    return ret;								\
  }									\
									\
  TYPE									\
  stats_norm_rand##SUFFIX(TYPE mu, TYPE sig)				\
  {									\
    return mu + sig * stats_norm_std_rand##SUFFIX();			\
  }									\
									\
  TYPE									\
  stats_norm_std_f##SUFFIX(TYPE x)					\
  {									\
    									\
    return exp##SUFFIX(- x * x / 2) / sqrt##SUFFIX(2*M_PI);		\
  }									\
									\
  TYPE									\
  stats_norm_std_F##SUFFIX(TYPE x)					\
  {									\
    /* http://www.stat.tamu.edu/~jnewton/604/chap3.pdf */		\
    if (x < 0)								\
      return 1 - stats_norm_std_F##SUFFIX(-x);				\
    									\
    return 0.5 + 0.5 * ale_rilgamma##SUFFIX(x*x*0.5, 0.5);		\
  }									\
  									\
  TYPE									\
  stats_norm_F##SUFFIX(TYPE x, TYPE mu, TYPE sig)			\
  {									\
    return stats_norm_std_F##SUFFIX( (x - mu) / sig );			\
  }


GENERIC_FUNC(,double)
GENERIC_FUNC(l,long double)
