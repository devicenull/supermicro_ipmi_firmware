
#ifndef	_UTILCHNLCMD_H_
#define	_UTILCHNLCMD_H_

#ifdef _UTILCHNLCMD_C_
BYTE UtilChannelID2DeviceID(BYTE ChannelID);
BYTE UtilChannelID2MediaType(BYTE ChannelID);
STATUS UtilChannelSend(MsgBufFmt *pMsgBuf);
STATUS UtilIsChannelEnablePEFAlerting(BYTE b_ChanNo);
STATUS UtilIsChannelExist(BYTE ChannelID);
STATUS UtilIsICMBChannel(BYTE ChannelID);
STATUS UtilIsLANChannel(BYTE ChannelID);
STATUS UtilIsUARTChannel(BYTE ChannelID);
void UtilLoadChannelInfoDefault2PS(void);
void UtilPSChnlCommit(void);
STATUS UtilSearchChannelNo(BYTE MediaType, BYTE *p_ChannelNo);
STATUS UtilSearchLanChannelNo(BYTE b_IRQ, BYTE *p_ChannelNo);
STATUS UtilSearchLanChnNoByAddr(BYTE b_Address, BYTE *p_ChannelNo);
#else
extern BYTE UtilChannelID2DeviceID(BYTE ChannelID);
extern BYTE UtilChannelID2MediaType(BYTE ChannelID);
extern STATUS UtilChannelSend(MsgBufFmt *pMsgBuf);
extern STATUS UtilIsChannelEnablePEFAlerting(BYTE b_ChanNo);
extern STATUS UtilIsChannelExist(BYTE ChannelID);
extern STATUS UtilIsICMBChannel(BYTE ChannelID);
extern STATUS UtilIsLANChannel(BYTE ChannelID);
extern STATUS UtilIsUARTChannel(BYTE ChannelID);
extern void UtilLoadChannelInfoDefault2PS(void);
extern void UtilPSChnlCommit(void);
extern STATUS UtilSearchChannelNo(BYTE MediaType, BYTE *p_ChannelNo);
extern STATUS UtilSearchLanChannelNo(BYTE b_IRQ, BYTE *p_ChannelNo);
extern STATUS UtilSearchLanChnNoByAddr(BYTE b_Address, BYTE *p_ChannelNo);
#endif

#endif 

