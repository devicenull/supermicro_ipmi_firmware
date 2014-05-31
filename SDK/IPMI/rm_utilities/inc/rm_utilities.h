

#ifndef	_RM_UTILITIES_H_
#define	_RM_UTILITIES_H_



int		UtilGetSOLSema(void); 

int		UtilPutSOLSema(void); 



BYTE	utilGetMessageQueue (BYTE *p_dw_Enqueued);

void	utilGetSystemGUID (BYTE *p_b_Data);

void	utilSetSystemGUID (
						   BYTE *p_b_Data, 
						   BYTE b_length
						  );

void 	utilSetUserPayloadAccess(
								 BYTE b_CMDOperation, 
								 BYTE b_UserID, 
								 BYTE b_CMDChanNum, 
								 BYTE *p_b_data
								);

void 	utilSuspendOrResumePayloadEncryption(BYTE b_Operation);



STATUS	InitWatchdog(void);

STATUS 	UtilEnterWDTMutex(void);

STATUS 	UtilExitWDTMutex(void);

STATUS 	UtilStopWDT(void);

BYTE   	UtilSetWDTInitCountdownValue(
									 BYTE b_lscountdown, 
									 BYTE b_mscountdown
									);
BYTE   	UtilKeepWDTAlive(void);

BYTE   	UtilResetWDt(void);



void utilSetupAutoARP(void);



WORD	UtilSDRCmdsInit(void);           
    
WORD	UtilRetrieveSingleSDR (
							   WORD, 
							   WORD *
							  );
							  

void	UtilSelInitInfo(
						DWORD *p_dw_addtm, 
						WORD *p_w_last, 
						WORD *p_w_revID, 
						WORD *p_w_nums
					   );
					  
int		UtilSelAddEntry(
						BYTE *p_b_data, 
						WORD w_Idx
					   );
						
void	UtilSelUpdateSelCrc(BYTE *p_b_NvramSel);

void	UtilSelClrAll(WORD);

void	UtilSelGetIDs(
					  WORD w_Id, 
					  WORD *p_w_RealId, 
					  WORD *p_w_NextId
					 );
						
BYTE*	UtilSelAbstractEntry(BYTE *p_b_NvramSel);

BYTE	UtilSelGetSelInfo(BYTE *p_b_NvramSel);

void	UtilSelUpdateSelInfo(
							 BYTE *p_b_NvramSel, 
							 BYTE b_NewInfo
							);
								
void	UtilSelUpdateSelUsed(
							 BYTE *p_b_InfoBuff, 
							 WORD w_Idx, 
							 WORD w_Limit
							);
							
BOOLEAN	UtilSelGetSelUsed(
						  BYTE *p_b_InfoBuff, 
						  WORD w_Idx, 
						  WORD w_Limit
						  );

WORD	UtilSelGetTm(DWORD *p_dw_tm);

WORD	UtilSelSetTm(DWORD dw_tm);

void	UtilSelClrCrc(BYTE *p_b_Buff);

void	UtilSelUpdateCrc(
						 BYTE *p_b_Buff, 
						 BYTE b_Crc
						);

BYTE	UtilSelComputCrc(
						 BYTE *p_b_Buff, 
						 WORD w_len
						);
						
STATUS	UtilSelCheckCrc(WORD w_Idx);


void	UtilSelPefLogEvt2InternalBuf(BYTE *p_b_data);

void	UtilSelCmdInit(
					   BYTE b_p1, 
  					   BYTE b_p2, 
					   void *p_b_p3
					  );
									  

BOOLEAN	UtilSelLogEnable(void);

STATUS	UtilSelPefProcSelEvt(
							 BYTE *p_b_Sel, 
							 BYTE b_Len, 
							 BYTE *p_b_Info, 
							 WORD *p_w_NextId, 
							 WORD w_SelId
							);

STATUS	UtilSelPefDoneSelEvt(
							 WORD w_SelId, 
							 DWORD dw_Time, 
							 BYTE b_Stat
							);
				
						

						
BYTE	UtilGetSensorEventEnable(
							     BYTE ,
							     BYTE *,
							     BYTE *,
							     BYTE *,
							     BYTE *,
							     BYTE *
							    );
                                     
BYTE 	UtilGetSensorEventStatus(
							     BYTE ,
							     BYTE *,
							     BYTE *,
							     BYTE *,
						         BYTE *,
						         BYTE *
						        );
                                     
BYTE 	UtilGetSensorHysteresis(
								BYTE ,
								BYTE *,
								BYTE *
							   );

BYTE 	UtilGetSensorReading(
							 BYTE ,
							 BYTE *,
							 BYTE *,
							 BYTE *,                                     
							 BYTE *,
							 BYTE *
							);

BYTE 	UtilGetSensorThreshold(
							   BYTE ,
							   BYTE * ,
							   BYTE * ,
							   BYTE * ,
							   BYTE * ,
							   BYTE * ,
							   BYTE * ,
							   BYTE * 
							  );

BYTE 	UtilRearmSensorEvents(
							  BYTE ,
							  BYTE ,
							  BYTE ,
							  BYTE ,
							  BYTE ,
							  BYTE 
							 );
                                  
BYTE 	UtilSetSensorEventEnable(
								 BYTE ,
								 BYTE ,
								 BYTE ,
								 BYTE ,
								 BYTE ,
								 BYTE 
								);
                                           
// joe --- begin 7-24-2010
void UtilSetSDR_M(
					BYTE ,
					BYTE ,
					BYTE 
				);

BYTE 	UtilSetSDRParameter(
							BYTE ,
							BYTE ,
							BYTE *,
							BYTE
						   );
// joe --- end 7-24-2010 
                                           
BYTE 	UtilSetSensorHysteresis(
								BYTE ,
								BYTE ,
								BYTE 
							   );
                                            
BYTE 	UtilSetSensorThreshold(
							   BYTE ,
							   BYTE ,
							   BYTE ,
							   BYTE ,
	                           BYTE ,
	                           BYTE ,
	                           BYTE ,
	                           BYTE ,
	                           BYTE );
	                         
 
void 	SENSORMTX_LOCK(void);

void 	SENSORMTX_UNLOCK(void);

						                                                        


BYTE 	UtilGetPowerStatus(void);

void 	UtilSetPowerRestorePolicy(BYTE b_Policy);

void 	POHTimerFunc(DWORD dw_Info);

STATUS 	POHTimerInit(void);


WORD 	UtilGetPETSequenceNO(void);

void 	UtilImmedPETEvent(                   
                    	  DWORD dw_EventID,
						  void  *Ptr
                         );

void 	UtilPEFPostponeTimerWakeupCheck(void); 

STATUS 	UtilPETAcknowledgeFilter(BYTE *p_b_ReqData);   

void 	UtilResetLastProcessedEventID(void);

void 	UtilSetLastProcIDBySW(BYTE b_data);



BYTE 	UtilChannelID2DeviceID(BYTE ChannelID);

BYTE 	UtilChannelID2MediaType(BYTE ChannelID);

void 	UtilChannelInfoVolatileInit(void);

STATUS 	UtilChannelSend(MsgBufFmt *pMsgBuf);

STATUS 	UtilIsChannelEnablePEFAlerting(BYTE b_ChanNo);

STATUS 	UtilIsChannelExist(BYTE ChannelID);

STATUS 	UtilIsICMBChannel(BYTE ChannelID);

STATUS 	UtilIsLANChannel(BYTE ChannelID);

STATUS 	UtilIsUARTChannel(BYTE ChannelID);

void 	UtilLoadChannelInfoDefault2PS(void);

void 	UtilPSChnlCommit(void);

STATUS 	UtilSearchChannelNo(
							BYTE MediaType, 
							BYTE *p_ChannelNo
						   );

STATUS 	UtilSearchLanChannelNo(
							   BYTE b_IRQ, 
							   BYTE *p_ChannelNo
							  );

STATUS 	UtilSearchLanChnNoByAddr(
								 BYTE b_Address, 
								 BYTE *p_ChannelNo
								);

							  


void	UtilPSSOLCommit(void); 

void 	UtilPSSerialCommit(void); 

void 	UtilPSUserCommit(void);
					  
BYTE 	UtilGetEnabledUsrCount(BYTE b_chnlNum);

BYTE 	UtilGetUserID(BYTE *p_b_UserName);

STATUS 	UtilEnterPSCriticalSection(void);

STATUS 	UtilLeavePSCriticalSection(void);

void 	UtilLoadUserInfoDefault2PS(void);





void 	UtilSetUserEnable(
						  BYTE b_userID,
						  BYTE b_Enabled
						 );

BYTE 	UtilSetUserName(
						BYTE b_userID,
						BYTE* a_b_Name
					   );

void 	UtilGetUserName(
						BYTE * a_b_Name, 
						BYTE b_userID
						);
void 	UtilSetUserPassword(
							BYTE b_PasswordLength,
							BYTE b_userID,
							BYTE* b_Password
						   );

#ifdef 	OEM_AST2000_LOGINFO
void 	UtilLoadLogInfoDefault2PS(void);
#endif							  
          

void	UtilNvramSelInit(void);

void	UtilNvramSelDelEntryId(WORD w_Idx);

WORD	UtilNvramSelGet1stEntryId(void);

void	UtilNvramSelEndingEntryId(WORD w_Idx);

WORD	UtilNvramSelGetNextEntryId(WORD w_NowIdx);

WORD	UtilNvramSelGetAvailEntry(BYTE *);

void	UtilNvramSelLinkEntry(
							  WORD w_Old, 
							  WORD w_New
							 );
							 
WORD	UtilNvramSelGetEntry(
							 BYTE *p_b_Sel, 
							 WORD w_Idx
							);
							
WORD	UtilNvramSelAddEntry(
							 BYTE *p_b_data, 
							 WORD w_Idx, 
							 WORD w_Commit
							);
							
DWORD	UtilNvramSelGetMaxSize(void);

void	UtilNvramSelSet1stEntryId(WORD firstID);

void	UtilSelNVRamWrite(
						  BYTE b_NVRamType,
	                      WORD w_offset,
						  WORD w_length,
						  BYTE *p_b_WriteBuf
						 );

void	UtilSelNVRamRead(
						 BYTE b_NVRamType,
						 WORD w_offset,
						 WORD w_length,
						 BYTE *p_b_ReadBuf
						);

BOOLEAN	UtilSelInNvramErase(void);

void	UtilSELNVRamInit(
						 DWORD, 
						 void  *
						);          

#endif


