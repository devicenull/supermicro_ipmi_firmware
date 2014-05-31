
#ifndef _SESSCMD_H_
#define _SESSCMD_H_

#define SESS_NOT_FOUND			0xFFFFFFFF

#define AUTH_NONE       	0
#define AUTH_MD2        	1
#define AUTH_MD5        	2
#define AUTH_RES        	3
#define AUTH_STRAIGHT   	4
#define AUTH_OEM        	5

#define PRI_RES         	0
#define PRI_CALLBACK    	1
#define PRI_USER        	2
#define PRI_OPERATOR    	3
#define PRI_ADMIN       	4
#define PRI_OEM         	5

#define NULL_SESSION_TYPE   0
#define RMCP_TYPE           1
#define RMCP_PLUS_TYPE      2
#define SERIAL_TYPE         3
#define TEMP_SESS_ID        0xFF000000

#define PADDING_VERIFY		0
#define PADDING_APPEND      1

#define SESS_CACHE_NUM      5

typedef enum
{
    SESS_IDLE,
    SESS_ALLOCATED,
    SESS_ACTIVED,
    SESS_CLOSING
} En_SessStatusTypeTag;

typedef struct
{
    BYTE b_handle;
    BYTE b_AuthType;
    DWORD dw_TmpSessID;
    BYTE a_b_ChalString[16];
} St_RMCPTypeTag;

typedef struct
{
    BYTE b_Mgmtag;
    BYTE b_CSID;
    BYTE b_EncryON;
    BYTE b_PayloadType;
    DWORD dw_RC_SessID;
    BYTE b_Role;
    BYTE b_Reserve;
    BYTE a_b_RCRanNum[16];
    BYTE a_b_MSRanNum[16];
    BYTE a_b_SIK[20];
    BYTE a_b_IntegSIK[20];
    BYTE a_b_EncrySIK[16];
} St_RMCPPLUSTypeTag;

typedef struct
{
    
    BYTE b_SessChnlNo;
    BYTE b_SessMaxPri;
    BYTE b_SessPri;
    BYTE b_SessUserID;
    BYTE b_RMCPType;
    BYTE b_reserve1;
    BYTE b_reserve2;
    BYTE b_reserve3;
  	BYTE a_b_SourceIP[4];
    BYTE a_b_SourcePort[2];
    DWORD dw_InSeqNo;
    DWORD dw_OutSeqNo;    
    DWORD dw_SessTimeStamp;
    DWORD dw_SessID;
    DWORD a_dw_SessCache[SESS_CACHE_NUM][2];
	union
	{
		St_RMCPTypeTag 		St_RMCPPacket;
		St_RMCPPLUSTypeTag 	St_RMCPPlusPacket;
    } Un_RMCPTag;
    En_SessStatusTypeTag En_status;
} St_SessTblTypeTag;


#ifndef _SESSCMD_C_


extern BYTE			ApiHuntValidAndFirstSessIdx(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut,WORD *w_first_session);
extern BYTE			ApiUsrChAccessPrivilegeLvl(BYTE b_SourceChl, BYTE b_UseChlAccess);
extern BYTE			ApiSearchSessTblIdx(void);
extern BYTE 		ApiCloseSessionSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern void			ApiStoreSessInSessTbl(MsgBufFmt *p_St_MsgBufIn, BYTE b_Idx, BYTE b_UserID);
extern void			ApiGenerateChalString(BYTE b_Idx);

extern STATUS SessInit(void);
extern CMD_STATUS GetSessChalCMD(MsgBufFmt *,MsgBufFmt *);
extern CMD_STATUS ActivateSessCMD(MsgBufFmt *,MsgBufFmt *);
extern CMD_STATUS SetSessionPriLevelCMD(MsgBufFmt *,MsgBufFmt *);
extern CMD_STATUS GetSessionInfoCMD(MsgBufFmt *,MsgBufFmt *);
extern CMD_STATUS CloseSessionCMD(MsgBufFmt *,MsgBufFmt *);
extern DWORD TModeActivateSessCMD(BYTE b_Chnlno, BYTE *p_b_Name, BYTE *p_b_Passwd, BYTE *p_b_CompCode);  
                                                                                             
#else 
                                                                                       

BYTE				ApiHuntValidAndFirstSessIdx(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut, WORD *w_first_session);
BYTE				ApiUsrChAccessPrivilegeLvl(BYTE b_SourceChl, BYTE b_UseChlAccess);
BYTE				ApiSearchSessTblIdx(void);
BYTE				ApiCloseSessionSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
void				ApiStoreSessInSessTbl(MsgBufFmt *p_St_MsgBufIn, BYTE b_Idx, BYTE b_UserID);
void				ApiGenerateChalString(BYTE b_Idx);

STATUS				SessInit(void );
CMD_STATUS			GetSessChalCMD(MsgBufFmt *,MsgBufFmt *);
CMD_STATUS			ActivateSessCMD(MsgBufFmt *,MsgBufFmt *);
CMD_STATUS			SetSessionPriLevelCMD(MsgBufFmt *,MsgBufFmt *);
CMD_STATUS			GetSessionInfoCMD(MsgBufFmt *,MsgBufFmt *);
CMD_STATUS			CloseSessionCMD(MsgBufFmt *,MsgBufFmt *);
DWORD				TModeActivateSessCMD(BYTE b_Chnlno, BYTE *p_b_Name, BYTE *p_b_Passwd, BYTE *p_b_CompCode); 
                                                                          
#endif	

#endif  





