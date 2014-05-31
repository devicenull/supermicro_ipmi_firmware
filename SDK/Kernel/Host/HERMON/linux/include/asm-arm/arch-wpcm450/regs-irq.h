/* linux/include/asm/arch-wpcm450/regs-irq.h
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
 */


#ifndef ___ASM_ARCH_REGS_IRQ_H
#define ___ASM_ARCH_REGS_IRQ_H "$Id: regs-irq.h 1.3 2008/02/11 12:33:04Z achechik Exp $"

/* interrupt controller */

#define WPCM450_IRQREG(x)   ((x) + WPCM450_VA_IRQ)

/* ----- Define	the interrupt controller registers ----- */
#define WPCM450_AIC_SCR1	WPCM450_IRQREG(0x004)		/* Source control register 7 (timer0) */
#define WPCM450_AIC_SCR6	WPCM450_IRQREG(0x018)		/* Source control register 6 (UART) */
#define WPCM450_AIC_SCR7	WPCM450_IRQREG(0x01C)		/* Source control register 7 (timer0) */
#define WPCM450_AIC_SCR8	WPCM450_IRQREG(0x020)		/* Source control register 8 (timer1) */
#define WPCM450_AIC_SCR20	WPCM450_IRQREG(0x050)		/* Source control register 20 (usb2.0) */
#define WPCM450_AIC_SCR21	WPCM450_IRQREG(0x054)		/* Source control register 21 (usb1.1) */
#define WPCM450_AIC_ISR		WPCM450_IRQREG(0x108)     	/* Interrupt status register */
#define WPCM450_AIC_IPER	WPCM450_IRQREG(0x10C)
#define WPCM450_AIC_ISNR	WPCM450_IRQREG(0x110)
#define WPCM450_AIC_IMR		WPCM450_IRQREG(0x114)		/* Interrupt mask register */
#define WPCM450_AIC_GEN		WPCM450_IRQREG(0x084)
#define WPCM450_AIC_MECR	WPCM450_IRQREG(0x120)		/* Mask enable command register */
#define WPCM450_AIC_MDCR	WPCM450_IRQREG(0x124)		/* Mask disable command register */
#define WPCM450_AIC_SSCR	WPCM450_IRQREG(0x128)		/* Source set command register */
#define WPCM450_AIC_SCCR	WPCM450_IRQREG(0x12C)
#define WPCM450_AIC_EOSCR	WPCM450_IRQREG(0x130)

#endif /* ___ASM_ARCH_REGS_IRQ_H */
