# $OpenLDAP: pkg/ldap/build/top.mk,v 1.103.2.5 2008/02/11 23:26:38 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2008 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
##---------------------------------------------------------------------------
#
# Top-level Makefile template
#

PACKAGE= OpenLDAP
VERSION= 2.4.11
RELEASEDATE= 2008/07/16


SHELL = /bin/sh

top_builddir = ../../

srcdir = .
top_srcdir = ../..

prefix = /usr/local
exec_prefix = ${prefix}
ldap_subdir = /openldap

bindir = ${exec_prefix}/bin
datadir = ${prefix}/share$(ldap_subdir)
includedir = ${prefix}/include
infodir = ${prefix}/info
libdir = ${exec_prefix}/lib
libexecdir = ${exec_prefix}/libexec
localstatedir = ${prefix}/var
mandir = ${prefix}/man
moduledir = ${exec_prefix}/libexec$(ldap_subdir)
sbindir = ${exec_prefix}/sbin
sharedstatedir = ${prefix}/com
sysconfdir = ${prefix}/etc$(ldap_subdir)
schemadir = $(sysconfdir)/schema

PLAT = UNIX
EXEEXT = 
OBJEXT = o

BUILD_LIBS_DYNAMIC = static

SHTOOL = $(top_srcdir)/build/shtool

INSTALL = $(SHTOOL) install -c
INSTALL_PROGRAM = $(INSTALL)
INSTALL_DATA = $(INSTALL) -m 644
INSTALL_SCRIPT = $(INSTALL)

STRIP = -s

LINT = lint
5LINT = 5lint

MKDEP = $(top_srcdir)/build/mkdep $(MKDEPFLAG) \
	-d "$(srcdir)" -c "$(MKDEP_CC)" -m "$(MKDEP_CFLAGS)"
MKDEP_CC	= /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-gcc
MKDEP_CFLAGS = -M

MKVERSION = $(top_srcdir)/build/mkversion -v "$(VERSION)"

LIBTOOL = $(SHELL) $(top_builddir)/libtool
LIBRELEASE = 2.4
LIBVERSION = 3:0:1
LTVERSION = -release $(LIBRELEASE) -version-info $(LIBVERSION)

# libtool --only flag for libraries: platform specific
NT_LTONLY_LIB = # --only-$(BUILD_LIBS_DYNAMIC)
LTONLY_LIB = $(UNIX_LTONLY_LIB)

# libtool --only flag for modules: depends on linkage of module
# The BUILD_MOD macro is defined in each backend Makefile.in file
LTONLY_yes = --tag=disable-shared
LTONLY_mod = --tag=disable-static
LTONLY_MOD = $(LTONLY_$(BUILD_MOD))

# platform-specific libtool flags
NT_LTFLAGS_LIB = -no-undefined -avoid-version -rpath $(libdir)
NT_LTFLAGS_MOD = -no-undefined -avoid-version -rpath $(moduledir)
UNIX_LTFLAGS_LIB = $(LTVERSION) -rpath $(libdir)
UNIX_LTFLAGS_MOD = $(LTVERSION) -rpath $(moduledir)

# libtool flags
LTFLAGS     = $(UNIX_LTFLAGS)
LTFLAGS_LIB = $(UNIX_LTFLAGS_LIB)
LTFLAGS_MOD = $(UNIX_LTFLAGS_MOD)

# LIB_DEFS defined in liblber and libldap Makefile.in files.
# MOD_DEFS defined in backend Makefile.in files.

# platform-specific LINK_LIBS defined in various Makefile.in files.
# LINK_LIBS referenced in library and module link commands.
LINK_LIBS = $(MOD_LIBS) $(UNIX_LINK_LIBS)

LTSTATIC = -static

LTLINK   = $(LIBTOOL) --mode=link \
	$(CC) $(LTSTATIC) $(LT_CFLAGS) $(LDFLAGS) $(LTFLAGS)

LTCOMPILE_LIB = $(LIBTOOL) $(LTONLY_LIB) --mode=compile \
	$(CC) $(LT_CFLAGS) $(LT_CPPFLAGS) $(LIB_DEFS) -c

LTLINK_LIB = $(LIBTOOL) $(LTONLY_LIB) --mode=link \
	$(CC) $(LT_CFLAGS) $(LDFLAGS) $(LTFLAGS_LIB)

LTCOMPILE_MOD = $(LIBTOOL) $(LTONLY_MOD) --mode=compile \
	$(CC) $(LT_CFLAGS) $(LT_CPPFLAGS) $(MOD_DEFS) -c

LTLINK_MOD = $(LIBTOOL) $(LTONLY_MOD) --mode=link \
	$(CC) $(LT_CFLAGS) $(LDFLAGS) $(LTFLAGS_MOD)

LTINSTALL = $(LIBTOOL) --mode=install $(INSTALL) 
LTFINISH = $(LIBTOOL) --mode=finish

# Misc UNIX commands used in build environment
AR = /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-ar
BASENAME = basename
CAT = cat
CHMOD = chmod
DATE = date
ECHO = $(SHTOOL) echo
HOSTNAME = $(SHTOOL) echo -e "%h%d"
LN = $(SHTOOL) mkln
LN_H = $(LN)
LN_S = $(LN) -s
MAKEINFO = ${SHELL} /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/OpenLDAP/openldap/build/missing --run makeinfo
MKDIR = $(SHTOOL) mkdir -p
MV = $(SHTOOL) move
PWD = pwd
RANLIB = /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-ranlib
RM = rm -f
SED = sed
SUBST = $(SHTOOL) subst

# For manual pages
# MANCOMPRESS=@MANCOMPRESS@
# MANCOMPRESSSUFFIX=@MANCOMPRESSSUFFIX@
MANCOMPRESS=$(CAT)
MANCOMPRESSSUFFIX=

SOELIM=soelim

INCLUDEDIR= $(top_srcdir)/include
LDAP_INCPATH= -I$(LDAP_INCDIR) -I$(INCLUDEDIR)
LDAP_LIBDIR= $(top_builddir)/libraries

LUTIL_LIBS = 
LTHREAD_LIBS = 

BDB_LIBS = 

LDAP_LIBLBER_LA = $(LDAP_LIBDIR)/liblber/liblber.la
LDAP_LIBLDAP_LA = $(LDAP_LIBDIR)/libldap/libldap.la
LDAP_LIBLDAP_R_LA = $(LDAP_LIBDIR)/libldap_r/libldap_r.la

LDAP_LIBREWRITE_A = $(LDAP_LIBDIR)/librewrite/librewrite.a
LDAP_LIBLUNICODE_A = $(LDAP_LIBDIR)/liblunicode/liblunicode.a
LDAP_LIBLUTIL_A = $(LDAP_LIBDIR)/liblutil/liblutil.a

LDAP_L = $(LDAP_LIBLUTIL_A) \
	$(LDAP_LIBLDAP_LA) $(LDAP_LIBLBER_LA)
SLAPD_L = $(LDAP_LIBLUNICODE_A) $(LDAP_LIBREWRITE_A) \
	$(LDAP_LIBLUTIL_A) $(LDAP_LIBLDAP_R_LA) $(LDAP_LIBLBER_LA)

WRAP_LIBS = 
# AutoConfig generated 
AC_CC	= /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-gcc
AC_CFLAGS = -I/root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/OpenSSL/openssl/local/include
AC_DEFS = -I/root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/OpenSSL/openssl/local/include  # -DHAVE_CONFIG_H
AC_LDFLAGS = -L/root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/OpenSSL/openssl/local/lib -lcrypto -lssl
AC_LIBS = -lresolv 

KRB4_LIBS = 
KRB5_LIBS = 
KRB_LIBS =  
SASL_LIBS = 
TLS_LIBS = -lssl -lcrypto
AUTH_LIBS = 
SECURITY_LIBS = $(SASL_LIBS) $(KRB_LIBS) $(TLS_LIBS) $(AUTH_LIBS)
ICU_LIBS = 

MODULES_CPPFLAGS = 
MODULES_LDFLAGS = 
MODULES_LIBS = 
SLAPD_PERL_LDFLAGS = 

SLAPD_SQL_LDFLAGS = 
SLAPD_SQL_INCLUDES = 
SLAPD_SQL_LIBS = 

SLAPD_LIBS =       $(ICU_LIBS)

# Our Defaults
CC = $(AC_CC)
DEFS = $(LDAP_INCPATH) $(XINCPATH) $(XDEFS) $(AC_DEFS) $(DEFINES)
CFLAGS = $(AC_CFLAGS) $(DEFS)
LDFLAGS = $(LDAP_LIBPATH) $(AC_LDFLAGS) $(XLDFLAGS)
LIBS = $(XLIBS) $(XXLIBS) $(AC_LIBS) $(XXXLIBS)

LT_CFLAGS = $(AC_CFLAGS)
LT_CPPFLAGS = $(DEFS)

all:		all-common all-local FORCE
install:	install-common install-local FORCE
clean:		clean-common clean-local FORCE
veryclean:	veryclean-common veryclean-local FORCE
depend:		depend-common depend-local FORCE

# empty common rules
all-common:
install-common:
clean-common:
veryclean-common:	clean-common FORCE
depend-common:
lint-common:
lint5-common:

# empty local rules
all-local:
install-local:
clean-local:
veryclean-local:	clean-local FORCE
depend-local:
lint-local:
lint5-local:

veryclean: FORCE
	$(RM) Makefile
	$(RM) -r .libs

Makefile: Makefile.in $(top_srcdir)/build/top.mk

pathtest:
	$(SHTOOL) --version

# empty rule for forcing rules
FORCE:

##---------------------------------------------------------------------------

## Makefile.in for slapd
# $OpenLDAP: pkg/ldap/servers/slapd/Makefile.in,v 1.186.2.6 2008/02/11 23:26:43 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2008 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
## <http://www.OpenLDAP.org/license.html>.

SLAPTOOLS=slapadd slapcat slapdn slapindex slappasswd slaptest slapauth slapacl
PROGRAMS=slapd $(SLAPTOOLS)
XPROGRAMS=sslapd libbackends.a .backend liboverlays.a
XSRCS=version.c

SUBDIRS=back-* shell-backends slapi overlays

NT_SRCS = nt_svc.c
NT_OBJS = nt_svc.o ../../libraries/liblutil/slapdmsg.res

SRCS	= main.c globals.c bconfig.c config.c daemon.c \
		connection.c search.c filter.c add.c cr.c \
		attr.c entry.c backend.c result.c operation.c \
		dn.c compare.c modify.c delete.c modrdn.c ch_malloc.c \
		value.c ava.c bind.c unbind.c abandon.c filterentry.c \
		phonetic.c acl.c str2filter.c aclparse.c init.c user.c \
		lock.c controls.c extended.c passwd.c \
		schema.c schema_check.c schema_init.c schema_prep.c \
		schemaparse.c ad.c at.c mr.c syntax.c oc.c saslauthz.c \
		oidm.c starttls.c index.c sets.c referral.c root_dse.c \
		sasl.c module.c mra.c mods.c sl_malloc.c zn_malloc.c limits.c \
		operational.c matchedValues.c cancel.c syncrepl.c \
		backglue.c backover.c ctxcsn.c ldapsync.c frontend.c \
		slapadd.c slapcat.c slapcommon.c slapdn.c slapindex.c \
		slappasswd.c slaptest.c slapauth.c slapacl.c component.c \
		aci.c alock.c txn.c \
		$(UNIX_SRCS)

OBJS	= main.o globals.o bconfig.o config.o daemon.o \
		connection.o search.o filter.o add.o cr.o \
		attr.o entry.o backend.o backends.o result.o operation.o \
		dn.o compare.o modify.o delete.o modrdn.o ch_malloc.o \
		value.o ava.o bind.o unbind.o abandon.o filterentry.o \
		phonetic.o acl.o str2filter.o aclparse.o init.o user.o \
		lock.o controls.o extended.o passwd.o \
		schema.o schema_check.o schema_init.o schema_prep.o \
		schemaparse.o ad.o at.o mr.o syntax.o oc.o saslauthz.o \
		oidm.o starttls.o index.o sets.o referral.o root_dse.o \
		sasl.o module.o mra.o mods.o sl_malloc.o zn_malloc.o limits.o \
		operational.o matchedValues.o cancel.o syncrepl.o \
		backglue.o backover.o ctxcsn.o ldapsync.o frontend.o \
		slapadd.o slapcat.o slapcommon.o slapdn.o slapindex.o \
		slappasswd.o slaptest.o slapauth.o slapacl.o component.o \
		aci.o alock.o txn.o \
		$(UNIX_OBJS)

LDAP_INCDIR= ../../include -I$(srcdir) -I$(srcdir)/slapi -I.
LDAP_LIBDIR= ../../libraries

SLAP_DIR=
SLAPD_STATIC_DEPENDS= libbackends.a liboverlays.a
SLAPD_STATIC_BACKENDS=back-ldif
SLAPD_DYNAMIC_BACKENDS=

SLAPI_LIBS= 

XDEFS = $(MODULES_CPPFLAGS)
XLDFLAGS = $(MODULES_LDFLAGS)

XLIBS = $(SLAPD_STATIC_DEPENDS) $(SLAPD_L) $(MODULES_LIBS)
XXLIBS = $(SLAPD_LIBS) $(SECURITY_LIBS) $(LUTIL_LIBS)
XXXLIBS = $(LTHREAD_LIBS) $(SLAPI_LIBS)

BUILD_OPT = "--enable-slapd"
BUILD_SRV = no

all-local-srv: all-cffiles

NT_SLAPD_DEPENDS = slapd.exp
NT_SLAPD_OBJECTS = slapd.exp symdummy.o $(OBJS) version.o

UNIX_SLAPD_DEPENDS = $(SLAPD_STATIC_DEPENDS) version.o $(SLAPD_L)
UNIX_SLAPD_OBJECTS = $(OBJS) version.o

SLAPD_DEPENDS = $(UNIX_SLAPD_DEPENDS)
SLAPD_OBJECTS = $(UNIX_SLAPD_OBJECTS)

# Notes about slapd for Windows
# =============================
# slapd.exe must export all of its global symbols, just like a DLL.
# The purpose of this is to allow dynamic modules (dynamic backends
# or external dynamic modules) to bind with the symbols at run-time.
# 
# Exporting symbols from an .EXE is a bit tricky and involves multiple
# steps. First a .DEF file must be generated. The .DEF file indicates
# the set of symbols that are to be exported. Many times, it's possible
# to manually create this file with an editor. However, with slapd,
# we want to export EVERY global symbol that it knows about (NOT including
# symbols that are imported from other DLLs). The set of symbols to
# export INCLUDES symbols from all static libraries that slapd gets
# linked with, e.g. avl, lunicode, lutil, etc. This list
# will also include liblber and libldap_r if they were built as static
# libraries. ALSO included will be symbols from other STATIC libraries
# outside the domain of the OpenLDAP source tree, e.g. regex, ltdl,
# crypto, ssl, sasl, etc. (If these libraries are dynamic, we won't want
# to include their symbols in the list). The correct set of symbols
# CAN be determined at build time. The slapd.def target automatically
# determines the correct set of symbols and generates the slapd.def file.
#
# The slapd.def file, serving multiple purposes, will:
#
# 1) be used to generate libslapd.a, the import library for slapd.exe.
#
# 2) be used to generate the symdummy.c file.
#
# 3) be used to help create slapd.exp, the binary-formated slapd export file.
#
# The import library is used by dynamic modules at link time. With this
# library, dynamic modules indicate to the linker that it will resolve
# these symbols from the slapd.exe binary at run-time. Of course, whenever
# a module imports dynamic symbols, those symbols should be marked with
# the __declspec(dllimport) directive in the header files that the dynamic
# modules build with. In OpenLDAP, this is handled automatically in the
# header files. (See ldap_cdefs.h for an explanation). Writers of
# dynamic backend modules should keep in mind that slapd.exe might export
# other global symbols that are not part of OpenLDAP (e.g. regex, ltdl,
# crypto, ssl, sasl, etc.) When a writer actually uses (i.e. imports) these
# symbols, he must verify that the header files from these external packages
# include a mechanism to mark imported symbols with the __declspec(dllimport)
# directive. Whether or not such a mechanism exists, the writer must be
# able to include these directives appropriately when their symbols are
# being imported from slapd.exe. The directive is not completely necessary
# for functions, but it is required for variables.
#
# The symdummy.c file basically references EVERY symbol available to slapd.exe,
# including symbols that slapd.exe never actually referenced. The file
# is compiled and included at link time. Without this object file, slapd.exe
# would NOT export symbols that it never referenced. The reason that these
# symbols must still be exported is because a dynamic module may want to
# use a symbol even if it had not been referenced by slapd.exe.
#

#
# slapd.def REALLY depends upon all slapd objects and all static libraries
# included in $(LIBS), including static libraries outside of OpenLDAP.
# When slapd.def is built, the absolute paths to all static libraries
# (both inside and outside of OpenLDAP) are generated. We don't have
# any way to include this generated list as a dependency of slapd.def (sigh).
# Thus, we do the best we can by depending on version.o, which depends
# on its own very long list of dependencies.
#
slapd.def: libbackends.a liboverlays.a version.o
	@for i in XX $(LDFLAGS) ; do \
	    path=`expr "$$i" : "-L\(.*\)"`; \
	    if test $$? != 0; then continue; fi; \
	    paths="$$paths $$path"; \
	done; \
	objs=""; \
	for i in $(OBJS) version.o $(LIBS) ; do \
	    obj="" ; \
	    case $$i in \
		-l*) \
		    done="" ;\
		    base=`expr "$$i" : "-l\(.*\)"`; \
		    for p in . $$paths ; do \
			for ext in la dll dll.a a ; do \
			    path=$$p/lib$$base.$$ext; \
			    test ! -f $$path && continue; \
			    if test $$ext = la ; then \
				for t in dlname old_library ; do \
				    line=`grep "^$$t=" $$path`; \
				    lib=`expr "$$line" : "[^']*'\(.*\)'"`; \
				    test -n "$$lib" && test -f $$p/$$lib && \
					path=$$p/$$lib && break; \
				done; \
				test $$t = dlname && ext=dll; \
				test $$t = old_library && ext=a; \
			    fi; \
			    if test $$ext = a ; then \
				obj=$$path; \
			    fi; \
			    done=done; \
			    break; \
			done; \
			test -n "$$done" && break; \
		    done; \
		    test -z "$$obj" && continue; \
		    ;; \
		*.la) \
		    if test -n "$(LTSTATIC)"; then \
			    base=`expr "$$i" : ".*/\(.*\).la"`; \
			    path=`expr "$$i" : "\(.*/\).*"`; \
			    obj=$$path.libs/$$base.a; \
		    fi; \
		    ;; \
		*.dll.a) \
		    ;; \
		*.o | *.a) \
		    obj=$$i; \
	    esac; \
	    objs="$$objs $$obj"; \
	done; \
	echo dlltool --exclude-symbols main,ServiceMain@8 --export-all-symbols \
			--output-def $@.tmp $$objs; \
	dlltool --exclude-symbols main,ServiceMain@8 --export-all-symbols \
			--output-def $@.tmp $$objs;
	echo EXPORTS > $@
	$(SED) -e 1,2d -e 's/ @ [0-9][0-9]*//' $@.tmp | sort >> $@
	$(RM) $@.tmp

symdummy.c: slapd.def
	$(RM) $@
	@echo "generating $@..."; \
	echo "static void never_called() {" > $@.tmp; \
	cat $< | while read line; \
	do \
	    set dummy $$line; \
	    case $$# in \
		3) \
		    echo "int $$2();" >> $@; \
		    echo "$$2();" >> $@.tmp; \
		    ;; \
		4) \
		    echo "extern int $$2;" >> $@; \
		    echo "$$2 = 0;" >> $@.tmp; \
		    ;; \
	    esac; \
	done; \
	echo "" >> $@; \
	echo "}" >> $@.tmp; \
	cat $@.tmp >> $@; \
	$(RM) $@.tmp

libslapd.a: symdummy.o
	dlltool --dllname slapd.exe --input-def slapd.def --output-lib $@

slapd.exp: libslapd.a
	@echo $(LTLINK) -Wl,--base-file,slapd.base -o slapd \
		$(OBJS) symdummy.o version.o $(LIBS) $(WRAP_LIBS); \
	$(LTLINK) -Wl,--base-file,slapd.base -o slapd \
		$(OBJS) symdummy.o version.o $(LIBS) $(WRAP_LIBS)
	$(RM) slapd.exe
	@echo dlltool --dllname slapd.exe --input-def slapd.def \
		--base-file slapd.base --output-exp $@; \
	dlltool --dllname slapd.exe --input-def slapd.def \
		--base-file slapd.base --output-exp $@; \
	echo $(LTLINK) -Wl,--base-file,slapd.base -o slapd $@ \
		$(OBJS) symdummy.o version.o $(LIBS) $(WRAP_LIBS); \
	$(LTLINK) -Wl,--base-file,slapd.base -o slapd $@ \
		$(OBJS) symdummy.o version.o $(LIBS) $(WRAP_LIBS)
	$(RM) slapd.exe
	@echo dlltool --dllname slapd.exe --input-def slapd.def \
		--base-file slapd.base --output-exp $@; \
	dlltool --dllname slapd.exe --input-def slapd.def \
		--base-file slapd.base --output-exp $@

slapi/.libs/libslapi.a: FORCE
	(cd slapi; $(MAKE) $(MFLAGS) all)

libslapi.a: slapi/.libs/libslapi.a
	cp slapi/.libs/libslapi.a .

slapd: $(SLAPD_DEPENDS) 
	$(LTLINK) -o $@ $(SLAPD_OBJECTS) $(LIBS) \
		$(WRAP_LIBS)
	$(RM) $(SLAPTOOLS)
	for i in $(SLAPTOOLS); do \
		$(LN_S) slapd$(EXEEXT) $$i$(EXEEXT); done


sslapd: version.o
	$(LTLINK) -static -o $@ $(OBJS) version.o $(LIBS) $(WRAP_LIBS)

dummy $(SLAPD_DYNAMIC_BACKENDS): slapd
	cd $@; $(MAKE) $(MFLAGS) all
	@touch $@

dynamic_overlays: slapd
	cd overlays; $(MAKE) $(MFLAGS) dynamic

#
# In Windows, dynamic backends have to be built after slapd. For this
# reason, we only build static backends now and dynamic backends later.
#
.backend: FORCE
	@if test -n "$(SLAPD_STATIC_BACKENDS)"; then \
	    echo "building static backends..."; \
	    for i in XX $(SLAPD_STATIC_BACKENDS); do \
	    	if test $$i != XX; then \
		    echo " "; echo "  cd $$i; $(MAKE) $(MFLAGS) all"; \
		    ( cd $$i; $(MAKE) $(MFLAGS) all ); \
		    if test $$? != 0; then exit 1; fi; \
		fi; \
	    done; \
	    echo " "; \
	fi

libbackends.a: .backend
	@$(RM) -r tmp
	@$(MKDIR) tmp
	@-for i in back-*/*.a; do \
		( \
		  cd tmp; \
		  $(AR) x ../$$i; \
		  pre=`echo $$i | $(SED) -e 's/\/.*$$//' -e 's/back-//'`; \
		  for j in *.o; do \
			mv $$j $${pre}$$j; \
		  done; \
		  $(AR) ruv libbackends.a *.o 2>&1 | grep -v truncated; \
		  $(RM) *.o __.SYMDEF  ________64ELEL_ ; \
		  echo "added backend library $$i"; \
		  echo ""; \
		); \
	done
	@mv -f tmp/libbackends.a ./libbackends.a
	@$(RM) -r tmp
	@if test ! -z "$(RANLIB)" ; then \
		$(RANLIB) libbackends.a; \
	fi
	@ls -l libbackends.a; echo ""

liboverlays.a: FORCE
	cd overlays; $(MAKE) $(MFLAGS) static

version.c: Makefile
	@-$(RM) $@
	$(MKVERSION) -s -n Versionstr slapd > $@

version.o: version.c $(OBJS) $(SLAPD_LIBDEPEND) 

backends.o: backends.c $(srcdir)/slap.h

depend-local-srv: FORCE
	@for i in $(SUBDIRS); do \
		if test -d $$i && test -f $$i/Makefile ; then \
			echo; echo "  cd $$i; $(MAKE) $(MFLAGS) depend"; \
			( cd $$i; $(MAKE) $(MFLAGS) depend ); \
			if test $$? != 0 ; then exit 1; fi ; \
		fi; \
	done
	@echo ""

clean-local:
	$(RM) *.exp *.def *.base *.a *.objs symdummy.c

veryclean-local:
	$(RM) backends.c

clean-local-srv: FORCE
	@for i in $(SUBDIRS); do \
		if test -d $$i && test -f $$i/Makefile ; then \
			echo; echo "  cd $$i; $(MAKE) $(MFLAGS) clean"; \
			( cd $$i; $(MAKE) $(MFLAGS) clean ); \
			if test $$? != 0 ; then exit 1; fi ; \
		fi; \
	done
	$(RM) *.tmp all-cffiles

veryclean-local-srv: FORCE
	@for i in $(SUBDIRS); do \
		if test -d $$i && test -f $$i/Makefile ; then \
			echo; echo "  cd $$i; $(MAKE) $(MFLAGS) clean"; \
			( cd $$i; $(MAKE) $(MFLAGS) veryclean ); \
		fi; \
	done

install-local-srv: install-slapd install-tools \
	install-conf install-db-config install-schema install-tools

install-slapd: FORCE
	-$(MKDIR) $(DESTDIR)$(libexecdir)
	-$(MKDIR) $(DESTDIR)$(localstatedir)/run
	$(LTINSTALL) $(INSTALLFLAGS) $(STRIP) -m 755 \
		slapd$(EXEEXT) $(DESTDIR)$(libexecdir)
	@for i in $(SUBDIRS); do \
	    if test -d $$i && test -f $$i/Makefile ; then \
		echo; echo "  cd $$i; $(MAKE) $(MFLAGS) install"; \
		( cd $$i; $(MAKE) $(MFLAGS) install ); \
		if test $$? != 0 ; then exit 1; fi ; \
	    fi; \
	done

all-cffiles: slapd $(SLAPD_DYNAMIC_BACKENDS) dynamic_overlays
	@if test $(PLAT) = NT; then \
	    sysconfdir=`cygpath -w $(sysconfdir) | \
		$(SED) -e 's/\\\\/\\\\\\\\\\\\\\\\/g'`; \
	    localstatedir=`cygpath -w $(localstatedir) | \
		$(SED) -e 's/\\\\/\\\\\\\\\\\\\\\\/g'`; \
	    moduledir=`cygpath -w $(moduledir) | \
		$(SED) -e 's/\\\\/\\\\\\\\\\\\\\\\/g'`; \
	else \
	    sysconfdir=$(sysconfdir); \
	    localstatedir=$(localstatedir); \
	    moduledir=$(moduledir); \
	fi; \
	$(SED) -e "s;%SYSCONFDIR%;$$sysconfdir;" \
		-e "s;%LOCALSTATEDIR%;$$localstatedir;" \
		-e "s;%MODULEDIR%;$$moduledir;" \
		$(srcdir)/slapd.conf > slapd.conf.tmp ; \
	touch all-cffiles

install-schema: FORCE
	@if test -d $(DESTDIR)$(schemadir) ; then \
		echo "MOVING EXISTING SCHEMA DIR to $(DESTDIR)$(schemadir).$$$$" ; \
		mv $(DESTDIR)$(schemadir) $(DESTDIR)$(schemadir).$$$$ ; \
	fi
	$(MKDIR) $(DESTDIR)$(schemadir)
	@SD=$(DESTDIR)$(schemadir) ; \
	files=`cd $(srcdir)/schema ; echo README *.ldif *.schema` ; \
	for i in $$files ; do \
		echo $(INSTALL) $(INSTALLFLAGS) -m 444 schema/$$i $$SD/$$i ; \
		$(INSTALL) $(INSTALLFLAGS) -m 444 $(srcdir)/schema/$$i $$SD/$$i ; \
	done

install-conf: FORCE
	@-$(MKDIR) $(DESTDIR)$(sysconfdir)
	$(INSTALL) $(INSTALLFLAGS) -m 600 slapd.conf.tmp $(DESTDIR)$(sysconfdir)/slapd.conf.default
	if test ! -f $(DESTDIR)$(sysconfdir)/slapd.conf; then \
		echo "installing slapd.conf in $(sysconfdir)"; \
		echo "$(INSTALL) $(INSTALLFLAGS) -m 600 slapd.conf.tmp $(DESTDIR)$(sysconfdir)/slapd.conf"; \
		$(INSTALL) $(INSTALLFLAGS) -m 600 slapd.conf.tmp $(DESTDIR)$(sysconfdir)/slapd.conf; \
	else \
		echo "PRESERVING EXISTING CONFIGURATION FILE $(DESTDIR)$(sysconfdir)/slapd.conf" ; \
	fi

install-db-config: FORCE
	@-$(MKDIR) $(DESTDIR)$(localstatedir) $(DESTDIR)$(sysconfdir)
	@-$(INSTALL) -m 700 -d $(DESTDIR)$(localstatedir)/openldap-data
	$(INSTALL) $(INSTALLFLAGS) -m 600 $(srcdir)/DB_CONFIG \
		$(DESTDIR)$(localstatedir)/openldap-data/DB_CONFIG.example
	$(INSTALL) $(INSTALLFLAGS) -m 600 $(srcdir)/DB_CONFIG \
		$(DESTDIR)$(sysconfdir)/DB_CONFIG.example

install-tools: FORCE
	-$(MKDIR) $(DESTDIR)$(sbindir)
	for i in $(SLAPTOOLS); do \
		$(RM) $(DESTDIR)$(sbindir)/$$i$(EXEEXT); \
		$(LN_S) -f $(DESTDIR)$(libexecdir)/slapd$(EXEEXT) $(DESTDIR)$(sbindir)/$$i$(EXEEXT); \
	done

# $OpenLDAP: pkg/ldap/build/srv.mk,v 1.18.2.3 2008/02/11 23:26:38 kurt Exp $
## This work is part of OpenLDAP Software <http://www.openldap.org/>.
##
## Copyright 1998-2008 The OpenLDAP Foundation.
## All rights reserved.
##
## Redistribution and use in source and binary forms, with or without
## modification, are permitted only as authorized by the OpenLDAP
## Public License.
##
## A copy of this license is available in the file LICENSE in the
## top-level directory of the distribution or, alternatively, at
## <http://www.OpenLDAP.org/license.html>.
##---------------------------------------------------------------------------
#
# Makefile Template for Servers
#

all-common: all-$(BUILD_SRV)
all-no lint-no 5lint-no depend-no install-no:
	@echo "run configure with $(BUILD_OPT) to make $(PROGRAMS)"

clean-common: clean-srv FORCE
veryclean-common: veryclean-srv FORCE

lint-common: lint-$(BUILD_SRV)

5lint-common: 5lint-$(BUILD_SRV)

depend-common: depend-$(BUILD_SRV)

install-common: install-$(BUILD_SRV)

all-local-srv:
all-yes: all-local-srv FORCE

install-local-srv:
install-yes: install-local-srv FORCE

lint-local-srv:
lint-yes: lint-local-srv FORCE
	$(LINT) $(DEFS) $(DEFINES) $(SRCS)

5lint-local-srv:
5lint-yes: 5lint-local-srv FORCE
	$(5LINT) $(DEFS) $(DEFINES) $(SRCS)

clean-local-srv:
clean-srv: 	clean-local-srv FORCE
	$(RM) $(PROGRAMS) $(XPROGRAMS) $(XSRCS) *.o a.out core .libs/* *.exe

depend-local-srv:
depend-yes: depend-local-srv FORCE
	$(MKDEP) $(DEFS) $(DEFINES) $(SRCS)

veryclean-local-srv:
veryclean-srv: 	clean-srv veryclean-local-srv

Makefile: $(top_srcdir)/build/srv.mk
