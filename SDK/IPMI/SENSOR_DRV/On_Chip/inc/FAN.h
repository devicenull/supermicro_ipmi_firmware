
#ifndef _FAN_H_
#define _FAN_H_


extern STATUS SetupPWMFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PryPWMFANDrv(DWORD, DWORD, void *, void *);
extern STATUS WritePWMFANDrv(DWORD, DWORD, void *, void *);
extern STATUS SetupDAFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PryDAFANDrv(DWORD, DWORD, void *, void *);
extern STATUS WriteDAFANDrv(DWORD, DWORD, void *, void *);
extern STATUS ReadFANDrv(DWORD, DWORD, void *, void *);

BYTE OEMWritePWMFANDrv(
                        DWORD dw_HWInfo_1, 
                        DWORD dw_HWInfo_2, 
                        void *p_HWInfoPtr, 
                        void *p_DataBuf
                      );
                      
BYTE OEMWriteDAFANDrv(
                       DWORD dw_HWInfo_1, 
                       DWORD dw_HWInfo_2, 
                       void *p_HWInfoPtr, 
                       void *p_DataBuf
                     );

#ifdef _FAN_C_

St_SensorAPITag ws_St_OnChipPWMFANAPI =
{
    SetupPWMFANDrv,
    PryPWMFANDrv,
    WritePWMFANDrv,
    ReadFANDrv
};

St_SensorAPITag ws_St_OnChipDAFANAPI =
{
    SetupDAFANDrv,
    PryDAFANDrv,
    WriteDAFANDrv,
    ReadFANDrv
};

extern STATUS PltSetupPWMFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PltPryPWMFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PltWritePWMFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PltSetupDAFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PltPryDAFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PltWriteDAFANDrv(DWORD, DWORD, void *, void *);
extern STATUS PltReadFANDrv(DWORD, DWORD, void *, void *);

#else

extern St_SensorAPITag ws_St_OnChipPWMFANAPI;
extern St_SensorAPITag ws_St_OnChipDAFANAPI;

#endif   








#endif  





