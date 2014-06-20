
CLEAN_FS=$(BASEDIR)/FileSystem/clean_filesystem
CLEAN_WEBFS=$(BASEDIR)/FileSystem/clean_webfs

# sizes are in MB... increasing them may require adjusting how the flash image is laid out
ROOT_FS_SIZE = 22
ROOT_FS_OUTPUT = $(BASEDIR)/images/rootfs
WEB_FS_SIZE = 9
WEB_FS_OUTPUT = $(BASEDIR)/images/webfs

FS_TEMP = $(BASEDIR)/FileSystem/tmp

install: copyfiles fixperms genweb genroot genimage

copyfiles:
	# While we're in install, we're running as root (this is enforced by the Makefile in the root directory)
	# this means we can safely create the contents of /dev/ and fix permissions on any root-only files
	rsync -a $(CLEAN_FS)/ $(FS_PATH)/

	cd $(FS_PATH); tar xf $(BASEDIR)/FileSystem/device_files.tar

fixperms:
	chmod +x $(FS_PATH)/etc/init.d/*

genweb:
	# Generate the webfs cramfs image
	$(Q)echo
	$(Q)echo ------------------- wwwfs -------------------
	$(Q)echo

	/sbin/mkfs.cramfs $(CLEAN_WEBFS) $(WEB_FS_OUTPUT)

genroot:
	# Generate the rootfs cramfs image
	$(Q)echo
	$(Q)echo ------------------- rootfs -------------------
	$(Q)echo

	/sbin/mkfs.cramfs $(FS_PATH) $(ROOT_FS_OUTPUT)

genimage:
	# Generate the final flashable image

	cd $(OUTPUT_IMG-y) && make install
