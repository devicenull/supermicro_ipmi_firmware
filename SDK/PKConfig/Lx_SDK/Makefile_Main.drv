include $(PRJ_PATH)/.config
include ./config.in

all:
	@echo "********************* Build Drivers *********************"
	@for i in $(DRIVER_BUILD-y); do make -C $$i; done	

install:

clean:
	@rm -rf $(BINPATH);
	
