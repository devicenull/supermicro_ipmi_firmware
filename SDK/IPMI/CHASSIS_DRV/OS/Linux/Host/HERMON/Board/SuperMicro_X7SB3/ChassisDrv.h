


#ifndef _AST_CHASSISDRV_H_
#define _AST_CHASSISDRV_H_

extern void   PltCHASSIS_PowerReset(void);
extern void   PltCHASSIS_PowerDown(void);
extern void   PltCHASSIS_PowerCycle(BYTE);
extern void   Plt_WDT_Reset(void);
extern void   Plt_WDT_Stop(void);
extern void   Plt_WDT_Expire(void);
extern void   utilSetupAutoARP(void);
extern int	  SensorScanDetect(void);
/* Tony, 12/24/2012 add bbp timeout when all power supply are failed { */	
extern void SetPowerFail(unsigned char enable);
/* Tony, 12/24/2012 add bbp timeout when all power supply are failed } */	
#endif

