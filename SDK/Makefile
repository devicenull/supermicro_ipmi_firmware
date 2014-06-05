-include .config
-include .machine
-include ./ProjectConfig-HERMON

CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
CXX=$(CROSS_COMPILE)g++
NM=$(CROSS_COMPILE)nm
RANLIB=$(CROSS_COMPILE)ranlib
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump 
STRIP = $(CROSS_COMPILE)strip
export CC LD AR CXX NM RANLIB OBJCOPY OBJDUMP STRIP CROSS_COMPILE

BASEDIR = $(shell pwd)
# The plan is to move all the source code to this directory, to prevent conflicts with make targets
SRCDIR = $(BASEDIR)/src
OUTPUT_PATH = $(BASEDIR)/output
FS_PATH = $(OUTPUT_PATH)/filesystem/
# legacy defines :(
PRJ_PATH = $(BASEDIR)
BINPATH = $(BASEDIR)/bin
MODULEPATH = $(BASEDIR)/bin/module
LIBPATH = $(BASEDIR)/lib
IMAGE_PATH = $(BASEDIR)/images
OUTPUT_IMG-y = $(BASEDIR)/MKIMG_Tool/
Q = @
export BASEDIR SRCDIR PRJ_PATH BINPATH LIBPATH IMAGE_PATH MODULEPATH OUTPUT_IMG-y Q FS_PATH

LIBS += -lcgi -lipmi -lchnl  -lm -lsol  -lutility -lnvram  -lipmicrypt -lcommon -lsys -lrt -luart -lupdate -lnet -lmqueue -lgpio -lxml2 -lldap_client -lradius_client -lwdt
LDFLAGS += -L$(BASEDIR)/OpenSSL/openssl/local/lib -L$(BASEDIR)/lib -lcrypto -lssl $(LIBS)
export LDFLAGS

MYMCU=HERMON
export MYMCU

all: outputdir kernel libxml2 openssl openldap ipmi vusb ikvm busybox dhcpv6 lighttpd sfcc sfcb openwsman smash smtp ddns ntp lldp nwfirewall webpage mdinfo

outputdir:
	mkdir -p  $(FS_PATH)/bin $(FS_PATH)/sbin $(FS_PATH)/lib $(FS_PATH)/etc/init.d

kernel:
	$(Q)echo
	$(Q)echo =================== kernel ===================
	$(Q)echo
	cd $(BASEDIR)/Kernel/Host/HERMON/linux && LDFLAGS="" make

libxml2:
	$(Q)echo
	$(Q)echo =================== libxml2 ===================
	$(Q)echo
	cd $(BASEDIR)/libXML2 && make

openssl:
	$(Q)echo
	$(Q)echo =================== openssl ===================
	$(Q)echo
	cd $(BASEDIR)/OpenSSL && make

openldap:
	$(Q)echo
	$(Q)echo =================== openldap ===================
	$(Q)echo
	cd $(BASEDIR)/OpenLDAP && make

ipmi:
	$(Q)echo
	$(Q)echo =================== ipmi ===================
	$(Q)echo
	cd $(BASEDIR)/IPMI && make

vusb:
	$(Q)echo
	$(Q)echo =================== vusb ===================
	$(Q)echo
	cd $(BASEDIR)/USB_VD && make

ikvm:
	$(Q)echo
	$(Q)echo =================== ikvm ===================
	$(Q)echo
	cd $(BASEDIR)/IKVM && make

busybox:
	$(Q)echo
	$(Q)echo =================== busybox ===================
	$(Q)echo
	cd $(BASEDIR)/src/busybox && make

dhcpv6:
	$(Q)echo
	$(Q)echo =================== dhcpv6 ===================
	$(Q)echo
	cd $(BASEDIR)/DHCPV6 && make

lighttpd:
	$(Q)echo
	$(Q)echo =================== lighttpd ===================
	$(Q)echo
	cd $(BASEDIR)/Web_Server/httpd && make

sfcc:
	$(Q)echo
	$(Q)echo =================== sfcc ===================
	$(Q)echo
	cd $(BASEDIR)/SFCC && make

sfcb:
	$(Q)echo
	$(Q)echo =================== sfcb ===================
	$(Q)echo
	cd $(BASEDIR)/SFCB && make

openwsman:
	$(Q)echo
	$(Q)echo =================== openwsman ===================
	$(Q)echo
	cd $(BASEDIR)/OpenWSMAN && make

smash:
	$(Q)echo
	$(Q)echo =================== smash ===================
	$(Q)echo
	cd $(BASEDIR)/SMASH && make

smtp:
	$(Q)echo
	$(Q)echo =================== smtp ===================
	$(Q)echo
	cd $(BASEDIR)/SMTP && make

ddns:
	$(Q)echo
	$(Q)echo =================== ddns ===================
	$(Q)echo
	cd $(BASEDIR)/DDNS && make

ntp:
	$(Q)echo
	$(Q)echo =================== ntp ===================
	$(Q)echo
	cd $(BASEDIR)/NTP && make

lldp:
	$(Q)echo
	$(Q)echo =================== lldp ===================
	$(Q)echo
	cd $(BASEDIR)/LLDP && make

nwfirewall:
	$(Q)echo
	$(Q)echo =================== nwfirewall ===================
	$(Q)echo
	cd $(BASEDIR)/NW_FIREWALL && make

webpage:
	$(Q)echo
	$(Q)echo =================== webpage ===================
	$(Q)echo
	cd $(BASEDIR)/Web_Server && make

mdinfo:
	$(Q)echo
	$(Q)echo =================== mdinfo ===================
	$(Q)echo
	cd $(BASEDIR)/PKConfig/MDInfo && make

# This really just generates the output image.  Called install for legacy reasons :)
install:
ifneq ($(shell id -u),0)
	$(error make install must be run as root)
endif
	$(Q)echo
	$(Q)echo =================== install ===================
	$(Q)echo
	cd $(BASEDIR)/Kernel/Host/HERMON/linux && LDFLAGS="" make install
	cd $(BASEDIR)/FileSystem && make install
