cmd_arch/arm/lib/ashldi3.o := /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-gcc -Wp,-MD,arch/arm/lib/.ashldi3.o.d  -nostdinc -isystem /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -mlittle-endian -D__ASSEMBLY__ -mapcs-32 -mno-thumb-interwork -D__LINUX_ARM_ARCH__=5 -march=armv5te -mtune=arm9tdmi -msoft-float    -c -o arch/arm/lib/ashldi3.o arch/arm/lib/ashldi3.S

deps_arch/arm/lib/ashldi3.o := \
  arch/arm/lib/ashldi3.S \
  include/linux/linkage.h \
  include/linux/config.h \
    $(wildcard include/config/h.h) \
  include/asm/linkage.h \

arch/arm/lib/ashldi3.o: $(deps_arch/arm/lib/ashldi3.o)

$(deps_arch/arm/lib/ashldi3.o):
