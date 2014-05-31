
#ifndef _GLOBAL_H_
#define _GLOBAL_H_  



#ifdef _GLOBAL_C_


#else

extern CMD_STATUS GetDeviceIDCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ColdResetCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetSelfTestResultCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetDeviceGUIDCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetACPIPowerStateCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetACPIPowerStateCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

#endif   

#endif  





