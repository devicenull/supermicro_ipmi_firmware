
#ifndef _ADM1178DRV_H_
#define _ADM1178DRV_H_


extern STATUS SetupADM1178Drv(DWORD , DWORD, void *, void *);
extern STATUS PryADM1178Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteADM1178Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadADM1178Drv(DWORD , DWORD, void *, void *);

#ifdef _ADM1178DRV_C_

St_SensorAPITag ws_St_ADM1178API =
{
    SetupADM1178Drv,
    PryADM1178Drv,
    WriteADM1178Drv,
    ReadADM1178Drv
};

#else

extern St_SensorAPITag ws_St_ADM1178API;

#endif  
#endif  




