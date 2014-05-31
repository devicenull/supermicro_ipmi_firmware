-include $(PRJ_PATH)/.SDKPath
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
.PHONY: extract_config extract_configall extract extract_mix extract_clean
CONFIGIN_PATH = $(PRJ_PATH)/PKConfig/Lx_Config/

extract_config:
	$(Q)if [ -f .config ];then echo "please make extract_clean!!"; exit 1; fi
	$(Q)if [ -f mconf ];then if [ ! -x mconf ];then chmod 755 mconf;fi;else echo "Lose mconf file";exit 1;fi
	$(Q)if [ -f PKConfig/Lx_Config/Config_Main.in ];then ./mconf PKConfig/Lx_Config/Config_Main.in;else echo "Lose Config file(1)";fi
#add comment in order to choose multiple web styles
	$(Q)$(SED_CMD) -e 's/\(^choice\)/#\1/'  -e 's/\(prompt\)/#\1/'  -e 's/\(default\)/#\1/' \
	-e 's/\(endchoice\)/#\1/' -i $(CONFIGIN_PATH)/Config_web_style.in
	$(Q)make extract_configall
	$(call SDKPATH_CMD)

extract_configall:
	$(Q)if [ ! -f ./Project_File/OS/Linux/Host/$(MYMCU)/Board/$(BOARD)/Projectfile ];then echo "Can't found the config file." ; exit 1 ; fi
	$(Q)cp -f ./Project_File/OS/Linux/Host/$(MYMCU)/Board/$(BOARD)/Projectfile .config
	$(Q)if [ -f PKConfig/Lx_Config/Config_Extr.in ];then ./mconf PKConfig/Lx_Config/Config_Extr.in;else echo "Lose Config file(2)";fi
	$(Q)if [ ! -f .config ] ;then exit 1 ;fi
	$(Q)cp .tmpconfig.h autoconfig.h
#remove comment 
	$(Q)$(call SED_TASK,'s/#\(.*$$\)/\1/g',$(CONFIGIN_PATH)/Config_web_style.in)
	
ifeq ($(CONFIG_EXTRACT_ALL_SRC),y)
MKFILE = $(PRJ_PATH)/PKConfig/Lx_Script/ExtractRootAllSrc.mk
CONFINFILE =  $(CONFIGIN_PATH)/Extr_Config/Config_Extr_Allsrc.in
endif
ifeq ($(CONFIG_EXTRACT_ALL_OBJ),y)
MKFILE = $(PRJ_PATH)/PKConfig/Lx_Script/ExtractRootAllObj.mk
endif
ifeq ($(CONFIG_EXTRACT_SRC_AND_OBJ),y)
CONFINFILE =  $(CONFIGIN_PATH)/Extr_Config/Config_Extr_Mix.in
endif

extract_mix:
	$(Q)echo "extract src and obj................"

extract:
	$(Q)cp .config config_extractor
	$(Q)make extract -f $(MKFILE)
	$(Q)$(CP_CMD) -af $(PRJ_PATH)/JavaAP/Makefile.GPL $(SDKPATH)/JavaAP/Makefile
	$(call RM_TASK,$(subst $(PRJ_PATH),$(SDKPATH),$(MKFILE)))
	$(call RM_TASK,$(subst $(PRJ_PATH),$(SDKPATH),$(CONFINFILE)))
	$(Q)$(ECHO_CMD) " SDK Extraction Complete "

extract_clean:
	$(Q)echo "extract clean................"
	$(if $(SDKPATH), $(call RM_TASK,$(SDKPATH)))
	$(call RM_TASK,.config config_extractor .SDKPath autoconfig.h .*.cmd .*.old .*.h)
