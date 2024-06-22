#include <stdlib.h>
#include <time.h>

#include "ale/error.h"
#include "ale/bitset.h"
#include "ale/random.h"

#define MAX_SET (1ull << 30)
#define MAX_SPARSE_SET (1 << 14)

int
main(int argc, char *argv[argc])
{
  struct bitset bs;
  double diff, rate;
  struct timespec st, et;
  
  printf("%s:\n", argv[0]);

  bitset_init(&bs, MAX_SET);
 
  // set
  clock_gettime(CLOCK_MONOTONIC, &st);	
  for (size_t i = 0 ; i < MAX_SET ; i++)
    {	
      bitset_set(&bs, i);
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = MAX_SET / (diff * 1000 * 1000);
  
  printf("Set %12.2F Mbit/sec\n",rate);

  // iterate
  clock_gettime(CLOCK_MONOTONIC, &st);
  ssize_t val = -1;
  size_t sum = 0;
  while ( bitset_iterate(&bs, &val) )
    {	
      sum ++;
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = MAX_SET / (diff * 1000 * 1000);
  
  printf("Iterate %12.2F Mbit/sec (%zu)\n", rate, sum);

  // iterate sparse
  bitset_reset(&bs);
  for (size_t i = 0 ; i < MAX_SPARSE_SET ; i++)
    {
      bitset_set(&bs, rd_rand_u64() % MAX_SET);
    }
	      
  clock_gettime(CLOCK_MONOTONIC, &st);
  val = -1;
  sum = 0;
  while ( bitset_iterate(&bs, &val) )
    {	
      sum ++;
    }
  clock_gettime(CLOCK_MONOTONIC, &et);
  diff = (et.tv_sec - st.tv_sec) + (et.tv_nsec - st.tv_nsec) / 1e9;
  rate = sum / (diff * 1000 * 1000);
  
  printf("Iterate sparse %12.2F Mbit/sec (%zu)\n", rate, sum);

  
  printf("\n");
  
  bitset_destroy(&bs);


  return EXIT_SUCCESS;
}
