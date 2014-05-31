/* linux/arch/arm/mach-wpcm450/irq.c
 *
 * Copyright (c) 2006 Winbond Electronics
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
 *
 * Changelog:
 *
 *   21-Aug-2006  shirley yu <clyu2@winbond.com>
 *                add wpcm450 platform supporting
 *
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/ptrace.h>
#include <linux/sysdev.h>

#include <asm/hardware.h>
#include <asm/irq.h>
#include <asm/io.h>

#include <asm/mach/irq.h>

#include <asm/arch/regs-irq.h>

#include "cpu.h"
#include "pm.h"

#define irqdbf(x...)	//printk(x...)
#define irqdbf2(x...)	//printk(x...)

static void
wpcm_irq_mask(unsigned int irqno)
{
	unsigned long mask;

	mask = 1UL << irqno;
	__raw_writel(mask, WPCM450_AIC_MDCR);
}

static inline void
wpcm_irq_ack(unsigned int irqno)
{	
//	if(irqno == IRQ_TIMER0)
 //  		__raw_writel(0x01, WPCM450_TISR); /* clear TIF0 */

	__raw_writel(0x01, WPCM450_AIC_EOSCR);
	
}

static inline void
wpcm_irq_maskack(unsigned int irqno)
{
	__raw_writel(1 << irqno, WPCM450_AIC_SCCR);	//edge trigger clear
}


static void
wpcm_irq_unmask(unsigned int irqno)
{
	unsigned long mask;
	
	if (irqno == IRQ_TIMER0)
	{
	  __raw_writel(7<<16 | __raw_readl(WPCM450_AIC_GEN), WPCM450_AIC_GEN);
	  //__raw_writel(7<<16, WPCM450_AIC_GEN);
		__raw_writel (1<<IRQ_T_INT_GROUP, WPCM450_AIC_MECR);
	}

	mask = (1UL << irqno);
	__raw_writel(mask, WPCM450_AIC_MECR);
}

struct irqchip wpcm_irq_edge_chip = {
	.ack	   = wpcm_irq_maskack,
	.mask	   = wpcm_irq_mask,
	.unmask	   = wpcm_irq_unmask,
	.set_wake	   = NULL
};

static struct irqchip wpcm_irq_chip = {
	.ack	   = wpcm_irq_ack,
	.mask	   = wpcm_irq_mask,
	.unmask	   = wpcm_irq_unmask,
	.set_wake	   = NULL
};



/* wpcm450_init_irq
 *
 * Initialise WPCM450 IRQ system
*/

void __init wpcm450_init_irq(void)
{
	unsigned long pend;
	unsigned long last;
	int irqno;
	int i;

	__raw_writel(0xFFFFFFFE, WPCM450_AIC_MDCR); /* disable all interrupts */
	
	/* register the main interrupts */

	irqdbf("wpcm450_init_irq: registering wpcm_0 interrupt handlers\n");

	for (irqno = IRQ_WDT; irqno <= IRQ_ADC; irqno++) {
		/* set all the wpcm450 internal irqs */

		switch (irqno) {
			/* deal with the special IRQs (cascaded) */

		case IRQ_UART0:
		case IRQ_TIMER0:
			set_irq_chip(irqno, &wpcm_irq_chip);
			set_irq_handler(irqno, do_level_IRQ);
			set_irq_flags(irqno, IRQF_VALID);
			break;

		default:
			//irqdbf("registering irq %d (wpcm_irq)\n", irqno);
			set_irq_chip(irqno, &wpcm_irq_chip);
			set_irq_handler(irqno, do_level_IRQ);
			set_irq_flags(irqno, IRQF_VALID);
		}
	}

	irqdbf("wpcm450: registered interrupt handlers\n");
}
