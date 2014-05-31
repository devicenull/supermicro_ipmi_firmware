include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
OBJS :=$(filter-out Net_API.o , $(call OBJECT_FILE, *.c OS/Linux/*.c))

#execute file name  
EXECUTABLE = ipmi_lan


all:$(OBJS)
	@echo " LD "$(EXECUTABLE)
	$(Q)$(CC)  $(OBJS) -o $(BINPATH)/$(EXECUTABLE) $(LDFLAGS)
include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
