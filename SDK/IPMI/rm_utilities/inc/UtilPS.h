
#ifndef _UTILPS_H_
#define _UTILPS_H_

#include "./../../COMMON/inc/status.h"

#ifdef _UTILPS_C_
STATUS UtilEnterPSCriticalSection(void);
STATUS UtilLeavePSCriticalSection(void);
void   UtilPSSOLCommit(void);
void   UtilPSSerialCommit(void);
#else
extern STATUS UtilEnterPSCriticalSection(void);
extern STATUS UtilLeavePSCriticalSection(void);
extern void   UtilPSSOLCommit(void);
extern void   UtilPSSerialCommit(void);
#endif 

#endif 

