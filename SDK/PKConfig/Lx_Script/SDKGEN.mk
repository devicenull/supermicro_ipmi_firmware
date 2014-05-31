.PHONY: sdk_menuconfig sdk
-include .config
PreRelFolder=`cd $(PRJ_PATH);cd $(CONFIG_OLDSDK_FOLDER);pwd`
FileList = $(PRJ_PATH)/.Filelist
TmpFolder = $(PRJ_PATH)/tmp

sdk_menuconfig:
	@if [ -f $(PRJ_PATH)/.sdkconfig ];then mv $(PRJ_PATH)/.sdkconfig $(PRJ_PATH)/.config;fi
	@if [ -f mconf ];then if [ ! -x mconf ];then chmod 755 mconf;fi;else echo "Lose mconf file";exit 1;fi
	@if [ -f PKConfig/Lx_Config/Config_all.in ];then ./mconf PKConfig/Lx_Config/Config_sdk.in;else echo "Lose Config file";fi
	@make sdk

sdk:
	@if [ ! -f .config ];then echo "Please do 'make sdk_menuconfig' command !";exit 1;fi
	@if [ ! -d $(PreRelFolder) ];then echo "Old release folder doesn't exist!";exit 1;fi
	@if [ ! -d $(TmpFolder) ];then mkdir $(TmpFolder);else rm -rf $(TmpFolder)/* ;fi
	@echo "Copying souce code & Please wait a minute"
	@if [ ! -f $(FileList) ];then cd $(PRJ_PATH);find ./ -type f ! -wholename '*.metadata*' >$(FileList);fi
	@tar cf -  -T $(FileList) --exclude=$(FileList) --exclude=$(TmpFolder) |  tar xfBp - -C $(TmpFolder);
	@cp ./SDK_Generator/OS/Linux/Host/$(MYMCU)/$(BOARD)/$(SDK_RELEASE_MODE)/sdkgen_t.sh $(TmpFolder)
	@cp ./SDK_Generator/OS/Linux/Host/$(MYMCU)/$(BOARD)/$(SDK_RELEASE_MODE)/sdkgen_t.awk $(TmpFolder)
	@cd $(TmpFolder);./sdkgen_t.sh
	@rm -f $(TmpFolder)/sdkgen_t.sh
	@rm -f $(TmpFolder)/sdkgen_t.awk
	@cd $(TmpFolder)/PKConfig/SDK_Diff/;bash ./patch_gen.sh $(PreRelFolder)  $(TmpFolder) auto p1;mv *.tar.gz $(PRJ_PATH) ;cd -
	@mv $(PRJ_PATH)/.config $(PRJ_PATH)/.sdkconfig
