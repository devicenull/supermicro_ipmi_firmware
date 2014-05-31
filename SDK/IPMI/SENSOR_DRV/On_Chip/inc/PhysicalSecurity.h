      
#ifndef _PHYSICAL_SECURITY_H_
#define _PHYSICAL_SECURITY_H_

#define GENERAL_CHASSIS_INTRUSION                                       0x00

#define DEVICE_ABSENT                                                   0x00
#define DEVICE_PRESENT                                                  0x01
#define EVENT_READING_CODE_UNSPECIFIED                                  0x0F


extern STATUS SetupPhcScrDrv(DWORD , DWORD, void *, void *);
extern STATUS PryPhcScrDrv(DWORD , DWORD, void *, void *);
extern STATUS WritePhcScrDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadPhcScrDrv(DWORD , DWORD, void *, void *);

#ifdef _PHYSICAL_SECURITY_C_

St_SensorAPITag ws_St_PhysicalSecurityAPI =
{
    SetupPhcScrDrv,
    PryPhcScrDrv,
    WritePhcScrDrv,
    ReadPhcScrDrv
};

extern STATUS PltSetupPhcScrDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltPryPhcScrDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltWritePhcScrDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltReadPhcScrDrv(DWORD, DWORD, void  *, void  *);

#else

extern St_SensorAPITag ws_St_PhysicalSecurityAPI;

#endif  
#endif  





