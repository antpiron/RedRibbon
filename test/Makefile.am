LDADD = $(top_srcdir)/src/librrho.la
AM_LDFLAGS = -static
AM_CFLAGS = -g -O0 -fsanitize=bounds -I$(top_srcdir)/src

check_PROGRAMS = t-rrho t-rrho-hyper t-rrho-generic t-rrho-rectangle t-rrho-rectangle-min-ea

TESTS = $(check_PROGRAMS) t-rrho-r-rectangle.R t-rrho-r-rectangle-min-ea.R

check-valgrind:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do echo "valgrind $$prog"; valgrind -q ./$$prog; done
