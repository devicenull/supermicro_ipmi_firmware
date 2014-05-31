
#ifndef _SYSTEMTIMER_H_
#define _SYSTEMTIMER_H_


#ifdef _SYSTEMTIMER_C_

STATUS SystemTimerInit(void );
void SystemTimer(DWORD dw_dummy);
void SetTime(DWORD dw_time);
DWORD GetTime(void);

#else

extern STATUS SystemTimerInit(void );
extern void SetTime(DWORD dw_time);
extern DWORD GetTime(void);

#endif
#endif




