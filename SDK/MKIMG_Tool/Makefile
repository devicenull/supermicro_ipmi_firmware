-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib


TOOL_PATH   = $(PRJ_PATH)/MKIMG_Tool/ipmi_firmware_tools/
WORK_PATH   = $(TOOL_PATH)/data/

install:
	$(Q)echo ==== in MKIMG_Tool ====
	cp $(IMAGE_PATH)/out_kernel.bin $(WORK_PATH)/kernel.bin
	cp $(IMAGE_PATH)/bootloader_pcb_rev_b.bin $(WORK_PATH)/bootloader.bin
	cp $(IMAGE_PATH)/out_rootfs_img.bin $(WORK_PATH)/1stFS.bin
	cp $(IMAGE_PATH)/out_webfs_img.bin $(WORK_PATH)/2ndFS.bin
	cd $(TOOL_PATH); python rebuild_image.py
	cp -f $(WORK_PATH)/rebuilt_image.bin $(IMAGE_PATH)/HERMONEVB_all.bin
	$(Q)echo
	$(Q)echo
	$(Q)echo Image Generation Complete.  Output Path: $(IMAGE_PATH)/HERMONEVB_all.bin
	$(Q)echo
	$(Q)echo
	

