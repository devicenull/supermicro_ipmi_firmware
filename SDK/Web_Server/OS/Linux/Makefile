-include $(PRJ_PATH)/.SDKPath
-include $(PRJ_PATH)/.config
-include $(PRJ_PATH)/.machine
include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib

WEB_PATH = $(PRJ_PATH)/Web_Server
WEB_STYLE_PATH = $(PRJ_PATH)/Web_Server/OS/Linux/Web_Style/$(WEBSTYLE)
PRJ_CGI_PATH = $(PRJ_PATH)/Web_Server/OS/Linux/Host/$(MYMCU)/Board/$(BOARD)
CGIPATH=$(PRJ_PATH)/Web_Server/OS/Linux
LIBNAME=libweb.a
CGILIB_OBJS = $(call OBJECT_FILE,$(CGIPATH)/lib/*.c $(CGIPATH)/Web_Style/$(WEBSTYLE)/*.c \
			$(CGIPATH)/OEM/$(MYMCU)/Board/$(BOARD)/*.c)
-include $(WEB_STYLE_PATH)/cgi_src/oem_cgi.in
	
CGI_DEPS = $(call DEPENDENCY_FILE,$(CGILIB_OBJS) $(CGIPATH)/*.c $(OEM_CGI_SRC))
CGI_OBJS = $(call OBJECT_FILE, $(CGIPATH)/*.c $(OEM_CGI_SRC))

LDFLAGS += -lcgi -lweb -lldap_client -lradius_client -lutility -L$(PRJ_PATH)/OpenSSL/openssl/local/lib/ -lssl -lcrypto
CFLAGS += -Werror -I$(PRJ_PATH)/libXML2/libxml2/local/include/libxml2/ 
CFLAGS+= -D_FORTIFY_SOURCE=2 -O2 -Wformat -Wformat-security
LDFLAGS +=  -Wl,-z,relro -Wl,-z,now

export CGILIB_OBJS  LDFLAGS CFLAGS

all: lib $(CGI_OBJS) $(CGI_OBJS:.o=.cgi)
%.o:%.c 
	$(Q)$(ECHO_CMD) " CC "$@
	$(CC) $(CFLAGS) -c $< -o $@

%.d:%.c
	$(Q)$(CC) -M $(CFLAGS) $< > $@.$$$$;\
	sed 's,\($*\).o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@;\
	rm -f   $@.$$$$

%.cgi:%.o $(CGILIB_OBJS)
	$(Q)$(ECHO_CMD) "Build CGI file: "$@"....."
	$(Q)$(CC)  $(LDFLAGS)  $< -o $@ 
	$(Q)$(ECHO_CMD) "Strip CGI file: "$@"....."
	$(Q)$(STRIP) $@


.PHONY: lib
lib : $(CGILIB_OBJS)
	$(Q)$(ECHO_CMD) "Building web library................."
	$(Q)$(CC)  -Wl -shared   -o $(LIBPATH)/$(LIBNAME:.a=.so) $^	
-include $(CGI_DEPS)

CLEAN_PATTERN = '*.o' '*.d' '*.cgi'
CLEAN_FILES = $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),./,,)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk

clean_all: clean

extract:
	@echo "******************** extract web data ********************"
#Common CGI
	@if [ ! -d $(SDK_WEBPATH_CGI) ];then mkdir -p $(SDK_WEBPATH_CGI); \
	cp -f $(CGILIBPATH)/*.cgi $(SDK_WEBPATH_CGI)/ ; \
	cp -f $(PRJ_PATH)/PKConfig/Lx_SDK/Makefile.webcgi $(SDK_WEBPATH_CGI)/Makefile ; fi
	@make -C $(WEB_STYLE_PATH) extract;
