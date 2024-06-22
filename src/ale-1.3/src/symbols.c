#include "ale/symbols.h"


void
symbols_init_full(struct ale_symbols *s, void (*free_symbol_data)(void *))
{
  mem_init(&s->pool);
  
  sl_init(&s->tables);
  mem_callback(&s->pool, &s->tables, (void (*)(void *)) sl_destroy);
  
  s->top = NULL;
  s->free_symbol_data = free_symbol_data;
}

void
symbols_init(struct ale_symbols *s)
{
  symbols_init_full(s, NULL);
}


void
symbols_destroy(struct ale_symbols *s)
{  
  mem_destroy(&s->pool);
}

struct ale_symbol_table*
symbols_new_table(struct ale_symbols *s, const char *name, struct ale_symbol_table *parent)
{
  struct ale_symbol_table *st = mem_malloc(&s->pool, sizeof(struct ale_symbol_table));

  if ( NULL != name )
    {
      size_t len = strlen(name);
      st->name = mem_malloc(&s->pool, len + 1);
      strcpy(st->name, name);
    }
  else
    st->name = NULL;
  
  st->parent = parent;
  st->n = 0;
  
  index_init(&st->index);
  mem_callback(&s->pool, &st->index, (void (*)(void *)) index_destroy);
  
  vector_ale_symbol_init(&st->symbols);
  mem_callback(&s->pool, &st->symbols, (void (*)(void *)) vector_ale_symbol_destroy);
  
  return st;
}

struct ale_symbol_table*
symbols_push_table(struct ale_symbols *s, const char *name)
{
  struct ale_symbol_table *st = symbols_new_table(s, name, s->top);

  sl_push(&s->tables, st);
  s->top = st;
  
  return st;
}

struct ale_symbol*
symbols_add(struct ale_symbols *s, const char *name, int type, void *data)
{
  struct ale_symbol symbol;

  symbol.type = type;

  size_t len = strlen(name);
  symbol.name = mem_malloc(&s->pool, len + 1);
  strcpy(symbol.name, name);
  
  symbol.data = data;
  if (NULL != s->free_symbol_data)
    mem_callback(&s->pool, data, s->free_symbol_data);

  vector_ale_symbol_set(&s->top->symbols, s->top->n, symbol);

  index_set(&s->top->index, name, s->top->n);
    
  return &s->top->symbols.data[s->top->n++];
}

struct ale_symbol*
symbols_get(struct ale_symbols *s, const char *name)
{
  struct ale_symbol *symbol = NULL;

  for (struct ale_symbol_table *top = s->top ; NULL != top ; top = top->parent)
    {
      ssize_t index = index_get(&top->index, name);

      if (index >= 0)
	{
	  symbol = top->symbols.data + index;
	  break;
	}
    }
  
  return symbol;
}
