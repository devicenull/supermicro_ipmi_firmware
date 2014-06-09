-include $(PRJ_PATH)/.SDKPath
HTTPD_SRCDIR = $(SRCDIR)/lighttpd/

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
	$(Q)set -e;\
	if [ ! -d $(HTTPD_SRCDIR)/lighttpd/local ]; then \
	cd $(HTTPD_SRCDIR)/lighttpd; \
	./configure $(LIGHTTPD_HOST) $(LIGHTTPD_PREFIX) $(LIGHTTPD_CONF); \
	make; make install; make install-strip; fi
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
