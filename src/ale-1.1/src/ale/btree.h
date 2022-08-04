#ifndef __BTREE_H
#define __BTREE_H

#include <stdint.h>

typedef int (*bt_cmp_func_t)(void*, void*);
typedef void (*bt_freekey_func_t)(void*);
typedef void* (*bt_dupkey_func_t)(void*);
typedef int (*bt_keytostr_func_t)(char *, size_t, void*);

enum {
  BT_ERRNO_INVALID_ORDER = 1
};

struct btnode {
  int type;      // internal, leaf
  int order;     // nbr of data's or nodes
  void **key;  
  union {
    void **data;
    struct btnode **nodes;
  } childs;
};

struct btfuncs {
  bt_cmp_func_t cmpkey;
  bt_dupkey_func_t dupkey;
  bt_freekey_func_t freekey;
  bt_keytostr_func_t keytostr;
};

extern struct btfuncs btfuncs_intptr_t;

struct btree 
{
  int order;
  struct btnode *root;
  struct btfuncs f;
};


int bt_init(struct btree *bt, int order, struct btfuncs funcs);
void bt_destroy(struct btree *bt);


void* bt_search(struct btree *bt, void *key);
int bt_insert(struct btree *bt, void *key, void *data);
void* bt_delete(struct btree *bt, void *key);

void bt_print(struct btree *bt); 

// ---------------------------------------------------------------------------------------------
// Internal use only
// ---------------------------------------------------------------------------------------------

#define BT_NODE_LEAF (1)
#define BT_NODE_INTERNAL (0)
#define BT_ISLEAF(node) ((node).type & BT_NODE_LEAF)
#define BT_ISINTERNAL(node) (! BT_ISLEAF(node))

#define BT_ISFULL(bt,node) ( BT_ISLEAF(node)?(node).order == (bt).order -1:(node).order == (bt).order )
#define BT_ISMINIMAL(bt,node) ( ( BT_ISLEAF(node)?(node).order == ((bt).order -1) / 2:(node).order == (bt).order / 2) )

#define BT_OPT_SPLIT 1
#define BT_OPT_MERGE (1 << 1)

struct btnode* bt_mknode(int btorder, int type);
void bt_freenode(struct btnode *node);

void bt_slice(struct btnode *dest, struct btnode *src,  int from, int count);
struct btnode* bt_split_right_leaf(struct btree *bt, struct btnode *node, int from);
struct btnode* bt_split_right_internal(struct btree *bt, struct btnode *node, int from);

void bt_internal_node_insert(struct btnode *node, int index, void *key, struct btnode *right_node);

int bt_split_leaf_child(struct btree *bt, struct btnode *node, int index);
int bt_split_internal_child(struct btree *bt, struct btnode *node, int index);
int bt_split_child(struct btree *bt, struct btnode *node, int index);
int bt_split_root(struct btree *bt);

struct btnode* bt_search_leaf(struct btree *bt, void *key, int *index, int *found, int options);



#endif
