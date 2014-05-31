
# Author: Erwin Waterlander
# Copyright (C) 2009-2012 Erwin Waterlander
# This file is distributed under the same license as the dos2unix package.

prefix=c:/djgpp
#ENABLE_NLS=1
ENABLE_NLS=
VERSIONSUFFIX=-dos32

ifdef ENABLE_NLS
LDFLAGS_EXTRA = -lintl -liconv
endif
ZIPOBJ_EXTRA = bin/cwsdpmi.exe

all:
	$(MAKE) all EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK_MAN="cp -f" docsubdir=dos2unix EO_XNOTATION=1 UCS=

install:
	$(MAKE) install EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK_MAN="cp -f" docsubdir=dos2unix EO_XNOTATION=1 UCS=

uninstall:
	$(MAKE) uninstall EXE=.exe prefix=$(prefix) docsubdir=dos2unix

clean:
	$(MAKE) clean EXE=.exe ENABLE_NLS=$(ENABLE_NLS) prefix=$(prefix)

mostlyclean:
	$(MAKE) mostlyclean EXE=.exe ENABLE_NLS=$(ENABLE_NLS) prefix=$(prefix)

dist:
	$(MAKE) dist-zip EXE=.exe prefix=$(prefix) VERSIONSUFFIX="$(VERSIONSUFFIX)" ZIPOBJ_EXTRA="${ZIPOBJ_EXTRA}" ENABLE_NLS=$(ENABLE_NLS) docsubdir=dos2unix

strip:
	$(MAKE) strip LINK_MAN="cp -f" EXE=.exe

