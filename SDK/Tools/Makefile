DOS2UNIX_PACKEGE=dos2unix-6.0

all: 
	if [ ! -d  bin ];then mkdir bin;fi
	echo "build $(DOS2UNIX_PACKEGE)..............."
	tar zxf $(DOS2UNIX_PACKEGE).tar.gz;cd $(DOS2UNIX_PACKEGE);make
	cp $(DOS2UNIX_PACKEGE)/dos2unix bin/
	
CLEAN_FILES = $(DOS2UNIX_PACKEGE) bin
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
#================= SDK param  ===================
SDK_FEATURE_NAME = Tools
SDK_FILE_CP_LIST = *.tar.gz Makefile
#================= end of SDK param ===================
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
