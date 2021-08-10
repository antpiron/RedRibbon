AM_CFLAGS = $(OPENMP_CFLAGS) -Wall -Wextra -I$(top_srcdir) $(INCLUDE_R_HEADER_PATH) 
AM_LFLAGS=-R


LIBRARY_SOURCES=rrho.c rrho.h correlation.c

lib_LTLIBRARIES = librrho.la 
#noinst_lib_LTLIBRARIES=libale-debug.la
librrho_la_SOURCES = $(LIBRARY_SOURCES)
include_HEADERS = rrho.h


