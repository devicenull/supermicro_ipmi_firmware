# $Id: Makefile.am,v 1.76 2008/09/05 20:01:56 mchasal Exp $  
#
#  Makefile.am
# 
#   © Copyright IBM Corp. 2005, 2007
# 
#  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
#  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
#  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
# 
#  You can obtain a current copy of the Eclipse Public License from
#  http://www.opensource.org/licenses/eclipse-1.0.php
# 
#  Author:        Viktor Mihajlovski <mihajlov@de.ibm.com>
#  Contributions: Adrian Schuur <schuur@de.ibm.com>
#                 Sven Schuetz <sven@de.ibm.com>
# 
#  Description:
# 
#  Makefile process input for sfcb.
# 
# 

sfcbdocdir=$(datadir)/doc/sfcb-$(VERSION)
sfcbdatadir=$(datadir)/sfcb
sfcbconfdir=$(sysconfdir)/sfcb
sfcbstatedir=$(localstatedir)/lib/sfcb
initdir=$(sysconfdir)/init.d
pamdir=$(sysconfdir)/pam.d

MANFILES=man/genSslCert.1 man/getSchema.1 man/sfcbd.1 man/sfcbmof.1 \
	man/sfcbrepos.1 man/sfcbstage.1 man/sfcbunstage.1 man/sfcbuuid.1 \
	man/wbemcat.1 man/xmltest.1

BUILT_SOURCES=queryParser.c queryLexer.c sqlParser.c sqlLexer.c cimXmlOps.c cimXmlParserProcessed.c $(MANFILES)

AM_YFLAGS=-d

AM_CPPFLAGS=-DSFCB_CONFDIR=\"$(sfcbconfdir)\" \
	-DSFCB_STATEDIR=\"$(sfcbstatedir)\" \
	-DSFCB_LIBDIR=\"$(libdir)\" \
	-DSFCB_BINARY=\"$(sbindir)/sfcbd\" \
	-I$(srcdir)/sfcUtil \
	@SFCB_CPPFLAGS@ \
        @SFCB_CMPI_PLATFORM@ 
AM_CFLAGS=@SFCB_CFLAGS@
AM_LDFLAGS=@SFCB_LDFLAGS@

SUBDIRS=. $(MOFC_DIR)

if INDICATIONS
INDICATION_LIBS = libsfcIndCIMXMLHandler.la
else
INDICATION_LIBS =
endif

if LOCAL_CONNECT_ONLY
HTTP_ADAPTER_LIBS =
HTTP_ADAPTER_LIBS_LINK =
else
HTTP_ADAPTER_LIBS = libsfcHttpAdapter.la
HTTP_ADAPTER_LIBS_LINK = -lsfcHttpAdapter
endif


#
# if enable_local_connect_no_indication is set to "yes"
# then we won't build libsfcCimXmlCodec.so, libsfcHttpAdapter.so
# and libsfcIndCIMXMLHandler..so
#
if LOCAL_CONNECT_NO_INDICATION
CIMXMLCODEC_LIBS =
CIMXMLCODEC_LIBS_LINK =

HTTP_ADAPTER_LIBS =
HTTP_ADAPTER_LIBS_LINK =

INDICATION_LIBS =
else
CIMXMLCODEC_LIBS = libsfcCimXmlCodec.la
CIMXMLCODEC_LIBS_LINK = -lsfcCimXmlCodec

HTTP_ADAPTER_LIBS = libsfcHttpAdapter.la
HTTP_ADAPTER_LIBS_LINK = -lsfcHttpAdapter
endif

if SLP
SLP_LIBS = libslpAgent.la
SLP_LIB = -lslpAgent
SLP_HEADER = cimslpCMPI.h cimslp.h cimslpSLP.h cimslpUtil.h
else
SLP_LIBS =
SLP_LIB =
SLP_HEADER =
endif

if SLP_HOSTNAME_LIB
SLP_HOSTNAME_LIBS = libsfcSlpHostname.la
else
SLP_HOSTNAME_LIBS =
endif

if QUALREP
QUALREP_LIBS = libsfcQualifierProvider.la
QUALREP_FILES = qualifier.c
QUALREP_HEADER = qualifier.h
else
QUALREP_LIBS = 
QUALREP_FILES =
QUALREP_HEADER =
endif

if JDBC
JDBC_FILES = \
    dbpAdapter.c \
    sqlLexer.l \
    sqlParser.y \
    sqlStatement.c \
    avltree.c
else
JDBC_FILES =
endif

if PAM
    PAM_LIBS = libsfcBasicPAMAuthentication.la 
else
    PAM_LIBS =
endif

lib_LTLIBRARIES = \
   libsfcUtil.la \
   libsfcBrokerCore.la \
   libsfcFileRepository.la \
   libsfcInternalProvider.la \
   libsfcInteropProvider.la \
   libsfcInteropServerProvider.la \
   libsfcProfileProvider.la \
   libsfcClassProvider.la \
   libsfcClassProviderGz.la \
   libsfcClassProviderMem.la \
   $(CIMXMLCODEC_LIBS) \
   $(HTTP_ADAPTER_LIBS) \
   libsfcBasicAuthentication.la \
   libsfcCertificateAuthentication.la \
   libsfcObjectImplSwapI32toP32.la \
   libcimcClientSfcbLocal.la \
   $(QUALREP_LIBS) \
   $(SLP_LIBS) \
   $(SLP_HOSTNAME_LIBS) \
   $(INDICATION_LIBS) \
   $(PAM_LIBS) 

if SLP_ALONE
SLP_PROGRAMFILES = cimslp
cimslp_SOURCES = \
	cimslp.c \
	cimslpCMPI.c \
	cimslpSLP.c \
	cimslpUtil.c 

cimslp_LDADD=-lslp -lcmpisfcc -lsfcBrokerCore
else
SLP_PROGRAMFILES = 
endif

sbin_PROGRAMS = \
   sfcbd 

bin_PROGRAMS = \
   sfcbmofpp sfcbdump $(SLP_PROGRAMFILES)

noinst_PROGRAMS = \
   sfcbdumpP32onI32 classSchema2c

libsfcUtil_la_SOURCES = \
	sfcUtil/hashtable.c \
	sfcUtil/utilFactory.c \
	sfcUtil/utilHashtable.c \
	sfcUtil/utilStringBuffer.c \
	sfcUtil/libsfcUtil.Versions
libsfcUtil_la_LDFLAGS = -Wl,--version-script,$(srcdir)/sfcUtil/libsfcUtil.Versions

libsfcBrokerCore_la_SOURCES = \
    args.c \
    array.c \
    brokerEnc.c \
    brokerUpc.c \
    brokerOs.c \
    context.c \
    datetime.c \
    enumeration.c \
    instance.c \
    objectpath.c \
    result.c \
    selectexp.c \
    selectcond.c \
    subcond.c \
    predicate.c \
    string.c \
    value.c \
    support.c \
    providerRegister.c \
    objectImpl.c \
    constClass.c \
    genericlist.c \
    msgqueue.c \
    providerMgr.c \
    providerDrv.c \
    trace.c \
    control.c \
    queryParser.y \
    queryLexer.l \
    queryOperation.c \
    queryStatement.c \
    cimXmlGen.c \
    mrwlock.c \
    mlog.c \
    $(JDBC_FILES) \
    $(QUALREP_FILES)

libsfcBrokerCore_la_CFLAGS = $(AM_CFLAGS) @SFCB_CMPI_OS@ 
libsfcBrokerCore_la_LIBADD=-lsfcUtil
libsfcBrokerCore_la_DEPENDENCIES=libsfcUtil.la

if JDBC
sqlLexer.c: $(srcdir)/sqlLexer.l
	$(LEX) -t $< | sed -e "s/yy/sfcSql/g" > $@

sqlParser.c: $(srcdir)/sqlParser.y
	$(YACC) $(AM_YFLAGS) -p sfcSql -o $@ $< 
endif

queryLexer.c: $(srcdir)/queryLexer.l
	$(LEX) -t $< | sed -e "s/yy/sfcQuery/g" > $@

queryParser.c: $(srcdir)/queryParser.y
	$(YACC) $(AM_YFLAGS) -p sfcQuery -o $@ $< 

cimXmlOps.c: $(srcdir)/cimXmlOps.y
	$(YACC) $(AM_YFLAGS) -p sfcXml -o $@ $< 

cimXmlParserProcessed.c: $(srcdir)/cimXmlParser.c
	sed -e "s/yy/sfcXml/g" $< > $@

libcimcClientSfcbLocal_la_SOURCES = \
   cimcClientSfcbLocal.c
libcimcClientSfcbLocal_la_LIBADD=-lsfcBrokerCore
libcimcClientSfcbLocal_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcFileRepository_la_SOURCES = \
	fileRepository.c
libsfcFileRepository_la_LIBADD=-lsfcBrokerCore	
libsfcFileRepository_la_DEPENDENCIES=libsfcBrokerCore.la

#
# if enable_local_connect_no_indication is not turned on then
# we will build libsfcHttpAdapter
#
if !LOCAL_CONNECT_NO_INDICATION
libsfcHttpAdapter_la_SOURCES = \
   httpAdapter.c \
   httpComm.c
libsfcHttpAdapter_la_LIBADD=-lsfcBrokerCore $(CIMXMLCODEC_LIBS_LINK)
libsfcHttpAdapter_la_DEPENDENCIES=libsfcBrokerCore.la $(CIMXMLCODEC_LIBS)
endif

libsfcSlpHostname_la_SOURCES = \
   sfcSlpHostname.c

libsfcBasicAuthentication_la_SOURCES = \
   sfcBasicAuthentication.c

if PAM
libsfcBasicPAMAuthentication_la_SOURCES = \
   sfcBasicPAMAuthentication.c
libsfcBasicPAMAuthentication_la_LIBADD=-lpam
endif

libsfcCertificateAuthentication_la_SOURCES = \
   sfcCertificateAuthentication.c

libsfcInternalProvider_la_SOURCES = \
   internalProvider.c
libsfcInternalProvider_la_LIBADD=-lsfcBrokerCore -lsfcFileRepository
libsfcInternalProvider_la_DEPENDENCIES=libsfcBrokerCore.la libsfcFileRepository.la

libsfcObjectImplSwapI32toP32_la_SOURCES = \
   objectImplSwapI32toP32.c 
libsfcObjectImplSwapI32toP32_la_LIBADD=-lsfcBrokerCore
libsfcObjectImplSwapI32toP32_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcInteropProvider_la_SOURCES = \
   interopProvider.c 
libsfcInteropProvider_la_LIBADD=-lsfcBrokerCore
libsfcInteropProvider_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcInteropServerProvider_la_SOURCES = \
   interopServerProvider.c 
libsfcInteropServerProvider_la_LIBADD=-lsfcBrokerCore 
libsfcInteropServerProvider_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcIndCIMXMLHandler_la_SOURCES = \
   indCIMXMLHandler.c \
   indCIMXMLExport.c 
libsfcIndCIMXMLHandler_la_LIBADD=-lsfcBrokerCore -lsfcInternalProvider $(CIMXMLCODEC_LIBS_LINK) $(HTTP_ADAPTER_LIBS_LINK)
libsfcIndCIMXMLHandler_la_DEPENDENCIES=libsfcBrokerCore.la libsfcInternalProvider.la \
	$(CIMXMLCODEC_LIBS) $(HTTP_ADAPTER_LIBS)

libsfcProfileProvider_la_SOURCES = \
   profileProvider.c 
libsfcProfileProvider_la_LIBADD=-lsfcBrokerCore
libsfcProfileProvider_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcClassProviderGz_la_SOURCES = \
   classProviderGz.c
libsfcClassProviderGz_la_LIBADD=-lsfcBrokerCore @SFCB_LIBZ@
libsfcClassProviderGz_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcClassProvider_la_SOURCES = \
   classProvider.c
libsfcClassProvider_la_LIBADD=-lsfcBrokerCore
libsfcClassProvider_la_DEPENDENCIES=libsfcBrokerCore.la

libsfcClassProviderMem_la_SOURCES = \
   classProviderMem.c classSchemaMem.c interop_classSchema.c
libsfcClassProviderMem_la_LIBADD=-lsfcBrokerCore
libsfcClassProviderMem_la_DEPENDENCIES=libsfcBrokerCore.la

#
# if enable_local_connect_no_indication is not turned on then
# we will build libsfcCimXmlCodec
#
if !LOCAL_CONNECT_NO_INDICATION
libsfcCimXmlCodec_la_SOURCES = \
   cimXmlOps.y \
   cimXmlParserProcessed.c \
   cimXmlRequest.c 
libsfcCimXmlCodec_la_LIBADD=-lsfcBrokerCore 
libsfcCimXmlCodec_la_DEPENDENCIES=libsfcBrokerCore.la
endif

if SLP
libslpAgent_la_CFLAGS = $(AM_CFLAGS)
libslpAgent_la_SOURCES = \
    cimslp.c \
    cimslpCMPI.c \
    cimslpSLP.c \
    cimslpUtil.c
libslpAgent_la_LIBADD=-lslp -lcmpisfcc
endif

if QUALREP
libsfcQualifierProvider_la_SOURCES = \
   qualifierProvider.c
libsfcQualifierProvider_la_LIBADD=-lsfcBrokerCore -lsfcFileRepository
libsfcQualifierProvider_la_DEPENDENCIES=libsfcBrokerCore.la libsfcFileRepository.la
endif

sfcbd_SOURCES=sfcBroker.c
sfcbd_LDADD=-lsfcBrokerCore $(CIMXMLCODEC_LIBS_LINK) $(HTTP_ADAPTER_LIBS_LINK) $(SLP_LIB)
sfcbd_DEPENDENCIES=libsfcBrokerCore.la $(CIMXMLCODEC_LIBS) $(HTTP_ADAPTER_LIBS)

sfcbmofpp_SOURCES=mofpp.c

sfcbdump_SOURCES=sfcbdump.c

classSchema2c_SOURCES=classSchema2c.c
classSchema2c_LDADD=-lsfcBrokerCore
classSchema2c_DEPENDENCIES=libsfcBrokerCore.la

noinst_HEADERS=array.h   httpComm.h control.h    providerMgr.h \
	constClass.h   msgqueue.h     providerRegister.h \
	cimXmlParser.h    native.h       support.h cimXmlGen.h \
	cimXmlRequest.h  genericlist.h  objectImpl.h   trace.h \
	sfcUtil/hashtable.h      sfcUtil/utilft.h mlog.h \
	cmpidt.h cmpift.h cmpiftx.h cmpimacs.h cmpimacsx.h cmpios.h cmpipl.h \
	fileRepository.h internalProvider.h \
	selectexp.h queryOperation.h \
	sfcVersion.h mrwlock.h sqlStatement.h avltree.h \
        cimcClientSfcbLocal.h $(QUALREP_HEADER) cmpidtx.h classSchemaMem.h \
        objectpath.h instance.h $(SLP_HEADER) sfcbenum.h

man_MANS=$(MANFILES)

EXTRA_DIST=sfcb.cfg.pre.in sfcb.spec sfcbrepos.sh.in sfcbstage.sh.in \
	sfcbunstage.sh.in sfcbuuid.sh.in sfcb.init-redhat.in sfcb.init-suse.in \
	sfcb.init-none.in cimXmlParser.c

dist_sfcbdata_SCRIPTS=genSslCert.sh getSchema.sh

dist_sfcbdata_DATA=default.reg interop.mof indication.mof

nodist_bin_SCRIPTS=sfcbrepos sfcbstage sfcbunstage sfcbuuid

dist_bin_SCRIPTS=wbemcat xmltest

init_SCRIPTS=sfcb

sfcbdoc_DATA=README AUTHORS COPYING

sfcbconf_DATA=sfcb.cfg

if PAM
dist_pam_DATA=pam/sfcb
endif

if DOCS
noinst_DATA=doc/html/index.html

doc/html/index.html: sfcb-dox.conf
	$(DOXYGEN) sfcb-dox.conf
endif

SUFFIXES = .1 .1.pre

.1.pre.1:
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

sfcbrepos: sfcbrepos.sh
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

sfcbstage: sfcbstage.sh
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

sfcbunstage: sfcbunstage.sh
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

sfcbuuid: sfcbuuid.sh
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

sfcb.cfg: sfcb.cfg.pre
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

getSchema.sh: getSchema.sh.pre
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

sfcb: sfcb.$(INIT_STYLE)
	sed -e s?$$\{prefix\}?$(prefix)?g \
	-e s?$$\{exec_prefix\}?$(prefix)?g $< > $@

providerRegister: providerRegister.c
	touch $@

unittest:
	cd test && sh check_all.sh

install-data-local:
	test -d $(DESTDIR)$(sfcbstatedir)/registration/repository || $(mkdir_p) $(DESTDIR)$(sfcbstatedir)/registration/repository
	test -d $(DESTDIR)$(sfcbstatedir)/stage/mofs/root/interop || $(mkdir_p) $(DESTDIR)$(sfcbstatedir)/stage/mofs/root/interop
	test -d $(DESTDIR)$(sfcbstatedir)/stage/regs || $(mkdir_p) $(DESTDIR)$(sfcbstatedir)/stage/regs
	$(INSTALL_DATA) ./default.reg $(DESTDIR)$(sfcbstatedir)/stage
	$(INSTALL_DATA) $(srcdir)/interop.mof $(DESTDIR)$(sfcbstatedir)/stage/mofs/root/interop
if INDICATIONS
	$(INSTALL_DATA) $(srcdir)/indication.mof $(DESTDIR)$(sfcbstatedir)/stage/mofs
endif
if DOCS
	test -d $(DESTDIR)$(sfcbdocdir)/html || $(mkdir_p) $(DESTDIR)$(sfcbdocdir)/html
	$(INSTALL) -m 644 $(srcdir)/doc/html/* $(DESTDIR)$(sfcbdocdir)/html
endif 

uninstall-local:
	rm -f $(DESTDIR)$(sfcbstatedir)/stage/default.reg
	rm -f $(DESTDIR)$(sfcbstatedir)/stage/root/interop/interop.mof
if INDICATIONS
	rm -f $(DESTDIR)$(sfcbstatedir)/stage/mofs/indication.mof
endif
if DOCS
	rm -rf $(DESTDIR)$(sfcbdocdir)/html
endif

clean-local:
	rm -f sfcbrepos sfcbstage sfcbunstage sfcbuuid sfcb.cfg getSchema.sh sfcb \
		sfcb.init-redhat sfcb.init-suse sfcb.init-none sfcb-dox.conf \
		$(MANFILES)
	rm -rf doc/*

dist-hook:
	test -d "$(distdir)" &&	rm -rf `find $(distdir) -type d -name CVS`

install-cimschema: getSchema.sh
	test -d $(DESTDIR)$(sfcbdatadir) || $(mkdir_p) $(DESTDIR)$(sfcbdatadir)
	sh getSchema.sh -f $(DESTDIR)$(sfcbdatadir)

create-sslkeys:
if SSL
	$(sfcbdatadir)/genSslCert.sh $(sfcbconfdir)
endif

footprint:
	@flist='$(sbin_PROGRAMS) $(bin_PROGRAMS) $(lib_LTLIBRARIES) $(dist_sfcbdata_SCRIPTS) $(init_SCRIPTS) $(man1_MANS) $(dist_man1_MANS) $(dist_pam_DATA) $(dist_sfcbdata_DATA) $(sfcbconf_DATA) $(sfcbdoc_DATA) '; \
	rm -f "MANIFEST"; \
	for p in $$flist; do \
		f=`echo $$p|sed 's/$(EXEEXT)$$//'`; \
		echo "$$f" >> MANIFEST; \
	done;\
	./footprint.pl;


postinstall: install-cimschema create-sslkeys
	test -f $(DESTDIR)$(sfcbstatedir)/registration/providerRegister || $(INSTALL_DATA) $(DESTDIR)$(sfcbstatedir)/stage/default.reg $(DESTDIR)$(sfcbstatedir)/registration/providerRegister 
