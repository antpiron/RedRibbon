LDADD = ../src/librrho.la
AM_LDFLAGS = -static
AM_CFLAGS = -g -O0 -fsanitize=bounds -I$(top_srcdir)/src

check_PROGRAMS = 

TESTS = $(check_PROGRAMS)

check-valgrind:	$(check_PROGRAMS)
	@for prog in $(check_PROGRAMS); do echo "valgrind $$prog"; valgrind -q ./$$prog; done
