-include $(PRJ_PATH)/.SDKPath
-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine

WEB_PATH = $(PRJ_PATH)/Web_Server
export WEB_PATH

#SDK configuration
SDK_WEBPATH = $(SDKPATH)/Web_Server
SDK_WEBPATH_CGI = $(SDKPATH)/Web_Server/OS/Linux
SDK_WEBPATH_WEB_STYLE = $(SDK_WEBPATH_CGI)/Web_Style/$(WEBSTYLE)
SDK_WEBPATH_BOARD_SPECIFIC_CGI = $(SDK_WEBPATH_WEB_STYLE)/cgi_src
#SDK_WEBPATH_CGI_LIB = $(SDK_WEBPATH_CGI)/lib
export SDK_WEBPATH SDK_WEBPATH_CGI SDK_WEBPATH_WEB_STYLE SDK_WEBPATH_BOARD_SPECIFIC_CGI #SDK_WEBPATH_CGI_LIB

all:
	@make -C OS/Linux/
	
install:
#copy WEB pages/CGI/Java
	@if [ ! -d $(WEBFS_PATH) ];then mkdir -p $(WEBFS_PATH);fi
	make install --directory=$(WEB_PATH)/OS/Linux/Web_Style/$(WEBSTYLE-y)
	
clean:
	@make -C OS/Linux/ clean

clean_all: clean

SDK_FEATURE_NAME = Web_Server

PATTERN = '*.c' '*.h' '*.in' 'Makefile' '*.js' '*.css' '*.www' '*.htm' '*.html' \
 '*.png' '*.gif' '*.bmp' '*.jnlp' '*.ico' '*.jpg' '*.tar.gz'
FILE_LIST = $(call GEN_SEARCH_LIST,$(PATTERN),$(foreach n,$(WEBSTYLE),OS/Linux/Web_Style/$(n)/),,)


FILE_LIST +=$(wildcard ./Makefile \
./httpd/httpd \
./httpd/*.tar.gz \
./httpd/*.conf \
./httpd/*.pem \
./httpd/Makefile \
./OS/Linux/*.c \
./OS/Linux/Makefile \
./OS/Linux/inc/*.h \
./OS/Linux/lib/*.c \
./OS/Linux/OEM/$(MYMCU)/Board/$(BOARD)/*.c \
./OS/Linux/lib/Makefile)

ifeq ($(CONFIG_EXTRACT_ALL_OBJ),y)
SDK_FILE_CP_LIST = $(FILE_LIST:.c=.o)
else
SDK_FILE_CP_LIST = $(FILE_LIST)
endif
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk

