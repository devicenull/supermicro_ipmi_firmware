SMASHDIR = $(SRCDIR)/smash
CCINCLUDE = $(PRJ_PATH)/SFCC/sfcc/local/include/CimClientLib
CCAPIDIR = $(SMASHDIR)/SFCC_API
SMASHDST = $(PRJ_PATH)/SMASH/SMASH
CXX = $(CROSS_COMPILE)gcc
HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
DBDIR = $(SRCDIR)/smash/dropbear
DROPBEAR_VERSION=2013.60
DBSRCDIR = $(SRCDIR)/smash/dropbear-$(DROPBEAR_VERSION)
SHELLDIR = $(SMASHDIR)/shell
TREE_UTIL_SRC_PATH = $(PRJ_PATH)/SMASH/tree-1.5.2.2-src/src/tree/1.5.2.2/tree-1.5.2.2-src
DTKSMASH_DIR = $(DTKPATH)/SMASH
SMASH_VERSION = 1.04
IPMI_INCLUDE =  -I $(PRJ_PATH)/IPMI/SLAVE_TASK/inc -I $(PRJ_PATH)/IPMI/BRIDGE/inc -I $(PRJ_PATH)/IPMI/SOL/inc -I $(PRJ_PATH)/IPMI/LIB_CGI/inc -I $(PRJ_PATH)/IPMI/COMMON/inc -I $(PRJ_PATH)/IPMI/Platform/inc -I $(PRJ_PATH)/IPMI/SENSOR_DRV/inc -I $(PRJ_PATH)/IPMI/MSG_PROCESSOR/inc -I $(PRJ_PATH)/IPMI/rm_utilities/inc -I $(PRJ_PATH)/IPMI/EVENT/inc -I $(PRJ_PATH)/IPMI/MSG_BUFFER/inc -I $(PRJ_PATH)/IPMI/SENSOR/inc -I $(PRJ_PATH)/IPMI/NVRAM/inc -I $(PRJ_PATH)/IPMI/UpdateMode/inc -I $(PRJ_PATH)/IPMI/MSG_CHANNEL/inc -I $(PRJ_PATH)/IPMI/PS/inc -I $(PRJ_PATH)/IPMI/PEF/inc -I $(PRJ_PATH)/IPMI/MSG_LISTENER/inc -I $(PRJ_PATH)/IPMI/IPMI/inc -I $(PRJ_PATH)/IPMI/I2C_DRV/inc
IPMI_CMD_INCLUDE = -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Chassis/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Chnl/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/DCMI/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Event/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Firewall/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Fru/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/ICMB/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Lan/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Message/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/OEM/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/OpenBlade/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Pef/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/RMCP+_Payload/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Sdr/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Sel/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Sensor/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Serial/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Session/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Sol/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/Usr/inc -I $(PRJ_PATH)/IPMI/IPMI_CMDS/WatchDog/inc
DROPBEAR_ARM_CONF = -host=$(HOST) --prefix=$(DBDIR)  CPPFLAGS="-I$(PRJ_PATH)/SFCB/zlib-1.2.3/zlib/include -I$(PRJ_PATH)/IPMI/RTOS/Linux/timer/inc/ -I$(PRJ_PATH)/ -I$(PRJ_PATH)/IPMI/COMMON/Host/HERMON/Board/SuperMicro_X7SB3/inc/ $(IPMI_INCLUDE) $(IPMI_CMD_INCLUDE) -I $(PRJ_PATH)/IPMI/RTOS/Linux/inc/ -I $(PRJ_PATH)/IPMI/MSG_CHANNEL/LAN/inc/ -I $(PRJ_PATH)/IPMI/MSG_CHANNEL/UART/OS/Linux/inc/ -I $(PRJ_PATH)/IPMI/RTOS/Linux/lan/inc/ -I $(PRJ_PATH)/IPMI/SLAVE_TASK/Host/HERMON/Board/SuperMicro_X7SB3/inc/ -I $(PRJ_PATH)/IPMI/rm_utilities/inc/ -I $(PRJ_PATH)/IPMI/IPMI_CMDS/OEM/Host/HERMON/Board/SuperMicro_X7SB3/inc/ -I $(PRJ_PATH)/Web_Server/OS/Linux/inc/ -I $(PRJ_PATH)/IPMI/UpdateMode/flash_drv/inc/" \
					LDFLAGS="$(LDFLAGS) -L$(PRJ_PATH)/SFCB/zlib-1.2.3/zlib/lib -L$(PRJ_PATH)/lib" \
					--disable-syslog --disable-utmp --disable-utmpx --disable-lastlog CFLAGS=" -g $(CFLAGS) "

export SMASHDST SMASH_VERSION
all:
	@echo "*************Build Dropbear Begin******************"
	if [ ! -d $(DBDIR) ];then mkdir $(DBDIR); fi
	if [ ! -d $(DBSRCDIR) ];then make build_dropbear ;fi
	make build_dropbear
	cp -af $(DBDIR) $(FS_PATH)/usr/local/;
	@echo "*************Build Dropbear End******************"
	@echo "*************Build Shell******************"
	cd $(SHELLDIR); \
	make;
	@echo "*************Build Shell End******************"
	if [ ! -d $(SMASHDST) ];then \
        mkdir $(SMASHDST); fi
	if [ ! -d $(SMASHDST)/lib ];then \
        mkdir $(SMASHDST)/lib; fi
	cd $(CCAPIDIR); \
	make;
	cd $(SMASHDIR); \
	#$(CXX) -o chport chport.c $(CFLAGS) $(LDFLAGS) -Os; \
	$(CXX) -o ShowProperty ShowProperty.c -I$(CCINCLUDE) -lSFCC_API -L$(SMASHDST)/lib -Os; \
	$(CXX) -o deletemethod deletemethod.c -I$(CCINCLUDE) -lSFCC_API -L$(SMASHDST)/lib -Os; \
	$(CXX) -o start start.c -I$(CCINCLUDE)   -lSFCC_API -L$(SMASHDST)/lib -Os; \
	$(CXX) -o stop stop.c -I$(CCINCLUDE) -lSFCC_API -L$(SMASHDST)/lib -Os; \
	$(CXX) -o reset reset.c -I$(CCINCLUDE) -lSFCC_API -L$(SMASHDST)/lib -Os; \
	$(CXX) -o load load.c $(CFLAGS) $(LDFLAGS) -I$(CCINCLUDE) -lSFCC_API -L$(SMASHDST)/lib -Os; \
	$(CXX) -o buildFS buildFS.c -I$(CCINCLUDE) -lSFCC_API -L$(SMASHDST)/lib -Os;
	@echo "*************Build Tree Utility******************"
	@if [ ! -d ${TREE_UTIL_SRC_PATH} ];then \
	tar -zxvf tree-1.5.2.2.tar.gz; fi
	@cd ${TREE_UTIL_SRC_PATH}; \
	make CC=${CC} prefix=${TREE_UTIL_SRC_PATH}/bin XOBJS=strverscmp.o
	@cp -f ${TREE_UTIL_SRC_PATH}/tree $(SMASHDST)
	@$(STRIP) --strip-unneeded $(SMASHDST)/tree;	
	@echo "*************Copy SMASH to FileSystem******************"
	rm -rf $(SMASHDST)/lib/*.o;
	mv -f $(SMASHDIR)/ShowProperty $(SMASHDST);
	mv -f $(SMASHDIR)/buildFS $(SMASHDST);
	mv -f $(SMASHDIR)/deletemethod $(SMASHDST);
	#mv -f $(SMASHDIR)/chport $(SMASHDST);
	mv -f $(SMASHDIR)/start $(SMASHDST);
	mv -f $(SMASHDIR)/stop $(SMASHDST);
	mv -f $(SMASHDIR)/reset $(SMASHDST);
	mv -f $(SMASHDIR)/load $(SMASHDST);
	mv -f $(SMASHDIR)/msh $(SMASHDST);
	$(STRIP) --strip-unneeded $(SMASHDST)/lib/*;
	$(STRIP) --strip-unneeded $(SMASHDST)/ShowProperty;
	$(STRIP) --strip-unneeded $(SMASHDST)/buildFS;
	$(STRIP) --strip-unneeded $(SMASHDST)/deletemethod;
	#$(STRIP) --strip-unneeded $(SMASHDST)/chport;
	$(STRIP) --strip-unneeded $(SMASHDST)/start;
	$(STRIP) --strip-unneeded $(SMASHDST)/stop;
	$(STRIP) --strip-unneeded $(SMASHDST)/reset;
	$(STRIP) --strip-unneeded $(SMASHDST)/load;
	$(STRIP) --strip-unneeded $(SMASHDST)/msh;
	# X9 JACK cp -af $(SMASHDIR)/db.sh $(SMASHDST);
#	cp -af $(SMASHDIR)/uart_ctrl.sh $(PRJ_PATH)/bin;
#	cp -af $(SMASHDIR)/respawn_smash.sh $(PRJ_PATH)/bin;
	chmod 777 $(SMASHDIR)/smashd;
	cp $(SMASHDIR)/smashd $(FS_PATH)/etc/init.d;
	chmod 777 -R $(SMASHDST); \
	mv $(SMASHDST)/lib/* $(FS_PATH)/lib;
	cp -af $(SMASHDST) $(FS_PATH);

build_dropbear:
	tar jxf dropbear-$(DROPBEAR_VERSION).tar.bz2; 
	cd $(DBSRCDIR); ./configure $(DROPBEAR_ARM_CONF); 
	cd $(DBSRCDIR);make && make install;
	cd $(DBDIR)/bin/;rm -rf dbclient  dropbearconvert; 
	cd $(DBDIR);rm -rf share; 
	$(STRIP) --strip-unneeded $(DBDIR)/sbin/dropbear; 
	$(STRIP) --strip-unneeded $(DBDIR)/bin/dropbearkey; 

	
.PHONY: extract build_dropbear
extract:
	$(Q)echo "******************** extract SMASH ********************"
	$(Q)if [ ! -d $(DTKSMASH_DIR) ];then mkdir -p $(DTKSMASH_DIR); \
	cd $(DTKSMASH_DIR); mkdir SFCC_API; mkdir shell; mkdir SFCC_API/inc; fi
	$(Q)cp -f $(PRJ_PATH)/SMASH/*.c $(DTKSMASH_DIR)/
	$(Q)cp -f $(PRJ_PATH)/SMASH/*.gz $(DTKSMASH_DIR)/
	$(Q)cp -f $(PRJ_PATH)/SMASH/*.sh $(DTKSMASH_DIR)/		
	$(Q)cp -f $(PRJ_PATH)/SMASH/SFCC_API/*.c $(DTKSMASH_DIR)/SFCC_API/
	$(Q)cp -f $(PRJ_PATH)/SMASH/SFCC_API/Makefile $(DTKSMASH_DIR)/SFCC_API/
	$(Q)cp -rf $(PRJ_PATH)/SMASH/SFCC_API/inc $(DTKSMASH_DIR)/SFCC_API/
	$(Q)cp -f $(PRJ_PATH)/SMASH/shell/*.c $(DTKSMASH_DIR)/shell/
	$(Q)rm -f $(DTKSMASH_DIR)/shell/msh.c
	$(Q)cp -f $(PRJ_PATH)/SMASH/shell/msh.o $(DTKSMASH_DIR)/shell/
	$(Q)cp -f $(PRJ_PATH)/SMASH/shell/*.h $(DTKSMASH_DIR)/shell/
	$(Q)cp -f $(PRJ_PATH)/SMASH/shell/*.gz $(DTKSMASH_DIR)/shell/
	$(Q)cp -f $(PRJ_PATH)/SMASH/shell/*.l $(DTKSMASH_DIR)/shell/
	$(Q)cat $(PRJ_PATH)/SMASH/shell/Makefile | sed -e 's/msh.c/msh.o/g' > $(DTKSMASH_DIR)/shell/Makefile
	$(Q)cat $(PRJ_PATH)/SMASH/Makefile| sed \
	-e '/SDK/d' -e '/DTK/d' \
	-e '/extract/d' \
	-e '/cat Makefile|/d' \
	-e "/-e [\'\"]/d" \
	> $(DTKSMASH_DIR)/Makefile
	$(Q)chmod 744 $(DTKSMASH_DIR)/Makefile
	
CLEAN_PATTERN = '*.o' '*.d'
CLEAN_FILES = $(SMASHDST) $(DBSRCDIR) $(DBDIR)  tree-1.5.2.2-src dropbear_patch \
            shell/flex-2.5.35 shell/readline-5.2 shell/termcap-1.3.1 shell/shell shell/buil_shell SMASH/SMASH \
            $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),$(SMASHDIR),,) \
            $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),$(CCAPIDIR),,) \
            $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),$(SHELLDIR),,)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
	
SDK_FEATURE_NAME = SMASH
SDK_FILE_CP_LIST =  *.tar.gz  *.o smashd db.sh  Makefile shell/*.tar.gz shell/Makefile shell/*.o\
					smashd shell/*.l SFCC_API/*.tar.gz SFCC_API/*.o SFCC_API/Makefile
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
