

#ifndef _GLOBAL_PARAMETER_H_
#define _GLOBAL_PARAMETER_H_

#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <PltConfig.h>

#include <OS_api.h>
#include <msgbuf.h>

#include <Payload.h>
#include <RMCPplus.h>
#include <Message.h>
#include <sesscmd.h>
#include <lancmd.h>
#include <chnlcmd.h>
#include <sdrcmds.h>
#include <selcmds.h>
#include <pefcmd.h>
#include <sensorcmds.h>
#include <chassiscmd.h>
#include <firewall.h>

#include <UART.h>
#include <wdtcmd.h>
#include <SOLCmd.h>
#include <lan.h>
#include "user.h"

#include <pef.h>
#include <SOL.h>

#include <sensor.h>
#include <event.h>
#include <sensorinitagent.h>
#include <utilities.h>
#include <Net_API.h>
#include <UARTLib.h>
#include <flash.h>
#include "OEMgparam.h"
#include "IPMI/IPMI_CMDS/RMCP+_Payload/inc/RMCPplus.h"

typedef struct IPCKeyVarStruct
{
    struct {
	           key_t at_key[IPCKeyMaxNum];
	           int at_w_id[IPCKeyMaxNum];
           }SHMInfo;

    struct {
	           key_t at_key[IPCKeyMaxNum];
	           int at_w_id[IPCKeyMaxNum];
           }SEMInfo;

    struct {
	           key_t at_key[IPCKeyMaxNum];
	           int at_w_id[IPCKeyMaxNum];
           }MSGInfo;
}St_IPCKeyVarTag;

#define at_SHMKey	at_p_St_GlobalVar->IPCKeyVarInfo.SHMInfo.at_key
#define at_w_SHMID	at_p_St_GlobalVar->IPCKeyVarInfo.SHMInfo.at_w_id
#define at_SEMKey	at_p_St_GlobalVar->IPCKeyVarInfo.SEMInfo.at_key
#define at_w_SEMID	at_p_St_GlobalVar->IPCKeyVarInfo.SEMInfo.at_w_id
#define at_MSGKey	at_p_St_GlobalVar->IPCKeyVarInfo.MSGInfo.at_key
#define at_w_MSGID	at_p_St_GlobalVar->IPCKeyVarInfo.MSGInfo.at_w_id


typedef struct ChassisDrvVarStruct
{
	OS_TIMER 	at_EVB_LED_Timer;
	OS_TIMER 	at_ChassisTimer;
	OS_TIMER	at_ChassisIdentifyTimer;
#ifdef CONFIG_WDT_DRV
	OS_TIMER	at_b_HWWDTimer;
#endif
	BYTE 		at_b_PowerOffTimer;
	BYTE 		at_b_PWCFlg;
	BYTE 		at_b_PWCTimer;
	BYTE 		at_B_PowerStatus;
	BYTE 		at_b_PowerDelayTimer;
	BYTE		at_b_PWDownFlg;
	BYTE		at_b_PowerDownDelay;
	BYTE		at_b_PowerUpDelay;
	BYTE		at_b_PowerResetDelay;
	BYTE		at_b_PowerSoftDownDelay;
}St_ChassisDrvVarTag;


#define at_b_PWDownFlg			    at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PWDownFlg
#define at_EVB_LED_Timer			at_p_St_GlobalVar->ChassisDrvVarInfo.at_EVB_LED_Timer
#define at_ChassisTimer				at_p_St_GlobalVar->ChassisDrvVarInfo.at_ChassisTimer
#define at_ChassisIdentifyTimer     at_p_St_GlobalVar->ChassisDrvVarInfo.at_ChassisIdentifyTimer
#ifdef CONFIG_WDT_DRV
#define at_b_HWWDTimer           	at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_HWWDTimer 
#endif
#define at_b_PowerOffTimer      	at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PowerOffTimer
#define at_b_PWCFlg           		at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PWCFlg
#define at_b_PWCTimer           	at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PWCTimer
#define at_B_PowerStatus           	at_p_St_GlobalVar->ChassisDrvVarInfo.at_B_PowerStatus
#define at_b_PowerDelayTimer        at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PowerDelayTimer
#define at_b_PowerDownDelay		at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PowerDownDelay		
#define at_b_PowerUpDelay		at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PowerUpDelay		
#define at_b_PowerResetDelay		at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PowerResetDelay		
#define at_b_PowerSoftDownDelay		at_p_St_GlobalVar->ChassisDrvVarInfo.at_b_PowerSoftDownDelay


typedef struct ChassisVarStruct
{
	volatile BYTE	at_b_SysRestartCause;
	volatile BYTE	at_b_lastRestartCause;
	BYTE			at_b_PWCycleTimer;
	
	BYTE			at_b_SetInProgress;
	BYTE			at_b_BootFlagValidBitClr;
	BYTE			at_a_b_BootInfoAck;
	BYTE			at_a_b_BootFlag[5];
	BYTE			at_a_b_BootParaMask[16];
	BYTE			at_b_IsSetInProgress;
	BYTE			at_b_PowerPolicy;
	OS_TIMER		at_POHTimer;
	DWORD			at_dw_Boot_Time;
	BYTE			at_b_BootFlagCountDown;
	BYTE			at_b_ForceIdentifyOn;
	BYTE			at_b_LastPowerState;
	BYTE			at_b_PowerOnByCMD;
}St_ChassisVarTag;

#define at_b_SysRestartCause		at_p_St_GlobalVar->ChassisVarInfo.at_b_SysRestartCause
#define at_b_lastRestartCause		at_p_St_GlobalVar->ChassisVarInfo.at_b_lastRestartCause
#define at_b_PWCycleTimer			at_p_St_GlobalVar->ChassisVarInfo.at_b_PWCycleTimer
#define at_b_SetInProgress			at_p_St_GlobalVar->ChassisVarInfo.at_b_SetInProgress
#define at_b_BootFlagValidBitClr	at_p_St_GlobalVar->ChassisVarInfo.at_b_BootFlagValidBitClr
#define at_a_b_BootInfoAck			at_p_St_GlobalVar->ChassisVarInfo.at_a_b_BootInfoAck
#define at_a_b_BootFlag				at_p_St_GlobalVar->ChassisVarInfo.at_a_b_BootFlag
#define at_a_b_BootParaMask			at_p_St_GlobalVar->ChassisVarInfo.at_a_b_BootParaMask
#define at_b_IsSetInProgress		at_p_St_GlobalVar->ChassisVarInfo.at_b_IsSetInProgress
#define at_b_PowerPolicy			at_p_St_GlobalVar->ChassisVarInfo.at_b_PowerPolicy
#define at_POHTimer					at_p_St_GlobalVar->ChassisVarInfo.at_POHTimer
#define at_dw_Boot_Time				at_p_St_GlobalVar->ChassisVarInfo.at_dw_Boot_Time
#define at_b_BootFlagCountDown		at_p_St_GlobalVar->ChassisVarInfo.at_b_BootFlagCountDown
#define at_b_ForceIdentifyOn		at_p_St_GlobalVar->ChassisVarInfo.at_b_ForceIdentifyOn
#define at_b_LastPowerState			at_p_St_GlobalVar->ChassisVarInfo.at_b_LastPowerState
#define at_b_PowerOnByCMD			at_p_St_GlobalVar->ChassisVarInfo.at_b_PowerOnByCMD

typedef struct ChnlVarStruct
{
	St_ChnlAccessTag at_a_St_ChnlAccessTbl[16];
}St_ChnlVarTag;

#define at_a_St_ChnlAccessTbl       at_p_St_GlobalVar->ChnlVarInfo.at_a_St_ChnlAccessTbl


typedef struct EvtVarStruct
{
	OS_QUEUE			at_EvtTaskMsgQ;
	OS_QUEUE			at_EvtMsgBuffQ;
	BYTE				at_b_EvtRcverAddr;     
	BYTE				at_b_EvtRcverLun;
	BYTE				at_b_EvtRcverSelf; 
	BYTE				at_b_EvtStatFlag;
    OS_MUTEX			at_IPMIEVENTMTX;
    BYTE                bBuff16[16];
}St_EvtVarTag;

#define at_EvtTaskMsgQ				at_p_St_GlobalVar->EvtVarInfo.at_EvtTaskMsgQ
#define at_EvtMsgBuffQ				at_p_St_GlobalVar->EvtVarInfo.at_EvtMsgBuffQ
#define at_b_EvtRcverAddr			at_p_St_GlobalVar->EvtVarInfo.at_b_EvtRcverAddr
#define at_b_EvtRcverLun			at_p_St_GlobalVar->EvtVarInfo.at_b_EvtRcverLun
#define at_b_EvtRcverSelf			at_p_St_GlobalVar->EvtVarInfo.at_b_EvtRcverSelf
#define at_b_EvtStatFlag			at_p_St_GlobalVar->EvtVarInfo.at_b_EvtStatFlag
#define at_IPMIEVENTMTX				at_p_St_GlobalVar->EvtVarInfo.at_IPMIEVENTMTX
#define bBuff16				        at_p_St_GlobalVar->EvtVarInfo.bBuff16

typedef struct GetMsgStruct
{
	  OS_QUEUE			at_RcvMsgQueue;
    OS_MUTEX			at_MsgMutex;
    MsgBufFmt     *at_RcvMsgQ;
    MsgBufFmt     at_RcvMsgQBuff;
}St_GetMsgTag;

#define at_RcvMsgQueue			at_p_St_GlobalVar->GetMsgInfo.at_RcvMsgQueue
#define at_RcvMsgQBuff			at_p_St_GlobalVar->GetMsgInfo.at_RcvMsgQBuff
#define at_RcvMsgQ    			at_p_St_GlobalVar->GetMsgInfo.at_RcvMsgQ
#define at_MsgMutex			    at_p_St_GlobalVar->GetMsgInfo.at_MsgMutex

typedef struct GlobalCMDVarStruct
{
    BYTE at_b_ACPI_Device_State;
    BYTE at_b_ACPI_System_State;
}St_GlobalCMDVarTag;

#define at_b_ACPI_System_State	at_p_St_GlobalVar->GlobalVarInfo.at_b_ACPI_System_State
#define at_b_ACPI_Device_State	at_p_St_GlobalVar->GlobalVarInfo.at_b_ACPI_Device_State

typedef struct NTPVarStruct
{
	BYTE at_b_NTPState;
	BYTE b_NTP_Ready;
	BYTE at_a_b_NTPServerIP_Pri[128];
	BYTE at_a_b_NTPServerIP_Sec[128];
}St_NTPVarTag;

#define at_p_NTP				at_p_St_GlobalVar->NTPVarInfo
#define at_b_NTPState			at_p_NTP.at_b_NTPState
#define at_b_NTP_Ready			at_p_NTP.b_NTP_Ready
#define at_a_b_NTPServerIP_Pri	at_p_NTP.at_a_b_NTPServerIP_Pri
#define at_a_b_NTPServerIP_Sec	at_p_NTP.at_a_b_NTPServerIP_Sec

#ifdef ATEN_TFTP_FEATURE
typedef struct TFTPDVarStruct
{
	BYTE at_b_TFTPDState; 
}St_TFTPDVarTag;

#define at_p_TFTPD			at_p_St_GlobalVar->TFTPDVarInfo
#define	at_b_TFTPDState		at_p_TFTPD.at_b_TFTPDState
#endif

typedef struct DateTimeVarStruct
{
	int  at_dw_TimeZoneValue;
	int  at_dw_TimeZoneSemID;
	int  at_dw_DST;
}St_DateTimeVarTag;

#define at_p_Date_Time_PTR		at_p_St_GlobalVar->DateTimeVarInfo
#define at_dw_TimeZoneValue     at_p_Date_Time_PTR.at_dw_TimeZoneValue
#define at_dw_TimeZoneSemID		at_p_Date_Time_PTR.at_dw_TimeZoneSemID
#define at_dw_DST				at_p_Date_Time_PTR.at_dw_DST


typedef struct LANVarStruct
{
    St_LanDestInfoTag 		at_a_St_LanDestVolatileInfo[MAX_LAN_NUM];
    St_LanVolatileInfoTag 	at_St_LanVolatileInfo;
    St_NICTag			at_a_St_NIC[MAX_LAN_NUM];
    WORD			at_w_LanConfigInfo;
    WORD			at_w_NeedToCfgLan;
    WORD			at_w_RMCPPKtsRecvCounter;
    BYTE			at_b_DNSServerIP[4];
    BYTE			at_b_session_timeout;
    int				at_LanModulePid;
#ifdef IPMI_DHCP
   OS_TIMER			at_CHKIPStateTimer; 
	BYTE			at_b_GetIPfromDHCP;
#endif 
}St_LANVarTag;




#define at_p_LAN			at_p_St_GlobalVar->LANVarInfo
#define at_a_St_NIC			at_p_LAN.at_a_St_NIC
#define at_a_St_LanDestVolatileInfo 	at_p_LAN.at_a_St_LanDestVolatileInfo
#define at_St_LanVolatileInfo       	at_p_LAN.at_St_LanVolatileInfo
#define at_w_LanConfigInfo		at_p_LAN.at_w_LanConfigInfo
#define at_w_NeedToCfgLan		at_p_LAN.at_w_NeedToCfgLan
#define at_w_RMCPPKtsRecvCounter	at_p_LAN.at_w_RMCPPKtsRecvCounter
#define at_b_DNSServerIP		at_p_LAN.at_b_DNSServerIP
#define at_b_session_timeout	at_p_LAN.at_b_session_timeout
#ifdef IPMI_DHCP
#define at_CHKIPStateTimer		at_p_LAN.at_CHKIPStateTimer
#define at_b_GetIPfromDHCP		at_p_LAN.at_b_GetIPfromDHCP
#define at_LanModulePid			at_p_LAN.at_LanModulePid
#endif


typedef struct PEFVarStruct{
	WORD 					at_a_w_PEFTaskStack[PEFTASKSTACKSIZE/2];
	
	St_EventMsgRecordTag    at_St_PEFData;
	St_PEFActionTag         at_St_PEFAction;
	St_AlertQueueTag        at_a_St_AlertQueue[MAX_ALERT_POLICY_TABLE_ENTRY];
	St_PefVolatileInfoTag   at_St_PefVolatileInfo;
	BYTE 					at_a_b_PETOutGoingBuf[MAX_PET_OUTGOING_BUF_SIZE];
	
	DWORD 					at_dw_ImmedPETEvent;
	St_EventMsgRecordTag    at_St_ImmedPEFData;
	St_AlertQueueTag        at_St_ImmedAlertQueue;
	BYTE 					at_a_b_ImmedPETOutGoingBuf[MAX_PET_OUTGOING_BUF_SIZE];
	
	OS_TIMER  				at_PEFPostponeTimer;
	OS_TIMER  				at_PETRetryTimer;
	OS_MUTEX  				at_PEFMTX;
	
	St_ASEntryTag			at_a_St_ASVolatileEntry[1];
}St_PEFVarTag;
#define at_a_w_PEFTaskStack         at_p_St_GlobalVar->PEFVarInfo.at_a_w_PEFTaskStack
#define at_St_PEFData               at_p_St_GlobalVar->PEFVarInfo.at_St_PEFData
#define at_St_PEFAction             at_p_St_GlobalVar->PEFVarInfo.at_St_PEFAction
#define at_a_St_AlertQueue          at_p_St_GlobalVar->PEFVarInfo.at_a_St_AlertQueue
#define at_St_PefVolatileInfo       at_p_St_GlobalVar->PEFVarInfo.at_St_PefVolatileInfo
#define at_a_b_PETOutGoingBuf       at_p_St_GlobalVar->PEFVarInfo.at_a_b_PETOutGoingBuf
#define at_dw_ImmedPETEvent         at_p_St_GlobalVar->PEFVarInfo.at_dw_ImmedPETEvent
#define at_St_ImmedPEFData          at_p_St_GlobalVar->PEFVarInfo.at_St_ImmedPEFData
#define at_St_ImmedAlertQueue       at_p_St_GlobalVar->PEFVarInfo.at_St_ImmedAlertQueue
#define at_a_b_ImmedPETOutGoingBuf  at_p_St_GlobalVar->PEFVarInfo.at_a_b_ImmedPETOutGoingBuf
#define at_PEFPostponeTimer         at_p_St_GlobalVar->PEFVarInfo.at_PEFPostponeTimer
#define at_PETRetryTimer            at_p_St_GlobalVar->PEFVarInfo.at_PETRetryTimer
#define at_PEFMTX                   at_p_St_GlobalVar->PEFVarInfo.at_PEFMTX

#define at_a_St_ASVolatileEntry     at_p_St_GlobalVar->PEFVarInfo.at_a_St_ASVolatileEntry


#ifdef IPMI_RMCP_PLUS
typedef struct RMCPplusVarStruct
{
	BYTE							at_b_DataLength; 
	BYTE							at_b_KeyLength;
}St_RMCPplusVarTag;

#define at_b_DataLength				at_p_St_GlobalVar->RMCPplusVarInfo.at_b_DataLength
#define at_b_KeyLength				at_p_St_GlobalVar->RMCPplusVarInfo.at_b_KeyLength
#endif

typedef struct PayloadVarStruct
{
	St_PayloadCmdTag				st_PayloadVrb;
	St_PayloadAccessPerLAN 			at_St_DefaultPayloadAccessPerLAN[MAX_LAN_NUM];
	St_UserPayloadAccessTag			at_St_DefaultUserPayloadAccess;
}St_PayloadVarTag;

#define at_p_Payload						at_p_St_GlobalVar->PayloadVarInfo		
#define at_St_PayloadVrb            		at_p_Payload.st_PayloadVrb
#define at_St_DefaultPayloadAccessPerLAN  	at_p_St_GlobalVar->PayloadVarInfo.at_St_DefaultPayloadAccessPerLAN
#define at_St_DefaultUserPayloadAccess		at_p_St_GlobalVar->PayloadVarInfo.at_St_DefaultUserPayloadAccess

typedef struct SDRVarStruct
{
    ULONG					at_dw_SDR_last_add_time;
	ULONG					at_dw_SDR_last_erase_time;
	WORD					at_w_SDR_CURR_PTR;
	WORD					at_w_SDR_TAIL_PTR;
	WORD					at_w_SDR_REC_TOTAL;
	WORD					at_w_SENSOR_TOTAL;
	WORD					at_w_LAST_SDR_ID;
	WORD					at_w_TOTAL_SPACE;
	BYTE					at_b_SDR_BUF_TAIL;
	BYTE					at_b_SDR_OVERFLOW;
	BYTE					at_b_InitAgentDone;
	St_SdrReservedInfoTag	at_St_SdrResvInfo;
	OS_MUTEX				at_IPMISDRCLRMTX;
	OS_MUTEX				at_IPMISDRMTX;
	BYTE					at_b_ExtraAlertStringConf;
}St_SDRVarTag;

#define at_dw_SDR_last_add_time         at_p_St_GlobalVar->SDRVarInfo.at_dw_SDR_last_add_time
#define at_dw_SDR_last_erase_time		at_p_St_GlobalVar->SDRVarInfo.at_dw_SDR_last_erase_time
#define at_w_SDR_CURR_PTR				at_p_St_GlobalVar->SDRVarInfo.at_w_SDR_CURR_PTR
#define at_w_SDR_TAIL_PTR				at_p_St_GlobalVar->SDRVarInfo.at_w_SDR_TAIL_PTR
#define at_w_SDR_REC_TOTAL				at_p_St_GlobalVar->SDRVarInfo.at_w_SDR_REC_TOTAL
#define at_w_SENSOR_TOTAL				at_p_St_GlobalVar->SDRVarInfo.at_w_SENSOR_TOTAL
#define at_w_LAST_SDR_ID				at_p_St_GlobalVar->SDRVarInfo.at_w_LAST_SDR_ID
#define at_w_TOTAL_SPACE				at_p_St_GlobalVar->SDRVarInfo.at_w_TOTAL_SPACE
#define at_b_SDR_BUF_TAIL				at_p_St_GlobalVar->SDRVarInfo.at_b_SDR_BUF_TAIL
#define at_b_SDR_OVERFLOW				at_p_St_GlobalVar->SDRVarInfo.at_b_SDR_OVERFLOW
#define at_b_InitAgentDone				at_p_St_GlobalVar->SDRVarInfo.at_b_InitAgentDone
#define at_St_SdrResvInfo				at_p_St_GlobalVar->SDRVarInfo.at_St_SdrResvInfo
#define at_IPMISDRCLRMTX				at_p_St_GlobalVar->SDRVarInfo.at_IPMISDRCLRMTX
#define at_IPMISDRMTX					at_p_St_GlobalVar->SDRVarInfo.at_IPMISDRMTX
#define at_b_ExtraAlertStringConf		at_p_St_GlobalVar->SDRVarInfo.at_b_ExtraAlertStringConf

#define	SDR_BitMap_BufferSize	32
#define	SDR_Unit_Length			67			//SDR(64B)+ptr(2B)+CHK(1B)
typedef struct
{
	BYTE			at_b_SDRBitMap[SDR_BitMap_BufferSize];
	BYTE			at_b_SDRtotal_real;
	BYTE			at_b_ADDSDR_EN;
	OS_MUTEX		at_SDRMTX;
}St_SDRHeaderTag;

#define at_b_SDRBitMap				at_p_St_GlobalVar->SDRHeaderInfo.at_b_SDRBitMap
#define at_b_SDRtotal_real			at_p_St_GlobalVar->SDRHeaderInfo.at_b_SDRtotal_real
#define at_b_ADDSDR_EN				at_p_St_GlobalVar->SDRHeaderInfo.at_b_ADDSDR_EN
#define at_SDRMTX					at_p_St_GlobalVar->SDRHeaderInfo.at_SDRMTX

typedef struct  SELVarStruct
{
	OS_MUTEX			at_IPMISELMTX;

	DWORD 				at_dw_LastDelTime;
	DWORD 				at_dw_LastAddTime;
	WORD  				at_w_LastEntryIdx;
	WORD  				at_w_NumEntries;
	BYTE  				at_b_LogStatus;    
	BYTE  				at_b_InChnl;
	DWORD 				at_dw_SELNVRamEvent;
	St_SelPefEvtTag		at_St_SelPefEvtBuf;
	BYTE 				at_a_b_LastSelNvramRecord[SEL_ENTRY_BYTES];
	WORD 				at_w_SelOffset;
#ifdef SEL_AGING
	BYTE 				at_b_IsSELAgingEnable;
#else
	BYTE		 		at_b_IsSELAgingEnable;
#endif
#ifdef SEL_DISCRETE
	BYTE 				at_b_IsSELContinuous;
#else
	BYTE		 		at_b_IsSELContinuous;
#endif

#ifndef _EXT_RAM_SYNC_SEL_C_
#define _EXT_RAM_SYNC_SEL_C_
	unsigned char         *at_p_ExtRamSelHead;
#endif

	WORD 				at_SEL_init_fin;
	WORD at_w_FirstSelRealIndex;
}St_SELVarTag;
#define at_IPMISELMTX				at_p_St_GlobalVar->SELVarInfo.at_IPMISELMTX
#define at_dw_LastDelTime			at_p_St_GlobalVar->SELVarInfo.at_dw_LastDelTime
#define at_dw_LastAddTime			at_p_St_GlobalVar->SELVarInfo.at_dw_LastAddTime
#define at_w_LastEntryIdx			at_p_St_GlobalVar->SELVarInfo.at_w_LastEntryIdx
#define at_w_NumEntries				at_p_St_GlobalVar->SELVarInfo.at_w_NumEntries
#define at_b_LogStatus				at_p_St_GlobalVar->SELVarInfo.at_b_LogStatus
#define at_b_InChnl					at_p_St_GlobalVar->SELVarInfo.at_b_InChnl
#define at_dw_SELNVRamEvent			at_p_St_GlobalVar->SELVarInfo.at_dw_SELNVRamEvent
#define at_St_SelPefEvtBuf			at_p_St_GlobalVar->SELVarInfo.at_St_SelPefEvtBuf
#define at_a_b_LastSelNvramRecord	at_p_St_GlobalVar->SELVarInfo.at_a_b_LastSelNvramRecord
#define at_w_SelOffset				at_p_St_GlobalVar->SELVarInfo.at_w_SelOffset
#ifdef SEL_AGING
#define at_b_IsSELAgingEnable		at_p_St_GlobalVar->SELVarInfo.at_b_IsSELAgingEnable
#else
#define at_b_IsSELAgingEnable		at_p_St_GlobalVar->SELVarInfo.at_b_IsSELAgingEnable
#endif
#ifdef SEL_DISCRETE
#define at_b_IsSELContinuous		at_p_St_GlobalVar->SELVarInfo.at_b_IsSELContinuous
#else
#define at_b_IsSELContinuous		at_p_St_GlobalVar->SELVarInfo.at_b_IsSELContinuous
#endif

#ifndef _EXT_RAM_SYNC_SEL_C_
#define _EXT_RAM_SYNC_SEL_C_
#define at_p_ExtRamSelHead		at_p_St_GlobalVar->SELVarInfo.at_p_ExtRamSelHead
#endif

#define at_SEL_init_fin				at_p_St_GlobalVar->SELVarInfo.at_SEL_init_fin
#define at_w_FirstSelRealIndex 		at_p_St_GlobalVar->SELVarInfo.at_w_FirstSelRealIndex

typedef struct SensorVarStruct
{
	OS_TIMER				at_IPMISENSORTMR;
	OS_MUTEX				at_IPMISENSORMTX;
	OS_MUTEX				at_SensorDriveMTX;
	unsigned char	at_b_CurrentIndex;
	STATUS					at_b_ReadSensor;
	BOOLEAN					at_b_MainBoardPower;
	BYTE					at_B_ScanI2C;
	BYTE					at_b_DisableScanCounter;
 	BOOLEAN					at_b_SensorTaskRunning;
	BYTE					at_b_HaveSELSensor;
#ifdef SUPPORT_SIGNED_SDR
	signed char				s_UNR, s_UC, s_UNC, s_LNR, s_LC, s_LNC;
#endif

#ifdef OEM_CALLBACK_FUNCTION
#include "../sensor_drv/inc/OEM_CallBackFunctionTable.h"
	St_SensorDataTag		*at_St_OEMCallBack_SensorData;
	unsigned char			at_b_OEMCallBack_Max_RELATED_Num;
	unsigned char			at_b_OEMCallBack_Max_REACTED_Num;
#endif
#ifdef OWN_GLOBE_BMC
	BYTE					at_B_GBSensorInitRun;
#endif
	

	DWORD				at_dw_SensorProcessPID;
	at_s_MSG_CTRL_LIST		at_a_St_McList[MAX_MSG_CTRL_NUM];
	at_s_INIT_AGENT_RESULT 	at_St_InitAgentResult;
	
	//St_SensorDataTag		*at_p_St_SELSensorData;
    BYTE					at_a_b_SdrBuf[68];
#ifdef RAM_SENSOR_EN
    unsigned char			at_anl_SnBuf[17];
#endif     
}St_SensorVarTag;
#ifdef RAM_SENSOR_EN
#define at_anl_SnBuf			at_p_St_GlobalVar->SensorVarInfo.at_anl_SnBuf 
#endif     
#define at_a_b_SdrBuf			at_p_St_GlobalVar->SensorVarInfo.at_a_b_SdrBuf	
#define at_IPMISENSORTMR		at_p_St_GlobalVar->SensorVarInfo.at_IPMISENSORTMR
#define at_IPMISENSORMTX		at_p_St_GlobalVar->SensorVarInfo.at_IPMISENSORMTX
#define at_SensorDriveMTX		at_p_St_GlobalVar->SensorVarInfo.at_SensorDriveMTX
#define at_b_CurrentIndex		at_p_St_GlobalVar->SensorVarInfo.at_b_CurrentIndex
#define at_b_ReadSensor			at_p_St_GlobalVar->SensorVarInfo.at_b_ReadSensor
#define at_b_MainBoardPower		at_p_St_GlobalVar->SensorVarInfo.at_b_MainBoardPower
#define at_dw_SensorProcessPID		at_p_St_GlobalVar->SensorVarInfo.at_dw_SensorProcessPID
#define at_B_ScanI2C			at_p_St_GlobalVar->SensorVarInfo.at_B_ScanI2C
#define at_b_DisableScanCounter	at_p_St_GlobalVar->SensorVarInfo.at_b_DisableScanCounter
#define at_b_SensorTaskRunning  at_p_St_GlobalVar->SensorVarInfo.at_b_SensorTaskRunning
#define at_b_HaveSELSensor		at_p_St_GlobalVar->SensorVarInfo.at_b_HaveSELSensor
#ifdef SUPPORT_SIGNED_SDR
#define s_UNR					at_p_St_GlobalVar->SensorVarInfo.s_UNR
#define s_UC					at_p_St_GlobalVar->SensorVarInfo.s_UC
#define s_UNC					at_p_St_GlobalVar->SensorVarInfo.s_UNC
#define s_LNR					at_p_St_GlobalVar->SensorVarInfo.s_LNR
#define s_LC					at_p_St_GlobalVar->SensorVarInfo.s_LC
#define s_LNC					at_p_St_GlobalVar->SensorVarInfo.s_LNC
#endif

#ifdef OEM_CALLBACK_FUNCTION
#include "../sensor_drv/inc/OEM_CallBackFunctionTable.h"
#define at_St_OEMCallBack_SensorData		at_p_St_GlobalVar->SensorVarInfo.at_St_OEMCallBack_SensorData
#define at_b_OEMCallBack_Max_RELATED_Num	at_p_St_GlobalVar->SensorVarInfo.at_b_OEMCallBack_Max_RELATED_Num
#define at_b_OEMCallBack_Max_REACTED_Num	at_p_St_GlobalVar->SensorVarInfo.at_b_OEMCallBack_Max_REACTED_Num
#endif
#ifdef OWN_GLOBE_BMC
#define at_B_GBSensorInitRun				at_p_St_GlobalVar->SensorVarInfo.at_B_GBSensorInitRun
#endif
#define at_a_St_McList						at_p_St_GlobalVar->SensorVarInfo.at_a_St_McList
#define at_St_InitAgentResult				at_p_St_GlobalVar->SensorVarInfo.at_St_InitAgentResult
//#define at_p_St_SELSensorData				at_p_St_GlobalVar->SensorVarInfo.at_p_St_SELSensorData

typedef struct SessVarStruct
{
	OS_MUTEX	at_SessTblMux;
	OS_TIMER	at_SessTimer;
	BYTE 		at_b_MaxLanSessNum;
	BYTE		at_b_SessIndex;
	BYTE		at_b_ActivSessNum;
	BYTE		at_b_SessHandle;
	BYTE		at_b_SessMutexGet;
	BYTE		at_b_MaxActivateSessNum;
	DWORD		at_dw_TSessionIDCount;
}St_SessVarTag;


#define at_SessTblMux           	at_p_St_GlobalVar->SessVarInfo.at_SessTblMux
#define at_SessTimer				at_p_St_GlobalVar->SessVarInfo.at_SessTimer
#define at_b_MaxLanSessNum          at_p_St_GlobalVar->SessVarInfo.at_b_MaxLanSessNum
#define at_b_SessIndex				at_p_St_GlobalVar->SessVarInfo.at_b_SessIndex
#define at_b_ActivSessNum			at_p_St_GlobalVar->SessVarInfo.at_b_ActivSessNum
#define at_b_SessHandle				at_p_St_GlobalVar->SessVarInfo.at_b_SessHandle
#define at_b_SessMutexGet			at_p_St_GlobalVar->SessVarInfo.at_b_SessMutexGet
#define at_b_MaxActivateSessNum		at_p_St_GlobalVar->SessVarInfo.at_b_MaxActivateSessNum
#define at_dw_TSessionIDCount		at_p_St_GlobalVar->SessVarInfo.at_dw_TSessionIDCount

#ifdef IPMI_RMCP_PLUS_SUB_SOL
typedef struct SolStruct
{
    BYTE		    at_b_SOLBufStatus;
    WORD 		    w_SolPoolLength;
    St_SOLStrTag  	    st_SOLVlb;
    St_SOLRetryTag 	    st_SOLRetry;
    St_SOLVolatileTag       st_SOLValVbl;
    OS_TIMER		    SOLRetryTimer;
    volatile unsigned char  b_SOLSetInProgress;
    OS_QUEUE		    at_SOLQueue;
    OS_SEMAPHORE	    at_SOLSmf;	
}St_SolVarTag;

#define at_p_SOL      		at_p_St_GlobalVar->SolVarInfo
#define at_b_SOLBufStatus       at_p_SOL.at_b_SOLBufStatus
#define at_w_SolPoolLength	at_p_SOL.w_SolPoolLength
#define at_St_SOLVlb		at_p_SOL.st_SOLVlb
#define at_St_SOLRetry		at_p_SOL.st_SOLRetry
#define at_dw_SOL_QueueID	at_p_SOL.dw_SOL_QueueID
#define at_SOLRetryTimer	at_p_SOL.SOLRetryTimer
#define at_St_SOLValVbl		at_p_SOL.st_SOLValVbl
#define at_SOLSmf               at_p_SOL.at_SOLSmf
#define at_b_SOLSetInProgress   at_p_SOL.b_SOLSetInProgress
#define at_SOLQueue             at_p_SOL.at_SOLQueue
#endif	

typedef struct WdtVarStruct
{
	St_WdtDataTag	at_St_WdtDataInfo;
	BOOLEAN			at_En_WdtPreTimeoutMsgFlag;
	BOOLEAN			at_En_WdtPreTimeoutStart;
	BYTE			at_b_WdtTimerStart;
	BYTE			at_b_WdtGetTimerUse;
	BYTE			at_b_WdtExpirationflag;
	OS_TIMER 		at_WDTTimer;
	OS_MUTEX 		at_IPMIWDTMTX;
	WORD			at_w_WDT_Pre_Timeout;
	WORD			at_w_WDT_Countdown;
	BOOLEAN			at_b_WDT_FIRST_INIT;
	BOOLEAN			at_b_WDT_LOG;
	BYTE			at_b_Pre_Timer_Flag;
	DWORD			at_dw_WDT_SLAVE_TASK;
}St_WdtVarTag;

#define at_St_WdtDataInfo           at_p_St_GlobalVar->WdtVarInfo.at_St_WdtDataInfo
#define at_En_WdtPreTimeoutMsgFlag	at_p_St_GlobalVar->WdtVarInfo.at_En_WdtPreTimeoutMsgFlag
#define at_En_WdtPreTimeoutStart	at_p_St_GlobalVar->WdtVarInfo.at_En_WdtPreTimeoutStart
#define at_b_WdtTimerStart			at_p_St_GlobalVar->WdtVarInfo.at_b_WdtTimerStart
#define at_b_WdtGetTimerUse			at_p_St_GlobalVar->WdtVarInfo.at_b_WdtGetTimerUse
#define at_b_WdtExpirationflag		at_p_St_GlobalVar->WdtVarInfo.at_b_WdtExpirationflag
#define at_WDTTimer					at_p_St_GlobalVar->WdtVarInfo.at_WDTTimer
#define at_IPMIWDTMTX				at_p_St_GlobalVar->WdtVarInfo.at_IPMIWDTMTX
#define at_w_WDT_Pre_Timeout		at_p_St_GlobalVar->WdtVarInfo.at_w_WDT_Pre_Timeout
#define at_w_WDT_Countdown			at_p_St_GlobalVar->WdtVarInfo.at_w_WDT_Countdown
#define at_b_WDT_FIRST_INIT			at_p_St_GlobalVar->WdtVarInfo.at_b_WDT_FIRST_INIT
#define at_b_WDT_LOG				at_p_St_GlobalVar->WdtVarInfo.at_b_WDT_LOG
#define at_b_Pre_Timer_Flag			at_p_St_GlobalVar->WdtVarInfo.at_b_Pre_Timer_Flag
#define at_dw_WDT_SLAVE_TASK		at_p_St_GlobalVar->WdtVarInfo.at_dw_WDT_SLAVE_TASK

#ifdef IPMI_FW_FIREWALL
typedef struct FirewallVarStruct
{
	unsigned short int at_SubStartOffset;
}St_FirewallVarTag;

#define at_SubStartOffset			at_p_St_GlobalVar->FirewallVarInfo.at_SubStartOffset
#endif


typedef struct PSVarStruct
{
	BYTE		at_b_PSFinishInitFlag;
	OS_MUTEX	at_PSMTX;
	OS_MUTEX	at_OEMPSMTX;
}St_PSVarTag;
#define at_b_PSFinishInitFlag   at_p_St_GlobalVar->PSVarInfo.at_b_PSFinishInitFlag
#define at_PSMTX				at_p_St_GlobalVar->PSVarInfo.at_PSMTX
#define at_OEMPSMTX				at_p_St_GlobalVar->PSVarInfo.at_OEMPSMTX

typedef struct UARTChnlVarStruct
{
	s_UARTChnlTag at_a_St_UARTChnlInfo[MAX_SER_CHNL];
	int	at_a_dw_UartFd[MAX_SER_CHNL];
	unsigned char at_b_IsUARTChnlInfoInit;
	BYTE UARTDevFileName[MAX_SER_CHNL][16];
}St_UARTChnlVarTag;

#define at_a_St_UARTChnlInfo        at_p_St_GlobalVar->UARTChnlVarInfo.at_a_St_UARTChnlInfo
#define at_a_dw_UartFd              at_p_St_GlobalVar->UARTChnlVarInfo.at_a_dw_UartFd
#define at_b_IsUARTChnlInfoInit     at_p_St_GlobalVar->UARTChnlVarInfo.at_b_IsUARTChnlInfoInit
#define UARTDevFileName             at_p_St_GlobalVar->UARTChnlVarInfo.UARTDevFileName

typedef struct WEBCGIVarStruct
{
	DWORD	at_dw_ipmi_pid[24];
	BYTE 	at_b_ipmi_pid_total;
	BYTE 	at_b_progress_percent;
	BYTE	at_b_online_counter;
	BYTE    at_b_upgradefail;
	WORD	at_w_lanconfig_flag;
	OS_MUTEX	at_unload_window_MTX;
	OS_TIMER	at_RebootTimer; 
	OS_MUTEX at_lock_FS_upload_MTX;
	OS_MUTEX 	at_csr_MTX;
	unsigned int  at_FS_size;
	unsigned int  at_FS_offset;
	BYTE	at_b_user_log_count[MAX_USER_NUM];
	DWORD   at_dw_user_log_time[MAX_USER_NUM];	
	OS_MUTEX  at_FILE_MTX;	
}St_WEBCGIVarTag;

#define ipmi_pid				at_p_St_GlobalVar->WEBCGIVarInfo.at_dw_ipmi_pid
#define ipmi_pid_total			at_p_St_GlobalVar->WEBCGIVarInfo.at_b_ipmi_pid_total
#define progress_percent		at_p_St_GlobalVar->WEBCGIVarInfo.at_b_progress_percent
#define online_counter			at_p_St_GlobalVar->WEBCGIVarInfo.at_b_online_counter
#define upgradefail				at_p_St_GlobalVar->WEBCGIVarInfo.at_b_upgradefail
#define lanconfig_flag			at_p_St_GlobalVar->WEBCGIVarInfo.at_w_lanconfig_flag
#define at_unload_window_MTX	at_p_St_GlobalVar->WEBCGIVarInfo.at_unload_window_MTX
#define at_RebootTimer			at_p_St_GlobalVar->WEBCGIVarInfo.at_RebootTimer
#define at_lock_FS_upload_MTX	at_p_St_GlobalVar->WEBCGIVarInfo.at_lock_FS_upload_MTX
#define at_FS_size				at_p_St_GlobalVar->WEBCGIVarInfo.at_FS_size
#define at_FS_offset			at_p_St_GlobalVar->WEBCGIVarInfo.at_FS_offset
#define at_b_user_log_count		at_p_St_GlobalVar->WEBCGIVarInfo.at_b_user_log_count
#define at_dw_user_log_time		at_p_St_GlobalVar->WEBCGIVarInfo.at_dw_user_log_time
#define at_csr_MTX				at_p_St_GlobalVar->WEBCGIVarInfo.at_csr_MTX
#define at_FILE_MTX				at_p_St_GlobalVar->WEBCGIVarInfo.at_FILE_MTX
#ifdef CHASSIS_MANAGER

typedef struct StateManager
{ BYTE Exist_SlaveAddress[64];
	BYTE Previous_State[64];
	BYTE Current_State[64];
	BYTE StateChangeFlg;
	BYTE StateChangeBMCAddr;
}St_StateManagerTag;

#define at_St_StateManager        at_p_St_GlobalVar->St_StateManagerInfo
#define at_Exist_SlaveAddress     at_p_St_GlobalVar->St_StateManagerInfo.Exist_SlaveAddress
#define at_Previous_State         at_p_St_GlobalVar->St_StateManagerInfo.Previous_State
#define at_Current_State          at_p_St_GlobalVar->St_StateManagerInfo.Current_State
#define at_StateChangeFlg         at_p_St_GlobalVar->St_StateManagerInfo.StateChangeFlg
#define at_StateChangeBMCAddr     at_p_St_GlobalVar->St_StateManagerInfo.StateChangeBMCAddr

#endif


typedef struct VSProfile
{
    BYTE MaxVSNum;
    BYTE totalVSnum;
    BYTE devAttribute[MAX_PLUGIN_NUM];
    struct in6_addr sin_addr[MAX_PLUGIN_NUM];
} St_VSProfileTag;

#define at_St_VSInfo              at_p_St_GlobalVar->St_VSInfo

typedef struct DCMIVar
{
    BYTE AssetTag;
	BYTE ATag[64];
} St_DCMIVarTag;

#define at_b_AssetTag             	at_p_St_GlobalVar->DCMIVarInfo.AssetTag
#define at_b_ATag             		at_p_St_GlobalVar->DCMIVarInfo.ATag


typedef struct RestoreBackupFuncStruct
{
	BYTE CurrentState;
	BYTE ServiceID; 
	BYTE ServiceDir; 
	BYTE LastProcLength; 
	DWORD SequenceNum; 
	DWORD CompleteLength; 
	DWORD DataLength;		
	DWORD LastUpdateTime;
	BYTE FuncIndex;
	BYTE FuncStatus; 
	BYTE UpgradeProgress; 
	BYTE BIOSBoardID[5];
	FILE *FileDesc;		
	int  MsgID;
	BYTE *ShmAddr;
	BYTE MEUpdateEnable;
	BYTE NVEraseEnable;
	BYTE b_SMBIOSEraseEnable;
}St_RestoreBackupFuncTag;

#define at_b_RBCurrentState		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.CurrentState
#define at_b_RBServiceID		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.ServiceID
#define at_b_RBServiceDir		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.ServiceDir
#define at_b_RBLastProcLength	at_p_St_GlobalVar->at_RestoreBackupFuncInfo.LastProcLength
#define at_dw_RBSequenceNum		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.SequenceNum
#define at_dw_RBCompleteLength	at_p_St_GlobalVar->at_RestoreBackupFuncInfo.CompleteLength
#define at_dw_RBDataLength		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.DataLength
#define at_dw_LastUpdateTime	at_p_St_GlobalVar->at_RestoreBackupFuncInfo.LastUpdateTime
#define at_b_RBFuncIndex		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.FuncIndex
#define at_b_RBFuncStatus		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.FuncStatus
#define at_b_RBUpgradeProgress	at_p_St_GlobalVar->at_RestoreBackupFuncInfo.UpgradeProgress
#define at_a_b_BIOSBoardID		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.BIOSBoardID
#define at_fd_RBFileDesc		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.FileDesc
#define at_dw_RBMsgID			at_p_St_GlobalVar->at_RestoreBackupFuncInfo.MsgID
#define at_p_RBShmAddr			at_p_St_GlobalVar->at_RestoreBackupFuncInfo.ShmAddr
#define at_b_MEUpdateEnable		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.MEUpdateEnable
#define at_b_NVEraseEnable		at_p_St_GlobalVar->at_RestoreBackupFuncInfo.NVEraseEnable
#define at_b_SMBIOSEnable       at_p_St_GlobalVar->at_RestoreBackupFuncInfo.b_SMBIOSEraseEnable

typedef struct bios_layout_info 
{
	DWORD	bios_offset; 
    DWORD	bios_size; 
    DWORD	me_offset;
    DWORD	me_size;
    DWORD	uimg_start_offset; 
	BYTE	bios_status;
	BYTE    ab_biosRevDate[64];
	
} St_BIOSLayoutInfo;


#define	at_dw_BiosFlashBOFFSET	at_p_St_GlobalVar->at_BiosLayoutInfo.bios_offset
#define at_dw_BiosFlashBSIZE    at_p_St_GlobalVar->at_BiosLayoutInfo.bios_size
#define	at_dw_MEFlashOFFSET		at_p_St_GlobalVar->at_BiosLayoutInfo.me_offset
#define at_dw_MEFlashSIZE    	at_p_St_GlobalVar->at_BiosLayoutInfo.me_size
#define at_dw_BiosFlashUOFFSET  at_p_St_GlobalVar->at_BiosLayoutInfo.uimg_start_offset
#define at_b_BiosUpdateStatus	at_p_St_GlobalVar->at_BiosLayoutInfo.bios_status
#define at_ab_biosRevDate       at_p_St_GlobalVar->at_BiosLayoutInfo.ab_biosRevDate

typedef struct GlobalVarStruct
{
	St_IPCKeyVarTag				IPCKeyVarInfo;
    St_ChassisDrvVarTag			ChassisDrvVarInfo;
	St_ChassisVarTag			ChassisVarInfo;
    St_ChnlVarTag				ChnlVarInfo;
    St_EvtVarTag				EvtVarInfo;
    St_GlobalCMDVarTag			GlobalVarInfo;
	St_NTPVarTag				NTPVarInfo;
#ifdef ATEN_TFTP_FEATURE
	St_TFTPDVarTag				TFTPDVarInfo;
#endif
	St_DateTimeVarTag			DateTimeVarInfo;
	  St_LANVarTag				LANVarInfo;
    St_PEFVarTag				PEFVarInfo;
#ifdef IPMI_RMCP_PLUS
    St_RMCPplusVarTag			RMCPplusVarInfo;
#endif
	  St_PayloadVarTag			PayloadVarInfo;
   St_SDRVarTag				SDRVarInfo;
	 St_SDRHeaderTag			SDRHeaderInfo;   
   St_GetMsgTag          GetMsgInfo;
    St_SELVarTag				SELVarInfo;
    St_SessVarTag				SessVarInfo;
#ifdef IPMI_RMCP_PLUS_SUB_SOL
	St_SolVarTag				SolVarInfo;
#endif
	St_WdtVarTag				WdtVarInfo;
	St_PSVarTag					PSVarInfo;
    St_UARTChnlVarTag			UARTChnlVarInfo;
    
    
    OS_MUTEX                    I2CMutexInfo[8];    
    OS_MUTEX			I2COPMutex; 
    OS_MUTEX			at_I2C_STOP_CONT_FLAG_MUX;    
    DWORD                       at_dw_SYSTEM_TIMER;
    DWORD						at_dw_SystemTick;
    St_WEBCGIVarTag				WEBCGIVarInfo;
    int				FlashFd[TOTAL_FLASH_TYPE];
    St_SensorVarTag				SensorVarInfo;
    int       at_RandomSeed;
#ifdef CHASSIS_MANAGER          
    St_StateManagerTag          St_StateManagerInfo;
#endif
    unsigned char				ucNVRAMDirty;
    unsigned char				ucNVRAMInitReady;
    unsigned char 				ucNVRAMInitCnt;
    St_VSProfileTag             St_VSInfo;
    int 						at_IKVMPid;
    unsigned char 					at_GPIO_data[2];
    unsigned char 					at_GPIO_dir[2];
#ifdef IPMI_FW_FIREWALL
	St_FirewallVarTag			FirewallVarInfo;
#endif
#ifdef IPMI_DCMI_FEATURE	
	St_DCMIVarTag				DCMIVarInfo;
#endif	
    WORD						at_w_PlatformId;
    unsigned int 				at_Critial_Flag;
    unsigned int 				at_Critial_Times;
    unsigned int				at_LAST_POWERSTATUS;
    unsigned int				at_CURRNT_POWERSTATUS;
    unsigned int 				at_HostBoot_Time;
    unsigned int				at_POWER_OFF_ON;
    unsigned int				at_I2C_STOP_CONT_FLAG;
    unsigned char				at_FW_UpdateMode;
    unsigned char				at_Preserve_Config;

#if defined(CONFIG_CPU_WINBOND_HERMON) || defined(CONFIG_CPU_NUVOTON_YARKON)	
    OS_MUTEX					at_PostCodeMtx;
#endif
	OS_MUTEX					at_UIDCtrlMtx;
	BYTE						at_UIDStatus;
	BYTE						at_UidWorkTime;
	BYTE 						at_b_selBladeNum;
	BYTE 						at_b_alarmReset;
	BYTE 						at_b_fan_assert_count;
	BYTE 						at_b_temp_assert_count;
	BYTE						at_b_psu_assert_count;
	BYTE						at_b_blade_assert_count;
	BYTE						at_b_blade_scan_timer;
	BYTE						at_b_blade_assert[30];
	BYTE						at_b_blade_prev_assert[30];	
	BYTE						at_b_DumpFlashComplete;	
	BYTE						at_b_GPU_InitAdjust;

	St_OEMGlobalVarTag			at_OEMGlobalVarInfo;	
        OS_MUTEX					at_RestoreBackupMtx;
	St_RestoreBackupFuncTag		at_RestoreBackupFuncInfo;
	St_BIOSLayoutInfo			at_BiosLayoutInfo;
	St_UserExPlainInfoTag		UserExVarInfo[MAX_USER_NUM];
}St_GlobalVarTag;

#define at_b_selBladeNum	at_p_St_GlobalVar->at_b_selBladeNum
#define at_b_alarmReset		at_p_St_GlobalVar->at_b_alarmReset
#define at_b_fan_assert_count	at_p_St_GlobalVar->at_b_fan_assert_count
#define at_b_temp_assert_count	at_p_St_GlobalVar->at_b_temp_assert_count
#define at_b_psu_assert_count	at_p_St_GlobalVar->at_b_psu_assert_count
#define at_b_blade_assert_count	at_p_St_GlobalVar->at_b_blade_assert_count
#define at_b_blade_scan_timer at_p_St_GlobalVar->at_b_blade_scan_timer
#define at_b_blade_assert at_p_St_GlobalVar->at_b_blade_assert
#define at_b_blade_prev_assert at_p_St_GlobalVar->at_b_blade_prev_assert

#if defined(CONFIG_CPU_WINBOND_HERMON) || defined(CONFIG_CPU_NUVOTON_YARKON)
#define at_PostCodeMtx        at_p_St_GlobalVar->at_PostCodeMtx
#endif
#define at_UIDCtrlMtx        at_p_St_GlobalVar->at_UIDCtrlMtx
#define at_UIDStatus         at_p_St_GlobalVar->at_UIDStatus
#define at_UidWorkTime         at_p_St_GlobalVar->at_UidWorkTime
#define at_IKVMPid				at_p_St_GlobalVar->at_IKVMPid
#define at_RandomSeed		at_p_St_GlobalVar->at_RandomSeed
#define at_dw_SYSTEM_TIMER        at_p_St_GlobalVar->at_dw_SYSTEM_TIMER
#define at_dw_SystemTick		 at_p_St_GlobalVar->at_dw_SystemTick
#define I2CMutexInfo        at_p_St_GlobalVar->I2CMutexInfo
#define I2COPMutex			at_p_St_GlobalVar->I2COPMutex
#define at_I2C_STOP_CONT_FLAG_MUX        at_p_St_GlobalVar->at_I2C_STOP_CONT_FLAG_MUX
#define	FlashFd		at_p_St_GlobalVar->FlashFd
#define	at_ucNVRAMDirty		at_p_St_GlobalVar->ucNVRAMDirty
#define	at_ucNVRAMInitReady		at_p_St_GlobalVar->ucNVRAMInitReady
#define	at_ucNVRAMInitCnt		at_p_St_GlobalVar->ucNVRAMInitCnt
#define	at_GPIO_data		at_p_St_GlobalVar->at_GPIO_data
#define	at_GPIO_dir		at_p_St_GlobalVar->at_GPIO_dir
#define at_w_PlatformId  at_p_St_GlobalVar->at_w_PlatformId
#define at_Critial_Flag at_p_St_GlobalVar->at_Critial_Flag
#define at_Critial_Times at_p_St_GlobalVar->at_Critial_Times
#define at_LAST_POWERSTATUS at_p_St_GlobalVar->at_LAST_POWERSTATUS
#define at_CURRNT_POWERSTATUS at_p_St_GlobalVar->at_CURRNT_POWERSTATUS
#define at_HostBoot_Time at_p_St_GlobalVar->at_HostBoot_Time
#define at_POWER_OFF_ON at_p_St_GlobalVar->at_POWER_OFF_ON
#define at_I2C_STOP_CONT_FLAG at_p_St_GlobalVar->at_I2C_STOP_CONT_FLAG
#define at_FW_UpdateMode at_p_St_GlobalVar->at_FW_UpdateMode
#define at_Preserve_Config at_p_St_GlobalVar->at_Preserve_Config
#define at_b_DumpFlashComplete 	at_p_St_GlobalVar->at_b_DumpFlashComplete

#define at_RestoreBackupMtx			at_p_St_GlobalVar->at_RestoreBackupMtx
#define at_b_GPU_InitAdjust         			at_p_St_GlobalVar->at_b_GPU_InitAdjust
#define at_St_UserExVarInfo			at_p_St_GlobalVar->UserExVarInfo

#ifndef _SYSCALL_C_
extern St_GlobalVarTag		*at_p_St_GlobalVar;
extern St_SessTblTypeTag	*at_p_a_St_SessTbl;
extern St_SensorDataTag		*at_St_SensorData;
extern St_FruDataTag		*at_p_St_FruData;
extern const St_CipherSuiteInfoTag at_a_St_CipherSuite[];

extern const BYTE at_b_SupportCipherSuiteCount;
#else
const St_CipherSuiteInfoTag at_a_St_CipherSuite[]= 
{
   // {0x00,0x00,0x00,0x00},
	{0x01,0x01,0x00,0x00}
    ,{0x02,0x01,0x01,0x00}
#ifdef IPMI_RMCP_PLUS_SUB_AES   
    ,{0x03,0x01,0x01,0x01}
#endif    
    ,{0x06,0x02,0x00,0x00}
    ,{0x07,0x02,0x02,0x00}
#ifdef IPMI_RMCP_PLUS_SUB_AES    
    ,{0x08,0x02,0x02,0x01}
#endif    
	,{0x0B,0x02,0x03,0x00}
#ifdef IPMI_RMCP_PLUS_SUB_AES    
	,{0x0C,0x02,0x03,0x01}
#endif   	
};


const BYTE at_b_SupportCipherSuiteCount = sizeof(at_a_St_CipherSuite)/sizeof(St_CipherSuiteInfoTag);
#endif 


#endif



