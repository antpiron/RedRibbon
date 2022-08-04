#ifndef __SKIPLIST_H
#define __SKIPLIST_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/types.h>
#include <stddef.h>
#include <ale/error.h>

static inline ssize_t	
skl_randomlevel(size_t maxlevel)
{					
  uint64_t rnd = rand();	
  size_t lvl = 0;
    
  while ( 1ull == (rnd & 1) && lvl < (maxlevel)-1 )
    { lvl++; rnd >>= 1; }

  return lvl+1;
}


#define SKL_INIT(name,maxlevel,keytype,valuetype,cmp_func)		\
  struct skl_##name##_node						\
  {									\
    keytype key;							\
    valuetype value;							\
    struct skl_##name##_node *forward[maxlevel];			\
  };									\
  									\
  static inline struct skl_##name##_node*				\
  skl_##name##_makeNode(size_t newLevel, keytype key, valuetype value)	\
  {									\
    struct skl_##name##_node *node =					\
      malloc(offsetof(struct skl_##name##_node,forward) +		\
	     sizeof(struct skl_##name##_node*) * newLevel);		\
    ERROR_UNDEF_FATAL(NULL == node, "skl_makeNode() Failed to allocate memory\n"); \
									\
    node->key = key;							\
    node->value = value;						\
    									\
    return node;							\
  }									\
									\
  static inline void							\
  skl_##name##_destroyNode(struct skl_##name##_node *node)		\
  {									\
    free(node);								\
  }									\
									\
  struct skl_##name							\
  {									\
    ssize_t level;							\
    struct skl_##name##_node header;					\
  };									\
									\
  static inline void							\
  skl_##name##_init(struct skl_##name *skl)				\
  {									\
    skl->level = 0;							\
    for (ssize_t i = 0 ; i < (maxlevel) ; i++)				\
      skl->header.forward[i] = NULL;					\
  }								 	\
  									\
  static inline void							\
  skl_##name##_destroy_full(struct skl_##name *skl,			\
			    void (*destroy_key_func)(keytype key),	\
			    void (*destroy_value_func)(valuetype value)) \
  {									\
    if ( skl->level > 0)						\
      {									\
	struct skl_##name##_node *node = skl->header.forward[0];	\
	while (NULL != node)						\
	  {								\
	    struct skl_##name##_node *next = node->forward[0];		\
	    if (NULL != destroy_key_func)				\
	      destroy_key_func(node->key);				\
	    if (NULL != destroy_value_func)				\
	      destroy_value_func(node->value);				\
	    skl_##name##_destroyNode(node);				\
	    node = next;						\
	  }								\
      }									\
  }								 	\
  									\
  static inline void							\
  skl_##name##_destroy(struct skl_##name *skl)				\
  {									\
    skl_##name##_destroy_full(skl, NULL, NULL);				\
  }									\
									\
  static inline int							\
  skl_##name##_search(struct skl_##name *skl, keytype key,		\
		      struct skl_##name##_node **node)			\
  {									\
    struct skl_##name##_node *x = &skl->header;				\
    int cmp = -1;							\
									\
    for (ssize_t i = skl->level-1 ; i > -1 ; i--)			\
      {									\
	while (NULL != x->forward[i]					\
	       && (cmp = cmp_func(x->forward[i]->key, key)) < 0 )	\
	  x = x->forward[i];						\
      }									\
    									\
    if (0 == cmp)							\
      {									\
	*node = x->forward[0];						\
	return 1;							\
      }									\
    *node = x;								\
    return 0;								\
  }									\
									\
  static inline int							\
  skl_##name##_search_update(struct skl_##name *skl, keytype key,	\
			     struct skl_##name##_node **node,		\
			     struct skl_##name##_node *update[maxlevel]) \
  {									\
    struct skl_##name##_node *x = &skl->header;				\
    int cmp = -1;							\
									\
    for (ssize_t i = skl->level-1 ; i > -1 ; i--)			\
      {									\
	while (NULL != x->forward[i]					\
	       && (cmp = cmp_func(x->forward[i]->key, key)) < 0 )		\
	  x = x->forward[i];						\
	update[i] = x;							\
      }									\
									\
    *node = x->forward[0];						\
    if (0 == cmp)							\
      return 1;								\
									\
    return 0;								\
  }									\
									\
  static inline int							\
  skl_##name##_insert(struct skl_##name *skl, keytype key,		\
		      valuetype value, valuetype *oldvalue)		\
  {									\
    struct skl_##name##_node *x;					\
    struct skl_##name##_node *update[maxlevel];				\
									\
    if ( skl_##name##_search_update(skl, key, &x, update) )		\
      {									\
	if (NULL != oldvalue)						\
	  *oldvalue = x->value;						\
	x->value = value;						\
	return 1;							\
      }									\
									\
    ssize_t newLevel = skl_randomlevel(maxlevel);			\
    if (newLevel > skl->level)						\
      {									\
	for (ssize_t i = newLevel-1 ; i >=  skl->level ; i--)		\
	  update[i] = &skl->header;					\
	skl->level = newLevel;						\
      }									\
    x = skl_##name##_makeNode(newLevel, key, value);			\
    for (ssize_t i = 0 ; i < newLevel ; i++)				\
      {									\
	x->forward[i] = update[i]->forward[i];				\
	update[i]->forward[i] = x;					\
      }									\
									\
    return 0;								\
  }									\
									\
  static inline int							\
  skl_##name##_delete(struct skl_##name *skl, keytype key,		\
		      keytype *oldkey, valuetype *oldvalue)		\
  {									\
    struct skl_##name##_node *x;					\
    struct skl_##name##_node *update[maxlevel];				\
									\
    if ( skl_##name##_search_update(skl, key, &x, update) )		\
      {									\
	if (NULL != oldvalue)						\
	  *oldvalue = x->value;						\
	if (NULL != oldkey)						\
	  *oldkey = x->key;						\
									\
	for (ssize_t i = 0 ; i < skl->level ; i++)			\
	  {								\
	    if (update[i]->forward[i] != x)				\
	      break;							\
	    update[i]->forward[i] = x->forward[i] ;			\
	  }								\
	skl_##name##_destroyNode(x);					\
	for ( ; skl->level > 0						\
		&& NULL == skl->header.forward[skl->level-1] ;		\
	      skl->level--) ;						\
	return 1;							\
      }									\
									\
    return 0;								\
  }									\

#endif
