




#ifndef _PEFCMD_H_
#define _PEFCMD_H_




#define MAX_PEF_TABLE_ENTRY             16
#define MAX_ALERT_POLICY_TABLE_ENTRY    16

#ifdef OEM_ARIMA_ALERT_STRING_SIZE
#define MAX_ALERT_STRING_ENTRY          16
#else
#define MAX_ALERT_STRING_ENTRY          2
#endif

#define BYTES_OF_ONE_BLOCK              16
#define MAX_ALERT_STRING_LENGTH         32
#define NUMBERS_OF_BLOCK                (MAX_ALERT_STRING_LENGTH / BYTES_OF_ONE_BLOCK)


#define PARAM_SET_IN_PROGRESS                           0
#define PARAM_PEF_CONTROL                               1
#define PARAM_PEF_ACTION_GLOBAL_CONTROL                 2
#define PARAM_PEF_STARTUP_DELAY                         3
#define PARAM_PEF_ALERT_STARTUP_DELAY                   4
#define PARAM_NUMBER_OF_EVENT_FILTERS                   5
#define PARAM_EVENT_FILTER_TABLE                        6
#define PARAM_EVENT_FILTER_TABLE_DATA_1                 7
#define PARAM_NUMBER_OF_ALERT_POLICY_ENTRIES            8
#define PARAM_ALERT_POLICY_TABLE                        9
#define PARAM_SYSTEM_GUID                               10
#define PARAM_NUMBER_OF_ALERT_STRINGS                   11
#define PARAM_ALERT_STRING_KEYS                         12
#define PARAM_ALERT_STRINGS                             13
#define PARAM_NUMBER_OF_GROUP_CONTROL_TABLE_ENTRIES     14
#define PARAM_GROUP_CONTROL_TABLE                       15

#define PROCESSED_BY_BMC        0x01
#define PROCESSED_BY_SMS        0x02

#define PEF_DIAGNOSTIC_INTERRUPT    0x20
#define PEF_OEM_ACTION              0x10
#define PEF_POWER_CYCLE             0x08
#define PEF_RESET                   0x04
#define PEF_POWER_DOWN              0x02
#define PEF_ALERT                   0x01

#define PEF_ALERT_STARTUP_DELAY_ENABLE  0x08
#define PEF_STARTUP_DELAY_ENABLE        0x04

#define ALERT_IMMEDIATE_INIT_ALERT      0
#define ALERT_IMMEDIATE_GET_STATUS      1
#define ALERT_IMMEDIATE_CLEAR_STATUS    2

#define ALERT_POLICY_TABLE_LENGTH		3


#pragma pack(push,1) 
typedef struct
{
    BYTE    FC;                  
    BYTE    EFA;                 
    BYTE    APN;                 
    BYTE    ES;                  
    BYTE    GIDB1;               
    BYTE    GIDB2;               
    BYTE    ST;                  
    BYTE    SN;                  
    BYTE    ET;                  
    BYTE    ED1EOM[2];           
    BYTE    ED1AM;               
    BYTE    ED1C1;               
    BYTE    ED1C2;               
    BYTE    ED2AM;               
    BYTE    ED2C1;               
    BYTE    ED2C2;               
    BYTE    ED3AM;               
    BYTE    ED3C1;               
    BYTE    ED3C2;               

} St_PEFEntryTag;   
#pragma pack(pop) 

#pragma pack(push,1) 
typedef struct
{
    BYTE    PNP;             
    BYTE    CNDS;           
    BYTE    ASK;            

} St_APTEntryTag;   
#pragma pack(pop) 
#pragma pack(push,1)
typedef struct
{
    BYTE    EFN;            
    BYTE    ASS;            
    BYTE    String[NUMBERS_OF_BLOCK][BYTES_OF_ONE_BLOCK];    

} St_ASEntryTag;   
#pragma pack(pop) 
#pragma pack(push,1) 
typedef struct
{
    BYTE    Flag;            
    BYTE    Data[16];       
} St_GUIDInfoTag;   
#pragma pack(pop) 
#pragma pack(push,1) 
typedef struct
{
    BYTE PEFCtrl;   
    BYTE PEF_AGC;   
    BYTE PEF_SD;    
    BYTE PEF_ASD;   
    BYTE LastProcIDBySW[2];     
	BYTE LastProcIDByBMC[2];    
    St_GUIDInfoTag  GUID;
    St_PEFEntryTag  PEFEntry[MAX_PEF_TABLE_ENTRY];
    St_APTEntryTag  APTEntry[MAX_ALERT_POLICY_TABLE_ENTRY];
    St_ASEntryTag   ASEntry[MAX_ALERT_STRING_ENTRY];
} St_PEFInfoTag;      
#pragma pack(pop) 

#ifdef _PEFCMD_C_


#else

extern CMD_STATUS GetPEFCapabilityCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS ArmPEFPostponeTimerCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetPEFPCfgParamCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetPEFPCfgParamCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SetLastProcessedEventIDCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetLastProcessedEventIDCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS PEFImmediateCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS PEFAcknowledgeCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);

extern void ResetLastProcessedEventID(void);
extern void LoadPEFInfoDefault2PS(void);
extern void PefInfoVolatileInit(void);
extern void PEFEntryTablePreConfig(BYTE b_p1, BYTE b_p2, void *p_b_p3);

extern BYTE ApiArmPEFPostponeTimerSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE ApiSetPEFPCfgParamSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE ApiPEFImmediateSubFnCheck(MsgBufFmt *p_St_MsgBufIn);

#endif  


#endif  





