
#ifndef _MESSAGE_H_
#define _MESSAGE_H_


#define CURRENT_CHANNEL                                 0x0E

#define DISABLE_CHANNEL                                 0
#define ENABLE_CHANNEL                                  1
#define GET_CHANNEL_STATE                               2
#define RESERVED_STATE                                  3

#define NO_TRACKING                                     0
#define TRACKING                                        1
#define SEND_RAW                                        2

#define NO_SEND                                         0
#define IPMB_SEND                                       8
#define LAN_SEND                                        9
#define PENDING_BRIDGED_RESPONSE_TABLE_SIZE             2

#define MESSGSA_RESPONSE                                0xFF

#define COMMAND_RESERVED                                0xFF



typedef struct
{
    BYTE b_EnOrDisable;
    BYTE b_RcvMsgQISROnOrOff;
}St_MsgCmdTag;



typedef struct
{
    BYTE b_Using;
    BYTE b_Timer;
    BYTE b_Rsp;
    
	BYTE b_AuthType;
	BYTE b_Pri;
	
    BYTE b_rsAddr;
    BYTE b_netFnrsLun;
    BYTE b_rqAddr;
    BYTE b_rqSeqrqLUN;
    BYTE b_datalength;
    BYTE b_cmd;
    BYTE b_data[32];

  	BYTE b_SourceIP[4];

   	BYTE b_SourcePort[2];

   	BYTE b_SourceChannel;
    
	DWORD dw_SessSeqNum;
	DWORD dw_SessID;
	
	DWORD *p_dw_MsgPtr;
}St_PendingBrgReqTableTag;


#ifndef _MESSAGE_C_

extern CMD_STATUS EnableMsgChnRcvCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetMessageCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SendMessageCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetBmcGlobalEnableCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetBmcGlobalEnableCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ReadEvtMsgBuffCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetSystemGUIDCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetSystemGUIDCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ClearMsgFlagCmd(MsgBufFmt *MsgBufIn, MsgBufFmt *MsgBufOut);
extern CMD_STATUS GetMsgFlagCmd(MsgBufFmt *MsgBufIn, MsgBufFmt *MsgBufOut);
extern STATUS CtlRcvMsgQISR(BYTE , void *);
extern STATUS RdRcvMsgQISR(BYTE , void *);

#else

STATUS MsgSendOut(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
STATUS BrgRsTblEncode(MsgBufFmt *, St_PendingBrgReqTableTag *);
STATUS BrgRsTblDecode(St_PendingBrgReqTableTag *, MsgBufFmt *);
STATUS MessageEncode(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
STATUS MessageDecode(BYTE, MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
void MsgEvtProcess(DWORD);
void MSGTimerFunc(DWORD);
BYTE ClearMsgFlag(MsgBufFmt *MsgBufIn);
BYTE GetMsgFlag(MsgBufFmt *MsgBufIn);
BYTE SetBmcGEnable(BYTE bSetting);
BYTE GetBmcGEnable(BYTE *pbSetting);

#endif		

#endif      




