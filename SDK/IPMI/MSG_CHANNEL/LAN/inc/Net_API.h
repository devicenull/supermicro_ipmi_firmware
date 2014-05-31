#ifndef _NET_API_H_
#define _NET_API_H_

#include "libnet.h"

#ifdef OS_LINUX 
#define OS_TCPSocketCreate  OS_SocketCreate
#define OS_UDPSocketCreate  OS_SocketCreate
#define OS_TCPSocketBind 	OS_SocketBind
#define OS_UDPSocketBind 	OS_SocketBind
#define OS_TCPSocketRecv 	OS_SocketRecv
#define OS_UDPSocketRecv 	OS_SocketRecv
#define OS_TCPSocketSend 	OS_SocketSend
#define OS_UDPSocketSend 	OS_SocketSend
#endif



#ifdef _NET_API_C_

void OS_CHANGE_ULONG_ENDIAN(void *);
void OS_CHANGE_USHORT_ENDIAN(void *);
void OS_IncreaseRMCPPKtsRecv(BYTE );

extern void CHANGE_ULONG_ENDIAN(void *);
extern void CHANGE_USHORT_ENDIAN(void *);

#else 

extern STATUS TCPSocketCreate(Socket* );
extern STATUS UDPSocketCreate(Socket* );
extern STATUS UDPSocketBind(Socket ,DWORD );
extern STATUS TCPSocketBind(Socket ,DWORD );
extern STATUS UDPSocketRecv(Socket ,void *,DWORD* ,DWORD* ,WORD* );
extern STATUS TCPSocketRecv(Socket ,void *,DWORD* ,DWORD* ,WORD* );
extern STATUS UDPSocketSend(Socket ,void *,DWORD ,DWORD ,WORD );
extern STATUS TCPSocketSend(Socket ,void *,DWORD ,DWORD ,WORD );
extern WORD   BYTE2WORD(BYTE ,BYTE );
extern DWORD  BYTE2DWORD(BYTE ,BYTE ,BYTE ,BYTE );
extern void   OS_CHANGE_USHORT_ENDIAN(void * );
extern void   OS_CHANGE_ULONG_ENDIAN(void *);
extern void IncreaseRMCPPKtsRecv(BYTE );
#endif

#endif

 
