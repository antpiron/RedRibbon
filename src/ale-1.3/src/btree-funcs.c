#include <stdlib.h>
#include <inttypes.h>

#include "ale/error.h"
#include "ale/btree.h"


int 
bt_intptr_t_cmp(void *a, void *b)
{
  return (intptr_t) b -  (intptr_t) a;
}


void bt_intptr_t_free(void *ptr) { (void) (ptr); }

void* 
bt_intptr_t_dup(void *a)
{
  return a;
}

int
bt_intptr_t_keytostr(char *str, size_t size, void *key)
{
  return snprintf(str, size, "%"PRIdPTR, (intptr_t) key);
}

struct btfuncs btfuncs_intptr_t = { .cmpkey = bt_intptr_t_cmp, 
				    .dupkey = bt_intptr_t_dup, 
				    .freekey = bt_intptr_t_free,
				    .keytostr = bt_intptr_t_keytostr };
