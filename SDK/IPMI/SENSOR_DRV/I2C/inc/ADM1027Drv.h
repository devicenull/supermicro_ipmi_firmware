
#ifndef _ADM1027DRV_H_
#define _ADM1027DRV_H_


#define ADM1027_CONFIGURATION_AMONT           3

#define ADM1027_CONFIGURATION_1               0x40
#define ADM1027_CONFIGURATION_2               0x73
#define ADM1027_CONFIGURATION_3               0x78
#define ADM1027_PWM_1_CONFIGURATION           0x5C
#define ADM1027_PWM_2_CONFIGURATION           0x5D
#define ADM1027_PWM_3_CONFIGURATION           0x5E
#define ADM1027_PWM_1_CURRENT_DUTY_CYCLE      0x30
#define ADM1027_PWM_2_CURRENT_DUTY_CYCLE      0x31
#define ADM1027_PWM_3_CURRENT_DUTY_CYCLE      0x32
#define ADM1027_PWM_WRITABLE                  0xE0
#define ADM1027_PWM_INVERT                    0x10
#define ADM1027_PWM_FAN_TIME_OUT_100          0x01
#define ADM1027_PWM_FAN_TIME_OUT_250          0x02
#define ADM1027_PWM_FAN_MID_SPEED             0xC0
#define ADM1027_MONITOR_ON                    0x00
#define ADM1027_START_ON                      0x01
#define ADM1027_TEMP_THRESHOLD                0x80
#define ADM1027_VID_DATA_REG				  0x43

typedef struct
{
    BYTE b_CfgRgr;
    BYTE b_CfgVle;
} St_ADM1027ConfigureRegisterValueTypeTag;

typedef struct
{
    BYTE b_PWMCfgRgr;
    BYTE b_DutyCycleCfgRrg;
} St_ADM1027PWMRegisterTypeTag;

extern STATUS SetupADM1027Drv(DWORD , DWORD, void *, void *);
extern STATUS PryADM1027Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteADM1027Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadADM1027Drv(DWORD , DWORD, void *, void *);

#ifdef _ADM1027DRV_C_

St_SensorAPITag ws_St_ADM1027API =
{
    SetupADM1027Drv,
    PryADM1027Drv,
    WriteADM1027Drv,
    ReadADM1027Drv
};

#else

extern St_SensorAPITag ws_St_ADM1027API;

#endif  
#endif  





