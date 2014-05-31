-include $(PRJ_PATH)/.SDKPath
WSMAN_SUITE_DIR = $(PRJ_PATH)/OpenWSMAN
HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
override PATH:=$(PRJ_PATH)/SFCC/curl/local/bin:$(PATH)
CFLAG = $(CFLAGS)
unexport CC LD AR CXX NM RANLIB OBJCOPY OBJDUMP \
STRIP CFLAGS LDFLAGS LIBS

DTKWSMAN_DIR = $(DTKPATH)/OpenWSMAN

PKG_CONFIG_PATH = $(TC_LOCAL)/lib/pkgconfig

OPENWSMAN_DIR = $(WSMAN_SUITE_DIR)/openwsman/openwsman-2.1.0

CFG_HOST  = --host=$(HOST)

WSMAN_CFG = --prefix=/wsman/openwsman --exec-prefix=/wsman/openwsman \
CFLAGS="`pkg-config --cflags openssl` -Os"

export PKG_CONFIG_PATH WSMAN_SUITE_DIR

PKG_LIST := $(OPENWSMAN_DIR)

all: 
ifeq ($(CONFIG_GPL_BUILD),)
	$(Q)echo "******************** build WSMAN(wsman_account_handler) ********************"
	$(call CONDITION_CC1  ,wsman_account_handler.c,$(CFLAG)) 
	$(CROSS_COMPILE)gcc wsman_account_handler.o $(CFLAG) -shared -o $(PRJ_PATH)/lib/libwsman_account_handler.so
endif

install: CHECK_FOLDER $(PKG_LIST) CP_TASK

.PHONY: CHECK_FOLDER
CHECK_FOLDER:
	if [ ! -d $(WSMAN_SUITE_DIR)/openwsman ];then \
	cd $(WSMAN_SUITE_DIR); tar zxf openwsman.tar.gz; fi
	
.PHONY: $(OPENWSMAN_DIR)
$(OPENWSMAN_DIR):
# openwsman
	@echo "Build openwsman ........"
	if [ ! -f $(OPENWSMAN_DIR)/Makefile ]; then \
	cd $(OPENWSMAN_DIR);./configure $(WSMAN_CFG) $(CFG_HOST) CC=$(CROSS_COMPILE)gcc RANLIB=$(CROSS_COMPILE)ranlib CXX=$(CROSS_COMPILE)g++ STRIP=$(CROSS_COMPILE)strip; \
	cd $(OPENWSMAN_DIR); make; sudo make install; sudo make install-strip; \
	fi;

.PHONY: CP_TASK
CP_TASK:
	if [ ! -d /wsman ]; then mkdir /wsman; fi
	sudo chmod 777 -R /wsman
	sudo rm -rf /wsman/openwsman/lib/*.a \
	/wsman/openwsman/lib/*.la \
	/wsman/openwsman/lib/openwsman/authenticators/*.a \
	/wsman/openwsman/lib/openwsman/authenticators/*.la \
	/wsman/openwsman/lib/openwsman/plugins/*.a \
	/wsman/openwsman/lib/openwsman/plugins/*.la \
	/wsman/openwsman/include
	sudo cp -a /wsman $(FS_PATH)/
	cp -rf $(OPENWSMAN_DIR)/../cp_file/* $(FS_PATH)/wsman/openwsman/etc/openwsman/
	cp -f $(WSMAN_SUITE_DIR)/Host/$(MYMCU)/htpasswd $(FS_PATH)/bin/
	cd $(FS_PATH)/wsman/openwsman/etc/openwsman/; \
	ln -s /nv/wsman/simple_auth.passwd simple_auth.passwd
	rm -rf $(FS_PATH)/wsman/openwsman/include

CLEAN_FILES = $(WSMAN_SUITE_DIR)/htpasswd $(WSMAN_SUITE_DIR)/openwsman/ $(OPENWSMAN_DIR)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

SDK_FEATURE_NAME = OpenWSMAN
SDK_FILE_CP_LIST = openwsman.tar.gz *.o  Makefile Host/HERMON/htpasswd
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
