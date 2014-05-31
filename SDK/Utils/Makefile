-include $(PRJ_PATH)/.SDKPath
-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
CFLAGS+= -Werror -I$(PRJ_PATH)/libXML2/libxml2/local/include/libxml2/
CFLAGS+= -D_FORTIFY_SOURCE=2 -O2 -Wformat -Wformat-security
LDFLAGS +=  -Wl,-z,relro -Wl,-z,now

SH_DIR  = $(PRJ_PATH)/Utils/Host/$(MYMCU)/Board/$(BOARD)
SDKUtils_DIR = $(SDKPATH)/Utils
SDKBoard_DIR = $(SDKUtils_DIR)/Host/$(MYMCU)/Board/

OBJS = $(call OBJECT_FILE, *.c )
EXEC = $(OBJS:.o=) 

.PHONY: $(EXEC) 
all: $(EXEC) 
	$(Q)cd $(BINPATH);if [ ! -f save ] && [ -f savefromPS ]	  ;then ln -s  savefromPS  save	;fi
	$(Q)cd $(BINPATH);if [ ! -f restore ] && [ -f restoretoPS ] ;then ln -s  restoretoPS restore ;fi
	# $(Q)cd $(BINPATH);if [ ! -f savea ]   ;then ln -s  savefromPSa savea ;fi
	#******************************copy script***************************	
	$(Q)echo "copy restore_file.sh"
	$(Q)cp restore_file.sh $(BINPATH)
ifeq ($(CONFIG_PLATFORM_SUPERMICRO_SUBVERSION_X8DTT),y)
	$(Q)echo "copy $(SH_DIR)/$(SUBBOARD)/define_var.sh"
	$(Q)cp $(SH_DIR)/$(SUBBOARD)/define_var.sh $(BINPATH)
else
	$(Q)echo "copy $(SH_DIR)/define_var.sh"
	$(Q)cp $(SH_DIR)/define_var.sh $(BINPATH)
endif

$(EXEC): 
	$(Q)$(ECHO_CMD) "Build file: "$@ "....."
	$(Q)$(call CONDITION_CC,$@.c,$(CFLAGS), $(LDFLAGS)) 
	$(Q)$(ECHO_CMD) "Strip file: "$@"....."
	$(Q)$(STRIP) $@
	$(Q)mv $@ ../bin
	

CLEAN_FILES = $(OBJS)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

DEFAULT_PATTERN += '*.sh' '*.o'  
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
