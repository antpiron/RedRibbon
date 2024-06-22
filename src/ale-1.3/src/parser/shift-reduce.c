#include <string.h>
#include "ale/vector.h"
#include "ale/parser.h"
#include "ale/error.h"
#include "ale/stringutils.h"
#include "ale/memory.h"


/*
inline
void
parser_shift_init(struct parser_action *pa, size_t next_state)
{
  pa->type = PARSER_ACTION_SHIFT; 
  pa->shift.next_state = next_state;
}

inline
void
parser_shift_destroy(__attribute__((unused)) struct parser_action *pa)
{

}

void
parser_shift_free(struct parser_action *pa)
{
  parser_shift_destroy(pa);
  free(pa);
}

struct parser_action*
parser_shift_new(size_t next_state)
{
  struct parser_action *pa = malloc(sizeof(struct parser_action));
  
  parser_shift_init(pa,  next_state);

  return pa;
}

struct parser_action*
parser_shift_pool_new(struct mem_pool *pool, size_t next_state)
{
  struct parser_action *pa = mem_malloc(pool, sizeof(struct parser_action));
  
  parser_shift_init(pa,  next_state);

  return pa;
}

inline
void
parser_reduce_init(struct parser_action *pa, size_t lhs, size_t rhs_n,
		   void* (*callback)(size_t n, void* rhs[n], void *cls))
{
  pa->type = PARSER_ACTION_REDUCE;
  pa->reduce.lhs = lhs;
  pa->reduce.rhs_n = rhs_n;
  pa->reduce.callback = callback;
}

*/

struct stack_elem
{
  size_t state;
  void *value;
};

VECTOR_INIT_FULL(stack, struct stack_elem, (1u << 16));

struct stack
{
  size_t n;
  struct vector_stack data;
};

static void
stack_init(struct stack *stack)
{
  stack->n = 0;
  vector_stack_init(&stack->data);
}


static void
stack_destroy(struct stack *stack)
{
  vector_stack_destroy(&stack->data);
}

static void
stack_push(struct stack *stack, struct stack_elem elem)
{
  vector_stack_set(&stack->data, stack->n++, elem);
}

static struct stack_elem*
stack_top(struct stack *stack)
{
  if (0 == stack->n)
    return NULL;

  return stack->data.data + stack->n - 1;
}

static struct stack_elem*
stack_topn(struct stack *stack, size_t n)
{
  if (stack->n < n)
    return NULL;

  return stack->data.data - stack->n;
}

static int
stack_popn(struct stack *stack, size_t n)
{
  if (stack->n < n)
    return -1;

  stack->n -= n;
  
  return 0;
}

/* https://lambda.uta.edu/cse5317/notes/node18.html */
/*   push(0); */
/* read_next_token(); */
/* for(;;) */
/* {  s = top();    /\* current state is taken from top of stack *\/ */
/*    if (ACTION[s,current_token] == 'si')   /\* shift and go to state i *\/ */
/*    {  push(i); */
/*       read_next_token(); */
/*    } */
/*    else if (ACTION[s,current_token] == 'ri') */
/*    /\* reduce by rule i: X ::= A1...An *\/ */
/*    {  perform pop() n times; */
/*       s = top();    /\* restore state before reduction from top of stack *\/ */
/*       push(GOTO[s,X]);   /\* state after reduction *\/ */
/*    } */
/*    else if (ACTION[s,current_token] == 'a') */
/*       success!! */
/*    else error(); */
/* } */

void parser_shift_reduce_init(struct parser_shift_reduce *sr,
			      ssize_t (*goto_table)(size_t state, size_t lhs, void *cls),
			      struct parser_action* (*action_table)(size_t state, size_t token, void *cls),
			      struct lexer_token* (*next_token)(size_t state, void *cls))
{
  mem_init(&sr->pool);
  sr->goto_table = goto_table;
  sr->action_table = action_table;
  sr->next_token = next_token;
}

void
parser_shift_reduce_destroy(struct parser_shift_reduce *sr)
{
  mem_destroy(&sr->pool);
}

VECTOR_INIT_FULL(voidptr, void*, (1u << 5));


int
parser_shift_reduce(struct parser_shift_reduce *sr, void *value, void *cls)
{
  struct stack stack;
  struct lexer_token *token;
  int ret = -1;
  struct vector_voidptr voidptr;

  value = NULL;
  stack_init(&stack);
  vector_voidptr_init(&voidptr);

  stack_push(&stack, (struct stack_elem) {.state = 0} );
  token = sr->next_token(0, cls); 

  while (1)
    {
      struct stack_elem *top = stack_top(&stack);
      
      ERROR_CUSTOM_GOTO(NULL == top, PARSER_ERROR_EMPTY_STACK, ERROR_OUT_OF_LOOP);

      struct parser_action *action = sr->action_table(top->state, token->id, cls);
      switch (action->type)
	{
	case PARSER_ACTION_SHIFT:
	  size_t alloc_size = sizeof(char) * (strlen(token->str) + 1);
	  char *str = mem_malloc(&sr->pool, alloc_size);
	  memcpy(str, token->str, alloc_size);

	  stack_push(&stack, (struct stack_elem) { .state = action->shift.next_state,
						   .value = str});
	  token = sr->next_token(top->state, cls);

	  // printf("%zu: S%zu <<< %s\n", top->state, action->shift.next_state, str);
	  break;
	  
	case PARSER_ACTION_REDUCE:
	  if (NULL != action->reduce.callback)
	    {
	      struct stack_elem *elems = stack_topn(&stack, action->reduce.rhs_n);

	      for (size_t i = 0 ; i <  action->reduce.rhs_n ; i++)
		vector_voidptr_set(&voidptr, i, elems[i].value);
	      
	      value = action->reduce.callback(action->reduce.rhs_n, voidptr.data, cls);
	    }
	  
	  int err = stack_popn(&stack, action->reduce.rhs_n);
	  ERROR_CUSTOM_GOTO(err < 0, PARSER_ERROR_STACK_TOO_SMALL, ERROR_OUT_OF_LOOP);

	  // printf("%zu: R%zu goto ", top->state, action->reduce.lhs);
	  
	  top = stack_top(&stack);
	  ERROR_CUSTOM_GOTO(NULL == top, PARSER_ERROR_EMPTY_STACK, ERROR_OUT_OF_LOOP);

	  ssize_t goto_dst = sr->goto_table(top->state, action->reduce.lhs, cls);
	  ERROR_CUSTOM_GOTO(goto_dst < 0, PARSER_ERROR_REDUCE, ERROR_OUT_OF_LOOP);
	  stack_push(&stack,
		     (struct stack_elem) { .state = goto_dst,
					   .value = value } );
	  // printf("%zu\n", goto_dst);
	  break;
	  
	case PARSER_ACTION_ACCEPT:
	  ret = 0;
	  value = top->value;
	  goto OUT_OF_LOOP;
	  
	  break;

	default:
	  // ret = -1;
	  ERROR_CUSTOM_GOTO(1, PARSER_ERROR, ERROR_OUT_OF_LOOP);
	}
    }

 ERROR_OUT_OF_LOOP:
  ret = -1;
  
 OUT_OF_LOOP:
  stack_destroy(&stack);
  vector_voidptr_destroy(&voidptr);

  return ret;
}
