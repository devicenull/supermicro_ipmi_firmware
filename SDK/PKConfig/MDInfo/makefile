OBJ=mdinfo.o 
OUTNAME=mdinfo
all:$(OBJ)
	$(Q)echo "================  Build MDInfo ================"
	$(Q)gcc -o $(OUTNAME) $(OBJ) -Wall 
clean:
	-rm -f $(OUTNAME) $(OBJ) *.bak No*
%.o:%.c
	$(Q)gcc -c $<
install:

DEFAULT_PATTERN += '*.sh' 'Makefile'
include $(PRJ_PATH)/PKConfig/Lx_Script/Extract.mk
