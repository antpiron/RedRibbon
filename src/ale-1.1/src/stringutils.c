#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "ale/portability.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ale/stringutils.h"
#include "ale/error.h"

char* 
strjoin(const char *sep, const char **str)
{
  size_t size = 256;
  size_t res_len = 0;
  char* res = malloc(size);
  ERROR_UNDEF_RET(NULL == res, NULL);

  if (*str)
    {
      size_t sep_len = strlen(sep);

      while (1)
	{
	  for (const char *ch = *str ; '\0' != *ch ; ch++, res_len++)
	    {
	      if (res_len >= size-1)
		{
		  size *= 2; 
		  ERROR_UNDEF_RET(NULL == (res = realloc(res, size)), (free(res), NULL));
		}
	      res[res_len] = *ch;
	    }

	  if ( NULL == *++str)
	    break;

	  if (res_len + sep_len >= size-1)
	    {
	      while (res_len + sep_len >= size-1)
		size *= 2; 
	      ERROR_UNDEF_RET(NULL == (res = realloc(res,size)), (free(res), NULL));
	    }

	  memcpy(res+res_len, sep, sep_len);
	  res_len += sep_len;
	}
    }

  res[res_len] = '\0';

  return res;
}

void
strtolower(char *dst, const char *src)
{
  for ( ; *src ; dst++, src++)
    *dst = tolower(*src);
  *dst = '\0';
}

int
string_init(struct string *string)
{
  return string_init_size(string, STRING_DEFAULT_SIZE);
}

int
string_init_size(struct string *string, size_t size)
{
  string->len = 0;

  if ( size > 0)
    {
      string->str = malloc(size);
      ERROR_UNDEF_FATAL(NULL == string->str,
			"string_init() unable to allocate memory!\n");
    }

  string->alloc_size = size;

  return 0;
}

void
string_destroy(struct string *string)
{
  free(string->str);
}

struct string *
string_new(char *str)
{
  struct string *string = malloc(sizeof(struct string));
  ERROR_UNDEF_RET(NULL == string, NULL);

  string_init_size(string, 0);
  string_set(string, str);

  return string;
}

void
string_free(struct string *string)
{
  string_destroy(string);
  free(string);
}


int
string_set(struct string *dst, const char *str)
{
  size_t len = strlen(str);

  string_resize(dst, len);

  memcpy(dst->str, str, len);
  dst->str[len] = '\0';
  dst->len = len;
  
  return 0;
}

static inline
int
string_append_full_c(struct string *dst, const char *src, size_t src_len)
{
  size_t len = dst->len + src_len;

  string_resize(dst, len);

  memcpy(dst->str + dst->len, src, src_len);
  dst->str[len] = '\0';
  dst->len = len;
    
  return 0;
}

int
string_append(struct string *dst, struct string *src)
{
  return string_append_full_c(dst, src->str, src->len);
}

int
string_append_c(struct string *dst, const char *src)
{
  return string_append_full_c(dst, src, strlen(src));
}


char*
string_split(struct string *dst, const char *src, const char *sep, unsigned int options)
{
  char *sep_pos = NULL;
  
  string_truncate(dst);
  if ('\0' == src[0])
    return NULL;
  
  sep_pos = (options & STRING_ICASE) ? strcasestr(src, sep) : strstr(src, sep);
  if (NULL == sep_pos)
    {
      string_append_c(dst, src);
      return (char*) src + dst->len;
    }

  string_append_c_raw(dst, src, sep_pos - src); 

  return (char*) sep_pos + strlen(sep);
}
