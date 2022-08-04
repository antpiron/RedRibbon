#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <sys/time.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <unistd.h>
#include <ctype.h>
#include <stdatomic.h>

#include "ale/portability.h"
#include "ale/error.h"
#include "ale/siphash24.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int 
portability_posix_fadvise(int fd, off_t offset, off_t len, int advice)
{
  

  return 0;
}
#pragma GCC diagnostic pop


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
  uint64_t plusone = ++(*(uint64_t*) src);
  
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

// TODO: find better and PROVEN method to generate entropy
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

// Counter mode generator based on siphash
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int
portability_getrandom(void *buf, size_t buflen, unsigned int flags)
{
  // TODO: make it thread safe
  uint8_t *ubuf = buf;
  static _Atomic uint64_t counter = 0;
  uint8_t sip[SIP_HASHLEN];
  static uint8_t key[SIP_KEYLEN] = {0};

  // should be done only one time (lock?)
  if (0 == counter)
    gen_key(key);

  for (size_t i = 0 ; i < buflen ; i += SIP_HASHLEN)
    {
      size_t min = buflen-i;
      if (SIP_HASHLEN < min)
	min = SIP_HASHLEN;
      
      siphash(sip, (uint8_t*) &counter, sizeof(counter), key);
      memcpy(ubuf+i, sip, min);
       
      counter++;
    }
  
  return buflen;  
}
#pragma GCC diagnostic pop

#ifdef HAVE_GETRANDOM_SYSCALL
#include <linux/random.h>
#include <sys/syscall.h>
// Where is this fucking declaration in ubuntu???
long syscall(long number, ...);
int
portability_getrandom_syscall(void *buf, size_t buflen, unsigned int flags)
{
  return syscall(SYS_getrandom, buf, buflen, flags);
}
#endif

char*
portability_strcasestr(const char *haystack, const char *needle)
{
  char *result = NULL;
  // TODO: use Knuth–Morris–Pratt algorithm
  for (char *h = (char*) haystack ; *h ; h++)
    {
      char *n = (char*) needle;
      for (char *hh = (char*) h ; *n && *hh ; n++, hh++)
	if ( tolower(*hh) != tolower(*n) )
	  break;
      
      if ('\0' == *n)
	{
	  result = h;
	  break;
	}
    }

  return result;
}

