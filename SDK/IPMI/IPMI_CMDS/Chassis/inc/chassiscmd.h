
#ifndef _CHASSISCMD_H_
#define _CHASSISCMD_H_
#include <autoconfig.h>
#ifdef	OWN_GLOBE_BMC
#define	GB_POWER_PINTYPE_MASK	0x01
#endif



#define POWER_ON		1
#define POWER_OFF		0



#ifdef _CHASSISCMD_C_

#define POWER_DOWN_CMD		0
#define POWER_UP_CMD		1
#define POWER_CYCLE_CMD		2
#define HARD_RESET_CMD		3
#define PULSE_DIAG_CMD		4
#define SOFT_SHUTDOWN_CMD	5

#define POH_UNIT			0x3c

typedef enum
{
	SET_IN_PROGRESS = 0,
	SERVICE_PARTITION_SEL,
	SERVICE_PARTITION_SCAN,
	BMC_BOOT_FLAG_CLEAR,
	BOOT_INFO_ACK,
	BOOT_FLAG,
	BOOT_INIT_INFO,
	BOOT_INIT_MAILBOX
} En_BootOptionNnmTag;

void UtilStopWDT(void);
#else

extern BYTE UtilGetPowerStatus(void);
extern CMD_STATUS GetChassisCapCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetChassisStatusCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ChassisControlCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ChassisIDCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetPowerCycleIntervalCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetPowerRestorePolicyCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetSystemBootOptionCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetSystemBootOptionCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetSysRestartCauseCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetPOHCounterCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern STATUS POHTimerInit(void);
extern void POHTimerFunc(DWORD dw_Info);
extern BYTE ApiChassisControlSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE ApiChassisIDSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE ApiSetSystemBootOptionSubFnCheck(MsgBufFmt *p_St_MsgBufIn);

#endif   

#if  !defined(CONFIG_PLATFORM_SUPERMICRO_X7SB3)
extern STATUS PltCHASSIS_PowerInit(void);
extern BYTE PltCHASSIS_PowerStatus(void);
extern BYTE PltCHASSIS_LastPowerEvent(void);
extern BYTE PltCHASSIS_ChassisState(void);
extern void PltCHASSIS_PowerDown(void);
extern void PltCHASSIS_PowerUp(void);
extern void PltCHASSIS_PowerReset(void);
extern void PltCHASSIS_PulseDiagnosticInterrupt(void);
extern void PltCHASSIS_Identify(BYTE b_timer);
extern void PltCHASSIS_PowerCycle(BYTE b_PowerCycleInterval);
extern void PltCHASSIS_SoftShutdown(void);
extern STATUS POHTimerInit(void);
extern void POHTimerFunc(DWORD dw_Info);
extern BYTE PltCHASSIS_Capabilities_Flags(void);
extern void PltCHASSIS_Detecting_Reset(void);
#else

extern BYTE (* PltCHASSIS_PowerStatus) (void);
extern BYTE (* PltCHASSIS_Capabilities_Flags) (void);
extern BYTE (* PltCHASSIS_ChassisState)() ;
extern BYTE (* PltCHASSIS_LastPowerEvent)();
extern void (* PltCHASSIS_SoftShutdown)(void);
extern void (* PltCHASSIS_PowerCycle)(BYTE );
extern void (* PltCHASSIS_PulseDiagnosticInterrupt)(void);
extern void (* PltCHASSIS_PowerReset)();
extern void (* PltCHASSIS_PowerUp)(void);
extern void (* PltCHASSIS_PowerDown) (void);
extern void (* PltCHASSIS_Identify)(unsigned char);

#endif
#endif  





