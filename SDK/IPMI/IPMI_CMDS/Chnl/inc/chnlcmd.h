
#ifndef _CHNLCMD_H_
#define _CHNLCMD_H_  


#define CHNL_PRI_RES	    0
#define CHNL_PRI_CALLBACK	1
#define CHNL_PRI_USER		2
#define CHNL_PRI_OPERATOR	3
#define CHNL_PRI_ADMIN		4
#define CHNL_PRI_OEM		5

typedef struct
{
	BYTE b_ChnlAccess;
	BYTE b_PriLimit;
} St_ChnlAccessTag;

#ifndef _CHNLCMD_C_

extern St_ChnlAccessTag ws_a_St_ChnlAccessTbl[16];

extern CMD_STATUS SetChnlAccessCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetChnlAccessCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetChnlAuthCapCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetChnlInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

extern void UtilLoadChannelInfoDefault2PS(void);
extern void UtilChannelInfoVolatileInit(void);

extern BYTE ApiSetChannelAccessSubFnCheck(MsgBufFmt *p_St_MsgBufIn);

#else


#endif




#endif  





