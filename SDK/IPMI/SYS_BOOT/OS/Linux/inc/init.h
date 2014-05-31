

#ifndef _SYSINIT_H_
#define _SYSINIT_H_ 

#include <time.h>
extern STATUS I2CInit(void);
extern STATUS PSInit(void);
extern STATUS  NVRamInit(unsigned char b_NVRamType, unsigned char b_Index);
extern STATUS PltCHASSIS_PowerInit(void);
extern STATUS PltGPIO_Init(void);
extern STATUS WDTInit(void);
extern void DCMI_Init(void);
extern STATUS LanDrvinit(void);
extern STATUS PlatformFirstInit(void);
extern STATUS PowerSupplyInit(void);

typedef struct InitStruct
{
    char *p_b_InitFuncName;
    STATUS (*InitFunc)(void);
    STATUS (*InitSubFunc)(unsigned char b_var1, unsigned char b_var2);
} St_InitStructTag;
typedef unsigned char * InitModule;

InitModule a_PreInitModuleTable[] =
{
    "ipmi_timer",
    "slavetask",
    NULL
};
InitModule a_PostInitModuleTable[] =
{
    "ipmi_event",
    "ipmi_sensor",
    "ipmi_pef",
    "ipmi_sol",
#ifdef CONFIG_CPU_WINBOND_HERMON
	"gpio_task",
#endif  
    NULL
};

#ifdef _SYSINIT_C_ 

St_InitStructTag a_St_PreSysInitTable[] = 
{
    {"GlobalVarInit()", GlobalVarInit, NULL},
    {"PlatformFirstInit()",PlatformFirstInit,NULL},
	{"LanDrvinit()",LanDrvinit,NULL},
    {"I2CInit()", I2CInit, NULL},    
    {"NVRamInit()", NULL, NVRamInit},
    {"PSShmInit()", PSShmInit, NULL},
    {"PSInit()", PSInit, NULL},
#ifdef CONFIG_WDT_DRV    
    {"WDTInit()", WDTInit, NULL},     
#endif    
    {"TimerInit()", TimerInit, NULL},  
    {NULL}
};


St_InitStructTag a_St_PostSysInitTable[] = 
{
    {"SessInit()", SessInit, NULL},
    {"EventProcInit()", EventProcInit, NULL},
    {"PltGPIO_Init()", PltGPIO_Init, NULL},
    {"PltCHASSIS_PowerInit()", PltCHASSIS_PowerInit, NULL},
    {"UARTHW_Init()",UARTHW_Init,NULL},
    {"UtilSelCmdInit()", UtilSelCmdInit, NULL},
    {"UtilUsbHidInit()", UtilUsbHidInit, NULL},
    {"InitWatchdog()", InitWatchdog, NULL},
    {"InitAgentInit()", InitAgentInit, NULL},
#ifdef IPMI_FW_FIREWALL
    {"FWFireWallInit()", FWFireWallInit, NULL},
#endif
    {"SystemTimerInit()", SystemTimerInit, NULL},
	{"OEMPostOS_Init()", OEMPostOS_Init, NULL},
#ifdef IPMI_DCMI_FEATURE
	{"DCMI_Init()", DCMI_Init, NULL},
#endif
	{"PowerSupplyInit()",PowerSupplyInit,NULL},
    {NULL}
};


#endif

#endif



