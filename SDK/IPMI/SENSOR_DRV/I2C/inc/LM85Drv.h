     
#ifndef _LM85DRV_H_
#define _LM85DRV_H_


#define LM85_CONFIGURATION_AMONT            3
                                            
#define LM85_CONFIGURATION                  0x40
#define LM85_FAN_1_CONFIGURATION            0x5C
#define LM85_FAN_2_CONFIGURATION            0x5D
#define LM85_FAN_3_CONFIGURATION            0x5E
#define LM85_PWM_1_CURRENT_DUTY_CYCLE       0x30
#define LM85_PWM_2_CURRENT_DUTY_CYCLE       0x31
#define LM85_PWM_3_CURRENT_DUTY_CYCLE       0x32
#define LM85_FAN_ZONE                       0x60
#define LM85_PWM_INVERT                     0x10
#define LM85_PWM_FAN_SPIN_UP_100            0x01
#define LM85_PWM_FAN_TIME_OUT_250           0x02
#define LM85_PWM_FAN_MID_SPEED              0xFF
#define LM85_MONITOR_ON                     0x00
#define LM85_START_ON                       0x01
#define LM85_TEMP_THRESHOLD                 0x80
#define LM85_VID_DATA_REG				    0x43

typedef struct
{
    BYTE b_CfgRgr;
    BYTE b_CfgVle;
} St_LM85ConfigureRegisterValueTypeTag;

typedef struct
{
    BYTE b_PWMCfgRgr;
    BYTE b_DutyCycleCfgRrg;
} St_LM85PWMRegisterTypeTag;

extern STATUS SetupLM85Drv(DWORD , DWORD, void *, void *);
extern STATUS PryLM85Drv(DWORD , DWORD, void *, void *);
extern STATUS WriteLM85Drv(DWORD , DWORD, void *, void *);
extern STATUS ReadLM85Drv(DWORD , DWORD, void *, void *);

#ifdef _LM85DRV_C_

St_SensorAPITag ws_St_LM85API =
{
    SetupLM85Drv,
    PryLM85Drv,
    WriteLM85Drv,
    ReadLM85Drv
};

#else

extern St_SensorAPITag ws_St_LM85API;

#endif  
#endif  





