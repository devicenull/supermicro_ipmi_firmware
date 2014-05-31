

#ifndef	_UTILEVTCMD_H_
#define	_UTILEVTCMD_H_

#include <status.h>
#include <type.h>

#ifdef _UTILEVTCMD_C_
STATUS EventProcInit(void);
STATUS UtilEvtClrEvtMsgBuff(void);
BOOLEAN UtilEvtEvtMsgBuffEnable(void);
BOOLEAN UtilEvtEvtMsgBuffFull(void);
BOOLEAN UtilEvtEvtMsgBuffIntEnable(void);
STATUS UtilEvtGetEvtMsgBuff(BYTE *pData, BYTE *pbLen);
STATUS UtilEvtGetRecver(BYTE *pbAddr, BYTE *pbLun);
STATUS UtilEvtMsgRecvEnable(BOOLEAN enable);
STATUS UtilEvtPostEvtMsg(BYTE *pDataSrc, BYTE bLen, ULONG ulWait);
STATUS UtilEvtPutToEvtMsgBuff(BYTE *pData, BYTE bLen);
STATUS UtilEvtSetEvtMsgBuffEnable(BOOLEAN enable);
STATUS UtilEvtSetEvtMsgBuffInt(BOOLEAN enable);
STATUS UtilEvtSetRecver(BYTE bAddr, BYTE bLun);
#else
extern STATUS EventProcInit(void);
extern STATUS UtilEvtClrEvtMsgBuff(void);
extern BOOLEAN UtilEvtEvtMsgBuffEnable(void);
extern BOOLEAN UtilEvtEvtMsgBuffFull(void);
extern BOOLEAN UtilEvtEvtMsgBuffIntEnable(void);
extern STATUS UtilEvtGetEvtMsgBuff(BYTE *pData, BYTE *pbLen);
extern STATUS UtilEvtGetRecver(BYTE *pbAddr, BYTE *pbLun);
extern STATUS UtilEvtMsgRecvEnable(BOOLEAN enable);
extern STATUS UtilEvtPostEvtMsg(BYTE *pDataSrc, BYTE bLen, ULONG ulWait);
extern STATUS UtilEvtPutToEvtMsgBuff(BYTE *pData, BYTE bLen);
extern STATUS UtilEvtSetEvtMsgBuffEnable(BOOLEAN enable);
extern STATUS UtilEvtSetEvtMsgBuffInt(BOOLEAN enable);
extern STATUS UtilEvtSetRecver(BYTE bAddr, BYTE bLun);
#endif 

#endif 

