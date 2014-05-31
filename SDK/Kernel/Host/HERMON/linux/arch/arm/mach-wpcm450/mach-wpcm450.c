/***********************************************************************
 *
 * linux/arch/arm/mach-wpcm450/mach-wpcm450.c
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
 *
 ***********************************************************************/

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <linux/platform_device.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/mach-types.h>

#include <asm/arch/regs-serial.h>

#include "devs.h"
#include "cpu.h"
static struct wpcm450_uartcfg wpcm450_uartcfgs[] = {
	[0] = {
		.hwport	     = 0,
		.flags	     = 0,
		.ucon	     = 0,
		.ulcon	     = 0,
		.ufcon	     = 0,
	}
};
static struct platform_device *wpcm450_devices[] __initdata = {
};

static struct wpcm450_board wpcm450_board __initdata = {
	.devices       = wpcm450_devices,
	.devices_count = ARRAY_SIZE(wpcm450_devices)
};


static void __init mach_wpcm450_map_io(void)
	
{
//printk("wpcm450_fb_set_platdata --mach_wpcm450_map_io\n");
	//wpcm450_fb_set_platdata(&WPCM450_lcd_platdata);
	wpcm450_init_io(NULL, 0);
	wpcm4xx_init_clocks(0);
	wpcm4xx_init_uarts(wpcm450_uartcfgs, ARRAY_SIZE(wpcm450_uartcfgs));
	wpcm450_set_board(&wpcm450_board);
	//wpcm450_fb_set_platdata(&WPCM450_lcd_platdata);
}

MACHINE_START(WPCM450, "wpcm450") /* @TODO: request a new identifier and switch
				    * to SMDK2410 */
	/* Maintainer: Jonas Dietsche */
	.phys_io	= WPCM450_PA_UART,
	.io_pg_offst	= (((u32)WPCM450_VA_UART) >> 18) & 0xfffc,
	.boot_params	= 0,//WPCM450_SDRAM_PA + 0x100,
	.map_io		= mach_wpcm450_map_io,
	.init_irq	= wpcm450_init_irq,
	.init_machine	= NULL,
	.timer		= &wpcm450_timer,
MACHINE_END


