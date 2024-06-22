#ifndef __SIPHASH24_H
#define __SIPHASH24_H

// Source: https://131002.net/siphash/

#include <stdint.h>

// 128 bits version removed
#define SIP_KEYLEN 16
#define SIP_HASHLEN 8


// out: output HASHLEN bytes
// in: Input buffer
// inlen: Size of input buffer
// k: SIP_KEYLEN bytes for keyed hash
int siphash(uint8_t *out, const uint8_t *in, uint64_t inlen, const uint8_t *k);

#endif
