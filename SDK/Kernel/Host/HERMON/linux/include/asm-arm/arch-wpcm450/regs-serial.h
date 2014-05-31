/* linux/include/asm-arm/arch-wpcm450/regs-serial.h
 *
  * Copyright (c) 2006 winbond Electronics
 *	shirley yu <clyu2@winbond.com>
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
  *  Changelog:
 *    22-08-2006     clyu     Created file
 */

#ifndef __ASM_ARM_REGS_SERIAL_H
#define __ASM_ARM_REGS_SERIAL_H

#define WPCM450_VA_UART0      (WPCM450_VA_UART)
#define WPCM450_VA_UART1      (WPCM450_VA_UART + 0x100 )

#define WPCM450_PA_UART0      (WPCM450_PA_UART)
#define WPCM450_PA_UART1      (WPCM450_PA_UART + 0x100 )

#define WPCM450_COM_TX	  (0x00)
#define WPCM450_COM_RX	  (0x00)
#define WPCM450_COM_LSB	  (0x00)
#define WPCM450_COM_MSB	  (0x04)
#define WPCM450_COM_IER	  (0x04)
#define WPCM450_COM_IIR	  (0x08)
#define WPCM450_COM_FCR	  (0x08)
#define WPCM450_COM_LCR	  (0x0C)
#define WPCM450_COM_MCR	  (0x10)
#define WPCM450_COM_LSR	  (0x14)
#define WPCM450_COM_MSR	  (0x18)
#define WPCM450_COM_TOR	  (0x1C)


//uart0/uart1
#define UARTx_FCR_FIFO_LEVEL1 0x00
#define UARTx_FCR_FIFO_LEVEL4 0x40
#define UARTx_FCR_FIFO_LEVEL8 0x80
#define UARTx_FCR_FIFO_LEVEL14 0xc0


#define UART_FCR_FME	0x01
#define UART_FCR_RFR	0x02
#define UART_FCR_TFR	0x04

#define UART_TXRXFIFO_RESET	\
	(UART_FCR_FME | UART_FCR_RFR | UART_FCR_TFR)

#define UART_LSR_OE	0x02		// Overrun error
#define UART_LSR_PE	0x04		// Parity error
#define UART_LSR_FE	0x08		// Frame error
#define UART_LSR_BI	0x10		// Break detect
//#define UART_LSR_DTR	0x10		// Data terminal ready
#define UART_LSR_DR 0x01
#define UART_LSR_THRE 0x20
#define UART_IIR_NIP	0x01		// Transmit buffer register empty
#define UART_IIR_THRE	0x02		// Transmit buffer register empty
#define UART_IIR_DR		0x04		// Receive data ready
#define UART_IIR_TOUT	0x0C		// receive fifo timeout
#define UART_LSR_TEMT	0x40		// Transmit complete

#define UART_LCR_WLEN5	0x00
#define UART_LCR_WLEN6	0x01
#define UART_LCR_WLEN7	0x02
#define UART_LCR_WLEN8	0x03
#define UART_LCR_CSMASK	  (0x3)
#define UART_LCR_PARITY	0x08
#define UART_LCR_NPAR	0x00
#define UART_LCR_OPAR	0x00
#define UART_LCR_EPAR	0x10
#define UART_LCR_PMMASK	  (0x30)
#define UART_LCR_SPAR	0x20
#define UART_LCR_SBC	0x40
#define UART_LCR_NSB	0x00
#define UART_LCR_NSB1_5	0x04

#define UART_GCR_RX_INT	0x01
#define UART_GCR_TX_INT	0x08
#define UART_GCR_RX_STAT_INT	0x04

#define UART_IER_MSI
#define UART_IER_RLSI	0x04
#define UART_IER_THRI	0x02
#define UART_IER_RDI	0x01
	
#define UART_MSR		0
#define UART_MSR_DCD		0
#define UART_MSR_RI		0
#define UART_MSR_DSR		0
#define UART_MSR_CTS		0
#define UART_MSR_DDCD		0
#define UART_MSR_TERI		0
#define UART_MSR_DDSR		0
#define UART_MSR_DCTS		0
#define UART_MSR_ANY_DELTA	0

#define WPCM450_LSRSTAT_ANY	  (UART_LSR_OE | \
				   UART_LSR_FE | \
				   UART_LSR_BI)

#ifndef __ASSEMBLY__

/* struct wpcm450_uart_clksrc
 *
 * this structure defines a named clock source that can be used for the
 * uart, so that the best clock can be selected for the requested baud
 * rate.
 *
 * min_baud and max_baud define the range of baud-rates this clock is
 * acceptable for, if they are both zero, it is assumed any baud rate that
 * can be generated from this clock will be used.
 *
 * divisor gives the divisor from the clock to the one seen by the uart
*/

struct wpcm450_uart_clksrc {
	const char	*name;
	unsigned int	 divisor;
	unsigned int	 min_baud;
	unsigned int	 max_baud;
};

/* configuration structure for per-machine configurations for the
 * serial port
 *
 * the pointer is setup by the machine specific initialisation from the
 * arch/arm/mach-wpcm450/ directory.
*/

struct wpcm450_uartcfg {
	unsigned char	   hwport;	 /* hardware port number */
	unsigned char	   unused;
	unsigned short	   flags;
	unsigned long	   uart_flags;	 /* default uart flags */

	unsigned long	   ucon;	 /* value of ucon for port */
	unsigned long	   ulcon;	 /* value of ulcon for port */
	unsigned long	   ufcon;	 /* value of ufcon for port */

	struct s3c24xx_uart_clksrc *clocks;
	unsigned int		    clocks_size;
};

/* wpcm450_uart_devs
 *
 * this is exported from the core as we cannot use driver_register(),
 * or platform_add_device() before the console_initcall()
*/

extern struct platform_device *wpcm450_uart_devs[];

#endif /* __ASSEMBLY__ */

#endif /* __ASM_ARM_REGS_SERIAL_H */

