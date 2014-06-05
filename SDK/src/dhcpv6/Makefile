tmp_dir=wide-dhcpv6
Conf_Dest=$(FS_PATH)/etc/dhcpv6
PACKAGE=wide-dhcpv6-20080615.tar.gz
DTKDHCPV6_DIR = $(DTKPATH)/DHCPV6
ifeq ($(MYMCU),AST2050)
EXTCFLAGS=-DLINE_MAX=2048
endif

#================= SDK param  ===================
SDK_FEATURE_NAME = DHCPV6
SDK_FILE_CP_LIST = ./Makefile ./wide-dhcpv6-20080615.tar.gz ./conf/dhcp6c.conf-stateful \
./conf/dhcp6c.conf-stateless ./conf/dhcp6c-script
#================= end of SDK param ===================

all:
	$(Q)if [ ! -d $(tmp_dir) ];then mkdir $(tmp_dir);cd $(tmp_dir);tar zxf ../$(PACKAGE);fi
	$(Q)if [ ! -f $(tmp_dir)/dhcp6c ];then cd $(tmp_dir);CFLAGS=$(EXTCFLAGS) make  CC=$(CC) LD=$(LD) AR=$(AR) ;fi
	$(Q)cp -f $(tmp_dir)/dhcp6c $(FS_PATH)/bin/
	$(Q)if [ ! -d $(Conf_Dest) ];then mkdir $(Conf_Dest);fi
	$(Q)cp conf/* $(Conf_Dest)

CLEAN_FILES = $(tmp_dir)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
