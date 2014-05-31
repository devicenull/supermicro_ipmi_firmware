include $(PRJ_PATH)/.config

# Library
IPMI_LIB-y := rm_utilities     
IPMI_LIB-y += IPMI_CMDS
IPMI_LIB-y += COMMON
IPMI_LIB-y += CRYPT
IPMI_LIB-y += RTOS/Linux/
IPMI_LIB-y += RTOS/Linux/lan/
IPMI_LIB-y += NVRAM
IPMI_LIB-$(CONFIG_GPIO_DRV) += GPIO_DRV/OS/Linux
IPMI_LIB-y += UpdateMode
IPMI_LIB-y += MSG_CHANNEL
IPMI_LIB-y += LIB_CGI
IPMI_LIB-$(CONFIG_WDT_DRV) += WDT/OS/Linux
ifeq ($(MYMCU), HERMON)
IPMI_LIB-$(CONFIG_PWM_DRV)  += PWM_DRV/OS/Linux
endif

# Channel
IPMI_CHANNEL-$(CONFIG_CHNL_LAN) +=  MSG_CHANNEL/LAN/
IPMI_CHANNEL-$(CONFIG_CHNL_KCS) +=  MSG_CHANNEL/KCS/OS/Linux 
IPMI_CHANNEL-$(CONFIG_CHNL_UART) += MSG_CHANNEL/UART/OS/Linux
ifneq ($(BOARD), YARKON_EVB)
IPMI_CHANNEL-$(CONFIG_CHNL_IPMB) += MSG_CHANNEL/IPMB/OS/Linux 
endif
# Module
IPMI_MODULE-y := RTOS/Linux/timer
IPMI_MODULE-y += SLAVE_TASK
IPMI_MODULE-y += SENSOR
IPMI_MODULE-y += EVENT
IPMI_MODULE-y += PEF
IPMI_MODULE-$(CONFIG_MODULE_SOL) += SOL
IPMI_MODULE-y += SYS_BOOT/OS/Linux/
IPMI_MODULE-$(CONFIG_WDT_DRV) += WDT/OS/Linux
IPMI_MODULE-y += CALFWCRC
IPMI_MODULE-y += BTL_UPDATE
ifeq ($(MYMCU), HERMON)
IPMI_MODULE-y += GPIO_TASK
IPMI_MODULE-y += Debug_DRV
endif
ifeq ($(MYMCU), YARKON)
IPMI_MODULE-y += GPIO_TASK
endif
#for extractor
#ifdef CONFIG_EXTRACT_MOD
#-include $(PRJ_PATH)/.machine
#endif

# Driver
IPMI_DRIVER-$(CONFIG_I2C_DRV) := ./I2C_DRV/OS/Linux/Host/$(MYMCU)/driver  
IPMI_DRIVER-$(CONFIG_UART_DRV) +=  ./MSG_CHANNEL/UART/OS/Linux/Host/$(MYMCU)/ 
IPMI_DRIVER-$(CONFIG_KCS_DRV)  += ./MSG_CHANNEL/KCS/OS/Linux/Host/$(MYMCU)/
IPMI_DRIVER-$(CONFIG_GPIO_DRV) += GPIO_DRV/OS/Linux/Host/$(MYMCU)
IPMI_DRIVER-$(CONFIG_ADC_DRV) += ADC_DRV/OS/Linux/Host/$(MYMCU)
IPMI_DRIVER-$(CONFIG_WDT_DRV) += WDT/OS/Linux/Host/$(MYMCU)

ifeq ($(MYMCU), HERMON)
IPMI_DRIVER-y += NIC_DRV/OS/Linux/Host/$(MYMCU)/  
IPMI_DRIVER-y += UpdateMode/bios_flash_drv/OS/Linux/Host/$(MYMCU)
endif

ifeq ($(MYMCU), HERMON)
IPMI_DRIVER-$(CONFIG_PECI_DRV) += PECI_DRV/OS/Linux/Host/$(MYMCU)/driver 
IPMI_DRIVER-$(CONFIG_PWM_DRV) += PWM_DRV/OS/Linux/Host/$(MYMCU)
IPMI_DRIVER-$(CONFIG_TECHOMETER_DRV) += TECHOMETER_DRV/OS/Linux/Host/$(MYMCU)
IPMI_DRIVER-y += Debug_DRV/OS/Linux/Host/$(MYMCU)
endif

ifeq ($(MYMCU), YARKON)
IPMI_DRIVER-$(CONFIG_PECI_DRV) += PECI_DRV/OS/Linux/Host/$(MYMCU)/driver 
endif

CFLAGS+=  -D_FORTIFY_SOURCE=2 -O2 -Wformat -Wformat-security 

LDFLAGS+= -Wl,-z,relro -Wl,-z,now
OBJDIR = $(IPMI_CHANNEL-y)\
         $(IPMI_MODULE-y) \
         $(IPMI_DRIVER-y)\

LIBDIR = $(IPMI_LIB-y)\
	 $(IPMI_CHANNEL-y)\
	 $(IPMI_MODULE-y)
	
#default SDK files
EXCLUDEOBJS += $(strip $(subst ", ,$(CONFIG_EXCLUDE_OBJS)))
EXCLUDEOBJS += OEMInit.o  OEMPEFAction.o  PEF_EntryTable.o  Plt_Config.o  Plt_MiscAPI.o ProbeInfoTable.o ChanlTbl.o \
	       SOLHWInfo.o OEMCmd.o OEMCmdTbl.o ChassisDrv.o nvramtable.o SerMuxDrv.o I2CMUXDrv.o
export EXCLUDEOBJS

IPMISYSTEM = $(addprefix _IPMISYSTEM_,$(OBJDIR))
################################################################################
all:$(IPMILIB) $(IPMISYSTEM)

.PHONY: $(IPMISYSTEM) $(IPMILIB) extract
$(IPMILIB):
	$(Q)$(MAKE) lib CC=$(CC) -C "$(patsubst _IPMILIB_%,%,$@)"
$(IPMISYSTEM):
	$(Q)$(MAKE) CC=$(CC) -C "$(patsubst _IPMISYSTEM_%,%,$@)"

################################################################################
install:
#copy dynamical library
#	@if [ $(BUILD_OPTION) -eq 1 ];then cp $(PRJ_PATH)/lib/*.so $(FS_PATH)/lib/ ;fi


CLEAN_PATTERN = '*.o' '*.d' '*~' 'core' '.depend' '.*.cmd' '*.ko' '*.mod.c' '.tmp_versions' '*.symvers' 'modules.order'
CLEAN_FILES = $(BINPATH) $(LIBPATH) $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),./,,)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

ifeq ($(CONFIG_EXTRACT_ALL_OBJ),y)
include $(PRJ_PATH)/.SDKPath
DRVMODS = $(foreach i,$(IPMI_DRIVER-y),$(wildcard  $(i)/*.ko))
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
HEADER_FILES = $(shell find ./ -name "*.h") 
#HEADER_FILES = WDT/OS/Linux/Host/HERMON/inc/wdt_drv.h
define CP_TASK1
	echo $(2)
endef 
extract:
	@echo "************************ extract IPMI ************************"
	$(Q)$(call CP_TASK,$(PRJ_PATH)/lib/*,$(SDKPATH)/lib/)
	$(Q)$(call CP_TASK,$(PRJ_PATH)/IPMI/Makefile,$(SDKPATH)/IPMI/)
	$(Q)for d in $(IPMI_MODULE-y) $(IPMI_CHANNEL-y) $(IPMI_DRIVER-y);do echo "Jack " $$d; (cd $$d && make extract);done
	@echo "************************ extract header.sh ************************"
	$(Q)sh  $(PRJ_PATH)/PKConfig/Lx_Script/cp_header.sh $(SDKPATH)/IPMI
	@echo "************************ extract IPMI DONE ************************"
else
DEFAULT_PATTERN += '*.inc' '*.txt'
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
endif


clean_ipmi:
	$(Q)rm -f `find . -name *.o`; rm -f `find . -name *.d`;


