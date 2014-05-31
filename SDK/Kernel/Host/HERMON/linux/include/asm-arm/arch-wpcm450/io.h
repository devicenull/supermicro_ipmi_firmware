/*
 * linux/include/asm-arm/arch-w90p910/io.h
 *  from linux/include/asm-arm/arch-rpc/io.h
 *
 * Copyright (C) 1997 Russell King
 *	     (C) 2003 Simtec Electronics
 *
 * Modifications:
 *  06-Dec-1997	RMK	Created.
 *  02-Sep-2003 BJD	Modified for W90P910
 *  10-Mar-2005 LCVR	Changed W90P910_VA to W90P9XX_VA
 *  13-Oct-2005 BJD	Fixed problems with LDRH/STRH offset range
 */

#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#include <asm/hardware.h>

#define IO_SPACE_LIMIT 0xffffffff


#undef DECLARE_IO
#undef DECLARE_DYN_IN

/*
 * Constant address IO functions
 *
 * These have to be macros for the 'J' constraint to work -
 * +/-4096 immediate operand.
 */
#define __outbc(value,port)						\
({									\
		__asm__ __volatile__(					\
		"strb	%0, [%1, #0]	@ outbc"			\
		: : "r" (value), "r" ((port)));				\
})

#define __inbc(port)							\
({									\
	unsigned char result;						\
		__asm__ __volatile__(					\
		"ldrb	%0, [%1, #0]	@ inbc"				\
		: "=r" (result) : "r" ((port)));			\
	result;								\
})

#define __outwc(value,port)						\
({									\
	unsigned long v = value;					\
		__asm__ __volatile__(					\
		"strh	%0, [%1, #0]	@ outwc"			\
		: : "r" (v), "r" ((port)));				\
})

#define __inwc(port)							\
({									\
	unsigned short result;						\
		__asm__ __volatile__(					\
		"ldrh	%0, [%1, #0]	@ inwc"				\
		: "=r" (result) : "r" ((port)));			\
	result;								\
})

#define __outlc(value,port)						\
({									\
	unsigned long v = value;					\
		__asm__ __volatile__(					\
		"str	%0, [%1, #0]	@ outlc"			\
		: : "r" (v), "r" ((port)));		\
})

#define __inlc(port)							\
({									\
	unsigned long result;						\
		__asm__ __volatile__(					\
		"ldr	%0, [%1, #0]	@ inlc"				\
		: "=r" (result) : "r" ((port)));		\
	result;								\
})

#define __ioaddrc(port)	((void __iomem *)(port)))

#if 0//clyu
#define inb(p)		(__builtin_constant_p((p)) ? __inbc(p)	   : __inb(p))
#define inw(p)		(__builtin_constant_p((p)) ? __inwc(p)	   : __inw(p))
#define inl(p)		(__builtin_constant_p((p)) ? __inlc(p)	   : __inl(p))
#define outb(v,p)	(__builtin_constant_p((p)) ? __outbc(v,p) : __outb(v,p))
#define outw(v,p)	(__builtin_constant_p((p)) ? __outwc(v,p) : __outw(v,p))
#define outl(v,p)	(__builtin_constant_p((p)) ? __outlc(v,p) : __outl(v,p))
#else
#define inb(p)		__inbc(p)
#define inw(p)		__inwc(p)
#define inl(p)		__inlc(p)
#define outb(v,p)	__outbc(v,p)
#define outw(v,p)	__outwc(v,p)
#define outl(v,p)	__outlc(v,p)
#endif

#define __ioaddr(p)	(__builtin_constant_p((p)) ? __ioaddr(p)  : __ioaddrc(p))
/* the following macro is deprecated */
#define ioaddr(port)	__ioaddr((port))

#define insb(p,d,l)	__raw_readsb(__ioaddr(p),d,l)
#define insw(p,d,l)	__raw_readsw(__ioaddr(p),d,l)
#define insl(p,d,l)	__raw_readsl(__ioaddr(p),d,l)

#define outsb(p,d,l)	__raw_writesb(__ioaddr(p),d,l)
#define outsw(p,d,l)	__raw_writesw(__ioaddr(p),d,l)
#define outsl(p,d,l)	__raw_writesl(__ioaddr(p),d,l)

/*
 * 1:1 mapping for ioremapped regions.
 */
#define __mem_pci(x)	(x)


#endif
