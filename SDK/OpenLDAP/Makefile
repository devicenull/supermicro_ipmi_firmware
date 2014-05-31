-include $(PRJ_PATH)/.SDKPath
HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))

ifeq ($(MYMCU),YARKON)
GNU_SRC_ARG=-D_GNU_SOURCE
else
GNU_SRC_ARG=
endif
	
LDAP_SRCDIR = $(PRJ_PATH)/OpenLDAP
LDAP_ZIP    = $(wildcard $(LDAP_SRCDIR)/*.tar.gz)
LDAP_CFG    =  --with-tls=openssl --without-kerberos --disable-debug --without-cyrus-sasl \
--without-threads --disable-slapd --disable-slurpd --disable-shared \
-host=${HOST} \
CFLAGS="-I$(PRJ_PATH)/OpenSSL/openssl/local/include" \
CPPFLAGS="-I$(PRJ_PATH)/OpenSSL/openssl/local/include $(GNU_SRC_ARG)" \
LDFLAGS="-L$(PRJ_PATH)/OpenSSL/openssl/local/lib -lcrypto -lssl"

#--prefix=$(LDAP_SRCDIR)/openldap/local 
LIBNAME = libldap_client.so

DTKLDAP_DIR = $(DTKPATH)/OpenLDAP

override CFLAGS = 
override LDFLAGS = 
override LIBS =
all:
	$(Q)echo "******************** make OpenLDAP ********************"
	$(Q)cd $(LDAP_SRCDIR); \
	if [ ! -d openldap ];then mkdir openldap;cd openldap;tar zxf $(LDAP_ZIP);fi
	$(Q)cd $(LDAP_SRCDIR)/openldap;if [ ! -f $(LDAP_SRCDIR)/openldap/libraries/libldap/.libs/libldap.a ];then \
	./configure $(LDAP_CFG); make depend; make;fi
	$(Q)cd $(LDAP_SRCDIR); \
	if [ ! -d $(LIBPATH) ]; then mkdir $(LIBPATH); fi
	$(call CONDITION_CC1  ,ldap_client.c,-I$(LDAP_SRCDIR)/openldap/include ) 
	${CC} -shared -o $(LIBPATH)/$(LIBNAME) ldap_client.o \
	-L$(LDAP_SRCDIR)/openldap/libraries/libldap/.libs -lldap \
	-L$(LDAP_SRCDIR)/openldap/libraries/liblber/.libs -llber
	$(CROSS_COMPILE)strip $(LIBPATH)/$(LIBNAME)


CLEAN_FILES = $(LDAP_SRCDIR)/openldap $(wildcard *.o)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
SDK_FEATURE_NAME = OpenLDAP
SDK_FILE_CP_LIST =  ldap_client.c ldap.h openldap-config/ldap.conf \
					openldap-config/cacerts/client.pem Makefile *.tar.gz
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
