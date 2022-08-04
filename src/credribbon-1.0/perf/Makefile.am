LDADD = ../src/libredribbon.la
AM_LDFLAGS = -static
AM_CFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = p-rrho p-longdouble p-rrho-permutation p-rrho-permutation-ea


check-perf:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do ./$$prog; done

.PHONY:	check-perf
