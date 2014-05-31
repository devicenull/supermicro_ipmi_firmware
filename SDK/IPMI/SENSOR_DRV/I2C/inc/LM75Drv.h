
#ifndef _LM75DRV_H_
#define _LM75DRV_H_



typedef struct
{
    BYTE b_RegAddr;
    BYTE b_Value;
} St_LM75ConfigureTypeTag;


extern STATUS SetupLM75Drv(DWORD , DWORD, void *, void *);
extern STATUS PryLM75Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteLM75Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadLM75Drv(DWORD , DWORD, void *, void *);

#ifdef _LM75DRV_C_

St_SensorAPITag ws_St_LM75API =
{
    SetupLM75Drv,
    PryLM75Drv,
    WriteLM75Drv,
    ReadLM75Drv
};

#else

extern St_SensorAPITag ws_St_LM75API;

#endif  
#endif  





