/*
 * include/asm-arm/arch-wpcm450/entry-macro.S
 *
 * Low-level IRQ helper macros for WPCM450-based platforms
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
*/

/* We have a problem that the INTOFFSET register does not always
 * show one interrupt. Occasionally we get two interrupts through
 * the prioritiser, and this causes the INTOFFSET register to show
 * what looks like the logical-or of the two interrupt numbers.
 *
 * Thanks to Klaus, Shannon, et al for helping to debug this problem
*/

#define AIC_IPER	(0x10C)
#define AIC_ISNR	(0x110)


#include <asm/hardware.h>
#include <asm/arch/irqs.h>

	.macro	get_irqnr_and_base, irqnr, irqstat, base, tmp

		mov	\base, #WPCM450_VA_IRQ

		ldr	\irqnr, [ \base, #AIC_IPER]
		ldr	\irqnr, [ \base, #AIC_ISNR]
		cmp	\irqnr, #0		
		
	.endm

		/* currently don't need an disable_fiq macro */

		.macro	disable_fiq
		.endm
