



#ifndef PS_H_
#define PS_H_

#include <PltConfig.h>
#include <user.h>
#include <chnlcmd.h>
#include <lancmd.h>
#include <serialcmd.h>
#include <pefcmd.h>
#include <UtilPefCmd.h>
#include <UtilLancmd.h>
#include <Common.h>
#include <gdef.h>
#include <Payload.h>
#include <OEMCmd.h>
#ifdef IPMI_RMCP_PLUS_SUB_SOL
#include <SOLCmd.h>
#endif
#ifdef IPMI_ICMB_BRIDGE
#include <ICMB_BridgeCMD.h>
#endif
#if defined(ENABLE_OWN_AST_LDAP) || defined(ENABLE_ACTIVE_DIRECTORY)
#include <ldap_client.h>
#endif
#ifdef ENABLE_AUTH_RADIUS
#include <radius_client.h>
#endif

#ifdef OS_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#endif
#define MAX_PS_MODULE_SUPPORTED     20
#define PS_DEFAULT_START_OFFSET     MAX_PS_MODULE_SUPPORTED

typedef void (* PSInitFunction)(void);
typedef void (* ModuleVolatileInitFunction)(void);


void ChannelInfoVolatileInit(void);
void LanInfoVolatileInit(void);
void PefInfoVolatileInit(void);
void UserInfoVolatitleInit(void);



#ifdef IPMI_OPMA_FEATURE
typedef struct St_OPMAStruct_t {
	unsigned char OEMID[2];
	unsigned char ImplementaionID[2];
	unsigned char OPMASPEC[2];
}St_OPMAStruct;

typedef struct St_OPMAInfoTag_t {
	St_OPMAStruct							at_CurrentOPMAInfo;
  St_OPMAStruct							at_OPMAInfo[7];
  unsigned char 					at_OPMALocalLock;
}St_OPMAInfoTag;
#endif




#define PRESERV_FILE "/nv/PS.xml"


typedef struct PSDataStruct
{
	St_UserInfoTag 				a_St_UserInfo[MAX_USER_NUM];
	St_ChnlAccessTag 			a_St_ChnlAccessInfo[16];
	St_LanInfoTag 				a_St_LanInfo[MAX_LAN_NUM];
	St_SerChnlInfoTypeTag 		a_St_SerChnlInfo[MAX_SER_CHNL];
	St_PEFInfoTag 				St_PEFInfo;
#ifdef IPMI_RMCP_PLUS_SUB_SOL
	St_SOLNonVolatileTag 		St_SOLNonValInfo;
#endif
#ifdef IPMI_ICMB_BRIDGE
	St_ICMBBridgeInfoTag 		St_ICMBBridgeInfo;
#endif
	St_BMCControllerTag 		St_BMCCtrInfo;
	St_LanExtInfoTag 			a_St_LanExtInfo[MAX_LAN_NUM];   
	St_UserPayloadAccessTag 	a_St_UserPayloadAccess[MAX_USER_NUM]; 
#ifdef OEM_AST2000_LOGINFO
	St_LogInfoTag 				a_St_LogInfo[MAX_LOGRECORD_NUM];
#endif
#ifdef SMTP
	St_LanServicesInfoTag 		a_St_LanServicesInfo;
#endif
#ifdef ENABLE_OWN_AST_LDAP
	St_LdapInfoTag				a_St_LdapInfo;
#endif
	St_BoardInfoTag				St_BoardInfo;
	St_LanServicesConfigTag		St_LanServicesConfig;
#ifdef IPMI_OPMA_FEATURE 	
	St_OPMAInfoTag 	St_OPMAInfoConfig ;
#endif
#ifdef ENABLE_AUTH_RADIUS
	St_RadiusInfoTag			a_St_RadiusInfo;
#endif
#ifdef ENABLE_ACTIVE_DIRECTORY
	St_ActiveDirectorsyInfoTag	a_St_ActiveDirectoryInfo;
#endif
	BYTE b_temp4test;
	St_SysConfingInfoTag		St_SysConfigInfo;
	St_UserSecretPWInfoTag		a_St_UserSecretPWInfo[MAX_USER_NUM];	

} St_PrivateStorageTag;


typedef enum En_PSModule
{
     PS_USER_MODULE = 0,
     PS_CHANNEL_MODULE,
     PS_LAN_MODULE,
     PS_SERIAL_MODULE,
     PS_PEF_MODULE,
#ifdef IPMI_RMCP_PLUS_SUB_SOL
     PS_SOL_MODULE,
#endif
#ifdef IPMI_ICMB_BRIDGE
     PS_ICMB_BRIDGE_MODULE,
#endif
     PS_BMCCTRL_MODULE

    ,PS_EXT_LAN_MODULE
    
    ,PS_USER_PAYLOAD_MODULE
    
#ifdef OEM_AST2000_LOGINFO
    ,PS_LOG_MODULE
#endif	
#ifdef SMTP 
    ,PS_LANSERVICES_MODULE
#endif 
#ifdef ENABLE_OWN_AST_LDAP
    ,PS_LDAP_MODULE
#endif 
    ,PS_BOARD_MODLE
    ,PS_LANSERVICES_CONFIG_MODLE
#ifdef IPMI_OPMA_FEATURE    
    ,PS_OPMA_MODLE
#endif    
#ifdef ENABLE_AUTH_RADIUS
    ,PS_RADIUS_MODULE
#endif
#ifdef ENABLE_ACTIVE_DIRECTORY
	,PS_ACTIVE_DIRECTORY_MODULE
#endif
	,PS_USER_EXINFO_MODULE
    ,AllModule
} En_PSModuleTag;



#ifdef _PS_C_


#ifdef OS_THREAD_X
St_PrivateStorageTag at_St_PS;
OEMPSInfoType at_St_OEMPS;
#else 
St_PrivateStorageTag *at_p_St_PS = 0;
#define at_St_PS (*at_p_St_PS)
OEMPSInfoType *at_p_St_OEMPS = 0;
#define at_St_OEMPS (*at_p_St_OEMPS)
#endif

#define SIZEOFSTREMBER(MEMBERSTRUCT) (sizeof(MEMBERSTRUCT))
#define PTROFMEMBER(MEMBER) ((int) &((St_PrivateStorageTag *)0)->MEMBER)


typedef struct PSEntryTag_t{
	int PSEntryIndex;
	int PSEntrySize;
	int PSEntryOffset;
	ModuleVolatileInitFunction VolatileInitFunc;
}PSEntryTag;

PSEntryTag PSEntryInfo[] =
{	
	{PS_USER_MODULE,				SIZEOFSTREMBER(St_UserInfoTag)* MAX_USER_NUM,			PTROFMEMBER(a_St_UserInfo), 				NULL},
	{PS_CHANNEL_MODULE,				SIZEOFSTREMBER(St_ChnlAccessTag)* 16,					PTROFMEMBER(a_St_ChnlAccessInfo), 			ChannelInfoVolatileInit},
	{PS_LAN_MODULE,					SIZEOFSTREMBER(St_LanInfoTag)* MAX_LAN_NUM,				PTROFMEMBER(a_St_LanInfo), 					LanInfoVolatileInit},
	{PS_SERIAL_MODULE,				SIZEOFSTREMBER(St_SerChnlInfoTypeTag)* MAX_SER_CHNL,	PTROFMEMBER(a_St_SerChnlInfo), 				NULL},
	{PS_PEF_MODULE,					SIZEOFSTREMBER(St_PEFInfoTag),							PTROFMEMBER(St_PEFInfo), 					PefInfoVolatileInit},	                          			
#ifdef IPMI_RMCP_PLUS_SUB_SOL	                                                          	                                          															
	{PS_SOL_MODULE,					SIZEOFSTREMBER(St_SOLNonVolatileTag),					PTROFMEMBER(St_SOLNonValInfo), 				NULL},
#endif	                    			                                                      	                                          															
                            			                                                      	                                          															
#ifdef IPMI_ICMB_BRIDGE     			                                                      	                                          															
	{PS_ICMB_BRIDGE_MODULE,			SIZEOFSTREMBER(St_ICMBBridgeInfoTag),					PTROFMEMBER(St_ICMBBridgeInfo), 			NULL},
#endif	                    			                                                      	                                          															
                            			                                                      	                                          															
	{PS_BMCCTRL_MODULE,				SIZEOFSTREMBER(St_BMCControllerTag),					PTROFMEMBER(St_BMCCtrInfo), 				NULL},
	{PS_EXT_LAN_MODULE,				SIZEOFSTREMBER(St_LanExtInfoTag)* MAX_LAN_NUM,			PTROFMEMBER(a_St_LanExtInfo), 				NULL},
	{PS_USER_PAYLOAD_MODULE,		SIZEOFSTREMBER(St_UserPayloadAccessTag)* MAX_USER_NUM,	PTROFMEMBER(a_St_UserPayloadAccess), 		NULL},
	                          			                                                      	                                          															
#ifdef OEM_AST2000_LOGINFO				                                                      	                                          															
	{PS_LOG_MODULE,					SIZEOFSTREMBER(St_LogInfoTag)* MAX_LOGRECORD_NUM,		PTROFMEMBER(a_St_LogInfo), 					NULL},
#endif	                                                                                	                                          															
                                                                                        	                                          															
#ifdef SMTP                                                                             	                                          															
	{PS_LANSERVICES_MODULE,			SIZEOFSTREMBER(St_LanServicesInfoTag),					PTROFMEMBER(a_St_LanServicesInfo), 			NULL},
#endif     
#ifdef ENABLE_OWN_AST_LDAP	                                                                        	                                          															                                                                             		                                                          	                                          															
	{PS_LDAP_MODULE,				SIZEOFSTREMBER(St_LdapInfoTag),							PTROFMEMBER(a_St_LdapInfo), 				NULL},                                                                                	                                          															                                                                                     	                                          															
#endif
	{PS_BOARD_MODLE,				SIZEOFSTREMBER(St_BoardInfoTag),						PTROFMEMBER(St_BoardInfo), 					NULL},
	{PS_LANSERVICES_CONFIG_MODLE,	SIZEOFSTREMBER(St_LanServicesConfigTag),				PTROFMEMBER(St_LanServicesConfig), 			NULL},
#ifdef IPMI_OPMA_FEATURE 	
	{PS_OPMA_MODLE,					SIZEOFSTREMBER(St_OPMAInfoTag),							PTROFMEMBER(St_OPMAInfoConfig),		 		NULL},
#endif	
#ifdef ENABLE_AUTH_RADIUS
	{PS_RADIUS_MODULE,				SIZEOFSTREMBER(St_RadiusInfoTag),						PTROFMEMBER(a_St_RadiusInfo),		  		NULL},
#endif
#ifdef ENABLE_ACTIVE_DIRECTORY
	{PS_ACTIVE_DIRECTORY_MODULE,	SIZEOFSTREMBER(St_ActiveDirectorsyInfoTag),				PTROFMEMBER(a_St_ActiveDirectoryInfo), 		NULL},
#endif
	{PS_USER_EXINFO_MODULE,		SIZEOFSTREMBER(St_UserSecretPWInfoTag) *MAX_USER_NUM,	PTROFMEMBER(a_St_UserSecretPWInfo), 		UserInfoVolatitleInit},
};


BYTE b_PSTable_SIZE = sizeof(PSEntryInfo)/ sizeof(PSEntryTag);

void PSCommit(WORD);
#else

extern void PSCommit(WORD);
extern unsigned char OEMPSCommit(
                           unsigned int Offset,
                           unsigned int Len,
                           unsigned int Size,
                           void *p_DataBuf
                         );

#ifdef OS_THREAD_X
extern St_PrivateStorageTag at_St_PS;
extern OEMPSInfoType at_St_OEMPS;
#elif OS_LINUX
extern St_PrivateStorageTag *at_p_St_PS;
#define at_St_PS (*at_p_St_PS)
extern OEMPSInfoType *at_p_St_OEMPS;
#define at_St_OEMPS (*at_p_St_OEMPS)
#endif

#endif  
extern BYTE b_PSTable_SIZE;

#endif  





