


     
#ifndef _W83782DDRV_H_
#define _W83782DDRV_H_



#define W83782D_CONFIGURATION_REG       0x40
#define W83782D_CONFIGURATION_AMONT     4

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_W83782DConfigureTypeTag;

extern STATUS SetupW83782DDrv(DWORD , DWORD, void *, void *);
extern STATUS PryW83782DDrv(DWORD , DWORD, void *, void *);
extern STATUS WriteW83782DDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadW83782DDrv(DWORD , DWORD, void *, void *);

#ifdef _W83782DDRV_C_

St_SensorAPITag ws_St_W83782DAPI =
{
    SetupW83782DDrv,
    PryW83782DDrv,
    WriteW83782DDrv,
    ReadW83782DDrv
};

#else

extern St_SensorAPITag ws_St_W83782DAPI;

#endif  
#endif  





