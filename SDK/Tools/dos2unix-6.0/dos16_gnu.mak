
# Author: Erwin Waterlander
# Copyright (C) 2009-2011 Erwin Waterlander
# This file is distributed under the same license as the dos2unix package.

include version.mk

prefix=c:/dos16
ENABLE_NLS=

ifdef ENABLE_NLS
LDFLAGS_EXTRA = -lintl -liconv
endif

all:
	wmake -f wccdos16.mak

install:
	$(MAKE) install EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK="cp -f" docsubdir=dos2unix

uninstall:
	$(MAKE) uninstall EXE=.exe prefix=$(prefix) docsubdir=dos2unix

clean:
	$(MAKE) clean EXE=.exe ENABLE_NLS=$(ENABLE_NLS) prefix=$(prefix)

dist:
	$(MAKE) dist-zip EXE=.exe prefix=$(prefix) ZIPOBJ_EXTRA="${ZIPOBJ_EXTRA}" ENABLE_NLS=$(ENABLE_NLS) docsubdir=dos2unix ZIPFILE=d2u$(DOS2UNIX_VERSION_SHORT)b.zip

strip:
	wmake -f wccdos16.mak strip

