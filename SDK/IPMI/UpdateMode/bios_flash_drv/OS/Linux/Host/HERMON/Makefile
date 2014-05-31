
OBJNAME-$(CONFIG_SPI_FLASH) = spiflash_bios.ko


obj-m := spiflash_bios.o
ifneq ($(KERNELRELEASE),)
CFLAGS += -D__KERNEL__ -DMODULE -DDEBUG -I$(KERNELDIR)/include -Wall -DEXPORT_SYMTAB -O2
else
override LDFLAGS=
default:
	$(Q)if [ ! -z "$(wildcard *.c)" ];then $(MAKE) -C $(KERNELDIR) M=$(PWD) modules;fi
	$(Q)if [ -d ${MODULEPATH} ];then cp $(obj-m:.o=.ko) ${MODULEPATH}; fi	

endif
include $(PRJ_PATH)/PKConfig/Lx_Script/DrvTemplate.mk
