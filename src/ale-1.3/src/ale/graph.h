#ifndef __GRAPH_H
#define __GRAPH_H

#include <ale/bitset.h>
#include <ale/stack.h>
#include <inttypes.h>

STACK_INIT(edges, struct bitset)

#ifndef GRAPH_MAX_RECURSION_DEPTH
#  define GRAPH_MAX_RECURSION_DEPTH (1ll << 10)
#endif
  
enum { GRAPH_ERROR_OUT_OF_RANGE = 1,
       GRAPH_ERROR_TOO_MANY_RECURSIVE_CALLS};

struct graph
{
  struct bitset nodes;
  struct stack_edges edges;
};

void graph_init(struct graph *g);
void graph_destroy(struct graph *g);

ssize_t graph_add_node(struct graph *g);
ssize_t graph_get_node(struct graph *g, size_t node, int create);
int graph_add_edge(struct graph *g, size_t node1, size_t node2);
int graph_iterate_edges(struct graph *g, size_t node, ssize_t *state);
ssize_t graph_n_out(struct graph *g, size_t node);

struct graph_traversal
{									
  struct graph *g;
  struct bitset marks;
  void *cls;
};

#define GRAPH_TRAVERSAL(name,pre_func,post_func)			\
									\
  static int								\
  graph_traversal_##name##_dfs_rec(struct graph_traversal *gt,		\
				   size_t node, int max_rec_depth)	\
  {									\
    ERROR_CUSTOM_RET(max_rec_depth < 0,					\
		     GRAPH_ERROR_TOO_MANY_RECURSIVE_CALLS,		\
		     -1);						\
									\
    bitset_set(&gt->marks, node);					\
    pre_func;								\
									\
    ssize_t out = -1;							\
    while ( graph_iterate_edges(gt->g, node, &out) )			\
      {									\
	if ( ! bitset_isset(&gt->marks, out) )				\
	  {								\
	    int ret = graph_traversal_##name##_dfs_rec(gt,		\
						       out,		\
						       max_rec_depth - 1); \
	    ERROR_RET(ret < 0, -1);					\
	  }								\
      }									\
    									\
    post_func;								\
    									\
    return 0;								\
  }									\
  									\
  static int								\
  graph_traversal_##name##_dfs(struct graph *g, size_t root, void *cls) \
  {									\
    struct graph_traversal gt;						\
    int ret;								\
									\
    gt.g = g;								\
    gt.cls = cls;							\
    bitset_init(&gt.marks, g->nodes.n);					\
									\
    ret = graph_traversal_##name##_dfs_rec(&gt, root,			\
					   GRAPH_MAX_RECURSION_DEPTH);	\
									\
    bitset_destroy(&gt.marks);						\
									\
    return ret;								\
  }

#endif
