

#ifndef _UTILCHASSISCMD_H_
#define _UTILCHASSISCMD_H_

#ifdef _UTILCHASSISCMD_C_

BYTE UtilGetNowPowerStatus(void);
BYTE UtilGetPowerStatus(void);
BYTE UtilPowerUp(void);
BYTE UtilPowerDown(void);
BYTE UtilPowerReset(void);
void UtilSetPowerRestorePolicy(BYTE b_Policy);
STATUS POHTimerInit(void);
void POHTimerFunc(DWORD dw_Info);

#else

#ifdef __cplusplus
extern "C"{
#endif
extern BYTE UtilGetNowPowerStatus(void);
extern BYTE UtilGetPowerStatus(void);
extern BYTE UtilPowerUp(void);
extern BYTE UtilPowerDown(void);
extern BYTE UtilSoftPowerDown(void);
extern BYTE UtilPowerReset(void);
#ifdef __cplusplus
}
#endif

extern void UtilSetPowerRestorePolicy(BYTE b_Policy);
extern STATUS POHTimerInit(void);
extern void POHTimerFunc(DWORD dw_Info);

#endif

#endif

