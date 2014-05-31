
#ifndef _ADT7476DRV_H_
#define _ADT7476DRV_H_


#define ADT7476_CONFIGURATION_AMONT           3

#define ADT7476_CONFIGURATION_1               0x40
#define ADT7476_CONFIGURATION_2               0x73
#define ADT7476_CONFIGURATION_3               0x78
#define ADT7476_PWM_1_CONFIGURATION           0x5C
#define ADT7476_PWM_2_CONFIGURATION           0x5D
#define ADT7476_PWM_3_CONFIGURATION           0x5E
#define ADT7476_PWM_1_CURRENT_DUTY_CYCLE      0x30
#define ADT7476_PWM_2_CURRENT_DUTY_CYCLE      0x31
#define ADT7476_PWM_3_CURRENT_DUTY_CYCLE      0x32
#define ADT7476_PWM_1_THERM_LIMIT		      0x6A
#define ADT7476_PWM_2_THERM_LIMIT     		  0x6B
#define ADT7476_PWM_3_THERM_LIMIT      		  0x6C



#define ADT7476_PWM_1_THERM_THRESHOLD		  0x5A
#define ADT7476_PWM_2_THERM_THRESHOLD		  0x5A
#define ADT7476_PWM_3_THERM_THRESHOLD		  0x5A

#define ADT7476_PWM_CONTROL_TMP1              0x00
#define ADT7476_PWM_CONTROL_TMP2              0x20
#define ADT7476_PWM_CONTROL_TMP3              0x40

#define ADT7476_PWM_WRITABLE                  0xE0
#define ADT7476_PWM_INVERT                    0x10

#define ADT7476_PWM_FAN_TIME_OUT_100          0x01
#define ADT7476_PWM_FAN_TIME_OUT_250          0x02
#define ADT7476_PWM_FAN_MID_SPEED             0x50
	
#define ADT7476_PWM_DISABLE                   0x82
#define ADT7476_MONITOR_ON                    0x00
#define ADT7476_START_ON                      0x01
#define ADT7476_TEMP_THRESHOLD                0x40
#define ADT7476_VID_DATA_REG				  0x43

typedef struct
{
    BYTE b_CfgRgr;
    BYTE b_CfgVle;
} St_ADT7476ConfigureRegisterValueTypeTag;

typedef struct
{
    BYTE b_PWMCfgRgr;
    BYTE b_DutyCycleCfgRrg;
    BYTE b_PWMThermLimitReg;
} St_ADT7476PWMRegisterTypeTag;

extern STATUS SetupADT7476Drv(DWORD , DWORD, void *, void *);
extern STATUS PryADT7476Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteADT7476Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadADT7476Drv(DWORD , DWORD, void *, void *);

#ifdef _ADT7476DRV_C_

St_SensorAPITag ws_St_ADT7476API =
{
    SetupADT7476Drv,
    PryADT7476Drv,
    WriteADT7476Drv,
    ReadADT7476Drv
};

#else

extern St_SensorAPITag ws_St_ADT7476API;

#endif  
#endif  





