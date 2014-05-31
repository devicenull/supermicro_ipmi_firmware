
#ifndef _UTILLANCMD_H_
#define _UTILLANCMD_H_
#endif

#ifdef _UTILLANCMD_C_

STATUS EscapeShellCmd(BYTE *p_b_InBuf, BYTE b_flag);
void UtilSetIPAddress(BYTE, BYTE *);
void UtilSetMACAddress(BYTE, BYTE *);
void UtilSetSubnetMask(BYTE, BYTE *);
void UtilSetDefaultGatewayAddress(BYTE, BYTE *);
void UtilSetDefaultGatewayMACAddress(BYTE,BYTE *);
void UtilSetPriRMCPPort(BYTE, BYTE *);
STATUS UtilSetupAutoARP(void);
STATUS UtilClrStatistics(BYTE *);
STATUS UtilGetStatistics(BYTE, BYTE *);                  
void UtilDHCPStop(BYTE*);
void LanConfigApply(DWORD , void *);
extern BYTE UtilGetIPConf(DWORD *,DWORD *, BYTE *);
extern void LanConfigApply(DWORD , void *);
extern void UtilSendARP(DWORD,void *);
extern  DWORD UtilGetGWAddr(BYTE *);
extern  BYTE UtilGetMACAddr(BYTE *, BYTE *);
extern STATUS UtilGetGWMAC(DWORD ,BYTE *, BYTE *); //gateway mac fix 02-17-2011
STATUS UtilIsVLanEnable(BYTE b_DevID, WORD *p_w_ptr);
char UtilLANInterfaceSwitch(BYTE b_DevID, BYTE b_DevIfMode, WORD *p_w_LanInfo);
void UtilGetDevName_Public(BYTE b_DevID, BYTE *b_DevName, int, BYTE b_VLanIgnore);
#ifdef IPMI_DHCP
void UtilSetIPAddressSrc(BYTE, BYTE);                
#endif

#ifdef BMC_VLAN
char UtilVLanID(BYTE , BYTE*, BYTE *, BYTE);
void UtilVLanPriority(BYTE b_DevID, BYTE *devname);
#endif

#else
extern STATUS EscapeShellCmd(BYTE *p_b_InBuf, BYTE b_flag);
extern int UtilCheckIPV6Addr(const char *);
extern BYTE UtilGetIPConf(DWORD *,DWORD *, BYTE *);
extern  DWORD UtilGetGWAddr(BYTE *);
extern  BYTE UtilGetMACAddr(BYTE *, BYTE *);
extern void UtilSetIPAddress(BYTE, BYTE *);
extern void UtilSetMACAddress(BYTE, BYTE *);
extern void UtilSetSubnetMask(BYTE, BYTE *);
extern void UtilSetDefaultGatewayAddress(BYTE, BYTE *);
extern void UtilSetDefaultGatewayMACAddress(BYTE,BYTE *);
extern void UtilSetPriRMCPPort(BYTE, BYTE *);
extern STATUS UtilSetupAutoARP(void);
extern STATUS UtilClrStatistics(BYTE *);
extern STATUS UtilGetStatistics(BYTE, BYTE *);                  
extern void UtilDHCPStop(BYTE*);
extern STATUS UtilDHCPStart(BYTE, BYTE*);
extern void UtilSyncIPAddr (DWORD , void *);
extern void CheckIPChange();
extern void LanConfigApply(DWORD , void *);
extern void UtilSendARP(DWORD,void *);
extern STATUS UtilIsVLanEnable(BYTE b_DevID, WORD *p_w_ptr);
extern char UtilLANInterfaceSwitch(BYTE b_DevID, BYTE b_DevIfMode, WORD *p_w_LanInfo);
extern void UtilGetDevName_Public(BYTE b_DevID, BYTE *b_DevName, int, BYTE b_VLanIgnore);
#ifdef IPMI_DHCP
extern void UtilSetIPAddressSrc(BYTE, BYTE); 
#endif
extern void SetLink(DWORD dw_event, void *ptr);
#ifdef BMC_VLAN
extern char UtilVLanID(BYTE , BYTE *, BYTE *, BYTE);
extern void UtilVLanPriority(BYTE b_DevID, BYTE *devname);
#endif
/* Tony, 09/26/2012 Add information of network link status } */
//extern STATUS UtilLinkInfo(st_lan_link_info *p_st_lan_link_info);

//extern STATUS UtilGetNonAutoNegLanInfo(st_NonAutoNeg_lan_info *NonAutoNeg_lan_info);	// joe 1-4-2013
//extern STATUS UtilSetNonAutoNegLanInfo(st_NonAutoNeg_lan_info *NonAutoNeg_lan_info);	// joe 1-4-2013
extern int get_hostname(char *host);
extern int set_hostname(char *newhost);
#endif //end _UTILLANCMD_H_
