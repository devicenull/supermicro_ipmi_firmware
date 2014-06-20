include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
OBJS := $(call OBJECT_FILE, *.c)

EXECUTABLE = reg_op 
all:$(OBJS)
	@$(ECHO_CMD) " LD "$(EXECUTABLE)
	$(Q)$(CC) -o $(FS_PATH)/bin/$(EXECUTABLE) $(OBJS) $(LDFLAGS)
	
include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
