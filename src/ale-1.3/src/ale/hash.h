#ifndef __HASH_H
#define __HASH_H

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <ale/siphash24.h>
#include <ale/portability.h>
#include <ale/stringutils.h>

#define HASH_DEFAULT_SIZE ( (1 << 16) + 1 )
#define HASH_MAX_COL ( 1 << 5 )

#define HASH_SCALAR_FUNCTIONS(name,keytype)				\
  static inline size_t							\
  hash_func_##name(keytype buf, const uint8_t *key)			\
  {									\
    uint64_t hashed;							\
									\
    siphash((uint8_t *) &hashed, (uint8_t*) &buf, (uint64_t) sizeof(buf), key); \
									\
    return (size_t) hashed;						\
  }									\
									\
  static inline size_t							\
  hash_func_##name##_fast(keytype buf, const uint8_t *key)		\
  {									\
    /* SIP_KEYLEN == 16 */						\
    uint64_t lo = *(uint64_t*) key, hi = *(uint64_t*) (key + 8);	\
    return (size_t) (buf ^ lo ^ hi);					\
  }									\
									\
  static inline int							\
  equal_func_##name(keytype a, keytype b)				\
  {									\
    return a == b;							\
  }

HASH_SCALAR_FUNCTIONS(int, int)
HASH_SCALAR_FUNCTIONS(size_t, size_t)
HASH_SCALAR_FUNCTIONS(ssize_t, ssize_t)
HASH_SCALAR_FUNCTIONS(long, long)
HASH_SCALAR_FUNCTIONS(longlong, long long)



static inline size_t
hash_func_chars(char *buf, const uint8_t *key)
{
  uint64_t hashed;
  size_t len = strlen(buf);

  siphash((uint8_t *) &hashed, (uint8_t*) buf, (uint64_t) len, key);
  
  return (size_t) hashed;
}

static inline int
equal_func_chars(char *a, char *b)
{
  return 0 == strcmp(a, b);
}

static inline size_t
hash_func_string(struct string *buf, const uint8_t *key)
{
  uint64_t hashed;

  siphash((uint8_t *) &hashed, (uint8_t*) buf->str, (uint64_t) buf->len, key);
  
  return (size_t) hashed;
}

static inline int
equal_func_string(struct string *a, struct string *b)
{
  return 0 == strcmp(a->str, b->str);
}

#define HASH_INCREMENT(index,increment,modulo) do { index = (index + (increment)) % (modulo); } while (0)

#define _HASH_FREE (-1)
#define _HASH_DELETE (-2)
#define _HASH_NOT_FOUND (-3)

// Keyed hash function size_t hash_func(keytype buf, const uint8_t *key)
// int (*equal)(keytype a, keytype b)
#define HASH_INIT(name,keytype,valuetype,equal_func,hash_func)		\
  struct hash_##name##_kv						\
  {									\
    ssize_t index;							\
    keytype key;							\
    valuetype value;							\
  };									\
									\
  struct hash_##name							\
  {									\
    uint8_t keys[2][SIP_KEYLEN];					\
    size_t size;							\
    struct hash_##name##_kv *array;					\
  };									\
									\
  static inline size_t							\
  hash_##name##_hash(const struct hash_##name *hash, keytype buf)	\
  {									\
    return hash_func(buf, hash->keys[0]) % hash->size;			\
  }									\
									\
  static inline size_t							\
  hash_##name##_hash_increment(const struct hash_##name *hash, keytype buf) \
  {									\
    size_t ret = hash_func(buf, hash->keys[1]) %			\
      (hash->size / HASH_MAX_COL);					\
    return ret?ret:1;							\
  }									\
									\
  static inline void							\
  hash_##name##_init_size(struct hash_##name *hash, size_t size)	\
  {									\
    int eintr_count = 0;						\
    while ( -1 == getrandom(hash->keys, sizeof(hash->keys), 0) )	\
      {									\
	if (EAGAIN == errno || EINTR == errno)				\
	  {								\
	    eintr_count++;						\
	    ERROR_ERRNO_FATAL_FMT(3 <= eintr_count,			\
				  "getrandom() in hash_init_full() returned %d times EINTR\n", \
				  eintr_count);				\
	    continue;							\
	  }								\
									\
	ERROR_ERRNO_FATAL(1, "getrandom() in hash_init_full() failed\n"); \
      }									\
									\
    hash->size = size;							\
    hash->array = malloc(sizeof(struct hash_##name##_kv) * size);	\
    ERROR_UNDEF_FATAL(NULL == hash->array, "hash_init() Failed to allocate memory\n"); \
    for (size_t i = 0 ; i < size ; i++)					\
      hash->array[i].index = _HASH_FREE;				\
  }									\
									\
  static inline void							\
  hash_##name##_init(struct hash_##name *hash)				\
  {									\
    hash_##name##_init_size(hash, HASH_DEFAULT_SIZE);			\
  }									\
									\
  static inline void							\
  hash_##name##_destroy(struct hash_##name *hash)			\
  {									\
    free(hash->array);							\
  }									\
									\
  static inline void							\
  hash_##name##_destroy_full(struct hash_##name *hash,			\
			     void (*destroy_key_func)(keytype),		\
			     void (*destroy_value_func)(valuetype))	\
  {									\
    for (size_t i = 0 ; i < hash->size ; i++)				\
      {									\
	if (0 <= hash->array[i].index)					\
	  {								\
	    if (NULL != destroy_key_func)				\
	      destroy_key_func(hash->array[i].key);			\
	    if (NULL != destroy_value_func)				\
	      destroy_value_func(hash->array[i].value);			\
	  }								\
      }									\
    free(hash->array);							\
  }									\
									\
  static inline int							\
  hash_##name##_set(struct hash_##name *hash, keytype key,		\
		    valuetype value, valuetype *oldvalue);		\
									\
  static inline int							\
  hash_##name##_grow(struct hash_##name *hash, size_t size)		\
  {									\
    struct hash_##name new_hash;					\
									\
    hash_##name##_init_size(&new_hash, size);				\
    for (size_t i = 0 ; i < hash->size ; i++)				\
      {									\
	if (0 <= hash->array[i].index)					\
	  {								\
	    if ( -1 == hash_##name##_set(&new_hash, hash->array[i].key,	\
					 hash->array[i].value, NULL) )	\
	      {								\
		hash_##name##_destroy(&new_hash);			\
		ERROR_UNDEF_RET(1, -1);					\
	      }								\
	  }								\
      }									\
    hash_##name##_destroy(hash);					\
    *hash = new_hash;							\
    return 0;								\
  }									\
									\
  static inline ssize_t							\
  hash_##name##_find_bucket(const struct hash_##name *hash, keytype key) \
  {									\
    size_t index = hash_##name##_hash(hash, key);			\
    ssize_t first_index = index;					\
    size_t increment = 0;						\
    size_t i = 0;							\
    do									\
      {									\
	if (_HASH_FREE == hash->array[index].index)			\
	  return _HASH_FREE;						\
									\
	if ( first_index == hash->array[index].index &&			\
	     equal_func(hash->array[index].key, key) )			\
	  return index;							\
									\
	i++;								\
	if (i >= HASH_MAX_COL)						\
	  return _HASH_NOT_FOUND;					\
	if ( 0 == increment)						\
	  increment = hash_##name##_hash_increment(hash, key);		\
	HASH_INCREMENT(index,increment, hash->size);			\
      }									\
    while (1);								\
    /* Never reach here */						\
    ERROR_UNDEF_RET(1, -1);						\
  }									\
									\
  static inline int							\
  hash_##name##_get(const struct hash_##name *hash, keytype key, valuetype *value) \
  {									\
    ssize_t index = hash_##name##_find_bucket(hash, key);		\
    if ( 0 <= index)							\
      {									\
	*value = hash->array[index].value;				\
	return 1;							\
      }									\
									\
    return 0;								\
  }									\
									\
  static inline int							\
  hash_##name##_set(struct hash_##name *hash, keytype key,		\
		    valuetype value, valuetype *oldvalue)		\
  {									\
    size_t index = hash_##name##_hash(hash, key);			\
    ssize_t first_index = index;					\
    ssize_t delete_index = -1;						\
    size_t increment = 0;						\
    size_t i = 0;							\
    do									\
      {									\
	if (_HASH_FREE == hash->array[index].index)			\
	  {								\
	    if (0 <= delete_index)					\
	      index = delete_index;					\
	    hash->array[index].key = key;				\
	    hash->array[index].value = value;				\
	    hash->array[index].index = first_index;			\
	    return 0;							\
	  }								\
									\
	if ( _HASH_DELETE == hash->array[index].index )			\
	  {								\
	    if ( delete_index < 0)					\
	      delete_index = index;					\
	  }								\
	else if ( first_index == hash->array[index].index &&		\
		  equal_func(hash->array[index].key, key) )		\
	  {								\
	    if (NULL != oldvalue)					\
	      *oldvalue = hash->array[index].value;			\
	    hash->array[index].value = value;				\
	    return 1;							\
	  }								\
									\
	i++;								\
	if (i >= HASH_MAX_COL)						\
	  break;							\
	if ( 0 == increment)						\
	  increment = hash_##name##_hash_increment(hash, key);		\
	HASH_INCREMENT(index, increment, hash->size);			\
      }									\
    while (1);								\
									\
    for (size_t i = 0 ; i < 3 ; i++)					\
      {									\
	if ( -1 != hash_##name##_grow(hash, hash->size*(2 << i)) )	\
	  return hash_##name##_set(hash, key, value, oldvalue);		\
      }									\
    /* if here, really bad luck */					\
    ERROR_UNDEF_RET(1, -1);							\
  }									\
									\
  static inline int							\
  hash_##name##_delete(struct hash_##name *hash, keytype key,		\
		       keytype *oldkey, valuetype *oldvalue)		\
  {									\
    ssize_t index = hash_##name##_find_bucket(hash, key);		\
    if (index < 0)							\
      return 0;								\
									\
    if (NULL != oldkey)							\
      *oldkey = hash->array[index].key;					\
    if (NULL != oldvalue)						\
      *oldvalue = hash->array[index].value;				\
    hash->array[index].index = _HASH_DELETE;				\
    									\
    return 1;								\
  }									\

#endif
