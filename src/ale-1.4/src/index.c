#include "ale/index.h"


void
index_init(struct index *index)
{
  mem_init(&index->pool);
  vector_chars_init(&index->itos);
  hash_chars_size_t_init(&index->stoi);
  index->max = -1;
}

void
index_destroy(struct index *index)
{
  mem_destroy(&index->pool);
  vector_chars_destroy(&index->itos);
  hash_chars_size_t_destroy(&index->stoi);
}

int
index_set(struct index *index, const char *key, size_t i)
{
  size_t len = strlen(key);
  char *str = mem_malloc(&index->pool, len + 1);
  strcpy(str, key); 

  vector_chars_set(&index->itos, i, str);
  int ret = hash_chars_size_t_set(&index->stoi, str, i, NULL);

  if ((ssize_t) i > index->max)
    index->max = i;
  
  return ret;
}

ssize_t
index_get(const struct index *index, const char *key)
{
  size_t i;
  int ret = hash_chars_size_t_get(&index->stoi, (char*) key, &i);
  ERROR_UNDEF_RET(0 == ret, -1);
  
  return i;
}

char*
index_rget(const struct index *index, size_t i)
{
  if ((ssize_t) i > index->max)
    return NULL;
  return vector_chars_get(&index->itos, i);
}
