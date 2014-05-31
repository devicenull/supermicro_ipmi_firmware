

#ifndef _HERMON_I2C_H_
#define _HERMON_I2C_H_

#include <linux/ioctl.h> 
#include <asm/io.h>
#include <asm/hardware.h>

#define HERMON_I2C_MAJOR 	253
#define HERMON_I2C_MINOR	0
#define HERMON_I2C_NR_DEVS 	6    

#define HERMON_IRQ_012  26
#define HERMON_IRQ_3    23
#define HERMON_IRQ_4    27
#define HERMON_IRQ_5    29

#define IPMB_MIN_MSG_SIZE	0x06
#define MAX_BUF_SIZE		256

typedef union
{
    unsigned char DWORD;
} Un_SMBnSDARegTag;

typedef union
{
    unsigned char DWORD;
} Un_SMBnSTRegTag;

typedef union
{
    unsigned char DWORD;

} Un_SMBnCSTRegTag;

typedef union
{
    unsigned char DWORD;
} Un_SMBnCTL1RegTag;

typedef union
{
    unsigned char DWORD;

} Un_SMBnADDR1RegTag;

typedef union
{
    unsigned char DWORD;

} Un_SMBnCTL2RegTag;

typedef union
{
    unsigned char DWORD;

} Un_SMBnADDR2RegTag;

typedef union
{
    unsigned char DWORD;

} Un_SMBnCTL3RegTag;


typedef struct
{
    Un_SMBnSDARegTag            *IIC_SMBnSDAR;
    Un_SMBnSTRegTag             *IIC_SMBnSTR;
    Un_SMBnCSTRegTag            *IIC_SMBnCSTR;
    Un_SMBnCTL1RegTag           *IIC_SMBnCTL1R;
    Un_SMBnADDR1RegTag          *IIC_SMBnADDR1R;
    Un_SMBnCTL2RegTag           *IIC_SMBnCTL2R;
    Un_SMBnADDR2RegTag          *IIC_SMBnADDR2R;
    Un_SMBnCTL3RegTag           *IIC_SMBnCTL3R;
} St_HER_I2CRegTag;

#define OFFSET_SMBnSDA      0x00
#define OFFSET_SMBnST       0x02
#define OFFSET_SMBnCST      0x04
#define OFFSET_SMBnCTL1     0x06
#define OFFSET_SMBnADDR1    0x08
#define OFFSET_SMBnCTL2     0x0A
#define OFFSET_SMBnADDR2    0x0C
#define OFFSET_SMBnCTL3     0x0E


#define I2C_IDLE				0x00
#define I2C_REQUEST_MASTER		0x01
#define I2C_MASTER_SEND_ADDR	0x02
#define I2C_MASTER_TRANSMIT		0x03
#define I2C_MASTER_REPEAT_START	0x04
#define I2C_MASTER_RECEIVE		0x05
#define I2C_MASTER_STOP			0x06
#define I2C_ERROR				0x07
#define I2C_NACK				0x08
#define I2C_IPMB_START_RECEIVE	0x09
#define I2C_IPMB_END_RECEIVE	0x10

#define HER_SLVSTP          0x80
#define HER_SDAST           0x40
#define HER_BER             0x20
#define HER_NEGACK 			0x10
#define HER_STASTR 			0x08
#define HER_NMATCH          0x04
#define HER_MASTER          0x02
#define HER_XMIT            0x01

#define HER_STASTRE			0x80
#define HER_NMINTE			0x40
#define HER_GCMEN			0x20
#define HER_ACK				0x10
#define HER_INTEN			0x04
#define HER_STOP			0x02
#define HER_START			0x01

#define HER_SAEN			0x80

#define ENABLE				0x01

#define ARPMEN				0x04


#define REG1(reg)   ((volatile unsigned char)(reg))

#define RegSMBnSDA(x)   REG1(at_a_St_I2CChannel[x].IIC_SMBnSDAR->DWORD)     
#define RegSMBnST(x)    REG1(at_a_St_I2CChannel[x].IIC_SMBnSTR->DWORD)      
#define RegSMBnCST(x)   REG1(at_a_St_I2CChannel[x].IIC_SMBnCSTR->DWORD)     
#define RegSMBnCTL1(x)  REG1(at_a_St_I2CChannel[x].IIC_SMBnCTL1R->DWORD)    
#define RegSMBnADDR1(x) REG1(at_a_St_I2CChannel[x].IIC_SMBnADDR1R->DWORD)   
#define RegSMBnCTL2(x)  REG1(at_a_St_I2CChannel[x].IIC_SMBnCTL2R->DWORD)    
#define RegSMBnADDR2(x) REG1(at_a_St_I2CChannel[x].IIC_SMBnADDR2R->DWORD)   
#define RegSMBnCTL3(x)  REG1(at_a_St_I2CChannel[x].IIC_SMBnCTL3R->DWORD)    

typedef enum {
    MASTER_MODE = 0,
    SLAVE_MODE,
    IPMB_MODE
} En_I2CModeTag;

enum {
	BUSERROR = 1,
	BUSBUSY ,
	SLAVENACK ,
	TXERROR ,
	RXERROR ,
	SDANOTREADY ,
	ABRILOST
};

struct hermon_i2c_dev 
{
    int dev_index ;
    struct cdev cdev;           
    struct semaphore sem;       
    wait_queue_head_t waitq;
    int isr_flag;
    St_HER_I2CRegTag at_a_St_I2CChannel;
    int at_a_dw_bus_error;
    int at_a_normal_stop;
    int at_a_dw_i2c_pending;    
    int at_a_dw_arbit_lost;
    int at_a_tx_finished;
    int at_a_rx_finished;
    int at_a_stastr;
    int at_a_negack;
    int at_a_sdast;
    int at_errorno;
    char txb[MAX_BUF_SIZE];
    char rxb[MAX_BUF_SIZE];
    En_I2CModeTag En_mode;
	void (*callback_func)(unsigned char *, int, int);
	int write_count;    
	char current_tx_count;
	char current_rx_count;
	char target_tx_count;
	char target_rx_count;
	char b_Slaveaddr;
	char b_State;
	int at_b_HangRead;
	int at_b_I2CInitFlag;
};


int     hermon_i2c_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);

struct i2cCtlInfo_t 
{
    char b_Channel;
    char b_waittype;
    char b_Ctrl;
    char b_Type;
    char b_Slaveaddr;
    char result;
    char ret_data;
    char rx_len;
    char tx_len;
    char *rx_buf;
    char *tx_buf;
};



#define HERMON_I2C_IOC_MAGIC  'k'
#define HERMON_I2C_IOCWBYTE    _IOW(HERMON_I2C_IOC_MAGIC,   1, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCRBYTE    _IOWR(HERMON_I2C_IOC_MAGIC,   2, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCXDATA    _IOW(HERMON_I2C_IOC_MAGIC,  3, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCSTART    _IOW(HERMON_I2C_IOC_MAGIC,   4, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCREPSTART _IOW(HERMON_I2C_IOC_MAGIC,   5, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCSTOP     _IOW(HERMON_I2C_IOC_MAGIC,   6, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCRESET    _IOW(HERMON_I2C_IOC_MAGIC,   7, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCWTINT    _IOWR(HERMON_I2C_IOC_MAGIC,  8, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCWTBUSNB  _IOWR(HERMON_I2C_IOC_MAGIC,  9, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCWTSTOP   _IOWR(HERMON_I2C_IOC_MAGIC, 10, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCASMASTER _IOW(HERMON_I2C_IOC_MAGIC,  11, struct i2cCtlInfo_t)
#define HERMON_I2C_IOCASSLAVE  _IOW(HERMON_I2C_IOC_MAGIC,  12, struct i2cCtlInfo_t)

#define HERMON_I2C_IOC_MAXNR 12


const St_HER_I2CRegTag at_a_St_I2CChannel[HERMON_I2C_NR_DEVS] =
{
    {
        (Un_SMBnSDARegTag   *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnSDA),
        (Un_SMBnSTRegTag    *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnST),
        (Un_SMBnCSTRegTag   *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnCST),
        (Un_SMBnCTL1RegTag  *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnCTL1),
        (Un_SMBnADDR1RegTag *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnADDR1),
        (Un_SMBnCTL2RegTag  *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnCTL2),
        (Un_SMBnADDR2RegTag *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnADDR2),
        (Un_SMBnCTL3RegTag  *)(WPCM450_VA_B800 + 0x6000 + OFFSET_SMBnCTL3),
    },
    {
        (Un_SMBnSDARegTag   *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnSDA),
        (Un_SMBnSTRegTag    *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnST),
        (Un_SMBnCSTRegTag   *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnCST),
        (Un_SMBnCTL1RegTag  *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnCTL1),
        (Un_SMBnADDR1RegTag *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnADDR1),
        (Un_SMBnCTL2RegTag  *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnCTL2),
        (Un_SMBnADDR2RegTag *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnADDR2),
        (Un_SMBnCTL3RegTag  *)(WPCM450_VA_B800 + 0x6100 + OFFSET_SMBnCTL3),
    },
    {
        (Un_SMBnSDARegTag   *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnSDA),
        (Un_SMBnSTRegTag    *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnST),
        (Un_SMBnCSTRegTag   *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnCST),
        (Un_SMBnCTL1RegTag  *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnCTL1),
        (Un_SMBnADDR1RegTag *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnADDR1),
        (Un_SMBnCTL2RegTag  *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnCTL2),
        (Un_SMBnADDR2RegTag *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnADDR2),
        (Un_SMBnCTL3RegTag  *)(WPCM450_VA_B800 + 0x6200 + OFFSET_SMBnCTL3),
    },
    {
        (Un_SMBnSDARegTag   *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnSDA),
        (Un_SMBnSTRegTag    *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnST),
        (Un_SMBnCSTRegTag   *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnCST),
        (Un_SMBnCTL1RegTag  *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnCTL1),
        (Un_SMBnADDR1RegTag *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnADDR1),
        (Un_SMBnCTL2RegTag  *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnCTL2),
        (Un_SMBnADDR2RegTag *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnADDR2),
        (Un_SMBnCTL3RegTag  *)(WPCM450_VA_B800 + 0x6300 + OFFSET_SMBnCTL3),
    },
    {
        (Un_SMBnSDARegTag   *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnSDA),
        (Un_SMBnSTRegTag    *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnST),
        (Un_SMBnCSTRegTag   *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnCST),
        (Un_SMBnCTL1RegTag  *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnCTL1),
        (Un_SMBnADDR1RegTag *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnADDR1),
        (Un_SMBnCTL2RegTag  *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnCTL2),
        (Un_SMBnADDR2RegTag *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnADDR2),
        (Un_SMBnCTL3RegTag  *)(WPCM450_VA_B800 + 0x6400 + OFFSET_SMBnCTL3),
    },
    {
        (Un_SMBnSDARegTag   *)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnSDA),
        (Un_SMBnSTRegTag    *)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnST),
        (Un_SMBnCSTRegTag   *)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnCST),
        (Un_SMBnCTL1RegTag  *)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnCTL1),
        (Un_SMBnADDR1RegTag	*)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnADDR1),
        (Un_SMBnCTL2RegTag  *)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnCTL2),
        (Un_SMBnADDR2RegTag	*)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnADDR2),
        (Un_SMBnCTL3RegTag  *)(WPCM450_VA_B800 + 0x6500 + OFFSET_SMBnCTL3),
    },
};

#endif 


