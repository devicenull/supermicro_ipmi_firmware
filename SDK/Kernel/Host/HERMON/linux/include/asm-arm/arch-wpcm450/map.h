/* linux/include/asm-arm/arch-wpcm450/map.h
 *
 * (c) 2006 Winbond Electronics
 *  shirley yu <clyu2@winbond.com>
 *
 * WPCM450 - Memory map definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  22-Aug-2006 clyu   Created file
*/

#ifndef __ASM_ARCH_MAP_H
#define __ASM_ARCH_MAP_H

/* we have a bit of a tight squeeze to fit all our registers from
 * 0xF00000000 upwards, since we use all of the nGCS space in some
 * capacity, and also need to fit the WPCM450 registers in as well...
 *
 * we try to ensure stuff like the IRQ registers are available for
 * an single MOVS instruction (ie, only 8 bits of set data)
 *
 * Note, we are trying to remove some of these from the implementation
 * as they are only useful to certain drivers...
 */

#ifndef __ASSEMBLY__
#define WPCM450_ADDR(x)	  ((void __iomem *)0xF0000000 + (x))
#else
#define WPCM450_ADDR(x)	  (0xF0000000 + (x))
#endif


#define AHB_IO_BASE		0xB0000000
#define APB_IO_BASE		0xB8000000
#define CLOCKPW_BASE	(AHB_IO_BASE+0x200)
#define AIC_IO_BASE		(APB_IO_BASE+0x2000)
#define TIMER_IO_BASE	(APB_IO_BASE+0x1000)
 
#define WPCM450_VA_B000	   WPCM450_ADDR(0x00010000)
#define WPCM450_PA_B000    (0xB0000000)
#define WPCM450_SZ_B000	   SZ_64K

#define WPCM450_VA_B001	   WPCM450_ADDR(0x00020000)
#define WPCM450_PA_B001    (0xB0010000)
#define WPCM450_SZ_B001	   SZ_64K

#define WPCM450_VA_B800	   WPCM450_ADDR(0x00030000)
#define WPCM450_PA_B800    (0xB8000000)
#define WPCM450_SZ_B800	   SZ_64K

/* VCD_MEM */
#define WPCM450_VA_VCD_MEM WPCM450_ADDR(0x00100000)
#define WPCM450_PA_VCD_MEM (0x06400000)
#define WPCM450_SZ_VCD_MEM SZ_32M

/* Virtual USB MEM */
//#define WPCM450_VA_USB_MEM WPCM450_ADDR(0x00a00000)
//#define WPCM450_PA_USB_MEM (0x07200000)
//#define WPCM450_SZ_USB_MEM SZ_2M


#define WPCM450_SDRAM_PA    (0x00000000)
 
/* interrupt controller is the first thing we put in, to make
 * the assembly code for the irq detection easier
 */
#define WPCM450_VA_IRQ	   WPCM450_ADDR(0x00000000)
#define WPCM450_PA_IRQ	   (0xB8002000)
#define WPCM450_SZ_IRQ	   SZ_4K

/* Clock and Power management */
#define WPCM450_PA_CLKPWR  (WPCM450_PA_B000+0x0200)
#define WPCM450_VA_CLKPWR  (WPCM450_PA_CLKPWR - WPCM450_PA_B000 + WPCM450_VA_B000)
//#define WPCM450_SZ_CLKPWR  SZ_4K

/* External MAC control */
#define WPCM450_PA_EMC 	   (WPCM450_PA_B000+0x2000)
#define WPCM450_VA_EMC     (WPCM450_PA_EMC - WPCM450_PA_B000 + WPCM450_VA_B000)

/* UARTs */
#define WPCM450_PA_UART	   (WPCM450_PA_B800+0x0000)
#define WPCM450_VA_UART    (WPCM450_PA_UART - WPCM450_PA_B800 + WPCM450_VA_B800)

/* Timers */
#define WPCM450_PA_TIMER   (WPCM450_PA_B800+0x1000)
#define WPCM450_VA_TIMER   (WPCM450_PA_TIMER - WPCM450_PA_B800 + WPCM450_VA_B800)

/* GPIO ports */
#define WPCM450_PA_GPIO	   (WPCM450_PA_B800+0x3000)
#define WPCM450_VA_GPIO    (WPCM450_PA_GPIO - WPCM450_PA_B800 + WPCM450_VA_B800)

/* VCD */
#define WPCM450_PA_VCD	   (WPCM450_PA_B001+0x0000)
#define WPCM450_VA_VCD     (WPCM450_PA_VCD - WPCM450_PA_B001 + WPCM450_VA_B001)

/* GCR */
#define WPCM450_PA_GCR	   (WPCM450_PA_B000+0x0000)
#define WPCM450_VA_GCR     (WPCM450_PA_GCR - WPCM450_PA_B000 + WPCM450_VA_B000)

/* GFXI */
#define WPCM450_PA_GFXI	   (WPCM450_PA_B800+0x0300)
#define WPCM450_VA_GFXI    (WPCM450_PA_GFXI - WPCM450_PA_B800 + WPCM450_VA_B800)

/* USB Device port */
#define WPCM450_PA_USB20_DEV    (WPCM450_PA_B000+0x5000)
#define WPCM450_VA_USB20_DEV    (WPCM450_PA_USB20_DEV - WPCM450_PA_B000 + WPCM450_VA_B000)
#define WPCM450_SZ_USB20_DEV    SZ_4K

/* USB Device port */
#define WPCM450_PA_USB11_DEV    (WPCM450_PA_B000+0x6000)
#define WPCM450_VA_USB11_DEV    (WPCM450_PA_USB11_DEV - WPCM450_PA_B000 + WPCM450_VA_B000)
#define WPCM450_SZ_USB11_DEV    SZ_4K

#endif /* __ASM_ARCH_MAP_H */
