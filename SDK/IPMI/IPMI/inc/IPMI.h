
#ifndef _IPMI_H_
#define _IPMI_H_

#define NET_FN_CHASSIS          0x00
#define NET_FN_BRIDGE           0x02
#define NET_FN_SE               0x04 
#define NET_FN_APP              0x06
#define NET_FN_FW               0x08
#define NET_FN_STORAGE          0x0A 
#define NET_FN_TRANSPORT        0x0C

#define NET_FN_OEM_GROUP		0x2E
#define NET_FN_GROUP_EX			0x2C
#define NET_FN_OEM_WST          0x3E
#define NET_FN_OEM_WST1			0x30

#ifdef OPEN_BLADE
#define NET_FN_GROUP_EXTENSION	0x2C	
#endif

#define PRIV_CALLBACK   0x01
#define PRIV_USER       0x02
#define PRIV_OPERATOR   0x03
#define PRIV_ADMIN      0x04
#define PRIV_SYS_IF     0x05
#define PRIV_ANY        0x00
#define PRIV_RESERVED   0xFF  
#define PRIV_IPMI_MSG_DISABLE	0x08

#define CmdNotSupport   0xFF

#define DCMI_GRP_CODE	0xDC
#define ACTA_GRP_CODE	0x02
typedef struct {
    unsigned char Priv;
    CMD_STATUS (*IPMICmd) (MsgBufFmt * MsgBufIn, MsgBufFmt * MsgBufOut);
    BYTE (*SubFnCheck) (MsgBufFmt * MsgBufIn);
} IPMIPrivCmdStru;

typedef struct {
	unsigned char NetFn;
	unsigned char Offset;
    unsigned char TblSize;		
} IPMITabStru;

typedef struct {
    unsigned char NetFunction;
    unsigned char LUN;
    unsigned char CMD;
    unsigned char Priv;
    CMD_STATUS (*IPMICmd) (MsgBufFmt * MsgBufIn, MsgBufFmt * MsgBufOut);
} IPMICmdStru;


#ifdef _IPMI_C_

void IPMIInit(void);
void IPMIentry(MsgBufFmt * MsgBufIn, MsgBufFmt * MsgBufOut);
CMD_STATUS IPMItablelookup(MsgBufFmt * MsgBufIn, MsgBufFmt * MsgBufOut);


#else
extern void IPMIentry(MsgBufFmt * MsgBufIn, MsgBufFmt * MsgBufOut);
extern CMD_STATUS IPMItablelookup(MsgBufFmt * MsgBufIn, MsgBufFmt * MsgBufOut);
#endif
#endif





 
