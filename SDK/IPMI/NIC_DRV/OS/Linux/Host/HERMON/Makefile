obj-m := eth_wpcm450mac.o
ifneq ($(KERNELRELEASE),)
	CFLAGS += -D__KERNEL__ -DMODULE -I$(KERNELDIR)/include -O2 -DBYPASS_RX_OVERFLOW
else

    override LDFLAGS=
modules:
	$(Q)if [ ! -z "$(wildcard *.c)" ];then $(MAKE) -C $(KERNELDIR) M=$(PWD) modules;fi
	$(Q)if [ -d ${MODULEPATH} ];then cp $(obj-m:.o=.ko) ${MODULEPATH}; fi


modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install


.PHONY: modules modules_install clean

endif
include $(PRJ_PATH)/PKConfig/Lx_Script/DrvTemplate.mk
