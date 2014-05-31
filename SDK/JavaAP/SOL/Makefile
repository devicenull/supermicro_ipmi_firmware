-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
SDKJavaAPSOL_DIR = $(SDKPATH)/JavaAP/SOL
SRC_FILES = $(wildcard src/*.java)
all:

install:
	$(Q)if [ ! -d bin ]; then mkdir bin; fi
	$(Q)if [ ! -d bin/conv ]; then cp -r conv/ bin; rm -rf bin/conv/.metadata/; fi
	$(Q)if [ ! -d bin/icon ]; then cp -r icon/ bin; rm -rf bin/icon/.metadata/; fi
	$(Q)if [ -n "$(SRC_FILES)" ] ;then \
	$(JAVAC) -g:none -classpath bin/ -sourcepath src -d bin/ $(SRC_FILES); \
	fi;
	$(Q)$(JAR) -cfm SOL.jar $(MANIFEST) -C bin/ .;
	$(Q)$(PACK200) --repack SOL.jar;
	$(Q)$(JARSIGNER) -keystore $(KEYSTORE) -storepass $(STOREPASS) -keypass $(KEYPASS) SOL.jar $(PROVIDER);
	$(Q)$(PACK200) SOL.jar.pack.gz SOL.jar;
	$(Q)cp SOL.jar.pack.gz $(WEBFS_PATH)/SOL__V$(JAVA_SOL_VIEWER_FULL_VERSION).jar.pack.gz;
	$(Q)cp SOL.jar.pack.gz SOL__V$(JAVA_SOL_VIEWER_FULL_VERSION).jar.pack.gz;

CLEAN_FILES = bin SOL.jar SOL.jar.pack.gz
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

extract:
