LDADD = ../src/librrho.la
AM_LDFLAGS = -static
AM_CFLAGS = -I$(top_srcdir)/src

check_PROGRAMS = p-rrho


check-perf:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do ./$$prog; done

.PHONY:	check-perf
