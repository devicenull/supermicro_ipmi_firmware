#ifndef __PECI_DRV_H_
#define __PECI_DRV_H_

#define	HN_PECI_MAJOR 		254	
#define HN_PECI_IRQ			6		
#define PECI_MODULE_NAME 	"hermon_peci"


#define MAX_DATA_BUF_SZ		16

#define PECI_PHY_ADDR		0xB8000200		
#define REG(x, shift_num)	((*(volatile unsigned char *)(x + shift_num)))

#define PECI_CTL_STS		REG(PECIBA,0x00)		
#define PECI_RD_LEN			REG(PECIBA,0x04)		
#define PECI_ADDR			REG(PECIBA,0x08)		
#define PECI_CMD			REG(PECIBA,0x0C)		
#define PECI_WR_LEN			REG(PECIBA,0x1C)		
#define PECI_DAT_INOUT(x)	REG(PECIBA,0x40+4*x)	

#define PECI_INT_EN			(0x1<<6)
#define ST_ABORT_ERR		(0x1<<4)
#define ST_CRC_ERR			(0x1<<3)
#define ST_DONE				(0x1<<1)
#define ST_BUSY				(0x1<<0)

#define IOCTL_WRITE_READ	0xFFF6

typedef struct _peci_msg{
	unsigned char addr;		
	unsigned int write_len;	
	unsigned int read_len;	
	unsigned char cmd;		
	unsigned char *data;	
}PECI_MSG_TAG;



#endif
