#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <string.h>

#include "ale/netutils.h"

void
netutils_init(struct netutils *nu)
{
  nu->asn_len = 0;
  vector_asn_to_owner_init(&nu->asn_to_owner);
  skl_bgp4_init(&nu->bgp4);
  skl_rir_init(&nu->rir);
}

void
netutils_destroy(struct netutils *nu)
{
  for (size_t i = 0 ; i < nu->asn_len ; i++)
    {
      char *str = vector_asn_to_owner_get(&nu->asn_to_owner, i);
      if (NULL != str)
	free(str);
    }
  
  vector_asn_to_owner_destroy(&nu->asn_to_owner);
  skl_bgp4_destroy(&nu->bgp4);
  skl_rir_destroy(&nu->rir);
}


#define SKIP_AFTER(c) do {			\
    line = strchr(line, (c));			\
    if (NULL == line)				\
      return -1;				\
    line++;					\
  } while (0)

int
parse_ip(const char *line, uint32_t *ip)
{
  // want local endianness (not big endian like inet_aton)
  *ip = ( (uint32_t) atol(line) ) << 24;
  SKIP_AFTER('.');
  *ip |= ( (uint32_t) atol(line) ) << 16;
  SKIP_AFTER('.');
  *ip |= ( (uint32_t) atol(line) ) << 8;
  SKIP_AFTER('.');
  *ip |= (uint32_t) atol(line);

  return 0;
}

static int
parse_line_bgp4(const char *line, uint32_t *ip, uint32_t *netmask, uint32_t *asn)
{
  if ( -1 == parse_ip(line, ip) )
    return -1;

  SKIP_AFTER('/');   
  *netmask = ~ ( ( UINT32_C(1) << (32 - atol(line)) ) - UINT32_C(1) );

  SKIP_AFTER('\t');
  *asn = atol(line);
  
  return 0;
}

#define IP_NETMASK_TO64(ip,netmask) ( (((uint64_t) (ip)) << 32) | (uint64_t) (netmask) )

int
netutils_load_bgp4(struct netutils *nu, const char *filename)
{
#define MAXLEN 256
  char line[MAXLEN];
  FILE *file = fopen(filename, "r");
  ERROR_ERRNO_RET(NULL == file, -1);

  while (1)
    {
      char* ret = fgets(line, MAXLEN, file);
      uint32_t ip, netmask, asn;

      if (NULL == ret)
	{
	  ERROR_FERROR_RET(ferror(file), -1);
	  break;
	}

      if (-1 == parse_line_bgp4(line, &ip, &netmask, &asn))
	continue;

      skl_bgp4_insert(&nu->bgp4, IP_NETMASK_TO64(ip, netmask), asn, NULL);
    }
  
  ERROR_ERRNO_RET(EOF == fclose(file), -1);
  
  return 0;
}

static inline uint32_t
common_prefix_mask(uint32_t a, uint32_t b)
{
  uint32_t n = ~ (a ^ ~b);
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  return ~n;
}

uint32_t
netutils_bgp4_ip_to_asn(struct netutils *nu, const char *ip_str)
{
  struct skl_bgp4_node *node;
  uint32_t ip;
  
  if ( -1 == parse_ip(ip_str, &ip) )
    return 0;

  for (uint32_t netmask = ~ UINT32_C(0) ; 0 != netmask ; netmask <<= 1, ip &= netmask)
    {
      int ret = skl_bgp4_search(&nu->bgp4, IP_NETMASK_TO64(ip, netmask), &node);
      
      if ( ret )
	return node->value;
      else if (node != &nu->bgp4.header)
      	{
      	  uint32_t found_ip = node->key >> 32;
      	  uint32_t found_netmask = node->key & 0xFFFFFFFF;
	  
      	  if ( found_ip == (ip & found_netmask) )
      	    return node->value;
      	  else
	    {
	      uint32_t cpm = common_prefix_mask(ip, found_ip);

	      if (cpm < netmask)
		netmask = cpm;

	      if (found_netmask < netmask)
		netmask = found_netmask;
	    }
      	}
      else
      	return 0;
    }
  
  return 0;
}

#define SKIP_SPACES()				\
    while (*line && isspace(*line))		\
      {  line++; }

static int
parse_line_asn_to_owner(const char *line, uint32_t *asn, char **owner)
{
  char *str;

  SKIP_SPACES()
  *asn = atol(line);
  SKIP_AFTER(' ');

  str = *owner = strdup(line);
  ERROR_UNDEF_FATAL(NULL == *owner, "Unable to allocate memory in parse_line_asn_owner()\n");

  while (*str && *str != '\n') str++;
  *str = '\0';
  
  return 0;
}

int
netutils_load_asn_to_owner(struct netutils *nu, const char *filename)
{
  char line[MAXLEN];
  FILE *file = fopen(filename, "r");
  ERROR_ERRNO_RET(NULL == file, -1);

  while (1)
    {
      char* ret = fgets(line, MAXLEN, file);
      uint32_t asn;
      char *owner;

      if (NULL == ret)
	{
	  ERROR_FERROR_RET(ferror(file), -1);
	  break;
	}

      if (-1 == parse_line_asn_to_owner(line, &asn, &owner))
	continue;

      vector_asn_to_owner_set(&nu->asn_to_owner, asn, owner);
      
      for (size_t i = nu->asn_len ; i < asn ; i++)
	vector_asn_to_owner_set(&nu->asn_to_owner, i, NULL);
      nu->asn_len = asn + 1;
    }
  
  ERROR_ERRNO_RET(EOF == fclose(file), -1);
  
  return 0;
}


char*
netutils_asn_to_owner(struct netutils *nu, uint32_t asn)
{
  if (asn < nu->asn_len)
    return vector_asn_to_owner_get(&nu->asn_to_owner, asn);

  return NULL;
}

static int
parse_line_rir(const char *line, uint32_t *ip, struct rir *rir)
{
  struct tm tm = {0};
  // ripecc
  SKIP_AFTER('|');

  if ( ! ( isupper(*line) && isupper(line[1]) ) )
    return -1;
  
  rir->country[0] = line[0];
  rir->country[1] = line[1];
  rir->country[2] = '\0';
  
  SKIP_AFTER('|');

  if ( 0 != strncmp("ipv4", line, 4))
    return -1;

  SKIP_AFTER('|');
  if ( '*' == *line)
    return -1;
  
  if ( -1 == parse_ip(line, ip) )
    return -1;

  SKIP_AFTER('|');

  rir->count = atol(line);

  SKIP_AFTER('|');
  if ( NULL == strptime(line, "%Y%m%d", &tm) )
    return -1;
  rir->date = mktime(&tm);

  return 0;
}

int
netutils_load_rir(struct netutils *nu, const char *filename)
{
  char line[MAXLEN];
  FILE *file = fopen(filename, "r");
  ERROR_ERRNO_RET(NULL == file, -1);

  while (1)
    {
      char* ret = fgets(line, MAXLEN, file);
      struct rir rir;
      uint32_t ip;

      if (NULL == ret)
	{
	  ERROR_FERROR_RET(ferror(file), -1);
	  break;
	}

      if (-1 == parse_line_rir(line, &ip, &rir))
	continue;

      skl_rir_insert(&nu->rir, ip, rir, NULL);
    }
  
  ERROR_ERRNO_RET(EOF == fclose(file), -1);
  
  return 0;

}

int
netutils_rir_getinfo(struct netutils *nu, const char *ip_str, struct rir **rir)
{
  uint32_t ip;
  struct skl_rir_node *node;
  
  if (-1 == parse_ip(ip_str, &ip) )
    return 0;

  skl_rir_search(&nu->rir, ip, &node);
  if (node != &nu->rir.header
      && node->key <= ip
      && ip <= node->key + node->value.count - 1)
    {
      *rir = &node->value;
      return 1;
    }
  
  return 0;
}
