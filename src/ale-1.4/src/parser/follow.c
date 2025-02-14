#include "ale/error.h"
#include "ale/parser.h"
#include "ale/parser.h"

int
parser_follow_init(struct parser_follow *pfollow, struct parser_first *pfirst)
{
  struct parser_grammar *g = pfirst->g;
  struct bitset *follow = malloc(sizeof(struct bitset) * g->n_nonterminals);
  struct bitset first;
  int change;

  pfollow->pfirst = pfirst;
  pfollow->follow = follow;

  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    bitset_init(follow + i, g->n_terminals);

  bitset_init(&first, g->n_terminals);

  /* set start rule follow to $ */
  bitset_set(follow,  GRAMMAR_EOF);
  
  do
    {
      change = 0;
      for (size_t r = 0 ; r < g->n_rules ; r++)
        {
	  size_t dot = 0;
          struct grammar_rule *rule = g->rules.data + r;

	  for ( dot = 0 ; dot < rule->n_rhs ; dot++)
	    {
	      struct grammar_rule_node *node = rule->rhs.data + dot;
	      
	      if (GRAMMAR_NON_TERMINAL == node->type)
		{
		  struct bitset *nt_follow = follow + node->index;
		  size_t ones = change ? 0 : bitset_ones(nt_follow);
		  
		  bitset_reset(&first);
		  
		  parser_first(pfirst, &first, rule->n_rhs - dot - 1, rule->rhs.data + dot + 1, NULL);

		  if ( bitset_isset(&first, GRAMMAR_EPS) )
		    {
		      struct bitset *rule_follow = follow + rule->lhs;
		      
		      bitset_unset(&first, GRAMMAR_EPS);
		      bitset_or(nt_follow, nt_follow, rule_follow);
		    }
		  
		  bitset_or(nt_follow, nt_follow, &first);

		  if (! change && (bitset_ones(nt_follow) - ones) != 0)
		    change = 1;
		}
	    }
	}
    }
  while (change);

  bitset_destroy(&first);
  
  return 0;
}


void
parser_follow_destroy(struct parser_follow *pfollow)
{
  struct parser_grammar *g = pfollow->pfirst->g;
  
  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    bitset_destroy(pfollow->follow + i);
  
  free(pfollow->follow);
}

void
parser_follow_print(struct parser_follow *pfollow)
{
  struct parser_grammar *g = pfollow->pfirst->g;
  
  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    {
      struct bitset *follow = pfollow->follow + i;

      if (0 == i)
	printf("     follow(%s) = ", "[Start]");
      else
	printf("%3zu. follow(%s) = ", i, index_rget(&g->nonterminals, i));

      parser_terminals_print_full(g, follow, 1);
      printf("\n");
    }
}

struct bitset *
parser_follow(struct parser_follow *pfollow, size_t nt)
{
  return pfollow->follow + nt;
}
