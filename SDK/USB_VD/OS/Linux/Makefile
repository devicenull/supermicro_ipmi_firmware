CFLAGS += -O3
LDFLAGS += -lpthread -lusb

LIBNAME = libusb.a

all: $(OBJS)
	@echo " LD usb"
	$(Q)$(CC) $(CFLAGS) USB.o  -o $(FS_PATH)/bin/usb $(LDFLAGS)
	$(Q)$(STRIP) $(BINPATH)/usb
	@echo " LD uima"
	$(Q)$(CC) $(CFLAGS) uima.o  -o ${FS_PATH}/bin/uima $(LDFLAGS)
	$(Q)$(STRIP) ${BINPATH}/uima
	@echo " LD uiso"
	$(Q)$(CC) $(CFLAGS) uiso.o  -o ${FS_PATH}/bin//uiso $(LDFLAGS)
	$(Q)$(STRIP) ${BINPATH}/uiso

CLEAN_PATTERN = '*.o' '*.d'
CLEAN_FILES = $(call GEN_SEARCH_LIST,$(CLEAN_PATTERN),./,,)
include $(PRJ_PATH)/PKConfig/Lx_Script/clean.mk
include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
