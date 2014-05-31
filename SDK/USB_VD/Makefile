-include $(PRJ_PATH)/.SDKPath
-include $(PRJ_PATH)/.config
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

USB_DIR  = OS/Linux/Host/$(MYMCU)/ OS/Linux

all:
	$(Q)$(MAKE) -C OS/Linux lib
	$(Q)for i in $(USB_DIR); do (cd $$i && $(MAKE)); done
install:

clean:
	$(Q)for i in $(USB_DIR); do (cd $$i && $(MAKE) clean); done

extract:
ifeq ($(CONFIG_EXTRACT_ALL_SRC),y)
	$(Q)$(call SDK_CP_TASK, inc/*.h, $(SDKPATH)/USB_VD/)
endif 
	$(Q)$(call CP_TASK,./Makefile, $(SDKPATH)/USB_VD/)
	$(Q)for i in $(USB_DIR); do (cd $$i && $(MAKE) extract); done
ifeq ($(CONFIG_EXTRACT_ALL_OBJ),y)	
	$(Q)$(call SED_TASK,'/LIBOBJS/d',$(SDKPATH)/USB_VD/OS/Linux/Makefile)
	$(Q)$(call SED_TASK,'/CP_PLT_CFG/d',$(SDKPATH)/USB_VD/OS/Linux/Host/$(MYMCU)/Makefile)
endif