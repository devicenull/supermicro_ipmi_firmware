
#ifndef _PCF8574DRV_H_
#define _PCF8574DRV_H_

extern STATUS SetupPCF8574Drv(DWORD, DWORD, void *, void *);
extern STATUS PryPCF8574Drv(DWORD, DWORD, void *, void *);
extern STATUS WritePCF8574Drv(DWORD, DWORD, void *, void *);
extern STATUS ReadPCF8574Drv(DWORD, DWORD, void *, void *);

#ifdef _PCF8574DRV_C_

St_SensorAPITag ws_St_PCF8574API =
{
	SetupPCF8574Drv,
	PryPCF8574Drv,
	WritePCF8574Drv,
	ReadPCF8574Drv,
};

#else

extern St_SensorAPITag ws_St_PCF8574API;

#endif 

#endif 


