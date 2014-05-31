   
#ifndef _SLAVETASK_H_
#define _SLAVETASK_H_  


#define ALL_SLAVE_EVENT            0xFFFFFFFF
#define TOTAL_SLAVE_HANDLE_FUNC    32

typedef void (* SlaveEventHandleFunctionType)(DWORD, void *);

typedef struct
{
    void                           *Ptr; 
    SlaveEventHandleFunctionType   HandleFunc;
    
} St_SlaveEventHandleTag;


#ifdef _SLAVETASK_C_
STATUS SlaveRegisterEvent(void *p_HandleFuncPtr, void *p_dw_EventID);
STATUS SlaveCancelEvent(DWORD p_dw_EventID);
void SlaveTaskInit(void);
void SlaveTask(DWORD dw_input);
STATUS SlaveTaskEventSet(DWORD dw_EventID, void *);
#else
extern STATUS SlaveRegisterEvent(void *p_HandleFuncPtr, void *p_dw_EventID);
extern STATUS SlaveCancelEvent(DWORD p_dw_EventID);
extern void SlaveTaskInit(void);
extern void SlaveTask(DWORD dw_input);
extern STATUS SlaveTaskEventSet(DWORD dw_EventID, void *);

#endif  


#endif  





