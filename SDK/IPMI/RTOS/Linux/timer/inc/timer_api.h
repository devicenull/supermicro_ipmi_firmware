#ifndef  TIMER_API_H
#define  TIMER_API_H
#include "autoconfig.h"
#define SYSTEM_TIMER0				0x00
#define SOLRETRYFUNC_TIMER1 		0x01
#define SESSTIMERFUNC_TIMER2		0x02
#define WDTFUNC_TIMER3          	0x03
#define SENSOR_TIMER4          		0x04
#define PEF_POSTPONE_TIMER5			0x05
#ifdef CONFIG_WDT_DRV
#define HWWDT_TIMER6   				0x06
#endif
#define FLASH_TIMER7				0x07
#define CHASSIS_TIMER8          	0x08
#define PEF_RETRY_TIMER9  			0x09
#define FW_UPLOAD_REBOOT_TIMER10  	0x0a
#define CHASSIS_IDENTIFY_TIMER11  	0x0b
#define CPLD_TIMER12  	0x0C		//CPLD; bylinda
#define FPLED_TIMER					0x0D	// 6776 FP LED Control Jack 
#define PROCHOT0_TIMER				0x0E	// 4-24-2012
#define PROCHOT1_TIMER				0x0F	// 4-24-2012
#define UIDLED_TIMER				0x10
#define PSINIT_TIMER				0x11	// joe --- Multi-Node 8-15-2012
#define PSMONITOR_TIMER				0x12	// joe --- Multi-Node 8-15-2012
#define HDD_TIMER					0x13	// joe --- for HDD 9-18-2012
#define PS_FAIL_TIMER				0x14	// Tony, 12/24/2012 add bbp timeout when all power supply are failed
#define HEARTBEAT_TIMER15			0x15	// joe --- for production

extern void SystemTimer(DWORD dw_dummy);
extern void SOLRetryFunc(DWORD);
extern void SessTimerFunc(DWORD);
extern void ApiWatchdogTimerFunc(DWORD);
extern void SensorTimer(unsigned long);
extern void PEFPostponeTimerExpiryFunction(unsigned long);
extern int function_select(UINT select);
#ifdef CONFIG_WDT_DRV
extern void HWWDTimerFunc(DWORD);
#endif

#ifdef CONFIG_PLATFORM_SUPERMICRO_SUBVERSION_X9_new
extern void CPLDTimerFunc(DWORD);	//CPLD; by linda
#endif
extern void ChassisIdentifyTimerFunc(DWORD);
extern void FlashTimer(DWORD dw_dummy);
extern void ChassisTimerFunc(DWORD );
extern void PETRetryTimerExpiryFunction(DWORD dw_input);
extern void FPLedTimerFunc(DWORD);
extern void UIDLedTimerFunc(DWORD);

extern void PROCHot0TimerFunc(DWORD);	// joe 4-24-2012
extern void PROCHot1TimerFunc(DWORD);	// joe 4-24-2012
extern void PSInitTimerFunc(DWORD);		// joe 8-15-2012
extern void PSMonitorTimerFunc(DWORD);	// joe 8-15-2012
extern void HDDTimerFunc(DWORD);		// joe 9-18-2012
extern void PowerFailTimerFunc(DWORD);		// Tony, 12/24/2012 add bbp timeout when all power supply are failed
extern void LEDFunc(DWORD);
#endif
