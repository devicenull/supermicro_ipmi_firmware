


#ifndef _NCSI_H_
#define _NCSI_H_

#define MAX_PACKAGE_ID								0x07
#define MAX_INTERNAL_CHANNEL_ID						0x0F
#define MAX_IID										0xFF
                                     
#define MAX_CONTROL_PACKET_SIZE						0x40  
#define FCS_LENGTH                                  0x04
#define MC_ID										0x00						
#define HEADER_VER									0x01							
#define NCSI_ETHER_TYPE								0x88F8						
#define ETHER_HEADER_LEN							0x0E							
#define NCSI_HEADER_LEN								0x10

#define CLR_INIT_REQ								0x00				
#define SELECT_PACKAGE_REQ							0x01				
#define DESELECT_PACKAGE_REQ						0x02				
#define ENABLE_CHNL_REQ								0x03				
#define DISABLE_CHNL_REQ							0x04				
#define RESET_CHNL_REQ								0x05				
#define ENABLE_CHNL_TX_REQ							0x06
#define DISABLE_CHNL_TX_REQ							0x07
#define AEN_ENABLE_REQ								0x08
#define SET_LINK_REQ								0x09
#define GET_LINK_STATUS_REQ							0x0A
#define SET_VLAN_FILTER_REQ							0x0B
#define ENABLE_VLAN_REQ								0x0C
#define DISABLE_VLAN_REQ							0x0D
#define SET_MAC_ADDR_REQ							0x0E
#define ENABLE_BROADCAST_FILTER_REQ					0x10
#define DISABLE_BROADCAST_FILTER_REQ				0x11
#define ENABLE_MULTICAST_FILTER_REQ					0x12
#define DISABLE_MULTICAST_FILTER_REQ				0x13
#define SET_NCSI_FLOW_CTRL_REQ						0x14
#define GET_VER_ID_REQ								0x15
#define GET_CAPABILITIES_REQ						0x16
#define GET_PARAMETERS_REQ							0x17
#define GET_CTRL_PKT_STAT_REQ						0x18
#define GET_NCSI_STAT_REQ							0x19
#define GET_NCSI_PASS_THROUGH_STAT_REQ				0x1A
#define OEM_REQUEST									0x50


#define CLR_INIT_RESP								0x80				
#define SELECT_PACKAGE_RESP							0x81				
#define DESELECT_PACKAGE_RESP						0x82				
#define ENABLE_CHNL_RESP							0x83				
#define DISABLE_CHNL_RESP							0x84				
#define RESET_CHNL_RESP								0x85				
#define ENABLE_CHNL_TX_RESP							0x86
#define DISABLE_CHNL_TX_RESP						0x87
#define AEN_ENABLE_RESP								0x88
#define SET_LINK_RESP								0x89
#define GET_LINK_STATUS_RESP						0x8A
#define SET_VLAN_FILTER_RESP						0x8B
#define ENABLE_VLAN_RESP							0x8C
#define DISABLE_VLAN_RESP							0x8D
#define SET_MAC_ADDR_RESP							0x8E
#define ENABLE_BROADCAST_FILTER_RESP				0x90
#define DISABLE_BROADCAST_FILTER_RESP				0x91
#define ENABLE_MULTICAST_FILTER_RESP				0x92
#define DISABLE_MULTICAST_FILTER_RESP				0x93
#define SET_NCSI_FLOW_CTRL_RESP						0x94
#define GET_VER_ID_RESP								0x95
#define GET_CAPABILITIES_RESP						0x96
#define GET_PARAMETERS_RESP							0x97
#define GET_CTRL_PACKET_STATSTICS_RESP				0x98
#define GET_NCSI_STAT_RESP							0x99
#define GET_NCSI_PASS_THROUGH_STAT_RESP				0x9A
#define OEM_RESP									0xD0

#define AENNOTIFICATION                             0xFF

#define AENPKT 1
#define NONAENPKT 0

typedef enum 
{
	CMD_Completed,
	CMD_Failed,
	CMD_UnAvailable,
	CMD_UnSupported
	
}ResponseCodeTag;

typedef enum
{
	Code_NoErr,
	Code_NeedInitializtion,
	Code_UnSupportParam,
	Code_ChannelNotReady,
	Code_PackageNotReady,
	Code_InvailPayloadLen,
	Code_UnknownCommand=0x7FFF
	
}ReasonCodeTag;

typedef struct 
{
	unsigned char b_PackageID;
	unsigned char b_ChnlID;
}St_NCSIChnlTag;

typedef struct  __attribute__ ((packed))
{
	unsigned char   MCID;
	unsigned char   HDR_VER;
	unsigned char   RESERVED0;
	unsigned char   IID;
	unsigned char   CMD;
	unsigned char   CH_ID;
	unsigned short  Length;
	unsigned short  RESERVED1;
	unsigned short  RESERVED2;
	unsigned short  RESERVED3;
	unsigned short  RESERVED4;
}St_NCSICmdHDRTag;

typedef struct  
{
	St_NCSICmdHDRTag NCSI_Header;
	union  
	{
		struct
		{
			unsigned short ResCode;
			unsigned short ReasonCode;
		}ResponseCode;
		struct
		{
			unsigned long Reserved:24;
			unsigned long AENType:8;
			unsigned long LinkStatus;
			unsigned long OEMLinkStatus;	
		}AENPacket;
		
	};
}St_NCSICmdTag;

#endif  



