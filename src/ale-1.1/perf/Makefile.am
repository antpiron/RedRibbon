LDADD = $(top_srcdir)/src/libale.la
AM_LDFLAGS = -static
AM_CFLAGS = -O3 -I$(top_srcdir)/src

check_PROGRAMS = p-hash p-stats-std-rand p-portability-getrandom p-alg-ax-b-ols-solve

check-perf:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do ./$$prog; done

.PHONY:	check-perf
