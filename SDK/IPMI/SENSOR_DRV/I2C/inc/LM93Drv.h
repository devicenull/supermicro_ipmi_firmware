     
#ifndef _LM93DRV_H_
#define _LM93DRV_H_

#define LM93_CONFIGURATION_AMONT    1
#define LM93_CONFIGURATION_REG      0xE3 
#define LM93_VID_DATA_REG	        0X6C


typedef struct
{
    BYTE b_RegAddr;
    BYTE b_Value;
} St_LM93ConfigureTypeTag;

extern STATUS SetupLM93Drv(DWORD , DWORD, void *, void *);
extern STATUS PryLM93Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteLM93Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadLM93Drv(DWORD , DWORD, void *, void *);

#ifdef _LM93DRV_C_

St_SensorAPITag ws_St_LM93API =
{
    SetupLM93Drv,
    PryLM93Drv,
    WriteLM93Drv,
    ReadLM93Drv
};

#else

extern St_SensorAPITag ws_St_LM93API;

#endif  
#endif  





