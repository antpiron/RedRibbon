#ifndef __NETUTILS_H

#define __USE_XOPEN
#include <time.h>
#include <stdint.h>

#include <ale/skiplist.h>
#include <ale/vector.h>

static inline int
cmp_uint64_t(uint64_t a, uint64_t b)
{
  if (a < b)
    return -1;
  else if (a == b)
    return 0;
  else
    return 1;
}

static inline int
cmp_uint32_t(uint32_t a, uint32_t b)
{
  if (a < b)
    return -1;
  else if (a == b)
    return 0;
  else
    return 1;
}

// uint64_t = (ip<<32) | netmask ; uint32_t = asn
SKL_INIT(bgp4,18,uint64_t,uint32_t,cmp_uint64_t)

struct rir
{
  char country[3];
  uint32_t count;
  time_t date;
};

SKL_INIT(rir,18,uint32_t,struct rir,cmp_uint32_t)
VECTOR_INIT_FULL(asn_to_owner,char*, (1ul << 16))

struct netutils
{
  uint32_t asn_len;
  struct vector_asn_to_owner asn_to_owner;
  struct skl_bgp4 bgp4;
  struct skl_rir rir;
};

void netutils_init(struct netutils *nu);
void netutils_destroy(struct netutils *nu);

// http://thyme.apnic.net/current/data-raw-table format <ip>/<mask> <asn>
int netutils_load_bgp4(struct netutils *nu, const char *filename);
uint32_t netutils_bgp4_ip_to_asn(struct netutils *nu, const char *ip);

// http://thyme.apnic.net/current/data-used-autnums <asn>' '<owner>
int netutils_load_asn_to_owner(struct netutils *nu, const char *filename);
char* netutils_asn_to_owner(struct netutils *nu, uint32_t asn);

// ftp://ftp.ripe.net/pub/stats/ripencc/delegated-ripencc-extended-latest
// format ftp://ftp.ripe.net/pub/stats/ripencc/RIR-Statistics-Exchange-Format.txt
int netutils_load_rir(struct netutils *nu, const char *filename);
int netutils_rir_getinfo(struct netutils *nu, const char *ip_str, struct rir **rir);


#endif
