AC_INIT([RedRibbon], 0.2.0) dnl package name, version


dnl Now find the compiler and compiler flags to use
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi
CC=`"${R_HOME}/bin/R" CMD config CC`
CFLAGS=`"${R_HOME}/bin/R" CMD config CFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`
LDFLAGS=`"${R_HOME}/bin/R" CMD config LDFLAGS`

# Options
AC_ARG_ENABLE([ale], AS_HELP_STRING([--enable-ale], [Search for ale library  @<:@default=yes@:>@]),, [SEARCH_LIBALE=yes])
AC_ARG_ENABLE([cRedRibbon], AS_HELP_STRING([--enable-cRedRibbon], [Search for RedRibbon library  @<:@default=yes@:>@]),, [SEARCH_LIBREDRIBBON=yes])



# Checks for libraries.

PKG_CFLAGS="$PKG_CFLAGS -I."

LIBALE_SOURCES=$(cd src ; find  ale-*/src -name '*.c' | tr '\n' ' ')
if test x"$SEARCH_LIBALE" == xyes ; then
   AC_SEARCH_LIBS([stats_hyper_F], [ale],	
   		[LIBALE_SOURCES=],
		[])
fi

LIBREDRIBBON_SOURCES=$(cd src ; find  credribbon-*/src -name '*.c' | tr '\n' ' ')
if test x"$SEARCH_LIBREDRIBBON" == xyes ; then
   AC_SEARCH_LIBS([rrho_init], [redribbon],
	    	    [LIBREDRIBBON_SOURCES=],
		    [])
fi

PKG_SOURCES=$(cd src ; ls *.c | tr '\n' ' ')

AC_SUBST(PKG_SOURCES)
AC_SUBST(LIBALE_SOURCES)
AC_SUBST(LIBREDRIBBON_SOURCES)

dnl For libale
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


dnl Checks for pthread.
AC_SEARCH_LIBS([pthread_create], [pthread],,[AC_MSG_ERROR([
-------------------------
 libpthread not found
-------------------------])])

dnl substitute RODBC_CPPFLAGS and LIBS
AC_SUBST([PKG_CFLAGS], ["${PKG_CFLAGS}"])
AC_SUBST(LIBS)
AC_CONFIG_HEADERS([src/config.h])
dnl and do substitution in the src/Makevars.in and src/config.h
AC_CONFIG_FILES([src/Makevars src/ale-1.1/src/ale/portability.h])
AC_OUTPUT
