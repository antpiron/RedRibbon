LDADD = $(top_srcdir)/src/libale.la
AM_LDFLAGS = -static
AM_CFLAGS = -O3 -I$(top_srcdir)/src

check_PROGRAMS = 

check-perf:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do ./$$prog; done

.PHONY:	check-perf
