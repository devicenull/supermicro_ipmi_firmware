

#ifndef I2C_USERAP_H
#define I2C_USERAP_H

#include <linux/ioctl.h>

#define BUFFER_SIZE 256



struct i2cCtlInfo_t {
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



#define IPMB_IOC_MAGIC  'i'

#define IPMB_IOCSCONF   _IOW(IPMB_IOC_MAGIC,  1, struct i2cCtlInfo_t)
#define IPMB_IOCXMSG    _IOWR(IPMB_IOC_MAGIC, 2, struct i2cCtlInfo_t)
#define IPMB_IOCSTART   _IOW(IPMB_IOC_MAGIC, 3, struct i2cCtlInfo_t)
#define IPMB_IOCSTOP    _IOW(IPMB_IOC_MAGIC, 4, struct i2cCtlInfo_t)

#define IPMB_IOC_MAXNR 4

#ifndef _I2C_DRV_H_
typedef enum
{
	I2C_OK,
	I2C_TX_OK,
	I2C_RX_OK,
	I2C_NACK,
	I2C_AL,
	I2C_BUSY
} En_I2CSTATUSTag;
#endif

#ifndef _TYPE_H_
#define _TYPE_H_


#ifndef     NULL
#define     NULL             0
#endif



typedef unsigned char       BYTE;
typedef unsigned short int  WORD;
typedef unsigned long int   DWORD;



typedef enum
{
    FALSE = 0,
    TRUE
} BOOLEAN;

typedef enum
{
	STATUS_FAIL = 0,
    STATUS_SUCCESS = 1,      
    STATUS_READY,
    STATUS_SUSPEND,
    STATUS_TIMEOUT,
    STATUS_BUSY,
    STATUS_NAK,             
    STATUS_AL,              
    STATUS_TR               
} STATUS;


#define AST_I2C_CHANNELS   7
#define DEF_TIMEOUT   20
#define HI_LO_PERIOD  			50

typedef enum
{
	AST_COMPELETE,
	AST_SENDING,
	AST_RECEIVING,
	AST_NAK,
	AST_AL,
	BUS_ERROR,

	I2C_RETRY,
	AST_IDLE,
	AST_POWERUP,
	AST_TIME_OUT
} En_ASTI2CRESULTTag;


typedef struct
{
	unsigned char b_channel;
	unsigned char b_slaveaddress;
	unsigned char b_writebytecount1;
	unsigned char b_writebytecount2;
	unsigned char b_readbytecount;
	unsigned char *p_b_writebuf1;
	unsigned char *p_b_writebuf2;
	unsigned char *p_b_readbuf;
	unsigned char b_MUXInfo;
} St_I2CFrameTag;

#define OFFSET_FUNCTRL  0x00
#define OFFSET_CLKCTRL1 0x04
#define OFFSET_CLKCTRL2 0x08
#define OFFSET_INTRCTRL 0x0C
#define OFFSET_INTRST   0x10
#define OFFSET_CMDST    0x14
#define OFFSET_DEVADDR  0x18
#define OFFSET_BUFCTRL  0x1C
#define OFFSET_TXRXBuf  0x20
#define OFFSET_DMACTRL  0x24
#define OFFSET_DMAST    0x28


#define MASTER_FUNC_ENABLE		0x01
#define SLAVE_FUNC_ENABLE		0x02

#define I2C_TRANSMIT		1
#define I2C_RECEIVE			0

#define AL              (1<<3)
#define BB              (1<<16)
#define TNACK	        (1<<1)

#define I2CBASEADDR 0x1E78A000
#define I2CINT 12

#define I2C_TRANS_ACK         0x0001
#define I2C_TRANS_NACK        0x0002
#define I2C_RECV_DONE         0x0004
#define I2C_MASTER_ARB_LOS    0x0008
#define I2C_NORMAL_STOP       0x0010
#define I2C_ABNORMAL          0x0020
#define I2C_SCL_LOW_TIMEOUT   0x0040

#define SDA_ACTIVE_PULL_HIGH_ENABLE			0x00000100
#define SCL_ACTIVE_PULL_HIGH_ENABLE			0x00000080

extern STATUS I2CInit(void);
#endif  
extern En_I2CSTATUSTag PltI2C_ActiveWriteRead(MsgBufFmt *MsgBufIn, MsgBufFmt *MsgBufOut);


#endif 



