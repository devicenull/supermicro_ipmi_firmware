


     
#ifndef _PCA9555DRV_H_
#define _PCA9555DRV_H_


#define PCA9555_CONFIGURATION_REG        0x06

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_PCA9555ConfigureTypeTag;

extern STATUS SetupPCA9555Drv(DWORD , DWORD, void *, void *);
extern STATUS PryPCA9555Drv(DWORD , DWORD, void *, void *);
extern STATUS WritePCA9555Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadPCA9555Drv(DWORD , DWORD, void *, void *);

#ifdef _PCA9555DRV_C_

St_SensorAPITag ws_St_PCA9555API =
{
    SetupPCA9555Drv,
    PryPCA9555Drv,
    WritePCA9555Drv,
    ReadPCA9555Drv
};

#else

extern St_SensorAPITag ws_St_PCA9555API;

#endif  
#endif  





