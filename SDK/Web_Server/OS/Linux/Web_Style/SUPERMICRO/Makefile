-include $(PRJ_PATH)/.SDKPath
-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
-include cgi_src/oem_cgi.in
CGILIBPATH=$(PRJ_PATH)/Web_Server/OS/Linux/
WEB_STYLE_PATH=$(CGILIBPATH)/Web_Style/$(WEBSTYLE)

install:
	@if [ ! -d $(WEBFS_PATH)/cgi ];then \
	mkdir -p $(WEBFS_PATH)/cgi;fi
	@cp $(WEB_PATH)/OS/Linux/*.cgi $(WEBFS_PATH)/cgi/
	@if [ -n "$(PRJ_CGI_SRC)" ];then cp $(PRJ_CGI_SRC:.c=.cgi) $(WEBFS_PATH)/cgi/;fi
	@rm -f $(WEB_PATH)/OS/Linux/Web_Style/$(WEBSTYLE)/images/Snapshot.bmp

	@rm -f $(WEB_PATH)/OS/Linux/Web_Style/$(WEBSTYLE)/save_config.bin
	@echo $(WEB_PATH)/OS/Linux/Web_Style/$(WEBSTYLE)/
	@cp -rf $(WEB_PATH)/OS/Linux/Web_Style/$(WEBSTYLE)/* $(WEBFS_PATH)/
	@find $(WEBFS_PATH)/ -size 0 -exec rm -f {} \;
	@rm -f $(WEBFS_PATH)/*.c
	@rm -f $(WEBFS_PATH)/*.o
	@rm -f $(WEBFS_PATH)/*.d
	@rm -rf $(WEBFS_PATH)/cgi_src $(WEBFS_PATH)/Makefile
	@cd $(WEBFS_PATH);ln -s cgi cgi-bin

-include $(WEB_STYLE_PATH)/extract.in

