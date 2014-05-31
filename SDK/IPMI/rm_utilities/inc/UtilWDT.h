

#ifndef	_UTILWDT_H_
#define	_UTILWDT_H_

#include "./../../COMMON/inc/status.h"
#include "./../../COMMON/inc/type.h"

#ifdef _UTILWDT_C_
STATUS InitWatchdog(void);
STATUS UtilEnterWDTMutex(void);
STATUS UtilExitWDTMutex(void);
STATUS UtilStopWDT(void);
BYTE   UtilSetWDTInitCountdownValue(BYTE b_lscountdown, BYTE b_mscountdown);
BYTE   UtilKeepWDTAlive(void);
BYTE   UtilResetWDT(void);
#else
extern STATUS InitWatchdog(void);
extern STATUS UtilEnterWDTMutex(void);
extern STATUS UtilExitWDTMutex(void);
extern STATUS UtilStopWDT(void);
extern BYTE   UtilSetWDTInitCountdownValue(BYTE b_lscountdown, BYTE b_mscountdown);
extern BYTE   UtilKeepWDTAlive(void);
extern BYTE   UtilResetWDT(void);
#endif 

#endif 

