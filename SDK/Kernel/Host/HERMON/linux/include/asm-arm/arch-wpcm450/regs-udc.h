/* linux/include/asm/arch-wpcm450/regs-udc.h
 *
 * Copyright (C) 2004 Herbert Poetzl <herbert@13thfloor.at>
 *
 * This include file is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 *  Changelog:
 *    01-08-2004	Initial creation
 *    12-09-2004	Cleanup for submission
 *    24-10-2004	Fixed WPCM450_UDC_MAXP_REG definition
 *    10-03-2005	Changed WPCM450_VA to WPCM450_VA
 */

#ifndef __ASM_ARCH_REGS_UDC_H
#define __ASM_ARCH_REGS_UDC_H


#include <asm/arch/map.h>

#define WPCM450_USB11_PORT  0x1
#define WPCM450_USB20_PORT  0x2

#define WPCM450_USBDREG(port, x) ((x) + ((port==0x1)?(WPCM450_VA_USB11_DEV):(WPCM450_VA_USB20_DEV)))

#define WPCM450_UDC_ID_REG(port)               WPCM450_USBDREG(port,0x000) 
#define WPCM450_UDC_HWGENERAL(port)            WPCM450_USBDREG(port,0x004) 
#define WPCM450_UDC_HWDEVICE_REG(port)         WPCM450_USBDREG(port,0x00C)
#define WPCM450_UDC_HWTXBUF_REG(port)          WPCM450_USBDREG(port,0x010)
#define WPCM450_UDC_HWRXBUF_REG(port)          WPCM450_USBDREG(port,0x014)

#define WPCM450_UDC_GPTIMER0LD_REG(port)       WPCM450_USBDREG(port,0x080)
#define WPCM450_UDC_GPTIMER0CTRL_REG(port)     WPCM450_USBDREG(port,0x084)
#define WPCM450_UDC_GPTIMER1LD_REG(port)       WPCM450_USBDREG(port,0x088)
#define WPCM450_UDC_GPTIMER1CTRL_REG(port)     WPCM450_USBDREG(port,0x08C)

#define WPCM450_UDC_CAPLENGTH_REG(port)        WPCM450_USBDREG(port,0x100)
#define WPCM450_UDC_HCS_PARAMS_REG(port)       WPCM450_USBDREG(port,0x104) 
#define WPCM450_UDC_HCC_PARAMS_REG(port)       WPCM450_USBDREG(port,0x108) 
#define WPCM450_UDC_DCIVERSION_REG(port)       WPCM450_USBDREG(port,0x120)
#define WPCM450_UDC_DCCPARAMS_REG(port)        WPCM450_USBDREG(port,0x124)
#define WPCM450_UDC_USBCMD_REG(port)           WPCM450_USBDREG(port,0x140)
#define WPCM450_UDC_USBSTS_REG(port)           WPCM450_USBDREG(port,0x144)
#define WPCM450_UDC_USBINTR_REG(port)          WPCM450_USBDREG(port,0x148)
#define WPCM450_UDC_FRINDEX_REG(port)          WPCM450_USBDREG(port,0x14C)
#define WPCM450_UDC_USBDEVICEADDR_REG(port)    WPCM450_USBDREG(port,0x154)
#define WPCM450_UDC_ENDPOINTLISTADDR_REG(port) WPCM450_USBDREG(port,0x158)
#define WPCM450_UDC_BURSTSIZE_REG(port)        WPCM450_USBDREG(port,0x160)
#define WPCM450_UDC_ENDPTNAK_REG(port)         WPCM450_USBDREG(port,0x178)
#define WPCM450_UDC_ENDPTNAKEN_REG(port)       WPCM450_USBDREG(port,0x17C)

#define WPCM450_UDC_PORTSC1_REG(port)          WPCM450_USBDREG(port,0x184)

#define WPCM450_UDC_USBMODE_REG(port)          WPCM450_USBDREG(port,0x1A8)
#define WPCM450_UDC_ENPDTSETUPSTAT_REG(port)   WPCM450_USBDREG(port,0x1AC)
#define WPCM450_UDC_ENDPTPRIME_REG(port)       WPCM450_USBDREG(port,0x1B0)
#define WPCM450_UDC_ENDPTFLUSH_REG(port)       WPCM450_USBDREG(port,0x1B4)
#define WPCM450_UDC_ENDPTSTATUS_REG(port)      WPCM450_USBDREG(port,0x1B8)
#define WPCM450_UDC_ENDPTCOMPLETE_REG(port)    WPCM450_USBDREG(port,0x1BC)
#define WPCM450_UDC_ENDPTCTRL0_REG(port)       WPCM450_USBDREG(port,0x1C0)
#define WPCM450_UDC_ENDPTCTRL1_REG(port)       WPCM450_USBDREG(port,0x1C4)
#define WPCM450_UDC_ENDPTCTRL2_REG(port)       WPCM450_USBDREG(port,0x1C8)
#define WPCM450_UDC_ENDPTCTRL3_REG(port)       WPCM450_USBDREG(port,0x1CC)
#define WPCM450_UDC_ENDPTCTRL4_REG(port)       WPCM450_USBDREG(port,0x1D0)
#define WPCM450_UDC_ENDPTCTRL5_REG(port)       WPCM450_USBDREG(port,0x1D4)
#define WPCM450_UDC_ENDPTCTRL6_REG(port)       WPCM450_USBDREG(port,0x1D8)
#define WPCM450_UDC_ENDPTCTRL7_REG(port)       WPCM450_USBDREG(port,0x1DC)



#define WPCM450_UDC_PWR_ISOUP		(1<<7) // R/W
#define WPCM450_UDC_PWR_RESET		(1<<3) // R
#define WPCM450_UDC_PWR_RESUME		(1<<2) // R/W
#define WPCM450_UDC_PWR_SUSPEND		(1<<1) // R
#define WPCM450_UDC_PWR_ENSUSPEND	(1<<0) // R/W

#define WPCM450_UDC_PWR_DEFAULT		0x00

#define WPCM450_UDC_INT_EP4		(1<<4) // R/W (clear only)
#define WPCM450_UDC_INT_EP3		(1<<3) // R/W (clear only)
#define WPCM450_UDC_INT_EP2		(1<<2) // R/W (clear only)
#define WPCM450_UDC_INT_EP1		(1<<1) // R/W (clear only)
#define WPCM450_UDC_INT_EP0		(1<<0) // R/W (clear only)

#define WPCM450_UDC_USBINT_RESET	(1<<2) // R/W (clear only)
#define WPCM450_UDC_USBINT_RESUME	(1<<1) // R/W (clear only)
#define WPCM450_UDC_USBINT_SUSPEND	(1<<0) // R/W (clear only)

#define WPCM450_UDC_INTE_EP4		(1<<4) // R/W
#define WPCM450_UDC_INTE_EP3		(1<<3) // R/W
#define WPCM450_UDC_INTE_EP2		(1<<2) // R/W
#define WPCM450_UDC_INTE_EP1		(1<<1) // R/W
#define WPCM450_UDC_INTE_EP0		(1<<0) // R/W

#define WPCM450_UDC_USBINTE_RESET	(1<<2) // R/W
#define WPCM450_UDC_USBINTE_SUSPEND	(1<<0) // R/W


#define WPCM450_UDC_INDEX_EP0		(0x00)
#define WPCM450_UDC_INDEX_EP1		(0x01) // ??
#define WPCM450_UDC_INDEX_EP2		(0x02) // ??
#define WPCM450_UDC_INDEX_EP3		(0x03) // ??
#define WPCM450_UDC_INDEX_EP4		(0x04) // ??

#define WPCM450_UDC_ICSR1_CLRDT		(1<<6) // R/W
#define WPCM450_UDC_ICSR1_SENTSTL	(1<<5) // R/W (clear only)
#define WPCM450_UDC_ICSR1_SENDSTL	(1<<4) // R/W
#define WPCM450_UDC_ICSR1_FFLUSH	(1<<3) // W   (set only)
#define WPCM450_UDC_ICSR1_UNDRUN	(1<<2) // R/W (clear only)
#define WPCM450_UDC_ICSR1_PKTRDY	(1<<0) // R/W (set only)

#define WPCM450_UDC_ICSR2_AUTOSET	(1<<7) // R/W
#define WPCM450_UDC_ICSR2_ISO		(1<<6) // R/W
#define WPCM450_UDC_ICSR2_MODEIN	(1<<5) // R/W
#define WPCM450_UDC_ICSR2_DMAIEN	(1<<4) // R/W

#define WPCM450_UDC_OCSR1_CLRDT		(1<<7) // R/W
#define WPCM450_UDC_OCSR1_SENTSTL	(1<<6) // R/W (clear only)
#define WPCM450_UDC_OCSR1_SENDSTL	(1<<5) // R/W
#define WPCM450_UDC_OCSR1_FFLUSH	(1<<4) // R/W
#define WPCM450_UDC_OCSR1_DERROR	(1<<3) // R
#define WPCM450_UDC_OCSR1_OVRRUN	(1<<2) // R/W (clear only)
#define WPCM450_UDC_OCSR1_PKTRDY	(1<<0) // R/W (clear only)

#define WPCM450_UDC_OCSR2_AUTOCLR	(1<<7) // R/W
#define WPCM450_UDC_OCSR2_ISO		(1<<6) // R/W
#define WPCM450_UDC_OCSR2_DMAIEN	(1<<5) // R/W

#define WPCM450_UDC_SETIX(x)	    \
	__raw_writel(WPCM450_UDC_INDEX_ ## x, WPCM450_UDC_INDEX_REG);


#define WPCM450_UDC_EP0_CSR_OPKRDY	(1<<0)
#define WPCM450_UDC_EP0_CSR_IPKRDY	(1<<1)
#define WPCM450_UDC_EP0_CSR_SENTSTL	(1<<2)
#define WPCM450_UDC_EP0_CSR_DE		(1<<3)
#define WPCM450_UDC_EP0_CSR_SE		(1<<4)
#define WPCM450_UDC_EP0_CSR_SENDSTL	(1<<5)
#define WPCM450_UDC_EP0_CSR_SOPKTRDY	(1<<6)
#define WPCM450_UDC_EP0_CSR_SSE	(1<<7)

#define WPCM450_UDC_MAXP_8		(1<<0)
#define WPCM450_UDC_MAXP_16		(1<<1)
#define WPCM450_UDC_MAXP_32		(1<<2)
#define WPCM450_UDC_MAXP_64		(1<<3)


#endif
