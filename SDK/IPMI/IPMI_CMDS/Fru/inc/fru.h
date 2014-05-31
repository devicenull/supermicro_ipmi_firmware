     
#ifndef _FRU_H_
#define _FRU_H_  



#ifndef _FRU_C_

extern BYTE ApiCheckFRUIDMatchSDR(BYTE b_FruID);

extern CMD_STATUS GetFRUInvAreaInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ReadFRUDataCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS WriteFRUDataCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

#else

#endif

#endif  





