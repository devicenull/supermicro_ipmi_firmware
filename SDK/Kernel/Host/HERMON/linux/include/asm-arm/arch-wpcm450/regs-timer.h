/* linux/include/asm/arch-wpcm450/regs-timer.h
 *
 * Copyright (c) 2006 winbond Electronics
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * WPCM450 Timer configuration
 *
 *  Changelog:
 *    22-08-2003     clyu     Created file
*/


#ifndef __ASM_ARCH_REGS_TIMER_H
#define __ASM_ARCH_REGS_TIMER_H "$Id: regs-timer.h 1.1 2008/01/02 14:45:06Z afishman Exp $"

#define WPCM450_TIMERREG(x) (WPCM450_VA_TIMER + (x))

#define WPCM450_TCSR0	      WPCM450_TIMERREG(0x000)
#define WPCM450_TCSR1	      WPCM450_TIMERREG(0x004)
#define WPCM450_TICR0	      WPCM450_TIMERREG(0x008)
#define WPCM450_TICR1	      WPCM450_TIMERREG(0x00C)
#define WPCM450_TDR0	      WPCM450_TIMERREG(0x010)
#define WPCM450_TDR1	      WPCM450_TIMERREG(0x014)
#define WPCM450_TISR	      WPCM450_TIMERREG(0x018)
#define WPCM450_WTCR	      WPCM450_TIMERREG(0x01C)
#define WPCM450_TCSR2	      WPCM450_TIMERREG(0x020)
#define WPCM450_TCSR3	      WPCM450_TIMERREG(0x024)
#define WPCM450_TICR2	      WPCM450_TIMERREG(0x028)
#define WPCM450_TICR3	      WPCM450_TIMERREG(0x02C)
#define WPCM450_TDR2	      WPCM450_TIMERREG(0x030)
#define WPCM450_TDR3	      WPCM450_TIMERREG(0x034)
#define WPCM450_TCSR4	      WPCM450_TIMERREG(0x040)
#define WPCM450_TICR4	      WPCM450_TIMERREG(0x048)
#define WPCM450_TDR4	      WPCM450_TIMERREG(0x050)

#define WTCR_WTE			  0x0080
#define WTCR_WTIE			  0x0040
#define WTCR_WTIS			  0x0030
#define WTCR_WTIF			  0x0008
#define WTCR_WTRF			  0x0004
#define WTCR_WTRE			  0x0002
#define WTCR_WTR			  0x0001

#define WTIS0	0x00
#define WTIS1	0x01
#define WTIS2	0x02
#define WTIS3	0x03



#endif /*  __ASM_ARCH_REGS_TIMER_H */



