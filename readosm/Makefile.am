ACLOCAL_AMFLAGS = -I m4

SUBDIRS = headers src tests examples

EXTRA_DIST = makefile.vc nmake.opt mainpage.doxy

AUTOMAKE_OPTIONS = dist-zip

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = readosm.pc

coverage-init:
	lcov --directory src --capture --initial --output-file readosm_cov.info

coverage::
	lcov --rc lcov_branch_coverage=1 --directory src --output-file readosm_cov.info --capture
	genhtml --rc lcov_branch_coverage=1 -o covresults readosm_cov.info

MOSTLYCLEANFILES = readosm_cov.info 
