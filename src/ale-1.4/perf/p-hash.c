#include <stdlib.h>
#include <time.h>

#include "ale/error.h"
#include "ale/hash.h"

#define MAX_INSERT (1 << 22)

HASH_INIT(int, int , int, equal_func_int, hash_func_int)
HASH_INIT(int_fast, int , int, equal_func_int, hash_func_int_fast)
  
#define TEST(name)							\
  do {									\
    struct hash_##name hash;						\
    int ret, val, key;							\
    double diff, rate;							\
    struct timespec st, et;						\
									\
    printf("\nhash function = %s\n", #name);				\
    hash_##name##_init_size(&hash, MAX_INSERT << 2);			\
									\
    clock_gettime(CLOCK_MONOTONIC, &st);				\
    for (int i = 0 ; i < MAX_INSERT ; i++)				\
      {									\
	ERROR_UNDEF_FATAL_FMT(0 != (ret = hash_##name##_set(&hash, i, i, &val)), \
			      "FATAL: hash_" #name "_set(%d) returned %d\n", i, ret); \
      }									\
    clock_gettime(CLOCK_MONOTONIC, &et);				\
    diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;	\
    rate = MAX_INSERT / (diff * 1000 * 1000);				\
    									\
    printf("Set %12.2F MKeys/sec",rate);				\
    									\
    									\
    clock_gettime(CLOCK_MONOTONIC, &st);				\
    for (int i = 0 ; i < MAX_INSERT ; i++)				\
      {									\
	ERROR_UNDEF_FATAL_FMT(1 != (ret = hash_##name##_get(&hash, i, &val)), \
			      "FATAL: hash_" #name "_get(%d) returned %d\n", i, ret); \
	ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_" #name "_get(%d) value == %d != %d\n", i, val, i); \
      }									\
    clock_gettime(CLOCK_MONOTONIC, &et);				\
    diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;	\
    rate = MAX_INSERT / (diff * 1000 * 1000);				\
    									\
    printf("\nGet %12.2F MKeys/sec", rate);				\
    									\
    clock_gettime(CLOCK_MONOTONIC ,&st);				\
    for (int i = 0 ; i < MAX_INSERT ; i++)				\
      {									\
	ret = hash_##name##_delete(&hash, i, &key, &val);			\
	ERROR_UNDEF_FATAL_FMT(1 != ret, "FATAL: hash_" #name "_delete(%d) returned %d\n", i, ret); \
	ERROR_UNDEF_FATAL_FMT(i != key, "FATAL: hash_" #name "_delete(%d) key == %d != %d\n", i, key, i); \
	ERROR_UNDEF_FATAL_FMT(i != val, "FATAL: hash_" #name "_delete(%d) value == %d != %d\n", i, val, i); \
      }									\
    clock_gettime(CLOCK_MONOTONIC, &et);				\
    diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;	\
    rate = MAX_INSERT / (diff * 1000 * 1000);				\
    									\
    printf("\nDel %12.2F MKeys/sec",rate);				\
    									\
    printf("\n");							\
    									\
    hash_##name##_destroy(&hash);					\
    } while (0)

  
int
main(int argc, char *argv[argc])
{
  printf("%s: ", argv[0]);

  TEST(int);
  TEST(int_fast);
    
  return EXIT_SUCCESS;
}
