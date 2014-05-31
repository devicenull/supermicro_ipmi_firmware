#ifndef _MAX6651DRV_H_
#define _MAX6651DRV_H_

#define MAX6651_CONFIGURATION_REG	0x02
#define MAX6651_TACHCOUNTTIME_REG	0x16	
typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_MAX6651ConfigureTypeTag;

extern STATUS SetupMAX6651Drv(DWORD ,DWORD ,void * ,void *);
extern STATUS PryMAX6651Drv(DWORD ,DWORD ,void * ,void *);
extern STATUS WriteMAX6651Drv(DWORD ,DWORD ,void * ,void *);
extern STATUS ReadMAX6651Drv(DWORD ,DWORD ,void * ,void *);

#ifdef _MAX6651DRV_C_

St_SensorAPITag ws_St_MAX6651API=
{
	SetupMAX6651Drv,
	PryMAX6651Drv,
	WriteMAX6651Drv,
	ReadMAX6651Drv,
};

#else

extern St_SensorAPITag ws_St_MAX6651API;

#endif 

#endif 

