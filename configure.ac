#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([rrho], [1.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([src/rrho.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
# AC_PROG_CC_C11
ALE_AC_PROG_CC_C11

AC_CHECK_LIB([m],[cos])


##

AC_PROG_LIBTOOL
AM_PROG_CC_C_O
AC_DEFINE([_POSIX_C_SOURCE], [200809L], [Define the posix version])
AC_DEFINE([_XOPEN_SOURCE], [500], [Define the XOPEN])



# Checks for header files.
AC_CHECK_HEADERS([ale.h],,[AC_MSG_ERROR(The ale header is required in order to build the program!)])
# /usr/share/R/include/
INCLUDE_R_HEADER_PATH=""
AC_CHECK_FILES([/usr/share/R/include/R.h /usr/share/R/include/Rinternals.h],
 	       [INCLUDE_R_HEADER_PATH="-I/usr/share/R/include/"
	        AC_DEFINE([HAVE_R_H], [1], [Defined if you have the R header])],)
AC_SUBST(INCLUDE_R_HEADER_PATH)

# Checks for typedefs, structures, and compiler characteristics.


# Checks for library functions.

AC_SEARCH_LIBS([stats_hyper_F], [ale],,[AC_MSG_ERROR([
-------------------------
 libale not found
-------------------------])])

AC_SEARCH_LIBS([Rf_protect], [R],,)
dnl AC_TRY_RUN([
dnl #include <R.h>
dnl #include <Rinternals.h>
dnl #include <stdlib.h>
dnl SEXP mylen(SEXP a){
dnl      int na;
dnl      SEXP ret;
dnl      na = length(a);
dnl      PROTECT(ret = allocVector(INTSXP, 1));
dnl      INTEGER(ret)[0] = na;
dnl      UNPROTECT(1);
dnl      return ret;
dnl }
dnl int main(void)
dnl {
dnl 	return EXIT_SUCCESS;
dnl }
dnl ]
dnl , [ AC_DEFINE([HAVE_R], [1], [Defined if you have the R])
dnl   ]
dnl ,
dnl )


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile perf/Makefile])
AC_OUTPUT
