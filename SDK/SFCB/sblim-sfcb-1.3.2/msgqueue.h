
/*
 * msgqueue.h
 *
 * (C) Copyright IBM Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:     Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * Intra process communication support for sfcb.
 *
*/


#ifndef msgqueue_h
#define msgqueue_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define MSG_DATA 1
#define MSG_CTL 2

#define MSG_X_HTTP_STOPPING      1
#define MSG_X_NOT_SUPPORTED      2
#define MSG_X_PROVIDER           3
#define MSG_X_INVALID_CLASS      4
#define MSG_X_INVALID_NAMESPACE  5
#define MSG_X_PROVIDER_NOT_FOUND 6
#define MSG_X_EXTENDED_CTL_MSG   7
#define MSG_X_FAILED             8
#define MSG_X_LOCAL              9

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
#else
union semun {
   int val;
   struct semid_ds *buf;
   unsigned short int *array;
   struct seminfo *__buf;
};
#endif

typedef struct _spMessageHdr {
   short type;
   short xtra;
   int returnS;
   unsigned long totalSize;
   unsigned long segments;
   void *provId;
} SpMessageHdr;

typedef struct _msgSegment {
   void *data;
   unsigned type;
   unsigned length;
} MsgSegment;

typedef struct msgXctl {
   unsigned length;
   unsigned code;
   char data[1];
} MsgXctl;

#define MSG_SEG_CHARS 1
#define MSG_SEG_OBJECTPATH 2
#define MSG_SEG_INSTANCE 3
#define MSG_SEG_CONSTCLASS 4
#define MSG_SEG_ARGS 5
#define MSG_SEG_QUALIFIER 6

typedef struct msgList {
   long count;
   MsgSegment list[1];
} MsgList;

typedef struct sockRequest {
   int req;
   char msg[60];
} SockRequest;

#define COM_RECV 0
#define COM_SEND 1
#define COM_ALL -1

typedef enum comCloseOpt {
   cRcv = 0,
   cSnd = 1,
   cAll = -1
} ComCloseOpt;   
    

#define cRcvSocket(sp) (sp.receive) 
#define cSndSocket(sp) (sp.send) 

typedef struct comSockets {
   int receive;
   int send;
} ComSockets;

typedef struct mqgStat {
  char teintr,eintr,rdone;
} MqgStat;

extern MsgSegment setCharsMsgSegment(const char *);

extern ComSockets sfcbSockets; 
extern ComSockets providerSockets;
extern ComSockets resultSockets;
extern int localMode;

extern ComSockets getSocketPair(char* by);
extern void closeSocket(ComSockets *sp, ComCloseOpt o, char* by);

extern int spRecvCtlResult(int *s, int *from, void **data,
                           unsigned long *length);
extern int spSendCtlResult(int *to, int *from, short code, unsigned long count,
                           void *data, int options);
extern int spSendReq(int *to, int *from, void *data, unsigned long size, int internal);
extern int spRecvResult(int *q, int *from, void **data, unsigned long *length);
extern int spRecvReq(int *q, int *from, void **data, unsigned long *length, MqgStat *mqg);
extern int spSendResult(int *to, int *from, void *data, unsigned long size);
extern unsigned long getInode(int fd);

extern void initSocketPairs(int provs, int https, int shttps);

extern int semAcquireUnDo(int semid, int semnum);
extern int semAcquire(int semid, int semnum);
extern int semRelease(int semid, int semnum);
extern int semReleaseUnDo(int semid, int semnum);
extern int semMultiRelease(int semid, int semnum, int n);
extern int semGetValue(int semid, int semnum);
extern int semSetValue(int semid, int semnum, int value);
extern int initSem(int https, int shttps, int provs);

extern int provProcSem;
extern int provWorkSem;
extern key_t sfcbSemKey;
extern int sfcbSem;
extern int currentProc;
extern int noProvPause;
extern char *provPauseStr;
extern int noHttpPause;
extern char *httpPauseStr;

#define httpGuardId 0
#define httpProcsId 1
#define shttpGuardId 2
#define shttpProcsId 3
#define provProcBaseId 4
#define provProcGuardId 0
#define provProcInuseId 1
#define provProcAliveId 2

extern ComSockets *sPairs;
extern int ptBase,htBase,stBase,htMax,stMax;
extern int httpProcId;

extern void stopLocalConnectServer();
extern void localConnectServer();

#endif
