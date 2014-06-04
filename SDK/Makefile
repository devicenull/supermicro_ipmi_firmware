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
# legacy defines :(
PRJ_PATH = $(BASEDIR)
BINPATH = $(BASEDIR)/bin
MODULEPATH = $(BASEDIR)/bin/module
LIBPATH = $(BASEDIR)/lib
IMAGE_PATH = $(BASEDIR)/images
OUTPUT_IMG-y = $(BASEDIR)/MKIMG_Tool/
Q = @
export BASEDIR PRJ_PATH BINPATH LIBPATH IMAGE_PATH MODULEPATH OUTPUT_IMG-y Q

LIBS += -lcgi -lipmi -lchnl  -lm -lsol  -lutility -lnvram  -lipmicrypt -lcommon -lsys -lrt -luart -lupdate -lnet -lmqueue -lgpio -lxml2 -lldap_client -lradius_client -lwdt
LDFLAGS += -L$(BASEDIR)/OpenSSL/openssl/local/lib -L$(BASEDIR)/lib -lcrypto -lssl $(LIBS)
export LDFLAGS

MYMCU=HERMON
export MYMCU

all: kernel libxml2 openssl openldap ipmi vusb ikvm busybox dhcpv6 lighttpd sfcc sfcb openwsman smash smtp ddns ntp lldp nwfirewall webpage mdinfo

kernel:
	cd $(BASEDIR)/Kernel/Host/HERMON/linux && LDFLAGS="" make

libxml2:
	cd $(BASEDIR)/libXML2 && make

openssl:
	cd $(BASEDIR)/OpenSSL && make

openldap:
	cd $(BASEDIR)/OpenLDAP && make

ipmi:
	cd $(BASEDIR)/IPMI && make

vusb:
	cd $(BASEDIR)/USB_VD && make

ikvm:
	cd $(BASEDIR)/IKVM && make

busybox:
	cd $(BASEDIR)/busybox && make

dhcpv6:
	cd $(BASEDIR)/DHCPV6 && make

lighttpd:
	cd $(BASEDIR)/Web_Server/httpd && make

sfcc:
	cd $(BASEDIR)/SFCC && make

sfcb:
	cd $(BASEDIR)/SFCB && make

openwsman:
	cd $(BASEDIR)/OpenWSMAN && make

smash:
	cd $(BASEDIR)/SMASH && make

smtp:
	cd $(BASEDIR)/SMTP && make

ddns:
	cd $(BASEDIR)/DDNS && make

ntp:
	cd $(BASEDIR)/NTP && make

lldp:
	cd $(BASEDIR)/LLDP && make

nwfirewall:
	cd $(BASEDIR)/NW_FIREWALL && make

webpage:
	cd $(BASEDIR)/Web_Server && make

mdinfo:
	cd $(BASEDIR)/PKConfig/MDInfo && make

# This really just generates the output image.  Called install for legacy reasons :)
install:
ifneq ($(shell id -u),0)
	$(error make install must be run as root)
endif
	cd $(BASEDIR)/Kernel/Host/HERMON/linux && LDFLAGS="" make install
	cd $(BASEDIR)/FileSystem && make install
