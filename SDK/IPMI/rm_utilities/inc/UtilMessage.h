
#ifndef _MESSAGE_UTIL_H_
#define _MESSAGE_UTIL_H_



#ifdef _MESSAGE_UTIL_C_

#else
BYTE UtilGetMessageQueue(DWORD *p_dw_Enqueued);
void UtilGetMsgInit(void);
void UtilGetSystemGUID(BYTE *p_b_Data);
void UtilSetSystemGUID(BYTE *p_b_Data, BYTE b_length);

#endif		

#endif      




