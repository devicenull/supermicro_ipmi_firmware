
#ifndef _ADC_H_
#define _ADC_H_  


extern STATUS SetupADCDrv(DWORD, DWORD, void *, void *);
extern STATUS PryADCDrv(DWORD, DWORD, void *, void *);
extern STATUS WriteADCDrv(DWORD, DWORD, void *, void *);
extern STATUS ReadADCDrv(DWORD, DWORD, void *, void *);


#ifdef _ADC_C_

St_SensorAPITag at_St_OnChipADCAPI =
{
    SetupADCDrv,
    PryADCDrv,
    WriteADCDrv,
    ReadADCDrv
};

extern STATUS PltSetupADCDrv(DWORD, DWORD, void *, void *);
extern STATUS PltPryADCDrv(DWORD, DWORD, void *, void *);
extern STATUS PltWriteADCDrv(DWORD, DWORD, void *, void *);
extern STATUS PltReadADCDrv(DWORD, DWORD, void *, void *);

#else

extern St_SensorAPITag at_St_OnChipADCAPI;

#endif   








#endif  





