 
#ifndef _SELCMDS_H_
#define _SELCMDS_H_
#define SELPVCMD_BEGIN		0xE0
#define SELPVCMD_DUMPALL	0xE0	
#define SELPVCMD_QUIT		0xFE

#define SEL_ENTRY_BYTES		20
#define SEL_LISTHEAD_BYTES  10  
#define SEL_SPEC_ENTRY_SIZE 16
#define SEL_MAX_ENTRIES		(SEL_MAXSIZE_BYTES/SEL_ENTRY_BYTES)

#define SEL_FUNC_OK			0
#define SEL_FUNC_FAIL		1

#define SEL_NVRAM_NSAVE     0
#define SEL_NVRAM_SAVE      1

#define SEL_ENTRY_INFO_CLEAR 0x00
#define SEL_ENTRY_INFO_PEF   0x01

#ifndef SEL_VERSION
#define SEL_VERSION         0x51
#endif

#define GET_ENTRY_ADDR(idx)     (SEL_LISTHEAD_BYTES+SEL_ENTRY_BYTES*(idx-1))

typedef struct
{
    BYTE b_Status;
    BYTE a_b_Data[16];      
} St_SelPefEvtTag; 


#ifdef _SELCMDS_C_

BYTE				ApiDoingGetSelInfoCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);
BYTE				ApiDoingGetSELAllocationInfoCmd(MsgBufFmt *MsgOut);
BYTE				ApiDoingGetSelEntryCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);
BYTE				ApiDoingAddSelEntryCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);
BYTE				ApiDoingResvSelCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);
BYTE				ApiDoingClearSelCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);
BYTE				ApiDoingGetSelTimeCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);
BYTE				ApiDoingSetSelTimeCmd(MsgBufFmt *pMsgIn, MsgBufFmt *pMsgOut);

DWORD				ApiSelGetLastDelTime(void);

CMD_STATUS			GetSelInfoCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			GetSELAllocationInfoCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			GetSelEntryCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			AddSelEntryCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			ResvSelCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			ClearSelCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			GetSelTimeCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS			SetSelTimeCmd(MsgBufFmt *, MsgBufFmt *);

STATUS				ForceClearSelCmd(void);

#else

extern DWORD		ApiSelGetLastDelTime(void);

extern CMD_STATUS	GetSelInfoCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	GetSELAllocationInfoCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	GetSelEntryCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	AddSelEntryCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	ResvSelCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	ClearSelCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	GetSelTimeCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS	SetSelTimeCmd(MsgBufFmt *, MsgBufFmt *);

extern STATUS		ForceClearSelCmd(void);

#endif 
#endif 





