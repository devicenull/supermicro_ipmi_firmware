
#ifndef _PEF_H_
#define _PEF_H_


#define MAX_PET_OUTGOING_BUF_SIZE   250

#define SNMP_TRAP_PORT_NO   162


typedef enum
{
    PEF_POWER_DOWN_PRIORITY = 1,
    PEF_POWER_CYCLE_PRIORITY,
    PEF_POWER_RESET_PRIORITY,
    PEF_DIAGNOSTIC_INTERRUPT_PRIORITY,
    OEM_ACTION_PRIORITY = 0x80
    
} En_PEFActionPrioritiesTag;

typedef enum
{
    PET_READY = 0,
    PET_WAIT,
    PET_DONE,
    PET_RETRY,
    PET_TIMEOUT,
    PET_NOT_DONE,
    PET_PROCESSING,
    PEF_ACK_WITH_NORETRY
} En_PETStatusTag;

typedef enum
{
    OEM_PET_ACTION_NULL = 0,
    OEM_PET_ACTION_INIT,
	OEM_PET_ACTION_ALERT_STRING
} En_OEMPetActionTag;

typedef struct
{
    BYTE b_Type;
    BYTE b_AQCnt;
    
} St_PEFActionTag;

typedef struct
{
    BYTE PNP;               
    BYTE Channel;
    BYTE Destination;
    BYTE AlertStringNo;
    BYTE FilterNumber;
    BYTE Status;
    BYTE AckTimeoutTimer;
    BYTE RetryCount;
    WORD SequenceNo;
    DWORD LocalTimestamp;
} St_AlertQueueTag;   

typedef struct
{
    BYTE RecordID[2];
    BYTE RecordType;
    BYTE Timestamp[4];
	BYTE GeneratorID[2];
	BYTE EvMRev;
	BYTE SensorType;
	BYTE SensorNo;
	BYTE EventDirType;
	BYTE EventData[3];

#ifdef SMTP
	BYTE b_DevID;
#endif
	
} St_EventMsgRecordTag;


typedef struct
{
	St_EventMsgRecordTag *p_St_EventData;
	St_AlertQueueTag *p_St_AQ;
	BYTE *p_b_buffer;
	BYTE *p_b_idx;
} St_PETActionTag;

typedef struct
{
    BYTE action_code;
    void (*pet_func)(St_PETActionTag* p_St_PETObj);
} PETActionTblTag;


typedef struct
{
    BYTE PostponeTimer;
    BYTE PostponeTimer_backup;
    BYTE PostponeInprogress;
    BYTE PEFDisable;        
    BYTE SetInProgress;
    WORD w_SeqNo;

} St_PefVolatileInfoTag;      

#ifdef _PEF_C_

void PEFTask(DWORD dw_input);
void PEFPostponeTimerExpiryFunction(DWORD input);
STATUS GetEventMsgFromSEL(void);
STATUS SELRecordCheck(BYTE b_DelayFlg, WORD w_RecordID);
STATUS EventFilterMatch(void);
STATUS CompareFilterParams(St_PEFEntryTag *p_PEFEntry);
BYTE EventFilterActionDecision(BYTE EventAction);
void PolicyTableMatch(BYTE FilterNo, BYTE PolicyNo);
BYTE AlertStringMatch(BYTE FilterNo, BYTE StringKey);
void EventFilterActionHandler(void);
void EventAlertHandler(St_EventMsgRecordTag *p_EventData, St_AlertQueueTag *p_AlertQueue, BYTE *p_AlertBuf);
void SendAlertTrapViaLAN(St_EventMsgRecordTag *p_EventData, St_AlertQueueTag *p_AQ, St_LanDestInfoTag *p_LanAlertDest, BYTE *p_AlertBuf);
void EventActionDecision(BYTE *p_Type, BYTE Action);
void RecordLastBMCProcessedEventID(WORD w_RecordID);    
WORD PETMakeOutgoingData(St_EventMsgRecordTag *p_EventData, St_AlertQueueTag *p_AQ, BYTE *);
BYTE PETFillAlertString(BYTE *p_DestBuf, BYTE *p_SrcBuf);
void ImmedPETEvent(DWORD dw_EventID, void  *Ptr);
WORD GetPETSequenceNO(void);
STATUS PETAcknowledgeFilter(BYTE *p_ReqData);
void PEFPostponeTimerWakeupCheck(void);
BYTE SensorNoToEntityID(BYTE b_SensorNo);
BYTE SMTPClientSend(BYTE *p_smtpbuf);
void TimeStringArrange(DWORD dw_TimeTmp, void *p_String);
void DecodeTimeStamp(DWORD *p_TimeStamp, WORD *p_Year);
DWORD Hex2Dec(DWORD number, BYTE *string);
void Bin2Hex(unsigned char b_data, unsigned char *string);
void GetSensorString(BYTE b_sensor, void *string);
void SpecificSensorDescript(unsigned char b_sensor_type, St_EventMsgRecordTag	*p_SELData, unsigned char *data);
static void timesub(DWORD *timep, struct tm *tmp);
STATUS UDPPETSendData(BYTE* p_b_AlertBuf, WORD w_OutputLen,DWORD dw_IPAddr);
#else
extern void PEFPostponeTimerWakeupCheck(void);
extern STATUS PETAcknowledgeFilter(BYTE *p_ReqData);
extern WORD GetPETSequenceNO(void);


#endif  


#endif  





