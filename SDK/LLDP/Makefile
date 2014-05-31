
ifeq ($(MYMCU),AST2050)
BUILD_HOST = arm-aspeed-linux-gnu
else
BUILD_HOST = arm-linux
endif

LLDP_VERSION = openlldp-0.3alpha
LLDP_SRCDIR = $(PRJ_PATH)/LLDP
LLDP_LIBDIR = $(PRJ_PATH)/LLDP/$(LLDP_VERSION)

export	LLDP_LIBDIR

override CFLAGS = 
override LDFLAGS =
override LIBS =
override INCLUDEDIR =

all:
	$(Q)echo "******************** build LLDP Agent ********************"
	cd $(LLDP_LIBDIR)
	./configure --host=$(BUILD_HOST); make; fi

install:
	$(Q)echo "********************Copy LLDP Agent ********************"
	$(Q)cp -f $(LLDP_LIBDIR)/src/lldpd $(FS_PATH)/bin/

	
CLEAN_FILES = $(LLDP_LIBDIR)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
	
