/* linux/arch/arm/mach-wpcm450/pm.h
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

/* wpcm450_pm_init
 *
 * called from board at initialisation time to setup the power
 * management
*/

#ifdef CONFIG_PM

extern __init int wpcm450_pm_init(void);

#else

static inline int wpcm450_pm_init(void)
{
	return 0;
}
#endif

/* configuration for the IRQ mask over sleep */
extern unsigned long wpcm_irqwake_intmask;
extern unsigned long wpcm_irqwake_eintmask;

/* IRQ masks for IRQs allowed to go to sleep (see irq.c) */
extern unsigned long wpcm_irqwake_intallow;
extern unsigned long wpcm_irqwake_eintallow;

/* Flags for PM Control */

extern unsigned long wpcm_pm_flags;

/* from sleep.S */

extern void wpcm450_cpu_suspend(unsigned long *saveblk);
extern void wpcm450_cpu_resume(void);

extern unsigned long wpcm450_sleep_save_phys;

/* sleep save info */

struct sleep_save {
	void __iomem	*reg;
	unsigned long	val;
};

#define SAVE_ITEM(x) \
	{ .reg = (x) }

extern void wpcm450_pm_do_save(struct sleep_save *ptr, int count);
extern void wpcm450_pm_do_restore(struct sleep_save *ptr, int count);
