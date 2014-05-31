

#ifndef _UTILSOL_H_
#define _UTILSOL_H_

#include "./../../COMMON/inc/status.h"

#ifdef _UTILSOL_C_
STATUS UtilGetSOLSema(void);
STATUS UtilPutSOLSema(void);
#else
extern STATUS UtilGetSOLSema(void);
extern STATUS UtilPutSOLSema(void);
#endif 

#endif 


