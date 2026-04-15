#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/parser.h"

int
is_in_first(struct parser_first *pf, size_t nt, const char *str)
{
  size_t terminal = -1;
  size_t count = 0;
  ssize_t elem = index_get(&pf->g->terminals, str);
  
  while ( bitset_iterate(pf->first + nt, &terminal) )
    {
      if (elem == terminal )
	return 1;
    }

  return 0;
}

int
main(int argc, char *argv[argc])
{
  struct parser_grammar g;
  struct parser_first pf;
  ssize_t S, E, A, B, C, D;


  /*
    S -> E D
    E -> A B C
    A -> "a"
    A ->
    B -> "b"
    B -> 
    C -> "c"
    C ->
    D -> "d"
   */
  grammar_init(&g);
  S = grammar_add_nonterminal(&g, "S");
  E = grammar_add_nonterminal(&g, "E");
  A = grammar_add_nonterminal(&g, "A");
  B = grammar_add_nonterminal(&g, "B");
  C = grammar_add_nonterminal(&g, "C");
  D = grammar_add_nonterminal(&g, "D");

  grammar_add_rule_va(&g,
		      S,
		      G_NT(&g, "E"),
		      G_NT(&g, "D"),
		      NULL);

  
  grammar_add_rule_va(&g,
		      E,
		      G_NT(&g, "A"),
		      G_NT(&g, "B"),
		      G_NT(&g, "C"),
		      NULL);

  grammar_add_rule_va(&g,
		      A,
		      G_T(&g, "a"),
		      NULL);
  grammar_add_rule_va(&g,
		      A,
		      NULL);

  grammar_add_rule_va(&g,
		      B,
		      G_T(&g, "b"),
		      NULL);
  grammar_add_rule_va(&g,
		      B,	
		      NULL);

  grammar_add_rule_va(&g,
		      C,
		      G_T(&g, "c"),
		      NULL);
  grammar_add_rule_va(&g,
		      C,
		      NULL);

  grammar_add_rule_va(&g,
		      D,
		      G_T(&g, "d"),
		      NULL);

  grammar_print(&g);
  printf("\n");

  parser_first_init(&pf, &g);

  parser_first_print(&pf);

 
  ERROR_UNDEF_FATAL(is_in_first(&pf, S, ""), "parser_first_init(): EPS is in first(S)\n");
  size_t count = bitset_ones(pf.first + S);
  ERROR_UNDEF_FATAL_FMT(4 != count, "parser_first_init(): |first(S)| == %zu != 4\n", count);

  ERROR_UNDEF_FATAL(! is_in_first(&pf, E, ""), "parser_first_init(): EPS is not in first(E)\n");
  count = bitset_ones(pf.first + E);
  ERROR_UNDEF_FATAL_FMT(4 != count, "parser_first_init(): |first(E)| == %zu != 4\n", count);

  ERROR_UNDEF_FATAL(is_in_first(&pf, D, ""), "parser_first_init(): EPS is in first(D)\n");
  count = bitset_ones(pf.first + D);
  ERROR_UNDEF_FATAL_FMT(1 != count, "parser_first_init(): |first(E)| == %zu != 1\n", count);


  struct bitset first;

  bitset_init(&first, g.n_terminals);
  parser_first(&pf, &first, g.rules.data[1].n_rhs, g.rules.data[1].rhs.data, NULL);
  count = bitset_ones(&first);
  printf("first(E D) = ");
  parser_terminals_print(&g, &first);
  printf("\n");
  ERROR_UNDEF_FATAL_FMT(4 != count, "parser_first(): |first(A B C D)| == %zu != 4\n", count);

  parser_first(&pf, &first, g.rules.data[1].n_rhs - 1, g.rules.data[1].rhs.data + 1, NULL);
  count = bitset_ones(&first);
  printf("first(D) = ");
  parser_terminals_print(&g, &first);
  printf("\n");
  ERROR_UNDEF_FATAL_FMT(1 != count, "parser_first(): |first(B C D)| == %zu != 1\n", count);

  parser_first(&pf, &first, g.rules.data[1].n_rhs - 2, g.rules.data[1].rhs.data + 2, NULL);
  count = bitset_ones(&first);
  printf("first() = ");
  parser_terminals_print(&g, &first);
  printf("\n");
  ERROR_UNDEF_FATAL_FMT(1 != count, "parser_first(): |first(B C D)| == %zu != 1\n", count);

  bitset_destroy(&first);
    
  parser_first_destroy(&pf);
  
  grammar_destroy(&g);

  
  return EXIT_SUCCESS;
}
