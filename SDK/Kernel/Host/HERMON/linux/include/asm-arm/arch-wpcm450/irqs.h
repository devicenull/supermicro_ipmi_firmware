/* linux/include/asm-arm/arch-wpcm450/irqs.h
 *
 * Copyright (c) 2006 Winbond Electronics
 *   shirley yu <clyu2@winbond.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Changelog:
 *  22-Aug-2006 clyu  Created file
 */


#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H __FILE__


/* we keep the first set of CPU IRQs out of the range of
 * the ISA space, so that the PC104 has them to itself
 * and we don't end up having to do horrible things to the
 * standard ISA drivers....
 */

#define WPCM450_IRQ(x) (x)

/* main cpu interrupts */
#define IRQ_WDT		   WPCM450_IRQ(1)	
#define IRQ_GROUP0     WPCM450_IRQ(2)
#define IRQ_GROUP1     WPCM450_IRQ(3)
#define IRQ_GROUP2     WPCM450_IRQ(4)
#define IRQ_GROUP3     WPCM450_IRQ(5)
#define IRQ_PECI       WPCM450_IRQ(6)
#define IRQ_UART0      WPCM450_IRQ(7)
#define IRQ_UART1	   WPCM450_IRQ(8)
#define IRQ_KCS_HIB	   WPCM450_IRQ(9)
#define IRQ_FIU_SPI	   WPCM450_IRQ(10)
#define IRQ_SHM 	   WPCM450_IRQ(11)
#define IRQ_TIMER0     WPCM450_IRQ(12)
#define IRQ_TIMER1     WPCM450_IRQ(13)
#define IRQ_TGROUP     WPCM450_IRQ(14)
#define IRQ_EMCRX1     WPCM450_IRQ(15)
#define IRQ_EMCTX1     WPCM450_IRQ(16)
#define IRQ_EMCRX2     WPCM450_IRQ(17)
#define IRQ_EMCTX2     WPCM450_IRQ(18)
//#define IRQ_RESERVED   WPCM450_IRQ(19)
#define IRQ_USBD2      WPCM450_IRQ(20)
#define IRQ_USBD1      WPCM450_IRQ(21)
#define IRQ_VCD        WPCM450_IRQ(22)
#define IRQ_SMB3       WPCM450_IRQ(23)
#define IRQ_MFT0       WPCM450_IRQ(24)
#define IRQ_MFT1       WPCM450_IRQ(25)
#define IRQ_I2CGROUP   WPCM450_IRQ(26)
#define IRQ_I2C4       WPCM450_IRQ(27)
#define IRQ_PWM	       WPCM450_IRQ(28)
#define IRQ_I2C5       WPCM450_IRQ(29)
#define IRQ_MPG        WPCM450_IRQ(30)
#define IRQ_ADC		   WPCM450_IRQ(31)

#define IRQ_T_INT_GROUP	IRQ_TGROUP

#define NR_IRQS 32


#endif /* __ASM_ARCH_IRQ_H */
