#ifndef __PARSER_H
#define __PARSER_H

#include <stdlib.h>
#include <stddef.h>
#include <ale/memory.h>
#include <ale/stringutils.h>
#include <ale/index.h>
#include <ale/vector.h>
#include <ale/stack.h>
#include <ale/symbols.h>
#include <ale/bitset.h>
#include <ale/graph.h>

/* ============ */
/* Grammar */

enum { GRAMMAR_TERMINAL, GRAMMAR_NON_TERMINAL };

#define GRAMMAR_EPS_STR ""
#define GRAMMAR_EPS 0
/* EOF is used in follow and is the same code as EPS, since EPS is never in follow */ 
#define GRAMMAR_EOF_STR ""
#define GRAMMAR_EOF 0



struct grammar_rule_node
{
  int type;
  size_t index;
};

VECTOR_INIT(grammar_rule_node, struct grammar_rule_node)

struct grammar_rule
{
  size_t lhs;
  size_t n_rhs;
  struct vector_grammar_rule_node rhs;
};

VECTOR_INIT(grammar_rule, struct grammar_rule)
// HASH_INIT(name, keytype, valuetype, equal_function, hash_function)
VECTOR_INIT(nt, struct bitset)
  
struct parser_grammar
{
  struct mem_pool pool;
  size_t n_terminals, n_nonterminals, n_rules;
  ssize_t start_nt;
  struct index terminals, nonterminals;
  struct vector_grammar_rule rules;
  struct vector_nt nonterminals_bitsets; 
};

void grammar_init(struct parser_grammar *g);
void grammar_destroy(struct parser_grammar *g);

/* struct parser_grammar_node* grammar_node_regex(char *regex); */
/* struct parser_grammar_node* grammar_node_string(char *str); */
/* struct parser_grammar_node* grammar_node_nonterminal(size_t id); */

ssize_t grammar_add_nonterminal(struct parser_grammar *g, const char *name);
ssize_t grammar_add_terminal(struct parser_grammar *g, const char *str);

#define G_NT(G, NAME) &(struct grammar_rule_node) { .type = GRAMMAR_NON_TERMINAL, .index = grammar_add_nonterminal(G, NAME) }
#define G_T(G, STR) &(struct grammar_rule_node) { .type = GRAMMAR_TERMINAL, .index = grammar_add_terminal(G, STR) }

ssize_t grammar_add_rule_va(struct parser_grammar *g, size_t lhs, ...);
void grammar_set_start(struct parser_grammar *g, size_t start_nt);


void grammar_print(struct parser_grammar *g);

void parser_terminals_print_full(struct parser_grammar *g, struct bitset *bs, int isfollow);
void parser_terminals_print(struct parser_grammar *g, struct bitset *bs);


/* ===========
   First/follow
 */

struct parser_first
{
  struct parser_grammar *g;
  struct bitset *first;
};

int parser_first_init(struct parser_first *pf, struct parser_grammar *g);
void parser_first_destroy(struct parser_first *pf);
void parser_first_print(struct parser_first *pf);

void parser_first(struct parser_first *pf, struct bitset *first, size_t n, struct grammar_rule_node *nodes, struct bitset *follow);

struct parser_follow
{
  struct parser_first *pfirst;
  struct bitset *follow;
};

int parser_follow_init(struct parser_follow *pfollow, struct parser_first *pfirst);
void parser_follow_destroy(struct parser_follow *pfollow);
void parser_follow_print(struct parser_follow *pfollow);

struct bitset * parser_follow(struct parser_follow *pfollow, size_t nt);

/* ============ */
/* Items */

/* item follow types */
enum { FOLLOW_UNINITIALIZED, FOLLOW_LR0, FOLLOW_SLR, FOLLOW_LR1 };

struct parser_item
{
  size_t rule;
  size_t dot;
  struct bitset generatedFrom;
  struct bitset follow;
  unsigned int isCore : 1;
  unsigned int followType : 2;
  unsigned int plusFollow : 1; /* Use priority, associativity, ... rules to solve conflict */
};


STACK_INIT(parser_item, struct parser_item)

struct parser_items
{
  struct graph generated_from;
  struct stack_parser_item items;
};

struct parser_item_set
{
  int isClosed;
  struct parser_items *items;
  struct bitset elems;
};

STACK_INIT(parser_item_set, struct parser_item_set)
struct parser_graph
{
  struct parser_grammar *g;
  struct stack_parser_item items;    /* a pool of items */
  struct stack_parser_item_set sets; /* A pool of item sets */
  int isSlrFollowComputed;
  struct parser_follow slr_follow;   /* Each nonterminal follow */
};

enum {PARSER_ERROR_NOT_FOUND = 1};

void parser_graph_init(struct parser_graph *graph, struct parser_grammar *g);
void parser_graph_destroy(struct parser_graph *graph);

int parser_graph_build(struct parser_graph *graph);
/* TODO: define proper interface */
int parser_graph_srTables(struct parser_graph *graph, void *goto_table, void *action, void *lexer);

ssize_t parser_graph_addSet(struct parser_graph *graph);
int parser_graph_setClosure(struct parser_graph *graph, size_t set);
int parser_graph_iterateTEdges(struct parser_graph *graph, size_t set, ssize_t *state);
int parser_graph_iterateNTEdges(struct parser_graph *graph, size_t set, ssize_t *state);
ssize_t parser_graph_getTNextSet(struct parser_graph *graph, size_t set, size_t edge);
ssize_t parser_graph_getNTNextSet(struct parser_graph *graph, size_t set, size_t edge);
int parser_graph_iterateConflicts(struct parser_graph *graph, size_t set, ssize_t *state);
			 
ssize_t parser_graph_addItem(struct parser_graph *graph, size_t set,
			     size_t rule, size_t dot,  unsigned int isCore, struct bitset *generatedFrom);
ssize_t parser_graph_getItem(struct parser_graph *graph, size_t set,
			     size_t rule, size_t dot);
ssize_t parser_graph_getItemOrAdd(struct parser_graph *graph, size_t set,
				  size_t rule, size_t dot, struct bitset *generatedFrom);
int parser_graph_setItemFollow(struct parser_graph *graph, size_t item,
			       unsigned int followType, unsigned int plusFollow, struct bitset *follow);
struct bitset* parser_graph_getItemFollow_ptr(struct parser_graph *graph, size_t item);




// grammar_item_first(struct parser_grammar *g, struct parser_item);

/* ============ */
/* Lexer */

struct lexer_token
{
  size_t id;
  char *str;
};


/* ============ */
/* Shift-Reduce automata */

enum {
  PARSER_ACTION_ERROR,
  PARSER_ACTION_SHIFT,
  PARSER_ACTION_REDUCE,
  PARSER_ACTION_ACCEPT
};

enum {
  PARSER_TOKEN_EOF,
  PARSER_TOKEN_STRING,
  PARSER_TOKEN_REGEX
};

struct parser_shift_action
{
  size_t next_state;
};

struct parser_reduce_action
{
  size_t lhs;
  size_t rhs_n;
  void* (*callback)(size_t n, void* rhs[n], void *cls);
};

struct parser_action
{
  int type;
  union
  {
    struct parser_shift_action shift;
    struct parser_reduce_action reduce;
  };
};

/*
void parser_shift_init(struct parser_action *pa, size_t next_state);
void parser_shift_destroy(struct parser_action *pa);
void parser_shift_free(struct parser_action *pa);
struct parser_action* parser_shift_new(size_t next_state);
struct parser_action* parser_shift_pool_new(struct mem_pool *pool, size_t next_state);

void* parser_tree_callback(size_t n, void* rhs[n], void *cls);
void parser_reduce_init(struct parser_action *pa, size_t lhs, size_t rhs_n,
			void* (*callback)(size_t n, void* rhs[n], void *cls));
*/

struct parser_shift_reduce
{
  struct mem_pool pool;
  ssize_t (*goto_table)(size_t state, size_t lhs, void *cls);
  struct parser_action* (*action_table)(size_t state, size_t token, void *cls);
  struct lexer_token* (*next_token)(size_t state, void *cls);
};


enum { PARSER_ERROR_EMPTY_STACK = 1, PARSER_ERROR_STACK_TOO_SMALL, PARSER_ERROR_REDUCE, PARSER_ERROR };

void parser_shift_reduce_init(struct parser_shift_reduce *sr,
			      ssize_t (*goto_table)(size_t state, size_t lhs, void *cls),
			      struct parser_action* (*action_table)(size_t state, size_t token, void *cls),
			      struct lexer_token* (*next_token)(size_t state, void *cls));
void parser_shift_reduce_destroy(struct parser_shift_reduce *sr);

int parser_shift_reduce(struct parser_shift_reduce *sr, void *value , void *cls);

#endif
