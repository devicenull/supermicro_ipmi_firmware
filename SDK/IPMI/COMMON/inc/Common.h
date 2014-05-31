
#ifndef _COMMON_H_
#define _COMMON_H_

#define PEF_VERSION 									0x51

#ifdef OS_THREAD_X
#define IPMI_VERSION 									0x02
#define SDR_VERSION 									0x51
#define SEL_VERSION 									0x51



#define OFF                                             0
#define ON                                              1

#define DEFAULT_BMC_ADDRESS                             0x20
#define DEFAULT_ICMB_BRIDGE_DEVICE_ADDRESS              0x3412

typedef struct
{
    unsigned char b_BMCAddStatus;
    unsigned char b_BMCAdd;
    unsigned char b_DEVICEGUID[16];
    unsigned char b_BMCRestorePolicy;
    unsigned char b_IsPowerOn;
    unsigned char b_PowerCycleInterval;
    unsigned short int w_SelReservedID;
#ifdef BMC_DEBUG_PORT    
    unsigned char b_DebugPort;
#endif    
}St_BMCControllerTag;

#ifdef IPMI_ICMB_BRIDGE

#define ICMB_RUNTCMD      0x0001       
#define ICMB_BADCOMMAND   0x0002       
#define ICMB_SPURIOUSRESP 0x0004       
#define ICMB_SENT         0x0008       
#define ICMB_RECEIVED     0x0010       
#define ICMB_OTHERADDR    0x0020       
#define ICMB_RUNTPKT      0x0040       
#define ICMB_BADCHECKSUM  0x0080       
#define ICMB_UNKTYPE      0x0100       
#define ICMB_NOTSTART     0x0200       
#define ICMB_RESTART      0x0400       
#define ICMB_ARBWINS      0x0800       
#define ICMB_ARBLOSSES    0x1000       

#endif

extern void LAN_MAC_Address_Get(unsigned char, unsigned long int *, unsigned long int *);
extern void LAN_IP_Address_Get(unsigned char, unsigned long int *);
extern void LAN_HeaderInfo_Get(unsigned char, void **p_Packet, void **p_IP, void *ptr);
extern void LAN_PacketLength_Get( void *, unsigned short int *);


extern unsigned char LAN_IncomingPacketAppend(unsigned char *, unsigned short int ,void *,void *);
extern unsigned char LAN_OutgoingPacketGet(void *, unsigned char *);


extern void GetBMCCtrAdd(void *p_CtrAdd);
extern void LoadBMCtrlInfoDefault2PS(void);

#ifdef IPMI_ICMB_BRIDGE
extern void GetICMBBdgDvsAdd(void *p_CtrAdd);
extern void ICMBBridgeStatCount(unsigned short int w_SetBitIndex);
#endif      

#elif OS_LINUX      

#include <sys/types.h>
#include <sys/shm.h>
#include "autoconfig.h"

#define IPMI_VERSION 									0x51

#define OFF                                             0
#define ON                                              1

#ifdef OPEN_BLADE
#ifndef CHASSIS_MANAGER
#define DEFAULT_BMC_ADDRESS                             0x30
#define CMM_ADDRESS                                     0x20
#else 
#define DEFAULT_BMC_ADDRESS                             0x20
#define CMM_ADDRESS                                     0x20
#endif                
#else 
#define DEFAULT_BMC_ADDRESS                             0x20
#endif                
#define DEFAULT_ICMB_BRIDGE_DEVICE_ADDRESS              0x3412

#ifdef DEBUG
#define ipmi_dmsg(format,arg...)  debug_printf (format,__FILE__,__FUNCTION__,__LINE__,## arg)
#endif 

typedef struct
{
    BYTE b_BMCAddStatus;
    BYTE b_BMCAdd;
    BYTE b_DEVICEGUID[16];
    BYTE b_BMCRestorePolicy;
    BYTE b_IsPowerOn;
    BYTE b_PowerCycleInterval; 
    WORD w_SelReservedID;
#ifdef BMC_DEBUG_PORT    
    BYTE b_DebugPort;
#endif    
}St_BMCControllerTag;

typedef struct
{
    WORD 	w_BoardID; 
	BYTE	b_WFruCap;
	BYTE	b_HWMonitorChip;
	BYTE	b_NumLan;
#ifdef CONFIG_PLATFORM_SUPERMICRO_X7SB3 
    DWORD dw_DHCPTimeout;
    BYTE b_StaticIPAddr[4];
#endif	    
}St_BoardInfoTag;

typedef struct
{
	BYTE	b_DataConfig[10];
}St_SysConfingInfoTag;


#define SHMType 0
#define SEMType 1
#define MSGType 2

#define FILL_SHMINFO(key,id,slot) \
    if(id!=-1){\
	at_SHMKey[slot]=key;at_w_SHMID[slot]=id;\
    }
#define FILL_SEMINFO(key,id,slot) \
    if (id != -1) {           \
	at_SEMKey[slot]=key;at_w_SEMID[slot]=id; \
    }
    

#define FILL_MSGINFO(key,id,slot) \
    if (id != -1) {           \
	at_MSGKey[slot]=key;at_w_MSGID[slot]=id; \
    }
#ifdef CHASSIS_MANAGER
#define   MAX_COMPUTE_BLADE_SLOT    64
#endif 

#ifdef IPMI_ICMB_BRIDGE

typedef enum
{
    ICMB_RUNTCMD      = 0x0001,      
    ICMB_BADCOMMAND   = 0x0002,      
    ICMB_SPURIOUSRESP = 0x0004,      
    ICMB_SENT         = 0x0008,      
    ICMB_RECEIVED     = 0x0010,      
    ICMB_OTHERADDR    = 0x0020,      
    ICMB_RUNTPKT      = 0x0040,      
    ICMB_BADCHECKSUM  = 0x0080,      
    ICMB_UNKTYPE      = 0x0100,      
    ICMB_NOTSTART     = 0x0200,      
    ICMB_RESTART      = 0x0400,      
    ICMB_ARBWINS      = 0x0800,      
    ICMB_ARBLOSSES    = 0x1000       
} En_ICMBStatisticsIdxTag;   

#endif

extern int Register_IPC_Key(key_t, int);
extern void GetBMCCtrAdd(void *p_CtrAdd);

extern void debug_printf(const char *format, ...);
extern int Linux_TX_timer_create (void (*func) (void), int first_time, int cycle_time);
extern void pause_func(int sec, int usec);
extern int shm_get_shmid(key_t);
extern STATUS GlobalParaInit(void);
extern void GlobalParaFin(void);
#endif      

#endif      




