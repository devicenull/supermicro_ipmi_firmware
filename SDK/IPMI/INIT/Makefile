
#execute file name
EXECUTABLE =sysinit 

all: $()
	@echo " LD sysinit"
	$(Q)$(CC) $(CFLAGS) init.o -o ${BINPATH}/$(EXECUTABLE) $(LDFLAGS)
	@echo " LD getobcl"
	$(Q)$(CC) $(CFLAGS) obcl.o -o ${BINPATH}/obcl $(LDFLAGS)
	@echo " LD getprelanconf"
	$(Q)$(CC) $(CFLAGS) prelanconf.o -o ${BINPATH}/prelanconf $(LDFLAGS)




