
#ifndef _LANCMD_H_
#define _LANCMD_H_


#include <sys/types.h>
#include <unistd.h>

#include <PltConfig.h>
#include <gdef.h>
#include "ipv6_config.h"
#define PARAM_SET_IN_PROGRESS                               0
#define PARAM_AUTH_TYPE_SUPPORT                             1
#define PARAM_AUTH_TYPE_ENABLE                              2
#define PARAM_IP_ADDRESS                                    3
#define PARAM_IP_ADDRESS_SRC                                4
#define PARAM_MAC_ADDRESS                                   5
#define PARAM_SUBNET_MASK                                   6
#define PARAM_IPV4_HEADER_PARAM                             7
#define PARAM_PRI_RMCP_PORT                                 8
#define PARAM_SEC_RMCP_PORT                                 9
#define PARAM_GEN_ARP_CONTRL                                10
#define PARAM_GEN_ARP_INTERVAL                              11
#define PARAM_DEFAULT_GATEWAY_ADDRESS                       12
#define PARAM_DEFAULT_GATEWAY_MAC_ADDRESS                   13
#define PARAM_BACKUP_GATEWAY_ADDRESS                        14
#define PARAM_BACKUP_GATEWAY_MAC_ADDRESS                    15
#define PARAM_COMMUNITY_STRING                              16
#define PARAM_NUMBER_OF_DEST                                17
#define PARAM_DEST_TYPE                                     18
#define PARAM_DEST_ADDRESS                                  19
#define PARAM_VLAN_ID                                       20
#define PARAM_VLAN_PRIORITY                                 21
#define PARAM_DEST_ADDR_VLAN_TAGS                           25

#ifdef IPMI_RMCP_PLUS
#define PARAM_RMCP_PLUS_CIPHER_SUITE_ENTRY_SUPPORT          22
#define PARAM_RMCP_PLUS_CIPHER_SUITE_ENTRIES                23
#define PARAM_RMCP_PLUS_CIPHER_SUITE_PRIVILEGE_LEVELS       24
//#ifdef IPMI_RMCP_PLUS_SUB_AES
//#define SUPPORT_CIPHER_SUITE_COUNT                           9
//#else
//#define SUPPORT_CIPHER_SUITE_COUNT                           6
//#endif

#endif

#define VLAN_ENABLE                                         0x8000

#define ENABLE_BMC_GEN_ARP									(1 << 1)
#define ENABLE_BMC_GEN_GRAT_ARP								(1 << 0)




#define UNSPECIFIED_IP_ADDRESS              0
#define STATIC_IP_ADDRESS                   1
#define DHCP_OBTAIN_IP_ADDRESS              2
#define BOIS_LOAD_IP_ADDRESS                3
#define OTHERS_PROTOCOL_LOAD_IP_ADDRESS     4

#define DEDICATE_MODE 0
#define SHARE_MODE 1
#define FAILOVER_MODE 2

#define HOSTNAME_FILE   "/etc/hostname_for_dhcp"
#define HOSTNAME_LEN    64
#define DEFHTTPPORT 80
#define DEFHTTPSPORT 443
#define DEFIKVMPORT 5900
#define DEFVMPORT 623


struct entry {
    char *title;
    unsigned long int value;
};

struct tabtab {
    char *title;
    struct entry *tab; 
};

typedef struct
{
	unsigned long int IPPacketsRecv;
	unsigned long int RecvIPHeaderErr;
	unsigned long int RecvIPAddrErr;
	unsigned long int FragmentedIPPacketsRecv;
	unsigned long int IPPacketsTrans;
	unsigned long int UDPPacketsRecv;
} St_SNMPTag;

#define SNMP_KEY	2046

typedef struct
{
    BYTE DestType;
    BYTE AckRetryInterval;
    BYTE Retries;
    BYTE IPFormat;
    BYTE GateWaySel;
    BYTE AlertIP[4];
    BYTE AlertMAC[6];
} St_LanDestInfoTag;

typedef struct
{
    BYTE SetInProgress;
    BYTE TempGateWayAddr[4];
	BYTE TempGateWayMAC[6];

} St_LanVolatileInfoTag;      

typedef struct
{
	BYTE AuthTypeEnable[5];
	BYTE IPAddr[4];
	BYTE IPAS;
	BYTE MACAddr[6];
	BYTE SubNetMask[4];
	BYTE IPv4Header[3];
	BYTE PriRMCPPort[2];
	BYTE SecRMCPPort[2];
	BYTE WDT_ARP_CTL;
	BYTE ARP_CTL;
	BYTE ARP_INTVAL;
	BYTE DefaultGateWayAddr[4];
	BYTE DefaultGateWayMAC[6];
	BYTE BAKDefaultGateWayAddr[4];
	BYTE BAKDefaultGateWayMAC[6];
	BYTE CommunityString[18];
	St_LanDestInfoTag   St_LanDestInfo[MAX_LAN_ALERT_DESTINATION_SUPPORT];
#ifdef IPMI_RMCP_PLUS
	BYTE  CS_PrivLevels[9];
	BYTE  Channel_SecurityKey[20];
	BYTE  KG_status;	
	BYTE  RemoteSession; 
#endif
} St_LanInfoTag;

#include "pefcmd.h"
#ifdef SMTP
#define MAXSMTPSERVLEN 128
#define MAXSNDMAILADDRLEN 64  
#define MAXMSG_LENGTH	64
#define MAXMAILRECEIVER MAX_PEF_TABLE_ENTRY

typedef struct 
{
	BYTE Receiver_En;
	BYTE SendMailAddr[MAXSNDMAILADDRLEN];	
	BYTE Message[MAXMSG_LENGTH];
	BYTE Subject[MAXMSG_LENGTH];
	BYTE Sender[MAXSNDMAILADDRLEN];
}St_MailInfoTag;

typedef struct 
{
#define MAXACCOUNTLEN 64
#define MAXPASSWORDLEN 64
	BYTE Auth_En;
	BYTE DNSAddr[4];
	BYTE SMTP_SERVER[MAXSMTPSERVLEN];
	St_MailInfoTag Mail_Receiver[MAXMAILRECEIVER];
	BYTE Account[MAXACCOUNTLEN];
	BYTE Password[MAXPASSWORDLEN];
	BYTE SMTP_SSL;
	BYTE Alert_En;
}St_LanServicesInfoTag;
#endif 

typedef struct
{
    BYTE Reserved[18];
    BYTE b_DevIfMode; 
    BYTE b_SOLTimeOutEn;
    WORD w_VLANID;
    BYTE b_VLANPRIORITY;
	BYTE b_DHCPv6Mode;	
#ifdef IPMI_RMCP_PLUS
    BYTE RandomNumber_Key[20];
#endif
} St_LanExtInfoTag;

typedef struct
{
	WORD Http_PortNum;
	WORD Https_PortNum;
	WORD Telnet_PortNum;
	WORD SSH_PortNum;
	WORD SNMP_PortNum;
	WORD IKVM_PortNum;
	WORD VUSB_PortNum;
	WORD SMTP_PortNum;
	WORD Reserved_PortNum;
} St_LanServicesConfigTag;

#ifdef _LANCMD_C_
BYTE DetectBMCARPRespEnable(BYTE b_DeviceID);

#else
extern BYTE DetectBMCARPRespEnable(BYTE b_DeviceID);
extern St_LanDestInfoTag ws_a_St_LanDestVolatileInfo[MAX_LAN_NUM];
extern St_LanVolatileInfoTag ws_St_LanVolatileInfo;


extern CMD_STATUS SetLanConfigCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetLanConfigCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SpdBMCARPsCMD(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS GetLanStatisticsCmd(MsgBufFmt *pMsgBuf,MsgBufFmt *pRespMsgBuf);
extern CMD_STATUS SuspendBMCSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern CMD_STATUS SetLanConfSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE ApiSetLanConfigSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE ApiSpdBMCARPsSubFnCheck(MsgBufFmt *p_St_MsgBufIn);

extern void LoadLanInfoDefault2PS(void);
extern void LoadLanExtInfoDefault2PS(void);
extern void LoadLANServicesInfoDefault2PS(void);

#endif  


#endif  





