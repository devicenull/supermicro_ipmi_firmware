.PHONY: extract_basic 
OTHER_FOLDERS_FOR_EXTRACT := $(PRJ_PATH)/MKIMG_Tool/ $(PRJ_PATH)/PKConfig/ $(PRJ_PATH)/Project_File/
-include $(PRJ_PATH)/.config
include  $(PRJ_PATH)/.machine
-include $(PRJ_PATH)/.SDKPath
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

WEBSTYLE_EVB = ATEN_NEW
WEBSTYLE_PATTERN = $(shell echo $(BOARD) | awk -F_ '{ print $$1}')

extract_basic:
	$(Q)$(ECHO_CMD) " SDK Extraction Procedure "
	$(Q)for i in $(FW_ELEMENT_DIR) $(FS-y) $(FS_ELEMENT_DIR)   ;do (cd $$i && $(MAKE) extract);done
	$(Q)for i in $(OTHER_FOLDERS_FOR_EXTRACT);do (cd $$i && $(MAKE) extract);done
	$(Q)$(CP_CMD) -f ./conf $(SDKPATH)/
	$(Q)$(CP_CMD) -f ./mconf $(SDKPATH)/
	$(Q)$(CP_CMD) -f ./ProjectConfig-$(MYMCU) $(SDKPATH)/
	$(Q)$(CP_CMD) -f ./ProjectConfig-JavaAP $(SDKPATH)/
	$(Q)$(CP_CMD) -f ./ProjectConfig-x86* $(SDKPATH)/
	$(Q)$(CP_CMD) -f ./.machine $(SDKPATH)/ 
	$(Q)$(CP_CMD) -f ./.include $(SDKPATH)/
	$(Q)$(CP_CMD) -f ./.release $(SDKPATH)/
	$(Q)$(CP_CMD) -af ./Makefile $(SDKPATH)/Makefile
#remove other soc chips and append "endchoice" before "endmenu"
	$(Q)$(SED_CMD) -e '/source/,/endchoice/{/$(MYMCU)/I!d}'  \
	-e '/endmenu/i\endchoice' -i $(SDKPATH)/PKConfig/Lx_Config/Config_mcu.in \
	&& $(ECHO_CMD) " Sed this file Config_mcu.in"

	$(Q)$(SED_CMD) -e '/source/,/endchoice/{/$(BOARD)/I!d}'  \
	-e '/endmenu/i\endchoice' -i $(SDKPATH)/PKConfig/Lx_Config/Config_plat.in \
	&& $(ECHO_CMD) " Sed this file Config_plat.in"

#remove other platform and soc chip files
	$(Q)$(FIND_CMD) $(SDKPATH)/PKConfig/Lx_Config/MCU_Config/*.in  ! -iname "*$(MYMCU)*" | xargs rm -rf
	$(Q)$(FIND_CMD) $(SDKPATH)/PKConfig/Lx_Config/Plat_Config/*.in  ! -iname "*$(BOARD)*" | xargs rm -rf
ifeq ($(shell echo $(BOARD) | awk -F_ '{ print $$2}'),EVB)
#IF release the AST2400EVB, add the supermicro webstyle
ifeq ($(MYMCU),AST2400)
	$(Q)$(FIND_CMD) $(SDKPATH)/PKConfig/Lx_Config/Web_Config/*.in  ! -iname "*$(WEBSTYLE_EVB)*" -and ! -iname "*SUPERMICRO_2010*" | xargs rm -rf
else
	$(Q)$(FIND_CMD) $(SDKPATH)/PKConfig/Lx_Config/Web_Config/*.in  ! -iname "*$(WEBSTYLE_EVB)*" | xargs rm -rf
endif
else
	$(Q)$(FIND_CMD) $(SDKPATH)/PKConfig/Lx_Config/Web_Config/*.in  ! -iname "*$(WEBSTYLE_PATTERN)*" | xargs rm -rf
endif

#remove other web styles 
	$(Q)cat $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in
ifeq ($(shell echo $(BOARD) | awk -F_ '{ print $$2}'),EVB)
ifeq ($(MYMCU),AST2400)
	$(Q)$(SED_CMD) -e '/source/,/endchoice/{/$(WEBSTYLE_EVB)/I!d}'  \
	-e '/endmenu/i\endchoice'  -i $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in
	$(Q)$(SED_CMD) -e '/endchoice/i\source PKConfig/Lx_Config/Web_Config/Config_Supermicro_2010.in'  -i $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in
	$(Q)cat $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in
else
	$(Q)$(SED_CMD) -e '/source/,/endchoice/{/$(WEBSTYLE_EVB)/I!d}'  \
	-e '/endmenu/i\endchoice'  -i $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in\
	&& $(ECHO_CMD) " Sed this file Config_web_style.in"	
	$(Q)cat $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in
endif
else
	$(Q)$(SED_CMD) -e '/source/,/endchoice/{/$(WEBSTYLE_PATTERN)/I!d}'  \
	-e '/endmenu/i\endchoice'  -i $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in\
	&& $(ECHO_CMD) " Sed this file Config_web_style.in"	
	$(Q)cat $(SDKPATH)/PKConfig/Lx_Config/Config_web_style.in
endif

#remove dummy files
	$(Q)$(FIND_CMD) $(SDKPATH)  -name ".metadata" | xargs rm -rf