include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
-include $(PRJ_PATH)/.SDKPath

include $(PRJ_PATH)/.config
include $(PRJ_PATH)/PKConfig/Lx_Script/ExtractLIB.mk

.PHONY: extract

extract:
ifneq ($(SDK_FEATURE_NAME),)
	@make extract_specific
else
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
endif

