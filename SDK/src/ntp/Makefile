NTP_BIN_DIR = $(PRJ_PATH)/src/ntp/bin
NTP_ETC_DIR = $(PRJ_PATH)/src/ntp/etc
NTP_TOOL_ROOTDIR = $(PRJ_PATH)/src/ntp/tools
NTP_VERSION = ntp-4.2.4p4
NTP_TOOL_DIR = $(PRJ_PATH)/src/ntp/tools/$(NTP_VERSION)
DTKNTP_DIR = $(DTKPATH)/NTP

#override CFLAGS = -Os
#override LDFLAGS =
override LIBS =
override INCLUDEDIR =


all:
	$(Q)if [ ! -f $(NTP_BIN_DIR)/ntpdate ] || [ ! -f $(NTP_BIN_DIR)/ntpd ];then \
	mkdir -p $(NTP_BIN_DIR); cd $(NTP_TOOL_ROOTDIR); make ntp_program; fi
	$(Q)echo "================  Install NTP ================" 
	$(Q)cd $(NTP_BIN_DIR); cp -f ntpd ntpdate $(FS_PATH)/bin 
	$(Q)if [ ! -d  $(FS_PATH)/usr/local/etc/ntp ];then \
    	mkdir $(FS_PATH)/usr/local/etc/ntp; fi
	cp -f $(NTP_ETC_DIR)/ntp.conf $(FS_PATH)/usr/local/etc/ntp
	$(Q)ln -sf /nv/ntp/ntp.conf $(FS_PATH)/etc/ntp.conf
	$(Q)ln -sf /nv/timezone $(FS_PATH)/usr/local/etc/timezone
	$(Q)echo "================ build NTP done ================"

CLEAN_FILES = $(NTP_BIN_DIR)/ntpdate $(NTP_BIN_DIR)/ntpd $(NTP_TOOL_DIR)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

SDK_FEATURE_NAME = NTP
SDK_FILE_CP_LIST = ./bin/ntp_service.sh ./bin/ntp_setenv.c ./etc/ntp.conf ./Makefile \
./tools/Makefile ./tools/*.tar.gz 

include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
