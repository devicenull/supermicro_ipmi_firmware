-include $(PRJ_PATH)/.SDKPath
HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
SMTP_SRCDIR = $(PRJ_PATH)/SMTP
SMTP_ZIP    = $(wildcard $(SMTP_SRCDIR)/*.tar.gz)
SMTP_CFG    = --host=${HOST} -with-ssl=openssl --libdir=$(PRJ_PATH)/OpenSSL/openssl/lib/

DTKSMTP_DIR = $(DTKPATH)/SMTP

override CFLAGS = 
override LDFLAGS = 
override LIBS =

all:
	$(Q)echo "CC=" $(CC)
	$(Q)echo "AR=" $(AR)
	$(Q)cd $(SMTP_SRCDIR); \
	if [ ! -d smtp ];then mkdir smtp;cd smtp;tar zxf $(SMTP_ZIP);fi
	$(Q)cd $(SMTP_SRCDIR)/smtp;if [ ! -f $(SMTP_SRCDIR)/smtp/src/msmtp ];then \
	./configure $(SMTP_CFG);make;fi
	$(CROSS_COMPILE)strip $(SMTP_SRCDIR)/smtp/src/msmtp
	$(Q)cp $(SMTP_SRCDIR)/smtp/src/msmtp $(BINPATH)/msmtp
	$(Q)cp $(SMTP_SRCDIR)/smtpclient.sh $(BINPATH)/

install:
	#******************************do nothing***************************
	
CLEAN_FILES = smtp
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

#================= SDK param  ===================
SDK_FEATURE_NAME = SMTP
SDK_FILE_CP_LIST = ./Makefile  *.tar.gz  ./ReadMe ./smtpclient.sh
#================= end of SDK param ===================

include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
