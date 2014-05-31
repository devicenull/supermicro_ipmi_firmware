cmd_miscutils/bbconfig.o := /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/arm-linux-gcc -Wp,-MD,miscutils/.bbconfig.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.12.0)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement  -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os -fpic     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(bbconfig)"  -D"KBUILD_MODNAME=KBUILD_STR(bbconfig)" -c -o miscutils/bbconfig.o miscutils/bbconfig.c

deps_miscutils/bbconfig.o := \
  miscutils/bbconfig.c \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/buffer.h) \
    $(wildcard include/config/ubuffer.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/inux.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/o.h) \
    $(wildcard include/config/ntf.h) \
    $(wildcard include/config/t.h) \
    $(wildcard include/config/l.h) \
    $(wildcard include/config/wn.h) \
    $(wildcard include/config/.h) \
    $(wildcard include/config/ktop.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/debug/crond/option.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/ture/editing/savehistory.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config///.h) \
    $(wildcard include/config/nommu.h) \
    $(wildcard include/config//nommu.h) \
    $(wildcard include/config//mmu.h) \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/byteswap.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/byteswap.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/endian.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/features.h \
    $(wildcard include/config/c99.h) \
    $(wildcard include/config/ix.h) \
    $(wildcard include/config/ix2.h) \
    $(wildcard include/config/ix199309.h) \
    $(wildcard include/config/ix199506.h) \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/en/extended.h) \
    $(wildcard include/config/x98.h) \
    $(wildcard include/config/en2k.h) \
    $(wildcard include/config/gefile.h) \
    $(wildcard include/config/gefile64.h) \
    $(wildcard include/config/e/offset64.h) \
    $(wildcard include/config/d.h) \
    $(wildcard include/config/c.h) \
    $(wildcard include/config/ntrant.h) \
    $(wildcard include/config/tify/level.h) \
    $(wildcard include/config/i.h) \
    $(wildcard include/config/ern/inlines.h) \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/cdefs.h \
    $(wildcard include/config/espaces.h) \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/gnu/stubs.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/endian.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/arpa/inet.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/netinet/in.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/stdint.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/wchar.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/wordsize.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/socket.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/uio.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/types.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/types.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include/stddef.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/typesizes.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/time.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/select.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/select.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sigset.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/time.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/sysmacros.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/pthreadtypes.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sched.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/uio.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/socket.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include/limits.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include/syslimits.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/limits.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/posix1_lim.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/local_lim.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/linux/limits.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/posix2_lim.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/xopen_lim.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/stdio_lim.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sockaddr.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/socket.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/sockios.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/in.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include/stdbool.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/mount.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/ioctl.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/ioctls.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/ioctls.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/ioctl.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm-generic/ioctl.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/ioctl-types.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/ttydefaults.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/ctype.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/xlocale.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/dirent.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/dirent.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/errno.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/errno.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/linux/errno.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/errno.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm-generic/errno.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm-generic/errno-base.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/fcntl.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/fcntl.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/stat.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/stat.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/inttypes.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/netdb.h \
    $(wildcard include/config/3/ascii/rules.h) \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/rpc/netdb.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/siginfo.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/netdb.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/setjmp.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/setjmp.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/signal.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/signum.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sigaction.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sigcontext.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/sigcontext.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sigstack.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/ucontext.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/procfs.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/time.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/user.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sigthread.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/stdio.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/libio.h \
    $(wildcard include/config/a.h) \
    $(wildcard include/config/ar/t.h) \
    $(wildcard include/config//io/file.h) \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/_G_config.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/wchar.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/gconv.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/include/stdarg.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/sys_errlist.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/stdlib.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/waitflags.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/waitstatus.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/alloca.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/string.h \
    $(wildcard include/config/ing/inlines.h) \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/poll.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/poll.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/mman.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/mman.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/wait.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/resource.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/resource.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/termios.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/termios.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/unistd.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/posix_opt.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/environments.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/confname.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/getopt.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/utime.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/param.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/linux/param.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/asm/param.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/mntent.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/paths.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/sys/statfs.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/statfs.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/locale.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/bits/locale.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/pwd.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/grp.h \
  /root/ATEN/source/OEM/STD_V317_LOGO/STD_X9_237_Security_X10/SDK/ToolChain/Host/HERMON/gcc-3.4.4-glibc-2.3.5-armv4/arm-linux/bin/../lib/gcc/arm-linux/3.4.4/../../../../arm-linux/sys-include/shadow.h \
  include/xatonum.h \
  include/bbconfigopts.h \
    $(wildcard include/config/have/dot/config.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/extra/compat.h) \
    $(wildcard include/config/feature/assume/unicode.h) \
    $(wildcard include/config/feature/buffers/use/malloc.h) \
    $(wildcard include/config/loc.h) \
    $(wildcard include/config/show/usage.h) \
    $(wildcard include/config/feature/verbose/usage.h) \
    $(wildcard include/config/feature/compress/usage.h) \
    $(wildcard include/config/feature/installer.h) \
    $(wildcard include/config/feature/devpts.h) \
    $(wildcard include/config/feature/clean/up.h) \
    $(wildcard include/config/feature/suid.h) \
    $(wildcard include/config/feature/suid/config.h) \
    $(wildcard include/config/feature/suid/config/quiet.h) \
    $(wildcard include/config/quiet.h) \
    $(wildcard include/config/feature/have/rpc.h) \
    $(wildcard include/config/static.h) \
    $(wildcard include/config/pie.h) \
    $(wildcard include/config/build/libbusybox.h) \
    $(wildcard include/config/feature/shared/busybox.h) \
    $(wildcard include/config/cross/compiler/prefix.h) \
    $(wildcard include/config/debug.h) \
    $(wildcard include/config/debug/pessimize.h) \
    $(wildcard include/config/no/debug/lib.h) \
    $(wildcard include/config/dmalloc.h) \
    $(wildcard include/config/efence.h) \
    $(wildcard include/config/include/susv2.h) \
    $(wildcard include/config/parse.h) \
    $(wildcard include/config/install/no/usr.h) \
    $(wildcard include/config/install/applet/symlinks.h) \
    $(wildcard include/config/install/applet/hardlinks.h) \
    $(wildcard include/config/install/applet/script/wrappers.h) \
    $(wildcard include/config/install/applet/dont.h) \
    $(wildcard include/config/install/sh/applet/symlink.h) \
    $(wildcard include/config/install/sh/applet/hardlink.h) \
    $(wildcard include/config/install/sh/applet/script/wrapper.h) \
    $(wildcard include/config/prefix.h) \
    $(wildcard include/config/password/minlen.h) \
    $(wildcard include/config/md5/size/vs/speed.h) \
    $(wildcard include/config/feature/fast/top.h) \
    $(wildcard include/config/feature/etc/networks.h) \
    $(wildcard include/config/feature/editing/max/len.h) \
    $(wildcard include/config/feature/editing/fancy/prompt.h) \
    $(wildcard include/config/feature/verbose/cp/message.h) \
    $(wildcard include/config/feature/copybuf/kb.h) \
    $(wildcard include/config/monotonic/syscall.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/ar.h) \
    $(wildcard include/config/feature/ar/long/filenames.h) \
    $(wildcard include/config/bunzip2.h) \
    $(wildcard include/config/bzip2.h) \
    $(wildcard include/config/cpio.h) \
    $(wildcard include/config/feature/cpio/o.h) \
    $(wildcard include/config/dpkg.h) \
    $(wildcard include/config/dpkg/deb.h) \
    $(wildcard include/config/feature/dpkg/deb/extract/only.h) \
    $(wildcard include/config/gunzip.h) \
    $(wildcard include/config/gzip.h) \
    $(wildcard include/config/rpm2cpio.h) \
    $(wildcard include/config/rpm.h) \
    $(wildcard include/config/tar.h) \
    $(wildcard include/config/feature/tar/create.h) \
    $(wildcard include/config/feature/tar/autodetect.h) \
    $(wildcard include/config/feature/tar/from.h) \
    $(wildcard include/config/feature/tar/oldgnu/compatibility.h) \
    $(wildcard include/config/feature/tar/oldsun/compatibility.h) \
    $(wildcard include/config/feature/tar/gnu/extensions.h) \
    $(wildcard include/config/feature/tar/long/options.h) \
    $(wildcard include/config/feature/tar/uname/gname.h) \
    $(wildcard include/config/uncompress.h) \
    $(wildcard include/config/unlzma.h) \
    $(wildcard include/config/feature/lzma/fast.h) \
    $(wildcard include/config/unzip.h) \
    $(wildcard include/config/basename.h) \
    $(wildcard include/config/cal.h) \
    $(wildcard include/config/cat.h) \
    $(wildcard include/config/catv.h) \
    $(wildcard include/config/chgrp.h) \
    $(wildcard include/config/chmod.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/chroot.h) \
    $(wildcard include/config/cksum.h) \
    $(wildcard include/config/comm.h) \
    $(wildcard include/config/cp.h) \
    $(wildcard include/config/cut.h) \
    $(wildcard include/config/date.h) \
    $(wildcard include/config/feature/date/isofmt.h) \
    $(wildcard include/config/dd.h) \
    $(wildcard include/config/feature/dd/signal/handling.h) \
    $(wildcard include/config/feature/dd/ibs/obs.h) \
    $(wildcard include/config/df.h) \
    $(wildcard include/config/feature/df/inode.h) \
    $(wildcard include/config/dirname.h) \
    $(wildcard include/config/dos2unix.h) \
    $(wildcard include/config/unix2dos.h) \
    $(wildcard include/config/du.h) \
    $(wildcard include/config/feature/du/default/blocksize/1k.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/feature/fancy/echo.h) \
    $(wildcard include/config/env.h) \
    $(wildcard include/config/feature/env/long/options.h) \
    $(wildcard include/config/expand.h) \
    $(wildcard include/config/feature/expand/long/options.h) \
    $(wildcard include/config/expr.h) \
    $(wildcard include/config/expr/math/support/64.h) \
    $(wildcard include/config/false.h) \
    $(wildcard include/config/fold.h) \
    $(wildcard include/config/head.h) \
    $(wildcard include/config/feature/fancy/head.h) \
    $(wildcard include/config/hostid.h) \
    $(wildcard include/config/id.h) \
    $(wildcard include/config/install.h) \
    $(wildcard include/config/feature/install/long/options.h) \
    $(wildcard include/config/length.h) \
    $(wildcard include/config/ln.h) \
    $(wildcard include/config/logname.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/feature/ls/filetypes.h) \
    $(wildcard include/config/feature/ls/followlinks.h) \
    $(wildcard include/config/feature/ls/recursive.h) \
    $(wildcard include/config/feature/ls/sortfiles.h) \
    $(wildcard include/config/feature/ls/timestamps.h) \
    $(wildcard include/config/feature/ls/username.h) \
    $(wildcard include/config/feature/ls/color.h) \
    $(wildcard include/config/feature/ls/color/is/default.h) \
    $(wildcard include/config/md5sum.h) \
    $(wildcard include/config/mkdir.h) \
    $(wildcard include/config/feature/mkdir/long/options.h) \
    $(wildcard include/config/mkfifo.h) \
    $(wildcard include/config/mknod.h) \
    $(wildcard include/config/mv.h) \
    $(wildcard include/config/feature/mv/long/options.h) \
    $(wildcard include/config/nice.h) \
    $(wildcard include/config/nohup.h) \
    $(wildcard include/config/od.h) \
    $(wildcard include/config/printenv.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/pwd.h) \
    $(wildcard include/config/readlink.h) \
    $(wildcard include/config/feature/readlink/follow.h) \
    $(wildcard include/config/realpath.h) \
    $(wildcard include/config/rm.h) \
    $(wildcard include/config/rmdir.h) \
    $(wildcard include/config/feature/rmdir/long/options.h) \
    $(wildcard include/config/seq.h) \
    $(wildcard include/config/sha1sum.h) \
    $(wildcard include/config/sleep.h) \
    $(wildcard include/config/feature/fancy/sleep.h) \
    $(wildcard include/config/feature/float/sleep.h) \
    $(wildcard include/config/sort.h) \
    $(wildcard include/config/feature/sort/big.h) \
    $(wildcard include/config/split.h) \
    $(wildcard include/config/feature/split/fancy.h) \
    $(wildcard include/config/stat.h) \
    $(wildcard include/config/feature/stat/format.h) \
    $(wildcard include/config/stty.h) \
    $(wildcard include/config/sum.h) \
    $(wildcard include/config/sync.h) \
    $(wildcard include/config/tac.h) \
    $(wildcard include/config/tail.h) \
    $(wildcard include/config/feature/fancy/tail.h) \
    $(wildcard include/config/tee.h) \
    $(wildcard include/config/feature/tee/use/block/io.h) \
    $(wildcard include/config/ck/io.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/feature/test/64.h) \
    $(wildcard include/config/touch.h) \
    $(wildcard include/config/tr.h) \
    $(wildcard include/config/feature/tr/classes.h) \
    $(wildcard include/config/feature/tr/equiv.h) \
    $(wildcard include/config/true.h) \
    $(wildcard include/config/tty.h) \
    $(wildcard include/config/uname.h) \
    $(wildcard include/config/unexpand.h) \
    $(wildcard include/config/feature/unexpand/long/options.h) \
    $(wildcard include/config/uniq.h) \
    $(wildcard include/config/usleep.h) \
    $(wildcard include/config/uudecode.h) \
    $(wildcard include/config/uuencode.h) \
    $(wildcard include/config/wc.h) \
    $(wildcard include/config/feature/wc/large.h) \
    $(wildcard include/config/who.h) \
    $(wildcard include/config/whoami.h) \
    $(wildcard include/config/yes.h) \
    $(wildcard include/config/feature/preserve/hardlinks.h) \
    $(wildcard include/config/feature/autowidth.h) \
    $(wildcard include/config/feature/human/readable.h) \
    $(wildcard include/config/feature/md5/sha1/sum/check.h) \
    $(wildcard include/config/chvt.h) \
    $(wildcard include/config/clear.h) \
    $(wildcard include/config/deallocvt.h) \
    $(wildcard include/config/dumpkmap.h) \
    $(wildcard include/config/kbd/mode.h) \
    $(wildcard include/config/loadfont.h) \
    $(wildcard include/config/loadkmap.h) \
    $(wildcard include/config/openvt.h) \
    $(wildcard include/config/reset.h) \
    $(wildcard include/config/resize.h) \
    $(wildcard include/config/feature/resize/print.h) \
    $(wildcard include/config/setconsole.h) \
    $(wildcard include/config/feature/setconsole/long/options.h) \
    $(wildcard include/config/setfont.h) \
    $(wildcard include/config/setkeycodes.h) \
    $(wildcard include/config/setlogcons.h) \
    $(wildcard include/config/showkey.h) \
    $(wildcard include/config/mktemp.h) \
    $(wildcard include/config/pipe/progress.h) \
    $(wildcard include/config/run/parts.h) \
    $(wildcard include/config/feature/run/parts/long/options.h) \
    $(wildcard include/config/feature/run/parts/fancy.h) \
    $(wildcard include/config/start/stop/daemon.h) \
    $(wildcard include/config/feature/start/stop/daemon/fancy.h) \
    $(wildcard include/config/feature/start/stop/daemon/long/options.h) \
    $(wildcard include/config/which.h) \
    $(wildcard include/config/awk.h) \
    $(wildcard include/config/feature/awk/math.h) \
    $(wildcard include/config/cmp.h) \
    $(wildcard include/config/diff.h) \
    $(wildcard include/config/feature/diff/binary.h) \
    $(wildcard include/config/feature/diff/dir.h) \
    $(wildcard include/config/feature/diff/minimal.h) \
    $(wildcard include/config/ed.h) \
    $(wildcard include/config/patch.h) \
    $(wildcard include/config/sed.h) \
    $(wildcard include/config/vi.h) \
    $(wildcard include/config/feature/vi/max/len.h) \
    $(wildcard include/config/feature/vi/8bit.h) \
    $(wildcard include/config/feature/vi/colon.h) \
    $(wildcard include/config/feature/vi/yankmark.h) \
    $(wildcard include/config/feature/vi/search.h) \
    $(wildcard include/config/feature/vi/use/signals.h) \
    $(wildcard include/config/nals.h) \
    $(wildcard include/config/feature/vi/dot/cmd.h) \
    $(wildcard include/config/feature/vi/readonly.h) \
    $(wildcard include/config/feature/vi/setopts.h) \
    $(wildcard include/config/feature/vi/set.h) \
    $(wildcard include/config/feature/vi/win/resize.h) \
    $(wildcard include/config/feature/vi/optimize/cursor.h) \
    $(wildcard include/config/feature/allow/exec.h) \
    $(wildcard include/config/find.h) \
    $(wildcard include/config/feature/find/print0.h) \
    $(wildcard include/config/feature/find/mtime.h) \
    $(wildcard include/config/feature/find/mmin.h) \
    $(wildcard include/config/feature/find/perm.h) \
    $(wildcard include/config/feature/find/type.h) \
    $(wildcard include/config/feature/find/xdev.h) \
    $(wildcard include/config/feature/find/maxdepth.h) \
    $(wildcard include/config/feature/find/newer.h) \
    $(wildcard include/config/feature/find/inum.h) \
    $(wildcard include/config/feature/find/exec.h) \
    $(wildcard include/config/feature/find/user.h) \
    $(wildcard include/config/feature/find/group.h) \
    $(wildcard include/config/feature/find/not.h) \
    $(wildcard include/config/feature/find/depth.h) \
    $(wildcard include/config/feature/find/paren.h) \
    $(wildcard include/config/feature/find/size.h) \
    $(wildcard include/config/feature/find/prune.h) \
    $(wildcard include/config/feature/find/delete.h) \
    $(wildcard include/config/feature/find/path.h) \
    $(wildcard include/config/feature/find/regex.h) \
    $(wildcard include/config/feature/find/context.h) \
    $(wildcard include/config/grep.h) \
    $(wildcard include/config/feature/grep/egrep/alias.h) \
    $(wildcard include/config/feature/grep/fgrep/alias.h) \
    $(wildcard include/config/feature/grep/context.h) \
    $(wildcard include/config/xargs.h) \
    $(wildcard include/config/feature/xargs/support/confirmation.h) \
    $(wildcard include/config/feature/xargs/support/quotes.h) \
    $(wildcard include/config/feature/xargs/support/termopt.h) \
    $(wildcard include/config/feature/xargs/support/zero/term.h) \
    $(wildcard include/config/init.h) \
    $(wildcard include/config/debug/init.h) \
    $(wildcard include/config/feature/use/inittab.h) \
    $(wildcard include/config/ttab.h) \
    $(wildcard include/config/feature/kill/removed.h) \
    $(wildcard include/config/feature/kill/delay.h) \
    $(wildcard include/config/feature/init/sctty.h) \
    $(wildcard include/config/feature/init/syslog.h) \
    $(wildcard include/config/feature/extra/quiet.h) \
    $(wildcard include/config/feature/init/coredumps.h) \
    $(wildcard include/config/feature/initrd.h) \
    $(wildcard include/config/halt.h) \
    $(wildcard include/config/mesg.h) \
    $(wildcard include/config/addgroup.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/delgroup.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/adduser.h) \
    $(wildcard include/config/feature/adduser/long/options.h) \
    $(wildcard include/config/deluser.h) \
    $(wildcard include/config/getty.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/feature/wtmp.h) \
    $(wildcard include/config/login.h) \
    $(wildcard include/config/pam.h) \
    $(wildcard include/config/login/scripts.h) \
    $(wildcard include/config/feature/nologin.h) \
    $(wildcard include/config/feature/securetty.h) \
    $(wildcard include/config/passwd.h) \
    $(wildcard include/config/feature/passwd/weak/check.h) \
    $(wildcard include/config/cryptpw.h) \
    $(wildcard include/config/chpasswd.h) \
    $(wildcard include/config/su.h) \
    $(wildcard include/config/feature/su/syslog.h) \
    $(wildcard include/config/feature/su/checks/shells.h) \
    $(wildcard include/config/sulogin.h) \
    $(wildcard include/config/vlock.h) \
    $(wildcard include/config/chattr.h) \
    $(wildcard include/config/fsck.h) \
    $(wildcard include/config/lsattr.h) \
    $(wildcard include/config/modprobe/small.h) \
    $(wildcard include/config/feature/modprobe/small/options/on/cmdline.h) \
    $(wildcard include/config/feature/modprobe/small/check/already/loaded.h) \
    $(wildcard include/config/depmod.h) \
    $(wildcard include/config/feature/depmod/prune/fancy.h) \
    $(wildcard include/config/feature/depmod/alias.h) \
    $(wildcard include/config/insmod.h) \
    $(wildcard include/config/feature/insmod/version/checking.h) \
    $(wildcard include/config/feature/insmod/ksymoops/symbols.h) \
    $(wildcard include/config/feature/insmod/loadinkmem.h) \
    $(wildcard include/config/feature/insmod/load/map.h) \
    $(wildcard include/config/feature/insmod/load/map/full.h) \
    $(wildcard include/config/rmmod.h) \
    $(wildcard include/config/lsmod.h) \
    $(wildcard include/config/feature/lsmod/pretty/2/6/output.h) \
    $(wildcard include/config/modprobe.h) \
    $(wildcard include/config/feature/modprobe/multiple/options.h) \
    $(wildcard include/config/feature/modprobe/fancy/alias.h) \
    $(wildcard include/config/feature/modprobe/blacklist.h) \
    $(wildcard include/config/feature/check/tainted/module.h) \
    $(wildcard include/config/feature/2/4/modules.h) \
    $(wildcard include/config/feature/2/6/modules.h) \
    $(wildcard include/config/default/modules/dir.h) \
    $(wildcard include/config/default/depmod/file.h) \
    $(wildcard include/config/feature/query/module/interface.h) \
    $(wildcard include/config/dmesg.h) \
    $(wildcard include/config/feature/dmesg/pretty.h) \
    $(wildcard include/config/fbset.h) \
    $(wildcard include/config/feature/fbset/fancy.h) \
    $(wildcard include/config/feature/fbset/readmode.h) \
    $(wildcard include/config/fdflush.h) \
    $(wildcard include/config/fdformat.h) \
    $(wildcard include/config/fdisk.h) \
    $(wildcard include/config/fdisk/support/large/disks.h) \
    $(wildcard include/config/feature/fdisk/writable.h) \
    $(wildcard include/config/feature/aix/label.h) \
    $(wildcard include/config/feature/sgi/label.h) \
    $(wildcard include/config/feature/sun/label.h) \
    $(wildcard include/config/feature/osf/label.h) \
    $(wildcard include/config/feature/fdisk/advanced.h) \
    $(wildcard include/config/findfs.h) \
    $(wildcard include/config/freeramdisk.h) \
    $(wildcard include/config/fsck/minix.h) \
    $(wildcard include/config/mkfs/minix.h) \
    $(wildcard include/config/feature/minix2.h) \
    $(wildcard include/config/getopt.h) \
    $(wildcard include/config/hexdump.h) \
    $(wildcard include/config/feature/hexdump/reverse.h) \
    $(wildcard include/config/hd.h) \
    $(wildcard include/config/hwclock.h) \
    $(wildcard include/config/feature/hwclock/long/options.h) \
    $(wildcard include/config/feature/hwclock/adjtime/fhs.h) \
    $(wildcard include/config/ipcrm.h) \
    $(wildcard include/config/ipcs.h) \
    $(wildcard include/config/losetup.h) \
    $(wildcard include/config/mdev.h) \
    $(wildcard include/config/feature/mdev/conf.h) \
    $(wildcard include/config/feature/mdev/rename.h) \
    $(wildcard include/config/feature/mdev/rename/regexp.h) \
    $(wildcard include/config/feature/mdev/exec.h) \
    $(wildcard include/config/feature/mdev/load/firmware.h) \
    $(wildcard include/config/mkswap.h) \
    $(wildcard include/config/feature/mkswap/v0.h) \
    $(wildcard include/config/more.h) \
    $(wildcard include/config/feature/use/termios.h) \
    $(wildcard include/config/mios.h) \
    $(wildcard include/config/volumeid.h) \
    $(wildcard include/config/feature/volumeid/ext.h) \
    $(wildcard include/config/feature/volumeid/reiserfs.h) \
    $(wildcard include/config/feature/volumeid/fat.h) \
    $(wildcard include/config/feature/volumeid/hfs.h) \
    $(wildcard include/config/feature/volumeid/jfs.h) \
    $(wildcard include/config/feature/volumeid/xfs.h) \
    $(wildcard include/config/feature/volumeid/ntfs.h) \
    $(wildcard include/config/feature/volumeid/iso9660.h) \
    $(wildcard include/config/feature/volumeid/udf.h) \
    $(wildcard include/config/feature/volumeid/luks.h) \
    $(wildcard include/config/feature/volumeid/linuxswap.h) \
    $(wildcard include/config/feature/volumeid/cramfs.h) \
    $(wildcard include/config/feature/volumeid/romfs.h) \
    $(wildcard include/config/feature/volumeid/sysv.h) \
    $(wildcard include/config/feature/volumeid/ocfs2.h) \
    $(wildcard include/config/feature/volumeid/linuxraid.h) \
    $(wildcard include/config/mount.h) \
    $(wildcard include/config/feature/mount/fake.h) \
    $(wildcard include/config/feature/mount/verbose.h) \
    $(wildcard include/config/feature/mount/helpers.h) \
    $(wildcard include/config/feature/mount/label.h) \
    $(wildcard include/config/feature/mount/nfs.h) \
    $(wildcard include/config/feature/mount/cifs.h) \
    $(wildcard include/config/feature/mount/flags.h) \
    $(wildcard include/config/feature/mount/fstab.h) \
    $(wildcard include/config/pivot/root.h) \
    $(wildcard include/config/rdate.h) \
    $(wildcard include/config/rdev.h) \
    $(wildcard include/config/readprofile.h) \
    $(wildcard include/config/rtcwake.h) \
    $(wildcard include/config/script.h) \
    $(wildcard include/config/setarch.h) \
    $(wildcard include/config/swaponoff.h) \
    $(wildcard include/config/feature/swapon/pri.h) \
    $(wildcard include/config/switch/root.h) \
    $(wildcard include/config/umount.h) \
    $(wildcard include/config/feature/umount/all.h) \
    $(wildcard include/config/feature/mount/loop.h) \
    $(wildcard include/config/feature/mtab/support.h) \
    $(wildcard include/config/adjtimex.h) \
    $(wildcard include/config/bbconfig.h) \
    $(wildcard include/config/chat.h) \
    $(wildcard include/config/feature/chat/nofail.h) \
    $(wildcard include/config/feature/chat/tty/hifi.h) \
    $(wildcard include/config/feature/chat/implicit/cr.h) \
    $(wildcard include/config/feature/chat/swallow/opts.h) \
    $(wildcard include/config/feature/chat/send/escapes.h) \
    $(wildcard include/config/feature/chat/var/abort/len.h) \
    $(wildcard include/config/feature/chat/clr/abort.h) \
    $(wildcard include/config/chrt.h) \
    $(wildcard include/config/crond.h) \
    $(wildcard include/config/feature/crond/call/sendmail.h) \
    $(wildcard include/config/crontab.h) \
    $(wildcard include/config/dc.h) \
    $(wildcard include/config/devfsd.h) \
    $(wildcard include/config/devfsd/modload.h) \
    $(wildcard include/config/devfsd/fg/np.h) \
    $(wildcard include/config/devfsd/verbose.h) \
    $(wildcard include/config/eject.h) \
    $(wildcard include/config/feature/eject/scsi.h) \
    $(wildcard include/config/fbsplash.h) \
    $(wildcard include/config/inotifyd.h) \
    $(wildcard include/config/last.h) \
    $(wildcard include/config/feature/last/small.h) \
    $(wildcard include/config/feature/last/fancy.h) \
    $(wildcard include/config/less.h) \
    $(wildcard include/config/feature/less/maxlines.h) \
    $(wildcard include/config/feature/less/brackets.h) \
    $(wildcard include/config/feature/less/flags.h) \
    $(wildcard include/config/feature/less/flagcs.h) \
    $(wildcard include/config/feature/less/marks.h) \
    $(wildcard include/config/feature/less/regexp.h) \
    $(wildcard include/config/hdparm.h) \
    $(wildcard include/config/feature/hdparm/get/identity.h) \
    $(wildcard include/config/feature/hdparm/hdio/scan/hwif.h) \
    $(wildcard include/config/feature/hdparm/hdio/unregister/hwif.h) \
    $(wildcard include/config/feature/hdparm/hdio/drive/reset.h) \
    $(wildcard include/config/feature/hdparm/hdio/tristate/hwif.h) \
    $(wildcard include/config/feature/hdparm/hdio/getset/dma.h) \
    $(wildcard include/config/makedevs.h) \
    $(wildcard include/config/feature/makedevs/leaf.h) \
    $(wildcard include/config/feature/makedevs/table.h) \
    $(wildcard include/config/man.h) \
    $(wildcard include/config/microcom.h) \
    $(wildcard include/config/mountpoint.h) \
    $(wildcard include/config/mt.h) \
    $(wildcard include/config/raidautorun.h) \
    $(wildcard include/config/readahead.h) \
    $(wildcard include/config/runlevel.h) \
    $(wildcard include/config/rx.h) \
    $(wildcard include/config/setsid.h) \
    $(wildcard include/config/strings.h) \
    $(wildcard include/config/taskset.h) \
    $(wildcard include/config/feature/taskset/fancy.h) \
    $(wildcard include/config/time.h) \
    $(wildcard include/config/ttysize.h) \
    $(wildcard include/config/watchdog.h) \
    $(wildcard include/config/feature/prefer/ipv4/address.h) \
    $(wildcard include/config/verbose/resolution/errors.h) \
    $(wildcard include/config/arp.h) \
    $(wildcard include/config/arping.h) \
    $(wildcard include/config/brctl.h) \
    $(wildcard include/config/feature/brctl/fancy.h) \
    $(wildcard include/config/feature/brctl/show.h) \
    $(wildcard include/config/dnsd.h) \
    $(wildcard include/config/ether/wake.h) \
    $(wildcard include/config/fakeidentd.h) \
    $(wildcard include/config/ftpget.h) \
    $(wildcard include/config/ftpput.h) \
    $(wildcard include/config/feature/ftpgetput/long/options.h) \
    $(wildcard include/config/hostname.h) \
    $(wildcard include/config/httpd.h) \
    $(wildcard include/config/feature/httpd/ranges.h) \
    $(wildcard include/config/feature/httpd/use/sendfile.h) \
    $(wildcard include/config/dfile.h) \
    $(wildcard include/config/feature/httpd/reload/config/sighup.h) \
    $(wildcard include/config/sighup.h) \
    $(wildcard include/config/feature/httpd/setuid.h) \
    $(wildcard include/config/feature/httpd/basic/auth.h) \
    $(wildcard include/config/feature/httpd/auth/md5.h) \
    $(wildcard include/config/feature/httpd/config/with/mime/types.h) \
    $(wildcard include/config/with/mime/types.h) \
    $(wildcard include/config/feature/httpd/cgi.h) \
    $(wildcard include/config/feature/httpd/config/with/script/interpr.h) \
    $(wildcard include/config/with/script/interpr.h) \
    $(wildcard include/config/feature/httpd/set/remote/port/to/env.h) \
    $(wildcard include/config/feature/httpd/encode/url/str.h) \
    $(wildcard include/config/feature/httpd/error/pages.h) \
    $(wildcard include/config/feature/httpd/proxy.h) \
    $(wildcard include/config/ifconfig.h) \
    $(wildcard include/config/feature/ifconfig/status.h) \
    $(wildcard include/config/status.h) \
    $(wildcard include/config/feature/ifconfig/slip.h) \
    $(wildcard include/config/slip.h) \
    $(wildcard include/config/feature/ifconfig/memstart/ioaddr/irq.h) \
    $(wildcard include/config/memstart/ioaddr/irq.h) \
    $(wildcard include/config/feature/ifconfig/hw.h) \
    $(wildcard include/config/hw.h) \
    $(wildcard include/config/feature/ifconfig/broadcast/plus.h) \
    $(wildcard include/config/broadcast/plus.h) \
    $(wildcard include/config/ifenslave.h) \
    $(wildcard include/config/ifupdown.h) \
    $(wildcard include/config/ifupdown/ifstate/path.h) \
    $(wildcard include/config/feature/ifupdown/ip.h) \
    $(wildcard include/config/feature/ifupdown/ip/builtin.h) \
    $(wildcard include/config/feature/ifupdown/ifconfig/builtin.h) \
    $(wildcard include/config/builtin.h) \
    $(wildcard include/config/feature/ifupdown/ipv4.h) \
    $(wildcard include/config/feature/ifupdown/ipv6.h) \
    $(wildcard include/config/feature/ifupdown/mapping.h) \
    $(wildcard include/config/feature/ifupdown/external/dhcp.h) \
    $(wildcard include/config/inetd.h) \
    $(wildcard include/config/feature/inetd/support/builtin/echo.h) \
    $(wildcard include/config/feature/inetd/support/builtin/discard.h) \
    $(wildcard include/config/feature/inetd/support/builtin/time.h) \
    $(wildcard include/config/feature/inetd/support/builtin/daytime.h) \
    $(wildcard include/config/feature/inetd/support/builtin/chargen.h) \
    $(wildcard include/config/feature/inetd/rpc.h) \
    $(wildcard include/config/ip.h) \
    $(wildcard include/config/feature/ip/address.h) \
    $(wildcard include/config/feature/ip/link.h) \
    $(wildcard include/config/feature/ip/route.h) \
    $(wildcard include/config/feature/ip/tunnel.h) \
    $(wildcard include/config/feature/ip/rule.h) \
    $(wildcard include/config/feature/ip/short/forms.h) \
    $(wildcard include/config/feature/ip/rare/protocols.h) \
    $(wildcard include/config/ipaddr.h) \
    $(wildcard include/config/iplink.h) \
    $(wildcard include/config/iproute.h) \
    $(wildcard include/config/iptunnel.h) \
    $(wildcard include/config/iprule.h) \
    $(wildcard include/config/ipcalc.h) \
    $(wildcard include/config/feature/ipcalc/fancy.h) \
    $(wildcard include/config/feature/ipcalc/long/options.h) \
    $(wildcard include/config/nameif.h) \
    $(wildcard include/config/feature/nameif/extended.h) \
    $(wildcard include/config/nc.h) \
    $(wildcard include/config/nc/server.h) \
    $(wildcard include/config/nc/extra.h) \
    $(wildcard include/config/netstat.h) \
    $(wildcard include/config/feature/netstat/wide.h) \
    $(wildcard include/config/feature/netstat/prg.h) \
    $(wildcard include/config/nslookup.h) \
    $(wildcard include/config/ping.h) \
    $(wildcard include/config/ping6.h) \
    $(wildcard include/config/feature/fancy/ping.h) \
    $(wildcard include/config/pscan.h) \
    $(wildcard include/config/sendmail.h) \
    $(wildcard include/config/fetchmail.h) \
    $(wildcard include/config/slattach.h) \
    $(wildcard include/config/telnet.h) \
    $(wildcard include/config/feature/telnet/ttype.h) \
    $(wildcard include/config/feature/telnet/autologin.h) \
    $(wildcard include/config/telnetd.h) \
    $(wildcard include/config/feature/telnetd/standalone.h) \
    $(wildcard include/config/tftp.h) \
    $(wildcard include/config/tftpd.h) \
    $(wildcard include/config/feature/tftp/get.h) \
    $(wildcard include/config/feature/tftp/put.h) \
    $(wildcard include/config/feature/tftp/blocksize.h) \
    $(wildcard include/config/debug/tftp.h) \
    $(wildcard include/config/traceroute.h) \
    $(wildcard include/config/feature/traceroute/verbose.h) \
    $(wildcard include/config/feature/traceroute/source/route.h) \
    $(wildcard include/config/feature/traceroute/use/icmp.h) \
    $(wildcard include/config/p.h) \
    $(wildcard include/config/app/udhcpd.h) \
    $(wildcard include/config/app/dhcprelay.h) \
    $(wildcard include/config/app/dumpleases.h) \
    $(wildcard include/config/feature/udhcpd/write/leases/early.h) \
    $(wildcard include/config/dhcpd/leases/file.h) \
    $(wildcard include/config/app/udhcpc.h) \
    $(wildcard include/config/feature/udhcpc/arping.h) \
    $(wildcard include/config/feature/udhcp/port.h) \
    $(wildcard include/config/feature/udhcp/debug.h) \
    $(wildcard include/config/feature/rfc3397.h) \
    $(wildcard include/config/dhcpc/default/script.h) \
    $(wildcard include/config/udhcpc/slack/for/buggy/servers.h) \
    $(wildcard include/config/vconfig.h) \
    $(wildcard include/config/wget.h) \
    $(wildcard include/config/feature/wget/statusbar.h) \
    $(wildcard include/config/feature/wget/authentication.h) \
    $(wildcard include/config/feature/wget/long/options.h) \
    $(wildcard include/config/zcip.h) \
    $(wildcard include/config/tcpsvd.h) \
    $(wildcard include/config/udpsvd.h) \
    $(wildcard include/config/free.h) \
    $(wildcard include/config/fuser.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/killall5.h) \
    $(wildcard include/config/nmeter.h) \
    $(wildcard include/config/feature/pidof/single.h) \
    $(wildcard include/config/feature/pidof/omit.h) \
    $(wildcard include/config/ps.h) \
    $(wildcard include/config/feature/ps/wide.h) \
    $(wildcard include/config/feature/ps/time.h) \
    $(wildcard include/config/feature/ps/unusual/systems.h) \
    $(wildcard include/config/renice.h) \
    $(wildcard include/config/bb/sysctl.h) \
    $(wildcard include/config/top.h) \
    $(wildcard include/config/feature/top/cpu/usage/percentage.h) \
    $(wildcard include/config/feature/top/cpu/global/percents.h) \
    $(wildcard include/config/feature/top/decimals.h) \
    $(wildcard include/config/uptime.h) \
    $(wildcard include/config/watch.h) \
    $(wildcard include/config/feature/sh/is/ash.h) \
    $(wildcard include/config/feature/sh/is/hush.h) \
    $(wildcard include/config/feature/sh/is/msh.h) \
    $(wildcard include/config/feature/sh/is/none.h) \
    $(wildcard include/config/ash.h) \
    $(wildcard include/config/ash/bash/compat.h) \
    $(wildcard include/config/ash/job/control.h) \
    $(wildcard include/config/ash/read/nchars.h) \
    $(wildcard include/config/ash/read/timeout.h) \
    $(wildcard include/config/ash/alias.h) \
    $(wildcard include/config/ash/math/support.h) \
    $(wildcard include/config/ash/math/support/64.h) \
    $(wildcard include/config/ash/getopts.h) \
    $(wildcard include/config/ash/builtin/echo.h) \
    $(wildcard include/config/ash/builtin/printf.h) \
    $(wildcard include/config/ash/builtin/test.h) \
    $(wildcard include/config/ash/cmdcmd.h) \
    $(wildcard include/config/ash/mail.h) \
    $(wildcard include/config/ash/optimize/for/size.h) \
    $(wildcard include/config/ash/random/support.h) \
    $(wildcard include/config/ash/expand/prmt.h) \
    $(wildcard include/config/hush.h) \
    $(wildcard include/config/hush/help.h) \
    $(wildcard include/config/hush/interactive.h) \
    $(wildcard include/config/hush/job.h) \
    $(wildcard include/config/hush/tick.h) \
    $(wildcard include/config/hush/if.h) \
    $(wildcard include/config/hush/loops.h) \
    $(wildcard include/config/hush/case.h) \
    $(wildcard include/config/lash.h) \
    $(wildcard include/config/msh.h) \
    $(wildcard include/config/feature/sh/extra/quiet.h) \
    $(wildcard include/config/feature/sh/standalone.h) \
    $(wildcard include/config/feature/sh/nofork.h) \
    $(wildcard include/config/cttyhack.h) \
    $(wildcard include/config/syslogd.h) \
    $(wildcard include/config/feature/rotate/logfile.h) \
    $(wildcard include/config/feature/remote/log.h) \
    $(wildcard include/config/feature/syslogd/dup.h) \
    $(wildcard include/config/feature/ipc/syslog.h) \
    $(wildcard include/config/feature/ipc/syslog/buffer/size.h) \
    $(wildcard include/config/logread.h) \
    $(wildcard include/config/feature/logread/reduced/locking.h) \
    $(wildcard include/config/klogd.h) \
    $(wildcard include/config/logger.h) \
    $(wildcard include/config/runsv.h) \
    $(wildcard include/config/runsvdir.h) \
    $(wildcard include/config/sv.h) \
    $(wildcard include/config/svlogd.h) \
    $(wildcard include/config/chpst.h) \
    $(wildcard include/config/setuidgid.h) \
    $(wildcard include/config/envuidgid.h) \
    $(wildcard include/config/envdir.h) \
    $(wildcard include/config/softlimit.h) \
    $(wildcard include/config/chcon.h) \
    $(wildcard include/config/feature/chcon/long/options.h) \
    $(wildcard include/config/getenforce.h) \
    $(wildcard include/config/getsebool.h) \
    $(wildcard include/config/load/policy.h) \
    $(wildcard include/config/matchpathcon.h) \
    $(wildcard include/config/restorecon.h) \
    $(wildcard include/config/runcon.h) \
    $(wildcard include/config/feature/runcon/long/options.h) \
    $(wildcard include/config/selinuxenabled.h) \
    $(wildcard include/config/setenforce.h) \
    $(wildcard include/config/setfiles.h) \
    $(wildcard include/config/feature/setfiles/check/option.h) \
    $(wildcard include/config/setsebool.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/lpd.h) \
    $(wildcard include/config/lpr.h) \
    $(wildcard include/config/lpq.h) \

miscutils/bbconfig.o: $(deps_miscutils/bbconfig.o)

$(deps_miscutils/bbconfig.o):
