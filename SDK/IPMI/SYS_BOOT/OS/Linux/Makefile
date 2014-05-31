include $(PRJ_PATH)/PKConfig/Lx_Script/Makefile.lib
remove = syscall.o
OBJS := $(filter-out $(remove), $(call OBJECT_FILE, *.c))

EXECUTABLE =sysinit 

all: $(OBJS)
	@$(ECHO_CMD) " LD sysinit"
	$(Q)$(CC) $(CFLAGS) init.o -o ${BINPATH}/$(EXECUTABLE) $(LDFLAGS)

include $(PRJ_PATH)/PKConfig/Lx_Script/rule.mk
