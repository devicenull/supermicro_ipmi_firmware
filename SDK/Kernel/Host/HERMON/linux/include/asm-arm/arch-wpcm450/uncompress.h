/* linux/include/asm-arm/arch-wpcm450/uncompress.h
 *
 * (c) 2006 Winbond Electronics
 *  shirley yu <clyu2@winbond.com>
 *
 * WPCM450 - uncompress code
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
*/

#ifndef __ASM_ARCH_UNCOMPRESS_H
#define __ASM_ARCH_UNCOMPRESS_H

#include <linux/config.h>

/* defines for UART registers */
#include "asm/arch/regs-serial.h"
#include <asm/arch/map.h>

/* working in physical space... */
#undef WPCM450_GPIOREG
#undef WPCM450_WDOGREG

#define WPCM450_GPIOREG(x) ((WPCM450_PA_GPIO + (x)))
#define WPCM450_WDOGREG(x) ((WPCM450_PA_WATCHDOG + (x)))

/* how many bytes we allow into the FIFO at a time in FIFO mode */
#define FIFO_MAX	 (14)

//#define uart_base WPCM450_PA_UART + (0x4000*CONFIG_WPCM450_LOWLEVEL_UART_PORT)
#define uart_base WPCM450_PA_UART

static __inline__ void
uart_wr(unsigned int reg, unsigned int val)
{
	volatile unsigned int *ptr;

	ptr = (volatile unsigned int *)(reg + uart_base);
	*ptr = val;
}

static __inline__ unsigned int
uart_rd(unsigned int reg)
{
	volatile unsigned int *ptr;

	ptr = (volatile unsigned int *)(reg + uart_base);
	return *ptr;
}


/* we can deal with the case the UARTs are being run
 * in FIFO mode, so that we don't hold up our execution
 * waiting for tx to happen...
*/

static void putc(int ch)
{
		/* not using fifos */
	
	while (!(uart_rd(WPCM450_COM_LSR) & UART_LSR_THRE))
			barrier();
	

	/* write byte to transmission register */
	uart_wr(WPCM450_COM_TX, ch);
}

static inline void flush(void)
{
}

#define __raw_writel(d,ad) do { *((volatile unsigned int *)(ad)) = (d); } while(0)

/* CONFIG_WPCM450_BOOT_WATCHDOG
 *
 * Simple boot-time watchdog setup, to reboot the system if there is
 * any problem with the boot process
*/

#ifdef CONFIG_WPCM450_BOOT_WATCHDOG

#else
#define arch_decomp_wdog_start()
#define arch_decomp_wdog()
#endif

static void error(char *err);

static void
arch_decomp_setup(void)
{
	/* we may need to setup the uart(s) here if we are not running
	 * on an BAST... the BAST will have left the uarts configured
	 * after calling linux.
	 */

	arch_decomp_wdog_start();
}


#endif /* __ASM_ARCH_UNCOMPRESS_H */
