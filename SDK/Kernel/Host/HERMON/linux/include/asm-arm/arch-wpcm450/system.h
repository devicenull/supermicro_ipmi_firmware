/* linux/include/asm-arm/arch-wpcm450/system.h
 *
 * (c) 2006 Winbond Electronics
 *  shirley yu <clyu2@winbond.com>
 *
 * WPCM450 - System function defines and includes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
 */

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/arch/map.h>
#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-timer.h>

void (*wpcm450_idle)(void);

void wpcm450_default_idle(void)
{
#if 0//clyu
	void __iomem *reg = WPCM450_CLKSEL;
	unsigned long tmp;
	int i;
	/* idle the system by using the idle mode which will wait for an
	 * interrupt to happen before restarting the system.
	 */

	/* Warning: going into idle state upsets jtag scanning */

	__raw_writel(__raw_readl(reg) | (1<<2), reg);

	/* the winbond port seems to do a loop and then unset idle.. */
	for (i = 0; i < 50; i++) {
		tmp += __raw_readl(reg); /* ensure loop not optimised out */
	}

	/* this bit is not cleared on re-start... */

	__raw_writel(__raw_readl(reg) & ~(1<<2), reg);
#endif
}

static void arch_idle(void)
{
	if (wpcm450_idle != NULL)
		(wpcm450_idle)();
	else
		wpcm450_default_idle();
}


static void
arch_reset(char mode)
{
	if (mode == 's') {
		cpu_reset(0);
	}

	printk(KERN_ERR "arch_reset: attempting watchdog reset WTCR=%x\n", __raw_readl(WPCM450_WTCR));
//	__raw_writel(__raw_readl(WPCM450_WTCR) & ~WTCR_WTE, WPCM450_WTCR);	  /* disable watchdog, to be safe  */
        __raw_writel(__raw_readl(WPCM450_WTCR) & ~WTCR_WTE & ~WTCR_WTRF, WPCM450_WTCR);        /* disable watchdog, to be safe – or just write 0x400  */
	
	/* put initial values into count and data */
//	__raw_writel(__raw_readl(WPCM450_WTCR) & ~WTCR_WTIS |  (WTIS0 | WTCR_WTIE | WTCR_WTE |WTCR_WTRE), WPCM450_WTCR);
        __raw_writel(__raw_readl(WPCM450_WTCR) & ~WTCR_WTIS & ~WTCR_WTRF |  (WTIS0 | WTCR_WTIE | WTCR_WTE |WTCR_WTRE), WPCM450_WTCR); // =>after patching   – or just write the wanted value with  WTRF bit clear

	/* set the watchdog to go and reset... */
//	__raw_writel(WTCR_WTR | __raw_readl(WPCM450_WTCR) , WPCM450_WTCR);
    __raw_writel(WTCR_WTR | __raw_readl(WPCM450_WTCR) & ~WTCR_WTRF , WPCM450_WTCR);//after patching – I think this is not necessary, its only “touch” the WD and delay the reset.

	volatile int i;for (i =0; i < 1000 ;i++) udelay(2000);
	printk(KERN_ERR "Watchdog reset failed to assert reset\n");

	/* we'll take a jump through zero as a poor second */
	cpu_reset(0);
}
