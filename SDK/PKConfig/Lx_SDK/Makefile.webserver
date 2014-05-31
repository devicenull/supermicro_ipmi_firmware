-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
WEB_PATH = $(PRJ_PATH)/Web_Server
export WEB_PATH

all:
	@echo "******************** build web server ********************"
	@make -C OS/Linux/

install:
	@echo "******************** install web server ********************"
	@if [ ! -d $(WEBFS_PATH) ];then mkdir -p $(WEBFS_PATH);fi
	make install --directory=$(WEB_PATH)/OS/Linux/Web_Style/$(WEBSTYLE-y)

clean:
	@echo "******************** clean web server ********************"
