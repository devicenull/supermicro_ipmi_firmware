

#ifndef _LM95231DRV_H_
#define _LM95231DRV_H_

#define LM95231_CONFIGURATION_AMONT   1
#define LM95231_CONFIGURATION_REG     0x03 

typedef struct
{
    BYTE b_RegAddr;
    BYTE b_Value;
} St_LM95231ConfigureTypeTag;

extern STATUS SetupLM95231Drv(DWORD , DWORD, void *, void *);
extern STATUS PryLM95231Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteLM95231Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadLM95231Drv(DWORD , DWORD, void *, void *);

#ifdef _LM95231DRV_C_

St_SensorAPITag ws_St_LM95231API =
{
    SetupLM95231Drv,
    PryLM95231Drv,
    WriteLM95231Drv,
    ReadLM95231Drv
};

#else

extern St_SensorAPITag ws_St_LM95231API;

#endif  
#endif  





