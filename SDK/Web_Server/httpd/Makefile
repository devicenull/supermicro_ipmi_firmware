-include $(PRJ_PATH)/.SDKPath
HTTPD_SRCDIR = $(PRJ_PATH)/Web_Server/httpd

LIGHTTPD_HOST   = --host=$(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
LIGHTTPD_PREFIX = --prefix=$(HTTPD_SRCDIR)/lighttpd/local --exec-prefix=$(HTTPD_SRCDIR)/lighttpd/local
LIGHTTPD_CONF   = --with-openssl=$(PRJ_PATH)/OpenSSL/openssl/local/ssl \
--without-zlib --without-bzip2 --without-pcre \
--with-openssl-includes=$(PRJ_PATH)/OpenSSL/openssl/local/include \
--with-openssl-libs=$(PRJ_PATH)/OpenSSL/openssl/local/lib

DTKHTTPD_DIR = $(DTKPATH)/Web_Server/httpd

override CFLAGS = 
override LDFLAGS =
override LIBS =
START_DATE=$(shell date +"%g%m%d")
END_DATE=$(shell date +"%g%m%d" --date="+3 year")

all:

install:
#copy httpd
	$(Q)echo "******************** make httpd ********************"
	$(Q)set -e;\
	if [ ! -d $(HTTPD_SRCDIR)/lighttpd/local ]; then \
	cd $(HTTPD_SRCDIR)/lighttpd; \
	./configure $(LIGHTTPD_HOST) $(LIGHTTPD_PREFIX) $(LIGHTTPD_CONF); \
	make; make install; make install-strip; fi
	$(Q)echo "******************** build httpd ********************"
	$(Q)if [ ! -d  $(FS_PATH)/usr/local/httpd ];then \
	mkdir $(FS_PATH)/usr/local/httpd; fi
	$(Q)cd $(HTTPD_SRCDIR); \
	cp -f server.pem $(FS_PATH)/usr/local/httpd/; \
	cp -f lighttpd.conf $(FS_PATH)/usr/local/httpd/; \
	cp -rf lighttpd/local/*  $(FS_PATH)/usr/local/httpd/; \
	cp -fd httpd $(FS_PATH)/etc/init.d; \
	sed -i 's/\(START_DATE=\)/\1${START_DATE}000000Z/' $(FS_PATH)/etc/ssl/CA.sh ;\
	sed -i 's/\(END_DATE=\)/\1${END_DATE}000000Z/' $(FS_PATH)/etc/ssl/CA.sh;\
	rm -rf $(FS_PATH)/usr/local/httpd/share/man;
	
CLEAN_FILES =  $(HTTPD_SRCDIR)/lighttpd local
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

extract:
	$(Q)echo "******************** extract httpd ********************"
	$(Q)if [ ! -d $(DTKHTTPD_DIR) ];then mkdir -p $(DTKHTTPD_DIR); fi
	$(Q)cd $(HTTPD_SRCDIR); cp -f Makefile server.pem *.conf *.tar.gz $(DTKHTTPD_DIR)/
	$(Q)cat Makefile| sed \
	-e '/SDK/d' -e '/DTK/d' \
	-e '/extract/d' \
	-e '/cat Makefile/d' \
	-e '/Makefile_tmp/d' -e "/-e [\'\"]/d" \
	> $(DTKHTTPD_DIR)/Makefile

