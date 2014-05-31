

#ifndef	_UTILRESETIP_H_
#define	_UTILRESETIP_H_

#include "./../../COMMON/inc/status.h"
#include "./../../COMMON/inc/type.h"

#ifdef _UTILRESETIP_C_

BYTE   UtilResetIP(void);
#else

extern BYTE   UtilResetIP(void);
#endif 

#endif 

