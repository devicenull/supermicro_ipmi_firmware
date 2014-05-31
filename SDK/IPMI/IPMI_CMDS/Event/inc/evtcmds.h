
#ifndef _EVTCMDS_H_
#define _EVTCMDS_H_


#define EVTCMD_FUNC_OK			0
#define EVTCMD_FUNC_FAIL		1



#ifdef _EVTCMDS_C_

CMD_STATUS SetEvtReceiverCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
CMD_STATUS GetEvtReceiverCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
CMD_STATUS EvtMsgCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

#else

extern CMD_STATUS SetEvtReceiverCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetEvtReceiverCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS EvtMsgCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

#endif 
#endif 





