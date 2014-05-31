/* arch/arm/mach-wpcm450/wpcm450.h
 *
* Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * Header file for wpcm450 machine directory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
  *  Changelog:
 *    22-08-2003     clyu     Created file
*/

extern  int wpcm450_init(void);

extern void wpcm450_map_io(struct map_desc *mach_desc, int size);

extern void wpcm450_init_uarts(struct wpcm450_uartcfg *cfg, int no);

extern void wpcm450_init_clocks(int xtal);

