obj-m := reg_op_drv.o
ifneq ($(KERNELRELEASE),)
	CFLAGS += -D__KERNEL__ -DMODULE -I$(KERNELDIR)/include -Wall -O2 
else

    override LDFLAGS=
default:
	$(Q)if [ ! -z "$(wildcard *.c)" ];then $(MAKE) -C $(KERNELDIR) M=$(PWD) modules;fi
	$(Q)if [ -d ${MODULEPATH} ];then cp $(obj-m:.o=.ko) ${MODULEPATH}; fi	

endif
include $(PRJ_PATH)/PKConfig/Lx_Script/DrvTemplate.mk
