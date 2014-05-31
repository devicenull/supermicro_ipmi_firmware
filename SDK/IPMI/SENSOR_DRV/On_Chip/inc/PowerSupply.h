      
#ifndef _POWER_SUPPLY_H_
#define _POWER_SUPPLY_H_

#define PRESENCE_DETECTED                                               0x00
#define POWER_SUPPY_FAILURE_DETECTED                                    0x01
#define PREDICTIVE_FAILURE                                              0x02
#define POWER_SUPPY_INPUT_LOST                                          0x03

#define GENERIC_OFFSET_TRANSITION_TO_RUNNING                            0x0001
#define GENERIC_OFFSET_TRANSITION_TO_IN_TEST                            0x0002
#define GENERIC_OFFSET_TRANSITION_TO_POWER_OFF                          0x0004
#define GENERIC_OFFSET_TRANSITION_TO_ON_LINE                            0x0008
#define GENERIC_OFFSET_TRANSITION_TO_OFF_LINE                           0x0010
#define GENERIC_OFFSET_TRANSITION_TO_OFF_DUTY                           0x0020
#define GENERIC_OFFSET_TRANSITION_TO_DEGRADED                           0x0040
#define GENERIC_OFFSET_TRANSITION_TO_POWER_SAVE                         0x0080
#define GENERIC_OFFSET_INSTALL_ERROR                                    0x0100


#define EVENT_READING_CODE_UNSPECIFIED                                  0x0F


extern STATUS SetupPowerSupplyDrv(DWORD , DWORD, void *, void *);
extern STATUS PryPowerSupplyDrv(DWORD , DWORD, void *, void *);
extern STATUS WritePowerSupplyDrv(DWORD , DWORD, void *, void *);
extern STATUS ReadPowerSupplyDrv(DWORD , DWORD, void *, void *);

#ifdef _POWER_SUPPLY_C_

St_SensorAPITag ws_St_PowerSupplyAPI =
{
    SetupPowerSupplyDrv,
    PryPowerSupplyDrv,
    WritePowerSupplyDrv,
    ReadPowerSupplyDrv
};

extern STATUS PltSetupPowerSupplyDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltPryPowerSupplyDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltWritePowerSupplyDrv(DWORD, DWORD, void  *, void  *);
extern STATUS PltReadPowerSupplyDrv(DWORD, DWORD, void  *, void  *);

#else

extern St_SensorAPITag ws_St_PowerSupplyAPI;

#endif  
#endif  





