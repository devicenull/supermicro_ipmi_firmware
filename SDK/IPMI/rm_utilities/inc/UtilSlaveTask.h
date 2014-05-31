

#ifndef _UTILSLAVETASK_H_
#define _UTILSLAVETASK_H_

#include <SlaveTaskTbl.h>

#define SLVMSG_HIGHEST (1L)
#define SLVMSG_NORMAL  (2L)
#define SLVMSG_LOWEST  (3L)

typedef struct {
    long       mtype;
    DWORD      dw_FuncId;
    Un_ParaTag un_Param;
} St_SlaveTaskMessageTag;

#ifdef _UTILSLAVETASK_C_
STATUS UtilGetMsgQ(void);
STATUS UtilReleaseMsgQ(void);
STATUS UtilSlaveTaskEventSet(DWORD dw_FuncID, void *ptr);
STATUS UtilSendSlaveTaskMSG(DWORD dw_FuncId, void *un_Param, BYTE b_prior);
STATUS UtilRecvSlaveTaskMsg(St_SlaveTaskMessageTag *St_TaskMessageBuf);
#else
extern STATUS UtilGetMsgQ(void);
extern STATUS UtilReleaseMsgQ(void);
extern STATUS UtilSlaveTaskEventSet(DWORD dw_FuncID, void *ptr);
extern STATUS UtilSendSlaveTaskMSG(DWORD dw_FuncId, void *un_Param, BYTE b_prior);
extern STATUS UtilRecvSlaveTaskMsg(St_SlaveTaskMessageTag *St_TaskMessageBuf);
#endif 

#endif 

