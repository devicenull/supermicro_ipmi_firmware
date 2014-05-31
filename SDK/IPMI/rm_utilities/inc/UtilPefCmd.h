
#ifndef _UTILPEFCMD_H_
#define _UTILPEFCMD_H_

#define  DEST_TYPE_PET_TRAP  	0x00
#define  DEST_TYPE_OEM1			0x06     
#define  DEST_TYPE_OEM2			0x07     

#ifdef _UTILPEFCMD_C_
STATUS UtilPETAcknowledgeFilter(BYTE *p_b_ReqData);
BOOLEAN UtilPEFPostponeTimerWakeupCheck(void);
WORD UtilGetPETSequenceNO(void);
void UtilImmedPETEvent(
                    DWORD dw_EventID,
                    void  *Ptr
                  );
void UtilResetLastProcessedEventID(void);
#else
extern STATUS UtilPETAcknowledgeFilter(BYTE *p_b_ReqData);
extern BOOLEAN UtilPEFPostponeTimerWakeupCheck(void);
extern WORD UtilGetPETSequenceNO(void);
extern void UtilImmedPETEvent(
                     DWORD dw_EventID,
                     void  *Ptr
                   );
extern void UtilResetLastProcessedEventID(void);
#endif

#endif


