#include "ale/error.h"
#include "ale/graph.h"


void
graph_init(struct graph *g)
{
  bitset_init(&g->nodes, 0);
  stack_edges_init(&g->edges);
}

void
graph_destroy(struct graph *g)
{
  for (ssize_t i = -1 ; bitset_iterate(&g->nodes, &i) ;)
    {
      bitset_destroy(stack_edges_get_ptr(&g->edges, i));
    }
  
  stack_edges_destroy(&g->edges);
  bitset_destroy(&g->nodes);
}

ssize_t
graph_get_node(struct graph *g, size_t node, int create)
{
  if ( ! bitset_isset(&g->nodes, node) )
    {
      ERROR_CUSTOM_RET(! create, GRAPH_ERROR_OUT_OF_RANGE, -1);
      
      struct bitset *bs = stack_edges_get_ptr(&g->edges, node);
      
      ERROR_RET(bitset_init(bs, 0) < 0, -1);

      bitset_set(&g->nodes, node);
    }
  
  return node;
}

ssize_t
graph_add_node(struct graph *g)
{
  return graph_get_node(g, g->nodes.n, 1);
}

int
graph_add_edge(struct graph *g, size_t node1, size_t node2)
{
  ERROR_CUSTOM_RET( ! bitset_isset(&g->nodes, node1) || ! bitset_isset(&g->nodes, node2), GRAPH_ERROR_OUT_OF_RANGE, -1);

  struct bitset *bs = stack_edges_get_ptr(&g->edges, node1);
  
  bitset_set(bs, node2);
  
  return 0;
}

int
graph_iterate_edges(struct graph *g, size_t node, ssize_t *state)
{
  if (! bitset_isset(&g->nodes, node))
    return 0;
  
  struct bitset *bs = stack_edges_get_ptr(&g->edges, node);
  
  return bitset_iterate(bs, state);
}

ssize_t
graph_n_out(struct graph *g, size_t node)
{
  ERROR_CUSTOM_RET(! bitset_isset(&g->nodes, node), GRAPH_ERROR_OUT_OF_RANGE, -1);

  struct bitset *bs = stack_edges_get_ptr(&g->edges, node);
  
  return bitset_ones(bs);
}
