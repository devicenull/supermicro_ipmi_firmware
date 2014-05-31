# called from kernel build system: just declare what our modules are
obj-m := usb_hid.o
usb_hid-objs := wpcm450_hid.o wpcm450_dev.o wpcm450_vusbhs_dev.o wpcm450_usbprv.o
ifeq ($(KERNELRELEASE),)

else

override LDFLAGS=
default:
	$(Q)if [ ! -z "$(wildcard *.c)" ];then $(MAKE) -C $(KERNELDIR) M=$(PWD) modules;fi
	$(Q)if [ -d ${MODULEPATH} ];then cp $(obj-m:.o=.ko) ${MODULEPATH}; fi	

endif
include $(PRJ_PATH)/PKConfig/Lx_Script/DrvTemplate.mk
