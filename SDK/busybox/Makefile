BUSY_SRCDIR = $(PRJ_PATH)/busybox/

DTKBUSYBOX_DIR = $(DTKPATH)/busybox

CONFIG_FILE_NAME = Host/$(MYMCU)/config

override CFLAGS = 
override LDFLAGS =

#================= SDK param  ===================
SDK_FEATURE_NAME = busybox
SDK_FILE_CP_LIST = ./Makefile ./busybox*.tar.gz ./Host/$(MYMCU)/config patch/decompress_uncompress.patch
#================= end of SDK param ===================

all:
	$(Q)echo "******************** build busybox ********************"
	#copy busybox
	$(Q)cp $(CONFIG_FILE_NAME) busybox/.config
	if [ ! -f $(BUSY_SRCDIR)/busybox/decompress_uncompress.patch ] ;then \
	cp $(BUSY_SRCDIR)/patch/* $(BUSY_SRCDIR)/busybox;cd $(BUSY_SRCDIR)/busybox ;\
	patch -p1 < decompress_uncompress.patch;\
	fi
	$(Q)cd busybox; make > ../busybox.log 2>&1; make install >> ../busybox.log 2>&1 ; cp -a _install/*  $(FS_PATH)/; 

CLEAN_FILES = busybox/ busybox.log
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
