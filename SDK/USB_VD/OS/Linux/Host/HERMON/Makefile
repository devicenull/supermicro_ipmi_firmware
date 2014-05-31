MODULENAME = ikvm_vmass.ko

obj-m := ikvm_vmass.o
ikvm_vmass-objs := wpcm450_vmass.o wpcm450_dev.o wpcm450_vusbhs_dev.o wpcm450_usbprv.o
ifneq ($(KERNELRELEASE),)
#	CFLAGS += -DDEBUG_2 -DDEBUG -D__KERNEL__ -DMODULE -I$(KERNELDIR)/include -Wall -DEXPORT_SYMTAB -O2 
else
    override LDFLAGS=
ifneq ($(BOARD),)
else
endif

default:
	$(Q)if [ ! -z "$(wildcard *.c)" ];then $(MAKE) -C $(KERNELDIR) M=$(PWD) modules;fi
	$(Q)if [ -d ${MODULEPATH} ];then cp $(obj-m:.o=.ko) ${MODULEPATH}; fi	
endif
include $(PRJ_PATH)/PKConfig/Lx_Script/DrvTemplate.mk
