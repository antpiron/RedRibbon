#ifndef __BITSET_H
#define __BITSET_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"

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
  bs->alloc_size = (n + 63) / 64;
  ERROR_ERRNO_RET( NULL == (bs->buf = calloc( bs->alloc_size, sizeof(uint64_t)) ), -1);
  
  return 0;
}

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
  int ret;

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
  memcpy(dst->buf, src->buf,  cpy_size * sizeof(uint64_t));
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
  return ( bs->buf[index / 64] >> (index % 64) ) & 1ull;
}

static inline int
bitset_isempty(struct bitset *bs)
{
  int ret = 0;

  for (size_t i = 0 ; !ret && i < (bs->n + 63) / 64 ; i++)
    ret = ret || bs->buf[i];

  return !ret;
}

static inline ssize_t
bitset_iterate(struct bitset *bs, ssize_t lastpos)
{
  ssize_t startpos = lastpos + 1;

  /* TODO: optimize */
  for (size_t i = startpos ; i < bs->n ; i++)
    {
      if ( bitset_isset(bs, i) )
	return i;
    }
  return -1;
}

static inline void
bitset_xor(struct bitset *dst, struct bitset *a, struct bitset *b)
{
  size_t s = b->n;
  if (a->n < s)
    s = a->n;
  // if (dst->n < s)
  //  s = dst->n;

  bitset_grow(dst, s);
  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = a->buf[i] ^ b->buf[i];
}

static inline void
bitset_and(struct bitset *dst, struct bitset *a, struct bitset *b)
{
  size_t s = b->n;
  if (a->n < s)
    s = a->n;
  // if (dst->n < s)
  //  s = dst->n;

  bitset_grow(dst, s);
  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = a->buf[i] & b->buf[i];
}

static inline void
bitset_or(struct bitset *dst, struct bitset *a, struct bitset *b)
{
  size_t s = b->n;
  if (a->n < s)
    s = a->n;
  // if (dst->n < s)
  //  s = dst->n;

  bitset_grow(dst, s);
  for (size_t i = 0 ; i < (s + 63) / 64 ; i++)
    dst->buf[i] = a->buf[i] | b->buf[i];
}

static inline void
bitset_not(struct bitset *dst, struct bitset *a)
{
  size_t s = a->n;
  size_t s64 = (s-1) / 64 ;
  size_t slast = (s-1) % 64;
  uint64_t mask = (slast == 63)?0ull-1ull:(1ull << (slast + 1)) - 1ull;

  bitset_grow(dst, s);

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

static inline void
bitset_destroy(struct bitset *bs)
{
  free(bs->buf);
}

#endif
