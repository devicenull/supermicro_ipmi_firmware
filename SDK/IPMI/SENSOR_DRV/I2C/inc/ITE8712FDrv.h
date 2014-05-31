
#ifndef _ITE8712FDRV_H_
#define _ITE8712FDRV_H_

#define ITE8712F_CONFIGURATION_REG     0x40
#define ITE8712F_CONFIGURATION_AMONT   4


typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_ITE8712FConfigureTypeTag;

extern STATUS SetupITE8712FDrv ( DWORD , DWORD , void * , void * );
extern STATUS PryITE8712FDrv ( DWORD , DWORD , void * , void * );
extern STATUS WriteITE8712FDrv ( DWORD , DWORD , void * , void * );
extern STATUS ReadITE8712FDrv ( DWORD , DWORD , void * , void * );

#ifdef _ITE8712FDRV_C_

St_SensorAPITag ws_St_ITE8712FAPI=
{
	SetupITE8712FDrv,
	PryITE8712FDrv,
	WriteITE8712FDrv,
	ReadITE8712FDrv,
};

#else


extern St_SensorAPITag ws_St_ITE8712FAPI;

#endif 

#endif 



