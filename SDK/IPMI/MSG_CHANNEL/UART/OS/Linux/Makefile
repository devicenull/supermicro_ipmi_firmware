include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
OBJS := $(call OBJECT_FILE, *.c)

EXECUTABLE=ipmi_uart

LIBNAME = libuart.a
LIBOBJS = UARTLib.o ../../../../SOL/$(HOSTPATH)/SOLHWInfo.o

all:$(OBJS)
	@$(ECHO_CMD) " LD "$(EXECUTABLE)
	$(Q)$(CC)  $(OBJS) -o $(BINPATH)/$(EXECUTABLE) $(LDFLAGS)


include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
