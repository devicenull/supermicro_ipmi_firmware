
#ifndef _SDRCMDS_H_
#define _SDRCMDS_H_


#define MAX_SDR_REC_LEN                    64

#ifndef SDR_VERSION
#define SDR_VERSION 									0x51
#endif

typedef struct 
{
    WORD w_ReservedID;
    BYTE b_Cacelled;
    
} St_SdrReservedInfoTag;

#ifdef _SDRCMDS_C_



void SDRTimer(ULONG dummy);

void ApiResvCancel(void);
WORD ApiReserveSDRRepositoryCmd (void);
WORD ApiDoingGetSDRReposInfoCmd (MsgBufFmt *);
WORD ApiDoingGetSDRReposAllocInfoCmd (MsgBufFmt *);
WORD ApiDoingGetSDRCmd (MsgBufFmt *, MsgBufFmt *);
WORD ApiDoingPartialAddSDRCmd (MsgBufFmt *, MsgBufFmt *);
WORD ApiDoingClearSDRReposCmd (MsgBufFmt *, MsgBufFmt *);

CMD_STATUS GetSDRRepositoryInfoCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSDRRepositoryAllocationInfoCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS ReserveSDRRepositoryCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSDRCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS AddSDRCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS PartialAddSDRCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS DeleteSDRCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS ClearSDRRepositoryCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetSDRRepositoryTimeCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS SetSDRRepositoryTimeCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS RunInitializationAgentCmd(MsgBufFmt *, MsgBufFmt *);
CMD_STATUS GetDeviceSDRCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
CMD_STATUS GetDeviceSDRInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

#else

extern void ApiResvCancel(void);
extern WORD ApiReserveSDRRepositoryCmd (void);
extern WORD ApiCheckSDRFreeSpace (void);
extern WORD ApiDoingGetSDRReposInfoCmd (MsgBufFmt *);
extern WORD ApiDoingGetSDRReposAllocInfoCmd (MsgBufFmt *);
extern WORD ApiDoingGetSDRCmd (MsgBufFmt *, MsgBufFmt *);
extern WORD ApiDoingPartialAddSDRCmd (MsgBufFmt *, MsgBufFmt *);
extern WORD ApiDoingClearSDRReposCmd (MsgBufFmt *, MsgBufFmt *);

extern CMD_STATUS GetSDRRepositoryInfoCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSDRRepositoryAllocationInfoCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS ReserveSDRRepositoryCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSDRCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS AddSDRCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS PartialAddSDRCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS DeleteSDRCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS ClearSDRRepositoryCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetSDRRepositoryTimeCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS SetSDRRepositoryTimeCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS RunInitializationAgentCmd(MsgBufFmt *, MsgBufFmt *);
extern CMD_STATUS GetDeviceSDRCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);
extern CMD_STATUS GetDeviceSDRInfoCmd(MsgBufFmt *p_St_MsgBufIn, MsgBufFmt *p_St_MsgBufOut);

#endif
#endif






