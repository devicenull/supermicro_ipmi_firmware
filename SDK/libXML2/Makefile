-include $(PRJ_PATH)/.SDKPath

HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
LIBXML2_DIR = $(PRJ_PATH)/libXML2/libxml2/libxml2-2.6.32
unexport CC LD AR CXX NM RANLIB OBJCOPY OBJDUMP \
STRIP CFLAGS LDFLAGS LIBS

XML2_CFG = --prefix=$$PWD/../local  --host=$(HOST)  CFLAGS=" -Os "\
--enable-ipv6=no \
--with-pic \
--without-c14n \
--without-debug \
--without-docbook \
--without-ftp \
--without-history \
--without-html \
--without-http \
--without-iso8859x \
--without-legacy \
--without-mem-debug \
--without-run-debug \
--without-valid \
--without-writer \
--without-xinclude 

DTKlibXML2_DIR = $(DTKPATH)/libXML2

all:
	@echo "Build libxml2 ........"
	$(Q)if [ ! -d $(LIBXML2_DIR)/../local ]; then \
	tar zxf libxml2.tar.gz ;\
	cp $(LIBXML2_DIR)/../../patch/* $(LIBXML2_DIR);cd $(LIBXML2_DIR) ;\
	patch -p2 < xpath.patch; \
	./configure $(XML2_CFG) ; \
	make; make install; make install-strip;\
	fi
	$(Q)cp $(LIBXML2_DIR)/../local/lib/*.so $(PRJ_PATH)/lib
	$(Q)cd $(PRJ_PATH)/lib/;ln -s libxml2.so libxml2.so.2;cd -
	$(Q)if [ ! -d  $(TC_LOCAL)/lib/pkgconfig ];then mkdir  $(TC_LOCAL)/lib/pkgconfig;fi 
	$(Q)cp $(LIBXML2_DIR)/../local/lib/pkgconfig/*  $(TC_LOCAL)/lib/pkgconfig

install:
	#******************************do nothing***************************

CLEAN_FILES = libxml2/
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
#================= SDK param  ===================
SDK_FEATURE_NAME = libXML2
SDK_FILE_CP_LIST = libxml2.tar.gz  Makefile
#================= end of SDK param ===================
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
