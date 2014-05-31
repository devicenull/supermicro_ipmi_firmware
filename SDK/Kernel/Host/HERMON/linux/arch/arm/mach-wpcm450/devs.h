/* arch/arm/mach-wpcm450/devs.h
 *
 * Copyright (c) 2006 Winbond Electronics
 * All rights reserved.
 *
 * shirley yu (clyu2@winbond.com)
 *
 * Header file for wpcm450 standard platform devices
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *   23-Aug-2006  clyu created file
*/
#include <linux/config.h>
#include <linux/platform_device.h>

extern struct platform_device *wpcm450_uart_devs[];
extern struct platform_device wpcm_device_timer0;
extern struct platform_device wpcm_device_timer1;
extern struct platform_device wpcm_device_timer2;
extern struct platform_device wpcm_device_timer3;
extern struct platform_device wpcm_device_timer4;

