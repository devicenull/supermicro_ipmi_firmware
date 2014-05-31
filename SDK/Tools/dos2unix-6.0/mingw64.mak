
# Author: Erwin Waterlander
# Copyright (C) 2012 Erwin Waterlander
# This file is distributed under the same license as the dos2unix package.

CC = x86_64-w64-mingw32-gcc
STRIP = x86_64-w64-mingw32-strip
CRT_GLOB_OBJ = C:/mingw64/mingw/lib/CRT_glob.o

prefix=c:/usr/local64
ENABLE_NLS=

ifdef ENABLE_NLS
LDFLAGS_EXTRA = -lintl -liconv
# Using MinGW gettext,iconv
#ZIPOBJ_EXTRA = bin/libintl-8.dll bin/libiconv-2.dll
endif
LDFLAGS_EXTRA += $(CRT_GLOB_OBJ)

all:
	$(MAKE) all EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK="cp -f" CC=$(CC) EO_XNOTATION=1

install:
	$(MAKE) install EXE=.exe ENABLE_NLS=$(ENABLE_NLS) LDFLAGS_EXTRA="$(LDFLAGS_EXTRA)" prefix=$(prefix) LINK="cp -f" CC=$(CC) EO_XNOTATION=1

uninstall:
	$(MAKE) uninstall EXE=.exe prefix=$(prefix)

clean:
	$(MAKE) clean EXE=.exe ENABLE_NLS=$(ENABLE_NLS) prefix=$(prefix)

mostlyclean:
	$(MAKE) mostlyclean EXE=.exe ENABLE_NLS=$(ENABLE_NLS) prefix=$(prefix)

dist:
	$(MAKE) dist-zip EXE=.exe prefix=$(prefix) VERSIONSUFFIX="-win64" ZIPOBJ_EXTRA="${ZIPOBJ_EXTRA}" ENABLE_NLS=$(ENABLE_NLS)

strip:
	$(MAKE) strip LINK="cp -f" EXE=.exe  STRIP=$(STRIP)

