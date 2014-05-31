      
#ifndef _MSGLISTENER_H_
#define _MSGLISTENER_H_


#ifdef _MSGLISTENER_C_
void MsgListenerTask(unsigned long int i);
#else
extern TX_QUEUE ws_MsgListenerQueue;

extern void MsgListenerInit(void);
#endif   




#endif  





