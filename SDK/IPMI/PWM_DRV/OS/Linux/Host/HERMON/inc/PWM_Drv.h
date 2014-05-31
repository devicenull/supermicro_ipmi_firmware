

#define LPC_IRQ			28

#define PWMM0_BA		0xB8007000
#define PWMM1_BA		0xB8007100
#define MFSEL2_BA		0xB0000010
#define CLK_BA			0xB0000200
#define PWMM_REG_TOTAL_SIZE	0x44
#define MFSEL2_SIZE		0x04
#define CLK_REG_TOTAL_SIZE	0x14

#define PWM_MAJOR		240
#define PWM_MODULE_NAME		"Pulse Width Modulation"

#define PWMM0_ADDRESS		(unsigned int)uipPWMM0_VA
#define M0_PPR			VPlong(PWMM0_ADDRESS)
#define M0_PPR_C01		VPchar(PWMM0_ADDRESS)
#define M0_PPR_C23		VPchar(PWMM0_ADDRESS+0x01)
#define M0_CSR			VPlong(PWMM0_ADDRESS+0x04)
#define M0_PCR			VPlong(PWMM0_ADDRESS+0x08)
#define M0_CNR(offset)          VPshort(PWMM0_ADDRESS + 0x0C + offset * 0x0C)
#define M0_CMR(offset)          VPshort(PWMM0_ADDRESS + 0x10 + offset * 0x0C)
#define M0_PDR(offset)          VPshort(PWMM0_ADDRESS + 0x14 + offset * 0x0C)
#define M0_PIER			VPlong(PWMM0_ADDRESS+0x3C)
#define M0_PIIR			VPlong(PWMM0_ADDRESS+0x40) 

#define PWMM1_ADDRESS		(unsigned int)uipPWMM1_VA
#define M1_PPR                  VPlong(PWMM1_ADDRESS)
#define M1_PPR_C01		VPchar(PWMM1_ADDRESS)
#define M1_PPR_C23		VPchar(PWMM1_ADDRESS+0x01)
#define M1_CSR                  VPlong(PWMM1_ADDRESS+0x04)                      
#define M1_PCR                  VPlong(PWMM1_ADDRESS+0x08) 
#define M1_CNR(offset)		VPshort(PWMM1_ADDRESS + 0x0C + offset * 0x0C)
#define M1_CMR(offset)		VPshort(PWMM1_ADDRESS + 0x10 + offset * 0x0C)
#define M1_PDR(offset)		VPshort(PWMM1_ADDRESS + 0x14 + offset * 0x0C)
#define M1_PIER                 VPlong(PWMM1_ADDRESS+0x3C) 
#define M1_PIIR                 VPlong(PWMM1_ADDRESS+0x40)

#define MFSEL2			VPlong((unsigned int)uipMFSEL2_VA)

#define CLK_VA			(unsigned int)uipCLK_VA
#define CLKEN			VPlong(CLK_VA+0x00)
#define CLKSEL			VPlong(CLK_VA+0x04)
#define CLKDIV			VPlong(CLK_VA+0x08)
#define PLLCON0			VPlong(CLK_VA+0x0C)
#define PLLCON1			VPlong(CLK_VA+0x10)

#define VPlong(x)		(*(unsigned int *)(x))
#define VPshort(x)              (*(unsigned short *)(x))
#define VPchar(x)               (*(unsigned char *)(x))
 
#define SET_BIT(reg, bit)       ((reg) |= (0x01<<(bit)))
#define READ_BIT(reg, bit)      ((reg) & (0x01<<(bit)))
#define CLEAR_BIT(reg, bit)     ((reg) &= (~(0x01<<(bit))))
#define IS_BIT_SET(reg, bit)    (((reg) & (0x01<<(bit)))!=0)

#define SET_MODE                0
#define SET_FREQUENCY           1
#define GET_FREQUENCY           2
#define SET_DUTY_CYCLE          3
#define GET_DUTY_CYCLE          4
#define CHANNEL_ENABLE          5
#define CHANNEL_DISABLE         6
#define PWM_DISABLE             7
#define SET_INVERTOR            8
#define GET_BASE_FREQ		9

#define ONE_SHOT_MODE           0
#define TOGGLE_MODE             1

#define PWM_SWITCH_EN           1
#define PWM_SWITCH_DISEN        0

typedef unsigned char      	BYTE;
typedef unsigned short int 	WORD;
typedef unsigned long int  	DWORD;

typedef struct PWM_Info
{
    BYTE b_Cmd;
    WORD w_CounterValue;
    WORD w_ComparatorValue;
    BYTE b_PreScaler;
    BYTE b_ClockDivider;
    BYTE b_ModuleAndChannelNum;
    BYTE b_Mode;
    BYTE b_InvertFlag;
    DWORD dw_BaseFreq;
}St_PWM_InfoTag;


ssize_t PWM_read(struct file *, char *, size_t, loff_t *);
ssize_t PWM_write(struct file *, const char *, size_t count, loff_t *);
int PWM_ioctl(struct inode *, struct file *, unsigned int, unsigned long);
int PWM_open(struct inode *, struct file *);
int FindAPBClock(void);
irqreturn_t PWMDrvIsr(int irq, void *dev_id, struct pt_regs *regs);


