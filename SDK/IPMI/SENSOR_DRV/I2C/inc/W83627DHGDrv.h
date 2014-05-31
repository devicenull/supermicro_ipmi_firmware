

#ifndef _W83627DHGDRV_H_
#define _W83627DHGDRV_H_

#define W83627DHG_CONFIGURATION_REG          	0x40
#define W83627DHG_VBATMONITOR_REG          		0x5D
#define W83627DHG_CONFIGURATION_AMONT        	4
#define W83627DHG_REG50h_55h_BANKSELECT			0x4E
#define W83627DHG_REG50h_55h_BANK0				0x00
#define W83627DHG_REG50h_55h_BANK1				0x01
#define W83627DHG_REG50h_55h_BANK2				0x02
#define W83627DHG_REG50h_55h_BANK4				0x04
#define W83627DHG_REG50h_55h_BANK5				0x05
#define W83627DHG_SMIMASK_REG          			0x46
#define W83627DHG_CPU1_OVERTEMP_REG     		0x55
#define W83627DHG_CPU2_OVERTEMP_REG     		0x55
#define W83627DHG_CPU_OFFSET_REG				0x5F

typedef struct 
{
	BYTE  b_RegAddr;
	BYTE  b_Value;
} St_W83627DHGConfigureTypeTag;
extern STATUS Plt_W83627DHG_ClearCaseOpenLatch(void);
extern STATUS SetupW83627DHGDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS PryW83627DHGDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteW83627DHGDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadW83627DHGDrv (DWORD ,DWORD ,void * ,void *);

#ifdef _W83627DHGDRV_C_

St_SensorAPITag ws_St_W83627DHGAPI=
{
	SetupW83627DHGDrv,
	PryW83627DHGDrv,
	WriteW83627DHGDrv,
	ReadW83627DHGDrv,
};

#else


extern St_SensorAPITag ws_St_W83627DHGAPI;

#endif 

#endif 


