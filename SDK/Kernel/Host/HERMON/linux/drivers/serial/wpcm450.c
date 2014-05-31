/*
 * linux/drivers/serial/wpcm450.c
 *
 * Driver for onboard UARTs on the Winbond WPCM450
 *
 *
 * (c) 2006 Winbond Electronics
 *
 * Changelog:
 *
 * 18-Aug-2006  CLYU  Created file
*/

#include <linux/config.h>

#if defined(CONFIG_SERIAL_WPCM450_CONSOLE) && defined(CONFIG_MAGIC_SYSRQ)
#define SUPPORT_SYSRQ
#endif

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/sysrq.h>
#include <linux/console.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/delay.h>
#include <linux/clk.h>

#include <asm/io.h>
#include <asm/irq.h>

#include <asm/hardware.h>

#include <asm/arch/regs-serial.h>

static irqreturn_t wpcm450_serial_tx_chars(int irq, void *id, struct pt_regs *regs);

/* structures */

struct wpcm450_uart_info {
	char			*name;
	unsigned int		type;
	unsigned int		fifosize;

	/* uart controls */
	int (*reset_port)(struct uart_port *, struct wpcm450_uartcfg *);
};

struct wpcm450_uart_port {
	unsigned char			rx_claimed;
	unsigned char			tx_claimed;

	struct wpcm450_uart_info	*info;
	struct wpcm450_uart_clksrc	*clksrc;
	struct clk			*clk;
	struct clk			*baudclk;
	struct uart_port		port;
};


/* configuration defines */

#define dbg(x...) do {} while(0)


/* UART name and device definitions */

#define WPCM450_SERIAL_NAME	"ttyS"
#define WPCM450_SERIAL_DEVFS    "tts/"
#define WPCM450_SERIAL_MAJOR	4
#define WPCM450_SERIAL_MINOR	64


/* conversion functions */

#define wpcm450_dev_to_port(__dev) (struct uart_port *)dev_get_drvdata(__dev)
#define wpcm450_dev_to_cfg(__dev) (struct wpcm450_uartcfg *)((__dev)->platform_data)

/* we can support 5 uarts, but not always use them */

#define NR_PORTS (2)


/* port irq numbers */

#define TX_IRQ(port) ((port)->irq)
#define RX_IRQ(port) ((port)->irq)

/* register access controls */

#define portaddr(port, reg) ((port)->membase + (reg))

#define rd_regb(port, reg) (__raw_readb(portaddr(port, reg)))
#define rd_regl(port, reg) (__raw_readl(portaddr(port, reg)))

#define wr_regb(port, reg, val) \
  do { __raw_writeb(val, portaddr(port, reg)); } while(0)

#define wr_regl(port, reg, val) \
  do { __raw_writel(val, portaddr(port, reg)); } while(0)

/* macros to change one thing to another */

#define tx_enabled(port) ((port)->unused[0])
#define rx_enabled(port) ((port)->unused[1])

#define tx_disable(port) wr_regl(port, WPCM450_COM_IER, rd_regl(port, WPCM450_COM_IER) & ~UART_IER_THRI)
#define tx_enable(port)	wr_regl(port, WPCM450_COM_IER, rd_regl(port, WPCM450_COM_IER) | UART_IER_THRI)
#define rx_disable(port) wr_regl(port, WPCM450_COM_IER, rd_regl(port, WPCM450_COM_IER) & ~UART_IER_RDI);	\
	wr_regl(port, WPCM450_COM_TOR, 0x00)
#define rx_enable(port)	wr_regl(port, WPCM450_COM_IER, rd_regl(port, WPCM450_COM_IER) | UART_IER_RDI); \
	wr_regl(port, WPCM450_COM_TOR, 0x80 + 0x20)

/* flag to ignore all characters comming in */
#define RXSTAT_DUMMY_READ (0x10000000)

static inline struct wpcm450_uart_port *to_ourport(struct uart_port *port)
{
	return container_of(port, struct wpcm450_uart_port, port);
}

/* translate a port to the device name */

static inline const char *wpcm450_serial_portname(struct uart_port *port)
{
	return to_platform_device(port->dev)->name;
}

static int wpcm450_serial_txempty_nofifo(struct uart_port *port)
{
	return (rd_regl(port, WPCM450_COM_LSR) & UART_LSR_THRE);
}

static void wpcm450_serial_rx_enable(struct uart_port *port)
{
	unsigned long flags;
	unsigned int fcr;
	int count = 10000;

	spin_lock_irqsave(&port->lock, flags);

	fcr = rd_regl(port, WPCM450_COM_FCR);
	fcr |= UART_FCR_RFR | UART_FCR_FME | UARTx_FCR_FIFO_LEVEL14;
	wr_regl(port, WPCM450_COM_FCR, fcr);
	
	rx_enable(port);
	rx_enabled(port) = 1;
	spin_unlock_irqrestore(&port->lock, flags);
}

static void wpcm450_serial_rx_disable(struct uart_port *port)
{
	unsigned long flags;
	unsigned int ucon;
//printk("wpcm450_serial_rx_disable\n");
	spin_lock_irqsave(&port->lock, flags);

	rx_disable(port);

	rx_enabled(port) = 0;
	spin_unlock_irqrestore(&port->lock, flags);
}

static void wpcm450_serial_stop_tx(struct uart_port *port)
{
//	printk("\nwpcm450_serial_stop_tx %d\n",tx_enabled(port));
	if (tx_enabled(port)) {
		//disable_irq(TX_IRQ(port));
		tx_disable(port);
		tx_enabled(port) = 0;
		if (port->flags & UPF_CONS_FLOW)
			wpcm450_serial_rx_enable(port);
	}
}

static void wpcm450_serial_start_tx(struct uart_port *port)
{
//	printk("wpcm450_serial_start_tx %d\n",tx_enabled(port));
	if (!tx_enabled(port)) {
		if (port->flags & UPF_CONS_FLOW)
			wpcm450_serial_rx_disable(port);

		//enable_irq(TX_IRQ(port));
		tx_enable(port);
		tx_enabled(port) = 1;
	}
}


static void wpcm450_serial_stop_rx(struct uart_port *port)
{
	if (rx_enabled(port)) {
		//printk("wpcm450_serial_stop_rx: port=%p\n", port);
		//disable_irq(RX_IRQ(port));
		rx_disable(port);
		rx_enabled(port) = 0;
	}
}

static void wpcm450_serial_enable_ms(struct uart_port *port)
{
}

static inline struct wpcm450_uart_info *wpcm450_port_to_info(struct uart_port *port)
{
	return to_ourport(port)->info;
}

static inline struct wpcm450_uartcfg *wpcm450_port_to_cfg(struct uart_port *port)
{
	if (port->dev == NULL)
		return NULL;

	return (struct wpcm450_uartcfg *)port->dev->platform_data;
}

static int max_count;
/* ? - where has parity gone?? */
static inline int  CAN_GETC(struct uart_port *port, unsigned int iir)
{
	if(iir == UART_IIR_DR)
		return max_count--;
	else if (iir == UART_IIR_TOUT)
		return rd_regl(port, WPCM450_COM_LSR) & UART_LSR_DR;
	return -1;
}		

static irqreturn_t
wpcm450_serial_rx_chars(int irq, void *dev_id, struct pt_regs *regs)
{
	struct wpcm450_uart_port *ourport = dev_id;
	struct uart_port *port = &ourport->port;
	struct tty_struct *tty = port->info->tty;
	unsigned int iir, ch, flag, ufstat, lsrstat;
	struct wpcm450_uart_info *info = ourport->info;
	irqreturn_t ret;
	//int max_count = 64;
	
	max_count = info->fifosize;
	
again:	
	iir = rd_regl(port, WPCM450_COM_IIR) & 0x0E;

	if(iir == UART_IIR_THRE)
	{
		ret = wpcm450_serial_tx_chars(irq, dev_id, regs);
		//printk("ret = %d\n",ret);
		return ret;
	}
	if(iir == UART_IIR_DR)
		max_count = info->fifosize;
	else if(iir == UART_IIR_TOUT)
		max_count = 1;
	else
		return IRQ_HANDLED;
	//printk("\nwpcm450_serial_rx_chars %x\n",iir);
	while ((max_count = CAN_GETC(port, iir)) > 0) {

		lsrstat = rd_regl(port, WPCM450_COM_LSR);
		ch = rd_regb(port, WPCM450_COM_RX);
//printk("port->flags=%x,ch=%x,max_count=%d\n",port->flags,ch,max_count);
		if (port->flags & UPF_CONS_FLOW) {
			int txe = wpcm450_serial_txempty_nofifo(port);

			if (rx_enabled(port)) {
				if (!txe) {
					rx_enabled(port) = 0;
					continue;
				}
			} else {
				if (txe) {
					wr_regl(port, WPCM450_COM_FCR, rd_regl(port, WPCM450_COM_FCR) | UART_FCR_RFR | UART_FCR_FME);
					rx_enabled(port) = 1;
					goto out;
				}
				continue;
			}
		}

		
		/* insert the character into the buffer */

		flag = TTY_NORMAL;
		port->icount.rx++;
//printk("00lsrstat=%x\n",lsrstat);
		if (unlikely(lsrstat & WPCM450_LSRSTAT_ANY)) {
			dbg("rxerr: port ch=0x%02x, rxs=0x%08x\n",
		    	ch, lsrstat);

			/* check for break */
			if (lsrstat & UART_LSR_BI) {
				dbg("break!\n");
				port->icount.brk++;
				if (uart_handle_break(port))
			    	goto ignore_char;
			}

			if (lsrstat & UART_LSR_FE)
				port->icount.frame++;
			if (lsrstat & UART_LSR_OE)
				port->icount.overrun++;

			lsrstat &= port->read_status_mask;

			if (lsrstat & UART_LSR_BI)
				flag = TTY_BREAK;
			else if (lsrstat & UART_LSR_PE)
				flag = TTY_PARITY;
			else if (lsrstat & ( UART_LSR_FE | UART_LSR_OE))
				flag = TTY_FRAME;
		}

		if (uart_handle_sysrq_char(port, ch, regs))
			goto ignore_char;
//printk("lsrstat=%x\n",lsrstat);
		uart_insert_char(port, lsrstat, UART_LSR_OE, ch, flag);

	ignore_char:
		continue;
	}
	
#if 0
	iir = rd_regl(port, WPCM450_COM_IIR);
	if(!(iir & UART_IIR_NIP))
			goto again;
#endif
//	printk("call tty_flip_buffer_push\n");
	tty_flip_buffer_push(tty);

 out:
	return IRQ_HANDLED;
}

static irqreturn_t wpcm450_serial_tx_chars(int irq, void *id, struct pt_regs *regs)
{
	struct wpcm450_uart_port *ourport = id;
	struct uart_port *port = &ourport->port;
	struct circ_buf *xmit = &port->info->xmit;
	struct wpcm450_uart_info *info = ourport->info;
	int count = info->fifosize;

	if (port->x_char) {
		wr_regb(port, WPCM450_COM_TX, port->x_char);
		port->icount.tx++;
		port->x_char = 0;
		goto out;
	}

	/* if there isnt anything more to transmit, or the uart is now
	 * stopped, disable the uart and exit
	*/

	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		wpcm450_serial_stop_tx(port);
		goto out;
	}

	/* try and drain the buffer... */

	while (!uart_circ_empty(xmit) && count-- > 0) {
		wr_regb(port, WPCM450_COM_TX, xmit->buf[xmit->tail]);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
	}
//printk("\nxmit->tail=%x, xmit->head=%x, xmit->buf = %x\n",xmit->tail,xmit->head,xmit->buf);
	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit))
		wpcm450_serial_stop_tx(port);

 out:
	return IRQ_HANDLED;
}

static unsigned int wpcm450_serial_tx_empty(struct uart_port *port)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);
	unsigned long lsrstat = rd_regl(port, WPCM450_COM_LSR);

	if( (lsrstat&UART_LSR_THRE)  == 0)
		return 0;
	else
		return 1;
}

/* no modem control lines */
static unsigned int wpcm450_serial_get_mctrl(struct uart_port *port)
{
	unsigned int umstat = rd_regb(port,WPCM450_COM_MSR);

#if 0//clyu
	if (umstat & WPCM450_UMSTAT_CTS)
		return TIOCM_CAR | TIOCM_DSR | TIOCM_CTS;
	else
		return TIOCM_CAR | TIOCM_DSR;
#endif
}

static void wpcm450_serial_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	/* todo - possibly remove AFC and do manual CTS */
}

static void wpcm450_serial_break_ctl(struct uart_port *port, int break_state)
{
	unsigned long flags;
	unsigned int ucon;

	spin_lock_irqsave(&port->lock, flags);

	ucon = rd_regl(port, WPCM450_COM_LCR);

	if (break_state)
		ucon |= UART_LCR_SBC;
	else
		ucon &= ~UART_LCR_SBC;

	wr_regl(port, WPCM450_COM_LCR, ucon);

	spin_unlock_irqrestore(&port->lock, flags);
}

static void wpcm450_serial_shutdown(struct uart_port *port)
{
	struct wpcm450_uart_port *ourport = to_ourport(port);
	
	if (ourport->tx_claimed || ourport->rx_claimed) 
		free_irq(RX_IRQ(port), ourport);
	
	if (ourport->tx_claimed) {	
		tx_disable(port);
		tx_enabled(port) = 0;
		ourport->tx_claimed = 0;
	}

	if (ourport->rx_claimed) {
		//printk("wpcm450_serial_shutdown\n");
		rx_disable(port);
		ourport->rx_claimed = 0;
		rx_enabled(port) = 0;
	}
}


static int wpcm450_serial_startup(struct uart_port *port)
{
	struct wpcm450_uart_port *ourport = to_ourport(port);
	int ret;

	//printk("wpcm450_serial_startup: mapbase=%p membase=%x irq=%d\n",   port->mapbase, port->membase, RX_IRQ(port));

	ret = request_irq(RX_IRQ(port),
			  wpcm450_serial_rx_chars, SA_INTERRUPT,
			  wpcm450_serial_portname(port), ourport);

	if (ret != 0) {
		printk(KERN_ERR "cannot get irq %d\n", RX_IRQ(port));
		return ret;
	}
	
	//printk("rx enable\n");
	rx_enable(port);
	rx_enabled(port) = 1;

	ourport->rx_claimed = 1;

	//printk("requesting tx irq...\n");
	
	//tx_enable(port);
	//tx_enabled(port) = 1;
	

	ourport->tx_claimed = 1;

	//printk("wpcm450_serial_startup ok\n");

	/* the port reset code should have done the correct
	 * register setup for the port controls */

	return ret;

 err:
	wpcm450_serial_shutdown(port);
	return ret;
}

/* power power management control */

static void wpcm450_serial_pm(struct uart_port *port, unsigned int level,
			      unsigned int old)
{
	struct wpcm450_uart_port *ourport = to_ourport(port);
#if 0
	switch (level) {
	case 3:
		if (!IS_ERR(ourport->baudclk) && ourport->baudclk != NULL)
			clk_disable(ourport->baudclk);

		clk_disable(ourport->clk);
		break;

	case 0:
		clk_enable(ourport->clk);

		if (!IS_ERR(ourport->baudclk) && ourport->baudclk != NULL)
			clk_enable(ourport->baudclk);

		break;
	default:
		printk(KERN_ERR "wpcm450_serial: unknown pm %d\n", level);
	}
#endif
}

/* baud rate calculation
 *
 * The UARTs on the WPCM450/S3C2440 can take their clocks from a number
 * of different sources, including the peripheral clock ("pclk") and an
 * external clock ("uclk"). The S3C2440 also adds the core clock ("fclk")
 * with a programmable extra divisor.
 *
 * The following code goes through the clock sources, and calculates the
 * baud clocks (and the resultant actual baud rates) and then tries to
 * pick the closest one and select that.
 *
*/


#define MAX_CLKS (8)

struct baud_calc {
	struct wpcm450_uart_clksrc	*clksrc;
	unsigned int			 calc;
	unsigned int			 quot;
	struct clk			*src;
};

static int wpcm450_serial_calcbaud(struct baud_calc *calc,
				   struct uart_port *port,
				   struct wpcm450_uart_clksrc *clksrc,
				   unsigned int baud)
{
	unsigned long rate = port->uartclk;
	
	
	
	calc->quot = rate / (baud * 16);

	calc->quot -= 2;
	
	return 1;
}

static unsigned int wpcm450_serial_getclk(struct uart_port *port,
					  struct wpcm450_uart_clksrc **clksrc,
					  struct clk **clk,
					  unsigned int baud)
{
	struct wpcm450_uartcfg *cfg = wpcm450_port_to_cfg(port);
	struct wpcm450_uart_clksrc *clkp;
	struct baud_calc res;
	int i;	
	
	clkp = cfg->clocks;
	wpcm450_serial_calcbaud(&res, port, clkp, baud);
	

	/* ok, we now need to select the best clock we found */

	printk(KERN_DEBUG "selected clock %p quot %d\n",
	       port->uartclk, res.quot);

	/* store results to pass back */

	*clksrc = res.clksrc;
	*clk    = res.src;

	return res.quot;
}

static void wpcm450_serial_set_termios(struct uart_port *port,
				       struct termios *termios,
				       struct termios *old)
{
	struct wpcm450_uartcfg *cfg = wpcm450_port_to_cfg(port);
	struct wpcm450_uart_port *ourport = to_ourport(port);
	struct wpcm450_uart_clksrc *clksrc = NULL;
	struct clk *clk = NULL;
	unsigned long flags;
	unsigned int baud, quot;
	unsigned int ulcon;
	unsigned int umcon;

	/*
	 * We don't support modem control lines.
	 */
	termios->c_cflag &= ~(HUPCL | CMSPAR);
	termios->c_cflag |= CLOCAL;

	/*
	 * Ask the core to calculate the divisor for us.
	 */

	baud = uart_get_baud_rate(port, termios, old, 0, 115200*8);
//printk("wpcm450_serial_set_termios baud = %d\n",baud);
	if (baud == 38400 && (port->flags & UPF_SPD_MASK) == UPF_SPD_CUST)
		quot = port->custom_divisor;
	else
	  quot = 11;//7;//wpcm450_serial_getclk(port, &clksrc, &clk, baud);

	/* check to see if we need  to change clock source */
#if 0
	if (ourport->clksrc != clksrc || ourport->baudclk != clk) {
		wpcm450_serial_setsource(port, clksrc);

		if (ourport->baudclk != NULL && !IS_ERR(ourport->baudclk)) {
			clk_disable(ourport->baudclk);
			ourport->baudclk  = NULL;
		}

		clk_enable(clk);

		ourport->clksrc = clksrc;
		ourport->baudclk = clk;
	}
#endif
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		dbg("config: 5bits/char\n");
		ulcon = UART_LCR_WLEN5;
		break;
	case CS6:
		dbg("config: 6bits/char\n");
		ulcon = UART_LCR_WLEN6;
		break;
	case CS7:
		dbg("config: 7bits/char\n");
		ulcon = UART_LCR_WLEN7;
		break;
	case CS8:
	default:
		dbg("config: 8bits/char\n");
		ulcon = UART_LCR_WLEN8;
		break;
	}

	if (termios->c_cflag & CSTOPB)
		ulcon |= UART_LCR_NSB;

	if (termios->c_cflag & PARENB) {
		ulcon |= UART_LCR_PARITY;
		if (termios->c_cflag & PARODD)
			ulcon |= UART_LCR_OPAR;
		else
			ulcon |= UART_LCR_EPAR;
	} else {
		ulcon |= UART_LCR_NPAR;
	}

	spin_lock_irqsave(&port->lock, flags);

	wr_regl (port, WPCM450_COM_LCR, 0x80); /* select divisor latch registers */    
    wr_regl (port, WPCM450_COM_LSB, quot&0x00FF);	  /* Baud rate = 9600   : 15,000,000/(16*(96+2)) */
    wr_regl (port, WPCM450_COM_MSB, quot&0xFF00);
	wr_regl(port, WPCM450_COM_LCR, ulcon);
	
	wr_regl (port, WPCM450_COM_MCR, 0x00);
	
    //wr_regl (port, WPCM450_COM_TOR, 0x80+0x20); /* Timeout if more than ??? bits xfer time */
    //wr_regl (port, WPCM450_COM_FCR, 0x8F); /* 8-byte FIFO trigger level, reset Tx and Rx FIFO */


	//printk("uart: quot = 0x%08x\n",   quot);

	/*
	 * Update the per-port timeout.
	 */
	uart_update_timeout(port, termios->c_cflag, baud);

	/*
	 * Which character status flags are we interested in?
	 */
	port->read_status_mask = UART_LSR_OE;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= UART_LSR_FE | UART_LSR_PE;

	/*
	 * Which character status flags should we ignore?
	 */
	port->ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= UART_LSR_OE;
	if (termios->c_iflag & IGNBRK && termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= UART_LSR_FE;

	/*
	 * Ignore all characters if CREAD is not set.
	 */
	if ((termios->c_cflag & CREAD) == 0)
		port->ignore_status_mask |= RXSTAT_DUMMY_READ;

	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *wpcm450_serial_type(struct uart_port *port)
{
	switch (port->type) {
	case PORT_WPCM450:
		return "WPCM450";
	default:
		return NULL;
	}
}

#define MAP_SIZE (0x100)

static void wpcm450_serial_release_port(struct uart_port *port)
{
	release_mem_region(port->mapbase, MAP_SIZE);
}

static int wpcm450_serial_request_port(struct uart_port *port)
{
	const char *name = wpcm450_serial_portname(port);
	return request_mem_region(port->mapbase, MAP_SIZE, name) ? 0 : -EBUSY;
}

static void wpcm450_serial_config_port(struct uart_port *port, int flags)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);

	if (flags & UART_CONFIG_TYPE &&
	    wpcm450_serial_request_port(port) == 0)
		port->type = info->type;
}

/*
 * verify the new serial_struct (for TIOCSSERIAL).
 */
static int
wpcm450_serial_verify_port(struct uart_port *port, struct serial_struct *ser)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);

	if (ser->type != PORT_UNKNOWN && ser->type != info->type)
		return -EINVAL;

	return 0;
}


#ifdef CONFIG_SERIAL_WPCM450_CONSOLE

static struct console wpcm450_serial_console;

#define WPCM450_SERIAL_CONSOLE &wpcm450_serial_console
#else
#define WPCM450_SERIAL_CONSOLE NULL
#endif

static struct uart_ops wpcm450_serial_ops = {
	.pm		= wpcm450_serial_pm,
	.tx_empty	= wpcm450_serial_tx_empty,
	.get_mctrl	= wpcm450_serial_get_mctrl,
	.set_mctrl	= wpcm450_serial_set_mctrl,
	.stop_tx	= wpcm450_serial_stop_tx,
	.start_tx	= wpcm450_serial_start_tx,
	.stop_rx	= wpcm450_serial_stop_rx,
	.enable_ms	= wpcm450_serial_enable_ms,
	.break_ctl	= wpcm450_serial_break_ctl,
	.startup	= wpcm450_serial_startup,
	.shutdown	= wpcm450_serial_shutdown,
	.set_termios	= wpcm450_serial_set_termios,
	.type		= wpcm450_serial_type,
	.release_port	= wpcm450_serial_release_port,
	.request_port	= wpcm450_serial_request_port,
	.config_port	= wpcm450_serial_config_port,
	.verify_port	= wpcm450_serial_verify_port,
};


static struct uart_driver wpcm450_uart_drv = {
	.owner		= THIS_MODULE,
	.dev_name	= "wpcm450_serial",
	.nr		= NR_PORTS,
	.cons		= WPCM450_SERIAL_CONSOLE,
	.driver_name	= WPCM450_SERIAL_NAME,
	.devfs_name	= WPCM450_SERIAL_DEVFS,
	.major		= WPCM450_SERIAL_MAJOR,
	.minor		= WPCM450_SERIAL_MINOR,
};

static struct wpcm450_uart_port wpcm450_serial_ports[NR_PORTS] = {
	[0] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART0,
			.uartclk	= 24000000,
			.fifosize	= 14,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_SPD_VHI | UPF_BOOT_AUTOCONF,
			.line		= 0,
		}
	},
	[1] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.irq		= IRQ_UART1,
			.uartclk	= 24000000,
			.fifosize	= 16,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 1,
		}
	},
#if NR_PORTS > 2

	[2] = {
		.port = {
			.lock		= SPIN_LOCK_UNLOCKED,
			.iotype		= UPIO_MEM,
			.irq		= IRQ_S3CUART_RX2,
			.uartclk	= 0,
			.fifosize	= 16,
			.ops		= &wpcm450_serial_ops,
			.flags		= UPF_BOOT_AUTOCONF,
			.line		= 2,
		}
	}
#endif
};

/* _wpcm450_serial_resetport
 *
 * wrapper to call the specific reset for this port (reset the fifos
 * and the settings)
*/

static inline int _wpcm450_serial_resetport(struct uart_port * port,
					   struct wpcm450_uartcfg *cfg)
{
	struct wpcm450_uart_info *info = wpcm450_port_to_info(port);

	return (info->reset_port)(port, cfg);
}

/* wpcm450_serial_init_port
 *
 * initialise a single serial port from the platform device given
 */

static int wpcm450_serial_init_port(struct wpcm450_uart_port *ourport,
				    struct wpcm450_uart_info *info,
				    struct platform_device *platdev)
{
	struct uart_port *port = &ourport->port;
	struct wpcm450_uartcfg *cfg;
	struct resource *res;

	dbg("wpcm450_serial_init_port: port=%p, platdev=%p\n", port, platdev);

	if (platdev == NULL)
		return -ENODEV;
		
	cfg = wpcm450_dev_to_cfg(&platdev->dev);

	if (port->mapbase != 0)
		return 0;

	/* setup info for port */
	port->dev	= &platdev->dev;
	ourport->info	= info;

	/* copy the info in from provided structure */
	ourport->port.fifosize = info->fifosize;

	dbg("wpcm450_serial_init_port: %p (hw %d)...\n", port, cfg->hwport);

	port->uartclk = 24000000;//clyu
	
	if (cfg->uart_flags & UPF_CONS_FLOW) {
		dbg("s3c24xx_serial_init_port: enabling flow control\n");
		port->flags |= UPF_CONS_FLOW;
	}
	
	/* sort our the physical and virtual addresses for each UART */

	res = platform_get_resource(platdev, IORESOURCE_MEM, 0);
	if (res == NULL) {
		printk(KERN_ERR "failed to find memory resource for uart\n");
		return -EINVAL;
	}

	dbg("resource %p (%lx..%lx)\n", res, res->start, res->end);

	port->mapbase	= res->start;
	port->membase	= WPCM450_VA_UART + (res->start - WPCM450_PA_UART);
	port->irq	= platform_get_irq(platdev, 0);
	if (port->irq < 0)
		port->irq = 0;

	ourport->clk	= clk_get(&platdev->dev, "uart");

	dbg("port: map=%08x, mem=%08x, irq=%d, clock=%ld\n",
	    port->mapbase, port->membase, port->irq, port->uartclk);

	/* reset the fifos (and setup the uart) */
	_wpcm450_serial_resetport(port, cfg);
	return 0;
}

/* Device driver serial port probe */

static int probe_index = 0;

static int _wpcm450_serial_probe(struct platform_device *dev,
				struct wpcm450_uart_info *info)
{
	struct wpcm450_uart_port *ourport;
	int ret;

	dbg("_wpcm450_serial_probe(%p, %p) %d\n", dev, info, probe_index);

	ourport = &wpcm450_serial_ports[probe_index];
	probe_index++;

	dbg("%s: initialising port %p...\n", __FUNCTION__, ourport);

	ret = wpcm450_serial_init_port(ourport, info, dev);
	if (ret < 0)
		goto probe_err;

	dbg("%s: adding port\n", __FUNCTION__);
	uart_add_one_port(&wpcm450_uart_drv, &ourport->port);
	platform_set_drvdata(dev, &ourport->port);

	return 0;

 probe_err:
	return ret;
}

static int wpcm450_serial_remove(struct platform_device *dev)
{
	struct uart_port *port = wpcm450_dev_to_port(&dev->dev);

	if (port)
		uart_remove_one_port(&wpcm450_uart_drv, port);

	return 0;
}

/* UART power management code */

#ifdef CONFIG_PM

static int wpcm450_serial_suspend(struct platform_device *dev, pm_message_t state)
{
	struct uart_port *port = wpcm450_dev_to_port(&dev->dev);

	if (port)
		uart_suspend_port(&wpcm450_uart_drv, port);

	return 0;
}

static int wpcm450_serial_resume(struct platform_device *dev)
{
	struct uart_port *port = wpcm450_dev_to_port(&dev->dev);
	struct wpcm450_uart_port *ourport = to_ourport(port);

	if (port) {
		clk_enable(ourport->clk);
		_wpcm450_serial_resetport(port, wpcm450_port_to_cfg(port));
		clk_disable(ourport->clk);

		uart_resume_port(&wpcm450_uart_drv, port);
	}

	return 0;
}

#else
#define wpcm450_serial_suspend NULL
#define wpcm450_serial_resume  NULL
#endif

static int _wpcm450_serial_init(struct platform_driver *drv,
			       struct wpcm450_uart_info *info)
{
	dbg("_wpcm450_serial_init(%p,%p)\n", drv, info);
	return platform_driver_register(drv);
}


/* now comes the code to initialise either the wpcm450 serial
 * port information
*/

static int wpcm450_serial_resetport(struct uart_port *port,
				    struct wpcm450_uartcfg *cfg)
{
	dbg("wpcm450_serial_resetport: port=%p (%08lx), cfg=%p\n",
	    port, port->mapbase, cfg);
	
	/* reset both fifos */
	if(port->line == 0)
		wr_regl (port, WPCM450_COM_FCR, 0x0F | UARTx_FCR_FIFO_LEVEL14); /* reset Tx and Rx FIFO */

	return 0;
}

static struct wpcm450_uart_info wpcm450_uart_inf = {
	.name		= "Winbond WPCM450 UART",
	.type		= PORT_WPCM450,
	.fifosize	= 16,
	.reset_port	= wpcm450_serial_resetport,
};

/* device management */

static int wpcm450_serial_probe(struct platform_device *dev)
{
	return _wpcm450_serial_probe(dev, &wpcm450_uart_inf);
}

static struct platform_driver wpcm450_serial_drv = {
	.probe		= wpcm450_serial_probe,
	.remove		= wpcm450_serial_remove,
	.suspend	= wpcm450_serial_suspend,
	.resume		= wpcm450_serial_resume,
	.driver		= {
		.name	= "wpcm450-uart",
		.owner	= THIS_MODULE,
	},
};

static inline int wpcm450_serial_init(void)
{
	return _wpcm450_serial_init(&wpcm450_serial_drv, &wpcm450_uart_inf);
}

static inline void wpcm450_serial_exit(void)
{
	platform_driver_unregister(&wpcm450_serial_drv);
}

#define wpcm450_uart_inf_at &wpcm450_uart_inf


/* module initialisation code */

static int __init wpcm450_serial_modinit(void)
{
	int ret;

	ret = uart_register_driver(&wpcm450_uart_drv);
	if (ret < 0) {
		printk(KERN_ERR "failed to register UART driver\n");
		return -1;
	}

	wpcm450_serial_init();

	return 0;
}

static void __exit wpcm450_serial_modexit(void)
{
	wpcm450_serial_exit();

	uart_unregister_driver(&wpcm450_uart_drv);
}


module_init(wpcm450_serial_modinit);
module_exit(wpcm450_serial_modexit);

/* Console code */

#ifdef CONFIG_SERIAL_WPCM450_CONSOLE

static struct uart_port *cons_uart;

static int
wpcm450_serial_console_txrdy(struct uart_port *port, unsigned int ufcon)
{
	unsigned long lsrstat;

	lsrstat = rd_regl(port, WPCM450_COM_LSR);
	return (lsrstat & UART_LSR_THRE) ? 1 : 0;
}

static void
wpcm450_serial_console_putchar(struct uart_port *port, int ch)
{
#if 1//clyu for emulate
	while (!wpcm450_serial_console_txrdy(port, 0))
		barrier();
#endif
	wr_regb(cons_uart, WPCM450_COM_TX, ch);
}

static void
wpcm450_serial_console_write(struct console *co, const char *s,
			     unsigned int count)
{
	uart_console_write(cons_uart, s, count, wpcm450_serial_console_putchar);
}

static void __init
wpcm450_serial_get_options(struct uart_port *port, int *baud,
			   int *parity, int *bits)
{
	struct wpcm450_uart_clksrc clksrc;
	struct clk *clk;
	unsigned int lcrcon,div;
	unsigned long rate;

	lcrcon  = UART_LCR_WLEN8 | UART_LCR_NPAR;
	*baud = 115200;

		/* consider the serial port configured if the tx/rx mode set */

		switch (lcrcon & UART_LCR_CSMASK) {
		case UART_LCR_WLEN5:
			*bits = 5;
			break;
		case UART_LCR_WLEN6:
			*bits = 6;
			break;
		case UART_LCR_WLEN7:
			*bits = 7;
			break;
		default:
		case UART_LCR_WLEN8:
			*bits = 8;
			break;
		}
		
		if(lcrcon & UART_LCR_PARITY)
		{
			switch (lcrcon & UART_LCR_PMMASK) {
			case UART_LCR_EPAR:
				*parity = 'e';
				break;
			case UART_LCR_OPAR:
				*parity = 'o';
				break;
			//case UART_LCR_NPAR:
			default:
				*parity = 'n';
			}
		}
		else
			*parity = 'n';

		/* now calculate the baud rate */
		
		//rate = port->uartclk;
		//div = rd_regl(port, WPCM450_COM_MSB)<<8 | rd_regl(port, WPCM450_COM_LSB);

		//*baud = rate / ( 16 * (div + 2));
		dbg("calculated baud %d\n", *baud);
	

}

/* wpcm450_serial_init_ports
 *
 * initialise the serial ports from the machine provided initialisation
 * data.
*/

static int wpcm450_serial_init_ports(struct wpcm450_uart_info *info)
{
	struct wpcm450_uart_port *ptr = wpcm450_serial_ports;
	struct platform_device **platdev_ptr;
	int i;

	dbg("wpcm450_serial_init_ports: initialising ports...\n");

	platdev_ptr = wpcm450_uart_devs;

	for (i = 0; i < NR_PORTS; i++, ptr++, platdev_ptr++) {
		wpcm450_serial_init_port(ptr, info, *platdev_ptr);
	}

	return 0;
}

static int __init
wpcm450_serial_console_setup(struct console *co, char *options)
{
	struct uart_port *port;
	int baud = 9600;
	int bits = 8;
	int parity = 'n';
	int flow = 'n';

	//printk("wpcm450_serial_console_setup: co=%p (%d), %s\n", co, co->index, options);

	/* is this a valid port */

	if (co->index == -1 || co->index >= NR_PORTS)
		co->index = 0;

	port = &wpcm450_serial_ports[co->index].port;

	/* is the port configured? */

	if (port->mapbase == 0x0) {
		co->index = 0;
		port = &wpcm450_serial_ports[co->index].port;
	}

	cons_uart = port;

	dbg("wpcm450_serial_console_setup: port=%p (%d)\n", port, co->index);

	/*
	 * Check whether an invalid uart number has been specified, and
	 * if so, search for the first available port that does have
	 * console support.
	 */
	if (options)
		uart_parse_options(options, &baud, &parity, &bits, &flow);
	else
		wpcm450_serial_get_options(port, &baud, &parity, &bits);

	dbg("wpcm450_serial_console_setup: baud %d\n", baud);

	return uart_set_options(port, co, baud, parity, bits, flow);
}

/* wpcm450_serial_initconsole
 *
 * initialise the console from one of the uart drivers
*/

static struct console wpcm450_serial_console =
{
	.name		= WPCM450_SERIAL_NAME,
	.device		= uart_console_device,
	.flags		= CON_PRINTBUFFER,
	.index		= -1,
	.write		= wpcm450_serial_console_write,
	.setup		= wpcm450_serial_console_setup
};

static int wpcm450_serial_initconsole(void)
{
	struct wpcm450_uart_info *info;
	struct platform_device *dev = wpcm450_uart_devs[0];

	dbg("wpcm450_serial_initconsole\n");

	/* select driver based on the cpu */

	if (dev == NULL) {
		printk(KERN_ERR "wpcm450: no devices for console init\n");
		return 0;
	}

	if (strcmp(dev->name, "wpcm450-uart") == 0) {
		info = wpcm450_uart_inf_at;
	} else {
		printk(KERN_ERR "wpcm450: no driver for %s\n", dev->name);
		return 0;
	}

	if (info == NULL) {
		printk(KERN_ERR "wpcm450: no driver for console\n");
		return 0;
	}

	wpcm450_serial_console.data = &wpcm450_uart_drv;
	wpcm450_serial_init_ports(info);

	register_console(&wpcm450_serial_console);
	return 0;
}

console_initcall(wpcm450_serial_initconsole);

#endif /* CONFIG_SERIAL_WPCM450_CONSOLE */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ben Dooks <ben@simtec.co.uk>");
MODULE_DESCRIPTION("Samsung WPCM450/S3C2440 Serial port driver");
