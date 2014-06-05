-include $(PRJ_PATH)/.SDKPath
CIMOM_SUITE_DIR = $(PRJ_PATH)/SFCB
CB_ROOT_DIR = $(PRJ_PATH)/SFCB
CB_DST_DIR = /SFCB
CB_X86_DST_DIR = /SFCBx86
SFCB_DIR = $(CB_ROOT_DIR)/sblim-sfcb-1.3.2
ZLIB_DIR = $(CB_ROOT_DIR)/zlib-1.2.3
SFCC_ROOT_DIR = $(PRJ_PATH)/SFCC
CURL_DIR = $(SFCC_ROOT_DIR)/curl/local
CURL_X86_DIR = $(SFCC_ROOT_DIR)/curl/curlx86/local
CURL_SRC_DIR = $(SFCC_ROOT_DIR)/curl/curl-7.18.1
DTKCIMOM_DIR = $(DTKPATH)/SFCB
DTKSFCB_DIR = $(DTKPATH)/SFCB
PROVIDER_SRC_DIR = $(CB_ROOT_DIR)/providers


CMPI_BASE_NAME = Linux_Base
CMPI_BASE_DIR = $(PROVIDER_SRC_DIR)/sblim-cmpi-base-1.5.7

TEXT_CONSOLE_DIR = $(PROVIDER_SRC_DIR)/TextRedirection
LOG_RECORD_DIR = $(PROVIDER_SRC_DIR)/LogRecord
THRESHOLD_SENSOR_DIR = $(PROVIDER_SRC_DIR)/ThresholdSensor
DISCRETE_SENSOR_DIR = $(PROVIDER_SRC_DIR)/DiscreteSensor
RECORD_LOG_DIR = $(PROVIDER_SRC_DIR)/RecordLog
POWER_CONTROL_DIR = $(PROVIDER_SRC_DIR)/PowerControl

SFCB_STAGE = $(CB_DST_DIR)/local/var/lib/sfcb/stage
SFCB_STAGE_MOF = $(SFCB_STAGE)/mofs
SFCB_STAGE_REG = $(SFCB_STAGE)/regs
CMPI_BASE_STAGE_MOF = $(SFCB_STAGE_MOF)/root/cimv2
CMPI_BASE_STAGE_REG = $(SFCB_STAGE_REG)

CFG_SFCB_PRF = \
--prefix=$(CB_DST_DIR)/local  --enable-local-connect-only \
CIMSCHEMA_SOURCE="$(CB_ROOT_DIR)/cim_schema_2.19.1Final-MOFs-Full.zip" \
--localstatedir=$(CB_ROOT_DIR)$(CB_DST_DIR)/local/var \
--datarootdir=$(CB_ROOT_DIR)$(CB_DST_DIR)/local/share \
CIMSCHEMA_MOF=cim_schema_2.19.1.mof \
CIMSCHEMA_SUBDIRS=true

CFG_SFCB_X86_PRF = \
--prefix=$(CB_X86_DST_DIR)/local \
CIMSCHEMA_SOURCE="$(CB_ROOT_DIR)/cim_schema_2.19.1Final-MOFs-Full.zip" \
CIMSCHEMA_MOF=cim_schema_2.19.1.mof \
CIMSCHEMA_SUBDIRS=true \
--build=i686-pc-linux-gnu \
--host=i686-pc-linux-gnu \
--libdir=$(CB_ROOT_DIR)$(CB_X86_DST_DIR)/local/lib \
--localstatedir=$(CB_ROOT_DIR)$(CB_DST_DIR)/local/var \
--datarootdir=$(CB_ROOT_DIR)$(CB_DST_DIR)/local/share \
CC=gcc \
AR=ar \
RANLIB=ranlib \
STRIP=strip \
CXX=g++ \
LD=ld \
NM=nm \
LDFLAGS= 

CURL_CFG =\
--disable-largefile \
--disable-ftp \
--disable-file \
--disable-ldap \
--disable-ldaps \
--disable-dict \
--disable-telnet \
--disable-tftp \
--disable-manual \
--disable-ipv6 \
--disable-verbose \
--disable-sspi \
--disable-debug \
--disable-cookies \
--with-pic \
--without-libssh2 \
--without-gnutls \
--without-nss \
--without-ca-bundle \
--without-ca-path \
--without-libidn

HOST = arm-linux
CFG_HOST = --host=$(HOST)

LIBS=

export CMPI_BASE_STAGE_REG CMPI_BASE_STAGE_MOF

all:
	@echo "**********************Build curlx86*****************************"
	if [ -f $(CURL_SRC_DIR)/Makefile ];then \
	cd $(CURL_SRC_DIR); \
	make clean; make distclean; \
	fi
	if [ ! -d $(CURL_X86_DIR) ];then \
	cd $(CURL_SRC_DIR); \
	CC=gcc AR=ar LD=ld NM=nm OBJDUMP=objdump RANLIB=ranlib STRIP=strip CFLAGS="-D_GNU_SOURCE" CPPFLAGS="-D_GNU_SOURCE" LDFLAGS="" ./configure --prefix=$(CURL_X86_DIR) $(CURL_CFG); \
	make; make install; \
	make clean; make distclean; \
	fi
	@echo "***************************Build zlib***************************"
	if [ ! -d $(ZLIB_DIR) ];then \
	cd $(CB_ROOT_DIR); tar zxvf zlib-1.2.3.tar.gz; \
	cd $(ZLIB_DIR); \
	CC=$(HOST)-gcc AR="$(HOST)-ar cr"; \
	CFLAGS="-Os -D_GNU_SOURCE" ./configure -s --prefix=./zlib; \
	make; \
	make install; \
	make clean; \
	make distclean; \
	$(STRIP) ./zlib/lib/libz.so.*; \
	rm -rf ./zlib/share/man; \
	cd $(ZLIB_DIR); \
	CC=gcc AR="ar cr"; \
	CFLAGS="-Os -D_GNU_SOURCE" ./configure -s --prefix=./zlibx86; \
	make ; \
	make install; \
	fi
	@echo "***************************Build SFCB & Tar SFCB***************************"
	cd $(CB_ROOT_DIR);
	if [ ! -d $(SFCB_DIR) ];then \
	cd $(CB_ROOT_DIR); tar jxf sblim-sfcb-1.3.2.tar.bz2; fi
	if [ ! -f $(SFCB_DIR)/Makefile ];then \
	cd $(SFCB_DIR); \
	echo "***************************Build x86 SFCB***************************"; \
	./configure CFLAGS="-Os -D_GNU_SOURCE" CPPFLAGS="-I$(ZLIB_DIR)/zlibx86/include -I$(CURL_X86_DIR)/include" LDFLAGS="-L$(ZLIB_DIR)/zlibx86/lib -L$(CURL_X86_DIR)/lib" $(CFG_SFCB_X86_PRF); \
	make; \
	echo -n "System checking"; \
	until [ ! -f /tmp/.sfcbx86 ];do \
	echo -n "."; \
	sleep 1; done; \
	echo -ne "\n"; \
	touch /tmp/.sfcbx86; \
	make install; \
	make install-strip; \
	make postinstall; \
	cp -rf $(CB_X86_DST_DIR)/* $(CB_ROOT_DIR)$(CB_X86_DST_DIR);rm -rf $(CB_X86_DST_DIR); \
	rm -rf /tmp/.sfcbx86; \
	make clean; \
	make distclean; \
	echo "***************************Build ARM SFCB***************************"; \
	./configure CFLAGS=-Os CPPFLAGS="-I$(ZLIB_DIR)/zlib/include -I$(CURL_DIR)/include" LDFLAGS="-L$(ZLIB_DIR)/zlib/lib -L$(CURL_DIR)/lib" $(CFG_SFCB_PRF) $(CFG_HOST); \
	make ; \
	echo -n "System checking"; \
	until [ ! -f /tmp/.sfcb ];do \
	echo -n "."; \
	sleep 1; done; \
	echo -ne "\n"; \
	touch /tmp/.sfcb; \
	make install; \
	make install-strip; \
	make postinstall; \
	cp -rf $(CB_DST_DIR)/* $(CB_ROOT_DIR)$(CB_DST_DIR);rm -rf $(CB_DST_DIR) \
	rm -rf /tmp/.sfcb; \
	echo "*************************Build MOF*************************"; \
	export PATH=$(CB_ROOT_DIR)$(CB_X86_DST_DIR)/local/bin:$(CB_ROOT_DIR)$(CB_X86_DST_DIR)/local/sbin:$(PATH); \
	export LD_LIBRARY_PATH=$(ZLIB_DIR)/zlibx86/lib:$(CURL_X86_DIR)/lib:$(CB_ROOT_DIR)$(CB_X86_DST_DIR)/local/lib:$(LD_LIBRARY_PATH); \
	cd $(LOG_RECORD_DIR); \
	sh provider-register.sh -t sfcb -r mof/IPMI_LogRecord.registration -m mof/IPMI_LogRecord.mof; \
	cd $(THRESHOLD_SENSOR_DIR); \
	sh provider-register.sh -t sfcb -r mof/Threshold_Sensor.registration -m mof/Threshold_Sensor.mof; \
	cd $(TEXT_CONSOLE_DIR); \
	sh provider-register.sh -t sfcb -r mof/IPMI_TextRedirectionService.registration -m mof/IPMI_TextRedirectionService.mof; \
	cd $(RECORD_LOG_DIR); \
	sh provider-register.sh -t sfcb -r mof/IPMI_RecordLog.registration -m mof/IPMI_RecordLog.mof; \
	cd $(DISCRETE_SENSOR_DIR); \
	sh provider-register.sh -t sfcb -r mof/Discrete_Sensor.registration -m mof/Discrete_Sensor.mof; \
	cd $(POWER_CONTROL_DIR); \
	sh provider-register.sh -t sfcb -r mof/IPMI_PowerManagementService.registration -m mof/IPMI_PowerManagementService.mof; \
	rm -rf $(CB_ROOT_DIR)$(CB_DST_DIR)/local/share/sfcb/CIM/* $(CB_ROOT_DIR)$(CB_DST_DIR)/local/share/doc $(CB_ROOT_DIR)$(CB_DST_DIR)/local/share/man $(CB_ROOT_DIR)$(CB_DST_DIR)/local/var/lib/sfcb/registration/repository.previous/*; \
	fi
	@echo "*************************Build Providers*************************"
	if [ ! -d $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi ];then \
	mkdir -p $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi; fi
	if [ ! -f $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/libcmpiIPMI_LogRecordProvider.so.4 ]; then \
	cd $(LOG_RECORD_DIR); \
	make; \
	cp -af *LogRecordProvider.so* $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/; \
	fi
	if [ ! -f $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/libcmpiThreshold_SensorProvider.so.4 ]; then \
	cd $(THRESHOLD_SENSOR_DIR); \
	make; \
	cp -af *Threshold_SensorProvider.so* $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/; \
	fi
	if [ ! -f $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/libcmpiIPMI_TextRedirectionServiceProvider.so.4 ]; then \
	cd $(TEXT_CONSOLE_DIR); \
	make; \
	cp -af *libcmpiIPMI_TextRedirectionServiceProvider.so*  $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/; \
	fi
	if [ ! -f $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/libcmpiIPMI_RecordLogProvider.so.4 ]; then \
	cd $(RECORD_LOG_DIR); \
	make; \
	cp -af *RecordLogProvider.so* $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/; \
	fi
	if [ ! -f $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/libcmpiDiscrete_SensorProvider.so.4 ]; then \
	cd $(DISCRETE_SENSOR_DIR); \
	make; \
	cp -af *Discrete_SensorProvider.so* $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/; \
	fi
	if [ ! -f $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/libcmpiIPMI_PowerManagementServiceProvider.so.4 ]; then \
	cd $(POWER_CONTROL_DIR); \
	make; \
	cp -af *IPMI_PowerManagementServiceProvider.so* $(CB_ROOT_DIR)$(CB_DST_DIR)/local/lib/cmpi/; \
	fi
	@echo "************************Copy to FileSystem************************"
	chmod 777 -R $(CB_ROOT_DIR)$(CB_DST_DIR) $(ZLIB_DIR)/zlib $(CURL_DIR);
	cp -af $(ZLIB_DIR)/zlib/lib $(FS_PATH)
	cp -af $(CB_ROOT_DIR)$(CB_DST_DIR) $(FS_PATH);
	cp -af $(CB_ROOT_DIR)/sfcb.cfg     $(FS_PATH)$(CB_DST_DIR)/local/etc/sfcb;
	
CLEAN_PATTERN = '*.o' '*.d' '*.so' '*.so.*'
CLEAN_FILES =$(CB_DST_DIR) $(SFCB_DIR) $(CMPI_BASE_DIR) $(ZLIB_DIR) \
             $(CB_X86_DST_DIR) $(CB_ROOT_DIR)/SFCB $(CB_ROOT_DIR)/SFCBx86 \
             $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),$(PROVIDER_SRC_DIR),,)

include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
	
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
#================= SDK param  ===================
SDK_FEATURE_NAME = SFCB
PATTERN = '*.c' '*.h' '*.sh' '*.mof' '*.registration' 'Makefile'
SDK_FILE_CP_LIST = $(call GEN_SEARCH_LIST,$(PATTERN),providers/,,)
SDK_FILE_CP_LIST += zlib-1.2.3.tar.gz sblim-sfcb-1.3.2.tar.bz2  Makefile \
					cim_schema_2.19.1Final-MOFs-Full.zip  sfcb.cfg 
					
#================= end of SDK param ===================
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
