#ifndef _I2C_DRV_H_
#define _I2C_DRV_H_  


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


typedef enum
{
	I2C_OK,
	I2C_TX_OK,
	I2C_RX_OK,
	I2C_NACK,
	I2C_AL,
	I2C_BUSY
} En_I2CSTATUSTag;

extern unsigned char Plt_I2CMUXDrv(void *, St_I2CFrameTag);

#ifndef _I2C_DRV_C_

extern En_I2CSTATUSTag PltI2C_MasterWriteRead(St_I2CFrameTag);

extern STATUS PltI2C_ChannelInit(unsigned char);
extern void I2CMuxInit(void);
extern void PltI2C_SetupFrq(unsigned char, unsigned char);
extern void PltI2C_SMBInit(unsigned char, unsigned char, unsigned char);
extern STATUS PltI2C_Check(BYTE);
extern STATUS PltI2C_Resume(BYTE);
extern STATUS PI2CKidnapCheck(DWORD);
extern En_I2CSTATUSTag PTI2C_MasterWriteRead(St_I2CFrameTag);
extern unsigned char MUXI2CStart(unsigned char);
void MUXI2CStop(unsigned char);
extern unsigned char OEMI2C_MasterWriteRead(St_I2CFrameTag);
extern unsigned char OEMPltI2C_ChannelInit(unsigned char ChannelNo);

#endif


#endif  





