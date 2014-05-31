
#ifndef _PAYLOAD_H_
#define _PAYLOAD_H_

#define PAYLOAD_TYPE_SOL                                            1
#define PAYLOAD_TYPE_USB											0x27

#define GET_PAYLOAD_TYPE_MASK                                       0x3F
#define GET_PAYLOAD_INSTANCE_MASK                                   0x0F
#define GET_AUTH_TYPE_MASK                                          0x40
#define GET_PAYLOAD_ENCRYPT_MASK                                    0x80
#define GET_SOL_HANDSHAKE_MASK                                      0x02

#define USERPAYLOADACCESS_CHANNUM_MASK								0x0F
#define USERPAYLOADACCESS_USERID_MASK								0x3F
#define USERPAYLOADACCESS_OPERATION_MASK							0x40
#define	GET_USERPAYLOADACCESS_CHANNUM(Input)	(Input & USERPAYLOADACCESS_CHANNUM_MASK)
#define GET_USERPAYLOADACCESS_USERID(Input)		(Input & USERPAYLOADACCESS_USERID_MASK)
#define GET_USERPAYLOADACCESS_OPERATION(Input)	((Input & USERPAYLOADACCESS_OPERATION_MASK) >> 6)

#define GET_USERPAYLOADMASK_SOL										0x02

#define GET_PAYLOAD_TYPE(Input)                 (Input & GET_PAYLOAD_TYPE_MASK)
#define GET_PAYLOAD_INSTANCE(Input)             (Input & GET_PAYLOAD_INSTANCE_MASK)
#define GET_AUTH_TYPE(Input)                    ((Input & GET_AUTH_TYPE_MASK) >> 6)
#define GET_PAYLOAD_ENCRYPT(Input)     			((Input & GET_PAYLOAD_ENCRYPT_MASK) >> 7) 
#define GET_SOL_START_UP_HANDSHAKE(Input)       ((Input & GET_SOL_HANDSHAKE_MASK) >> 1)

#define PAYLOAD_ENCRYPT                                             0x80
#define PAYLOAD_AUTHENTICATED                                       0x40
#define PAYLOAD_TYPE_IPMI_MESSAGE                                   0x00
#define PAYLOAD_TYPE_RMCP_PLUS_OPEN_SESSION_RQS                     0x10
#define PAYLOAD_TYPE_RMCP_PLUS_OPEN_SESSION_RSP                     0x11
#define PAYLOAD_TYPE_RMCP_MESSAGE_1                                 0x12
#define PAYLOAD_TYPE_RMCP_MESSAGE_2                                 0x13
#define PAYLOAD_TYPE_RMCP_MESSAGE_3                                 0x14
#define PAYLOAD_TYPE_RMCP_MESSAGE_4                                 0x15
#define RMCP_PLUS_SESSION_ID_OFFSET                                 0x06
#define RMCP_PLUS_SESSION_SEQUENCE_NO_OFFSET                        0x0A
#define STANDARD_OEM_PAYLOAD_TYPE_NUMBERS							0x03
#define REGENERATE_INITIALIZATION_VECTOR							0x02

#define RMCP_PLUS_OPEN_SESSION_CMD_NO                               0x01
#define RMCP_PLUS_PAKP_MESSAGE_1_CMD_NO                             0x02
#define RMCP_PLUS_PAKP_MESSAGE_3_CMD_NO                             0x03
#define PAYLOAD_TYPE_SOL_INSTANCE									0x01
#define PAYLOAD_TYPE_LAN_INSTANCE									0x04


typedef struct 
{
    BYTE b_PrivilegeLevel;
    BYTE b_PayldType;
    BYTE b_PayldInstance;
    BYTE b_PayldAuthType;
    BYTE b_SOLHndshk;
    BYTE a_b_DestIP[4];
    BYTE a_b_DestPort[2];
    BYTE b_DestChannel;
}St_PayloadCmdTag;

typedef struct
{
	BYTE b_PayldNum;
	BYTE b_FormatVersion;

}St_PayloadTag;

typedef struct
{
	BYTE ChanNum;

	BYTE a_b_UserPayload[4];
	
} St_PayloadAccessPerLAN;

typedef struct
{
	
	St_PayloadAccessPerLAN a_St_PayloadAccessPerLAN[MAX_LAN_NUM];
	
} St_UserPayloadAccessTag;




extern void ApiSetActivatePayloadVrbData(MsgBufFmt *);
extern CMD_STATUS ActivatePayloadCMD(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS DeactivatePayloadCMD(MsgBufFmt *, MsgBufFmt *);
#ifdef IPMI_RMCP_PLUS
extern CMD_STATUS SuspendOrResumePayloadEncryptionCMD(MsgBufFmt *, MsgBufFmt *);
#endif

extern CMD_STATUS GetPayloadActivationStatusCMD(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetPayloadInstanceInfoCMD(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetChnlPayloadSupportCMD(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetChnlPayloadVersionCMD(MsgBufFmt *, MsgBufFmt *);

extern CMD_STATUS SetUserPayloadAccessCMD(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetUserPayloadAccessCMD(MsgBufFmt *, MsgBufFmt *);
extern void LoadUserPayloadDefault2PS(void);



#endif  


