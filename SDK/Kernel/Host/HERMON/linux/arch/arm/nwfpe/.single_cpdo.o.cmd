cmd_arch/arm/nwfpe/single_cpdo.o := /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-gcc -Wp,-MD,arch/arm/nwfpe/.single_cpdo.o.d  -nostdinc -isystem /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Os -fno-omit-frame-pointer -fno-optimize-sibling-calls -D__linux__ -Dlinux -fno-omit-frame-pointer -mapcs -mno-sched-prolog -mapcs-32 -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -malignment-traps -msoft-float -Uarm -Wdeclaration-after-statement    -gdwarf-2 -w -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(single_cpdo)"  -D"KBUILD_MODNAME=KBUILD_STR(nwfpe)" -c -o arch/arm/nwfpe/.tmp_single_cpdo.o arch/arm/nwfpe/single_cpdo.c

deps_arch/arm/nwfpe/single_cpdo.o := \
  arch/arm/nwfpe/single_cpdo.c \
  arch/arm/nwfpe/fpa11.h \
    $(wildcard include/config/fpe/nwfpe/xp.h) \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/linux/thread_info.h \
  include/linux/bitops.h \
  include/asm/types.h \
  include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  include/linux/compiler.h \
  include/linux/compiler-gcc3.h \
  include/linux/compiler-gcc.h \
  include/asm/system.h \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm/thread_info.h \
  include/asm/fpstate.h \
    $(wildcard include/config/iwmmxt.h) \
  include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
  include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
  arch/arm/nwfpe/fpsr.h \
  arch/arm/nwfpe/milieu.h \
  arch/arm/nwfpe/ARM-gcc.h \
  arch/arm/nwfpe/softfloat.h \
  arch/arm/nwfpe/fpopcode.h \

arch/arm/nwfpe/single_cpdo.o: $(deps_arch/arm/nwfpe/single_cpdo.o)

$(deps_arch/arm/nwfpe/single_cpdo.o):
