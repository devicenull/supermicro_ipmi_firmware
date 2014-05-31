include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
OBJS := $(call OBJECT_FILE, *.c)

#execute file name 
EXECUTABLE = ipmi_event

all: $(OBJS)
	@$(ECHO_CMD) " LD "$(EXECUTABLE)
	$(Q)$(CC) -o $(BINPATH)/$(EXECUTABLE) $(OBJS) $(LDFLAGS)
	
include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
