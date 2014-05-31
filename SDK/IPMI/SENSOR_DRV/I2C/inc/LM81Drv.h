     
#ifndef _LM81DRV_H_
#define _LM81DRV_H_

#define LM81_CONFIGURATION_AMONT   2
#define LM81_CONFIGURATION_REG     0x40 


typedef struct
{
    BYTE b_RegAddr;
    BYTE b_Value;
} St_LM81ConfigureTypeTag;


extern STATUS SetupLM81Drv(DWORD , DWORD, void *, void *);
extern STATUS PryLM81Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteLM81Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadLM81Drv(DWORD , DWORD, void *, void *);

#ifdef _LM81DRV_C_

St_SensorAPITag ws_St_LM81API =
{
    SetupLM81Drv,
    PryLM81Drv,
    WriteLM81Drv,
    ReadLM81Drv
};

#else

extern St_SensorAPITag ws_St_LM81API;

#endif  
#endif  





