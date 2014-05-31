/* linux/include/asm/arch-wpcm450/regs-clock.h
 *
 * Copyright (c) 2003,2004,2005 Simtec Electronics <linux@simtec.co.uk>
 *		      http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 clock register definitions
 *
 *  Changelog:
 *    18-Aug-2004 Ben Dooks         Added 2440 definitions
 *    08-Aug-2004 Herbert Pötzl     Added CLKCON definitions
 *    19-06-2003  Ben Dooks         Created file
 *    12-03-2004  Ben Dooks         Updated include protection
 *    29-Sep-2004 Ben Dooks	    Fixed usage for assembly inclusion
 *    10-Feb-2005 Ben Dooks	    Fixed CAMDIVN address (Guillaume Gourat)
 *    10-Mar-2005 Lucas Villa Real  Changed WPCM450_VA to WPCM450_VA
 *    27-Aug-2005 Ben Dooks	    Add clock-slow info
 *    20-Oct-2005 Ben Dooks	    Fixed overflow in PLL (Guillaume Gourat)
 *    20-Oct-2005 Ben Dooks	    Add masks for DCLK (Guillaume Gourat)
*/

#ifndef __ASM_ARM_REGS_CLOCK
#define __ASM_ARM_REGS_CLOCK "$Id: regs-clock.h 1.2 2008/01/10 15:07:53Z afishman Exp $"
#define WPCM450_CLKREG(x) ((x) + WPCM450_VA_CLKPWR)
#define WPCM450_CKREG(x) ((x) + WPCM450_VA_EBI)


#define WPCM450_CLKEN			  WPCM450_CLKREG(0x00)
#define WPCM450_CLKSEL			WPCM450_CLKREG(0x04)
#define WPCM450_CLKDIV			WPCM450_CLKREG(0x08)
#define WPCM450_CLKPLL0			WPCM450_CLKREG(0x0C)
#define WPCM450_CLKPLL1			WPCM450_CLKREG(0x10)
#define WPCM450_CLKPMCON		WPCM450_CLKREG(0x14)
#define WPCM450_CLKIRQWAKECON		WPCM450_CLKREG(0x18)
#define WPCM450_CLKIRQWAKEFALG		WPCM450_CLKREG(0x1C)
#define WPCM450_CLKIPSRST		WPCM450_CLKREG(0x20)

#define PLL0		0x00
#define PLL1		0x01
#define EXTAL15M	0x03

#endif /* __ASM_ARM_REGS_CLOCK */
