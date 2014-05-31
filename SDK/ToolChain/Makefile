TC_MAINDIR := $(PRJ_PATH)/ToolChain
TC_SRCDIR := $(TC_MAINDIR)/Host/$(MYMCU)/gcc-3.4.4-glibc-2.3.5-armv4
TC_HOST_TOOLS:= $(TC_MAINDIR)/Host/i386/mingw32 $(TC_MAINDIR)/Host/x86_64/mingw-w64

all:

install:


CLEAN_FILES = $(TC_SRCDIR) $(TC_HOST_TOOLS)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

#================= SDK param  ===================
SDK_FEATURE_NAME = ToolChain
SDK_FILE_CP_LIST = ./Makefile ./*.tar.gz ./Host/$(MYMCU)/*.tar.gz ./Host/i386/*.tar.gz ./Host/x86_64/*.tar.bz2
#================= end of SDK param ===================


include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
