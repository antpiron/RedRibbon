#ifndef __ALE_RANDOM_H
#define __ALE_RANDOM_H


#include <stdint.h>
#include <ale/siphash24.h>


struct rd_state
{
  uint64_t counter;
  int seeded;
  uint8_t key[SIP_KEYLEN];
};


void rd_setseed(uint64_t seed);
void rd_setseed_r(struct rd_state *state, uint64_t seed);

uint64_t rd_rand_u64();
uint64_t rd_rand_u64_r(struct rd_state *state);


int rd_getrandom(void *buf, size_t buflen, unsigned int flags);


#endif
