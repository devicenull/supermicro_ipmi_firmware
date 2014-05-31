

#define LPC_IRQ													   			 9
#define TOTAL_LPC_CHANNELS										   			 3
                                                                   
#define	LPC_ADDR    		                          WPCM450_VA_B800 + 0x8000
#define CHL0_OFFSET												   		0x000C
#define CHL1_OFFSET												   		0x001E
#define CHL2_OFFSET												   		0x0030
#define KCSR_SIZE												   		0X0090                                                                 
                                                                   
#define AIC_INT_CTL                                   WPCM450_VA_B800 + 0x2000
#define INT_CTL													   	0xB8002000
#define INTR_SIZE												   		 0x300

#define KCS_1                                                             0x11
#define KCS_2                                                             0x12
#define KCS_3                                                             0x17

#define SCR_RESET												   	0x00000047
#define INTR_MASK												   	0x00001000
                                                                   
#define AIC_SCR9_OFFSET	 									   		    0x0024
#define AIC_IMR_OFFSET											   		0x0114
#define AIC_MECR_OFFSET											   		0x0120
#define AIC_MDCR_OFFSET											   		0x0124
#define AIC_SSCR_OFFSET											   		0x0128
#define AIC_SCCR_OFFSET											   		0x012C
                                                                   
#define SMC                                                         0xC8001000
#define CTL                                                                  1

#define GCR_BA												    WPCM450_VA_B000
#define PWRON_OFFSET											   		0x0004
#define INTCR                                                             0x3C
#define STRAP56_MASK											   	0x00000030
#define LPC_BASE_ADDRESS0										   	0x0000164E
#define LPC_BASE_ADDRESS1										   	0x0000002E
#define LPC_BASE_ADDRESS2										   	0x0000007E
#define LPC_BASE_ADDRESS3										   	0x0000004E
#define HIFCR_OFFSET											   		0x0050


#define BYTE_REG(x)         *((volatile BYTE *) x)

#define KCS_BA				WPCM450_VA_B800 + 0x8000
#define KCSnST(n) 			(KCS_BA + 0x0C) + (0x12 * n) 	
#define KCSnDO(n)			(KCS_BA + 0x0E) + (0x12 * n) 	
#define KCSnDI(n)			(KCS_BA + 0x10) + (0x12 * n) 	
#define KCSnDOC(n)			(KCS_BA + 0x12) + (0x12 * n) 	
#define KCSnDOM(n)			(KCS_BA + 0x14) + (0x12 * n) 	
#define KCSnDIC(n)			(KCS_BA + 0x16) + (0x12 * n) 	
#define KCSnCTL(n)			(KCS_BA + 0x18) + (0x12 * n) 	
#define KCSnIC(n)			(KCS_BA + 0x1A) + (0x12 * n) 	
#define KCSnIE(n)			(KCS_BA + 0x1C) + (0x12 * n) 	

#define IBFIE_ENABLE                                                      0x01
#define KCSNST_IBF_ON                                                     0x02
#define KCSNST_MCD_BIT_MASK                                               0x08



#define VPlong(x)			*((volatile unsigned int *) x)
#define VPshort(x)			*((volatile unsigned short *) x)
#define VPchar(x)			*((volatile unsigned char *) x)

#define SET_BIT(reg, bit)		((reg) |= (0x01<<(bit)))
#define READ_BIT(reg, bit)		((reg) & (0x01<<(bit)))
#define CLEAR_BIT(reg, bit)		((reg) &= (~(0x01<<(bit))))
#define IS_BIT_SET(reg, bit)	(((reg) & (0x01<<(bit)))!=0)

#define POSTCODES_TOTAL_SIZE	0x12

#define POST_CODES_ARRAY_SIZE	0xff


#define BPCFA2L					VPchar(KCS_BA+0x042)		
#define BPCFA2M					VPchar(KCS_BA+0x044)
#define BPCFEN					VPchar(KCS_BA+0x046)
#define BPCFSTAT				VPchar(KCS_BA+0x048)
#define BPCFDATA				VPchar(KCS_BA+0x04A)
#define BPCFMSTAT				VPchar(KCS_BA+0x04C)

#define ADDR1EN		7
#define ADDR2EN		6
#define VDDIE		5
#define LRIE		4
#define FRIE		3
#define RI2IE		1
#define RI1IE		0

#define FIFO_V		7
#define FIFO_OF		5
#define AD			0

#define LR_CHG		6
#define LR_STAT		5
#define VDD_CHG		4
#define VDD_STAT	3
#define RI2_STS		1
#define RI1_STS		0


#ifdef KCS_DRV_C

typedef struct
{
    BYTE    b_InitFlag;
    BYTE    b_IsrState;
    BYTE    ab_RxBuf[KCS_PACKAGE_MAX_SIZE];
    BYTE    ab_TxBuf[KCS_PACKAGE_MAX_SIZE];
    BYTE    b_InDataSize;
    BYTE    *pb_OutDataPtr;
    BYTE    b_OutDataSize;
    BYTE    b_StatusCode;
    BYTE    b_LogChanlID;
    BYTE    b_BufferFullFlag;
    DWORD   dW_Addr;
    wait_queue_head_t  in_queue;
    BYTE    b_RecvCmd;
    BYTE    b_RecvNetLUN;
    //TX_QUEUE	*p_ChanlMsgQueGrp;
}St_KcsVarTag;

ssize_t kcs_read(struct file *, char *, size_t, loff_t *);
ssize_t kcs_write(struct file *, const char *, size_t count, loff_t *);
int kcs_ioctl(struct inode *, struct file *, unsigned int , unsigned long);
int kcs_open(struct inode *, struct file *);
int kcs_release(struct inode *, struct file *);
loff_t kcs_llseek(struct file *, loff_t, int);
irqreturn_t KCSDrvIsr(int irq, void *dev_id, struct pt_regs *regs);
void Plt_IBFIECtl(BYTE b_ChNo, BYTE b_Ctl);
void Plt_LPCEnlCtl(BYTE b_ChNo, BYTE b_Ctl);
void PltKCS_Put_ADR(BYTE b_KcsNo, DWORD dw_Addr);
void PltKCS_MiscInit(BYTE b_KcsNo);
void PltKCS_PutStatusBit_S0S1(BYTE b_KcsNo, BYTE b_Status);
BYTE PltKCS_ReadIntrptSrc(BYTE b_KcsNo);
void KCS_Potocol_Imp(BYTE b_DevID);
BYTE PltKCS_GetStatusBit_CD(BYTE b_KcsNo);
void PltKCS_Put_ODR(BYTE b_KcsNo, BYTE b_Data);
BYTE PltKCS_Get_IDR(BYTE b_KcsNo);
void KCS_Recv_WriteStartCmd(BYTE b_DevID);
void KCS_Recv_AbortCmd(BYTE b_DevID);
void KCS_Recv_IllegalCmd(BYTE b_DevID);
void KCS_ErrorHandler(BYTE  b_DevID);

#else

extern ssize_t kcs_read(struct file *, char *, size_t, loff_t *);
extern ssize_t kcs_write(struct file *, const char *, size_t count, loff_t *);
extern int kcs_ioctl(struct inode *, struct file *, unsigned int , unsigned long);
extern int kcs_open(struct inode *, struct file *);
extern int kcs_release(struct inode *, struct file *);
extern loff_t kcs_llseek(struct file *, loff_t, int);
extern irqreturn_t KCSDrvIsr(int irq, void *dev_id, struct pt_regs *regs);

#endif






