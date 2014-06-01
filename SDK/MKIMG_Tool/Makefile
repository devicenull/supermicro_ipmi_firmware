-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

TOOL_PATH   = $(PRJ_PATH)/MKIMG_Tool/Host/$(MYMCU)/Board/$(BOARD)
WORK_PATH   = $(PRJ_PATH)/MKIMG_Tool/tmp
BOARD_PATH  = $(PRJ_PATH)/MKIMG_Tool/Host/$(MYMCU)/
SRC_PATH  = $(PRJ_PATH)/MKIMG_Tool


HOST_CC    = gcc 
CHECK_CC_RULE = $(shell if $(HOST_CC) $(1)  -S -o /dev/null -xc /dev/null  > /dev/null 2>&1 ;then echo $(1) ;fi)
HOST_CC_CFLAG    = $(call CHECK_CC_RULE,-fno-stack-protector)

DTKMKIMG_DIR = $(DTKPATH)/MKIMG_Tool

ifeq ($(CONFIG_PLATFORM_SUPERMICRO_SUBVERSION_X9),y)
BTL_FILE = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/Board/$(BOARD)/BTL_X9
else 
BTL_FILE = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/Board/$(BOARD)/BTL
endif 


#u-boot MYMCUTYPE 1
ifeq ($(MYMCU),YARKON)
MYMCUTYPE=2
endif
ifeq ($(MYMCU),AST2050)
MYMCUTYPE=1
endif
#bootloader MYMCUTYPE 0
ifeq ($(MYMCU),HERMON)
MYMCUTYPE=0
endif
FWVERSION=$(ver)
#IMAGE_PATH
install:
	$(Q)echo "HOSTCC mkbin"
	# no source code provided, mkbin will not compile under x86_64
#	$(Q)$(call CONDITION_HOSTCC,mkbin.c,$(HOST_CC_CFLAG), )

ifeq ($(MYMCU),AST2050)
	$(Q)cd $(IMAGE_PATH); mv uImage.bin kernel.bin;
endif
ifeq ($(MYMCU),HERMON)
	# no source code provided, mkimage will not compile under x86_64
	#$(Q)cd $(TOOL_PATH)/../..;$(call CONDITION_HOSTCC,mkimage.c,,) 
	$(Q)cd $(TOOL_PATH)/../..;cp -rf mkimage $(TOOL_PATH)
	cd $(IMAGE_PATH); mv image.bin kernel.bin;
endif
ifeq ($(BOARD),SuperMicro_X7SB3)
	$(Q)cd $(TOOL_PATH); chmod 744 ./BTL_diff_test.sh; \
	./BTL_diff_test.sh $(BTL_FILE) $(TOOL_PATH)/bootloader_pcb_rev_b.bin
endif
ifeq ($(MYMCU),YARKON)
	$(Q)cd $(IMAGE_PATH); mv uImage.bin kernel.bin;
endif
	$(Q)if [ -d $(WORK_PATH) ];then rm -rf $(WORK_PATH); fi
	$(Q)mkdir $(WORK_PATH);cp $(TOOL_PATH)/* $(WORK_PATH)/ -rf
	$(Q)cp $(IMAGE_PATH)/* $(WORK_PATH)/ -rf
	$(Q)cp $(PRJ_PATH)/MKIMG_Tool/mkbin $(WORK_PATH)/
	$(Q)cd ${WORK_PATH};sh ${WORK_PATH}/cmd_script.sh
	$(Q)if [ ! -z  $(IMAGE_PATH) ] && [ -d  $(IMAGE_PATH) ] ;then \
	rm -f $(IMAGE_PATH)/*; \
	cd ${WORK_PATH};cp *.bin $(IMAGE_PATH)/; fi
	$(Q)rm -rf $(WORK_PATH)

ifneq ($(CONFIG_EXTRACT_MOD),y)
ifeq ($(MYMCU),AST2050)
# except IP1500, ADLink, Nexcom
#ifneq ($(BOARD), $(filter $(BOARD),IP1500 ADLINK_CMM NEXCOM_NSB7110))
ifeq ($(BOARD),AST2050_EVB)
	$(Q)bash $(PRJ_PATH)/PKConfig/MDInfo/mdinfo.sh $(MYMCUTYPE) $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/Board/$(BOARD)
endif
endif
ifeq ($(MYMCU),HERMON)
	$(Q)bash $(PRJ_PATH)/PKConfig/MDInfo/mdinfo.sh $(MYMCUTYPE) $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/Board/$(BOARD)
endif
ifeq ($(MYMCU),AST2400)
	$(Q)bash $(PRJ_PATH)/PKConfig/MDInfo/mdinfo.sh $(MYMCUTYPE) $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/Board/$(BOARD)
endif
endif
CLEAN_PATTERN = '*.o'
CLEAN_FILES = $(WORK_PATH) $(TOOL_PATH)/mkimage \
            $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),$(SRC_PATH),,) \
            $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),$(BOARD_PATH),,)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

DEFAULT_PATTERN += '*.bin' '*.sh' '*.inf' '*.o'
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
