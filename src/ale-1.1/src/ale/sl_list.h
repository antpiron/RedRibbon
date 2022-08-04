#ifndef __SL_LIST_H
#define __SL_LIST_H

struct sl_node {
  void* data;
  struct sl_node *next;
};


struct sl_node* sl_mknode(void *data, struct sl_node *next);

int sl_init(struct sl_node *node);
int sl_destroy(struct sl_node *node);
int sl_destroy_full(struct sl_node *node, void (*destroy_data)(void *));

int sl_insert(struct sl_node *node, size_t pos, void *data);
void* sl_remove(struct sl_node *node, size_t pos);

struct sl_node* sl_first(struct sl_node *node);
struct sl_node* sl_next(struct sl_node *node);
struct sl_node* sl_nth(struct sl_node *node, size_t pos);

struct sl_node* sl_search(struct sl_node *node, void *data, int (*equal)(void *a, void *b));
struct sl_node* sl_search_full(struct sl_node *node, void *data,
			       int (*equal)(void *a, void *b, void *cls),
			       void *cls);

int sl_push(struct sl_node *node, void *data);
int sl_pop(struct sl_node *node, void **data);
int sl_top(struct sl_node *node, void **data);


#endif
