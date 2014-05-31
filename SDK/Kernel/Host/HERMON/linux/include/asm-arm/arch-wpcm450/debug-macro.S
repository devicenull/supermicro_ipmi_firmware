/* linux/include/asm-arm/arch-wpcm450/debug-macro.S
 *
 * Debugging macro include header
 *
 *  Copyright (C) 1994-1999 Russell King
 *  Copyright (C) 2005 Simtec Electronics
 *
 *  Moved from linux/arch/arm/kernel/debug.S by Ben Dooks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *     10-Mar-2005 LCVR  Changed WPCM450_VA to WPCM450_VA
*/

#include <asm/arch/map.h>
#include <asm/arch/regs-serial.h>
#include <asm/arch/regs-gpio.h>

#define WPCM450_UART1_OFF (0x4000)
#define SHIFT_2440TXF (14-9)

		.macro addruart, rx
		mrc	p15, 0, \rx, c1, c0
		tst	\rx, #1
		ldreq	\rx, = WPCM450_PA_UART
		ldrne	\rx, = WPCM450_VA_UART
#if CONFIG_DEBUG_WPCM450_UART != 0
		add	\rx, \rx, #(WPCM450_UART1_OFF * CONFIG_DEBUG_WPCM450_UART)
#endif
		.endm

		.macro	senduart,rd,rx
		str	\rd, [\rx, # WPCM450_UTXH ]
		.endm

		.macro	busyuart, rd, rx
		ldr	\rd, [ \rx, # WPCM450_UFCON ]
		tst	\rd, #WPCM450_UFCON_FIFOMODE	@ fifo enabled?
		beq	1001f				@
		@ FIFO enabled...
1003:
		mrc	p15, 0, \rd, c1, c0
		tst	\rd, #1
		addeq	\rd, \rx, #(WPCM450_PA_GPIO - WPCM450_PA_UART)
		addne	\rd, \rx, #(WPCM450_VA_GPIO - WPCM450_VA_UART)
		bic	\rd, \rd, #0xff000
		ldr	\rd, [ \rd, # WPCM450_GSTATUS1 - WPCM450_GPIOREG(0) ]
		and	\rd, \rd, #0x00ff0000
		teq	\rd, #0x00440000		@ is it 2440?

		ldr	\rd, [ \rx, # WPCM450_UFSTAT ]
		moveq	\rd, \rd, lsr #SHIFT_2440TXF
		tst	\rd, #WPCM450_UFSTAT_TXFULL
		bne	1003b
		b	1002f

1001:
		@ busy waiting for non fifo
		ldr	\rd, [ \rx, # WPCM450_UTRSTAT ]
		tst	\rd, #WPCM450_UTRSTAT_TXFE
		beq	1001b

1002:		@ exit busyuart
		.endm

		.macro	waituart,rd,rx

		ldr	\rd, [ \rx, # WPCM450_UFCON ]
		tst	\rd, #WPCM450_UFCON_FIFOMODE	@ fifo enabled?
		beq	1001f				@
		@ FIFO enabled...
1003:
		mrc	p15, 0, \rd, c1, c0
		tst	\rd, #1
		addeq	\rd, \rx, #(WPCM450_PA_GPIO - WPCM450_PA_UART)
		addne	\rd, \rx, #(WPCM450_VA_GPIO - WPCM450_VA_UART)
		bic	\rd, \rd, #0xff000
		ldr	\rd, [ \rd, # WPCM450_GSTATUS1 - WPCM450_GPIOREG(0) ]
		and	\rd, \rd, #0x00ff0000
		teq	\rd, #0x00440000		@ is it 2440?

		ldr	\rd, [ \rx, # WPCM450_UFSTAT ]
		andne	\rd, \rd, #WPCM450_UFSTAT_TXMASK
		andeq	\rd, \rd, #S3C2440_UFSTAT_TXMASK
		teq	\rd, #0
		bne	1003b
		b	1002f

1001:
		@ idle waiting for non fifo
		ldr	\rd, [ \rx, # WPCM450_UTRSTAT ]
		tst	\rd, #WPCM450_UTRSTAT_TXFE
		beq	1001b

1002:		@ exit busyuart
		.endm
