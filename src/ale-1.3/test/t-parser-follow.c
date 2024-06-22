#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "ale/error.h"
#include "ale/parser.h"

int
is_in_follow(struct parser_follow *pf, size_t nt, const char *str)
{
  size_t terminal = -1;
  size_t count = 0;
  ssize_t elem = index_get(&pf->pfirst->g->terminals, str);
  
  while ( bitset_iterate(pf->follow + nt, &terminal) )
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
  struct parser_first pfirst;
  struct parser_follow pfollow;
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

  grammar_set_start(&g, S);
  
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

  parser_first_init(&pfirst, &g);
  parser_follow_init(&pfollow, &pfirst);

  parser_follow_print(&pfollow);

  ERROR_UNDEF_FATAL(! is_in_follow(&pfollow, S, ""), "parser_follow_init(): $ is not in follow(S)\n");
  ERROR_UNDEF_FATAL(is_in_follow(&pfollow, E, ""), "parser_follow_init(): $ is in follow(E)\n");
  ERROR_UNDEF_FATAL(! is_in_follow(&pfollow, E, "d"), "parser_follow_init(): $ is not in follow(E)\n");
  ERROR_UNDEF_FATAL(! is_in_follow(&pfollow, D, ""), "parser_follow_init(): $ is not in follow(D)\n");
   
    
  parser_follow_destroy(&pfollow);
  parser_first_destroy(&pfirst);
  
  grammar_destroy(&g);

  
  return EXIT_SUCCESS;
}
