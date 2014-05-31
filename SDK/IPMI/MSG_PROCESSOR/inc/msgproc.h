#ifndef _MSGPROC_H_
#define _MSGPROC_H_

#ifdef _MSGPROC_C_
void MsgProcesserTask(DWORD i);
TX_MUTEX ws_IPMICmdBufMTX;
#else
extern void MsgProcInit(void);
extern MsgBlkStruct  *MsgProcQExtract(void);
extern void MsgProcQInsert(MsgBlkStruct  *MsgBuf);
extern TX_MUTEX ws_IPMICmdBufMTX;
#endif  




#endif  





