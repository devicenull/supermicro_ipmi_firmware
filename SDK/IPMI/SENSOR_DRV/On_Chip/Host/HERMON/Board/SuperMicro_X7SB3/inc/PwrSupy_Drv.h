
#ifndef _PWRSUPY_DRV_H_
#define _PWRSUPY_DRV_H_

#define PS_PWRFAIL_N 0xE3000000

#ifdef _PWRSUPY_DRV_C_

STATUS PltSetupPowerSupplyDrv(DWORD, DWORD, void  *, void  *);
STATUS PltPryPowerSupplyDrv(DWORD, DWORD, void  *, void  *);
STATUS PltWritePowerSupplyDrv(DWORD, DWORD, void  *, void  *);
STATUS PltReadPowerSupplyDrv(DWORD, DWORD, void  *, void  *);

St_SensorAPITag at_St_PwrSupyAPI =
{
	PltSetupPowerSupplyDrv,
	PltPryPowerSupplyDrv,
	PltWritePowerSupplyDrv,
	PltReadPowerSupplyDrv
};

#else

extern STATUS PltSetupPowerSupplyDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltPryPowerSupplyDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltWritePowerSupplyDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltReadPowerSupplyDrv(DWORD, DWORD, void  *, void  *);

extern St_SensorAPITag at_St_PwrSupyAPI;

#endif   


#endif  





