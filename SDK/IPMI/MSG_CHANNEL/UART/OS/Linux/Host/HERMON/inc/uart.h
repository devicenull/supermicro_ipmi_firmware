

#ifndef _UARTDRV_H_
#define	_UARTDRV_H_



#define	UART_CHANNEL_NUM		2
#define	UART_MAJOR				248
#define	UART_MINOR_OFFSET		64
#define	ADDRESS_WIDTH			0x04
#define DEF_SIZE 				102400 // old size = 1024


#define MINSIZE_COPY_TO_USER	200
#define READBUF_TIMEOUT			0.1        

#ifdef UART_FLOW_CONTROL
#define	WARNING_SIZE		0x40	// old size = 0x40
#endif

#define	UART0_BASE_ADDR		WPCM450_VA_B800
#define	UART1_BASE_ADDR		WPCM450_VA_B800+0x100
#define AIC_INT_CTL         WPCM450_VA_B800+0x2000

#define	SYSTEM_MFSEL1				0x0C
#define SYSTEM_SPSWC				0x38


#define UART0_IRQ			7
#define UART1_IRQ			8
#define UART_PAGE_SIZE		0x100	



#define AIC_SCR6			0x18	
#define AIC_SCR8			0x20	
#define AIC_ISR		        0x108
#define AIC_IMR		        0x114	
#define AIC_MECR			0x120	
#define AIC_SSCR			0x128	

#define BAUD_RATE_9600      9600
#define BAUD_RATE_19200    19200
#define BAUD_RATE_38400    38400
#define BAUD_RATE_57600    57600
#define BAUD_RATE_115200  115200

#define UART_CHNL_ENABLE	0x01
#define UART_CHNL_DISABLE	0x00

#define ENABLE				0x01
#define DISABLE				0x00

#define IRPT_RLS_ISR		0x03 
#define IRPT_RDA_ISR		0x02 
#define IRPT_TOUT_ISR		0x06 
#define IRPT_THRE_ISR		0x01 
#define IRPT_MOS_ISR		0x00 

#define UART_RBR  	0x00
#define UART_THR  	0x00
#define UART_DLL  	0x00
#define UART_DLM  	0x04
#define UART_IER  	0x04
#define UART_IIR  	0x08
#define UART_FCR  	0x08
#define UART_LCR  	0x0C
#define UART_MCR  	0x10
#define UART_LSR  	0x14
#define UART_MSR  	0x18
#define UART_TOR  	0x1C




#define FCR_RFITL_1BYTE		0x00
#define FCR_RFITL_4BYTE		0x40
#define FCR_RFITL_8BYTE		0x80
#define FCR_RFITL_14BYTE	0xC0
#define FCR_TFR_BIT			(1 << 2)
#define FCR_RFR_BIT			(1 << 1)
#define FCR_FME_BIT			(1 << 0)

#define LCR_DLAB_BIT	7
#define LCR_BCB_BIT		6
#define LCR_PBE_BIT		(1 << 3)
#define LCR_NSB_BIT		(1 << 2)
#define LCR_WLS_8BIT	3


#define TOR_TOIE_BIT		(1 << 7)


#define IER_NDBGACK_BIT		4
#define IER_MSIE_BIT		3
#define IER_RLSIE_BIT		2
#define IER_THREIE_BIT		1
#define IER_RDAIE_BIT		0

#define LSR_ERR_RX_BIT		(1 << 7) 
#define LSR_TE_BIT			(1 << 6) 
#define LSR_THRE_BIT		(1 << 5) 
#define LSR_BII_BIT			(1 << 4) 
#define LSR_FEI_BIT			(1 << 3) 
#define LSR_PEI_BIT			(1 << 2) 
#define LSR_OEI_BIT			(1 << 1)
#define LSR_RFDR_BIT		(1 << 0)  

#define IIR_IID_BIT			0x0E
#define IIR_NIP_BIT			0x01

#define MCR_NRTS_ZERO		0x00
#define MCR_NRTS_ONE		0x0F

#define MSR_CTS_BIT        	0x10
#define MSR_DCTS_BIT        0x01


#define UART_TRANSMIT_LENGTH 	16

#define SERIAL_PORT_MODE_1	0x01
#define SERIAL_PORT_MODE_2	0x02
#define SERIAL_PORT_MODE_3	0x03

#ifdef _UARTDRV_C_

typedef struct 
{   
    union {
			unsigned char     RBR;                		
        	unsigned char     THR;                		
            unsigned char     DLL;                   	
		  }			RTD;
            
	unsigned char	Temp0[3];            
	union {
    		unsigned char     DLM;                  	
    		union {                                 	
          			unsigned char BYTE;                 
          			struct {                            
          				 	 unsigned char wk        :3;
          				 	 unsigned char NDBGACK_EN:1;
            		     	 unsigned char MSIE      :1;
            		     	 unsigned char RLSIE     :1;
            		         unsigned char THREIE    :1;
            		     	 unsigned char RDAIE     :1;
            		  	   }      BIT;                  
          	  	  }        IER;                  		
          }			DI;
    unsigned char	Temp1[3];
   	union {
   			union {										
   					unsigned char BYTE;					
   					struct{								
   							unsigned char FMES :1;		
   							unsigned char RFTLS:2;		
   							unsigned char DMS  :1;		
   							unsigned char IID  :3;		
   							unsigned char NIP  :1;		
   					  	  }		 BIT;					
   			      }		  IIR;							
   			union {										
   					unsigned char BYTE;					
   					struct{								
   							unsigned char RFITL:4;		
   							unsigned char DMS  :1;		
   							unsigned char TFR  :1;		
   							unsigned char RFR  :1;		
   					  		unsigned char FME  :1;		
   					  	  }		 BIT;					
   				  }		  FCR;							
   		  }			IF;
   	unsigned char	Temp2[3];	  
    union {										 		
    		unsigned char BYTE;						 	
    		struct{									 	
    				unsigned char DLAB:1;		 		
    				unsigned char BCB :1;		 		
    				unsigned char SPE :1;		 		
    				unsigned char EPE :1;		 		
    				unsigned char PBE :1;		 		
    				unsigned char NSB :1;		 		
    			  	unsigned char WLS :2;		 		
    			  }		 BIT;						 	
    	  }		    LCR;					 			
   	unsigned char	Temp3[3];	  
   	union {										 		
   			unsigned char BYTE;						 	
   			struct{									 	
   					unsigned char wk0  :3;		 		
   					unsigned char LBME :1;		 		
   					unsigned char NOUT2:1;		 		
   					unsigned char wk1  :1;		 		
   					unsigned char NRTS :1;		 		
   				  	unsigned char NDTR :1;		 		
   				  }		 BIT;						 	
   		  }		    MCR;						 		
   	unsigned char	Temp4[3];	  
   	union {										 		
   			unsigned char BYTE;						 	
   			struct{									 	
   				  	unsigned char ERR_RX:1;		 		
   					unsigned char TE    :1;		 		
   					unsigned char THRE  :1;		 		
   					unsigned char BII   :1;		 		
   					unsigned char FEI   :1;		 		
   					unsigned char PEI   :1;		 		
   					unsigned char OEI   :1;		 		
   					unsigned char RFDR  :1;		 		
   				  }		 BIT;						 	
   		  }		    LSR;						 		
   	unsigned char	Temp5[3];	
   	union {										 		
   			unsigned char BYTE;						 	
   			struct{									 	
   					unsigned char DCD :1;		 		
   					unsigned char wk0 :1;		 		
   					unsigned char DSR :1;		 		
   					unsigned char CTS :3;		 		
   					unsigned char DDCD:1;		 		
   			        unsigned char wk1 :1;		 		
   			        unsigned char DDSR:1;		 		
   					unsigned char DCTS:1;		 		
   				  }		 BIT;						 	
   		  }		    MSR;						 		
   	unsigned char	Temp6[3];
   	union {										 		
   			unsigned char BYTE;						 	
   			struct{									 	
   				  	unsigned char TOIE :1;	 		 	
   				  	unsigned char TOIC :7;	 		 	
   				  }		 BIT;						 	
   		  }		    TOR;						 		
    unsigned char	Temp7[3];
    union {										 		
   			unsigned int DWORD;						 	
   			struct{									 	
   					unsigned int wk0  :24;	 		 	
   					unsigned int RTSS :1;	 		 	
   					unsigned int DTRS :1;	 		 	
   					unsigned int DCDI :1;	 		 	
   					unsigned int wk1  :3;	 		 	
   					unsigned int SPMOD:2;	 		 	
   				  }		 BIT;						 	
   		  }		    SPSWC;						 		
   	unsigned int	INTCR;						 		
}St_UART_REG_TAG;



typedef struct 
{
	DWORD 	*p_dw_BaseAddr;

	BYTE 	b_Set;
	BYTE 	b_Len;     
	BYTE 	b_Spmod;   
	DWORD 	dw_Baud;
} St_HwInfoTag;

typedef struct 
{
	struct semaphore in;
	struct semaphore out;
} St_MutexTag;

typedef struct 
{
	BYTE *p_b_Buffer;
	BYTE *p_b_Start;
	BYTE *p_b_End;		
	BYTE b_Overrun;
} St_QueueTag;

typedef struct 
{
	BYTE b_Chl;
	St_MutexTag St_Sem;
	St_QueueTag St_InQ, St_OutQ;
	wait_queue_head_t  in_queue, out_queue;
	St_HwInfoTag St_HwInfo;
	BYTE b_state;
	BYTE b_ncts;
	BYTE b_nrts;
	BYTE b_FlowCtl;
	BYTE b_NullReg;  
}St_UARTDevTag;


unsigned int UART_IRQ[UART_CHANNEL_NUM] = {
	UART0_IRQ,
	UART1_IRQ
};

unsigned int uart_INT_addr[UART_CHANNEL_NUM] = {
	AIC_SCR6,
	AIC_SCR8
};

void PltUARTDrv_SetBaudRate(BYTE, DWORD);
void PltUARTDrv_FIFOCtl(St_UARTDevTag *,BYTE);
void PltUARTDrv_LineCtl(BYTE);
void PltUARTDrv_TimeoutCtl(BYTE);
void PltUARTDrv_SPSWCCtl(DWORD);
void PltUARTDrv_InterruptCtl(St_UARTDevTag *,BYTE, BYTE);
void PltUARTDrv_ModemCtl(St_UARTDevTag *,BYTE);
BYTE PltUARTDrv_ReadIntrptSrc(BYTE b_UartNo);
void PltUARTDrv_BREAKKEY(BYTE,BYTE);
static void write_uart_thr(St_UARTDevTag *dev, BYTE b_Chnl);
int uart_open(struct inode *inode, struct file *filp);
int uart_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
ssize_t uart_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t uart_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
int uart_release(struct inode *inode, struct file *filp);
irqreturn_t uart_sh_interrupt(int irq, void *dev_id, struct pt_regs *regs);
#endif 

extern int uart_open(struct inode *inode, struct file *filp);
extern int uart_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
extern ssize_t uart_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
extern ssize_t uart_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
extern int uart_release(struct inode *inode, struct file *filp);
extern irqreturn_t uart_sh_interrupt(int irq, void *dev_id, struct pt_regs *regs);

#define UART_IOC_MAGIC		'k'

#define	UART_IOCSBAUD		_IO(UART_IOC_MAGIC, 11)
#define	UART_IOCSEN			_IO(UART_IOC_MAGIC, 12)
#define	UART_IOCSUN			_IO(UART_IOC_MAGIC, 13)
#define	UART_IOCSTRILV		_IO(UART_IOC_MAGIC, 14)
#define	UART_IOCSNRTS		_IO(UART_IOC_MAGIC, 15)
#define	UART_IOCSFLOCTL		_IO(UART_IOC_MAGIC, 16)
#define	UART_IOCSPMOD		_IO(UART_IOC_MAGIC, 10)
#define UART_IOCSBREAKKEY _IO(UART_IOC_MAGIC, 17)

#define UART_NONFLOWCTRL	0
#define UART_HWFLOWCTRL		1

#endif 



