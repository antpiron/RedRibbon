#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.71])
AC_INIT([ale],[1.1],[BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE([subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/ale.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
# AC_PROG_CC_C11
ALE_AC_PROG_CC_C11

AC_CHECK_LIB([m],[cos])

LT_INIT
AM_PROG_CC_C_O
AC_DEFINE([_POSIX_C_SOURCE], [200809L], [Define the posix version])
AC_DEFINE([_XOPEN_SOURCE], [500], [Define the XOPEN])

# OS detection
case $host in
 *-*-linux*)
	AC_DEFINE([HAVE_LINUX], [], [Defined if you have linux])
	;;
esac

AC_CHECK_FUNCS([getrandom])

AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
#include <unistd.h>
#include <linux/random.h>
#include <sys/syscall.h>

int main(void)
{
        char buf[10];
        long r = syscall(SYS_getrandom, buf, sizeof(buf), 0);
	if (-1 == r)
	   return EXIT_FAILURE;
        return EXIT_SUCCESS;
}

]])],[ HAVE_GETRANDOM_SYSCALL="yes"
    AC_DEFINE([HAVE_GETRANDOM_SYSCALL], [], [Defined if you have the linux getrandom syscall])
  
],[],[])
AC_RUN_IFELSE([AC_LANG_SOURCE([[
#include <stdlib.h>
int main(void)
{
        __builtin_popcountll(10);
        return EXIT_SUCCESS;
}

]])],[ HAVE_POPCOUNTLL="1"
    AC_DEFINE([HAVE_POPCOUNTLL], [], [Defined if you have the popcountll gcc intrinsic])
  
],[],[])


RANDOM_HEADER="#include <sys/random.h>"
RANDOM_FUNC=""
if test "x$ac_cv_func_getrandom" != xyes; then
   RANDOM_HEADER=""
   if test "x$HAVE_GETRANDOM_SYSCALL" == xyes; then
      RANDOM_FUNC="int portability_getrandom_syscall(void *buf, size_t buflen, unsigned int flags);
#define getrandom portability_getrandom_syscall"
   else
      RANDOM_FUNC="#define getrandom portability_getrandom"
   fi
fi

AC_CHECK_FUNCS([strcasestr])

STRCASESTR_FUNC=""
if test "x$ac_cv_func_strcasestr" != xyes; then
   STRCASESTR_FUNC="char* portability_strcasestr(const char *haystack, const char *needle);
#define strcasestr portability_strcasestr"
else
   STRCASESTR_FUNC="char* strcasestr(const char *haystack, const char *needle);"
fi

AC_SUBST(RANDOM_FUNC)
AC_SUBST(RANDOM_HEADER)
AC_SUBST(FADVISE_FUNC)
AC_SUBST(STRCASESTR_FUNC)

AC_OPENMP
AM_CFLAGS="$AM_CFLAGS $OPENMP_CFLAGS"
AC_SUBST([AM_CFLAGS])

## Check size of long long
AC_CHECK_SIZEOF(long long)

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdlib.h string.h])
AC_CHECK_HEADERS([pthread.h],,[AC_MSG_ERROR(The pthread header is required in order to build the program!)])
## AC_CHECK_HEADERS([lapacke.h],,[AC_MSG_ERROR(The lapacke header is required in order to build the program!)])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T

# Checks for library functions.
AC_FUNC_STRERROR_R


# Checks for pthread.
AC_SEARCH_LIBS([pthread_create], [pthread],,[AC_MSG_ERROR([
-------------------------
 libpthread not found
-------------------------])])
dnl AC_SEARCH_LIBS([LAPACKE_dgels], [lapacke],,[AC_MSG_ERROR([
dnl -------------------------
dnl  liblapacke not found
dnl -------------------------])])


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile perf/Makefile doc/Makefile src/ale/portability.h])
AC_OUTPUT
