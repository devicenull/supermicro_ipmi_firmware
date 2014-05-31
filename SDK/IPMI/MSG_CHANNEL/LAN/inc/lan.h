#ifndef _LAN_COMMON_H_
#define _LAN_COMMON_H_

#include "Net_API.h"


#define AUTH_TYPE_RMCP_PLUS         0x06
#define AUTH_TYPE_LOC               4
#define SESSION_SEQ_NO_LOC          5
#define SESSION_ID_LOC              9
#define AUTH_CODE_LOC               13
#define IPMI_MSG_LEN_LOC(x)                 ((x == 0) ? 13 : 29)
#define IPMI_MSG_DATA_LOC(x)                ((x == 0) ? 14 : 30)
#define IPMI_CMD_LOC(x)                     ((x == 0) ? 19 : 35)
#define RMCPHEADER_PLUS_IPMISESSION_SIZE(x) ((x == 0) ? 14 : 30) 
#define RMCPPLUS_IPMI_CMD_LOC(x)            ((x == 0) ? 21 : 37)   
#define CHECKSUM_VERIFY             0
#define CHECKSUM_APPEND             1



typedef struct
{
    BYTE              ab_Data[SIZE_OF_UDP_RECV_BUF];

    WORD              w_Len;

    WORD              w_SrcPort;

    DWORD             dw_SrcIP;
}St_UDPRecvInfoTag;

typedef struct
{
#ifdef  OS_THREAD_X
    NX_PACKET_POOL  PacketPool;
    
    BYTE            *PacketBuf;

    NX_IP           IP;
    
    BYTE            IPThreadStack[IP_THREAD_STACK_SIZE];
    
    NICDriverFunc   IPLinkDriver;
       
   
    
    TX_THREAD       UDPThread;
    
    WORD            UDPThreadStack[UDP_THREAD_STACK_SIZE/2];
    
    BYTE            ArpCacheMemory[ARP_CACHE_MEM_SIZE];

    TX_SEMAPHORE    UDPRecvSemaphore;
    
    TX_SEMAPHORE    LANSendSemaphore; 

    TX_QUEUE	    *p_ChanlMsgQueGrp;
#endif
    
    BYTE            	b_LogChanlID;
    Socket   			SocketDesc;
	St_UDPRecvInfoTag   st_UDPRecvInfo;
}St_NICTag;






#ifdef _LAN_COMMON_C_
WORD   BYTE2WORD(BYTE ,BYTE );
DWORD  BYTE2DWORD(BYTE ,BYTE ,BYTE ,BYTE );
void   ActiveSessResp(DWORD , void *);
STATUS ASFMsgHandler(St_NICTag *);
#ifdef IPMI_RMCP_PLUS
STATUS DecodeRMCPPlusIPMIPKG(BYTE , void *, void *);
STATUS DecodeRMCPPlusSessPKG(WORD , void *, void *);
#endif
STATUS IPMIMsgCheckSumHandler(BYTE , WORD , BYTE *);
BYTE   UDPSendData(BYTE ,void *, DWORD ,DWORD ,WORD );
#else 
extern STATUS LAN_Init(BYTE ,void *,void *) ;
extern STATUS LAN_Recv( BYTE , void *, void * );
extern STATUS LAN_SetMisc(BYTE , void *, void *);
extern STATUS LAN_Transmit(BYTE ,void *,void *);
extern BYTE   UDPSendData(BYTE ,void *, DWORD ,DWORD ,WORD );
extern STATUS ASFMsgHandler(St_NICTag *);
extern void CHANGE_USHORT_ENDIAN(void *value);
extern void CHANGE_ULONG_ENDIAN(void *value);
extern STATUS SocketInit(BYTE b_DevID);              
#endif


#endif

