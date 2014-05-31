/* linux/drivers/spi/spi_wpcm450.c
 *
 * Copyright (c) 2006 Ben Dooks
 * Copyright (c) 2006 Simtec Electronics
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
*/


#define DEBUG

#include <linux/config.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/platform_device.h>

#include <linux/spi/spi.h>
#include <linux/spi/spi_bitbang.h>
#include <linux/delay.h>

#include <asm/io.h>
#include <asm/dma.h>
#include <asm/hardware.h>

//#include <asm/arch/regs-gpio.h>
//#include <asm/arch/regs-spi.h>
//#include <asm/arch/spi.h>


#define MAX_TXBUF_LEN 5
#define MAX_RXBUF_LEN 4

void wpcm450_spi_readID(struct spi_device *spi);

struct wpcm450_spi {
	/* bitbang has to be first */
	struct spi_bitbang	 bitbang;
	struct completion	 done;

	void __iomem		*regs;
	int			 irq;
	int			 len;
	int			 count;
	unsigned char *		 FIU_BA;

	/* data buffers */
	const unsigned char	*tx;
	unsigned char		*rx;
	

	struct clk		*clk;
	struct resource		*ioarea;
	struct spi_master	*master;
	struct spi_device	*curdev;
	struct device		*dev;
	struct wpcm450_spi_info *pdata;
};


static inline struct wpcm450_spi * to_hw(struct spi_device *sdev)
{
	return spi_master_get_devdata(sdev->master);
}


static void wpcm450_spi_hw_init(struct spi_device *spi) {

	//printk(KERN_INFO "%s\n",__FUNCTION__);

	struct wpcm450_spi *hw = to_hw(spi);
	
	unsigned char *GLO_CTRL_addr,*FLASH_BASE_addr;		

	GLO_CTRL_addr = (unsigned char *)(u32)ioremap_nocache((unsigned long)0xB0000000, 0x125);

	hw->FIU_BA = (unsigned char *)(u32)ioremap_nocache((unsigned long)0xC8000000, 0x200);

	*((unsigned char *) GLO_CTRL_addr+0x00C) |= 0 & ( (1 << 3) | (1 << 4) | (0 << 5)  );


	//PROT_CLEAR : UnLock  CFBB_PROT and PROT_LOCK
	*((unsigned char *) hw->FIU_BA + 0x11) = 0x87;
	udelay(10);
	*((unsigned char *) hw->FIU_BA + 0x11) = 0x61;  
	udelay(10);
	*((unsigned char *) hw->FIU_BA + 0x11) = 0x63; 
	udelay(10);
	
	//FIU_CFG : Set FL_Size to be Maximum
	*((unsigned char *) hw->FIU_BA) = 0x80; 


	//BURST_CFG : Set W_BURST/R_BURST to be Maximum
	*((unsigned char *) (hw->FIU_BA + 0x1 )) =   (0x2 << 4) | 0x2; 


	//RESP_CFG : Disable INT_EN and IAD_EN
	*((unsigned char *) (hw->FIU_BA + 0x2 )) = 0x0; 


	//CFBB_PROT : Disable Fireware Boot block protect
	*((unsigned char *) (hw->FIU_BA + 0x3 )) = 0xff; 


	//PROT_LOCK : Clear all Flash Access Window Lock 
	*((unsigned char *) (hw->FIU_BA + 0x10 )) = 0x0; 	

	//SPI_FL_CFG : Set Normal Read and DEV_SIZE to be Maximum (16MB)
	*((unsigned char *) (hw->FIU_BA + 0x14 )) = (0x0 << 6)| 0x20; 	

	//SPI_TIM : Set clock frequcncy
	*((unsigned char *) (hw->FIU_BA + 0x15 )) = 0x0B; 	

	//UMA_ECTS
	*((unsigned char *) (hw->FIU_BA + 0x1f )) = 0xf;

	while(1) 
	{
 		wpcm450_spi_readID(spi);
	}
	 	
} 


static int wpcm450_spi_read(struct spi_device *spi,unsigned char *Rx_buf, int len) {

	unsigned char status;

	struct wpcm450_spi *hw = to_hw(spi);

        int mcount = len / MAX_RXBUF_LEN  ;
	int qcount = len - (mcount * MAX_RXBUF_LEN);
	unsigned char *buf = Rx_buf;
	    		
	    while((mcount > 0) || (qcount > 0))
	    {
	        if(mcount > 0)
	        {	 			

			//UMA_CTS : Set Read setting
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (MAX_RXBUF_LEN) ; 

			//UMA_CTS : Triggle Read Action
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (MAX_RXBUF_LEN) ; 	


			while(1) 
			{
				status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
				status = 0x1 & (status >> 7);
				if (status == 0) {
					break;
				}
			}

	  		buf[0] = *((unsigned char *) (hw->FIU_BA + 0x1a ));
			buf[1] = *((unsigned char *) (hw->FIU_BA + 0x1b ));
			buf[2] = *((unsigned char *) (hw->FIU_BA + 0x1c ));
			buf[3] = *((unsigned char *) (hw->FIU_BA + 0x1d ));	
		}
	        else
	        {
			//UMA_CTS : Set Read setting
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (qcount) ; 				

			//UMA_CTS : Triggle Read Action
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (qcount) ; 	


			while(1) 
			{
				status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
				status = 0x1 & (status >> 7);
				if (status == 0) {
					break;
				}
			}

			switch (qcount) {
				case 3:
					buf[2] = *((unsigned char *) (hw->FIU_BA + 0x1c)); 		 			
				case 2:
		  			buf[1] = *((unsigned char *) (hw->FIU_BA + 0x1b));
				case 1:
					buf[0] = *((unsigned char *) (hw->FIU_BA + 0x1a));
			}

	                qcount = 0;
	
	        }
	        
	        buf += MAX_RXBUF_LEN ;
	        mcount--;
	    }

        return buf;

}

static int wpcm450_spi_write(struct spi_device *spi, unsigned char *Tx_buf, int len) {

	unsigned char status;
	struct wpcm450_spi *hw = to_hw(spi);

        int mcount = len / MAX_TXBUF_LEN ;
	int qcount = len - (mcount * MAX_TXBUF_LEN );
	unsigned char *buf = Tx_buf;

	    		
	    while((mcount > 0) || (qcount > 0))
	    {
	        if(mcount > 0)
	        {
		    	*((unsigned char *) (hw->FIU_BA + 0x16 )) = buf[0]; 		 			
	  		*((unsigned char *) (hw->FIU_BA + 0x1a )) = buf[1];
			*((unsigned char *) (hw->FIU_BA + 0x1b )) = buf[2];
			*((unsigned char *) (hw->FIU_BA + 0x1c )) = buf[3];
			*((unsigned char *) (hw->FIU_BA + 0x1d )) = buf[4];
			//UMA_CTS : Set Write setting
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (MAX_TXBUF_LEN -1) ; 		

			//UMA_CTS : Triggle Write Action
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (MAX_TXBUF_LEN -1) ; 		
		}
	        else
	        {
			switch (qcount) {
				case 4:
				*((unsigned char *) (hw->FIU_BA + 0x1c)) = buf[3]; 		 			
				case 3:
		  		*((unsigned char *) (hw->FIU_BA + 0x1b)) = buf[2];
				case 2:
				*((unsigned char *) (hw->FIU_BA + 0x1a)) = buf[1];
				case 1:
				*((unsigned char *) (hw->FIU_BA + 0x16)) = buf[0];
			}
			//UMA_CTS : Set Write setting
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (qcount-1) ; 		

			//UMA_CTS : Triggle Write Action
			*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (qcount-1) ; 	
	                qcount = 0;
	        }

		while(1) 
		{
			status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
			status = 0x1 & (status >> 7);
			if (status == 0) {
				break;
			}
		}
	        
	        buf += MAX_TXBUF_LEN ;
	        mcount--;
	    }

	return len;

}

static unsigned char wpcm450_spi_readb(struct spi_device *spi) {

	unsigned char status;

	unsigned char buf;

	struct wpcm450_spi *hw = to_hw(spi);

	*((unsigned char *) (hw->FIU_BA + 0x16 )) = 0x0; 		

	//UMA_CTS : Set Read setting
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (0) ; 				

	//UMA_CTS : Triggle Read Action
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (0) ; 				

	while(1) {
		status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
		status = 0x1 & (status >> 7);
		if (status == 0) {
			break;
		}
	}

	//UMA_DB0 : Get data from 
	buf = 0xff & ( *((unsigned char *) (hw->FIU_BA + 0x1a )) ); 		 		
	//printk(KERN_INFO "%s data: %x\n",__FUNCTION__,buf);

        return buf;

}

static void wpcm450_spi_writeb(struct spi_device *spi, unsigned char Tx_buf) {

	unsigned char status;

	struct wpcm450_spi *hw = to_hw(spi);

	//UMA_DB0 : Put data to
  	*((unsigned char *) (hw->FIU_BA + 0x16 )) = Tx_buf; 		 			
	 		 			

	//UMA_CTS : Set Write setting
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		


	while(1) {
		status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
		status = 0x1 & (status >> 7);
		if (status == 0) {
			break;
		}
	}

}

static void wpcm450_fit_write_read(struct spi_device *spi, unsigned char *Tx_buf, int tx_len, unsigned char *Rx_buf, int rx_len) 
{

	unsigned char status;

	struct wpcm450_spi *hw = to_hw(spi);

	//UMA_DB0 : Put data to
	switch (tx_len) {
	case 5:
		*((unsigned char *) (hw->FIU_BA + 0x1d )) = Tx_buf[4];
	case 4:
		*((unsigned char *) (hw->FIU_BA + 0x1c )) = Tx_buf[3];
	case 3:
		*((unsigned char *) (hw->FIU_BA + 0x1b )) = Tx_buf[2];
	case 2:
		*((unsigned char *) (hw->FIU_BA + 0x1a )) = Tx_buf[1];
	case 1:
    		*((unsigned char *) (hw->FIU_BA + 0x16 )) = Tx_buf[0]; 		 			
	}

	//UMA_CTS : Set Write setting
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (tx_len) ; 		

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (tx_len) ; 		

	while(1) {
		status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
		status = 0x1 & (status >> 7);
		if (status == 0) {
			break;
		}
	}

	switch (rx_len) {
	case 4:
		Rx_buf[3] = *((unsigned char *) (hw->FIU_BA + 0x1d)); 		 			
	case 3:
		Rx_buf[2] = *((unsigned char *) (hw->FIU_BA + 0x1c)); 		 			
	case 2:
		Rx_buf[1] = *((unsigned char *) (hw->FIU_BA + 0x1b));
	case 1:
		Rx_buf[0] = *((unsigned char *) (hw->FIU_BA + 0x1a));
	}


}

static void wpcm450_spi_readID(struct spi_device *spi) {
#if 0
	wpcm450_spi_writeb(spi,0x05);
	printk(KERN_INFO "wpcm450_spi_readID:%x\n",wpcm450_spi_readb(spi));
	udelay(1000);	

	unsigned char tx_buf[4] = {0x9f};
	unsigned char rx_buf[4] = {0,0,0,0};

	wpcm450_fit_write_read(spi,tx_buf,1,rx_buf,4);
	printk(KERN_INFO "read from_1 :%x\n",rx_buf[0]);
	printk(KERN_INFO "read from_2 :%x\n",rx_buf[1]);
	printk(KERN_INFO "read from_3 :%x\n",rx_buf[2]);
	printk(KERN_INFO "read from_4 :%d\n",rx_buf[3]);
#endif	

	

	struct wpcm450_spi *hw = to_hw(spi);
	unsigned char status;
	unsigned char buf[4];
	int count = 0;

	*((unsigned char *) (hw->FIU_BA + 0x16 )) = 0x03;
	
	*((unsigned char *) (hw->FIU_BA + 0x19 )) = 0 >> 16;
	*((unsigned char *) (hw->FIU_BA + 0x18 )) = 0 >> 8;
	*((unsigned char *) (hw->FIU_BA + 0x17 )) = 0;


	//UMA_CTS : Set Write setting
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (1 << 3) | (4) ; 		

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (hw->FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (1 << 3) | (4) ; 

	while(count < 256) {
		while(1) {
			status = *((unsigned char *) (hw->FIU_BA + 0x1e ));
			status = 0x1 & (status >> 7);
			if (status == 0) {
				break;
			}
		}

		buf[0] = *((unsigned char *) (hw->FIU_BA + 0x1a ));
		buf[1] = *((unsigned char *) (hw->FIU_BA + 0x1b ));
		buf[2] = *((unsigned char *) (hw->FIU_BA + 0x1c ));
		buf[3] = *((unsigned char *) (hw->FIU_BA + 0x1d ));

		count += 4;
	}

	
	//printk(KERN_INFO "%s count:%d\n",__FUNCTION__,count);
}

static void wpcm450_spi_chipeset(struct spi_device *spi, unsigned int chipsel) {
	
	struct wpcm450_spi *hw = to_hw(spi);

	//UMA_ECTS : Set Normal Read and DEV_SIZE to be Maximum (16MB)
	*((unsigned char *) (hw->FIU_BA + 0x1f )) |= 1 << chipsel; 		
}


static void wpcm450_spi_chipsel(struct spi_device *spi, int value)
{
	struct wpcm450_spi *hw = to_hw(spi);
	unsigned int cspol = spi->mode & SPI_CS_HIGH ? 1 : 0;
	unsigned int spcon;

	switch (value) {
	case BITBANG_CS_INACTIVE:
		break;

	case BITBANG_CS_ACTIVE:
		break;

	}
}

static int wpcm450_spi_setupxfer(struct spi_device *spi,
				 struct spi_transfer *t)
{
	struct wpcm450_spi *hw = to_hw(spi);
	unsigned int bpw;
	unsigned int hz;
	unsigned int div;

	bpw = t ? t->bits_per_word : spi->bits_per_word;
	hz  = t ? t->speed_hz : spi->max_speed_hz;

	if (bpw != 8) {
		dev_err(&spi->dev, "invalid bits-per-word (%d)\n", bpw);
		return -EINVAL;
	}


	//div = clk_get_rate(hw->clk) / hz;

	/* is clk = pclk / (2 * (pre+1)), or is it
	 *    clk = (pclk * 2) / ( pre + 1) */

	div = (div / 2) - 1;

	if (div < 0)
		div = 1;

	if (div > 255)
		div = 255;

	dev_dbg(&spi->dev, "setting pre-scaler to %d (hz %d)\n", div, hz);



	spin_lock(&hw->bitbang.lock);

	if (!hw->bitbang.busy) {

		//hw->bitbang.chipselect(spi, BITBANG_CS_INACTIVE);
		/* need to ndelay for 0.5 clocktick ? */

	}
	spin_unlock(&hw->bitbang.lock);


	return 0;
}

static int wpcm450_spi_setup(struct spi_device *spi)
{
	int ret;
	
	//printk(KERN_INFO "%s\n",__FUNCTION__);

	if (!spi->bits_per_word)
		spi->bits_per_word = 8;

	if ((spi->mode & SPI_LSB_FIRST) != 0)
		return -EINVAL;


	ret = wpcm450_spi_setupxfer(spi, NULL);
	if (ret < 0) {
		dev_err(&spi->dev, "setupxfer returned %d\n", ret);
		return ret;
	}

	dev_dbg(&spi->dev, "%s: mode %d, %u bpw, %d hz\n",
		__FUNCTION__, spi->mode, spi->bits_per_word,
		spi->max_speed_hz);

	return 0;
}

static inline unsigned int hw_txbyte(struct wpcm450_spi *hw, int count)
{
	return hw->tx ? hw->tx[count] : 0xff;
}

static int wpcm450_spi_txrx(struct spi_device *spi, struct spi_transfer *t)
{
	struct wpcm450_spi *hw = to_hw(spi);

	//printk(KERN_INFO "%s\n",__FUNCTION__);

	dev_dbg(&spi->dev, "txrx: tx %p, rx %p, len %d\n",
		t->tx_buf, t->rx_buf, t->len);

	hw->tx = t->tx_buf;
	hw->rx = t->rx_buf;
	hw->len = t->len;


	while(hw->count < hw->len) {

	if (hw->rx) {
		hw->rx[hw->count] =  wpcm450_spi_readb(spi);
		//hw->count = wpcm450_spi_read(spi , hw->rx , hw->len);
	}

	if (hw->tx) {
		wpcm450_spi_writeb(spi, hw_txbyte(hw, hw->count));
		//hw->count = wpcm450_spi_write(spi, hw->tx , hw->len);
	}

	hw->count++;	

	}


	return hw->count;
}


struct spi_board_info Gbi;

static int wpcm450_spi_probe(struct platform_device *pdev)
{
	struct wpcm450_spi *hw;
	struct spi_master *master;
	struct spi_board_info *bi;
	struct resource *res;
	int err = 0;
	int i;

	//printk(KERN_INFO "wpcm450_spi_probe\n");

	master = spi_alloc_master(&pdev->dev, sizeof(struct wpcm450_spi));
	if (master == NULL) {
		dev_err(&pdev->dev, "No memory for spi_master\n");
		err = -ENOMEM;
		goto err_nomem;
	}

	hw = spi_master_get_devdata(master);
	memset(hw, 0, sizeof(struct wpcm450_spi));

	hw->master = spi_master_get(master);
	hw->pdata = pdev->dev.platform_data;
	hw->dev = &pdev->dev;

	if (hw->pdata == NULL) {
		dev_err(&pdev->dev, "No platform data supplied\n");
		err = -ENOENT;
		goto err_no_pdata;
	}
	//printk(KERN_INFO "master->bus_num:%d\n",master->bus_num);

	platform_set_drvdata(pdev, hw);
	init_completion(&hw->done);

	/* setup the state for the bitbang driver */

	hw->bitbang.master         = hw->master;
	hw->bitbang.setup_transfer = wpcm450_spi_setupxfer;
	hw->bitbang.chipselect     = wpcm450_spi_chipsel;
	hw->bitbang.txrx_bufs      = wpcm450_spi_txrx;
	hw->bitbang.master->setup  = wpcm450_spi_setup;

	dev_dbg(hw->dev, "bitbang at %p\n", &hw->bitbang);
#if 0
	hw->clk = clk_get(&pdev->dev, "spi");
	if (IS_ERR(hw->clk)) {
		dev_err(&pdev->dev, "No clock for device\n");
		err = PTR_ERR(hw->clk);
		goto err_no_clk;
	}

	/* for the moment, permanently enable the clock */

	clk_enable(hw->clk);
#endif


	/* register our spi controller */

	err = spi_bitbang_start(&hw->bitbang);
	if (err) {
		dev_err(&pdev->dev, "Failed to register SPI master\n");
		goto err_register;
	}

	dev_dbg(hw->dev, "shutdown=%d\n", hw->bitbang.shutdown);

	return 0;

 err_register:
	clk_disable(hw->clk);
	clk_put(hw->clk);

 err_no_clk:
 err_no_iomap:
	release_resource(hw->ioarea);
	kfree(hw->ioarea);

 err_no_iores:
 err_no_pdata:
	spi_master_put(hw->master);;

 err_nomem:
	return err;
}

static int wpcm450_spi_remove(struct platform_device *dev)
{
	struct wpcm450_spi *hw = platform_get_drvdata(dev);

	platform_set_drvdata(dev, NULL);

	spi_unregister_master(hw->master);

	clk_disable(hw->clk);
	clk_put(hw->clk);

	release_resource(hw->ioarea);
	kfree(hw->ioarea);

	spi_master_put(hw->master);
	return 0;
}


#ifdef CONFIG_PM

static int wpcm450_spi_suspend(struct platform_device *pdev, pm_message_t msg)
{
	struct wpcm450_spi *hw = platform_get_drvdata(pdev);

	clk_disable(hw->clk);
	return 0;
}

static int wpcm450_spi_resume(struct platform_device *pdev)
{
	struct wpcm450_spi *hw = platform_get_drvdata(pdev);

	clk_enable(hw->clk);
	return 0;
}

#else
#define wpcm450_spi_suspend NULL
#define wpcm450_spi_resume  NULL
#endif

static struct platform_driver wpcm450_spidrv = {
	.probe		= wpcm450_spi_probe,
	.remove		= wpcm450_spi_remove,
	.suspend	= wpcm450_spi_suspend,
	.resume		= wpcm450_spi_resume,
	.driver		= {
		.name	= "wpcm450-spi",
		.owner	= THIS_MODULE,
	},
};


static int __init wpcm450_spi_init(void)
{
	int ret;
	struct platform_device *pdev;

	ret = platform_driver_register(&wpcm450_spidrv);
	pdev = platform_device_alloc("wpcm450-spi",1);
	platform_device_add_data(pdev, pdev , sizeof(struct wpcm450_spi));
	platform_device_add(pdev);
        return ret;
}

static void __exit wpcm450_spi_exit(void)
{
        platform_driver_unregister(&wpcm450_spidrv);
}

module_init(wpcm450_spi_init);
module_exit(wpcm450_spi_exit);

MODULE_DESCRIPTION("WPCM450 SPI Driver");
MODULE_AUTHOR("Casey Chang, <kcchang@aten.com.tw>");
MODULE_LICENSE("GPL");
