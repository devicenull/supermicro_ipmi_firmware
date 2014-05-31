     
#ifndef _PC87427DRV_H_
#define _PC87427DRV_H_

#define PC87427_CONFIGURATION_AMONT   1
#define PC87427_CONFIGURATION_REG     0xE3 
#define PC87427_VID_DATA_REG	   0X6C


typedef struct
{
    BYTE b_RegAddr;
    BYTE b_Value;
} St_PC87427ConfigureTypeTag;

extern STATUS SetupPC87427Drv(DWORD , DWORD, void *, void *);
extern STATUS PryPC87427Drv(DWORD , DWORD, void *, void *);
extern STATUS WritePC87427Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadPC87427Drv(DWORD , DWORD, void *, void *);

#ifdef _PC87427DRV_C_

St_SensorAPITag ws_St_PC87427API =
{
    SetupPC87427Drv,
    PryPC87427Drv,
    WritePC87427Drv,
    ReadPC87427Drv
};

#else

extern St_SensorAPITag ws_St_PC87427API;

#endif  
#endif  





