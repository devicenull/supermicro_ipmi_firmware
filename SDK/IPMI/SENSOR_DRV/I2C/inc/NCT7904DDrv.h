

#ifndef _NCT7904DDRV_H_
#define _NCT7904DDRV_H_

#define NCT7904D_REG4E_BANKSELECT				0xFF
#define NCT7904D_REG_CPU_OVERTEMP				0x55

extern STATUS Plt_NCT7904D_ClearCaseOpenLatch(void);
extern STATUS SetupNCT7904DDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS PryNCT7904DDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteNCT7904DDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadNCT7904DDrv (DWORD ,DWORD ,void * ,void *);

#ifdef _NCT7904DDRV_C_

St_SensorAPITag ws_St_NCT7904DAPI=
{
	SetupNCT7904DDrv,
	PryNCT7904DDrv,
	WriteNCT7904DDrv,
	ReadNCT7904DDrv,
};

#else

extern St_SensorAPITag ws_St_NCT7904DAPI;

#endif 



#endif