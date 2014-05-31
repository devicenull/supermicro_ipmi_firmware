

#ifndef _W83627HFDRV_H_
#define _W83627HFDRV_H_

#define W83627HF_CONFIGURATION_REG          0x40
#define W83627HF_CONFIGURATION_AMONT        4


typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_W83627HFConfigureTypeTag;

extern STATUS SetupW83627HFDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS PryW83627HFDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteW83627HFDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadW83627HFDrv (DWORD ,DWORD ,void * ,void *);

#ifdef _W83627HFDRV_C_

St_SensorAPITag ws_St_W83627HFAPI=
{
	SetupW83627HFDrv,
	PryW83627HFDrv,
	WriteW83627HFDrv,
	ReadW83627HFDrv,
};

#else


extern St_SensorAPITag ws_St_W83627HFAPI;

#endif 

#endif 


