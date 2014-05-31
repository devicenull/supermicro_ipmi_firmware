#ifndef _ICMB_H_
#define _ICMB_H_  

#ifdef IPMI_ICMB_BRIDGE

#define TOTAL_ICMB_CHANNEL                          1
#define TOTAL_ICMB_BUFFER                           2
#define ICMB_SAVE_SEQ_BUFFER_SIZE                   4

#define ICMB_DATA_LENGTH                            32
#define ICMB_BUFFER_SIZE                            35


#define ICMB_RETRY_TIME_PERIOD                      6




typedef struct
{
    BYTE b_Used;
    BYTE b_LglChnID;
    BYTE b_PhlChnID;
    BYTE b_RdDataSize[TOTAL_ICMB_BUFFER];
    BYTE b_WrtDataSize;
    BYTE b_SeqNo;
    BYTE b_PKGCounter[TOTAL_ICMB_BUFFER + 1];
    BYTE ab_ICMBRdBuffer[TOTAL_ICMB_BUFFER][ICMB_BUFFER_SIZE];
    BYTE ab_ICMBWrtBuffer[ICMB_BUFFER_SIZE];
    WORD w_ICMBBriAddr;
    TX_QUEUE	*p_ChanlMsgQueGrp;
    St_IPMB_BUFFER_INFOTag   s_BufInfo;
}ICMB_VARIABLE_TYPE;


typedef struct
{
    BYTE b_Used;
    BYTE b_ICMBGenSeq;
    BYTE b_RqSeq;
    BYTE b_Lun;
    BYTE b_rmtBr_L;
    BYTE b_rmtBr_H;
    BYTE b_cmd;
    BYTE b_Rq_netFnrsLun;
    BYTE b_channel;   
    DWORD dw_CreatTime;
}ICMBSaveSeqBuffer;


#ifdef _ICMB_C_

extern STATUS TrnEncode2ICMBpkg(BYTE, void *);
extern STATUS RcvDecode2MsgPkg(BYTE, void *);
extern void ICMBTimerFunc(DWORD dw_Info);
extern STATUS ICMBGetSeqNo(void *);
extern STATUS ICMBGetBufNo(void *, BYTE);
extern STATUS ReadytoFraming(BYTE,void *);
extern void Move(BYTE ,void *,BYTE ,BYTE);


#endif   

extern STATUS ICMB_Init(BYTE, void *, void *);
extern STATUS ICMB_Recv(BYTE, void *, void *);
extern STATUS ICMB_Transmit(BYTE, void *, void *);
extern STATUS ICMB_SetMisc(BYTE, void *, void *);
extern void ICMBGetBriAddr(void *,void *);
extern STATUS ICMBSetBriAddr(WORD, WORD);
extern STATUS MsgGetSeqNo(void *);

#endif   

#endif  





