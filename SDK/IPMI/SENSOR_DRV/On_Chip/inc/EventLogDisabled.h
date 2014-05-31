
      
#ifndef _EVENT_LOGGING_DISABLED_H_
#define _EVENT_LOGGING_DISABLED_H_

extern STATUS SetupEvtLogDisabledDrv(DWORD , DWORD, void *, void *);
extern STATUS PryEvtLogDisabledDrv(DWORD , DWORD, void *, void *);
extern STATUS WriteEvtLogDisabledDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadEvtLogDisabledDrv(DWORD , DWORD, void *, void *);

#ifdef _EVENT_LOGGING_DISABLED_C_

#define CORRECTABLE_MEMORY_ERROR_LOGGING_DISABLED	0x01
#define EVENT_TYPE_LOGGING_DISABLED					0x02
#define LOG_AREA_RESET_CLEARED						0x04
#define ALL_EVENT_LOGGING_DISABLED					0x08
#define SEL_FULL									0x10
#define SEL_ALMOST_FULL								0x20

extern WORD ws_w_NumEntries;
extern DWORD UtilNvramSelGetMaxSize(void);

St_SensorAPITag ws_St_EventLogDisabledAPI =
{
    SetupEvtLogDisabledDrv,
    PryEvtLogDisabledDrv,
    WriteEvtLogDisabledDrv,
    ReadEvtLogDisabledDrv
};

#else

extern St_SensorAPITag ws_St_EventLogDisabledAPI;

#endif  
#endif  





