include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

#extract objects to the specific folder
-include $(PRJ_PATH)/.SDKPath
SDK_SRCS =$(if $(EXTRACTSRC),$(SDKSRCS) $(SDKSRHS),$(SDKOBJS)) 
CONFIGFILE = $(SDKPATH)/MODConfig.in
#OBJS_PATH = $(strip $(EXTRPATH)/$(FOLDER))
CONFIGNAME = $(strip $(subst /,_,CONFIG_$(FOLDER)))
ifndef MAKETEMPLATE 
MAKETEMPLATE = Makefile.objs
endif

FOLDER_TOKEN = $(subst /, ,$(FOLDER))
TOKEN_NUM = $(words $(FOLDER_TOKEN))
SDK_FOLDER =  $(word 1,$(FOLDER_TOKEN))
MAIN_FOLDER = $(word 2,$(FOLDER_TOKEN))
FUNC_FOLDER = $(subst $(SDK_FOLDER)/$(MAIN_FOLDER)/,,$(FOLDER))

ifeq  ($(TOKEN_NUM),2)
OBJS_PATH =$(strip  $(SDKPATH)/$(MAIN_FOLDER))
else
OBJS_PATH =$(strip  $(SDKPATH)/$(MAIN_FOLDER)/$(FUNC_FOLDER))
endif
extract:
	@$(ADDOBJLIST)
	@echo "OBJS_PATH = $(OBJS_PATH), MAIN_FOLDER = $(MAIN_FOLDER), Func_Folder = $(FUNC_FOLDER)"
	@if [ $(TOKEN_NUM) -gt 2 ];then \
	if [ -f $(SDKPATH)/$(MAIN_FOLDER)/config.in ];then \
	echo "IPMI_BUILD-y += $(FUNC_FOLDER)" >> $(SDKPATH)/$(MAIN_FOLDER)/config.in; \
	else echo "IPMI_BUILD-y := $(FUNC_FOLDER)" > $(SDKPATH)/$(MAIN_FOLDER)/config.in;fi \
	;fi

