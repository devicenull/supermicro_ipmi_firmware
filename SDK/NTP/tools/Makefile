-include $(PRJ_PATH)/.SDKPath
NTP_BIN_DIR = $(PRJ_PATH)/NTP/bin
NTP_TOOL_ROOTDIR = $(PRJ_PATH)/NTP/tools
NTP_TOOL_DIR = $(PRJ_PATH)/NTP/tools/ntp-4.2.4p4
NTP_VERSION = ntp-4.2.4p4

ifeq ($(MYMCU),AST2050)
MY_CFG_HOST = arm-aspeed-linux-gnu
endif

ifeq ($(MYMCU),HERMON)
MY_CFG_HOST = arm-linux
endif

ifeq ($(MYMCU),YARKON)
MY_CFG_HOST = arm-none-linux-gnueabi
endif

override CFLAGS = -Os
override LDFLAGS =
override LIBS =
override INCLUDEDIR =

ntp_program:
	$(Q)echo "================ make NTP ==================="
	$(Q)cd $(NTP_TOOL_DIR); ./configure --prefix=/local --host=$(MY_CFG_HOST); make; $(STRIP) ./ntpdate/ntpdate ;\
	$(STRIP) ./ntpd/ntpd; cp -f ./ntpdate/ntpdate $(NTP_BIN_DIR)/; cp -f ./ntpd/ntpd $(NTP_BIN_DIR)/
	$(Q)echo "================ make NTP done ==================="

ntp_clean:
	$(Q)echo "================ clean NTP ==================="
	$(Q)cd $(NTP_TOOL_DIR); make clean

clean: ntp_clean
