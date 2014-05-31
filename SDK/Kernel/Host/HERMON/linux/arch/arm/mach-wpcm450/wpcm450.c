/* linux/arch/arm/mach-wpcm450/wpcm450.c
 *
 * Copyright (c) 2006 winbond Electronics
 *	shirley yu <clyu2@winbond.com>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 *  Changelog:
 *    22-08-2006     clyu     Created file
 *    30-12-2006     zswan    Change file
*/

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

#include <asm/arch/regs-clock.h>
#include <asm/arch/regs-serial.h>

#include "wpcm450.h"
#include "cpu.h"
#include "clock.h"

/* Initial IO mappings */

static struct map_desc wpcm450_iodesc[] __initdata = {	
};

static struct resource wpcm_uart0_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART0,
		.end   = WPCM450_PA_UART0 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART0,
		.end   = IRQ_UART0,
		.flags = IORESOURCE_IRQ,
	}

};

static struct resource wpcm_uart1_resource[] = {
	[0] = {
		.start = WPCM450_PA_UART1,
		.end   = WPCM450_PA_UART1 + 0x0ff,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_UART1,
		.end   = IRQ_UART1,
		.flags = IORESOURCE_IRQ,
	}
};

/* our uart devices */

static struct platform_device wpcm_uart0 = {
	.name		  = "wpcm450-uart",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(wpcm_uart0_resource),
	.resource	  = wpcm_uart0_resource,
};


static struct platform_device wpcm_uart1 = {
	.name		  = "wpcm450-uart",
	.id		  = 1,
	.num_resources	  = ARRAY_SIZE(wpcm_uart1_resource),
	.resource	  = wpcm_uart1_resource,
};

static struct platform_device *uart_devices[] __initdata = {
	&wpcm_uart0,
	&wpcm_uart1
};

static int wpcm450_uart_count = 0;

/* uart registration process */

void ComInit ()
{
    volatile char inChar;

    __raw_writel (0x80, WPCM450_VA_UART0 + WPCM450_COM_LCR); /* select divisor latch registers */
    
    __raw_writel (/*6*/ 5, WPCM450_VA_UART0 + WPCM450_COM_LSB);  /* Baud rate = 115200 : 15,000,000/(16*(6+2))  */
    //__raw_writel (COM_LSB, 96);	  /* Baud rate = 9600   : 15,000,000/(16*(96+2)) */
    __raw_writel (0, WPCM450_VA_UART0 + WPCM450_COM_MSB);

    __raw_writel (0x03, WPCM450_VA_UART0 + WPCM450_COM_LCR); /* none parity, 8 data bits, 1 stop bits */
    
    __raw_writel (0x01, WPCM450_VA_UART0 + WPCM450_COM_IER); /* RDA interrupt source on / LOOP disable */
    __raw_writel (0x00, WPCM450_VA_UART0 + WPCM450_COM_MCR);

    __raw_writel (0x80+0x20, WPCM450_VA_UART0 + WPCM450_COM_TOR); /* Timeout if more than ??? bits xfer time */

    __raw_writel (0x8F, WPCM450_VA_UART0 + WPCM450_COM_FCR); /* 8-byte FIFO trigger level, reset Tx and Rx FIFO */

    //inChar = __raw_readb (WPCM450_VA_UART0 + WPCM450_COM_RX); /* Reset interrupt status */

} /* end ComInit */



void __init wpcm450_init_uarts(struct wpcm450_uartcfg *cfg, int no)
{
	struct platform_device *platdev;
	int uart;

	for (uart = 0; uart < no; uart++, cfg++) {
		platdev = uart_devices[cfg->hwport];

		wpcm450_uart_devs[uart] = platdev;
		platdev->dev.platform_data = cfg;
	}

	wpcm450_uart_count = uart;
	
	ComInit();
}

/* wpcm450_map_io
 *
 * register the standard cpu IO areas, and any passed in from the
 * machine specific initialisation.
*/

void __init wpcm450_map_io(struct map_desc *mach_desc, int mach_size)
{
	/* register our io-tables */

	iotable_init(wpcm450_iodesc, ARRAY_SIZE(wpcm450_iodesc));
	iotable_init(mach_desc, mach_size);
}

void __init wpcm450_init_clocks(int xtal)
{
	//s3c24xx_setup_clocks(xtal, fclk, hclk, pclk);
}

int __init wpcm450_init(void)
{
	printk("WPCM450: Initialising architecture\n");

	return platform_add_devices(wpcm450_uart_devs, wpcm450_uart_count);
}
