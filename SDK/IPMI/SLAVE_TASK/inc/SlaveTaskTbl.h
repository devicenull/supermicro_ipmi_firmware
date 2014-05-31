

#ifndef _SLAVETASKTBL_H_
#define _SLAVETASKTBL_H_
#include "SlaveTaskOEMTbl.h"

#define SLVTASK_SYSYTEMREBOOT   		 1
#define SLVTASK_LANCONFIGAPPLY   		 2
#define SLVTASK_UTILSENDARP      		 3
#define SLVTASK_FLASHUPDATE     		 4
#define SLVTASK_UTILIMMEDPETEVENT 		 5
#define SLVTASK_UTILFWUPDATE 			 6
#define SLVTASK_APIWATCHDOGTIMEOUTACTION 7
#define SLVTASK_DUMPENTIREFLASH 		 8
#define SLVTASK_CHANGERMCPPORT			 9
#define SLVTASK_RBPREPARETASK           10
#define SLVTASK_RBUPDATETASK			11

typedef union {
	DWORD   u32_Info;
    DWORD   dw_IPinfo;
    WORD    w_lanInfo;
    BYTE    b_DevID;
    BYTE    b_flag;
} Un_ParaTag;

typedef struct {
    DWORD dw_FuncId;
    void (*TaskFunc)(DWORD dw_FuncID, Un_ParaTag *un_Param);
} St_SlaveTaskFuncTag;

#ifndef _SLAVETASKTBL_C_
extern int at_dw_SlaveTaskFuncTableSize;
extern int at_dw_SlaveTaskFuncOEMTableSize;
extern St_SlaveTaskFuncTag at_a_St_SlaveTaskFuncTable[];
extern St_SlaveTaskFuncTag at_a_St_SlaveTaskFuncOEMTable[];
#endif

#endif 


