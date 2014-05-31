-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
FS_PATH    = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/rootfs
FS_PATH_FINAL = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/rootfs_final
WEBFS_PATH = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/webfs
WEBFS_PATH_FINAL = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/webfs_final
FS_SRCDIR = $(PRJ_PATH)/FileSystem/Host/$(MYMCU)/
FS_ZIP     = $(wildcard $(FS_SRCDIR)/*.tar.gz)
ETCPath  = $(FS_PATH)/etc/


ROOT_FS_SIZE	= 22
ROOT_FS_NAME	= rootfs
WEB_FS_SIZE	= 9
WEB_FS_NAME	= webfs

VERSION_CTRL_KEYWORD = .svn

TC_SRCDIR  = $(subst .tar.gz,,$(wildcard $(PRJ_PATH)/ToolChain/Host/$(MYMCU)/*.tar.gz))

export FS_PATH WEBFS_PATH
export MYMCU FS_SRCDIR

FS_ELEMENT=$(addprefix _FS_ELEMENT_,$(FS_ELEMENT_DIR))

all:

.PHONY: $(FS_ELEMENT) 
$(FS_ELEMENT):
	$(MAKE) --directory=$(patsubst _FS_ELEMENT_%,%,$@)
	$(MAKE) install --directory=$(patsubst _FS_ELEMENT_%,%,$@)

BUILD_FS: $(FS_ELEMENT) 

install:
	$(Q)bash $(PRJ_PATH)/PKConfig/MDInfo/mdver.sh $(FS_SRCDIR) $(ver) 
	$(Q)if [ -d $(FS_PATH) ];then rm -rf $(FS_PATH) ; fi
	$(Q)mkdir -p $(FS_PATH)
	$(Q)cd $(FS_PATH); tar zxf $(FS_ZIP) 
	$(Q)if [ ! -d $(FS_PATH)/bin/ ];then mkdir -p $(FS_PATH)/bin/; fi
	$(Q)if [ -d $(WEBFS_PATH) ];then rm -rf $(WEBFS_PATH); fi
	$(Q)mkdir -p $(WEBFS_PATH)

#copy the necessary files of the firmware here
	$(Q)$(CP_CMD) $(FS_SRCDIR)/Board/$(BOARD)/rcS $(ETCPath)/init.d/rcS
	$(Q)if [ -f $(FS_SRCDIR)/Board/$(BOARD)/dhcp.script ]; then \
	$(CP_CMD) $(FS_SRCDIR)/Board/$(BOARD)/dhcp.script $(FS_PATH)/usr/share/udhcpc/default.script; \
	fi
	$(Q)if [ ! -d  $(FS_PATH)/usr/local/etc ];then mkdir $(FS_PATH)/usr/local/etc; fi 
	$(CP_CMD) -r $(PRJ_PATH)/OpenLDAP/openldap-config $(FS_PATH)/usr/local/etc/openldap; 
	$(Q)if [ -f $(BL_SRC_DIR)/tools/env/fw_env.config ] ;then \
		$(CP_CMD) -af $(BL_SRC_DIR)/tools/env/fw_env.config  $(ETCPath);\
	fi
ifeq ($(MYMCU),HERMON)	
ifeq ($(CONFIG_PLATFORM_SUPERMICRO_SUBVERSION_X9),y)
	cp $(FS_SRCDIR)/Board/$(BOARD)/BTL_X9  $(ETCPath)/BTL
else 
	cp $(FS_SRCDIR)/Board/$(BOARD)/BTL  $(ETCPath)
endif 
endif	
	$(Q)$(MAKE) BUILD_FS
#enable network console
ifeq ($(CONFIG_DEBUG_NETCONSOLE),y)
	$(Q) $(ECHO_CMD) "telnetd -p 1206" >> $(ETCPath)/init.d/rcS 
endif
#handle  platform independent files here
	make install --directory=Host/$(MYMCU)/Board/$(BOARD)/
	$(Q)$(CP_CMD) $(PRJ_PATH)/bin/* $(FS_PATH)/bin/ -rf
ifeq ($(BUILD_OPTION),1)
	$(Q)$(CP_CMD) -af $(PRJ_PATH)/lib/*.so* $(FS_PATH)/lib/
	$(CROSS_COMPILE)strip $(FS_PATH)/lib/*.so*
endif
#umount
	$(Q)$(ECHO_CMD) " Gen WWW Filesystem (CRAMFS).........."
	$(Q)cd $(WEBFS_PATH); \
	$(FIND_CMD) ./ | grep "$(VERSION_CTRL_KEYWORD)" | xargs -n 1 rm -rf; cd $(PRJ_PATH)/FileSystem
	$(Q)if [ -d $(WEBFS_PATH_FINAL) ];then umount $(WEBFS_PATH_FINAL); rm -rf $(WEBFS_PATH_FINAL); fi
	$(Q)mkdir -p $(WEBFS_PATH_FINAL)
	$(Q)dd if=/dev/zero of=$(WEB_FS_NAME)_tmp bs=1024K count=$(WEB_FS_SIZE) >>/dev/null 2>&1
	$(Q)/sbin/mke2fs -F -m0 $(WEB_FS_NAME)_tmp >>/dev/null 2>&1
	$(Q)mount -o loop $(WEB_FS_NAME)_tmp $(WEBFS_PATH_FINAL)
	$(Q)$(CP_CMD) -af $(WEBFS_PATH)/* $(WEBFS_PATH_FINAL)/
	$(call SHOW_FSSIZE,$(WEB_FS_SIZE),$(WEBFS_PATH_FINAL),$(WEB_FS_NAME))
	$(Q)/sbin/mkfs.cramfs $(WEBFS_PATH_FINAL) $(PRJ_PATH)/FileSystem/$(WEB_FS_NAME)
	$(Q)umount $(WEBFS_PATH_FINAL)
	$(Q)$(ECHO_CMD) " Gen Root Filesystem (CRAMFS).........."
	$(Q)cd $(FS_PATH); \
	$(FIND_CMD) ./ | grep "$(VERSION_CTRL_KEYWORD)" | xargs -n 1 rm -rf; \
	$(FIND_CMD) ./bin | grep "\.sh" | xargs -n 1 -r chmod u+x,g+x,o+x; \
	$(FIND_CMD) ./usr/bin | grep "\.sh" | xargs -n 1 -r chmod u+x,g+x,o+x; \
	$(FIND_CMD) ./sbin | grep "\.sh" | xargs -n 1 -r chmod u+x,g+x,o+x; \
	$(FIND_CMD) ./usr/sbin | grep "\.sh" | xargs -n 1 -r chmod u+x,g+x,o+x; \
	$(FIND_CMD) ./etc | grep "\.sh" | xargs -n 1 -r chmod u+x,g+x,o+x; \
	$(FIND_CMD) ./etc/init.d | grep "*" | xargs -n 1 -r chmod u+x,g+x,o+x; \
	chmod u+x,g+x,o+x ./etc/dhcpv6/dhcp6c-script; \
	chmod u+x,g+x,o+x ./usr/share/udhcpc/default.script; \
	chmod u+x,g+x,o+x ./etc/network/network6; \
	chmod u+x,g+x,o+x ./etc/init.d/rcS; \
	chmod u+x,g+x,o+x ./etc/init.d/httpd; \
	chmod u+x,g+x,o+x ./bin/htpasswd; \
	cd $(PRJ_PATH)/FileSystem
	$(Q)if [ -d $(FS_PATH_FINAL) ];then umount $(FS_PATH_FINAL); rm -rf $(FS_PATH_FINAL) ; fi
	$(Q)mkdir -p $(FS_PATH_FINAL)
	$(Q)dd if=/dev/zero of=$(ROOT_FS_NAME)_tmp bs=1024K count=$(ROOT_FS_SIZE) >>/dev/null 2>&1
	$(Q)/sbin/mke2fs -F -m0 $(ROOT_FS_NAME)_tmp >>/dev/null 2>&1
	$(Q)mount -o loop $(ROOT_FS_NAME)_tmp $(FS_PATH_FINAL)
	$(Q)$(CP_CMD) -af $(FS_PATH)/* $(FS_PATH_FINAL)/
	$(call SHOW_FSSIZE,$(ROOT_FS_SIZE),$(FS_PATH_FINAL),$(ROOT_FS_NAME))
	$(Q)/sbin/mkfs.cramfs $(FS_PATH_FINAL) $(PRJ_PATH)/FileSystem/$(ROOT_FS_NAME)
	$(Q)umount $(FS_PATH_FINAL)
	$(Q)if [ ! -d $(IMAGE_PATH) ]; then mkdir -p $(IMAGE_PATH); fi
	$(Q)$(CP_CMD) -f $(ROOT_FS_NAME) $(WEB_FS_NAME) $(IMAGE_PATH)
	$(Q)rm -rf $(FS_PATH) $(WEBFS_PATH) $(FS_PATH_FINAL) $(WEBFS_PATH_FINAL)
	$(Q)$(ECHO_CMD) " Gen All-in-one image & mdinfo .........."
	$(Q)cd ../$(OUTPUT_IMG-y); make install
	
clean:
	$(Q)echo "******************** clean File System ********************"
	$(Q)rm -rf  rootfs  rootfs_tmp  webfs  webfs_tmp	
	$(Q)for i in $(FS_ELEMENT_DIR); do make -C $$i clean; done
#	$(Q)for i in $(BB_ELEMENT_DIR); do make -C $$i clean; done
	$(Q)rm -rf $(TMPFS_PATH) $(FS_PATH) $(FS_PATH_FINAL) $(WEBFS_PATH) $(WEBFS_PATH_FINAL)

-include $(PRJ_PATH)/.SDKPath
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
DTKFS_PATH = $(DTKPATH)/FileSystem

FILE_LIST = $(wildcard ./Host/$(MYMCU)/*.tar.gz) ./Makefile Host/$(MYMCU)/Board

extract:
	$(Q)$(ECHO_CMD) " Extract File System "
	$(Q)$(call SDK_CP_TASK,$(FILE_LIST),$(DTKFS_PATH))
