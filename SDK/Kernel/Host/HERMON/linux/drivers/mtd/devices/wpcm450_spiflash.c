/*
 * MTD SPI driver for ST M25Pxx flash chips
 *
 * Author: Mike Lavender, mike@steroidmicros.com
 *
 * Copyright (c) 2005, Intec Automation Inc.
 *
 * Some parts are based on lart.c by Abraham Van Der Merwe
 *
 * Cleaned up and generalized based on mtd_dataflash.c
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>

#include <asm/semaphore.h>

#include <linux/delay.h>
#include <asm/io.h>


/* NOTE: AT 25F and SST 25LF series are very similar,
 * but commands for sector erase and chip id differ...
 */

#define FLASH_PAGESIZE		256

/* Flash opcodes. */
#define	OPCODE_WREN		6	/* Write enable */
#define	OPCODE_RDSR		5	/* Read status register */
#define	OPCODE_READ		3	/* Read data bytes */
#define	OPCODE_PP		2	/* Page program */
#define	OPCODE_SE		0xd8	/* Sector erase */
#define	OPCODE_RES		0xab	/* Read Electronic Signature */
#define	OPCODE_RDID		0x9f	/* Read JEDEC ID */

/* Status Register bits. */
#define	SR_WIP			1	/* Write in progress */
#define	SR_WEL			2	/* Write enable latch */
#define	SR_BP0			4	/* Block protect 0 */
#define	SR_BP1			8	/* Block protect 1 */
#define	SR_BP2			0x10	/* Block protect 2 */
#define	SR_SRWD			0x80	/* SR write protect */

/* Define max times to check status register before we give up. */
#define	MAX_READY_WAIT_COUNT	50000000


#ifdef CONFIG_MTD_PARTITIONS
#define	mtd_has_partitions()	(1)
#else
#define	mtd_has_partitions()	(0)
#endif



/****************************************************************************/
struct semaphore	SPIBus_lock;
EXPORT_SYMBOL_GPL(SPIBus_lock);
struct wpcm450_spiflash {
	struct spi_device	*spi;
	struct semaphore	lock;
	struct mtd_info		mtd;
	unsigned		partitioned;
	u8			command[4];
};

static inline struct wpcm450_spiflash *mtd_to_wpcm450_spiflash(struct mtd_info *mtd)
{
	return container_of(mtd, struct wpcm450_spiflash, mtd);
}

/****************************************************************************/

/*
 * Internal helper functions
 */


/*
 * Read the status register, returning its value in the location
 * Return the status register value.
 * Returns negative if error occurred.
 */

static unsigned char *HW_FIU_BA = NULL;

static void wpcm450_spi_fiu_hw_init() {

	//printk(KERN_INFO "%s\n",__FUNCTION__);
	
	unsigned char *GLO_CTRL_addr,*FLASH_BASE_addr;		

	GLO_CTRL_addr = (unsigned char *)(u32)ioremap_nocache((unsigned long)0xB0000000, 0x125);

	HW_FIU_BA = (unsigned char *)(u32)ioremap_nocache((unsigned long)0xC8000000, 0x200);

	*((unsigned char *) GLO_CTRL_addr+0x00C) |= 0 & ( (1 << 3) | (1 << 4) | (0 << 5)  );


	//PROT_CLEAR : UnLock  CFBB_PROT and PROT_LOCK
	*((unsigned char *) HW_FIU_BA + 0x11) = 0x87;
	udelay(10);
	*((unsigned char *) HW_FIU_BA + 0x11) = 0x61;  
	udelay(10);
	*((unsigned char *) HW_FIU_BA + 0x11) = 0x63; 
	udelay(10);
	
	//FIU_CFG : Set FL_Size to be Maximum
	*((unsigned char *) HW_FIU_BA) = 0x80; 


	//BURST_CFG : Set W_BURST/R_BURST to be Maximum
	*((unsigned char *) (HW_FIU_BA + 0x1 )) =   (0x2 << 4) | 0x2; 


	//RESP_CFG : Disable INT_EN and IAD_EN
	*((unsigned char *) (HW_FIU_BA + 0x2 )) = 0x0; 


	//CFBB_PROT : Disable Fireware Boot block protect
	*((unsigned char *) (HW_FIU_BA + 0x3 )) = 0xff; 


	//PROT_LOCK : Clear all Flash Access Window Lock 
	*((unsigned char *) (HW_FIU_BA + 0x10 )) = 0x0; 	

	//SPI_FL_CFG : Set Normal Read and DEV_SIZE to be Maximum (16MB)
	*((unsigned char *) (HW_FIU_BA + 0x14 )) = (0x0 << 6)| 0x20; 	

	//SPI_TIM : Set clock frequcncy
	*((unsigned char *) (HW_FIU_BA + 0x15 )) = 0x0B; 	

	//UMA_ECTS
	*((unsigned char *) (HW_FIU_BA + 0x1f )) = 0xf;
}


static void spi_fiu_wait() {

	unsigned char status = 0;
        int count = 0;

        do
	{
            udelay(10);
            status = *((unsigned char *) (HW_FIU_BA + 0x1e ));            
            count++;
            
	} while(0x80 & status) ;			
        
        if (count > 1000) {
            printk(KERN_INFO "%s wait: %d\n",__FUNCTION__,count);
        }
}

static void spi_fiu_check_wip() {

	unsigned char sr;

	while(1)
	{
		// Flash Cmd 
		*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_RDSR; 		 

		//UMA_CTS : Triggle Write Action
		*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (1) ; 		
		*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (1) ; 				

		spi_fiu_wait();	
                
                sr = *((unsigned char *) (HW_FIU_BA + 0x1a ));	

		if (!(sr & SR_WIP))
		{
			//printk(KERN_INFO "Clear WIP\n");
			return ;
		}
                
                udelay(10);
	}

}

static void spi_fiu_keep_write(unsigned char *tx_buf, int tx_len) {

	#define MAX_TXBUF_LEN 5

	unsigned char *buf = tx_buf;

        int mcount = tx_len / MAX_TXBUF_LEN ;
	int qcount = tx_len - (mcount * MAX_TXBUF_LEN );
        
         while((mcount > 0) || (qcount > 0))
	    {
	        if(mcount > 0)
	        {
                        *((unsigned char *) (HW_FIU_BA + 0x16 )) = buf[0]; 
		    	*((unsigned char *) (HW_FIU_BA + 0x1a )) = buf[1]; 		 			
	  		*((unsigned char *) (HW_FIU_BA + 0x1b )) = buf[2];
			*((unsigned char *) (HW_FIU_BA + 0x1c )) = buf[3];
			*((unsigned char *) (HW_FIU_BA + 0x1d )) = buf[4];			
			//UMA_CTS : Set Write setting
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (MAX_TXBUF_LEN-1) ; 		

			//UMA_CTS : Triggle Write Action
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (MAX_TXBUF_LEN-1) ; 		
		}
	        else
	        {
			switch (qcount) {			
                                case 4:
		  		*((unsigned char *) (HW_FIU_BA + 0x1c)) = buf[3];
                                case 3:
		  		*((unsigned char *) (HW_FIU_BA + 0x1b)) = buf[2];
				case 2:
				*((unsigned char *) (HW_FIU_BA + 0x1a)) = buf[1];
				case 1:
				*((unsigned char *) (HW_FIU_BA + 0x16)) = buf[0];
			}
			//UMA_CTS : Set Write setting
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (qcount-1) ; 		

			//UMA_CTS : Triggle Write Action
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (qcount-1) ; 	
	                qcount = 0;
	        }

		spi_fiu_wait();
	        
	        buf += MAX_TXBUF_LEN ;
	        mcount--;
	    }

	
}

static void spi_fiu_write_sync(struct wpcm450_spiflash *flash, struct spi_message *m) {

	struct spi_transfer	*t = NULL;
	struct spi_transfer	*tt[2];
	int count = 0;
	unsigned char *buf;

	list_for_each_entry (t, &m->transfers, transfer_list) {
		tt[count] = t;
		count++;
	}
        
        //UMA_ECTS
	*((unsigned char *) (HW_FIU_BA + 0x1f )) = 0x0;

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_PP; 		 

	buf = tt[0]->tx_buf;

	// Address
	*((unsigned char *) (HW_FIU_BA + 0x19 )) = buf[1];
	*((unsigned char *) (HW_FIU_BA + 0x18 )) = buf[2];
	*((unsigned char *) (HW_FIU_BA + 0x17 )) = buf[3];


	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		
        
        spi_fiu_wait();	 
        
	spi_fiu_keep_write(tt[1]->tx_buf,tt[1]->len);
       
        //UMA_ECTS
	*((unsigned char *) (HW_FIU_BA + 0x1f )) = 0xf;
        
        m->actual_length = tt[1]->len + 4;
        
  
} 


static void spi_fiu_keep_read(unsigned char *rx_buf, int rx_len, unsigned int addr ) {

	#define MAX_RXBUF_LEN 4

        int mcount = rx_len / MAX_RXBUF_LEN  ;
	int qcount = rx_len - (mcount * MAX_RXBUF_LEN);
	unsigned char *buf = rx_buf;

	//printk(KERN_INFO "%s\n",__FUNCTION__);
	//printk(KERN_INFO "Addr:%x\n",addr);
	    		
	    while((mcount > 0) || (qcount > 0))
	    {

            // Flash Cmd 
	    *((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_READ; 		 

       	    // Address 
	    *((unsigned char *) (HW_FIU_BA + 0x19 )) = addr >> 16;
	    *((unsigned char *) (HW_FIU_BA + 0x18 )) = addr >> 8;
	    *((unsigned char *) (HW_FIU_BA + 0x17 )) = addr;


            if(mcount > 0)
	        {	 			

			//UMA_CTS : Set Read setting
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (1 << 3) | (MAX_RXBUF_LEN) ; 

			//UMA_CTS : Triggle Read Action
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (1 << 3) | (MAX_RXBUF_LEN) ; 	

			spi_fiu_wait();

	  		buf[0] = *((unsigned char *) (HW_FIU_BA + 0x1a ));
			buf[1] = *((unsigned char *) (HW_FIU_BA + 0x1b ));
			buf[2] = *((unsigned char *) (HW_FIU_BA + 0x1c ));
			buf[3] = *((unsigned char *) (HW_FIU_BA + 0x1d ));	
		}
	        else
	        {
			//UMA_CTS : Set Read setting
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (1 << 3) | (qcount) ; 				

			//UMA_CTS : Triggle Read Action
			*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (1 << 3) | (qcount) ; 	

			spi_fiu_wait();

			switch (qcount) {
				case 3:
					buf[2] = *((unsigned char *) (HW_FIU_BA + 0x1c)); 		 			
				case 2:
		  			buf[1] = *((unsigned char *) (HW_FIU_BA + 0x1b));
				case 1:
					buf[0] = *((unsigned char *) (HW_FIU_BA + 0x1a));
			}

	                qcount = 0;
	
	        }
	        
	        buf += MAX_RXBUF_LEN ;
		addr += MAX_RXBUF_LEN;
	        mcount--;
	    }
}

static void spi_fiu_read_sync(struct wpcm450_spiflash *flash, struct spi_message *m) {

	struct spi_transfer	*t = NULL;
	struct spi_transfer	*tt[2];
	int addr;	
	int count = 0;
	int i = 0;
	unsigned char *buf;


	//printk(KERN_INFO "%s\n",__FUNCTION__);

	list_for_each_entry (t, &m->transfers, transfer_list) {
		tt[count] = t;
		count++;
	}

	buf = tt[0]->tx_buf;

	addr = (buf[1] << 16) | (buf[2] << 8) | (buf[3]); 

	spi_fiu_keep_read(tt[1]->rx_buf,tt[1]->len,addr);

	m->actual_length = tt[1]->len + 4;


} 

#if 0

static unsigned char tmp_buf[4096] = {
    1,2,3,4,5,6,7,8,
    9,10,11,12,13,14,
    15,16
};

static void spi_fiu_test_read() {

	int count = 16;
	int i = 0;
	unsigned int addr = 0x300000;

	spi_fiu_keep_read(tmp_buf, count , addr );

	while(i < count) {
		printk(KERN_INFO "spi_fiu_test_read:tmp_buf[%d]:%x\n",i,tmp_buf[i]);		
		i++;
	}

}


static void spi_fiu_test_erase() {

	int offset = 0x300000;

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_WREN; 		 

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		

	spi_fiu_wait();	

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_SE; 		 

	// Address
	*((unsigned char *) (HW_FIU_BA + 0x19 )) = offset >> 16;
	*((unsigned char *) (HW_FIU_BA + 0x18 )) = offset >> 8;
	*((unsigned char *) (HW_FIU_BA + 0x17 )) = offset;


	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		

	spi_fiu_wait();

	spi_fiu_check_wip();

	//spi_fiu_test_read();


}




static void spi_fiu_test_program() {

	int offset = 0x300000;

	spi_fiu_check_wip();

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_WREN; 		 

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		

	spi_fiu_wait();	

        
        *((unsigned char *) (HW_FIU_BA + 0x1f )) = 0x0;
        
	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_PP; 		 

	// Address
	*((unsigned char *) (HW_FIU_BA + 0x19 )) = offset >> 16;
	*((unsigned char *) (HW_FIU_BA + 0x18 )) = offset >> 8;
	*((unsigned char *) (HW_FIU_BA + 0x17 )) = offset;


	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 	
        
        spi_fiu_wait();	
        
        spi_fiu_keep_write(tmp_buf,16);
        
        *((unsigned char *) (HW_FIU_BA + 0x1f )) = 0xf;

	spi_fiu_check_wip();
        
        spi_fiu_test_read();
        

}


static void spi_fiu_test_readSR() {

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_RDSR; 		 

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (1) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (1) ; 		

	spi_fiu_wait();	

	printk(KERN_INFO "SR:0x%x\n",*((unsigned char *) (HW_FIU_BA + 0x1a )));	
	
	
}

static void spi_fiu_test_write()
{                
    
        int offset = 0x300000;
        
	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_PP; 		 

	// Address
	*((unsigned char *) (HW_FIU_BA + 0x19 )) = offset >> 16;
	*((unsigned char *) (HW_FIU_BA + 0x18 )) = offset >> 8;
	*((unsigned char *) (HW_FIU_BA + 0x17 )) = offset;


        *((unsigned char *) (HW_FIU_BA + 0x1f )) = 0x0;
	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		

	spi_fiu_wait();	
        
        
        spi_fiu_keep_write(tmp_buf, 16);
        

}
#endif

static int read_sr(struct wpcm450_spiflash *flash)
{
	ssize_t retval;
	
	u8 val;

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_RDSR; 		 

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (1) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | (1) ; 		

	spi_fiu_wait();	

	val = *((unsigned char *) (HW_FIU_BA + 0x1a ));
	retval = 0;// successful

	if (retval < 0) {
		dev_err(&flash->spi->dev, "error %d reading SR\n",
				(int) retval);
		return retval;
	}

	return val;
}


/*
 * Set write enable latch with Write Enable command.
 * Returns negative if error occurred.
 */
static inline int write_enable(struct wpcm450_spiflash *flash)
{

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_WREN; 		 

	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (0 << 3) | (0) ; 		

	spi_fiu_wait();	
        
	return 0;// successful

}


/*
 * Service routine to read status register until ready, or timeout occurs.
 * Returns non-zero if error.
 */
    static int wait_till_ready(struct wpcm450_spiflash *flash)
{
	int count;
	int sr;

	/* one chip guarantees max 5 msec wait here after page writes,
	 * but potentially three seconds (!) after page erase.
	 */
	for (count = 0; count < MAX_READY_WAIT_COUNT; count++) {
		if ((sr = read_sr(flash)) < 0)
			break;
                else if (!(sr & SR_WIP)) {
                    if (count > 1000) {
                        printk(KERN_INFO "%s,sr:%d, waitcount:%d\n",__FUNCTION__,sr,count);
                    }
			return 0;
                }

		/* REVISIT sometimes sleeping would be best */
                udelay(1);
	}
        printk(KERN_INFO "%s,sr:%d, waitcount:%d\n",__FUNCTION__,sr,count);
	return 1;
}


/*
 * Erase one sector of flash memory at offset ``offset'' which is any
 * address within the sector which should be erased.
 *
 * Returns 0 if successful, non-zero otherwise.
 */
static int erase_sector(struct wpcm450_spiflash *flash, u32 offset)
{
	DEBUG(MTD_DEBUG_LEVEL3, "%s: %s at 0x%08x\n", flash->spi->dev.bus_id,
			__FUNCTION__, offset);

	/* Wait until finished previous write command. */
	if (wait_till_ready(flash))
		return 1;

	/* Send write enable, then erase commands. */
	write_enable(flash);

	
	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_SE; 		 

	// Address
	*((unsigned char *) (HW_FIU_BA + 0x19 )) = offset >> 16;
	*((unsigned char *) (HW_FIU_BA + 0x18 )) = offset >> 8;
	*((unsigned char *) (HW_FIU_BA + 0x17 )) = offset;


	//UMA_CTS : Triggle Write Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (1 << 4) | (1 << 3) | (0) ; 		

	spi_fiu_wait();

	return 0;
}

/****************************************************************************/

/*
 * MTD implementation
 */

/*
 * Erase an address range on the flash chip.  The address range may extend
 * one or more erase sectors.  Return an error is there is a problem erasing.
 */
static int wpcm450_spiflash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct wpcm450_spiflash *flash = mtd_to_wpcm450_spiflash(mtd);
	u32 addr,len;

	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %d\n",
			flash->spi->dev.bus_id, __FUNCTION__, "at",
			(u32)instr->addr, instr->len);

	/* sanity checks */
        if (instr->addr + instr->len > flash->mtd.size) {
                return -EINVAL;
        }

	if ((instr->addr % mtd->erasesize) != 0
			|| (instr->len % mtd->erasesize) != 0) {
                   
		return -EINVAL;
	}

	addr = instr->addr;
	len = instr->len;
        


  	down(&flash->lock);
    down(&SPIBus_lock);  

	/* now erase those sectors */
	while (len) {
        
		if (erase_sector(flash, addr)) {
			instr->state = MTD_ERASE_FAILED;
			up(&flash->lock);
			up(&SPIBus_lock); 
			return -EIO;
		}
              
		addr += mtd->erasesize;
		len -= mtd->erasesize;
	}
        
        
	up(&SPIBus_lock); 
  	up(&flash->lock);

	instr->state = MTD_ERASE_DONE;       
	mtd_erase_callback(instr);
        

	return 0;
}

/*
 * Read an address range from the flash chip.  The address range
 * may be any size provided it is within the physical boundaries.
 */
static int wpcm450_spiflash_read(struct mtd_info *mtd, loff_t from, size_t len,
	size_t *retlen, u_char *buf)
{
	struct wpcm450_spiflash *flash = mtd_to_wpcm450_spiflash(mtd);
	struct spi_transfer t[2];
	struct spi_message m;

#if 0
	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %zd\n",
			flash->spi->dev.bus_id, __FUNCTION__, "from",
			(u32)from, len);
#endif

	/* sanity checks */
	if (!len)
		return 0;

	if (from + len > flash->mtd.size)
		return -EINVAL;

	spi_message_init(&m);
	memset(t, 0, (sizeof t));

	t[0].tx_buf = flash->command;
	t[0].len = sizeof(flash->command);
	spi_message_add_tail(&t[0], &m);

	t[1].rx_buf = buf;
	t[1].len = len;
	spi_message_add_tail(&t[1], &m);

	/* Byte count starts at zero. */
	if (retlen)
		*retlen = 0;

	down(&flash->lock);
	down(&SPIBus_lock);

	/* Wait till previous write/erase is done. */
	if (wait_till_ready(flash)) {
		/* REVISIT status return?? */
		up(&flash->lock);
		up(&SPIBus_lock);
		return 1;
	}

	/* NOTE:  OPCODE_FAST_READ (if available) is faster... */

	/* Set up the write data buffer. */
	flash->command[0] = OPCODE_READ;
	flash->command[1] = from >> 16;
	flash->command[2] = from >> 8;
	flash->command[3] = from;

	//spi_sync(flash->spi, &m);
	spi_fiu_read_sync(flash->spi, &m);

	*retlen = m.actual_length - sizeof(flash->command);

  	up(&flash->lock);
	up(&SPIBus_lock);
	return 0;
}

/*
 * Write an address range to the flash chip.  Data must be written in
 * FLASH_PAGESIZE chunks.  The address range may be any size provided
 * it is within the physical boundaries.
 */
static int wpcm450_spiflash_write(struct mtd_info *mtd, loff_t to, size_t len,
	size_t *retlen, const u_char *buf)
{
	struct wpcm450_spiflash *flash = mtd_to_wpcm450_spiflash(mtd);
	u32 page_offset, page_size;
	struct spi_transfer t[2];
	struct spi_message m;

	DEBUG(MTD_DEBUG_LEVEL2, "%s: %s %s 0x%08x, len %zd\n",
			flash->spi->dev.bus_id, __FUNCTION__, "to",
			(u32)to, len);

	if (retlen)
		*retlen = 0;

	/* sanity checks */
	if (!len)
		return(0);

	if (to + len > flash->mtd.size)
		return -EINVAL;

	spi_message_init(&m);
	memset(t, 0, (sizeof t));

	t[0].tx_buf = flash->command;
	t[0].len = sizeof(flash->command);
	spi_message_add_tail(&t[0], &m);

	t[1].tx_buf = buf;
	spi_message_add_tail(&t[1], &m);

  	down(&flash->lock);
	down(&SPIBus_lock);
	/* Wait until finished previous write command. */
	if (wait_till_ready(flash))
	{
		up(&SPIBus_lock);
		up(&flash->lock);
		return 1;
	}

	write_enable(flash);

	/* Set up the opcode in the write buffer. */
	flash->command[0] = OPCODE_PP;
	flash->command[1] = to >> 16;
	flash->command[2] = to >> 8;
	flash->command[3] = to;

	/* what page do we start with? */
	page_offset = to % FLASH_PAGESIZE;

	/* do all the bytes fit onto one page? */
	if (page_offset + len <= FLASH_PAGESIZE) {
		t[1].len = len;

		//spi_sync(flash->spi, &m);
		spi_fiu_write_sync(flash->spi, &m);

		*retlen = m.actual_length - sizeof(flash->command);
                //printk(KERN_INFO "1:%s,retlen:%d\n",__FUNCTION__,*retlen);
	} else {
		u32 i;

		/* the size of data remaining on the first page */
		page_size = FLASH_PAGESIZE - page_offset;

		t[1].len = page_size;
		//spi_sync(flash->spi, &m);
		spi_fiu_write_sync(flash->spi, &m);

		*retlen = m.actual_length - sizeof(flash->command);
                //printk(KERN_INFO "2:%s,retlen:%d\n",__FUNCTION__,*retlen);

		/* write everything in PAGESIZE chunks */
		for (i = page_size; i < len; i += page_size) {
			page_size = len - i;
			if (page_size > FLASH_PAGESIZE)
				page_size = FLASH_PAGESIZE;

			/* write the next page to flash */
			flash->command[1] = (to + i) >> 16;
			flash->command[2] = (to + i) >> 8;
			flash->command[3] = (to + i);

			t[1].tx_buf = buf + i;
			t[1].len = page_size;

			wait_till_ready(flash);

			write_enable(flash);

			//spi_sync(flash->spi, &m);
			spi_fiu_write_sync(flash->spi, &m);

                        if (retlen) {
				*retlen += m.actual_length - sizeof(flash->command);
                                //printk(KERN_INFO "3:%s,retlen:%d\n",__FUNCTION__,*retlen);
                        }
	        }
 	}
	up(&SPIBus_lock);
	up(&flash->lock);

	return 0;
}


/****************************************************************************/

/*
 * SPI device driver setup and teardown
 */

struct flash_info {
	char		*name;
	u8		id;
	u32		jedec_id;
	unsigned	sector_size;
	unsigned	n_sectors;
};

static struct flash_info __devinitdata wpcm450_spiflash_data [] = {
	/* REVISIT: fill in JEDEC ids, for parts that have them */
	{ "S25FL128P_16M_64KB", 0x01, 0x182001, 64 * 1024, 256 }, 
	{ "MX25L12805D_16M_64KB", 0xc2, 0x1820c2, 64 * 1024, 256 },
	{ "STM25P128_16M_256KB", 0x20, 0x182020, 256 * 1024, 64 },
	{ "STM25P64_8M_64KB", 0x20, 0x1720, 64 * 1024, 128 },
};

static void SPI_ReadPID(unsigned char *PID0, unsigned int *PID1)
{
	unsigned char ab_data[4];

	// Flash Cmd 
	*((unsigned char *) (HW_FIU_BA + 0x16 )) = OPCODE_RDID;	
	
	//UMA_CTS : Set Read setting
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (0 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | 3 ; 
	
	//UMA_CTS : Triggle Read Action
	*((unsigned char *) (HW_FIU_BA + 0x1e )) =  (1 << 7) | (0 << 5) | (0 << 4) | (0 << 3) | 3 ; 	
	
	spi_fiu_wait();	
	
	ab_data[0] = *((unsigned char *) (HW_FIU_BA + 0x1a ));
	ab_data[1] = *((unsigned char *) (HW_FIU_BA + 0x1b ));
	ab_data[2] = *((unsigned char *) (HW_FIU_BA + 0x1c ));

	*PID0 = ab_data[0];
	*PID1 = (ab_data[0]) | (ab_data[1] << 8) | (ab_data[2] << 16);

//	printk(KERN_INFO "FLASH *PID0:%x \n",*PID0);
	printk(KERN_INFO "FLASH *PID1:%x \n",*PID1);
	
	return;
}


/*
 * board specific setup should have ensured the SPI clock used here
 * matches what the READ command supports, at least until this driver
 * understands FAST_READ (for clocks over 25 MHz).
 */
static int __devinit wpcm450_spiflash_probe(struct spi_device *spi)
{
	struct flash_platform_data	*data;
	struct wpcm450_spiflash			*flash;
	struct flash_info		*info;
	unsigned			i;
	unsigned char pid0;
	unsigned int pid1;	
	
	/* Platform data helps sort out which chip type we have, as
	 * well as how this board partitions it.
	 */
	data = spi->dev.platform_data;
	if (!data || !data->type) {
		/* FIXME some chips can identify themselves with RES
		 * or JEDEC get-id commands.  Try them ...
		 */
		DEBUG(MTD_DEBUG_LEVEL1, "%s: no chip id\n",
				flash->spi->dev.bus_id);
		return -ENODEV;
	}

	SPI_ReadPID(&pid0, &pid1);	

	for (i = 0, info = wpcm450_spiflash_data; i < ARRAY_SIZE(wpcm450_spiflash_data); i++, info++) {
		if (pid1 == info->jedec_id)
			break;
	}
	if (i == ARRAY_SIZE(wpcm450_spiflash_data)) {
		DEBUG(MTD_DEBUG_LEVEL1, "%s: unrecognized id %s\n",
				flash->spi->dev.bus_id, data->type);
		return -ENODEV;
	}

	flash = kzalloc(sizeof *flash, SLAB_KERNEL);
	if (!flash)
		return -ENOMEM;

	flash->spi = spi;
	init_MUTEX(&SPIBus_lock);
	init_MUTEX(&flash->lock);
	dev_set_drvdata(&spi->dev, flash);

	if (data->name)
		flash->mtd.name = info->name;
	else
		flash->mtd.name = spi->dev.bus_id;

	flash->mtd.type = MTD_NORFLASH;
	flash->mtd.flags = MTD_CAP_NORFLASH;
	flash->mtd.size = info->sector_size * info->n_sectors;
	flash->mtd.erasesize = info->sector_size;
	flash->mtd.erase = wpcm450_spiflash_erase;
	flash->mtd.read = wpcm450_spiflash_read;
	flash->mtd.write = wpcm450_spiflash_write;

	dev_info(&spi->dev, "%s (%d Kbytes)\n", info->name,
			flash->mtd.size / 1024);

	DEBUG(MTD_DEBUG_LEVEL2,
		"mtd .name = %s, .size = 0x%.8x (%uM) "
			".erasesize = 0x%.8x (%uK) .numeraseregions = %d\n",
		flash->mtd.name,
		flash->mtd.size, flash->mtd.size / (1024*1024),
		flash->mtd.erasesize, flash->mtd.erasesize / 1024,
		flash->mtd.numeraseregions);

	if (flash->mtd.numeraseregions)
		for (i = 0; i < flash->mtd.numeraseregions; i++)
			DEBUG(MTD_DEBUG_LEVEL2,
				"mtd.eraseregions[%d] = { .offset = 0x%.8x, "
				".erasesize = 0x%.8x (%uK), "
				".numblocks = %d }\n",
				i, flash->mtd.eraseregions[i].offset,
				flash->mtd.eraseregions[i].erasesize,
				flash->mtd.eraseregions[i].erasesize / 1024,
				flash->mtd.eraseregions[i].numblocks);


	/* partitions should match sector boundaries; and it may be good to
	 * use readonly partitions for writeprotected sectors (BP2..BP0).
	 */

	if (mtd_has_partitions()) {
		struct mtd_partition	*parts = NULL;
		int			nr_parts = 0;

#ifdef CONFIG_MTD_CMDLINE_PARTS
		static const char *part_probes[] = { "cmdlinepart", NULL, };

		nr_parts = parse_mtd_partitions(&flash->mtd,
				part_probes, &parts, 0);
#endif


		
		if (nr_parts <= 0 && data && data->parts) {
			parts = data->parts;
			nr_parts = data->nr_parts;
		}

		if (nr_parts > 0) {
			for (i = 0; i < data->nr_parts; i++) {
				DEBUG(MTD_DEBUG_LEVEL2, "partitions[%d] = "
					"{.name = %s, .offset = 0x%.8x, "
						".size = 0x%.8x (%uK) }\n",
					i, data->parts[i].name,
					data->parts[i].offset,
					data->parts[i].size,
					data->parts[i].size / 1024);
			}
			flash->partitioned = 1;
			return add_mtd_partitions(&flash->mtd, parts, nr_parts);
		}
	} else if (data->nr_parts)
		dev_warn(&spi->dev, "ignoring %d default partitions on %s\n",
				data->nr_parts, data->name);

	return add_mtd_device(&flash->mtd) == 1 ? -ENODEV : 0;
}


static int __devexit wpcm450_spiflash_remove(struct spi_device *spi)
{
	struct wpcm450_spiflash	*flash = dev_get_drvdata(&spi->dev);
	int		status;

	/* Clean up MTD stuff. */
	if (mtd_has_partitions() && flash->partitioned)
		status = del_mtd_partitions(&flash->mtd);
	else
		status = del_mtd_device(&flash->mtd);
	if (status == 0)
		kfree(flash);
	return 0;
}


static struct spi_driver wpcm450_spiflash_driver = {
	.driver = {
		.name	= "wpcm450_spiflash16M",
		.bus	= &spi_bus_type,
		.owner	= THIS_MODULE,
	},
	.probe	= wpcm450_spiflash_probe,
	.remove	= __devexit_p(wpcm450_spiflash_remove),
};

//device info
static struct flash_platform_data wpcm450_spiflash_info = {
	.type = "wpcm450_spiflash16M"
};

static struct spi_board_info wpcm450_spiflash_spi_board_info[] __initdata = 
{
	{
	.modalias = "wpcm450_spiflash16M",
	.platform_data = &wpcm450_spiflash_info,
	.max_speed_hz	= 120000 /* max sample rate at 3V */ * 16,
	.bus_num	= 0,
	.chip_select	= 0,
	},

};

static int wpcm450_spiflash_init(void)
{
	//printk(KERN_INFO "wpcm450_spiflash_init\n");
	spi_register_board_info(wpcm450_spiflash_spi_board_info,ARRAY_SIZE(wpcm450_spiflash_spi_board_info));
	wpcm450_spi_fiu_hw_init();
	return spi_register_driver(&wpcm450_spiflash_driver);
}



static void wpcm450_spiflash_exit(void)
{
	spi_unregister_driver(&wpcm450_spiflash_driver);
}


module_init(wpcm450_spiflash_init);
module_exit(wpcm450_spiflash_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Casey Chang");
MODULE_DESCRIPTION("MTD SPI driver for WCPM450 chips");
