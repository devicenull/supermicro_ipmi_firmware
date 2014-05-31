
#ifndef _OPENBLADECMD_H_
#define _OPENBLADECMD_H_

#ifndef _I2C_DRV_H_
#ifndef I2C_USERAP_H
typedef enum
{
	I2C_OK,
	I2C_TX_OK,
	I2C_RX_OK,
	I2C_NACK,
	I2C_AL,
	I2C_BUSY
} En_I2CSTATUSTag;
#endif
#endif

#define NOT_PRESENT					0x00
#define INACTIVE					0x10
#define ACTIVATION_REQUESTED 		0x20
#define ACTIVATION_IN_PROGRESS		0x30
#define ACTIVE						0x40
#define DEACTIVATION_REQUESTED		0x50
#define DEACTIVATION_IN_PROGRESS	0x60
#define COMMUNICATION_LOST			0x70
#define DEACTIVATION_COMPLETED		0x80

#define DEACTIVATE				0
#define ACTIVATE				1
#define OBIdentifier    0x02



extern void OpenBladeInit(void);	
extern void OpenBladeStateMachine(void);	
extern void OpenBladeEvtMsg(unsigned char Mp, unsigned char Mc);	

extern CMD_STATUS GetOpenBladePropCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetModuleActivationCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetPowerLevelCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetPowerLevelCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS OBPlatformEventMsgcmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetModuleActivationPolicyCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetModuleActivationPolicyCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetChannelStateCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetChannelStateCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ModuleBMIControl(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS RenegotiatePowerCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetAddressInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetSystemEvtLogPolicyCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ModulePayloadControlCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetServiceInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS ApiDoingGetServiceInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut); 
extern CMD_STATUS GetAppletPackageURICmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetServiceEnableStateCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetServiceEnableStateCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS SetServiceTicketCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS StopServiceSessionCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
#endif  


