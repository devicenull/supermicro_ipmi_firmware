ifeq ($(CONFIG_EXTRACT_ALL_SRC),y)
DEFAULT_PATTERN += '*.c' '*.h' '*.cc' '*.H' '*.C' '*.cpp' '*.CPP' '*.in' '*.java' 'Makefile' '*.config' 
OTHER_PATTERN += 
else 
DEFAULT_PATTERN += '*.sh' '*.xml' '*.tar.gz'
OTHER_PATTERN +=
endif


CURRPATH = $(shell pwd)
SDK_TARGET = $(subst $(PRJ_PATH),$(SDKPATH),$(CURRPATH))


SEARCH_LIST = $(call GEN_SEARCH_LIST,$(DEFAULT_PATTERN),.,,$(OTHER_PATTERN))



.PHONY: extract_mix extract_allsrc extract_allobj extract_specific

extract_mix:
	$(Q)$(ECHO_CMD) "do extract_mix"

extract_allsrc:
	$(Q)$(ECHO_CMD) " Extract [SRC] $(CURRPATH)"
	$(Q)$(call SDK_CP_TASK, $(filter-out $(SDKPATH)%,$(sort $(SEARCH_LIST))), $(SDK_TARGET))

extract_allobj:
	$(Q)$(ECHO_CMD) " Extract [OBJ] $(CURRPATH)"
	$(Q)$(call CP_TASK, ./Makefile $(filter-out $(SDKPATH)%,$(filter-out $(LIBOBJS),$(OBJS))), $(SDK_TARGET))
	$(Q)$(call SDK_CP_TASK, $(filter-out $(SDKPATH)%,$(sort $(SEARCH_LIST))), $(SDK_TARGET))

SOURCE_PATTERN = %.c  %.cc  %.C %.cpp %.CPP %.java
extract_specific:
	$(Q)$(ECHO_CMD) " Extract $(SDK_FEATURE_NAME) "
	$(Q)$(call SDK_CP_TASK,$(if $(CONFIG_EXTRACT_ALL_SRC),$(SDK_FILE_CP_LIST), \
	$(filter-out $(SOURCE_PATTERN),$(SDK_FILE_CP_LIST:.c=.o))),$(SDKPATH)/$(SDK_FEATURE_NAME))
