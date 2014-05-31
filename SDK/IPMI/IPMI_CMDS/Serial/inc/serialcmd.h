
#ifndef _SERIALCMD_H_
#define _SERIALCMD_H_

#define SER_SET_IN_PROGRESS         0
#define AUTH_TYPE_SUPPORT			1
#define AUTH_TYPE_ENABLE			2
#define CONNECT_MODE				3
#define SESS_INACTIVE_TIMEOUT		4
#define SESS_TERMINATION			6
#define SER_COMM_MODE_SETTING		7
#define SER_TERM_MODE_CONFIG		29


#define GET_PRESENT_MUX_STATUS                      0
#define REQUEST_SWITCH_OF_MUX_TO_SYSTEM             1
#define REQUEST_SWITCH_OF_MUX_TO_BMC                2
#define FORCE_SWITCH_OF_MUX_TO_SYSTEM               3
#define FORCE_SWITCH_OF_MUX_TO_BMC                  4
#define BLOCK_REQUESTS_TO_SWITCH_MUX_TO_SYSTEM      5
#define ALLOW_REQUESTS_TO_SWITCH_MUX_TO_SYSTEM      6
#define BLOCK_REQUESTS_TO_SWITCH_MUX_TO_BMC         7
#define ALLOW_REQUESTS_TO_SWITCH_MUX_TO_BMC         8

#define FORCE_SWITCH_OF_MUX_TO_OFF                  10
#define FORCE_SWITCH_OF_MUX_TO_SOL                  11


typedef struct
{
	BYTE b_CallBackLevel;
	BYTE b_UserLevel;
	BYTE b_OperatorLevel;
	BYTE b_AdministratroLevel;
	BYTE b_OEMLevel;
} St_SerAuthTypeTag;

typedef struct 
{
	BYTE b_TermModeOption;
	BYTE b_NewLineSeq;
} St_SerTermModeConfigTypeTag;

typedef struct 
{
	St_SerAuthTypeTag St_AuthTypeEnable;
	BYTE b_ConnectMode;
	BYTE b_SessionTimeOut;
	BYTE b_DCDTermination;
	BYTE b_FlowCtrl;
	BYTE b_CommSetting;
	BYTE b_SessionTermination;
	BYTE b_SerialMode;
	BYTE b_SOLFlowCtrl;
	St_SerTermModeConfigTypeTag St_s_SerTermModeConfig;
} St_SerChnlInfoTypeTag;

#ifdef _SERIALCMD_C_
CMD_STATUS SetSerConfigCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSerConfigCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS SetSerMuxCmd(MsgBufFmt *, MsgBufFmt *);
void       ApiLoadSerInfoDefault2PS(void);
BYTE       ApiSetSerMuxSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
BYTE       ApiSetSerConfigSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
#else
extern CMD_STATUS SetSerConfigCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSerConfigCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetSerMuxCmd(MsgBufFmt *, MsgBufFmt *);
extern void       ApiLoadSerInfoDefault2PS(void);
extern BYTE       ApiSetSerMuxSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
extern BYTE       ApiSetSerConfigSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
#endif   

#endif  





