
#ifndef _RESPMATCH_H_
#define _RESPMATCH_H_




#ifdef _RESPMATCH_C_


#else
extern STATUS MsgRespMatch(
			       		    MsgBufFmt *p_St_RqMsgBuf,
			       		    MsgBufFmt *p_St_RsMsgBuf,
			       		    WORD w_WaitTime,
			       		    BYTE b_type
			       	      );

extern void MsgRespMatchInit(void);

extern void AddRespMsgList(
		        			MsgBlkStruct *p_St_RspMsgBlk,
				        	DWORD dw_ExpireTime
				          );
#endif   

#endif  





