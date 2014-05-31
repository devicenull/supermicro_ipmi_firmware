-include $(PRJ_PATH)/.SDKPath 

include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

OBJS_PATH =$(strip  $(SDKPATH)/$(MAIN_FOLDER)/$(FUNC_FOLDER))
PWD =$(shell pwd)

#----------------------- function for all source extraction mode

DEFAULT_PATTERN += '*.c' '*.h' '*.cc'
OTHER_PATTERN += 'Makefile' 'makefile' 'MAKEFILE' '*.gz' '*.tar' '*.in' '*.lib'

CURRPATH = $(shell pwd)
SDK_TARGET = $(subst $(PRJ_PATH),$(SDKPATH),$(CURRPATH))

SEARCH_LIST =

ifeq ($(CONFIG_EXTRACT_ALL_SRC),y)
SEARCH_LIST = $(call GEN_SEARCH_LIST,$(DEFAULT_PATTERN),.,,$(OTHER_PATTERN))
endif

#------------------------- end of function

include $(PRJ_PATH)/.config
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
.PHONY: extract extract_mix extract_allsrc extract_allobj clean
extract:
ifeq ($(CONFIG_EXTRACT_ALL_OBJ),y)
	@make extract_allobj
else
ifeq ($(CONFIG_EXTRACT_ALL_SRC),y)
	@make extract_allsrc
else
ifeq ($(CONFIG_EXTRACT_SRC_AND_OBJ),y)
	@make extract_mix
endif
endif
endif


extract_mix:
	@echo "do extract_mix"

extract_allsrc:
	$(Q)$(ECHO_CMD) " Extract [DRV]  $(CURRPATH) "	
	$(Q)$(call SDK_CP_TASK, $(filter-out $(SDKPATH)%,$(sort $(SEARCH_LIST))), $(SDK_TARGET))

extract_allobj:
	$(Q)$(Q)$(ECHO_CMD) " Extract [DRV]  $(CURRPATH) "
	$(Q)$(call CP_TASK ,$(filter-out $(SDKPATH)%,./Makefile $(obj-m:.o=.ko)),$(SDK_TARGET))
	
	
CLEAN_PATTERN = '*.o' '*.d' '*~' 'core' '.depend' '.*.cmd' '*.ko' '*.mod.c' '.tmp_versions' '*.symvers' '*.order'
CLEAN_FILES =  $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),./,,)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
