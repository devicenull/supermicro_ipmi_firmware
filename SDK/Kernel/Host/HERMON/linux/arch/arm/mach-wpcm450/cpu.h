/* arch/arm/mach-wpcm450/cpu.h
 *
 * Copyright (c) 2006 Winbond Electronics
 *	shirley yu <clyu2@winbond.com>
 *
 * Header file for WPCM450 CPU support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Modifications:
 *     22-Aug-2006 clyu  create file
*/

#define IODESC_ENT(x) { (unsigned long)WPCM450_VA_##x, __phys_to_pfn(WPCM450_PA_##x), WPCM450_SZ_##x, MT_DEVICE }
#define IODESC_USER(x) { (unsigned long)WPCM450_VA_##x, __phys_to_pfn(WPCM450_PA_##x), WPCM450_SZ_##x, MT_DEVICE_USER }
#define IODESC_MEM(x) { (unsigned long)WPCM450_VA_##x, __phys_to_pfn(WPCM450_PA_##x), WPCM450_SZ_##x, MT_LOW_VECTORS }

#ifndef MHZ
#define MHZ (1000*1000)
#endif

#define print_mhz(m) ((m) / MHZ), ((m / 1000) % 1000)

/* forward declaration */
struct wpcm450_uartcfg;
struct map_desc;

/* core initialisation functions */

extern void wpcm450_init_irq(void);

extern void wpcm450_init_io(struct map_desc *mach_desc, int size);

extern void wpcm4xx_init_uarts(struct wpcm450_uartcfg *cfg, int no);

extern void wpcm4xx_init_clocks(int xtal);

/* the board structure is used at first initialsation time
 * to get info such as the devices to register for this
 * board. This is done because platfrom_add_devices() cannot
 * be called from the map_io entry.
*/

struct wpcm450_board {
	struct platform_device  **devices;
	unsigned int              devices_count;

	struct clk		**clocks;
	unsigned int		  clocks_count;
};

extern void wpcm450_set_board(struct wpcm450_board *board);

/* timer for wpcm450 */

struct sys_timer;
extern struct sys_timer wpcm450_timer;

