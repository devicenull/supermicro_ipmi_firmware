# $Id: Makefile.am,v 1.3 2006/01/26 14:01:47 mihajlov Exp $

AM_CPPFLAGS=@MOFC_CPPFLAGS@
AM_CFLAGS=@MOFC_CFLAGS@
AM_LDFLAGS=@MOFC_LDFLAGS@

AM_LFLAGS=-i
AM_YFLAGS=-d

BUILT_SOURCES=mofc.h

if TESTSCANNER
bin_PROGRAMS=testmoflex mofc

testmoflex_SOURCES=mofs.l hash.c
testmoflex_CPPFLAGS=-DSCANDEBUG
else
bin_PROGRAMS=mofc
endif

mofc_SOURCES=mofc.y mofs.l mofparse.c hash.h hash.c symtab.h symtab.c mofdecl.h backend.h 
EXTRA_mofc_SOURCES=backend_print.c backend_sfcb.c
mofc_DEPENDENCIES=mofc-@BACKEND_OBJ@
mofc_LDADD=mofc-@BACKEND_OBJ@ @BACKEND_LIBS@ @LEXLIB@
mofc_CPPFLAGS=@BACKEND_INCLUDES@

EXTRA_DIST=runtest.sh tests/test1.mof tests/include/test2.mof \
	tests/include/test3.mof 


if ALTERNATE_MOFCNAME
install-exec-hook:
	mv $(DESTDIR)$(bindir)/mofc $(DESTDIR)$(bindir)/$(MOFCNAME)

uninstall-hook:
	rm $(DESTDIR)$(bindir)/$(MOFCNAME)
endif
