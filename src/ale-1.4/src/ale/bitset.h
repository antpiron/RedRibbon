#ifndef __BITSET_H
#define __BITSET_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <ale/portability.h>
#include <ale/error.h>
#include <ale/memory.h>

struct bitset
{
  size_t n;
  size_t alloc_size;
  uint64_t *buf;
};

static inline int
bitset_init(struct bitset *bs, size_t n)
{
  bs->n = n;
  bs->alloc_size = (0 == n) ? 1 : (n + 63) / 64;
  ERROR_ERRNO_RET( NULL == (bs->buf = calloc( bs->alloc_size, sizeof(uint64_t)) ), -1);
  
  return 0;
}

static inline void
bitset_destroy(struct bitset *bs)
{
  free(bs->buf);
}

static inline struct bitset*
bitset_new(size_t n)
{
  struct bitset *bs = malloc(sizeof(struct bitset));
  
  ERROR_ERRNO_RET( NULL == bs, NULL);
 
  ERROR_RET( -1 == bitset_init(bs, n), NULL);
  
  return bs;
}

static inline void
bitset_free(struct bitset *bs)
{
  bitset_destroy(bs);
  free(bs);
}

static inline struct bitset*
bitset_new_pool(struct mem_pool *pool, size_t n)
{
  struct bitset *bs = mem_malloc(pool, sizeof(struct bitset));
  
  ERROR_RET( NULL == bs, NULL);
 
  ERROR_RET( -1 == bitset_init(bs, n), NULL);

  mem_callback(pool, bs, (mem_callback_func_t) bitset_destroy);
  
  return bs;
}

/* Be careful if changing code here,
   undefined behavior for << and >> when shifted by BITCHAR * sizeof(uint64_t)
   "If the value of the right operand is negative or is greater than or equal to the width of
   the promoted left operand, the behavior is undefined."
   Everything looks fine by now.
*/
static inline void
bitset_setrange_no_grow(struct bitset *bs, size_t index, size_t len)
{
  size_t i1 = index / 64u;
  size_t m1 = index % 64u;
  size_t index2 = index + len - 1u;
  size_t i2 = index2 / 64u;
  size_t m2 = index2 % 64u;

  if (i1 == i2)
    bs->buf[i1] |= ~( (1ull << m1) - 1ull ) & (~0ull >> (63-m2) );
  else
    {
      bs->buf[i1] |= ~( (1ull << m1) - 1ull );
      for (size_t i = i1+1 ; i < i2 ; i++)
	bs->buf[i] = ~0ull;
      bs->buf[i2] |= ~0ull >> (63-m2);      
    }
}

static inline void
bitset_unsetrange_no_grow(struct bitset *bs, size_t index, size_t len)
{
  size_t i1 = index / 64u;
  size_t m1 = index % 64u;
  size_t index2 = index + len - 1u;
  size_t i2 = index2 / 64u;
  size_t m2 = index2 % 64u;

  if (i1 == i2)
    bs->buf[i1] &= ((1ull << m1) - 1ull) | ~(~0ull >> (63-m2));
  else
    {
      bs->buf[i1] &=  (1ull << m1) - 1ull ;
      for (size_t i = i1+1 ; i < i2 ; i++)
	bs->buf[i] = 0ull;
      bs->buf[i2] &= ~(~0ull >> (63-m2));      
    }
}

static inline int
bitset_grow(struct bitset *bs, size_t n)
{
  if ( n > bs->n)
    {
      size_t realloc_size = (n + 63) / 64;
  
      if (realloc_size > bs->alloc_size)
	{
	  void *ptr;
	  realloc_size += bs->alloc_size;
	  ptr = realloc(bs->buf, realloc_size * sizeof(uint64_t));
	  ERROR_ERRNO_RET( NULL == ptr, -1);
	  bs->buf = ptr;

	  // Zeroes only if realloc
	  memset(bs->buf + bs->alloc_size, 0, sizeof(uint64_t) * (realloc_size - bs->alloc_size));
	  bs->alloc_size = realloc_size;
	}

      bs->n = n;
   }

  return 0;
}

static inline int
bitset_resize_n(struct bitset *bs, size_t n)
{
  int ret = 0;

  if ( n < bs->n )
    {
      // cleanup already allocated bits
      bitset_unsetrange_no_grow(bs, n, bs->n - n);
    }
  else
    {
      ret = bitset_grow(bs, n);
      ERROR_RET(0 != ret, ret);
    }
  
  bs->n = n;

  return ret;
}

static inline void
bitset_reset(struct bitset *bs)
{
  for (size_t i = 0 ; i < (bs->n + 63) / 64 ; i++)
    bs->buf[i] = 0ull;
}

static inline int
bitset_cpy(struct bitset *dst, struct bitset *src)
{
  size_t s = src->n;

  bitset_grow(dst, s);

  size_t cpy_size = (s + 63) / 64;
  if ( cpy_size > 0 )
    {
      memcpy(dst->buf, src->buf,  cpy_size * sizeof(uint64_t));
    }
  // Zeroes the rest
  size_t zero_size = dst->alloc_size - cpy_size;
  if ( zero_size > 0 )
    memset(dst->buf + cpy_size, 0, sizeof(uint64_t) * zero_size);
  dst->n = s;
    
  return 0;
}

static inline void
bitset_set(struct bitset *bs, size_t index)
{
  bitset_grow(bs, index+1);
  bs->buf[index / 64] |= 1ull << (index % 64);
}

static inline void
bitset_unset(struct bitset *bs, size_t index)
{
  bitset_grow(bs, index+1);
  bs->buf[index / 64] &= ~(1ull << (index % 64));
}

static inline int
bitset_isset(struct bitset *bs, size_t index)
{
  
  return (index < bs->n) ? ( ( bs->buf[index / 64] >> (index % 64) ) & 1ull ) : 0;
}

static inline int
bitset_isempty(struct bitset *bs)
{
  int ret = 0;

  for (size_t i = 0 ; !ret && i < (bs->n + 63) / 64 ; i++)
    ret = ret || bs->buf[i];

  return !ret;
}

static inline int
bitset_iterate(struct bitset *bs, ssize_t *state)
{
  size_t startpos = (*state < 0) ? 0 : *state + 1;

  if (startpos < bs->n)
    {
      size_t startpos_mod = startpos % 64;
      
      if (0 < startpos_mod &&
	  0 == (bs->buf[startpos / 64] >> startpos_mod) )
	startpos += 64 - startpos_mod;
      
      /* here, if startpos is not divisible by 64 then some bits are non-null in the current uint64_t */
      for ( ;  startpos < bs->n ; startpos += 64)
	{
	  uint64_t v = bs->buf[startpos / 64];
	  
	  if (0 != v)
	    {
	      size_t c;
	      
	      v = v >> (startpos % 64);
	      
	      v = (v ^ (v - 1)) >> 1;  // Set v's trailing 0s to 1s and zero rest
	      for (c = 0; v; c++)
		{
		  v >>= 1;
		}
	      *state = startpos + c;
	      return 1;
	    }
	}
    }
  
  *state = (ssize_t) bs->n;
  
  return 0;
}


#define BITSET_OP(NAME, OP)						\
  static inline void							\
  bitset_##NAME(struct bitset *dst, struct bitset *a, struct bitset *b)	\
  {									\
    size_t max = MAX(dst->n, MAX(a->n, b->n));				\
    size_t a64 = (a->n + 63) / 64;					\
    size_t b64 = (b->n + 63) / 64;					\
									\
    bitset_grow(dst, max);				                \
									\
    for (size_t i = 0 ; i < (max + 63) / 64 ; i++)			\
      {									\
	uint64_t aa = (i < a64) ? a->buf[i] : 0;			\
	uint64_t bb = (i < b64) ? b->buf[i] : 0;			\
	dst->buf[i] = aa OP bb;						\
      }									\
   }

BITSET_OP(xor, ^)
BITSET_OP(and, &)
BITSET_OP(or, |)
  

static inline void
bitset_not(struct bitset *dst, struct bitset *a)
{
  size_t s = a->n;
  size_t s64 = (s-1) / 64 ;
  size_t slast = (s-1) % 64;
  uint64_t mask = (slast == 63)?0ull-1ull:(1ull << (slast + 1)) - 1ull;

  bitset_resize_n(dst, s);

  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = ~ a->buf[i];

  dst->buf[s64] &=  mask;
}

static inline size_t
bitset_ones(struct bitset *bs)
{
  // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan
  size_t c = 0; // c accumulates the total bits set in v

  for (size_t i = 0 ; i < (bs->n + 63) / 64 ; i++)
    {
#if defined HAVE_POPCOUNTLL && SIZEOF_LONG_LONG == 8
      c += __builtin_popcountll(bs->buf[i]);
#else
      uint64_t v = bs->buf[i];
      for (; v; c++)
	v &= v - 1; // clear the least significant bit set
#endif
     }
  
  return c;
}


static inline void
bitset_setrange(struct bitset *bs, size_t index, size_t len)
{
  bitset_grow(bs, index + len);
  bitset_setrange_no_grow(bs, index, len);
}

static inline void
bitset_unsetrange(struct bitset *bs, size_t index, size_t len)
{
  bitset_grow(bs, index + len);
  bitset_unsetrange_no_grow(bs, index, len);
}


#endif
