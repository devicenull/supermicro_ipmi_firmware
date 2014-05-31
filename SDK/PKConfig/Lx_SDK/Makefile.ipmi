include $(PRJ_PATH)/.config
include ./config.in

all:
	@echo "********************* Build IPMI *********************"
	@for i in $(IPMI_BUILD-y); do make -C $$i; done	
	@cd $(PRJ_PATH)/DRIVERS; make

install:

clean:
	@rm -rf $(BINPATH);
	
