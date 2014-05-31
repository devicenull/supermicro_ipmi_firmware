
#ifndef _ADT7463DRV_H_
#define _ADT7463DRV_H_


#define ADT7463_CONFIGURATION_AMONT           3

#define ADT7463_CONFIGURATION_1               0x40
#define ADT7463_CONFIGURATION_2               0x73
#define ADT7463_CONFIGURATION_3               0x78
#define ADT7463_PWM_1_CONFIGURATION           0x5C
#define ADT7463_PWM_2_CONFIGURATION           0x5D
#define ADT7463_PWM_3_CONFIGURATION           0x5E
#define ADT7463_PWM_1_CURRENT_DUTY_CYCLE      0x30
#define ADT7463_PWM_2_CURRENT_DUTY_CYCLE      0x31
#define ADT7463_PWM_3_CURRENT_DUTY_CYCLE      0x32
#define ADT7463_PWM_1_THERM_LIMIT		      0x6A
#define ADT7463_PWM_2_THERM_LIMIT     		  0x6B
#define ADT7463_PWM_3_THERM_LIMIT      		  0x6C



#define ADT7463_PWM_1_THERM_THRESHOLD		  0x5A
#define ADT7463_PWM_2_THERM_THRESHOLD		  0x5A
#define ADT7463_PWM_3_THERM_THRESHOLD		  0x5A

#define ADT7463_PWM_CONTROL_TMP1              0x00
#define ADT7463_PWM_CONTROL_TMP2              0x20
#define ADT7463_PWM_CONTROL_TMP3              0x40

#define ADT7463_PWM_WRITABLE                  0xE0
#define ADT7463_PWM_INVERT                    0x10
#define ADT7463_PWM_FAN_TIME_OUT_100          0x01
#define ADT7463_PWM_FAN_TIME_OUT_250          0x02
#define ADT7463_PWM_FAN_MID_SPEED             0x50
#define ADT7463_MONITOR_ON                    0x00
#define ADT7463_START_ON                      0x01
#define ADT7463_TEMP_THRESHOLD                0x80
#define ADT7463_VID_DATA_REG				  0x43

typedef struct
{
    BYTE b_CfgRgr;
    BYTE b_CfgVle;
} St_ADT7463ConfigureRegisterValueTypeTag;

typedef struct
{
    BYTE b_PWMCfgRgr;
    BYTE b_DutyCycleCfgRrg;
    BYTE b_PWMThermLimitReg;
} St_ADT7463PWMRegisterTypeTag;

extern STATUS SetupADT7463Drv(DWORD , DWORD, void *, void *);
extern STATUS PryADT7463Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteADT7463Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadADT7463Drv(DWORD , DWORD, void *, void *);

#ifdef _ADT7463DRV_C_

St_SensorAPITag ws_St_ADT7463API =
{
    SetupADT7463Drv,
    PryADT7463Drv,
    WriteADT7463Drv,
    ReadADT7463Drv
};

#else

extern St_SensorAPITag ws_St_ADT7463API;

#endif  
#endif  





