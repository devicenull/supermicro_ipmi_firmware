#ifndef __TECHOMETER_DRV_H_
#define __TECHOMETER_DRV_H_

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

#define	MFT_MODULE_NUM			2
#define	TECHOMETER_MAJOR 		251	
#define MFT0_IRQ				24		
#define MFT1_IRQ				25		

#define TECHOMETER_MODULE_NAME 	"hermon_techometer"

#define IOCTL_SETUP_MFSEL		 		0x1554
#define	IOCTL_SETUP_PRESCALE			0x1555
#define IOCTL_GET_APBCLOCK		 		0x1556

#define WriteRegB(port,value)		*((volatile unsigned char *)(port))=value
#define ReadRegB(port)			*((volatile unsigned char *)(port))
#define WriteRegW(port,value)		*((volatile unsigned short *)(port))=value
#define	ReadRegW(port)			*((volatile unsigned short *)(port))
#define WriteRegD(port,value)		*((volatile unsigned long *)(port))=value
#define	ReadRegD(port)			*((volatile unsigned long *)(port))


#define GCR_BA				WPCM450_VA_B000
#define MFSEL2				(GCR_BA+0x010)		

#define CLK_BA				WPCM450_VA_B000 + 0x200			
#define	CLKEN				(CLK_BA+0x00)					
#define	CLKSEL				(CLK_BA+0x04)					
#define	CLKDIV				(CLK_BA+0x08)					
#define	PLLCON0				(CLK_BA+0x0C)					
#define	PLLCON1				(CLK_BA+0x10)					
#define	PMCON				(CLK_BA+0x14)					
#define	IRQWAKECON			(CLK_BA+0x18)					
#define	IRQWAKEFLAG			(CLK_BA+0x1C)					
#define	IPSRST				(CLK_BA+0x20)					

#define MFT_BA				WPCM450_VA_B800 + 0x4000		

#define TnCNT1(n)			((MFT_BA+0x00)+(0x1000*n)) 		
#define TnCRA(n)			((MFT_BA+0x02)+(0x1000*n)) 		
#define TnCRB(n)   			((MFT_BA+0x04)+(0x1000*n)) 		
#define TnCNT2(n)  			((MFT_BA+0x06)+(0x1000*n)) 		
#define TnPRSC(n) 			((MFT_BA+0x08)+(0x1000*n)) 		
#define TnCKC(n)  			((MFT_BA+0x0A)+(0x1000*n)) 		
#define TnMCTRL(n) 			((MFT_BA+0x0C)+(0x1000*n)) 		
#define TnICTRL(n)			((MFT_BA+0x0E)+(0x1000*n)) 		
#define TnICLR(n) 			((MFT_BA+0x10)+(0x1000*n)) 		
#define TnIEN(n) 			((MFT_BA+0x12)+(0x1000*n)) 		
#define TnCPA(n) 			((MFT_BA+0x14)+(0x1000*n)) 		
#define TnCPB(n) 			((MFT_BA+0x16)+(0x1000*n)) 		
#define TnCPCFG(n)			((MFT_BA+0x18)+(0x1000*n)) 		
#define TnINASEL(n)			((MFT_BA+0x1A)+(0x1000*n)) 		
#define TnINBSEL(n)			((MFT_BA+0x1C)+(0x1000*n))		


#define	Timer_Input_Num		4
#define	MFT_TIMER_NUM		2
#define	TnTIMEOUT			0.5        
#define	MFT_TnPRSC			31



typedef enum
{
    TECHO_IDLE = 0,
    TECHO_CAP1,
    TECHO_CAP2 
}techo_state_t;

typedef  struct {
	BYTE	Tn_InputAB_Sel;
	WORD	Tn_Cap_Value;
} St_TECHOMETERInfoTag;

typedef struct 
{
	struct semaphore 		Tn_sem[MFT_TIMER_NUM];
	wait_queue_head_t		Tn_wq[MFT_TIMER_NUM];
	techo_state_t			MFT_STATE[MFT_TIMER_NUM];
	BYTE					MFT_ID;
	BYTE					TnPND[MFT_TIMER_NUM];
	WORD					TnCTN_Cap[MFT_TIMER_NUM];
}St_MFTModuleTag;



int techometer_open(struct inode *inode, struct file *file);
int techometer_release(struct inode *inode, struct file *file);
int techometer_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);
ssize_t techometer_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
ssize_t techometer_read(struct file *filp, char *data_buf, size_t count, loff_t *f_pos);
static irqreturn_t techometer_interrupt(int irq, void *dev_id, struct pt_regs *regs);

#endif
