
#ifndef	_WDTCMD_H_
#define	_WDTCMD_H_


#include <UtilSlaveTask.h>

#ifdef _WDTCMD_C_
void ApiWatchdogTimerFunc(DWORD dw_i);
void ApiWatchdogTimeoutFunc(DWORD dw_FuncID, Un_ParaTag *un_Param);
void ApiInitWatchdogTimerStruct(void);
void ApiSendLoggingEvent(BYTE b_wdt_event_data1, BYTE b_wdt_event_data2);
void ApiWatchdogTimeoutAction(DWORD dw_event,void *ptr);
BYTE ApiSetWatchdogTimer(MsgBufFmt *);
BYTE ApiGetWatchdogTimer(MsgBufFmt *);
BYTE ApiSetWatchdogSubFnCheck (MsgBufFmt *p_St_MsgBufIn);
CMD_STATUS ResetWatchdogTimerCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS SetWatchdogTimerCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetWatchdogTimerCmd(MsgBufFmt *, MsgBufFmt *);
#else
extern void ApiWatchdogTimerFunc(DWORD dw_i);
extern void ApiWatchdogTimeoutFunc(DWORD dw_FuncID, Un_ParaTag *un_Param);
extern void ApiInitWatchdogTimerStruct(void);
extern void ApiWatchdogTimeoutAction(DWORD dw_event,void *ptr);
extern BYTE ApiSetWatchdogTimer(MsgBufFmt *);
extern BYTE ApiGetWatchdogTimer(MsgBufFmt *);
extern BYTE ApiSetWatchdogSubFnCheck (MsgBufFmt *p_St_MsgBufIn);
extern CMD_STATUS ResetWatchdogTimerCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetWatchdogTimerCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetWatchdogTimerCmd(MsgBufFmt *, MsgBufFmt *);
#endif

typedef struct
{
	BYTE b_wdt_timer_use;
	BYTE b_wdt_timer_action;
	BYTE b_wdt_pre_timeout_interval;
	BYTE b_wdt_expiration_flag_clear;
	BYTE b_wdt_init_lscountdown;
	BYTE b_wdt_init_mscountdown;
} St_WdtDataTag;

#endif 






