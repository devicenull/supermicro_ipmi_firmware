
#ifndef _SOL_H_
#define _SOL_H_

#ifdef IPMI_RMCP_PLUS_SUB_SOL

#ifdef IPMI_RMCP_PLUS_SUB_SOL_BASIC
#define SOL_STACK_SIZE                                              1024
#else
#define SOL_STACK_SIZE                                              2048
#endif
#define SOL_MSG_Q_SIZE                                              128

#define ALL_SOL_EVENT                                               0xFFFFFFFF

#define AUTH_TYPE_FORMAT_OEM                                        0x05
#define AUTH_TYPE_FORMAT_RMCP_PLUS                                  0x06

#define ACK_WAITING                                                 1

#define SOL_LOGICAL_CHANNEL_NO                                      0

#define SOL_RETRY_BUFFER_STATUS                                     0
#define SOL_SEQ_NO_ACK_NACK                                         1
#define SOL_RETRY_TIME                                              2
#define SOL_RETRY_DATA_LENGTH                                       3
#define SOL_RETRY_DATA_BUFFER                                       4

#define SOL_RETRY_BUFFER_NO                                         1
#define SOL_RETRY_BUFFER_SIZE                                       5

#define SOL_RETRY_BUFFER_FULL                                       0xFF

#define SOL_RETRY_PUSH_Q                                            0x00000010
#define SOL_PARTIAL_SEND_PUSH_Q                                     0x00000020
#define SOL_PARTIAL_SEND_FINISH										0x00000040


typedef struct
{
    BYTE b_LglChnNo;
    BYTE b_SOLSeqNo;
    BYTE b_ACKSeqNo;
    BYTE b_ACKStatus;
} St_SOLStrTag;


extern STATUS 	SOLTaskInit(void);
extern STATUS 	SOLTrnEncode(WORD w_DataLnth, BYTE *p_a_b_SOLOutBuf, DWORD *dw_DataSize, BYTE b_IA, BYTE b_CA, void *p_SIK, void *p_SIK2);
extern void   	SOLLanPkgPushQ(void);
extern STATUS 	SOLReceiveLAN(BYTE , void *, void *,void *);
extern STATUS 	SOLUARTSend(void *,void *);
extern void 	SOLRetryFunc(DWORD dw_dummy);
extern void 	SOLHWTimerFun(DWORD dummy);
extern STATUS  	SOLReceiveUART(BYTE ,void *,void *);
extern BYTE 	SOLPartialSend(BYTE ,void *);
extern void    SOLMsgHandler ();
void Soluartsendapply(DWORD dw_event, void *ptr);

#endif  


typedef struct
{
    BYTE b_Status;
    BYTE b_SeqNo;
    BYTE b_RetryTimes;
    BYTE b_Index;
    WORD b_Datalen;
    BYTE ab_Buf[400];    
} St_SOLRetryTag;


#endif  






