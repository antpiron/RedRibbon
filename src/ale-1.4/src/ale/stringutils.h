#ifndef __ALE_STRINGUTILS_H
#define __ALE_STRINGUTILS_H

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <ale/error.h>

char* strjoin(const char *sep, const char **str); 
void strtolower(char *dst, const char *src); 

#define STRING_DEFAULT_SIZE (256)
struct string
{
  char *str;
  size_t len;
  size_t alloc_size;
};

enum {
      STRING_ICASE = 1
};

#define STRING_CONST(STR) &(struct string){.str = STR, .len = strlen(STR),  .alloc_size = 0}

int string_init(struct string *string);
int string_init_size(struct string *string, size_t size);
void string_destroy(struct string *string);

struct string *string_new(char *str);
void string_free(struct string *string);

// Internal usage
static inline void string_resize(struct string *dst, size_t len);

// Main functions
int string_set(struct string *dst, const char *str);
int string_append(struct string *dst, struct string *src);
int string_append_c(struct string *dst, const char *src);
static inline int string_append_c_raw(struct string *dst, const char *src, size_t len);
static inline int string_append_char(struct string *dst, const char src);
static inline ssize_t string_readline(struct string *dst, FILE *file);
static int string_chomp(struct string *dst);
static int string_truncate(struct string *str);
char* string_split(struct string *dst, const char *src, const char *sep, unsigned int options);


// ======
// INLINE
// ======

static inline
int
string_append_char(struct string *dst, const char src)
{
   size_t len = dst->len + 1;
   
   string_resize(dst, len);
   
   dst->str[dst->len] = src;
   dst->str[len] = '\0';
   dst->len = len;

   return 0;
}

static inline
int
string_append_c_raw(struct string *dst, const char *src, size_t len)
{
   size_t newlen = dst->len + len;
   
   string_resize(dst, newlen+1);
   
   memcpy(dst->str + dst->len, src, len);
   dst->str[newlen] = '\0';
   dst->len = newlen;

   return 0;
}

static inline
int
string_truncate(struct string *str)
{
  str->len = 0;
  if (str->alloc_size > 0)
    {
      str->str[0] = 0;
    }

  return 0;
}

static inline
int
string_chomp(struct string *dst)
{
  size_t new_eos = dst->len;

  for ( ; new_eos > 0 ; new_eos--)
    {
      char c = dst->str[new_eos - 1];

      if ('\n' != c && '\r' != c)
	break;

      dst->str[new_eos - 1] = 0;
      dst->len = new_eos - 1;
    }

  // dst->str[new_eos] = 0;
  // dst->len = new_eos;
  
  return 0;
}

/* static inline */
/* ssize_t */
/* string_readline(struct string *dst, FILE *file) */
/* { */
/*   int c; */
  
/*   string_truncate(dst); */
/*   do */
/*     { */
/*       c = getc(file); */
/*       if ( EOF == c ) */
/* 	{ */
/* 	  ERROR_FERROR_RET(0 != ferror(file), -1); */
/* 	  break; */
/* 	} */
	
/*       string_append_char(dst, c); */
/*     } */
/*   while ('\n' != c); */
  
/*   return dst->len; */
/* } */

static inline
ssize_t
string_readline(struct string *dst, FILE *file)
{
 ssize_t ret;
 
 ret = getline(&dst->str, &dst->alloc_size, file);
 ERROR_ERRNO_RET(-1 == ret, -1);
 dst->len = ret;
 
 return dst->len;
}



// ===================
// INTERNAL USAGE ONLY
// ===================

static inline void
string_resize(struct string *dst, size_t len)
{
  if (len + 1 > dst->alloc_size)
    {
      size_t new_size = ((len + 1 + STRING_DEFAULT_SIZE) / STRING_DEFAULT_SIZE) * STRING_DEFAULT_SIZE;

      if (dst->alloc_size > 0)
	dst->str = realloc(dst->str, new_size);
      else
	dst->str = malloc(new_size);
      ERROR_UNDEF_FATAL(NULL == dst->str, "string_append() unable to allocate memory!\n");
      dst->alloc_size = new_size;      
    }
}

#endif
