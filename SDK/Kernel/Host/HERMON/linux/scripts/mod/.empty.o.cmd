cmd_scripts/mod/empty.o := /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-gcc -Wp,-MD,scripts/mod/.empty.o.d  -nostdinc -isystem /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -fno-omit-frame-pointer -fno-optimize-sibling-calls -D__linux__ -Dlinux -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mapcs-32 -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -malignment-traps -msoft-float -Uarm -Wdeclaration-after-statement    -gdwarf-2 -w -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(empty)"  -D"KBUILD_MODNAME=KBUILD_STR(empty)" -c -o scripts/mod/.tmp_empty.o scripts/mod/empty.c

deps_scripts/mod/empty.o := \
  scripts/mod/empty.c \

scripts/mod/empty.o: $(deps_scripts/mod/empty.o)

$(deps_scripts/mod/empty.o):
