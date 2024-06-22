#include "ale/error.h"
#include "ale/parser.h"

/*
  A -> 'a' B C
  A ->
  B ->
  B -> 'b'
  C -> 'c'
  first(A) = { 'c', 'b' }
    
  first({}) = {EPS}
  first(terminal) = {terminal}
  first(NT) = {first(lhs) |  for all NT1 -> lhs}
  first(NT A B ...) = first(NT)                                  if EPS not in first(NT)
                    = (first{NT} - EPS) union first(A B ...)     if EPS in first(NT)
*/
int
parser_first_init(struct parser_first *pf, struct parser_grammar *g)
{
  struct bitset *first = malloc(sizeof(struct bitset) * g->n_nonterminals);
  int change;

  pf->g = g;
  pf->first = first;

  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    bitset_init(first + i, g->n_terminals);

  do
    {
      change = 0;
      for (size_t r = 0 ; r < g->n_rules ; r++)
	{
	  struct grammar_rule *rule = g->rules.data + r;
	  struct bitset *rule_first = first + rule->lhs;
	  
	  if (0 == rule->n_rhs)
	    {
	      /* empty rule, add epsilon */
	      if ( ! bitset_isset(rule_first, 0) )
		{
		  bitset_set(rule_first, 0);
		  change = 1;
		}
	    }
	  else
	    {
	      size_t dot = 0;
	      // do not count if change is already set
	      size_t ones = change ? 0 : bitset_ones(rule_first);

	      for ( dot = 0 ; dot < rule->n_rhs ; dot++)
		{
		  struct grammar_rule_node *node = rule->rhs.data + dot;
		  
		  if (GRAMMAR_TERMINAL == node->type)
		    {
		      if (! bitset_isset(rule_first, node->index) )
			{
			  bitset_set(rule_first, node->index);
			  change = 1;
			}
		      break;
		    }
		  
		  if (GRAMMAR_NON_TERMINAL == node->type)
		    {
		      struct bitset *node_first = first + node->index;
		      int isset = bitset_isset(rule_first, 0);

		      bitset_or(rule_first, rule_first, node_first);
		      
		      if (! bitset_isset(node_first, 0))
			break;

		      // Remove EPS if not previously set and node_first contains EPS
		      if (! isset )
			bitset_unset(rule_first, 0);
		    }
		}
	      
	      if (dot == rule->n_rhs && ! bitset_isset(rule_first, 0) )
		{
		  bitset_set(rule_first, 0);
		  change = 1;
		}

	      if (! change && (bitset_ones(rule_first) - ones) != 0)
		change = 1;
	    }
	  
	}
    }
  while (change);

  return 0;
}

void
parser_first_destroy(struct parser_first *pf)
{
  for (size_t i = 0 ; i < pf->g->n_nonterminals ; i++)
    bitset_destroy(pf->first + i);
  
  free(pf->first);
}

void
parser_first_print(struct parser_first *pf)
{
  struct parser_grammar *g = pf->g;
  
  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    {
      struct bitset *first = pf->first + i;
      
      printf("%3zu. first(%s) = ", i, index_rget(&g->nonterminals, i));

      parser_terminals_print(g, first);
      printf("\n");
    }
}

void
parser_first(struct parser_first *pf, struct bitset *first,
	     size_t n, struct grammar_rule_node *nodes, struct bitset *follow)
{
  bitset_reset(first);
  bitset_set(first, 0);
 
  for (size_t i = 0 ; i < n ; i++)
    {
      struct grammar_rule_node *node = nodes + i;

      bitset_unset(first, 0);
      if (GRAMMAR_TERMINAL == node->type)
	{
	  bitset_set(first, node->index);
	  break;
	}
      
      if (GRAMMAR_NON_TERMINAL == node->type)
	{
	  bitset_or(first, first, pf->first + node->index);
	  if (! bitset_isset(first, 0))
	    break;
	}
    }

  if ( bitset_isset(first, 0) && NULL != follow)
    {
      bitset_unset(first, 0);
      bitset_or(first, first, follow);
    }
}
