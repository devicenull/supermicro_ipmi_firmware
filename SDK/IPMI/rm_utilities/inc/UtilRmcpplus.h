
#ifndef _UTILRMCPPLUS_H_
#define _UTILRMCPPLUS_H_


#define NO_ERRORS                                       0x00

#define INSUFFICIENT_RESOURCES_TO_CREATE_A_SESSION      0x01

#define INVALID_SESSION_ID                              0x02

#define INVALID_PAYLOAD_TYPE                            0x03

#define INVALID_AUTHENTICATION_ALGORITHM                0x04

#define INVALID_INTEGRITY_ALGORITHM                     0x05

#define UNAUTHORIZED_ROLE_OR_PRIVILEGE_LEVEL_REQUESTED  0x0A 

#define INACTIVE_SESSION_ID                             0x08

#define INVALID_ROLE		                            0x09
#define INVALID_NAME_LENGTH                             0x0C

#define UNAUTHORIZED_NAME                               0x0D

#define INVALID_INTEGRITY_CHECK_VALUE                   0X0F

#define INVALID_CONFIDENTIALITY_ALGORITHM               0x10

#define NO_CIPHER_SUITE_MATCH                           0x11

#define ILLEGAL_OR_UNRECOGNIZED_PARAMETER               0x12

#define RESERVE                                         0x00

#define KEY_IS_LOCKED                                   0x80

#define KR_NOT_USED                                     0x84

#define HMAC_SHA1_LENGTH								0x14
#define HMAC_SHA1_96_LENGTH								0x0C
#define HMAC_MD5_LENGTH									0x10

#define RAKP_NONE										0x00
#define RAKP_HMAC_SHA1									0x01
#define RAKP_HMAC_MD5									0x02	
#define RAKP_HMAC_SHA1_96								0x01
#define RAKP_HMAC_MD5_128								0x02
#define RAKP_MD5_128									0x03

#define AES_CBC_128										0x01
#define xRC4_128										0x02
#define xRC4_40										0x03

#ifdef _UTILRMCPPLUS_C_
extern void UtilSetChannelSecurityKeys(BYTE, BYTE, BYTE *);
extern CMD_STATUS UtilOpenSessionRsCMD(MsgBufFmt *, MsgBufFmt *);           
extern CMD_STATUS UtilRAKPMessage1CMD(MsgBufFmt *, MsgBufFmt *);            
extern CMD_STATUS UtilRAKPMessage3CMD(MsgBufFmt *, MsgBufFmt *); 


#else

void UtilSetChannelSecurityKeys(BYTE, BYTE, BYTE *);
void ConcatenationSIKData(BYTE, BYTE, BYTE *, BYTE *);    
void ConcatenationData_RAKP1RS(BYTE, BYTE *,BYTE *);
void RMCPplusGetUserInfo(BYTE, BYTE *,BYTE *); 	
void ConcatenationVerifyData(BYTE, BYTE *,BYTE *);       
							                       							
CMD_STATUS UtilOpenSessionRsCMD(MsgBufFmt *, MsgBufFmt *);          
CMD_STATUS UtilRAKPMessage1CMD(MsgBufFmt *, MsgBufFmt *);           
CMD_STATUS UtilRAKPMessage3CMD(MsgBufFmt *, MsgBufFmt *);          
#endif

#endif

