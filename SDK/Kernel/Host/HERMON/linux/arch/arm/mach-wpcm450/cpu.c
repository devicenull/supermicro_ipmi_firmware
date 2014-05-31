/* linux/arch/arm/mach-wpcm450/cpu.c
 *
 * Copyright (c) 2006 Winbond Electronics
 *	shirley yu <clyu2@winbond.com>
 *
 * WPCM450 CPU Support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>

#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/delay.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include "cpu.h"
#include "clock.h"
#include "wpcm450.h"

struct cpu_table {
	unsigned long	idcode;
	unsigned long	idmask;
	void		(*map_io)(struct map_desc *mach_desc, int size);
	void		(*init_uarts)(struct wpcm450_uartcfg *cfg, int no);
	void		(*init_clocks)(int xtal);
	int		(*init)(void);
	const char	*name;
};

/* table of supported CPUs */

static const char name_wpcm450[]  = "WPCM450";


static struct cpu_table cpu_ids[] __initdata = {
	{
		.idcode		= 0x01900910,
		.idmask		= 0xffffffff,
		.map_io		= wpcm450_map_io,
		.init_clocks	= wpcm450_init_clocks,
		.init_uarts	= wpcm450_init_uarts,
		.init		= wpcm450_init,
		.name		= name_wpcm450
	},
};

/* minimal IO mapping */

static struct map_desc wpcm450_iodesc[] __initdata = {
	IODESC_ENT(IRQ),
	IODESC_USER(B000),
	IODESC_USER(B001),
	IODESC_USER(B800),
	IODESC_USER(VCD_MEM)
//	,IODESC_USER(USB_MEM)
};


static struct cpu_table *
wpcm_lookup_cpu(unsigned long idcode)
{
	struct cpu_table *tab;
	int count;

	tab = cpu_ids;
	for (count = 0; count < ARRAY_SIZE(cpu_ids); count++, tab++) {
		if ((idcode & tab->idmask) == tab->idcode)
			return tab;
	}

	return NULL;
}

/* board information */

static struct wpcm450_board *board;

void wpcm450_set_board(struct wpcm450_board *b)
{
	int i;

	board = b;

	if (b->clocks_count != 0) {
		struct clk **ptr = b->clocks;

		for (i = b->clocks_count; i > 0; i--, ptr++)
			wpcm450_register_clock(*ptr);
	}
}

/* cpu information */

static struct cpu_table *cpu;

void __init wpcm450_init_io(struct map_desc *mach_desc, int size)
{
	unsigned long idcode = 0x01900910;

	/* initialise the io descriptors we need for initialisation */
	iotable_init(wpcm450_iodesc, ARRAY_SIZE(wpcm450_iodesc));

	//idcode = __raw_readl(WPCM450_GCR_PDID);

	cpu = wpcm_lookup_cpu(idcode);
	
	if (cpu == NULL) {
		printk(KERN_ERR "Unknown CPU type 0x%08lx\n", idcode);
		panic("Unknown WPCM450 CPU");
	}

	if (cpu->map_io == NULL || cpu->init == NULL) {
		printk(KERN_ERR "CPU %s support not enabled\n", cpu->name);
		panic("Unsupported WPCM450 CPU");
	}

	printk("CPU %s (id 0x%08lx)\n", cpu->name, idcode);

	(cpu->map_io)(mach_desc, size);
}

/* wpcm450_init_clocks
 *
 * Initialise the clock subsystem and associated information from the
 * given master crystal value.
 *
 * xtal  = 0 -> use default PLL crystal value (normally 15MHz)
 *      != 0 -> PLL crystal value in Hz
*/

void __init wpcm4xx_init_clocks(int xtal)
{
	if (xtal == 0)
	  xtal = 14318000;//15*1000*1000;

	if (cpu == NULL)
		panic("wpcm4xx_init_clocks: no cpu setup?\n");

	if (cpu->init_clocks == NULL)
		panic("wpcm4xx_init_clocks: cpu has no clock init\n");
	else
		(cpu->init_clocks)(xtal);
}

void __init wpcm4xx_init_uarts(struct wpcm450_uartcfg *cfg, int no)
{
	if (cpu == NULL)
		return;

	if (cpu->init_uarts == NULL) {
		printk(KERN_ERR "wpcm4xx_init_uarts: cpu has no uart init\n");
	} else
		(cpu->init_uarts)(cfg, no);
}

static int __init wpcm_arch_init(void)
{
	int ret;

	// do the correct init for cpu

	if (cpu == NULL)
		panic("wpcm_arch_init: NULL cpu\n");

	ret = (cpu->init)();
	if (ret != 0)
		return ret;

	if (board != NULL) {
		struct platform_device **ptr = board->devices;
		int i;

		for (i = 0; i < board->devices_count; i++, ptr++) {
			ret = platform_device_register(*ptr);

			if (ret) {
				printk(KERN_ERR "wpcm450: failed to add board device %s (%d) @%p\n", (*ptr)->name, ret, *ptr);
			}
		}

		/* mask any error, we may not need all these board
		 * devices */
		ret = 0;
	}

	return ret;
}

arch_initcall(wpcm_arch_init);
