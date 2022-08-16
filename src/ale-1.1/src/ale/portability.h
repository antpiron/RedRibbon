#ifndef __PORTABILITY_H
#define __PORTABILITY_H

#include <limits.h>
#include <netdb.h>
#include <sys/types.h>
#include <stdint.h>
#include <math.h>
//#include <sys/random.h>
#include <sys/random.h>

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#if defined(__linux__)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define be16toh(x) betoh16(x)
#  define be32toh(x) betoh32(x)
#  define be64toh(x) betoh64(x)
#endif

#ifndef htonll
#define htonll(x) htobe64(x)
#endif

#ifndef ntohll
#define ntohll(x) be64toh(x)
#endif

// (Not in POSIX.1-2001.)  Print to stderr as well.
#ifndef LOG_PERROR
# define LOG_PERROR 0
#endif


#ifndef HAVE_POSIX_FADVISE
#  define posix_fadvise portability_posix_fadvise
#  ifndef POSIX_FADV_SEQUENTIAL
#    define POSIX_FADV_SEQUENTIAL 0
#  endif
#endif

#ifndef M_PI
# define M_PI 3.1415926535897932
#endif
  
int portability_posix_fadvise(int fd, off_t offset, off_t len, int advice);
int portability_getrandom(void *buf, size_t buflen, unsigned int flags);

char* portability_strcasestr(const char *haystack, const char *needle);




char* strcasestr(const char *haystack, const char *needle);

#endif
