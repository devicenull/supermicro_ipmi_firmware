-include $(PRJ_PATH)/.SDKPath
#APP = IKVM_APP

all:

ifneq ($(APP), IKVM_APP) 
	$(MAKE) -C Driver/KM/OS/Linux/Host/$(MYMCU)
	$(MAKE) -C Driver/Video/OS/Linux/Host/$(MYMCU)
endif
	$(MAKE) -C IKVMServer CONF=Release
ifneq ($(APP), IKVM_APP)
	cp IKVMServer/dist/Release/Platform_GNU-Generic/ikvmserver ${PRJ_PATH}/bin
endif

clean:
	$(MAKE) -C IKVMServer CONF=Release clean
	$(MAKE) -C Driver/KM/OS/Linux/Host/$(MYMCU) clean
	$(MAKE) -C Driver/Video/OS/Linux/Host/$(MYMCU) clean  

install:

-include $(PRJ_PATH)/.config

ifneq ($(APP), IKVM_APP)
ifeq ($(CONFIG_EXTRACT_ALL_SRC),y)
DEFAULT_PATTERN += '*.xml' '*.mk'
OTHER_PATTERN =
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
else
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

extract:
	$(Q)$(ECHO_CMD) " Extract IKVM "
	$(call CP_TASK,./Makefile,$(SDKPATH)/IKVM)
	$(call SDK_CP_TASK,./ServerLib/dist/Release/Platform_GNU-Generic/libserverlib.a,$(SDKPATH)/IKVM)
	$(call SDK_CP_TASK,$(wildcard ./ServerLib/nbproject/*.mk),$(SDKPATH)/IKVM)
	$(call SDK_CP_TASK,./IKVMServer/build/Release/Platform_GNU-Generic/ServerEntry.o,$(SDKPATH)/IKVM)
	$(call SDK_CP_TASK,./IKVMServer/Makefile,$(SDKPATH)/IKVM)
	$(call SDK_CP_TASK,$(wildcard ./IKVMServer/nbproject/*.mk),$(SDKPATH)/IKVM)
#comment the variables in Makefile
	$(Q)$(call SED_TASK,'s/\(BUILD_SUBPROJECTS_.*=\)/#\1/',$(SDKPATH)/IKVM/IKVMServer/nbproject/Makefile-impl.mk)
	$(Q)$(call SED_TASK,'s/\(OBJECTFILES*.=\)/#\1/',$(SDKPATH)/IKVM/IKVMServer/nbproject/Makefile-Release.mk)
	$(Q)$(MAKE) extract -C Driver/KM/OS/Linux/Host/$(MYMCU) 
	$(Q)$(MAKE) extract -C Driver/Video/OS/Linux/Host/$(MYMCU)
endif
endif