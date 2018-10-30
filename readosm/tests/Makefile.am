check_PROGRAMS = check_osm check_pbf check_err

AM_CFLAGS = -I@srcdir@/../headers
AM_LDFLAGS = -L../src -lreadosm -lz $(GCOV_FLAGS)

TESTS = $(check_PROGRAMS)

MOSTLYCLEANFILES = *.gcna *.gcno *.gcda

EXTRA_DIST = testdata/test.osm testdata/test.osm.pbf \
	testdata/noNodesPackedInfos.osm.pbf
