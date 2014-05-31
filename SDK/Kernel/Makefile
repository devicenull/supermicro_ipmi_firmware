-include $(PRJ_PATH)/.SDKPath
-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
KERNEL_SRCDIR = $(PRJ_PATH)/Kernel/Host/$(MYMCU)
KERNEL_ZIP = $(wildcard $(KERNEL_SRCDIR)/*.tar.gz)

DTKKERNEL_DIR = $(DTKPATH)/Kernel

ifeq ($(MYMCU),YARKON)
OBJCPY_ARG = .note.gnu.build-id
else
OBJCPY_ARG = .note
endif

override CFLAGS = 
override LDFLAGS =

all:
	$(Q)echo "Build Kernel ........"
	$(Q)cd $(KERNEL_SRCDIR);if [ ! -d $(KERNEL_SRCDIR)/linux ];then \
	mkdir linux;cd linux ;tar zxf $(KERNEL_ZIP); cp ../Board/$(BOARD)/config .config; cp ../Board/$(BOARD)/config ../;fi
ifeq ($(MYMCU),HERMON)
	$(Q)cp $(KERNEL_SRCDIR)/head.S $(KERNEL_SRCDIR)/linux/arch/arm/kernel
	if [ ! -f $(KERNEL_SRCDIR)/linux/.patched ] ;then \
	cd $(KERNEL_SRCDIR)/linux;\
	for i in  $(wildcard $(KERNEL_SRCDIR)/patch/*.patch);do patch -p1 < $$i;done;\
	touch .patched;\
	fi
endif
ifeq ($(BOARD),AST2050_EVB)
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/ftgmac100_26.c $(KERNEL_SRCDIR)/linux/drivers/net
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/ftgmac100_26.h $(KERNEL_SRCDIR)/linux/drivers/net
endif
ifeq ($(BOARD),IP1500)
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/config $(KERNEL_SRCDIR)/linux/.config
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/ftgmac100_26.c $(KERNEL_SRCDIR)/linux/drivers/net	
endif
ifeq ($(BOARD),NEXCOM_NSB7110)
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/config $(KERNEL_SRCDIR)/linux/.config
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/ftgmac100_26.c $(KERNEL_SRCDIR)/linux/drivers/net	
endif
ifeq ($(BOARD),ADLINK_CMM)
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/config $(KERNEL_SRCDIR)/linux/.config
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/ftgmac100_26.c $(KERNEL_SRCDIR)/linux/drivers/net	
endif

ifeq ($(BOARD),YARKON_EVB)
	cp $(KERNEL_SRCDIR)/Board/$(BOARD)/config $(KERNEL_SRCDIR)/linux/.config
endif
#Change Huawei Kernel Makefile toolchain version 
ifeq ($(BOARD),HW_STG2SMCA)
	cp -f $(KERNEL_SRCDIR)/Board/$(BOARD)/Makefile $(KERNEL_SRCDIR)/linux/
endif

	$(Q)cd $(KERNEL_SRCDIR);cd linux;make ARCH=arm;

install:
	$(Q)echo "Gen Kernel image....."	
	$(Q)cd $(KERNEL_SRCDIR)/linux;$(OBJCOPY) -O binary -R $(OBJCPY_ARG) \
	-R .comment -S vmlinux linux.bin;gzip -f -9 linux.bin
#	$(Q)cd $(KERNEL_SRCDIR)/linux;mkimage -A arm -O linux -T kernel \
#	-C gzip -a 40008000 -e 40008000 -d linux.bin.gz uImage
	$(Q)if [ ! -d $(IMAGE_PATH) ]; then \
	mkdir $(IMAGE_PATH); fi
ifneq ($(MYMCU),HERMON)
	$(Q)cp $(KERNEL_SRCDIR)/linux/linux.bin.gz $(IMAGE_PATH)/uImage.bin
else
	$(Q)cp $(KERNEL_SRCDIR)/image $(IMAGE_PATH)/image.bin
endif
	

CLEAN_FILES = $(KERNEL_SRCDIR)/linux $(KERNEL_SRCDIR)/image
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

#================= SDK param  ===================
SDK_FEATURE_NAME = Kernel
SDK_FILE_CP_LIST = Host/$(MYMCU)/config Host/$(MYMCU)/head.S Host/$(MYMCU)/*.tar.gz \
					./Makefile  Host/$(MYMCU)/*.c  Host/$(MYMCU)/*.h \
					 Host/$(MYMCU)/patch/* Host/$(MYMCU)/Board/$(BOARD)/*
#================= end of SDK param ===================
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
