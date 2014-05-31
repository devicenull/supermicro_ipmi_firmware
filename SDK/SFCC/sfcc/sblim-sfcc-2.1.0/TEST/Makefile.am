AM_CPPFLAGS = \
	-I$(srcdir)/../frontend/sfcc \
	-I$(top_srcdir)/cimc \
	-I../../ \
	$(MELUNIT_CXX_CFLAGS) \
	-I$(top_builddir)/src \
	-I$(top_srcdir)/src

if HAVE_MELUNIT_CXX

AM_LDFLAGS = \
	-L$(top_builddir)/src/lib -lcmpisfcc  \
	$(MELUNIT_CXX_LIBS)

OUR_UNIT_TESTS = \
	test_native

check_PROGRAMS = $(OUR_UNIT_TESTS)
TESTS = $(OUR_UNIT_TESTS)

OUR_LDADD = \
	$(top_builddir)/src/lib/libcmpisfcc.la

test_native_SOURCES = test_objectpath.cpp
test_native_LDADD = $(OUR_LDADD)

endif

noinst_HEADERS = show.h

noinst_PROGRAMS	= test \
                  test_an \
                  test_an_2 \
                  test_as \
                  test_as_2 \
                  test_ci \
                  test_di \
                  test_ec \
                  test_ecn \
                  test_ecn_ssl \
                  test_ei \
                  test_ein \
                  test_gc \
                  test_gi \
                  test_gp \
                  test_im1 \
                  test_im2 \
                  test_im3 \
                  test_im4 \
                  test_imos \
                  test_leak \
                  test_rf \
                  test_rfn \
                  test_si \
                  test_sp \
                  test_xq \
                  test_cpp \
                  test_qualifiers \
                  test_qualifiers2 \
                  v2test_ec \
                  v2test_ecn \
                  v2test_ei \
                  v2test_ein \
                  v2test_gc \
									print-types

test_SOURCES = test.c show.c
test_LDADD = ../libcmpisfcc.la

test_cpp_SOURCES = test_cpp.cpp

test_an_SOURCES = test_an.c \
                   show.c
test_an_LDADD = ../libcmpisfcc.la

test_an_2_SOURCES = test_an_2.c \
                   show.c
test_an_2_LDADD = ../libcmpisfcc.la

test_as_SOURCES = test_as.c \
                   show.c
test_as_LDADD = ../libcmpisfcc.la

test_as_2_SOURCES = test_as_2.c \
                   show.c
test_as_2_LDADD = ../libcmpisfcc.la

test_ci_SOURCES = test_ci.c \
                   show.c
test_ci_LDADD = ../libcmpisfcc.la

test_di_SOURCES = test_di.c \
                   show.c
test_di_LDADD = ../libcmpisfcc.la

test_ec_SOURCES = test_ec.c \
                   show.c
test_ec_LDADD = ../libcmpisfcc.la

test_ecn_SOURCES = test_ecn.c \
                   show.c
test_ecn_LDADD = ../libcmpisfcc.la

test_ecn_ssl_SOURCES = test_ecn_ssl.c \
                   show.c
test_ecn_ssl_LDADD = ../libcmpisfcc.la

test_ei_SOURCES = test_ei.c \
                   show.c
test_ei_LDADD = ../libcmpisfcc.la

test_ein_SOURCES = test_ein.c \
                   show.c
test_ein_LDADD = ../libcmpisfcc.la

test_gc_SOURCES = test_gc.c \
                  show.c
test_gc_LDADD = ../libcmpisfcc.la

test_gi_SOURCES = test_gi.c \
                  show.c
test_gi_LDADD = ../libcmpisfcc.la

test_gp_SOURCES = test_gp.c \
                  show.c
test_gp_LDADD = ../libcmpisfcc.la

test_im1_SOURCES = test_im1.c \
                  show.c
test_im1_LDADD = ../libcmpisfcc.la

test_im2_SOURCES = test_im2.c \
                  show.c
test_im2_LDADD = ../libcmpisfcc.la

test_im3_SOURCES = test_im3.c \
                  show.c
test_im3_LDADD = ../libcmpisfcc.la

test_im4_SOURCES = test_im4.c \
                  show.c
test_im4_LDADD = ../libcmpisfcc.la

test_imos_SOURCES = test_imos.c \
                  show.c
test_imos_LDADD = ../libcmpisfcc.la

test_leak_SOURCES = test_leak.c \
                  show.c
test_leak_LDADD = ../libcmpisfcc.la

test_rf_SOURCES = test_rf.c \
                  show.c
test_rf_LDADD = ../libcmpisfcc.la

test_rfn_SOURCES = test_rfn.c \
                  show.c
test_rfn_LDADD = ../libcmpisfcc.la

test_si_SOURCES = test_si.c \
                  show.c
test_si_LDADD = ../libcmpisfcc.la

test_sp_SOURCES = test_sp.c \
                  show.c
test_sp_LDADD = ../libcmpisfcc.la

test_xq_SOURCES = test_xq.c \
                  show.c
test_xq_LDADD = ../libcmpisfcc.la

test_qualifiers_SOURCES = test_qualifiers.c \
                  show.c
test_qualifiers_LDADD = ../libcmpisfcc.la

test_qualifiers2_SOURCES = test_qualifiers2.c \
                  show.c
test_qualifiers2_LDADD = ../libcmpisfcc.la

v2test_ec_SOURCES = v2test_ec.c show.c
v2test_ec_LDADD   = ../libcmpisfcc.la

v2test_ecn_SOURCES = v2test_ecn.c show.c
v2test_ecn_LDADD   = ../libcmpisfcc.la

v2test_ei_SOURCES = v2test_ei.c show.c
v2test_ei_LDADD   = ../libcmpisfcc.la

v2test_ein_SOURCES = v2test_ein.c show.c
v2test_ein_LDADD   = ../libcmpisfcc.la

v2test_gc_SOURCES = v2test_gc.c show.c
v2test_gc_LDADD   = ../libcmpisfcc.la

print_types_SOURCES = print-types.c

#@INC_AMINCLUDE@
