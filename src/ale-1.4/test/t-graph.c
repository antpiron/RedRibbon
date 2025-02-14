#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include "ale/error.h"
#include "ale/graph.h"

#define N (1ull << 8)

// #define POST_FUNC				\
  

GRAPH_TRAVERSAL(test,++*(size_t*) gt->cls,printf("%zu ", node))

int
main(int argc, char *argv[argc])
{
  struct graph g;
  ssize_t n_out;
  ssize_t root;
  ssize_t *nodes = malloc(sizeof(ssize_t) * N) ;

  graph_init(&g);
  ERROR_UNDEF_FATAL_FMT(0 != g.nodes.n, "FAIL: graph_init(), number of nodes == %zu != 0\n",  g.nodes.n);

  root = graph_add_node(&g);
  ERROR_UNDEF_FATAL_FMT(1 != g.nodes.n, "FAIL: graph_add_node(), n_nodes == %zu != 1\n",  g.nodes.n);
  ERROR_UNDEF_FATAL_FMT(0 != root, "FAIL: graph_add_node(), root == %zd != 0\n",  root);

  n_out = graph_n_out(&g, root);
  ERROR_UNDEF_FATAL_FMT(0 != n_out, "FAIL:  graph_n_out(root), n_out == %zd != 0\n",  n_out);

  for (size_t i = 0 ; i < N ; i++)
    {
      nodes[i] = graph_add_node(&g);
      graph_add_edge(&g, root, nodes[i]);
    }
  ERROR_UNDEF_FATAL_FMT(N + 1 != g.nodes.n, "FAIL: graph_add_node(), n_nodes == %zu != %d\n",  g.nodes.n, N + 1);

  n_out = graph_n_out(&g, root);
  ERROR_UNDEF_FATAL_FMT(N != n_out, "FAIL:  graph_n_out(root), n_out == %zd != %d\n",  n_out, N);

  ssize_t out = -1;
  ssize_t out_i = 1;
  for ( ; graph_iterate_edges(&g, root, &out) ;  out_i++)
    {
      ERROR_UNDEF_FATAL_FMT(out != out_i, "FAIL:  graph_iterate_edges(root), out == %zd != %zd\n",  out, out_i);
    }

  ERROR_UNDEF_FATAL_FMT(N + 1 != out_i, "FAIL:  graph_iterate_edges(root), out == %zd != %d\n",  out, N + 1);

  size_t n_nodes = 0;
  size_t count = 0;
  int ret = graph_traversal_test_dfs(&g, root, &count);
  ERROR_FATAL_FMT(ret < 0, "FAIL: graph_traversal_test_dfs(root) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(N + 1 != count, "FAIL: graph_traversal_test_dfs(root) count == %zu != %zu\n", count, N + 1);
    
  graph_destroy(&g);

  free(nodes);
  
  return EXIT_SUCCESS;
}
