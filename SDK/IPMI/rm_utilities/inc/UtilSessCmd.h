

#ifndef _UTILSESSCMD_H_
#define _UTILSESSCMD_H_

#ifdef _UTILSESSCMD_C_ 

void		UtilAssignPayloadTypeToSessTbl(BYTE Idx, BYTE b_Payload);
void		UtilAuthCodeGen(BYTE b_AuthType,DWORD dw_SessionID,DWORD *p_dw_SessionSeqNum,BYTE *p_b_IPMIMsgData,BYTE b_IPMIMsgDataLength,BYTE *p_b_AuthCode);

BYTE 		UtilSessIDToSessTblIdx(DWORD dw_SessID);

DWORD		UtilChnlNoToSessID(BYTE b_ChnlNo);
DWORD		UtilChnlNoToTSessID(BYTE b_ChnlNo);

STATUS		UtilRMCPSessSearch(DWORD dw_SessID,BYTE *b_Idx);
STATUS		UtilAuthCodeVerify(BYTE b_AuthType,DWORD dw_SessionID,DWORD dw_SessionSeqNum,BYTE *p_b_Pri,BYTE *p_b_IPMIMsgData,BYTE b_IPMIMsgDataLength,BYTE *p_b_AuthCode,BYTE *p_b_SourceIP,BYTE *p_b_SourcePort);


#ifdef IPMI_RMCP_PLUS

void		UtilRMCPplusSessRelease(BYTE b_Idx);
void		UtilRMCPplusSessClosingCheck(BYTE b_Idx);
void		UtilRMCPplusGetCipherSuiteInfo(BYTE b_Idx, BYTE *p_b_EncryON, BYTE *p_b_AuthAlgor, BYTE *p_b_IntgrityAlgor, BYTE *p_b_ConfidntyAlgor, void **p_IntegSIK, void **p_EncrySIK);
void		UtilRMCPplusGetRmtSessID(BYTE b_Idx, DWORD *dw_RmtSessID);
void		UtilRMCPplusAuthCodeGen(BYTE b_AlgoriType,BYTE b_AuthType,BYTE *p_b_Key,BYTE b_Keylength,BYTE *p_b_Data,BYTE b_DataLength,BYTE *p_b_AuthCode);

BYTE		UtilRMCPplusSessAlloc(BYTE b_ChnlNo);
BYTE		UtilRMCPplusSessActivate(BYTE b_Idx, BYTE b_ChnlNo);
BYTE		UtilRMCPplusIPMIIntegrityPadCntCal(WORD w_PayloadLen);
BYTE		*UtilRMCPplusIPMIIntegrityPadding(BYTE b_OP, BYTE b_AlgoriType, WORD w_PayloadLen, BYTE *p_b_PAD);
BYTE		*UtilRMCPplusIPMIIntegritySignAppend(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD w_srclen, BYTE *p_b_srcdata,BYTE *p_b_destdata);

STATUS		UtilRMCPplusSessSearch(DWORD dw_SessID, BYTE *b_Idx);
STATUS		UtilRMCPplusInBandSeqNoCheck(BYTE b_SessIdx,DWORD dw_SessSeqNum);
STATUS		UtilRMCPplusIPMIIntegritySignVerify(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD  w_srclen,BYTE *p_b_srcdata,BYTE *p_b_cmpdata);
STATUS		UtilRMCPplusAuthCodeVerify(BYTE b_AlgoriType,BYTE b_AuthType,BYTE *p_b_Key,BYTE b_Keylength,BYTE *p_b_Data,BYTE b_DataLength,BYTE *p_b_AuthCode);

#ifdef IPMI_RMCP_PLUS_SUB_AES

void		UtilRMCPplusPayloadDecryption(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD  w_srclen,BYTE *p_b_IV,void *p_packetdata);
void		UtilRMCPplusPayloadEncryption(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD  w_srclen,BYTE *p_b_packetdata);
BYTE		UtilRMCPplusIPMIConfidentialityPadCntCal(WORD w_PayloadLen);
BYTE		*UtilRMCPplusIPMIConfidentialityPadding(BYTE b_OP,BYTE b_AlgoriType,WORD w_PayloadLen,BYTE *p_b_PAD);
                                     
#endif 
#endif	


#else 


extern void			UtilAssignPayloadTypeToSessTbl(BYTE Idx, BYTE b_Payload);
extern void			UtilAuthCodeGen(BYTE b_AuthType,DWORD dw_SessionID,DWORD *p_dw_SessionSeqNum,BYTE *p_b_IPMIMsgData,BYTE b_IPMIMsgDataLength,BYTE *p_b_AuthCode);

extern BYTE 		UtilSessIDToSessTblIdx(DWORD dw_SessID);
extern DWORD		UtilChnlNoToSessID(BYTE b_ChnlNo);
extern DWORD		UtilChnlNoToTSessID(BYTE b_ChnlNo);

extern STATUS		UtilRMCPSessSearch(DWORD dw_SessID,BYTE *b_Idx);
extern STATUS		UtilAuthCodeVerify(BYTE b_AuthType,DWORD dw_SessionID,DWORD dw_SessionSeqNum,BYTE *p_b_Pri,BYTE *p_b_IPMIMsgData,BYTE b_IPMIMsgDataLength,BYTE *p_b_AuthCode,BYTE *p_b_SourceIP,BYTE *p_b_SourcePort);


#ifdef IPMI_RMCP_PLUS

extern void			UtilRMCPplusSessRelease(BYTE b_Idx);
extern void			UtilRMCPplusSessClosingCheck(BYTE b_Idx);
extern void			UtilRMCPplusGetCipherSuiteInfo(BYTE b_Idx, BYTE *p_b_EncryON, BYTE *p_b_AuthAlgor, BYTE *p_b_IntgrityAlgor, BYTE *p_b_ConfidntyAlgor, void **p_IntegSIK, void **p_EncrySIK);
extern void			UtilRMCPplusGetRmtSessID(BYTE b_Idx, DWORD *dw_RmtSessID);
extern void			UtilRMCPplusAuthCodeGen(BYTE b_AlgoriType,BYTE b_AuthType,BYTE *p_b_Key,BYTE b_Keylength,BYTE *p_b_Data,BYTE b_DataLength,BYTE *p_b_AuthCode);

extern BYTE			UtilRMCPplusSessAlloc(BYTE b_ChnlNo);
extern BYTE			UtilRMCPplusSessActivate(BYTE b_Idx, BYTE b_ChnlNo);
extern BYTE			UtilRMCPplusIPMIIntegrityPadCntCal(WORD w_PayloadLen);
extern BYTE			*UtilRMCPplusIPMIIntegrityPadding(BYTE b_OP, BYTE b_AlgoriType, WORD w_PayloadLen, BYTE *p_b_PAD);
extern BYTE			*UtilRMCPplusIPMIIntegritySignAppend(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD w_srclen, BYTE *p_b_srcdata,BYTE *p_b_destdata);

extern STATUS		UtilRMCPplusSessSearch(DWORD dw_SessID, BYTE *b_Idx);
extern STATUS		UtilRMCPplusInBandSeqNoCheck(BYTE b_SessIdx,DWORD dw_SessSeqNum);
extern STATUS		UtilRMCPplusIPMIIntegritySignVerify(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD  w_srclen,BYTE *p_b_srcdata,BYTE *p_b_cmpdata);
extern STATUS		UtilRMCPplusAuthCodeVerify(BYTE b_AlgoriType,BYTE b_AuthType,BYTE *p_b_Key,BYTE b_Keylength,BYTE *p_b_Data,BYTE b_DataLength,BYTE *p_b_AuthCode);

#ifdef IPMI_RMCP_PLUS_SUB_AES

extern void			UtilRMCPplusPayloadDecryption(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD  w_srclen,BYTE *p_b_IV,void *p_packetdata);
extern void			UtilRMCPplusPayloadEncryption(BYTE b_AlgoriType,BYTE *p_b_SIK,WORD  w_srclen,BYTE *p_b_packetdata);
extern BYTE			UtilRMCPplusIPMIConfidentialityPadCntCal(WORD w_PayloadLen);
extern BYTE			*UtilRMCPplusIPMIConfidentialityPadding(BYTE b_OP,BYTE b_AlgoriType,WORD w_PayloadLen,BYTE *p_b_PAD);
                                     
#endif 
#endif	
					  


#endif 
#endif 

