
#ifndef _SOLCMD_H_
#define _SOLCMD_H_


#ifdef IPMI_RMCP_PLUS_SUB_SOL

#define SOL_INITIAL_SESSION_ID 0xFF
#define SOL_INITIAL_IP 0x00

#define SET_IN_PROCESS                                              0
#define SOL_ENABLE                                                  1
#define SOL_AUTHENTICATION                                          2
#define SOL_CHARACTER_ACCUMULATE_THRESHOLD                          3
#define SOL_RETRY                                                   4
#define SOL_NON_VOLATILE_BIT_RATE                                   5
#define SOL_VOLATILE_BIT_RATE                                       6
#define SOL_PAYLOAD_CHANNEL                                         7
#define SOL_PAYLOAD_PORT_NUMBER                                     8

#define SOL_SESSION_STATE_ACTIVATING                                0
#define PAYLOAD_INSTANCE_BASED                                      1
#define SOL_MAJOR_FORMAT_VERSION                                    1
#define SOL_MINOR_FORMAT_VERSION                                    0
#define SOL_SET_COMPLETE                                            0
#define SOL_SET_IN_PROGRESS                                         1
#define SOL_GET_PARAMETER                                           0
#define SOL_GET_PARAMETER_REVISION_ONLY                             1

#define SOL_BIT_RATE_9600                                           6
#define SOL_BIT_RATE_19200                                          7
#define SOL_BIT_RATE_38400                                          8
#define SOL_BIT_RATE_57600                                          9
#define SOL_BIT_RATE_115200                                        10



typedef struct
{
    BYTE b_SOLActivating;
    BYTE b_SOLEnOrDisable;
    BYTE b_SOLAuthentication;
    BYTE b_SOLPrivilegeLevel;
    BYTE b_ChrAccumulate;
    BYTE b_ChrSendThreshold;
    BYTE b_RetryCount;
    BYTE b_RetryInterval;
    BYTE b_NonValBitRate;
} St_SOLNonVolatileTag;


typedef struct
{
    BYTE b_SOLEnOrDisable;
    BYTE b_SettingCondition;
    BYTE b_ValBitRate;
    BYTE b_SsnIdx;
    DWORD dw_SsnID;
    DWORD dw_IP;
    DWORD dw_PreviousTime;
    DWORD dw_CurrentTime;
} St_SOLVolatileTag;


#ifdef _SOLCMD_C_
void       ApiSettingInProgress(void);
void       ApiSettingComplete(void);
#else
extern CMD_STATUS SOLActivatingCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetSOLConfigurationCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSOLConfigurationCmd(MsgBufFmt *, MsgBufFmt *);
extern void       SetSessIdle(WORD w_SessNo);
extern void       LoadSOLInfoDefault2PS(void);
extern STATUS     ApiSOLCtlSsn(BYTE b_OnorOff, MsgBufFmt *);
extern BYTE       ApiSetSOLConfigurationSubFnCheck(MsgBufFmt *p_St_MsgBufIn);
#endif   


#endif  


#endif  





