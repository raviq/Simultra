noinst_PROGRAMS = test_osm1 test_osm2 test_osm3

AM_CFLAGS = -I@srcdir@/../headers
AM_LDFLAGS = -L../src -lreadosm -lz $(GCOV_FLAGS)

MOSTLYCLEANFILES = *.gcna *.gcno *.gcda

EXTRA_DIST = examples.doxy
