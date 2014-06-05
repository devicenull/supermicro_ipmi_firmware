
typedef  int  Socket ;

#ifdef _LIBNET_C_

STATUS OS_SocketSend(Socket ,void *,unsigned long ,unsigned long ,unsigned short );
STATUS OS_SocketRecv(Socket ,void *,unsigned long* ,unsigned long* ,unsigned short* );
STATUS OS_SocketBind(Socket ,unsigned long );
STATUS OS_SocketCreate(Socket * );
int tcp_server_create(char *, char *,int );

#else
extern int OS_SocketSend(Socket ,void *,unsigned long ,unsigned long ,unsigned short );
extern STATUS OS_SocketRecv(Socket ,void *,unsigned long* ,unsigned long* ,unsigned short* );
extern STATUS OS_SocketBind(Socket ,unsigned long );
extern STATUS OS_SocketCreate(Socket * );
extern int tcp_server_create(char *, char *,int );
#endif
