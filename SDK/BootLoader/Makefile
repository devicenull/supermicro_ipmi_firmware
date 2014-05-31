

all:
	$(Q)echo "*************** Build Bootloader ***************"
ifneq ($(MYMCU),HERMON)
	$(Q)make -C ./Host/$(MYMCU)
endif

install:
	$(Q)echo "*************** Install Bootloader ***************"
ifneq ($(MYMCU),HERMON)
	$(Q)make -C ./Host/$(MYMCU) install
endif

ifneq ($(MYMCU),HERMON)
CLEAN_FILES = $(PRJ_PATH)/MKIMG_Tool/Host/$(MYMCU)/Board/$(BOARD)/u-boot.bin \
         $(filter-out $(wildcard Host/$(MYMCU)/u-boot*.tar.gz),$(wildcard Host/$(MYMCU)/u-boot*)) \
         Host/$(MYMCU)/build_tmp
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
endif
#================= SDK param  ===================
SDK_FEATURE_NAME = BootLoader
ifneq ($(MYMCU),HERMON)
SDK_FILE_CP_LIST = Host/$(MYMCU)/*.tar.gz Host/$(MYMCU)/Board/$(BOARD)/*.mk Host/$(MYMCU)/Makefile \
					./Makefile Host/$(MYMCU)/Board/$(BOARD)/*.zip Host/$(MYMCU)/Board/$(BOARD)/*.txt
else
SDK_FILE_CP_LIST = ./Makefile Host/$(MYMCU)/Board/$(BOARD)/*.txt
endif
#================= end of SDK param ===================
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk