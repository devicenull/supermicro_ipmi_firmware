SSL_SRCDIR = $(SRCDIR)/openssl
SSL_ZIP    = $(wildcard $(SSL_SRCDIR)/*.tar.gz)
SSL_CFG    = no-asm no-camellia no-gmp no-krb5 no-mdc2 no-rc5 no-rfc3779 no-seed \
shared no-tlsext no-zlib --prefix=$$PWD/local/ --openssldir=$$PWD/local/ssl
SSL_MFG    = AR="$(CROSS_COMPILE)ar r"

DTKSSL_DIR = $(DTKPATH)/OpenSSL

override CFLAGS = 
override LDFLAGS =

all:
	$(Q)cd $(SSL_SRCDIR); \
	if [ ! -d openssl ];then mkdir openssl; cd openssl;tar zxf $(SSL_ZIP); mkdir lib; fi
	$(Q)cd $(SSL_SRCDIR)/openssl; if [ ! -f $(SSL_SRCDIR)/openssl/local/lib/libssl.so ];then ./config $(SSL_CFG); make $(SSL_MFG);\
	make install; $(STRIP) local/lib/*.so.*; $(STRIP) apps/openssl; cp libcrypto.so libssl.so lib; fi;\
	if [ ! -d $(TC_LOCAL)/lib/pkgconfig ];then mkdir $(TC_LOCAL)/lib/pkgconfig;fi;\
	cp -f local/lib/pkgconfig/* $(TC_LOCAL)/lib/pkgconfig/ 
	$(Q)rm -f $(PRJ_PATH)/lib/libcrypto.so.0.9.8 $(FS_PATH)/lib/libssl.so
	$(Q)cp $(SSL_SRCDIR)/openssl/local/lib/libcrypto.so.0.9.8 $(FS_PATH)/lib/
	$(Q)cp $(SSL_SRCDIR)/openssl/local/lib/libssl.so $(FS_PATH)/lib/
	$(Q)ln -s $(FS_PATH)/lib/libssl.so $(FS_PATH)/lib/libssl.so.0.9.8
	$(Q)ln -s $(FS_PATH)/lib/libcrypto.so.0.9.8 $(FS_PATH)/lib/libcrypto.so
	$(Q)cp $(SSL_SRCDIR)/openssl/apps/openssl $(FS_PATH)/bin


CLEAN_FILES = $(SSL_SRCDIR)/openssl
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
#================= SDK param  ===================
SDK_FEATURE_NAME = OpenSSL
SDK_FILE_CP_LIST = Makefile  openssl-0.9.8h.tar.gz
#================= end of SDK param ===================
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
