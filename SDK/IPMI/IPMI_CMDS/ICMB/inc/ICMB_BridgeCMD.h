
#ifndef _ICMB_BRIDGECMD_H_
#define _ICMB_BRIDGECMD_H_





#define ICMB_PACKET_MSG_TYPE_REQ            0x00
#define ICMB_PACKET_MSG_TYPE_RESP           0x80



#define GET_ICMB_ADDRESS_CMD				0x02
#define PREPARE_FOR_DISCOVERY_CMD          	0x10
#define SET_DISCOVERED_CMD			    	0x12
#define SEND_ICMB_EVENT_MSG_CMD             0x33

#define ICMB_TARGET                         0
#define IPMB_TARGET                         1

#define MAX_DISCOVERED_NUM                  8

typedef struct
{
	WORD	ICMB_Addr;
	BYTE	IPMB_Addr;
	BYTE	LUN;
}St_ICMBProxyTag;

typedef struct
{
	WORD	Recp_State;
	BYTE	SA;
	BYTE	LUN;
}St_ICMBEventRecpStateTag;

typedef enum
{
	Online = 0,
	Attention,
	Discovery_Start,
	Discovery_End
}En_ICMB_EVENT_CODE_Tag;

typedef struct
{
    BYTE	b_Discover;
    BYTE 	b_ListCount;
    WORD 	w_BriAddr[MAX_DISCOVERED_NUM];
}St_ICMBListTag;


typedef struct
{
    unsigned char b_ICMBBdgDvsAddStatus;
    unsigned short int w_ICMBBdgDvsAdd;
    unsigned char b_ChassisID;

} St_ICMBBridgeInfoTag;



#ifdef _ICMB_BRIDGECMD_C_


#else


extern void ICMBBridgeInfoVolatileInit(void);
extern void LoadICMBBridgeInfoDefault2PS(void);
extern CMD_STATUS GetBridgeStateCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetBridgeStateCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetICMBAddressCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetICMBAddressCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetBridgeProxyAddressCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetBridgeProxyAddressCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetICMBCapabilitiesCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetBridgeStatisticsCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ClearBridgeStatisticsCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS PrepareForDiscoveryCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetAddressCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetDiscoveredCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetChassisDeviceIDCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetChassisDeviceIDCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS BridgeRequestCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS BridgeMessageCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetBridgeEventCountCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetEventDestCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetEventRecpStateCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SentICMBEventMsgCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetEventRecpStateCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetEventDestCMD(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern void AddICMBAddress(MsgBufFmt *p_St_MsgBufIn);


#endif  

#endif  







