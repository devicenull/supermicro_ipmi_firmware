-include $(PRJ_PATH)/.SDKPath
DDNS_BIN_DIR = $(PRJ_PATH)/DDNS/bin
DDNS_TOOL_DIR = $(PRJ_PATH)/DDNS/tools
BIND_DIR = $(PRJ_PATH)/DDNS/tools/bind-9.8.1-P1
BIND_VERSION = bind-9.8.1-P1

MY_CFG_HOST = arm-linux
override CFLAGS = -Os
override LDFLAGS =
override LIBS =
override INCLUDEDIR =


ddns_program:
	$(Q)echo "================ make DDNS ==================="
	$(Q)set -e
	$(Q)if [ ! -d  $(BIND_DIR) ];then \
    cd $(DDNS_TOOL_DIR); tar -zxf $(BIND_VERSION).tar.gz ;fi
	$(Q)cd $(BIND_DIR);BUILD_CC=gcc ./configure --host=arm-linux --build=i686-pc-linux-gnu --without-openssl --without-randomdev --disable-epoll --without-gssapi ; make; $(STRIP) $(BIND_DIR)/bin/nsupdate/nsupdate \
    ;cp -f $(BIND_DIR)/bin/nsupdate/nsupdate $(DDNS_BIN_DIR)/
	$(Q)echo "================ make DDNS done ==================="

#ddns_clean:
#	$(Q)echo "================ clean DDNS ==================="
#	$(Q)cd $(BIND_DIR); make clean

#clean: ddns_clean
