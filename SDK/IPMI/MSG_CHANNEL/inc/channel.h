
#ifndef _CHANNEL_H_
#define _CHANNEL_H_  


#ifdef _CHANNEL_C_

#else

extern STATUS ChannelSend(MsgBufFmt *pMsgBuf);
extern BYTE ChannelID2DeviceID(BYTE ChannelID);
extern STATUS IsLANChannel(BYTE ChannelID);
extern STATUS IsUARTChannel(BYTE ChannelID);
extern STATUS IsChannelExist(BYTE ChannelID);


#endif  


#endif  





