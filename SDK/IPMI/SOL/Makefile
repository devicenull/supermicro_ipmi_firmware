include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
OBJS := $(call OBJECT_FILE, *.c OS/Linux/*.c)

LIBNAME = libsol.a
LIBOBJS = OS/Linux/SOLLib.o SOLCommon.o 
EXECUTABLE = ipmi_sol

all:$(OBJS)
	@$(ECHO_CMD) " LD "$(EXECUTABLE)
	$(Q)$(CC)  $(OBJS) -o $(BINPATH)/$(EXECUTABLE) $(LDFLAGS)
	
include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
