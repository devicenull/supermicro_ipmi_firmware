
#ifndef _EVENT_H_
#define _EVENT_H_

#ifdef OWN_AST_HOST_CPU
#define EVENT_PROC_STACK_SIZE  (512 + 64)
#else
#define EVENT_PROC_STACK_SIZE  512
#endif
#define EVENT_TASK_QNUMS       8
#define EVENT_MSG_BUFF_NUMS    1 

#define EVT_DATA_TYPE_EVTRECORD     0x01
#define EVT_DATA_TYPE_SETRCVER      0x02
#define EVT_DATA_TYPE_GETRCVER      0x03

#define EVT_STAT_FLG_CLEAR          0x00
#define EVT_STAT_FLG_SETRCVER       0x01
#define EVT_STAT_INIT_OK            0x02
#define EVT_STAT_MSGBUFF_ENABLE     0x04
#define EVT_STAT_MBUFF_INT          0x08

#define EVT_RECORD_MAXSIZE          9   

#ifdef _EVENT_C_

#else 

extern BOOLEAN UtilEvtEvtMsgBuffFull(void);
extern BOOLEAN UtilEvtEvtMsgBuffIntEnable(void);
extern STATUS UtilEvtSetEvtMsgBuffInt(BOOLEAN enable);
extern STATUS UtilEvtMsgRecvEnable(BOOLEAN enable);

#endif 
#endif 




