
#ifdef IPMI_FW_FIREWALL

#ifndef	_FIREWALL_RULE_H_
#define	_FIREWALL_RULE_H_
#endif 

extern BYTE CHKFirewallSubFnEnable(MsgBufFmt *MsgBufIn, BYTE b_SubFnIndex);
extern BYTE CHKFirewallSubFn(MsgBufFmt *MsgBufIn);
extern BYTE FirmwareFirewall(MsgBufFmt *MsgBufIn);

#endif 




