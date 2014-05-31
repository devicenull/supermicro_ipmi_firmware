



#ifndef _ADM1026DRV_H_
#define _ADM1026DRV_H_


#define ADM1026_START_MONITOR                 0x01

#define ADM1026_CONFIGURATION_1_REG           0x00
#define ADM1026_CONFIGURATION_2_REG           0x01
#define ADM1026_FAN_0_TO_3_DIVISOR_REG        0x02
#define ADM1026_FAN_4_TO_7_DIVISOR_REG        0x03
#define ADM1026_PWM_CONTROL_REG               0x05

#define ADM1026_PWM_DUTY_CYCLE_100            0xF0
#define ADM1026_PWM_DUTY_CYCLE_093            0xE0
#define ADM1026_PWM_DUTY_CYCLE_047            0x70
#define ADM1026_PWM_DUTY_CYCLE_040            0x60
#define ADM1026_PWM_DUTY_CYCLE_033            0x50
#define ADM1026_PWM_DUTY_CYCLE_007            0x10
#define ADM1026_PWM_DUTY_CYCLE_000            0x00



extern STATUS SetupADM1026Drv(DWORD , DWORD, void *, void *);
extern STATUS PryADM1026Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteADM1026Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadADM1026Drv(DWORD , DWORD, void *, void *);

#ifdef _ADM1026DRV_C_

St_SensorAPITag ws_St_ADM1026API =
{
    SetupADM1026Drv,
    PryADM1026Drv,
    WriteADM1026Drv,
    ReadADM1026Drv
};

#else

extern St_SensorAPITag ws_St_ADM1026API;

#endif  
#endif  





