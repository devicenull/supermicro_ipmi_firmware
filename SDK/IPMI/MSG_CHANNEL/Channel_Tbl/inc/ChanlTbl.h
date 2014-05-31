
#ifndef _CHANLTBL_H_
#define _CHANLTBL_H_


typedef struct 
{
    unsigned char b_HardWarePhysicalChanl;
    unsigned char b_ControlCode;
    unsigned char ab_Addr[4];
    unsigned char ab_SubAddr[6];    
    
}s_ChanlInitDataTag;

typedef STATUS (* ChanlInitServiceFunc)(unsigned char, void *, void *);
typedef STATUS (* ChanlRecvServiceFunc)(unsigned char, void *, void *);
typedef STATUS (* ChanlTransServiceFunc)(unsigned char, void *, void *);
typedef STATUS (* ChanlMiscSettingServiceFunc)(unsigned char, void *, void *);

typedef struct 
{
    unsigned char b_Attendance;
    unsigned char b_ProtocolType;
    unsigned char b_MediumType;
    unsigned char b_SessionSupport;

    ChanlInitServiceFunc        ChanlInitFunc;
    ChanlRecvServiceFunc        ChanlRecvFunc;
    ChanlTransServiceFunc       ChanlTransFunc;
    ChanlMiscSettingServiceFunc ChanlMiscSetFunc;

    unsigned char b_APName[32];
    unsigned char b_ModuleName[50];

    s_ChanlInitDataTag          s_ChanlInitData;
}St_ChannelTBLTag;


#define MAXIMUM_IPMI_TOTAL_CHANNEL                      16


#define CHANL_PROTOCOL_TYPE_IPMB          0x01
#define CHANL_PROTOCOL_TYPE_ICMB          0x02
#define CHANL_PROTOCOL_TYPE_SMBUS         0x04
#define CHANL_PROTOCOL_TYPE_KCS           0x05
#define CHANL_PROTOCOL_TYPE_SMIC          0x06
#define CHANL_PROTOCOL_TYPE_BT10          0x07
#define CHANL_PROTOCOL_TYPE_BT15          0x08
#define CHANL_PROTOCOL_TYPE_TMODE         0x09
#define CHANL_PROTOCOL_TYPE_OEM1          0x1C
#define CHANL_PROTOCOL_TYPE_OEM2          0x1E
#define CHANL_PROTOCOL_TYPE_OEM3          0x1F
#define CHANL_PROTOCOL_TYPE_OEMIPMB       0x20
#define CHANL_PROTOCOL_TYPE_INVALID       0xFF

#define CHANL_MEDIUM_TYPE_IPMB            0x01
#define CHANL_MEDIUM_TYPE_ICMB10          0x02
#define CHANL_MEDIUM_TYPE_ICMB09          0x03
#define CHANL_MEDIUM_TYPE_LAN8023         0x04
#define CHANL_MEDIUM_TYPE_RS232           0x05
#define CHANL_MEDIUM_TYPE_MISC_LAN        0x06
#define CHANL_MEDIUM_TYPE_PCISMBUS        0x07
#define CHANL_MEDIUM_TYPE_SMBUS10         0x08
#define CHANL_MEDIUM_TYPE_SMBUS20         0x09
#define CHANL_MEDIUM_TYPE_USB1            0x0A
#define CHANL_MEDIUM_TYPE_USB2            0x0B
#define CHANL_MEDIUM_TYPE_SYSTEM          0x0C
#define CHANL_MEDIUM_TYPE_INVALID         0xFF

#define CHANL_PRESENT                     0x01
#define CHANL_ABSENT                      0x00

#define SINGLE_SESSION			          0x01
#define MULTI_SESSION			          0x02
#define NONE_SESSION			          0x00

#ifdef _CHANLTBL_C_


#else

#ifndef OWN_GLOBE_BMC
extern St_ChannelTBLTag at_a_St_ChannelTable[];
extern St_ChannelTBLTag at_a_St_SOLInfo[];
#else
extern St_ChannelTBLTag at_a_St_ChannelTable[];
#endif

#endif  

#endif  




