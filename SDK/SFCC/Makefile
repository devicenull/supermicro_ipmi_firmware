-include $(PRJ_PATH)/.SDKPath
LIBS_SUITE_DIR = $(PRJ_PATH)/SFCC
HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
override PATH:=$(LIBS_SUITE_DIR)/curl/local/bin:$(PATH)
unexport CC LD AR CXX NM RANLIB OBJCOPY OBJDUMP \
STRIP CFLAGS LDFLAGS LIBS

DTKSFCC_DIR = $(DTKPATH)/SFCC

PKG_CONFIG_PATH = $(TC_LOCAL)/lib/pkgconfig

CURL_DIR      = $(LIBS_SUITE_DIR)/curl/curl-7.18.1
SFCC_DIR      = $(LIBS_SUITE_DIR)/sfcc/sblim-sfcc-2.1.0
CURL_BUILD_DIR = $(LIBS_SUITE_DIR)/curl
SFCC_BUILD_DIR = $(LIBS_SUITE_DIR)/sfcc


CFG_PRF   = --prefix=$$PWD/../local --exec-prefix=$$PWD/../local CFLAGS=" -Os "
CFG_HOST  = --host=$(HOST)

SFCC_CFG = --oldincludedir=$(TC_LOCAL)/include
SFCC_MFLAGS = libcimcClientXML_la_LIBADD=`curl-config --libs` INCLUDES=`curl-config --cflags`

CURL_CFG =\
--disable-largefile \
--disable-ftp \
--disable-file \
--disable-ldap \
--disable-ldaps \
--disable-dict \
--disable-telnet \
--disable-tftp \
--disable-manual \
--disable-ipv6 \
--disable-verbose \
--disable-sspi \
--disable-debug \
--disable-cookies \
--with-pic \
--without-libssh2 \
--without-gnutls \
--without-nss \
--without-ca-bundle \
--without-ca-path \
--without-libidn

export PKG_CONFIG_PATH LIBS_SUITE_DIR

PKG_LIST = $(CURL_DIR) $(SFCC_DIR)

all: 

install: CHECK_FOLDER $(PKG_LIST) CP_TASK

.PHONY: CHECK_FOLDER
CHECK_FOLDER:
	if [ ! -d $(LIBS_SUITE_DIR)/sfcc ];then \
	cd $(LIBS_SUITE_DIR); tar zxf libs_sfcc.tar.gz; fi

.PHONY: $(CURL_DIR)
$(CURL_DIR):
# curl
	@echo "Build curl ........"
	if [ ! -d $(CURL_DIR)/../local ]; then \
	cd $(CURL_DIR); ./configure $(CFG_PRF) $(CFG_HOST) $(CURL_CFG); \
	make; make install; make install-strip; \
	if [ ! -d $(PKG_CONFIG_PATH) ];then mkdir -p $(PKG_CONFIG_PATH); fi; \
	if [ ! -f $(PKG_CONFIG_PATH)/libcurl.pc ];then \
	cp ../local/lib/pkgconfig/* $(PKG_CONFIG_PATH)/; fi;\
	fi;

.PHONY: $(SFCC_DIR)
$(SFCC_DIR):
# sfcc
	@echo "Build sfcc ........"
	if [ ! -d $(SFCC_DIR)/../local ]; then \
	cd $(SFCC_DIR);./configure $(CFG_PRF) $(CFG_HOST) $(SFCC_CFG); \
	make $(SFCC_MFLAGS); make install; make install-strip; \
	if [ ! -f $(TC_LOCAL)/lib/libcmpisfcc.so ];then \
	cp -a $(SFCC_DIR)/../local/* $(TC_LOCAL)/; fi;\
	fi;

.PHONY: CP_TASK
CP_TASK:
	cp -af $(SFCC_DIR)/../local/lib/*.so* $(FS_PATH)/lib
	cp -af $(CURL_DIR)/../local/lib/*.so* $(FS_PATH)/lib



CLEAN_FILES = $(CURL_DIR) $(SFCC_DIR) $(CURL_BUILD_DIR) $(SFCC_BUILD_DIR)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

SDK_FEATURE_NAME = SFCC
SDK_FILE_CP_LIST = *.tar.gz Makefile
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
