

#ifndef _UTILSDRCMDS_H_
#define _UTILSDRCMDS_H_

#ifdef _UTILSDRCMDS_C_

WORD 				UtilSDRCmdsInit(void);
WORD 				UtilRetrieveSingleSDR (WORD, WORD *);

#else

extern WORD 		UtilSDRCmdsInit(void);
extern WORD 		UtilRetrieveSingleSDR (WORD, WORD *);

#endif 
#endif 


