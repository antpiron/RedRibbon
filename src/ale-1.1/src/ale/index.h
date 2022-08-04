#ifndef __INDEX_H
#define __INDEX_H

#include <ale/stringutils.h>
#include <ale/memory.h>
#include <ale/vector.h>
#include <ale/hash.h>

VECTOR_INIT(chars,char*)
// HASH_INIT(name, keytype, valuetype, equal_function, hash_function)
HASH_INIT(chars_size_t, char*, size_t, equal_func_chars, hash_func_chars)
  
struct index
{
  ssize_t max;
  struct mem_pool pool;
  struct vector_chars itos;
  struct hash_chars_size_t stoi;
};

void index_init(struct index *index);
void index_destroy(struct index *index);

int index_set(struct index *index, const char *key, size_t i);
ssize_t index_get(const struct index *index, const char *key);
char* index_rget(const struct index *index, size_t i);


#endif
