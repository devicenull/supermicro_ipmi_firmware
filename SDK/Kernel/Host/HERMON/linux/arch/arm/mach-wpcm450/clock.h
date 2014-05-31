/*
 * linux/arch/arm/mach-wpcm450/clock.h
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

struct clk {
	struct list_head      list;
	struct module        *owner;
	struct clk           *parent;
	const char           *name;
	int		      id;
	int		      usage;
	unsigned long         rate;
	unsigned long         ctrlbit;

	int		    (*enable)(struct clk *, int enable);
	int		    (*set_rate)(struct clk *c, unsigned long rate);
	unsigned long	    (*round_rate)(struct clk *c, unsigned long rate);
	int		    (*set_parent)(struct clk *c, struct clk *parent);
};

/* other clocks which may be registered by board support */

extern struct clk wpcm450_dclk0;
extern struct clk wpcm450_dclk1;
extern struct clk wpcm450_clkout0;
extern struct clk wpcm450_clkout1;
extern struct clk wpcm450_uclk;

extern struct clk clk_usb_bus;

/* exports for arch/arm/mach-wpcm450
 *
 * Please DO NOT use these outside of arch/arm/mach-wpcm450
*/

extern struct mutex clocks_mutex;

extern int wpcm450_clkcon_enable(struct clk *clk, int enable);
extern int wpcm450_register_clock(struct clk *clk);
