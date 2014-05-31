
#ifndef _MAX6649DRV_H_
#define _MAX6649DRV_H_

#define MAX6649_CONFIGURATION_REG     0x03 

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_MAX6649ConfigureTypeTag;

extern STATUS SetupMAX6649Drv(DWORD ,DWORD ,void * ,void *);
extern STATUS PryMAX6649Drv(DWORD ,DWORD ,void * ,void *);
extern STATUS WriteMAX6649Drv(DWORD ,DWORD ,void * ,void *);
extern STATUS ReadMAX6649Drv(DWORD ,DWORD ,void * ,void *);

#ifdef _MAX6649DRV_C_

St_SensorAPITag ws_St_MAX6649API=
{
	SetupMAX6649Drv,
	PryMAX6649Drv,
	WriteMAX6649Drv,
	ReadMAX6649Drv,
};

#else

extern St_SensorAPITag ws_St_MAX6649API;

#endif 

#endif 


