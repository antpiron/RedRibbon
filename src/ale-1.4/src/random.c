#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ctype.h>
#include <stdatomic.h>
#include <pthread.h>

#include "ale/portability.h"
#include "ale/error.h"
#include "ale/siphash24.h"
#include "ale/random.h"



static _Thread_local
struct rd_state global_state = {
  .counter = 0,
  .seeded = 0,
  .key = {
    0xc9,0x74,0x58,0x65,0x60,0xb6,0xb8,0x18,
    0xe7,0x63,0xd1,0xb2,0xae,0xc4,0x9f,0x8d } } ;

static pthread_once_t global_state_once = PTHREAD_ONCE_INIT;

static void*
memxor (void *restrict dest, const void *restrict src, size_t n)
{
  const char *s = src;
  char *d = dest;

  for (; n > 0; n--)
    *d++ ^= *s++;

  return dest;
}

static inline int
spread(uint8_t *dst, const uint8_t *src)
{
  // Maybe customized
  static const uint8_t key[SIP_KEYLEN] = { 0x2f,0xb9,0xa4,0x3b,0x80,0xac,0x77,0x7c,
					   0xff,0xde,0xf2,0x23,0xb6,0x7d,0xd8,0xd4 };
  uint64_t plusone = (*(uint64_t*) src) + 1;
  
  siphash(dst, src, SIP_HASHLEN, key);
  siphash(dst+SIP_HASHLEN, (uint8_t *) &plusone, sizeof(plusone), key);

  return 0;
}

static int
add_entropy(uint8_t *key, const uint8_t *src, size_t len)
{
  uint8_t sip[SIP_HASHLEN];
  uint8_t key2[SIP_KEYLEN];
  
  siphash(sip, src, len, key);
  spread(key2, sip);
  memxor(key, key2, SIP_KEYLEN);

  return 0;
}

// TODO: add more entropy: mmap, ...
static int
gen_key(uint8_t *key)
{
  pid_t pid = getpid();
  pid_t ppid = getppid();
  gid_t gid = getgid();
  static const uint8_t key_init[SIP_KEYLEN] = { 0xc9,0x74,0x58,0x65,0x60,0xb6,0xb8,0x18,
						0xe7,0x63,0xd1,0xb2,0xae,0xc4,0x9f,0x8d };
  struct timeval tv;
  struct rusage usage;
  uint64_t timesum;
  
  memcpy(key, key_init, SIP_KEYLEN);

  add_entropy(key, (uint8_t *) &pid, sizeof(pid));
  add_entropy(key, (uint8_t *) &gid, sizeof(gid));
  add_entropy(key, (uint8_t *) &ppid, sizeof(ppid));

  if (-1 != gettimeofday(&tv, NULL) )
    {
      timesum = (uint64_t) tv.tv_sec + (uint64_t) tv.tv_usec;
      add_entropy(key, (uint8_t *) &timesum, sizeof(timesum));
    }

  if (-1 != getrusage(RUSAGE_SELF, &usage) )
    {
      timesum = (uint64_t) usage.ru_utime.tv_sec + (uint64_t) usage.ru_utime.tv_usec;
      add_entropy(key, (uint8_t *) &timesum, sizeof(timesum));
      timesum = (uint64_t) usage.ru_stime.tv_sec + (uint64_t) usage.ru_stime.tv_usec;
      add_entropy(key, (uint8_t *) &timesum, sizeof(timesum));
    }
  
  return 0;
}


static inline void
init_state(struct rd_state *state)
{
  if (0 == state->counter)
    {
      gen_key(state->key);
      state->counter = 1;
    }  
}

static inline void
init_global_state_once()
{
  if (0 == global_state.seeded)
    {
      init_state(&global_state);
    }  
}

// Counter mode generator based on siphash
int
rd_getrandom(void *buf, size_t buflen, unsigned int flags)
{
  (void) (flags);
  uint8_t *ubuf = buf;
  uint8_t sip[SIP_HASHLEN];
  
  pthread_once(&global_state_once, init_global_state_once);
  
  for (size_t i = 0 ; i < buflen ; i += SIP_HASHLEN)
    {
      size_t min = buflen-i;
      if (SIP_HASHLEN < min)
	min = SIP_HASHLEN;
      
      siphash(sip, (uint8_t*) &global_state.counter, sizeof(global_state.counter), global_state.key);
      memcpy(ubuf+i, sip, min);
       
      global_state.counter++;
    }

  return buflen;  
}

void
rd_setseed_r(struct rd_state *state, uint64_t seed)
{
  uint8_t sip[SIP_HASHLEN];
  static const uint8_t key[SIP_KEYLEN] = { 0x4f,0xb9,0x14,0x3b,0x80,0xac,0x73,0x7c,
					   0xfb,0x1e,0xf2,0x03,0xb6,0x7d,0xd8,0x44 };
  
  state->seeded = state->counter = 1;

  siphash(sip, (uint8_t *) &seed, sizeof(seed), key);
  spread(state->key, sip);
  memxor(state->key, key, SIP_KEYLEN);  
}

void
rd_setseed(uint64_t seed)
{
  rd_setseed_r(&global_state, seed);
}

uint64_t
rd_rand_u64_r(struct rd_state *state)
{
  uint64_t ret;
  
  siphash((uint8_t *) &ret, (uint8_t *) & state->counter, sizeof(state->counter), state->key);

  state->counter++;
  
  return ret;
}

uint64_t
rd_rand_u64()
{
  uint64_t ret;
  
  pthread_once(&global_state_once, init_global_state_once);

  ret = rd_rand_u64_r(&global_state);

  return ret;
}
