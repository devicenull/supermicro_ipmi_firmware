ifdef  BOARD
-include Project_File/OS/Linux/Host/$(MYMCU)/Board/$(BOARD)/Projectfile
else 
-include .config
endif
include .machine
-include ./ProjectConfig-$(MYMCU)
-include ./Project_File/OS/Linux/Host/$(MYMCU)/Board/$(BOARD)/flash_layout.config
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
CXX=$(CROSS_COMPILE)g++
NM=$(CROSS_COMPILE)nm
RANLIB=$(CROSS_COMPILE)ranlib
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump 
STRIP = $(CROSS_COMPILE)strip
-include ./ProjectConfig-x86
X86_CC=$(X86_CROSS_COMPILE)gcc
X86_LD=$(X86_CROSS_COMPILE)ld
X86_AR=$(X86_CROSS_COMPILE)ar
X86_CXX=$(X86_CROSS_COMPILE)g++
X86_NM=$(X86_CROSS_COMPILE)nm
X86_RANLIB=$(X86_CROSS_COMPILE)ranlib
X86_OBJCOPY=$(X86_CROSS_COMPILE)objcopy
X86_OBJDUMP=$(X86_CROSS_COMPILE)objdump 
X86_STRIP = $(X86_CROSS_COMPILE)strip
export X86_CC X86_LD X86_AR X86_CXX X86_NM X86_RANLIB X86_OBJCOPY X86_OBJDUMP X86_STRIP
-include ./ProjectConfig-JavaAP

PRJ_PATH=$(shell pwd)
#for tool chain
TOOLCHAIN_ZIP=$(wildcard $(PRJ_PATH)/ToolChain/Host/$(MYMCU)/*.tar.gz)
X86_TOOLCHAIN_ZIP=$(wildcard $(PRJ_PATH)/ToolChain/Host/i386/gcc*.tar.gz)

ifneq ($(CONFIG_COMPILE_FLAGS),"")
USER_DEFINE_CFLAGS := $(shell echo $(CONFIG_COMPILE_FLAGS))
else
USER_DEFINE_CFLAGS :=
endif

CFLAGS = -Wall -Wno-unused-variable -fpic -DKVM_VIEWER_VERSION=\"$(JAVA_KVM_VIEWER_FULL_VERSION)\" -DKVM_VIEWER_LIB_VERSION=\"$(JAVA_KVM_LIB_VERSION)\" -DSOL_VIEWER_VERSION=\"$(JAVA_SOL_VIEWER_FULL_VERSION)\" -DOS_LINUX -DIPMI_RMCP_PLUS_SUB_SOL -DIPMI_RMCP_PLUS -DIPMI_RMCP_PLUS_SUB_AES -DOWN_AST_WEB_SERVER -DSMTP -DWSMAN -DIPMI_DHCP -DRAM_SENSOR_EN -DENABLE_OWN_AST_LDAP -DENABLE_AUTH_RADIUS -DBMC_VLAN -DFW_CHKSUM $(USER_DEFINE_CFLAGS)

ifeq ($(BOARD),SuperMicro_X7SB3)
CFLAGS +=   -DFRU_RECOVER
endif

ifeq ($(CONFIG_DEBUG),y)
CFLAGS +=   -DDEBUG 
endif

ifeq ($(CONFIG_MODULE_SEL_AGE),y)
CFLAGS +=   -DSEL_AGING
endif


ifeq ($(CONFIG_NM),y)
CFLAGS +=   -DNODE_MAN_EN
endif

ifeq ($(CONFIG_MODULE_FW_FIREWALL),y)
CFLAGS +=   -DIPMI_FW_FIREWALL
CFLAGS +=   -DRAMTbl
endif

#OPENWSMAN
ifeq ($(CONFIG_OPENWSMAN),y)
CFLAGS +=   -DWSMAN_FEATURE
endif


#OPMA
ifeq ($(CONFIG_PLATFORM_IP1500),y)
CFLAGS +=   -DIPMI_OPMA_FEATURE
else
ifeq ($(CONFIG_PLATFORM_ADLINK_CMM),y)
CFLAGS +=   -DIPMI_OPMA_FEATURE
endif
ifeq ($(CONFIG_PLATFORM_NEXCOM_NSB7110),y)
CFLAGS +=   -DIPMI_OPMA_FEATURE
endif
endif

#Quiet build 
Q = @
# 0: static link  1:dynamic link
ifeq ($(CONFIG_BUILD_SHARE_LIB),y)
    BUILD_OPTION = 1
else 
    BUILD_OPTION = 0
endif 

ifeq ($(CONFIG_STRIP_OBJECTS),y)
    STRIP_OPTION = 1
else 
    STRIP_OPTION = 0
endif

ifeq ($(CONFIG_SPIFLASH_DRV),y)
CFLAGS += -DNVRAM_FLASH_UPDATE
endif

ifeq ($(CONFIG_COMPUTE_BLADE),y)
CFLAGS +=   -DOPEN_BLADE
endif

ifeq ($(CONFIG_CHASSIS_MANAGER),y)
CFLAGS +=   -DCHASSIS_MANAGER
endif

ifneq ($(CONFIG_LINK_FLAGS),"")
LDFLAGS += $(CONFIG_LINK_FLAGS)
endif
ifeq ($(CONFIG_DDNS),y)
CFLAGS +=   -DDDNS_FEATURE
endif
ifeq ($(CONFIG_NTP),y)
CFLAGS +=	-DNTP_FEATURE
endif

ifeq ($(CONFIG_DCMI),y)
CFLAGS +=   -DIPMI_DCMI_FEATURE
endif

ifeq ($(CONFIG_TFTP),y)
CFLAGS +=   -DATEN_TFTP_FEATURE
endif

ifeq ($(CONFIG_NW_FIREWALL),y)
CFLAGS +=   -DNW_FIREWALL_FEATURE
endif

ifeq ($(CONFIG_SDK_PARTIAL_RELEASE),y)
SDK_RELEASE_MODE = PartialRelease
else  #SDK
SDK_RELEASE_MODE = TotalRelease
endif #SDK
# the paths of the library and executable file 
KERNELDIR = $(shell pwd)/Kernel/Host/$(MYMCU)/linux
MODULEPATH=$(shell pwd)/bin/module
BINPATH=$(shell pwd)/bin
LIBPATH=$(shell pwd)/lib
IMAGE_PATH= $(shell pwd)/images
MAKEFLAGS += --no-print-directory



include .include
#default inculde math library
LIBS += -lcgi -lipmi -lchnl  -lm -lsol  -lutility -lnvram  -lipmicrypt -lcommon -lsys -lrt -luart -lupdate -lnet -lmqueue -lgpio -lxml2


ifeq ($(CONFIG_WDT_DRV),y)
LIBS += -lwdt
endif

ifeq ($(MYMCU), HERMON)
ifeq ($(CONFIG_PWM_DRV),y)
LIBS +=  -lpwm
endif
endif
LDFLAGS += -lldap_client -lradius_client -L$(PRJ_PATH)/OpenSSL/openssl/local/lib -lcrypto -lssl
ifeq ($(MYMCU), PILOT3)
LIBS +=  -lswc
endif


ifeq ($(CONFIG_EXT_DVI),y)
CFLAGS += -DEXT_DVI
CCFLAGS= -DEXT_DVI
CXXFLAGS= -DEXT_DVI
endif
ifeq ($(CONFIG_INT_VGA),y)
CFLAGS += -DINT_VGA
CCFLAGS= -DINT_VGA
CXXFLAGS= -DINT_VGA
endif

ifeq ($(CONFIG_UPNP),y)
CFLAGS += -DUPNP_FEATURE
endif

ifeq ($(CONFIG_LLDP),y)
CFLAGS += -DLLDP_FEATURE
endif


LDFLAGS += $(LIBS) -L$(PRJ_PATH)/lib
CFLAGS += -I$(INCLUDEDIR)
HOSTPATH = OS/Linux/Host/$(MYMCU)/Board/$(BOARD)
export NM STRIP OBJDUMP OBJCOPY CC CXX LD AR RANLIB LDFLAGS ARFLAGS CFLAGS KERNELDIR CROSS_COMPILE ARCH MAKEFLAGS Q BUILD_OPTION WEBSTYLE WEBSTYLESUB STRIP_OPTION CXXFLAGS CCFLAGS
export INCLUDEDIR LIBS BOARD MYMCU PRJ_PATH MODULEPATH BINPATH LIBPATH HOSTPATH IMAGE_PATH 

#BOOTLOADER

#Kernel 
KERNEL-y = Kernel/

#FileSystem
FS-y = FileSystem/

#UploadImage
OUTPUT_IMG-y = MKIMG_Tool/

#LibXML2
LIBXML2-y = $(PRJ_PATH)/libXML2

#Utility
UTILS-y = $(PRJ_PATH)/Utils 

#IPMI module 
IPMI-y :=   $(PRJ_PATH)/IPMI/ 
#Open Blade module
STATE_MACHINE-$(CONFIG_COMPUTE_BLADE) :=  $(PRJ_PATH)/State_Machine/
CHASSIS_MANAGER-$(CONFIG_CHASSIS_MANAGER) := $(PRJ_PATH)/Chassis_Manager/

#LIBMQUEUE
LIBMQUEUE-y := $(PRJ_PATH)/ToolChain

#Common Library
OPENSSL-y = $(PRJ_PATH)/OpenSSL
OPENLDAP-y = $(PRJ_PATH)/OpenLDAP
SMTP-y = $(PRJ_PATH)/SMTP/

#CGI
WEB_PAGE-$(CONFIG_WEBSERVER) := $(PRJ_PATH)/Web_Server/

#Web server 
WEB_SERVER-$(CONFIG_WEBSERVER) :=  $(PRJ_PATH)/Web_Server/httpd

#OpenWSMAN
OPENWSMAN-$(CONFIG_OPENWSMAN) := $(PRJ_PATH)/OpenWSMAN
ifeq (y,$(CONFIG_OPENWSMAN))
CONFIG_SFCC = y
CONFIG_SFCB = y
endif

# vUSB module and driver
vUSB_MODULE-$(CONFIG_VIRTUALUSB) := $(PRJ_PATH)/USB_VD/
# IKVM Module and Driver
IKVM_MODULE-$(CONFIG_IKVM) := $(PRJ_PATH)/IKVM

# SNMP
SNMP-$(CONFIG_SNMP) := $(PRJ_PATH)/SNMP

#DDNS
DDNS-$(CONFIG_DDNS) := $(PRJ_PATH)/DDNS

#NTP
NTP-$(CONFIG_NTP) := $(PRJ_PATH)/NTP

#TFTP
TFTP-$(CONFIG_TFTP) := $(PRJ_PATH)/TFTP

#UPNP
UPNP-$(CONFIG_UPNP) := $(PRJ_PATH)/UPNP

#LLDP
LLDP-$(CONFIG_LLDP) := $(PRJ_PATH)/LLDP

#NW_FIREWALL
NW_FIREWALL-$(CONFIG_NW_FIREWALL) := $(PRJ_PATH)/NW_FIREWALL
#busybox
BUSYBOX-y = $(PRJ_PATH)/busybox/

#DHCPV6
ifneq ($(BOARD), YARKON_EVB)
DHCPV6-y = $(PRJ_PATH)/DHCPV6/
endif

JAVAAP-y = $(PRJ_PATH)/JavaAP/

#MDINFO
MDINFO-y = $(PRJ_PATH)/PKConfig/MDInfo

#SMASH
SMASH-$(CONFIG_SMASH) := $(PRJ_PATH)/SMASH/
ifeq (y,$(CONFIG_SMASH))
CONFIG_SFCC = y
CONFIG_SFCB = y
endif

#SFCC
SFCC-$(CONFIG_SFCC) := $(PRJ_PATH)/SFCC

#SFCB
SFCB-$(CONFIG_SFCB) := $(PRJ_PATH)/SFCB
#Tools
TOOLS-y := Tools/


OTHER_FOLDERS_FOR_EXTRACT := $(PRJ_PATH)/MKIMG_Tool/ $(PRJ_PATH)/PKConfig/ $(PRJ_PATH)/Project_File/

#Build Kernel,Bootloader,IPMI firmware and Common libary 
FW_ELEMENT_DIR = $(TOOLS-y) \
		 $(KERNEL-y)\
 		 $(LIBMQUEUE-y) \
		 $(LIBXML2-y) \
		 $(OPENSSL-y) \
		 $(OPENLDAP-y) \
		 $(IPMI-y) \
		 $(vUSB_MODULE-y) \
		 $(IKVM_MODULE-y) \
		 $(STATE_MACHINE-y) \
		 $(UTILS-y) \
		 $(CHASSIS_MANAGER-y)

#Build or Install the following packages under Filesystem
#The order of $(JAVAAP-y) must put before $(WEB_PAGE-y)
FS_ELEMENT_DIR = $(BUSYBOX-y) \
		 $(DHCPV6-y) \
		 $(WEB_SERVER-y) \
		 $(SNMP-y) \
		 $(SFCC-y) \
		 $(SFCB-y) \
		 $(OPENWSMAN-y) \
		 $(SMASH-y) \
		 $(SMTP-y) \
		 $(DDNS-y) \
		 $(NTP-y) \
		 $(TFTP-y) \
		 $(UPNP-y) \
		 $(LLDP-y) \
		 $(NW_FIREWALL-y) \
		 $(JAVAAP-y) \
		 $(WEB_PAGE-y) \
		 $(MDINFO-y) 

export  FW_ELEMENT_DIR FS_ELEMENT_DIR FS-y OUTPUT_IMG-y
LxSYSTEM=$(addprefix _lxSYSTEM_,$(FW_ELEMENT_DIR))
################################################################################
.PHONY: $(LxSYSTEM) PreBuildTask STRIPOBJS

all: PreBuildTask $(LxSYSTEM)
ifeq ($(STRIP_OPTION),1)
	$(Q)$(MAKE) STRIPOBJS
endif

STRIPOBJS:
	$(Q)echo "Strip all objects......"
	$(call STRIPOBJS_INFOLDER, $(PRJ_PATH)/bin/*  $(PRJ_PATH)/lib/* )

PreBuildTask:
	$(Q)if [ ! -d $(CROSS_COMPILE_BIN_DIR) ]; then \
	cd $(PRJ_PATH)/ToolChain/Host/$(MYMCU); tar zxf $(TOOLCHAIN_ZIP); fi
	$(Q)if [ ! -d $(X86_CROSS_COMPILE_BIN_DIR) ]; then \
	cd $(PRJ_PATH)/ToolChain/Host/i386; tar zxf $(X86_TOOLCHAIN_ZIP); fi
	$(Q)if [ ! -d $(BINPATH) ];then  mkdir $(BINPATH); fi
	$(Q)if [ ! -d $(MODULEPATH) ];then  mkdir $(MODULEPATH); fi
	$(Q)if [ ! -d $(LIBPATH) ];then  mkdir $(LIBPATH); fi
	$(Q)if [ ! -d $(IMAGE_PATH) ];then  mkdir $(IMAGE_PATH); fi
$(LxSYSTEM):
ifeq  ($(BOARD_DEF),1)
	$(Q)cp Project_File/OS/Linux/Host/$(MYMCU)/Board/$(BOARD)/Projectfile .config
	$(Q)./conf -o PKConfig/Lx_Config/Config_all.in >>/dev/null 
	$(Q)cp .tmpconfig.h autoconfig.h
endif
	$(Q)if [ ! -f .config ] ;then echo "Please execute \"make menuconfig\"!!!!" ;exit 1;fi
	$(Q)$(MAKE) CXX="$(CXX)" CC="$(CC)" LD="$(LD)" AR="$(AR)" NM="$(NM)" OBJCOPY="$(OBJCOPY)" OBJDUMP="$(OBJDUMP)" RANLIB="$(RANLIB)" -C "$(patsubst _lxSYSTEM_%,%,$@)"
	
	
################################################################################
install:
	$(Q)cd $(KERNEL-y); make install
	$(Q)cd $(FS-y); make install

-include ./SDKPath	
SDK_FOLDER_PATH=$(strip $(EXTRPATH_tmp))

clean: 
	echo "make clean disabled, it removes important source files"
#	$(Q)for i in $(FW_ELEMENT_DIR); do (cd $$i && make CC=$(CC) clean); done
#	$(Q)rm -rf $(BINPATH) $(LIBPATH) $(IMAGE_PATH);
#	$(Q)rm -rf .config autoconfig.h .tmpconfig.h  
#	$(Q)if [ ! -z $(SDK_FOLDER_PATH) ];then rm -rf $(SDK_FOLDER_PATH); echo "Delete SDK Folder : $(SDK_FOLDER_PATH)";fi
	
clean_all: 
	$(Q)for i in $(FW_ELEMENT_DIR); do (cd $$i && make CC=$(CC) clean); done
	$(Q)cd $(FS-y); make clean
	$(Q)rm -rf $(BINPATH) $(LIBPATH) $(IMAGE_PATH);
	$(Q)cd $(OUTPUT_IMG-y); make clean
	$(Q)cd $(TOOLS-y);make clean
#	$(Q)make -C $(PRJ_PATH)/ToolChain/ clean;
	$(Q)rm -rf .config autoconfig.h .tmpconfig.h
	$(Q)if [ ! -z $(SDK_FOLDER_PATH) ];then rm -rf $(SDK_FOLDER_PATH); echo "Delete SDK Folder : $(SDK_FOLDER_PATH)";fi
	$(Q)find ./ -name ".tmp_versions"|xargs rm -rf && $(ECHO) "Delete Gabage files!"
	$(Q)find ./ -name "*.bak"|xargs rm -rf

include PKConfig/Lx_Script/Makefile.lib
ROOT_MODULE = $(call FIND_PATH,Makefile)

menuconfig:
	$(Q)if [ -f mconf ];then if [ ! -x mconf ];then chmod 755 mconf;fi;else echo "Lose mconf file";exit 1;fi
	$(Q)if [ -f PKConfig/Lx_Config/Config_all.in ];then ./mconf PKConfig/Lx_Config/Config_all.in;else echo "Lose Config file";fi
	$(Q)cp .tmpconfig.h autoconfig.h
#project release

include .release
#Extractor
include PKConfig/Lx_Script/ExtractRoot.mk
