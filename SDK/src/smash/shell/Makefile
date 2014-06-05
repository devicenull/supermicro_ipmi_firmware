-include $(PRJ_PATH)/.config
SHELLDST=$(PRJ_PATH)/SMASH/shell/shell
HOST = $(subst -gcc,,$(notdir $(CROSS_COMPILE)gcc))
FLEXSRC_DIR = $(PRJ_PATH)/SMASH/shell/flex-2.5.35
READLINE_DIR = $(PRJ_PATH)/SMASH/shell/readline-5.2
TERMCAP_DIR = $(PRJ_PATH)/SMASH/shell/termcap-1.3.1

LDFLAGS += -lldap_client -L$(PRJ_PATH)/OpenSSL/openssl/local/lib/ -lssl -lcrypto 
CFLAGS+=-Wall -I$(SHELLDST)/include -I$(PRJ_PATH)/SMASH/shell -Os  -DSMASH_VERSION=\"$(SMASH_VERSION)\"
SRC=msh.c lex.yy.c smash_clpxml.c

all:
	@echo "*************Build shell library******************"
	if [ ! -d $(FLEXSRC_DIR) ];then \
	tar zxf flex-2.5.35.tar.gz; \
	cd $(FLEXSRC_DIR); \
	./configure --host=$(HOST) --prefix=$(SHELLDST) CC=$(CC); \
	cp -af config.h_bak config.h; \
	make; \
	make install; fi
	if [ ! -d $(READLINE_DIR) ];then \
	tar zxf readline-5.2.tar.gz; \
	cd $(READLINE_DIR); \
	./configure --host=$(HOST) --prefix=$(SHELLDST) CC=$(CC) --disable-static ; \
	make; \
	rm -rf doc; \
	make install; fi
	if [ ! -d $(TERMCAP_DIR) ];then \
	tar zxf termcap-1.3.1.tar.gz; \
	cd $(TERMCAP_DIR); \
	./configure --host=$(HOST) --prefix=$(SHELLDST) CC=$(CC); \
	make; \
	make install; fi
	@echo "*************Build shell******************"
	#flex parsecmd.l
ifeq ($(CONFIG_EXTRACT_MOD),y)
	@rm -rf msh.o
	$(CC) $(CFLAGS) $(LDFLAGS) -c msh.c
endif
	$(CC) $(CFLAGS) $(LDFLAGS) -o msh $(SRC) -lfl -lreadline -ltermcap -L$(SHELLDST)/lib
	@echo "*************copy shell library******************"
	$(STRIP) --strip-unneeded $(SHELLDST)/lib/*.so*;
	mv -f msh ../;
	cp -df $(SHELLDST)/lib/libreadline.so $(FS_PATH)/lib;
	cp -df $(SHELLDST)/lib/libreadline.so.5 $(FS_PATH)/lib;
	cp -df $(SHELLDST)/lib/libreadline.so.5.2 $(FS_PATH)/lib;
	

clean:
	@echo "*************Clean Flex******************"
	rm -rf $(FLEXSRC_DIR);
	@echo "*************Clean Readline******************"
	rm -rf $(READLINE_DIR);
	@echo "*************Clean Termcap******************"
	rm -rf $(TERMCAP_DIR);
	@echo "*************Clean shell******************"
	rm -rf $(SHELLDST);
	#rm -f msh msh.o lex.yy.c
