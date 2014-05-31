#ifndef _MSGBUF_H_
#define _MSGBUF_H_


#ifdef IPMI_RMCP_PLUS
//#define MSG_BUF_DATA_LENGTH 256
#define MSG_BUF_DATA_LENGTH 1500
#else
//#define MSG_BUF_DATA_LENGTH 226
#define MSG_BUF_DATA_LENGTH 1500
#endif




typedef struct MsgBufFmtTag
{
    void *whoami;
	unsigned char		b_AuthType;
	unsigned char	 	b_Pri;
#ifdef IPMI_RMCP_PLUS
    unsigned char		b_PayloadType;
#endif
#ifdef IPMI_ICMB_BRIDGE
	unsigned char 		b_MsgType;
#endif	
    unsigned char		b_rsAddr;
    unsigned char 		b_netFnrsLun;
    unsigned char 		b_rqAddr;
    unsigned char 		b_rqSeqrqLUN;
    unsigned short int 	w_datalength;
    unsigned char 		b_cmd;
    unsigned char 		b_data[MSG_BUF_DATA_LENGTH];
   
    union
    {
    	unsigned char b_SourceIP[4];
    	unsigned char b_DestIP[4];
    } IP;	
    union
    {
    	unsigned char b_SourcePort[2];
    	unsigned char b_DestPort[2];
    } Port;	
    union
    {
    	unsigned char b_SourceChannel;
    	unsigned char b_DestChannel;
    } Channel;	
    union
    {
    	unsigned char b_SourcePhysicalChannel;
    	unsigned char b_DestPhysicalChannel;
    } PhysicalChannel;	
  unsigned long int dw_SessSeqNum;
  unsigned long int dw_SessID;
} MsgBufFmt;





typedef struct MsgBlkStructTag
{
    MsgBufFmt PrimaryBuf;
#ifdef IPMI_ICMB_BRIDGE
	MsgBufFmt SecondBuf;
#endif	    
    
    unsigned long int dw_MsgExpireTime;
    struct MsgBlkStructTag *p_PreBlk;  
    struct MsgBlkStructTag *p_NextBlk; 
    
} MsgBlkStruct;


#define NULL_MSG (MsgBlkStruct  *) 0





typedef enum
{
    CMD_SUCCESS = 0,
    CMD_NOREPLY
} CMD_STATUS;


#ifdef _MSGBUF_C_

#else
extern void MsgBufPoolInit(void);
extern MsgBlkStruct  *MsgBufAlloc(void);
extern void MsgBufRelease(MsgBlkStruct  *pMsgBuf);
extern void MsgBufCopy(MsgBufFmt *pSourceMsgBuf,MsgBufFmt *pDestMsgBuf);
#endif    

#endif  




