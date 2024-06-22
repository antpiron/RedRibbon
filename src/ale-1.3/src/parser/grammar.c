#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>

#include "ale/error.h"
#include "ale/parser.h"
#include "ale/stringutils.h"


void
grammar_init(struct parser_grammar *g)
{
  size_t S;
  mem_init(&g->pool);
  
  g->n_terminals = g->n_nonterminals = g->n_rules = 0;
  g->start_nt = -1;

  vector_grammar_rule_init(&g->rules);


  index_init(&g->terminals);
  index_init(&g->nonterminals);
  vector_nt_init(&g->nonterminals_bitsets);

  /* Add epsilon at index 0 */
  grammar_add_terminal(g, "");


  /* Add augmented rule at index 0 */
  S = grammar_add_nonterminal(g, "");
  grammar_add_rule_va(g,
		      S,
		      NULL); 

}

void
grammar_set_start(struct parser_grammar *g, size_t start_nt)
{
  g->start_nt = start_nt;

  vector_grammar_rule_node_set(&g->rules.data[0].rhs,
			       0,
			       (struct grammar_rule_node) {
				 .type = GRAMMAR_NON_TERMINAL,
				 .index = start_nt});
  g->rules.data[0].n_rhs = 1;
}

void
grammar_destroy(struct parser_grammar *g)
{
  index_destroy(&g->terminals);
  index_destroy(&g->nonterminals);

  for (size_t i = 0 ; i < g->n_rules ; i++)
    {
      vector_grammar_rule_node_destroy(&g->rules.data[i].rhs);
    }
  
  vector_grammar_rule_destroy(&g->rules);

  
  for (size_t i = 0 ; i < g->n_nonterminals ; i++)
    {
      bitset_destroy(g->nonterminals_bitsets.data + i);
    }
  vector_nt_destroy(&g->nonterminals_bitsets);
  
  mem_destroy(&g->pool);
}

void
grammar_print(struct parser_grammar *g)
{  
  for (size_t i = 0 ; i < g->n_rules ; i++)
    {
      struct grammar_rule *rule = g->rules.data + i;

      if ( 0 == i )
	printf("     %-12s ::=", "[Start]");
      else
	printf("%3zu. %-12s ::=", i, index_rget(&g->nonterminals, rule->lhs));
      
      for (size_t j = 0 ; j < rule->n_rhs ; j++)
	{
	  struct grammar_rule_node *node = rule->rhs.data + j;
	  if (GRAMMAR_NON_TERMINAL == node->type)
	    printf(" %s", index_rget(&g->nonterminals, node->index));
	  else if (GRAMMAR_TERMINAL == node->type)
	    printf(" '%s'", index_rget(&g->terminals, node->index));
	}
      
      printf("\n");
    }
}

void
parser_terminals_print_full(struct parser_grammar *g, struct bitset *bs, int isEOF)
{
  printf("{");

  for (ssize_t j = 0, terminal = -1 ;  bitset_iterate(bs, &terminal) ; j++ )
       {
	 if (isEOF && 0 == terminal)
	   printf((0 == j)?"$":", $");
	 else
	   printf((0 == j)?"\"%s\"":", \"%s\"", index_rget(&g->terminals, terminal));
       }
      
  printf("}");
}

void
parser_terminals_print(struct parser_grammar *g, struct bitset *bs)
{
  parser_terminals_print_full(g, bs, 0);
}
  
inline
static
ssize_t
index_lookup_or_add(struct index *index, const char *str, size_t *n)
{
  ssize_t pos = index_get(index, str);

  if (pos < 0)
    { 
      int ret = index_set(index, str, *n);
      ERROR_RET(ret < 0, -1);

      pos = (*n)++;
    }

  return pos;
}

ssize_t
grammar_add_terminal(struct parser_grammar *g, const char *str)
{
  return index_lookup_or_add(&g->terminals, str, &g->n_terminals);
}

ssize_t
grammar_add_nonterminal(struct parser_grammar *g, const char *name)
{
  size_t old_n = g->n_nonterminals;
  ssize_t ret = index_lookup_or_add(&g->nonterminals, name, &g->n_nonterminals);

  /* New non terminal, initialize the bitset to rules for fast mapping */
  if (g->n_nonterminals != old_n)
    {
      struct bitset *bs = vector_nt_get_ptr(&g->nonterminals_bitsets, ret);
      bitset_init(bs, g->n_rules);
    }
  
  return ret;
}


ssize_t
grammar_add_rule_va(struct parser_grammar *g, size_t lhs, ...)
{
  va_list ap;
  struct grammar_rule rule;
  
  rule.lhs = lhs;
  rule.n_rhs = 0;

  vector_grammar_rule_node_init(&rule.rhs);
  
  va_start(ap, lhs);
  
  for (struct grammar_rule_node *node = va_arg(ap, struct grammar_rule_node *) ;
       NULL != node ;
       node = va_arg(ap, struct grammar_rule_node *) )
    {
      vector_grammar_rule_node_set(&rule.rhs, rule.n_rhs, *node);
      rule.n_rhs++;
    }
  
  va_end(ap);

  vector_grammar_rule_set(&g->rules, g->n_rules, rule);
  
  bitset_set(vector_nt_get_ptr(&g->nonterminals_bitsets, lhs),
	     g->n_rules);


  return g->n_rules++;
}

