


     
#ifndef _W83L784RDRV_H_
#define _W83L784RDRV_H_


#define W83L784R_CONFIGURATION_REG       0x40
#define W83L784R_CONFIGURATION_AMONT     4

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_W83L784RConfigureTypeTag;

extern STATUS SetupW83L784RDrv(DWORD , DWORD, void *, void *);
extern STATUS PryW83L784RDrv(DWORD , DWORD, void *, void *);
extern STATUS WriteW83L784RDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadW83L784RDrv(DWORD , DWORD, void *, void *);

#ifdef _W83L784RDRV_C_

St_SensorAPITag ws_St_W83L784RAPI =
{
    SetupW83L784RDrv,
    PryW83L784RDrv,
    WriteW83L784RDrv,
    ReadW83L784RDrv
};

#else

extern St_SensorAPITag ws_St_W83L784RAPI;

#endif  
#endif  





