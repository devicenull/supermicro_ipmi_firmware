
#ifndef _UTILSELCMDS_H_
#define _UTILSELCMDS_H_

#define MAXSYSEVENTLINE 512
#define RINGBUFFERLINE 10

#ifdef _UTILSELCMDS_C_

void UtilFindFirstSelRealIndex(WORD w_Idx);

void					UtilSelInitInfo(DWORD *p_dw_addtm, 
										WORD *p_w_last, 
										WORD *p_w_revID, 
										WORD *p_w_nums);
int 					UtilSelGetCurrentNumberRecord(void);
char 					UtilSelGetLogEnabled(void);
WORD 					UtilSelGetMaxNumberRecord(void);
										
void                    UtilSelPefGetLastEvtInfo(WORD *p_w_num_entries, DWORD *p_dw_addtm);
int						UtilSelAddEntry(BYTE *p_b_data, WORD w_Idx);
void					UtilSelUpdateSelCrc(BYTE *p_b_NvramSel);
void					UtilSelClrAll(WORD);
void					UtilSelGetIDs(WORD w_Id, WORD *p_w_RealId, WORD *p_w_NextId);
BYTE					*UtilSelAbstractEntry(BYTE *p_b_NvramSel);
BYTE					UtilSelGetSelInfo(BYTE *p_b_NvramSel);
void					UtilSelUpdateSelInfo(BYTE *p_b_NvramSel, BYTE b_NewInfo);
void					UtilSelUpdateSelUsed(BYTE *p_b_InfoBuff, WORD w_Idx, WORD w_Limit);
BOOLEAN					UtilSelGetSelUsed(BYTE *p_b_InfoBuff, WORD w_Idx, WORD w_Limit);

WORD					UtilSelGetTm(DWORD *p_dw_tm);
WORD					UtilSelSetTm(DWORD dw_tm);

void					UtilSelClrCrc(BYTE *p_b_Buff);
void					UtilSelUpdateCrc(BYTE *p_b_Buff, BYTE b_Crc);
BYTE					UtilSelComputCrc(BYTE *p_b_Buff, WORD w_len);
STATUS					UtilSelCheckCrc(WORD w_Idx);

void					UtilNvramSelInit(void);
void					UtilNvramSelDelEntryId(WORD w_Idx);
WORD					UtilNvramSelGet1stEntryId(void);
void					UtilNvramSelEndingEntryId(WORD w_Idx);
WORD					UtilNvramSelGetNextEntryId(WORD w_NowIdx);
WORD					UtilNvramSelGetAvailEntry(BYTE *);
void					UtilNvramSelLinkEntry(WORD w_Old, WORD w_New);
WORD					UtilNvramSelGetEntry(BYTE *p_b_Sel, WORD w_Idx);
WORD					UtilNvramSelAddEntry(BYTE *p_b_data, WORD w_Idx, WORD w_Commit);
DWORD					UtilNvramSelGetMaxSize(void);
void					UtilSelPefLogEvt2InternalBuf(BYTE *p_b_data);
void					UtilNvramSelSet1stEntryId(WORD firstID);

void					UtilSelNVRamWrite(BYTE b_NVRamType,
										  WORD w_offset,
										  WORD w_length,
										  BYTE *p_b_WriteBuf);

void					UtilSelNVRamRead(BYTE b_NVRamType,
										 WORD w_offset,
										 WORD w_length,
										 BYTE *p_b_ReadBuf);

STATUS					UtilSelCmdInit(void);
BOOLEAN					UtilSelInNvramErase(void);
BOOLEAN					UtilSelLogEnable(void);
STATUS                  UtilSelLogEvtMsg(BYTE *p_b_EvtRecord, BYTE b_Len, WORD w_Log, BYTE b_type);

STATUS                  UtilSelLogAvail(BYTE b_Stat); 

STATUS					UtilSelPefProcSelEvt(BYTE *p_b_Sel, 
											 BYTE b_Len, 
											 BYTE *p_b_Info, 
											 WORD *p_w_NextId, 
											 WORD w_SelId);

STATUS					UtilSelPefDoneSelEvt(WORD w_SelId, DWORD dw_Time, BYTE b_Stat);
void					UtilSELNVRamInit(DWORD, void  *);
void          UtilFileRing(void);


#else
extern void UtilFindFirstSelRealIndex(WORD w_Idx);
extern void				UtilSelInitInfo(DWORD *p_dw_addtm, 
										WORD *p_w_last, 
										WORD *p_w_revID, 
										WORD *p_w_nums);

extern void             UtilSelPefGetLastEvtInfo(WORD *p_w_num_entries, DWORD *p_dw_addtm);
extern int				UtilSelAddEntry(BYTE *p_b_data, WORD w_Idx);
extern void				UtilSelUpdateSelCrc(BYTE *p_b_NvramSel);
extern void				UtilSelGetIDs(WORD w_Id, WORD *p_w_RealId, WORD *p_w_NextId);
extern BYTE				*UtilSelAbstractEntry(BYTE *p_b_NvramSel);
extern BYTE				UtilSelGetSelInfo(BYTE *p_b_NvramSel);
extern void				UtilSelUpdateSelInfo(BYTE *p_b_NvramSel, BYTE b_NewInfo);
extern void				UtilSelUpdateSelUsed(BYTE *p_b_InfoBuff, WORD w_Idx, WORD w_Limit);
extern BOOLEAN			UtilSelGetSelUsed(BYTE *p_b_InfoBuff, WORD w_Idx, WORD w_Limit);

extern WORD				UtilSelGetTm(DWORD *p_dw_tm);
extern WORD				UtilSelSetTm(DWORD dw_tm);

extern void				UtilSelClrCrc(BYTE *p_b_Buff);
extern void				UtilSelUpdateCrc(BYTE *p_b_Buff, BYTE b_Crc);
extern BYTE				UtilSelComputCrc(BYTE *p_b_Buff, WORD w_len);
extern STATUS			UtilSelCheckCrc(WORD w_Idx);

extern void				UtilNvramSelInit(void);
extern void				UtilNvramSelDelEntryId(WORD w_Idx);
extern WORD				UtilNvramSelGet1stEntryId(void);
extern void				UtilNvramSelEndingEntryId(WORD w_Idx);
extern WORD				UtilNvramSelGetNextEntryId(WORD w_NowIdx);
extern WORD				UtilNvramSelGetAvailEntry(BYTE *);
extern void				UtilNvramSelLinkEntry(WORD w_Old, WORD w_New);

#ifdef __cplusplus
extern "C"{
#endif
extern void				UtilSelClrAll(WORD);
extern WORD				UtilNvramSelGetEntry(BYTE *p_b_Sel, WORD w_Idx);
extern int 	UtilSelGetCurrentNumberRecord(void);
extern char	UtilSelGetLogEnabled(void);
extern int	UtilSelGetMaxNumberRecord(void);
#ifdef __cplusplus
}
#endif

extern WORD				UtilNvramSelAddEntry(BYTE *p_b_data, WORD w_Idx, WORD w_Commit);
extern DWORD			UtilNvramSelGetMaxSize(void);
extern void				UtilSelPefLogEvt2InternalBuf(BYTE *p_b_data);
extern void				UtilNvramSelSet1stEntryId(WORD firstID);

extern void				UtilSelNVRamWrite(BYTE b_NVRamType,
										  WORD w_offset,
										  WORD w_length,
										  BYTE *p_b_WriteBuf);

extern void				UtilSelNVRamRead(BYTE b_NVRamType,
										 WORD w_offset,
										 WORD w_length,
										 BYTE *p_b_ReadBuf);

extern STATUS			UtilSelCmdInit(void);
extern BOOLEAN			UtilSelInNvramErase(void);
extern BOOLEAN			UtilSelLogEnable(void);
extern STATUS           UtilSelLogEvtMsg(BYTE *p_b_EvtRecord, BYTE b_Len, WORD w_Log, BYTE b_type);
extern STATUS           UtilSelLogAvail(BYTE b_Stat); 
extern STATUS			UtilSelPefProcSelEvt(BYTE *p_b_Sel, 
											 BYTE b_Len, 
											 BYTE *p_b_Info, 
											 WORD *p_w_NextId, 
											 WORD w_SelId);

extern STATUS			UtilSelPefDoneSelEvt(WORD w_SelId, DWORD dw_Time, BYTE b_Stat);
extern void				UtilSELNVRamInit(DWORD, void  *);
extern void       UtilSaveSysEventLog(int Action, char *useraccount, char *ipaddr);
#endif 
#endif 



