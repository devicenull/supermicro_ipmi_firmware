      
#ifndef _USER_H_
#define _USER_H_
#include"UtilCrypto.h"


#define MAX_USER_NAME_LEN 16
#define MAX_USER_PW_LEN 20
#define USER_DISABLE	0
#define USER_ENABLE		1

#pragma pack(push,1) 

typedef struct
{
	BYTE IKVMVideoEnable;	
	BYTE IKVMKMEnable;		
	BYTE IKVMKickEnable;	
} IKVMInfoTag;

typedef struct
{
	BYTE VUSBEnable;		
} VUSBInfoTag;

typedef struct __attribute__ ((packed))
{
	BYTE b_enable   :1;
	BYTE PasswdSize	:1;
	BYTE Reserve	:6;
} St_UserExtInfoTag;

typedef struct
{
	St_UserExtInfoTag userinfo;
	BYTE a_b_Name[MAX_USER_NAME_LEN];
	BYTE a_b_Passwd[MAX_USER_PW_LEN];	
	BYTE a_b_ChannelAccess[16];
	IKVMInfoTag IKVMInfo;
	VUSBInfoTag VUSBInfo;
	BYTE PrivilegeChange;
} St_UserInfoTag;

typedef struct
{
	BYTE b_CipherLen;
	BYTE isEncryptedName;
	BYTE a_b_CipherPasswd[CRYPTO_PW_MAX_LEN];
} St_UserSecretPWInfoTag;

typedef struct
{
	BYTE a_b_PlainUserName[MAX_USER_NAME_LEN];
	BYTE a_b_PlainPasswd[MAX_USER_PW_LEN];
} St_UserExPlainInfoTag;

#pragma pack(pop) 

#ifdef OEM_AST2000_LOGINFO
#pragma pack(push,1) 
	typedef struct
	{
		BYTE b_enable;
		BYTE b_InOut;
		DWORD dw_Time;
		BYTE a_b_Name[16];
		BYTE a_b_LogIPAddr[4];
		BYTE a_b_LogMACAddr[6];
	} St_LogInfoTag;
#pragma pack(pop) 
#define LOG_DISABLE	0
#define LOG_ENABLE		1	
#define LOG_In	0
#define LOG_Out		1	

#endif



#define USER_NOT_FOUND	0xFF

#ifndef _USER_C_

extern CMD_STATUS SetUserAccessCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetUserAccessCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetUserNameCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetUserNameCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetUserPwdCmd(MsgBufFmt *, MsgBufFmt *);

extern BYTE UtilGetUserID(BYTE *p_b_UserName);
extern void UtilLoadUserInfoDefault2PS(void);
#ifdef OEM_AST2000_LOGINFO
extern void UtilLoadLogInfoDefault2PS(void);
#endif

#else

#define CALL_BACK_RES	0x40
#define LINK_AUTH		0x20
#define IPMI_MSG		0x10
#define CHNL_PRI		0x0F 

#endif    

#endif  





