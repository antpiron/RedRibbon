AM_CFLAGS = $(OPENMP_CFLAGS) -Wall -Wextra -I$(top_srcdir) $(INCLUDE_R_HEADER_PATH) 
AM_LFLAGS=-R


LIBRARY_SOURCES=cRedRibbon.c cRedRibbon.h correlation.c

lib_LTLIBRARIES = libredribbon.la 
#noinst_lib_LTLIBRARIES=libale-debug.la
libredribbon_la_SOURCES = $(LIBRARY_SOURCES)
include_HEADERS = cRedRibbon.h


