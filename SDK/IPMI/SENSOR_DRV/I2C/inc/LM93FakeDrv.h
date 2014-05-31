     
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

extern STATUS SetupLM93FakeDrv(DWORD , DWORD, void *, void *);
extern STATUS PryLM93FakeDrv(DWORD , DWORD, void *, void *);
extern STATUS WriteLM93FakeDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadLM93FakeDrv(DWORD , DWORD, void *, void *);

#ifdef _LM93DRV_C_

St_SensorAPITag ws_St_LM93FakeAPI =
{
    SetupLM93FakeDrv,
    PryLM93FakeDrv,
    WriteLM93FakeDrv,
    ReadLM93FakeDrv
};

#else

extern St_SensorAPITag ws_St_LM93FakeAPI;

#endif  
#endif  





