# $Id: Makefile.am,v 1.17 2008/05/02 00:57:27 buccella Exp $

docdir=$(datadir)/doc/sfcc-$(VERSION)

Libcimcclient_CURRENT=0
Libcimcclient_REVISION=0
Libcimcclient_AGE=0

LibcimcClientXML_CURRENT=0
LibcimcClientXML_REVISION=0
LibcimcClientXML_AGE=0

Libcmpisfcc_CURRENT=1
Libcmpisfcc_REVISION=0
Libcmpisfcc_AGE=0

AM_LFLAGS=-i
AM_YFLAGS=-d

dist_man_MANS = man/man3/CMCIClient.3 man/man3/CMCIClientFT.associatorNames.3 \
	man/man3/CMCIClientFT.associators.3 \
	man/man3/CMCIClientFT.createInstance.3 \
	man/man3/CMCIClientFT.deleteInstance.3 \
	man/man3/CMCIClientFT.enumClasses.3 \
	man/man3/CMCIClientFT.enumClassNames.3 \
	man/man3/CMCIClientFT.enumInstanceNames.3 \
	man/man3/CMCIClientFT.enumInstances.3 \
	man/man3/CMCIClientFT.execQuery.3 \
	man/man3/CMCIClientFT.getClass.3 \
	man/man3/CMCIClientFT.getInstance.3 \
	man/man3/CMCIClientFT.getProperty.3 \
	man/man3/CMCIClientFT.invokeMethod.3 \
	man/man3/CMCIClientFT.referenceNames.3 \
	man/man3/CMCIClientFT.references.3 \
	man/man3/CMCIClientFT.setInstance.3 \
	man/man3/CMCIClientFT.setProperty.3 \
	man/man3/cmciConnect.3 man/man3/CMPIArgs.3 \
	man/man3/CMPIDateTime.3 man/man3/CMPIEnumeration.3 \
	man/man3/CMPIInstance.3 man/man3/CMPIObjectPath.3 \
	man/man3/CMPIStatus.3 man/man3/CMPIString.3 \
	man/man3/CMPIValue.3

dist_doc_DATA = README AUTHORS COPYING

EXTRA_DIST=sfcc.spec

noinst_HEADERS = \
	backend/cimxml/cimXmlParser.h \
	backend/cimxml/genericlist.h \
	backend/cimxml/grammar.h \
	backend/cimxml/parserUtil.h \
	backend/cimxml/sfcUtil/hashtable.h \
	backend/cimxml/nativeCimXml.h \
	backend/cimxml/sfcUtil/utilft.h \
        backend/cimxml/utilStringBuffer.h \
        frontend/sfcc/conn.h 

cimcinclude_HEADERS = \
	cimc/cimc.h \
	cimc/cimcdt.h \
	cimc/cimcft.h 

legacyinclude_HEADERS = \
	frontend/sfcc/native.h \
	frontend/sfcc/cmcidt.h \
	frontend/sfcc/cmcift.h \
	frontend/sfcc/cmci.h \
	frontend/sfcc/cmcimacs.h 

legacyincludedir = $(includedir)/CimClientLib
cimcincludedir = $(includedir)/cimc

lib_LTLIBRARIES= libcimcclient.la libcmpisfcc.la \
	libcimcClientXML.la 

libcimcclient_la_SOURCES = \
                   cimc/cimcclient.c \
		   cimc/libcimcclient.Versions

libcmpisfcc_la_SOURCES = \
                   frontend/sfcc/sfcclient.c \
		   frontend/sfcc/libcmpisfcc.Versions

libcimcClientXML_la_SOURCES = \
                   backend/cimxml/args.c \
                   backend/cimxml/array.c \
                   backend/cimxml/datetime.c \
                   backend/cimxml/enumeration.c \
                   backend/cimxml/instance.c \
                   backend/cimxml/indicationlistener.c \
                   backend/cimxml/constClass.c \
                   backend/cimxml/objectpath.c \
                   backend/cimxml/property.c \
                   backend/cimxml/qualifier.c \
                   backend/cimxml/string.c \
                   backend/cimxml/value.c \
                   backend/cimxml/client.c \
                   backend/cimxml/genericlist.c \
                   backend/cimxml/grammar.c \
                   backend/cimxml/parserUtil.c \
	           backend/cimxml/cimXmlParser.c \
		   backend/cimxml/sfcUtil/hashtable.c \
	   	   backend/cimxml/sfcUtil/utilFactory.c \
		   backend/cimxml/sfcUtil/utilHashtable.c \
	           backend/cimxml/sfcUtil/utilStringBuffer.c \
		   backend/cimxml/libcimcClientXML.Versions

libcimcclient_la_LDFLAGS = \
       -version-info $(Libcimcclient_CURRENT):$(Libcimcclient_REVISION):$(Libcimcclient_AGE) \
       -Wl,--version-script,$(srcdir)/cimc/libcimcclient.Versions
libcimcclient_la_LIBADD = -ldl

libcmpisfcc_la_CPPFLAGS = -I$(srcdir)/cimc -I$(srcdir)/backend/cimxml -I$(srcdir)/frontend/sfcc
libcmpisfcc_la_LIBADD = -lcimcclient
libcmpisfcc_la_DEPENDENCIES = libcimcclient.la
libcmpisfcc_la_LDFLAGS = \
       -version-info $(Libcmpisfcc_CURRENT):$(Libcmpisfcc_REVISION):$(Libcmpisfcc_AGE) \
       -Wl,--version-script,$(srcdir)/frontend/sfcc/libcmpisfcc.Versions

libcimcClientXML_la_CPPFLAGS = -I$(srcdir)/backend/cimxml/sfcUtil -I$(srcdir)/backend/cimxml  -I$(srcdir)/frontend/sfcc -I$(srcdir)/cimc
libcimcClientXML_la_LIBADD = -lcurl
libcimcClientXML_la_LDFLAGS = \
       -version-info $(LibcimcClientXML_CURRENT):$(LibcimcClientXML_REVISION):$(LibcimcClientXML_AGE) \
       -Wl,--version-script,$(srcdir)/backend/cimxml/libcimcClientXML.Versions

SUBDIRS = . TEST

#@INC_AMINCLUDE@
