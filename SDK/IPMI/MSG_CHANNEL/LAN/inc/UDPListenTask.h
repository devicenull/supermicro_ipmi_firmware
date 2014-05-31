#ifndef _UDPLISTENTASK_H_
#define _UDPLISTENTASK_H_

#define ASF_MSG     0x06
#define IPMI_MSG    0x07

#ifdef _UDPLISTENTASK_C_
void UDPListenTask(DWORD );


#else
extern void UDPListenTask(DWORD );

#endif 

#endif 




