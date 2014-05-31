
#ifndef _ADT7468DRV_H_
#define _ADT7468DRV_H_


#define ADT7468_CONFIGURATION_AMONT           5

#define ADT7468_CONFIGURATION_1               0x40
#define ADT7468_CONFIGURATION_2               0x73
#define ADT7468_CONFIGURATION_3               0x78
#define ADT7468_CONFIGURATION_4               0x7D
#define ADT7468_CONFIGURATION_5               0x7C
#define ADT7468_PWM_1_CONFIGURATION           0x5C
#define ADT7468_PWM_2_CONFIGURATION           0x5D
#define ADT7468_PWM_3_CONFIGURATION           0x5E
#define ADT7468_PWM_1_CURRENT_DUTY_CYCLE      0x30
#define ADT7468_PWM_2_CURRENT_DUTY_CYCLE      0x31
#define ADT7468_PWM_3_CURRENT_DUTY_CYCLE      0x32

#define ADT7468_PWM_WRITABLE                  0xE0
#define ADT7468_PWM_INVERT                    0x10
#define ADT7468_PWM_FAN_TIME_OUT_100          0x01
#define ADT7468_PWM_FAN_TIME_OUT_250          0x02
#define ADT7468_PWM_FAN_MID_SPEED             0xC0
#define ADT7468_MONITOR_ON                    0x00
#define ADT7468_START_ON                      0x01
#define ADT7468_TEMP_THRESHOLD                0x80
#define ADT7468_VID_DATA_REG				  0x43
#define ADT7468_2sC_1				          0x01

typedef struct
{
    BYTE b_CfgRgr;
    BYTE b_CfgVle;
} St_ADT7468ConfigureRegisterValueTypeTag;

typedef struct
{
    BYTE b_PWMCfgRgr;
    BYTE b_DutyCycleCfgRrg;
} St_ADT7468PWMRegisterTypeTag;

extern STATUS SetupADT7468Drv(DWORD , DWORD, void *, void *);
extern STATUS PryADT7468Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteADT7468Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadADT7468Drv(DWORD , DWORD, void *, void *);

#ifdef _ADT7468DRV_C_

St_SensorAPITag ws_St_ADT7468API =
{
    SetupADT7468Drv,
    PryADT7468Drv,
    WriteADT7468Drv,
    ReadADT7468Drv
};

#else

extern St_SensorAPITag ws_St_ADT7468API;

#endif  
#endif  





