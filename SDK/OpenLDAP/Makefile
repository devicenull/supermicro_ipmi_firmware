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
	$(Q)cd $(LDAP_SRCDIR); \
	if [ ! -d openldap ];then mkdir openldap;cd openldap;tar zxf $(LDAP_ZIP);fi
	$(Q)cd $(LDAP_SRCDIR)/openldap;if [ ! -f $(LDAP_SRCDIR)/openldap/libraries/libldap/.libs/libldap.a ];then \
	./configure $(LDAP_CFG); make depend; make;fi
	$(Q)cd $(LDAP_SRCDIR); \
	if [ ! -d $(FS_PATH)/lib ]; then mkdir $(FS_PATH)/lib; fi
	$(call CONDITION_CC1  ,ldap_client.c,-I$(LDAP_SRCDIR)/openldap/include ) 
	${CC} -shared -o $(FS_PATH)/lib/$(LIBNAME) ldap_client.o \
	-L$(LDAP_SRCDIR)/openldap/libraries/libldap/.libs -lldap \
	-L$(LDAP_SRCDIR)/openldap/libraries/liblber/.libs -llber
	$(CROSS_COMPILE)strip $(FS_PATH)/lib/$(LIBNAME)

