#include <stdlib.h>

#include "ale/error.h"
#include "ale/hash.h"

#define MAX_INSERT (1 << 18)

HASH_INIT(int, int , int, equal_func_int, hash_func_int)
HASH_INIT(string, struct string*, struct string*, equal_func_string, hash_func_string)
HASH_INIT(chars, char*, int, equal_func_chars, hash_func_chars)

int
main(int argc, char *argv[argc])
{
  struct hash_int hash;
  struct hash_string shash;
  struct hash_chars chars_hash;
  struct string *skey, *skey_ret;
  struct string *sval, *sval_ret;
  int ret, val, key;
  
  hash_int_init(&hash);
  ERROR_UNDEF_FATAL_FMT(HASH_DEFAULT_SIZE != hash.size, "FATAL: hash.size == %zu != %d\n", hash.size, HASH_DEFAULT_SIZE);

  ret = hash_int_set(&hash, 1, 1, NULL);
  ERROR_UNDEF_FATAL_FMT(0 != ret, "FATAL: hash_int_set(1) returned %d\n", ret);

  ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_int_get(&hash, 1, &val)), "FATAL: hash_int_get(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_get(1) value == %d != 1\n", val);

  val = 0;
  ret = hash_int_set(&hash, 1, 1, &val);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_set(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_set(1) value == %d != 1\n", val);

  ret = hash_int_delete(&hash, 1, &key, &val);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_delete(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != key, "FATAL: hash_int_delete(1) key == %d != 1\n", key);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_int_delete(1) value == %d != 1\n", val);

  ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_int_delete(&hash, 1, &key, &val)),
			"FATAL: hash_int_delete(1) returned %d\n", ret);

  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_int_set(&hash, i, i, &val)), "FATAL: hash_int_set(%d) returned %d\n", i, ret);
    }

  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_int_get(&hash, i, &val)), "FATAL: hash_int_get(%d) returned %d\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_int_get(%d) value == %d != %d\n", i, val, i);
    }
  
  for (int i = 0 ; i < MAX_INSERT ; i++)
    {
      ret = hash_int_delete(&hash, i, &key, &val);
      ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_int_delete(%d) returned %d\n", i, ret);
      ERROR_UNDEF_FATAL_FMT(i != key, "FATAL: hash_int_delete(%d) key == %d != %d\n", i, key, i);
      ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_int_delete(%d) value == %d != %d\n", i, val, i);
    }
  
  hash_int_destroy(&hash);

  // Strings

  fprintf(stderr,"HERE\n");
  skey = string_new("key");
  ERROR_FATAL(NULL == skey, "FATAL: string_new()\n");
  sval = string_new("val");
  ERROR_FATAL(NULL == sval, "FATAL: string_new()\n");

  fprintf(stderr, "HERE\n");
  hash_string_init(&shash);
  ERROR_UNDEF_FATAL_FMT(HASH_DEFAULT_SIZE != shash.size,
			"FATAL: hash.size == %zu != %d\n", shash.size, HASH_DEFAULT_SIZE);

  printf("HERE\n");
  ret = hash_string_set(&shash, skey, sval, NULL);
  ERROR_UNDEF_FATAL_FMT(0 != ret, "FATAL: hash_string_set(1) returned %d\n", ret);
  
  ret = hash_string_get(&shash, skey, &sval_ret);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_string_get(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL(sval != sval_ret, "FATAL: hash_string_get('key') != \n");

  ret = hash_string_delete(&shash, skey, &skey_ret, &sval_ret);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_string_delete(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL(skey != skey_ret, "FATAL: hash_string_delete(1) wrong key\n");
  ERROR_UNDEF_FATAL(sval != sval_ret, "FATAL: hash_string_delete(1) wrong val\n");

  ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_string_delete(&shash, skey, &skey_ret, &sval_ret)),
			"FATAL: hash_string_delete(1) returned %d\n", ret);


  hash_string_destroy(&shash);
  string_free(skey);

  hash_chars_init(&chars_hash);
  
  ret = hash_chars_set(&chars_hash, "Plop!", 1, NULL);
  ERROR_UNDEF_FATAL_FMT(0 != ret, "FATAL: hash_chars_set(1) returned %d\n", ret);

  ret = hash_chars_get(&chars_hash, "Plop!", &val);
  ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_chars_get(1) returned %d\n", ret);
  ERROR_UNDEF_FATAL_FMT(1 != val, "FATAL: hash_chars_get('key') val = %d != 1\n", val);

  hash_chars_destroy(&chars_hash);

  
  return EXIT_SUCCESS;
}
