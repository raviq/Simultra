
AM_CPPFLAGS = -I$(top_srcdir)/headers
AM_CPPFLAGS += -I$(top_srcdir)

lib_LTLIBRARIES = libreadosm.la 

libreadosm_la_SOURCES = readosm.c osm_objects.c osmxml.c protobuf.c

libreadosm_la_CFLAGS = -fvisibility=hidden
libreadosm_la_LDFLAGS = -version-info 1:0:0 -no-undefined

MOSTLYCLEANFILES = *.gcna *.gcno *.gcda
