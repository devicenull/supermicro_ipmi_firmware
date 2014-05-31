

#ifndef _R2Z_6400PDRV_H_
#define _R2Z_6400PDRV_H_



extern STATUS SetupR2Z_6400PDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS PryR2Z_6400PDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS WriteR2Z_6400PDrv (DWORD ,DWORD ,void * ,void *);
extern STATUS ReadR2Z_6400PDrv (DWORD ,DWORD ,void * ,void *);

#ifdef _R2Z_6400PDRV_C_

St_SensorAPITag ws_St_R2Z_6400PAPI=
{
	SetupR2Z_6400PDrv,
	PryR2Z_6400PDrv,
	WriteR2Z_6400PDrv,
	ReadR2Z_6400PDrv,
};

#else


extern St_SensorAPITag ws_St_R2Z_6400PAPI;

#endif 

#endif 


