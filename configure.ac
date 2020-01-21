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

# Checks for typedefs, structures, and compiler characteristics.


# Checks for library functions.

# Checks for pthread.
AC_SEARCH_LIBS([stats_hyper_F], [ale],,[AC_MSG_ERROR([
-------------------------
 libale not found
-------------------------])])


AC_CONFIG_FILES([Makefile src/Makefile test/Makefile perf/Makefile])
AC_OUTPUT
