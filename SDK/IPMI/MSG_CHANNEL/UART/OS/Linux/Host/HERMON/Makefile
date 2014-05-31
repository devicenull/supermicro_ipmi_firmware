obj-m := uart_drv.o
ifneq ($(KERNELRELEASE),)

CFLAGS += -D__KERNEL__ -DMODULE -I$(KERNELDIR)/include -Wall -DEXPORT_SYMTAB -O2 -I$(INCLUDEDIR)
CFLAGS += -DUART_FLOW_CONTROL

else
override LDFLAGS=
KBUILD_NOPEDANTIC=1
export KBUILD_NOPEDANTIC

default:
	$(Q)if [ ! -z "$(wildcard *.c)" ];then $(MAKE) -C $(KERNELDIR) M=$(PWD) modules;fi
	$(Q)if [ -d ${MODULEPATH} ];then cp $(obj-m:.o=.ko) ${MODULEPATH}; fi	

endif
include $(PRJ_PATH)/PKConfig/Lx_Script/DrvTemplate.mk
