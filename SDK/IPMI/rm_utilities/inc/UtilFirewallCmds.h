
#ifdef IPMI_FW_FIREWALL

#ifndef	_UTILFIREWALLCMDS_H_
#define	_UTILFIREWALLCMDS_H_

#include "./../../COMMON/inc/status.h"
#include "./../../COMMON/inc/type.h"


#if 1
#define NVRAM_DATA_START 	sizeof(WORD) * 4
#else
#define NVRAM_DATA_START 	sizeof(WORD) * 0
#endif
#define CMDCRC_LOC 			sizeof(WORD) * 0
#define SUBCRC_LOC 			sizeof(WORD) * 1
#define CMDINITCRC_LOC 		sizeof(WORD) * 2
#define SUBINITCRC_LOC 		sizeof(WORD) * 3

#define WRITE_NVRAM 		1
#define DONT_WRITE_NVRAM 	0


#ifdef _UTILFIREWALLCMDS_C_
WORD UtilbbCRCCal_helf(WORD PastCRC, BYTE *Startaddr,BYTE length );
WORD UtilCmdCRCCalCulate(unsigned short int Location, unsigned char WriteNVRAM);
WORD UtilSubFnCRCCalCulate(unsigned short int Location, unsigned char WriteNVRAM);
void UtilCustomTableLoading(void);
void UtilSaveEnableFlagsToNVRam(void);
STATUS FWFireWallInit(unsigned char p1, unsigned char p2, void *p3);

#else
extern WORD UtilbbCRCCal_helf(WORD PastCRC, BYTE *Startaddr,BYTE length );
extern WORD UtilCmdCRCCalCulate(unsigned short int Location, unsigned char WriteNVRAM);
extern WORD UtilSubFnCRCCalCulate(unsigned short int Location, unsigned char WriteNVRAM);
extern void UtilCustomTableLoading(void);
extern void UtilSaveEnableFlagsToNVRam(void);
extern STATUS FWFireWallInit(unsigned char p1, unsigned char p2, void *p3);

#endif 

#endif 

#endif 

