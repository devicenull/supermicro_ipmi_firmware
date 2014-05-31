-include $(PRJ_PATH)/.SDKPath

.PHONY: extract extract_loop
extract:
	@if [ -f .config ];then echo "Please make clean!!!";exit 1;fi
	@if [ -f mconf ];then if [ ! -x mconf ];then chmod 755 mconf;fi;else echo "Lose mconf file";exit 1;fi
	@if [ -f PKConfig/Lx_Config/Config_Extr.in ];then ./mconf PKConfig/Lx_Config/Config_Extr.in;else echo "Lose Config file";fi
	@if [ ! -f .config ] ;then exit 1 ;fi
	@cp .tmpconfig.h autoconfig.h
	@echo "============ make ============"
	@make
	@echo "============ make install ============"
	@make install
	@echo "============ SDK Environment Init ============"
	$(call SDKPATH_CMD)
	@echo "============ Extraction ============"
	@make extract_loop
	@cp .config config_extractor

extract_loop:
	-for i in $(MODULE_DIR);do (cd $$i && $(MAKE) extract);done
	-for i in $(OTHER_FOLDERS_FOR_EXTRACT);do (cd $$i && $(MAKE) extract);done
	@cp -f ./conf $(EXTRPATH_tmp)/
	@cp -f ./mconf $(EXTRPATH_tmp)/
	@cp -f ./ProjectConfig-$(MYMCU) $(EXTRPATH_tmp)/
	@cp -f ./ProjectConfig-JavaAP $(EXTRPATH_tmp)/
	@cat ./.config | sed -e '/CONFIG_EXTRACT/d' -e '/CONFIG_EXCLUDE/d' -e '/Extractor/d' > $(EXTRPATH_tmp)/.config
	@cat ./autoconfig.h | sed -e '/CONFIG_EXTRACT/d' -e '/CONFIG_EXCLUDE/d' -e '/Extractor/d' > $(EXTRPATH_tmp)/autoconfig.h
	@cp -f $(EXTRPATH_tmp)/.config $(EXTRPATH_tmp)/.config.default
	@cp -f $(EXTRPATH_tmp)/autoconfig.h $(EXTRPATH_tmp)/autoconfig.h.default
	@cp -f ./.machine $(EXTRPATH_tmp)/
	@cp -f ./.include $(EXTRPATH_tmp)/
	@rm -rf ./$(EXTRPATH_tmp)/*.in
	@cp -f ./Makefile ./$(EXTRPATH_tmp)/Makefile
	@echo "CONFIG_SDK_PARTIAL_RELEASE=n" >> ./$(EXTRPATH_tmp)/.config
	@cp ./PKConfig/Lx_SDK/Cmd_Script/*.sh ./$(EXTRPATH_tmp)/
	@chmod 744 ./$(EXTRPATH_tmp)/*.sh
	@cat ./Makefile | sed \
	-e '/ExtractRoot.mk/d' \
	-e '/Extractor/d' -e '/SDK/d' -e '/release/d' \
	-e 's/$$(BINPATH) $$(LIBPATH) $$(IMAGE_PATH)/$$(BINPATH) $$(IMAGE_PATH)/g' \
	> $(EXTRPATH_tmp)/Makefile
