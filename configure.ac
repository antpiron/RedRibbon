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

# Checks for library functions.

AC_SEARCH_LIBS([stats_hyper_F], [ale],
		[LIBALE_SOURCES=],
		[LIBALE_SOURCES='$(wildcard ale-*/src/*.c ale-*/src/*/*.c)'])
AC_SUBST(LIBALE_SOURCES)

AC_SEARCH_LIBS([rrho_init], [redribbon],
			    [],
			    [LIBREDRIBBON_SOURCES='$(wildcard credribbon-*/src/*.c credribbon-*/src/*/*.c)'])
AC_SUBST(LIBREDRIBBON_SOURCES)

dnl substitute RODBC_CPPFLAGS and LIBS
AC_SUBST([PKG_CFLAGS], ["${PKG_CFLAGS}"])
AC_SUBST(LIBS)
AC_CONFIG_HEADERS([src/config.h])
dnl and do substitution in the src/Makevars.in and src/config.h
AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
