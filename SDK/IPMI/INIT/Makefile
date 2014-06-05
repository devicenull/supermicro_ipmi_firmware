
#execute file name
EXECUTABLE =sysinit 

all: $()
	@echo " LD sysinit"
	$(Q)$(CC) $(CFLAGS) init.o -o ${FS_PATH}/bin/$(EXECUTABLE) $(LDFLAGS)
	@echo " LD getobcl"
	$(Q)$(CC) $(CFLAGS) obcl.o -o ${FS_PATH}/bin/obcl $(LDFLAGS)
	@echo " LD getprelanconf"
	$(Q)$(CC) $(CFLAGS) prelanconf.o -o ${FS_PATH}/prelanconf $(LDFLAGS)




