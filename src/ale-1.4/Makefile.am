ACLOCAL_AMFLAGS = -I m4
SUBDIRS = src test perf doc

check-perf:
	cd src/ && $(MAKE) $(AM_MAKEFLAGS)
	cd perf && $(MAKE) $(AM_MAKEFLAGS) check-perf

check-valgrind:
	cd test && $(MAKE) $(AM_MAKEFLAGS) check-valgrind

.PHONY:	check-perf check-valgrind
