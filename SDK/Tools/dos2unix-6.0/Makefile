# Author: Erwin Waterlander
#
#   Copyright (C) 2009-2012 Erwin Waterlander
#   All rights reserved.
# 
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
#   1. Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#   2. Redistributions in binary form must reproduce the above copyright
#      notice in the documentation and/or other materials provided with
#      the distribution.
# 
#   THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY
#   EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#   PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE
#   FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
#   OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
#   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
#   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
#   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#   Description
#
#	This is a GNU Makefile that uses GNU compilers, linkers and cpp. The
#	platform specific issues are determined by the various OS teets that
#	rely on the uname(1) command and directory locations.
#
#	Set additional flags for the build with variables CFLAGS_USER,
#	DEFS_USER and LDFLAGS_USER.

include version.mk

CC		= gcc
CPP		= cpp
CPP_FLAGS_POD	= ALL
STRIP		= strip

PACKAGE		= dos2unix
UNIX2DOS	= unix2dos
MAC2UNIX	= mac2unix
UNIX2MAC	= unix2mac

# Native Language Support (NLS)
ENABLE_NLS	= 1
# Large File Support (LFS)
LFS             = 1
DEBUG = 0
UCS = 1

EXE=

BIN		= $(PACKAGE)$(EXE)
UNIX2DOS_BIN	= $(UNIX2DOS)$(EXE)
MAC2UNIX_BIN	= $(MAC2UNIX)$(EXE)
UNIX2MAC_BIN	= $(UNIX2MAC)$(EXE)

# DJGPP support linking of .EXEs via 'stubify'.
# See djgpp.mak and http://www.delorie.com/djgpp/v2faq/faq22_5.html

LINK		= ln -sf
LINK_MAN	= $(LINK)

prefix		= /usr
exec_prefix	= $(prefix)
bindir		= $(exec_prefix)/bin
datarootdir	= $(prefix)/share
datadir		= $(datarootdir)

docsubdir	= $(PACKAGE)-$(DOS2UNIX_VERSION)
docdir		= $(datarootdir)/doc/$(docsubdir)
localedir	= $(datarootdir)/locale
mandir		= $(datarootdir)/man
man1dir		= $(mandir)/man1
manext		= .1
man1ext		= .1

ifdef ENABLE_NLS
	POT		= po/$(PACKAGE).pot
	POFILES		= $(wildcard po/??.po)
	MOFILES		= $(patsubst %.po,%.mo,$(POFILES))
	EOX_POFILES	= po/eo-x.po
	NLSSUFFIX       = -nls
endif

HTMLEXT = htm
DOCFILES	= $(PACKAGE).txt $(PACKAGE).$(HTMLEXT)
INSTALL_OBJS_DOC = README.txt NEWS.txt ChangeLog.txt COPYING.txt TODO.txt BUGS.txt $(DOCFILES)

#PODFILES	= man/man1/dos2unix.pod $(wildcard man/*/man1/dos2unix.pod)
PODFILES	= $(wildcard man/*/man1/dos2unix.pod)
MANFILES	= $(patsubst %.pod,%.1,$(PODFILES))

# On some systems (e.g. FreeBSD 4.10) GNU install is installed as `ginstall'.
INSTALL		= install

# On some systems (e.g. GNU Win32) GNU mkdir is installed as `gmkdir'.
MKDIR           = mkdir

ifdef ENABLE_NLS
	DOS2UNIX_NLSDEFS = -DENABLE_NLS -DLOCALEDIR=\"$(localedir)\" -DPACKAGE=\"$(PACKAGE)\"
endif

VERSIONSUFFIX	= -bin

# ......................................................... OS flags ...

OS =

ifndef OS
ifneq (, $(wildcard /boot/vmlinuz*))
	OS = linux
endif
endif

ifndef OS
ifeq ($(findstring CYGWIN,$(shell uname)),CYGWIN)
	OS = cygwin
endif
endif

ifeq (cygwin,$(OS))
ifdef ENABLE_NLS
	LDFLAGS_EXTRA = -lintl -liconv -Wl,--enable-auto-import
endif
	EXE = .exe
	# allow non-cygwin clients which do not understand cygwin
	# symbolic links to launch applications...
	LINK = ln -f
	# but use symbolic links for man pages, since man client
	# IS a cygwin app and DOES understand symlinks.
	LINK_MAN = ln -fs
	# Cygwin packaging standard avoids version numbers on
	# documentation directories.
	docsubdir	= $(PACKAGE)
	VERSIONSUFFIX	= -cygwin
endif

ifndef OS
ifeq ($(findstring MSYS,$(shell uname)),MSYS)
	OS = msys
	EXE = .exe
	VERSIONSUFFIX	= -msys
	EO_XNOTATION=1
	UCS =
ifdef ENABLE_NLS
	LDFLAGS_EXTRA = -lintl -liconv
endif
endif
endif

ifndef OS
ifeq ($(findstring MINGW32,$(shell uname)),MINGW32)
	OS = mingw32
	prefix=c:/usr/local
	EXE = .exe
	VERSIONSUFFIX	= -win32
	LINK = cp -f
	EO_XNOTATION=1
ifdef ENABLE_NLS
	LDFLAGS_EXTRA = -lintl -liconv
	ZIPOBJ_EXTRA = bin/libintl-8.dll bin/libiconv-2.dll
endif
endif
endif

ifndef OS
ifeq ($(shell uname),MS-DOS)
	OS = msdos
	prefix=c:/djgpp
	EXE = .exe
	VERSIONSUFFIX	= -dos32
	LINK_MAN = cp -f
	docsubdir = dos2unix
	EO_XNOTATION=1
	UCS =
	ZIPOBJ_EXTRA = bin/cwsdpmi.exe
ifdef ENABLE_NLS
	LDFLAGS_EXTRA = -lintl -liconv
endif
endif
endif

ifndef OS
ifeq (FreeBSD, $(shell uname -s))
	OS = freebsd
ifdef ENABLE_NLS
	CFLAGS_OS     = -I/usr/local/include
	LDFLAGS_EXTRA = -lintl -L/usr/local/lib
endif
endif
endif

ifeq (Darwin, $(shell uname -s))
	OS = Darwin
ifdef ENABLE_NLS
	CFLAGS_OS     = -I/usr/local/include
	LDFLAGS_EXTRA = -lintl -L/usr/local/lib
endif
endif


ifndef OS
ifneq (, $(wildcard /opt/csw))
	OS = sun
endif
endif

ifeq (sun,$(OS))
	# Running under SunOS/Solaris
	LDFLAGS_EXTRA = -lintl
endif

ifndef OS
ifeq (HP-UX, $(shell uname -s))
	OS = hpux
endif
endif

ifeq (hpux,$(OS))
	# Running under HP-UX
	EXTRA_DEFS += -Dhpux -D_HPUX_SOURCE
endif

ifndef OS
	OS = unknown
endif

# ............................................................ flags ...

# For systems that don't support Unicode or Latin-3, select
# Esperanto in X-notation format: EO_XNOTATION=1

ifdef EO_XNOTATION
EO_NOTATION = -x
endif

CFLAGS_USER	=
CFLAGS		= -O2 -Wall $(RPM_OPT_FLAGS) $(CPPFLAGS) $(CFLAGS_USER)

EXTRA_CFLAGS	= -DVER_REVISION=\"$(DOS2UNIX_VERSION)\" \
		  -DVER_DATE=\"$(DOS2UNIX_DATE)\" \
		  -DVER_AUTHOR=\"$(DOS2UNIX_AUTHOR)\" \
		  -DDEBUG=$(DEBUG) \
		  $(CFLAGS_OS)

ifeq ($(DEBUG), 1)
	EXTRA_CFLAGS += -g
endif

ifdef STATIC
	EXTRA_CFLAGS += -static
endif

ifdef UCS
	EXTRA_CFLAGS += -DD2U_UNICODE
endif

ifdef LFS
	EXTRA_CFLAGS += -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
endif

LDFLAGS_USER	=
LDFLAGS = $(RPM_OPT_FLAGS) $(LDFLAGS_EXTRA) $(LDFLAGS_USER)

DEFS_USER	=
DEFS		= $(EXTRA_DEFS) $(DEFS_USER)

# .......................................................... targets ...

all: $(BIN) $(MAC2UNIX_BIN) $(UNIX2DOS_BIN) $(UNIX2MAC_BIN) $(DOCFILES) $(MOFILES) $(EOX_POFILES) $(MANFILES) man/man1/dos2unix.1

common.o : common.c common.h
	$(CC) $(DEFS) $(EXTRA_CFLAGS) $(DOS2UNIX_NLSDEFS) $(CFLAGS) -c $< -o $@

querycp.o : querycp.c querycp.h
	$(CC) $(DEFS) $(EXTRA_CFLAGS) $(DOS2UNIX_NLSDEFS) $(CFLAGS) -c $< -o $@

dos2unix.o : dos2unix.c dos2unix.h querycp.h common.h
	$(CC) $(DEFS) $(EXTRA_CFLAGS) $(DOS2UNIX_NLSDEFS) $(CFLAGS) -c $< -o $@

unix2dos.o : unix2dos.c unix2dos.h querycp.h common.h
	$(CC) $(DEFS) $(EXTRA_CFLAGS) $(DOS2UNIX_NLSDEFS) $(CFLAGS) -c $< -o $@

$(BIN): dos2unix.o querycp.o common.o
	$(CC) $+ $(LDFLAGS) -o $@

$(UNIX2DOS_BIN): unix2dos.o querycp.o common.o
	$(CC) $+ $(LDFLAGS) -o $@

$(MAC2UNIX_BIN) : $(BIN)
	$(LINK) $< $@

%.1 : %.pod
	$(MAKE) -C man/man1

$(UNIX2MAC_BIN) : $(UNIX2DOS_BIN)
	$(LINK) $< $@

mofiles: $(MOFILES)

html: $(PACKAGE).$(HTMLEXT)

txt: $(PACKAGE).txt

ps: $(PACKAGE).ps

pdf: $(PACKAGE).pdf

doc: $(DOCFILES) $(MANFILES) man/man1/dos2unix.1

tags: $(POT)

merge: $(POFILES) $(EOX_POFILES)

po/%.po : $(POT)
	msgmerge -U $@ $(POT) --backup=numbered
	# change timestamp in case .po file was not updated.
	touch $@

%.mo : %.po
	msgfmt -c $< -o $@

po/eo.mo : po/eo$(EO_NOTATION).po
	msgfmt -c $< -o $@

$(POT) : dos2unix.c unix2dos.c common.c
	xgettext -C --keyword=_ $+ -o $(POT)

%.txt : man/man1/%.pod
	LC_CTYPE=C pod2text $< > $@

%.ps : man/man1/%.1
	groff -man $< -T ps > $@

%.pdf: %.ps
	ps2pdf $< $@

%.$(HTMLEXT) : man/man1/%.pod
	pod2html --title="$(PACKAGE) $(DOS2UNIX_VERSION) - DOS/MAC to UNIX and vice versa text file format converter" $< > $@

install: all
	$(MKDIR) -p -m 755 $(DESTDIR)$(bindir)
	$(INSTALL)  -m 755 $(BIN) $(DESTDIR)$(bindir)
	$(INSTALL)  -m 755 $(UNIX2DOS_BIN) $(DESTDIR)$(bindir)
ifeq ($(LINK),cp -f)
	$(INSTALL)  -m 755 $(MAC2UNIX_BIN) $(DESTDIR)$(bindir)
	$(INSTALL)  -m 755 $(UNIX2MAC_BIN) $(DESTDIR)$(bindir)
else
	cd $(DESTDIR)$(bindir); $(LINK) $(BIN) $(MAC2UNIX_BIN)
	cd $(DESTDIR)$(bindir); $(LINK) $(UNIX2DOS_BIN) $(UNIX2MAC_BIN)
endif
	$(MKDIR) -p -m 755 $(DESTDIR)$(man1dir)
	$(INSTALL)  -m 644 man/man1/$(PACKAGE).1 $(DESTDIR)$(man1dir)
ifeq ($(LINK_MAN),cp -f)
	$(INSTALL)  -m 644 man/man1/$(PACKAGE).1 $(DESTDIR)$(man1dir)/$(MAC2UNIX).1
	$(INSTALL)  -m 644 man/man1/$(PACKAGE).1 $(DESTDIR)$(man1dir)/$(UNIX2DOS).1
	$(INSTALL)  -m 644 man/man1/$(PACKAGE).1 $(DESTDIR)$(man1dir)/$(UNIX2MAC).1
else
	cd $(DESTDIR)$(man1dir); $(LINK_MAN) $(PACKAGE).1 $(MAC2UNIX).1
	cd $(DESTDIR)$(man1dir); $(LINK_MAN) $(PACKAGE).1 $(UNIX2DOS).1
	cd $(DESTDIR)$(man1dir); $(LINK_MAN) $(PACKAGE).1 $(UNIX2MAC).1
endif
	$(foreach manfile, $(MANFILES), $(MKDIR) -p -m 755 $(DESTDIR)$(datarootdir)/$(dir $(manfile)) ;)
	$(foreach manfile, $(MANFILES), $(INSTALL) -m 644 $(manfile) $(DESTDIR)$(datarootdir)/$(dir $(manfile)) ;)
	$(foreach manfile, $(MANFILES), cd $(DESTDIR)$(datarootdir)/$(dir $(manfile)) ; $(LINK_MAN) $(PACKAGE).1 $(MAC2UNIX).1 ;)
	$(foreach manfile, $(MANFILES), cd $(DESTDIR)$(datarootdir)/$(dir $(manfile)) ; $(LINK_MAN) $(PACKAGE).1 $(UNIX2DOS).1 ;)
	$(foreach manfile, $(MANFILES), cd $(DESTDIR)$(datarootdir)/$(dir $(manfile)) ; $(LINK_MAN) $(PACKAGE).1 $(UNIX2MAC).1 ;)
ifdef ENABLE_NLS
	@echo "-- install-mo"
	$(foreach mofile, $(MOFILES), $(MKDIR) -p -m 755 $(DESTDIR)$(localedir)/$(basename $(notdir $(mofile)))/LC_MESSAGES ;)
	$(foreach mofile, $(MOFILES), $(INSTALL) -m 644 $(mofile) $(DESTDIR)$(localedir)/$(basename $(notdir $(mofile)))/LC_MESSAGES/$(PACKAGE).mo ;)
endif
	@echo "-- install-doc"
	$(MKDIR) -p -m 755 $(DESTDIR)$(docdir)
	$(INSTALL) -m 644 $(INSTALL_OBJS_DOC) $(wildcard $(PACKAGE).ps) $(wildcard $(PACKAGE).pdf) $(DESTDIR)$(docdir)

uninstall:
	@echo "-- target: uninstall"
	-rm -f $(DESTDIR)$(bindir)/$(BIN)
	-rm -f $(DESTDIR)$(bindir)/$(MAC2UNIX_BIN)
	-rm -f $(DESTDIR)$(bindir)/$(UNIX2DOS_BIN)
	-rm -f $(DESTDIR)$(bindir)/$(UNIX2MAC_BIN)
ifdef ENABLE_NLS
	$(foreach mofile, $(MOFILES), rm -f $(DESTDIR)$(localedir)/$(basename $(notdir $(mofile)))/LC_MESSAGES/$(PACKAGE).mo ;)
endif
	-rm -f $(DESTDIR)$(mandir)/man1/$(PACKAGE).1
	-rm -f $(DESTDIR)$(mandir)/man1/$(MAC2UNIX).1
	-rm -f $(DESTDIR)$(mandir)/man1/$(UNIX2DOS).1
	-rm -f $(DESTDIR)$(mandir)/man1/$(UNIX2MAC).1
	$(foreach manfile, $(MANFILES), rm -f $(DESTDIR)$(datarootdir)/$(manfile) ;)
	-rm -rf $(DESTDIR)$(docdir)

mostlyclean:
	rm -f *.o
	rm -f $(BIN) $(UNIX2DOS_BIN) $(MAC2UNIX_BIN) $(UNIX2MAC_BIN)
	rm -f *.bak *~
	rm -f *.tmp
	rm -f man/man1/*.bak man/man1/*~
	rm -f man/*/man1/*.bak man/*/man1/*~
	rm -f po/*.bak po/*~
	rm -f po/*.mo

clean: mostlyclean
	rm -f $(DOCFILES) $(PACKAGE).ps $(PACKAGE).pdf
	rm -f man/man1/*.1
	rm -f man/*/man1/*.1

distclean: clean

maintainer-clean: distclean

realclean: maintainer-clean


ZIPOBJ	= bin/$(BIN) \
	  bin/$(MAC2UNIX_BIN) \
	  bin/$(UNIX2DOS_BIN) \
	  bin/$(UNIX2MAC_BIN) \
	  share/man/man1/$(PACKAGE).1 \
	  share/man/man1/$(MAC2UNIX).1 \
	  share/man/man1/$(UNIX2DOS).1 \
	  share/man/man1/$(UNIX2MAC).1 \
	  share/man/*/man1/$(PACKAGE).1 \
	  share/man/*/man1/$(MAC2UNIX).1 \
	  share/man/*/man1/$(UNIX2DOS).1 \
	  share/man/*/man1/$(UNIX2MAC).1 \
	  share/doc/$(docsubdir) \
	  $(ZIPOBJ_EXTRA)

ifdef ENABLE_NLS
ZIPOBJ += share/locale/*/LC_MESSAGES/$(PACKAGE).mo
endif

ZIPFILE = $(PACKAGE)-$(DOS2UNIX_VERSION)$(VERSIONSUFFIX)$(NLSSUFFIX).zip
TGZFILE = $(PACKAGE)-$(DOS2UNIX_VERSION)$(VERSIONSUFFIX)$(NLSSUFFIX).tar.gz
TBZFILE = $(PACKAGE)-$(DOS2UNIX_VERSION)$(VERSIONSUFFIX)$(NLSSUFFIX).tar.bz2

dist-zip:
	rm -f $(prefix)/$(ZIPFILE)
	cd $(prefix) ; unix2dos share/doc/$(docsubdir)/*.txt share/man/man1/$(PACKAGE).1 share/man/man1/$(MAC2UNIX).1 share/man/man1/$(UNIX2DOS).1 share/man/man1/$(UNIX2MAC).1
	cd $(prefix) ; unix2dos share/doc/$(docsubdir)/*.$(HTMLEXT)
	cd $(prefix) ; unix2dos share/man/*/man1/$(PACKAGE).1 share/man/*/man1/$(MAC2UNIX).1 share/man/*/man1/$(UNIX2DOS).1 share/man/*/man1/$(UNIX2MAC).1
	cd $(prefix) ; zip -r $(ZIPFILE) $(ZIPOBJ)
	mv -f $(prefix)/$(ZIPFILE) ..

dist-tgz:
	cd $(prefix) ; dos2unix share/doc/$(docsubdir)/*.txt share/man/man1/$(PACKAGE).1 share/man/man1/$(MAC2UNIX).1 share/man/man1/$(UNIX2DOS).1 share/man/man1/$(UNIX2MAC).1
	cd $(prefix) ; dos2unix share/man/*/man1/$(PACKAGE).1 share/man/*/man1/$(MAC2UNIX).1 share/man/*/man1/$(UNIX2DOS).1 share/man/*/man1/$(UNIX2MAC).1
	cd $(prefix) ; tar cvzf $(TGZFILE) $(ZIPOBJ)
	mv $(prefix)/$(TGZFILE) ..

dist-tbz:
	cd $(prefix) ; dos2unix share/doc/$(docsubdir)/*.txt share/man/man1/$(PACKAGE).1 share/man/man1/$(MAC2UNIX).1 share/man/man1/$(UNIX2DOS).1 share/man/man1/$(UNIX2MAC).1
	cd $(prefix) ; dos2unix share/man/*/man1/$(PACKAGE).1 share/man/*/man1/$(MAC2UNIX).1 share/man/*/man1/$(UNIX2DOS).1 share/man/*/man1/$(UNIX2MAC).1
	cd $(prefix) ; tar cvjf $(TBZFILE) $(ZIPOBJ)
	mv $(prefix)/$(TBZFILE) ..

dist: dist-tgz

strip:
	$(STRIP) $(BIN)
	$(STRIP) $(UNIX2DOS_BIN)
ifeq ($(LINK),cp -f)
	$(STRIP) $(MAC2UNIX_BIN)
	$(STRIP) $(UNIX2MAC_BIN)
endif

# End of file
