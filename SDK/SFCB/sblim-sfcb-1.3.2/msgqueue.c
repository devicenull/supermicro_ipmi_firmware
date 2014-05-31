
/*
 * msgqueue.c
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



#include <string.h>
#include <errno.h>
#include "cmpidt.h"
#include "providerMgr.h"
#include "msgqueue.h"
#include "trace.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


extern unsigned long exFlags;

ComSockets resultSockets;
ComSockets sfcbSockets;
ComSockets providerSockets;
int localMode=1;

ComSockets *sPairs;
int ptBase,htBase,stBase,htMax,stMax;
int httpProcId;
int currentProc=0;
int noProvPause=0;
char *provPauseStr=NULL;
int noHttpPause=0;
char *httpPauseStr=NULL;

key_t sfcbSemKey;
int sfcbSem = -1;


/*
 *              semaphore services
 */

int semAcquireUnDo(int semid, int semnum)
{
   int rc;
   
   struct sembuf semBuf = {semnum,-1,SEM_UNDO};
   rc=semop(semid,&semBuf,1);
   return rc;
}

int semAcquire(int semid, int semnum)
{
   int rc;
   
   struct sembuf semBuf = {semnum,-1,0};
   rc=semop(semid,&semBuf,1);
   return rc;
}

int semRelease(int semid, int semnum)
{
   int rc;
   struct sembuf semBuf = {semnum,1,0};
  
   rc=semop(semid,&semBuf,1);
   return rc;
}

int semReleaseUnDo(int semid, int semnum)
{
   int rc;
   struct sembuf semBuf = {semnum,1,SEM_UNDO};
  
   rc=semop(semid,&semBuf,1);
   return rc;
}

int semMultiRelease(int semid, int semnum, int n)
{
    struct sembuf semBuf = {semnum,1,0};
  
    return semop(semid,&semBuf,n);
}

int semGetValue(int semid, int semnum)
{
   union semun sun;
   return semctl(semid,semnum,GETVAL,sun);
}

int semSetValue(int semid, int semnum, int value)
{
   union semun sun;
   sun.val=value;
   return semctl(semid,semnum,SETVAL,sun);
}

int initSem(int https, int shttps, int provs)
{
   union semun sun;
   int i;
   
   sfcbSemKey=ftok(SFCB_BINARY,'S');

   if ((sfcbSem=semget(sfcbSemKey,1, 0600))!=-1) 
      semctl(sfcbSem,0,IPC_RMID,sun);
   
   if ((sfcbSem=semget(sfcbSemKey,4+(provs*3)+3,IPC_CREAT | IPC_EXCL | 0600))==-1) {
      char *emsg=strerror(errno);
      mlogf(M_ERROR,M_SHOW,"\n--- SFCB semaphore create key: 0x%x failed: %s\n",sfcbSemKey,emsg);
      mlogf(M_ERROR,M_SHOW,"     use \"ipcrm -S 0x%x\" to remove semaphore\n\n",sfcbSemKey);
      abort();
   }

   sun.val=1;
   semctl(sfcbSem,httpGuardId,SETVAL,sun);
   sun.val=https;
   semctl(sfcbSem,httpProcsId,SETVAL,sun);
   sun.val=1;
   semctl(sfcbSem,shttpGuardId,SETVAL,sun);
   sun.val=shttps;
   semctl(sfcbSem,shttpProcsId,SETVAL,sun);
   
   for (i=0; i<provs; i++) {
      sun.val=1;
      semctl(sfcbSem,(i*3)+provProcGuardId+provProcBaseId,SETVAL,sun);
      sun.val=0;
      semctl(sfcbSem,(i*3)+provProcInuseId+provProcBaseId,SETVAL,sun);
      semctl(sfcbSem,(i*3)+provProcAliveId+provProcBaseId,SETVAL,sun);
   }      
   return 0;   
}

int remSem()
{
      return semctl(sfcbSem,0,IPC_RMID,0);
}

MsgSegment setCharsMsgSegment(const char *str)
{
   MsgSegment s;
   s.data = (char*)str;
   if (str)
      s.length = strlen(str) + 1;
   else
      s.length = 0;
   s.type = MSG_SEG_CHARS;
   /*   if (s.length) s.length=((((s.length-1)/4)+1)*4); */
   return s;
}

static int spHandleError(int *s, char *m)
{
   _SFCB_ENTER(TRACE_MSGQUEUE, "handleError");
   char *emsg=strerror(errno);
   mlogf(M_ERROR,M_SHOW,"%s %d %d-%d %s\n", m, *s, currentProc, errno,emsg);
   //   _SFCB_ABORT();
   return -1;
}


/*
 *              spGetMsg
 */

static int spGetMsg(int *s, int *from, void *data, unsigned length, MqgStat* mqg)
{
   static char *em = "spGetMsg receiving from";
   ssize_t n,ol,r=0;
   int fromfd=-1;
   char ccmsg[CMSG_SPACE(sizeof(fromfd))];
   struct cmsghdr *cmsg;
   struct iovec iov;
   struct msghdr msg;

   _SFCB_ENTER(TRACE_MSGQUEUE, "spGetMsg");
   _SFCB_TRACE(1, ("--- Receiving from %d length %d", *s, length));

  msg.msg_name = 0;
  msg.msg_namelen = 0;
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  
  msg.msg_control = ccmsg;
  msg.msg_controllen = sizeof(ccmsg); 
  
   ol = length;
   for (;;) {
      if (mqg) mqg->teintr=0;
      
      iov.iov_base = data+r;
      iov.iov_len = length-r;

      if ((n = recvmsg(*s, &msg, 0)) < 0) {
         if (errno == EINTR) {
            _SFCB_TRACE(1, (" Receive interrupted %d",currentProc));
            if (mqg) {
               mqg->teintr=1;
               return 0;
            }
            continue;
         }
         return spHandleError(s, em);
      }

      if (n == 0) {
         char *emsg=strerror(errno);
         mlogf(M_ERROR,M_SHOW,"--- Warning: fd is closed: %s\n",emsg);
	 //  break;
	 return -1;
      }

      if (r==0) {  /* true for the first time through the loop */
         cmsg = CMSG_FIRSTHDR(&msg);
         if (cmsg) {
            if (!cmsg->cmsg_type == SCM_RIGHTS) {
               mlogf(M_ERROR,M_SHOW,"--- got control message of unknown type %d\n", cmsg->cmsg_type);
               return -1;
            }   
            fromfd = *(int*)CMSG_DATA(cmsg);
         }  
         msg.msg_control = 0;
         msg.msg_controllen = 0; 
      }
      
      r+=n;      
      if (r < ol) continue; /* continue as long as bytes read is < length */
      break;
   }
   
   if (from) *from = fromfd;
   
   return 0;
}


/*
 *              spRcvMsg
 */

static int spRcvMsg(int *s, int *from, void **data, unsigned long *length, MqgStat* mqg)
{
   SpMessageHdr spMsg;
   static char *em = "rcvMsg receiving from";
   MqgStat imqg;
   int fromfd;

   _SFCB_ENTER(TRACE_MSGQUEUE, "spRcvMsg");
   _SFCB_TRACE(1, ("--- Receiving from %d", *s));

   if (mqg==NULL) mqg=&imqg;
   do {
     if ((spGetMsg(s, &fromfd, &spMsg, sizeof(spMsg), mqg)) == -1)
       return spHandleError(s, em);
     
     if (mqg && mqg->teintr) {
       mqg->eintr=1;    
       mqg->rdone=0;
     }   
     if (mqg->teintr) mqg->eintr=1;    
   } while (mqg->teintr) ;       

   
   if (fromfd>0) spMsg.returnS=fromfd;   
   *from=spMsg.returnS;   

   _SFCB_TRACE(1, ("--- Received info segment %d bytes", sizeof(spMsg)));

   *length = spMsg.totalSize;

   mqg->rdone=1;
   mqg->eintr=0;
   
   if (*length) {
      *data = malloc(spMsg.totalSize + 8);
      do {
         if ((spGetMsg(s, NULL, *data, *length, mqg)) == -1)
            return spHandleError(s, em);
         if (mqg->teintr) mqg->eintr=1;    
      } while (mqg->teintr) ;       
      _SFCB_TRACE(1, ("--- Received data segment %d bytes", *length));
   }

   if (spMsg.type == MSG_DATA) {
      _SFCB_TRACE(1, ("--- Received %d bytes", *length));
      _SFCB_RETURN(0);
   }

   if (spMsg.xtra == MSG_X_EXTENDED_CTL_MSG) {
      *data = malloc(256);
      *length = 256;
      do {
         if ((spGetMsg(s, NULL, *data, *length, mqg)) == -1)
            return spHandleError(s, em);
         if (mqg->teintr) mqg->eintr=1;    
      } while (mqg->teintr) ;       
   }

   switch (spMsg.xtra) {
   case MSG_X_PROVIDER:
      *length = spMsg.segments;
      *data = spMsg.provId;
   case MSG_X_INVALID_NAMESPACE:
   case MSG_X_PROVIDER_NOT_FOUND:
   case MSG_X_INVALID_CLASS:
      _SFCB_RETURN(spMsg.xtra);
   case MSG_X_LOCAL:
      *length = 0;
      *data = NULL;
      _SFCB_RETURN(spMsg.xtra);
   default:
      *data = NULL;
      mlogf(M_ERROR,M_SHOW,"### %d ??? %ld-%d\n", currentProc, spMsg.type,
             spMsg.xtra);
      abort();
   }
   
   _SFCB_RETURN(spMsg.xtra);
}

int spRecvReq(int *s, int *from, void **data, unsigned long *length, MqgStat *mqg)
{
   int rc;
   _SFCB_ENTER(TRACE_MSGQUEUE, "spRecvReq");
   rc = spRcvMsg(s, from, data, length, mqg);
   _SFCB_RETURN(rc);
}

int spRecvResult(int *s, int *from, void **data, unsigned long *length)
{
   int rc;
   _SFCB_ENTER(TRACE_MSGQUEUE, "spRecvResult");
   rc = spRcvMsg(s, from, data, length, NULL);
   _SFCB_RETURN(rc);
}

int spRecvCtlResult(int *s, int *from, void **data, unsigned long *length)
{
   int rc;
   _SFCB_ENTER(TRACE_MSGQUEUE, "spRecvCtlResult");
   rc = spRcvMsg(s, from, data, length, NULL);

   _SFCB_RETURN(rc);
}

/*
 *              spSendMsg
 */

static int spSendMsg(int *to, int *from, int n, struct iovec *iov, int size)
{
   SpMessageHdr spMsg = { 0, 0, abs(*from), size };
   spMsg.type = MSG_DATA;
   static char *em = "spSendMsg sending to";
   struct msghdr msg;
   char ccmsg[CMSG_SPACE(sizeof(*from))];
   struct cmsghdr *cmsg;
   ssize_t rc;

   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendMsg");
   _SFCB_TRACE(1, ("--- Sending %d bytes to %d", size, *to));

   spMsg.returnS=abs(*from);
   
   if (*from>0) {
      msg.msg_control = ccmsg;
      msg.msg_controllen = sizeof(ccmsg);   
   
      cmsg = CMSG_FIRSTHDR(&msg);
      cmsg->cmsg_level = SOL_SOCKET;
      cmsg->cmsg_type = SCM_RIGHTS;
      cmsg->cmsg_len = CMSG_LEN(sizeof(*from));
      *(int*)CMSG_DATA(cmsg) = *from;
   }
   else { 
      msg.msg_control = 0;
      msg.msg_controllen = 0;
   }
   
   msg.msg_name = NULL;
   msg.msg_namelen = 0;
   msg.msg_flags = 0;
   msg.msg_iov = iov;
   msg.msg_iovlen = n;   

   iov[0].iov_base = &spMsg;
   iov[0].iov_len = sizeof(spMsg);

   if ((rc=sendmsg(*to, &msg, 0)) < 0) {
      return spHandleError(to, em);
   }
   
   _SFCB_TRACE(1, ("--- Sent %d bytes to %d", rc, *to));

   _SFCB_RETURN(0);
}


int spSendReq(int *to, int *from, void *data, unsigned long size, int internal)
{
   int rc,n,f=*from;
   struct iovec iov[2];
   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendReq");
   
   if (size) {
      n=2;
      iov[1].iov_base = data;
      iov[1].iov_len = size;
   }
   else n=1;
   
   if (internal) f=-(*from);
   rc = spSendMsg(to, &f, n, iov, size);
   _SFCB_RETURN(rc);
}

int spSendResult(int *to, int *from, void *data, unsigned long size)
{
   int rc,n;
   struct iovec iov[2];
   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendResult");
   
   if (size) {
      n=2;
      iov[1].iov_base = data;
      iov[1].iov_len = size;
   }
   else n=1;
   
   rc = spSendMsg(to, from, n, iov, size);
   _SFCB_RETURN(rc);
}

int spSendResult2(int *to, int *from, 
   void *d1, unsigned long s1, void *d2, unsigned long s2)
{
   int rc,n;
   struct iovec iov[4];
   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendResult2");
   
   iov[1].iov_base = d1;
   iov[1].iov_len = s1;
   
   if (s2) {
      n=3;
      iov[2].iov_base = d2;
      iov[2].iov_len = s2;
   }
   else n=2;
   
   rc = spSendMsg(to, from, n, iov, s1+s2);
   _SFCB_RETURN(rc);
}

int spSendAck(int to)
{
   int rc;
   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendAck");
   rc = write(to, "ack",4);
   _SFCB_RETURN(rc);
}

int spRcvAck(int from)
{
   int rc;
   char ack[8];
   _SFCB_ENTER(TRACE_MSGQUEUE, "spRcvAck");
   rc = read(from, ack,4);
   _SFCB_RETURN(rc);
}

/*
 *              sendCtl
 */

static int spSendCtl(int *to, int *from, short code, unsigned long count,
                     void *data)
{
   SpMessageHdr spMsg = { 0, 0, abs(*from), 0 };
   static char *em = "spSendCtl sending to";
   struct msghdr msg;
   struct iovec iov[2];   
   char ccmsg[CMSG_SPACE(sizeof(*from))];
   struct cmsghdr *cmsg;
   
   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendCtl");
   _SFCB_TRACE(1, ("--- Sending %d bytes to %d", sizeof(SpMessageHdr), *to));
        
   if (*from>0) {
      msg.msg_control = ccmsg;
      msg.msg_controllen = sizeof(ccmsg);
   
      cmsg = CMSG_FIRSTHDR(&msg);
      cmsg->cmsg_level = SOL_SOCKET;
      cmsg->cmsg_type = SCM_RIGHTS;
      cmsg->cmsg_len = CMSG_LEN(sizeof(*from));
      *(int*)CMSG_DATA(cmsg) = *from;
   }   
   else {
      msg.msg_control = 0;
      msg.msg_controllen = 0;
   }
   
   msg.msg_name = NULL;
   msg.msg_namelen = 0;
   msg.msg_flags = 0;
   msg.msg_iov = iov;
   msg.msg_iovlen = 1;

   spMsg.type = MSG_CTL;
   spMsg.xtra = code;
   spMsg.segments = count;
   spMsg.provId = data;

   iov[0].iov_base = &spMsg;
   iov[0].iov_len = sizeof(spMsg);

   if (sendmsg(*to, &msg, 0) < 0)
      return spHandleError(to, em);
      
   _SFCB_RETURN(0);
}

int spSendCtlResult(int *to, int *from, short code, unsigned long count,
                    void *data, int options)
{
   int rc;
   int f=*from;
   
   _SFCB_ENTER(TRACE_MSGQUEUE, "spSendCtlResult");

   if (options & OH_Internal) f=-(*from);
   rc = spSendCtl(to, &f, code, count, data);
   _SFCB_RETURN(rc);
}


void initSocketPairs(int provs, int https, int shttps)
{
   int i,t=(provs*2); //+https+shttps;
   
   sPairs=(ComSockets*)malloc(sizeof(ComSockets)*t);
   mlogf(M_INFO,M_SHOW,"--- initSocketPairs: %d\n",t);
   for (i=0; i<t; i++) {
      socketpair(PF_LOCAL, SOCK_STREAM, 0, &sPairs[i].receive);
   }   
   ptBase=provs;
   htBase=ptBase+provs;
   stBase=htBase+https;
   htMax=https;
   stMax=shttps;
}

void uninitSocketPairs()
{
   free(sPairs);
}


unsigned long getInode(int fd)
{
   struct stat st;
   fstat(fd,&st);
   return st.st_ino;
}

ComSockets getSocketPair(char *by)
{
   ComSockets sp;
   _SFCB_ENTER(TRACE_MSGQUEUE | TRACE_SOCKETS, "getSocketPair");
 
   socketpair(PF_LOCAL, SOCK_STREAM, 0, &sp.receive);
   _SFCB_TRACE(1,("--- %s rcv: %d - %d %d",by,sp.receive,getInode(sp.receive),currentProc));       
   _SFCB_TRACE(1,("--- %s snd: %d - %d %d",by,sp.send,getInode(sp.send),currentProc));       
   
   _SFCB_RETURN(sp);
}

void closeSocket(ComSockets *sp, ComCloseOpt o,char *by)
{
   _SFCB_ENTER(TRACE_MSGQUEUE | TRACE_SOCKETS, "closeSocket");
   
   if ((o==cRcv || o==cAll) && sp->receive!=0) {
     _SFCB_TRACE(1,("--- %s closing: %d - %d %d\n",by,sp->receive,getInode(sp->receive),currentProc));       
      close (sp->receive); 
      sp->receive=0;
   }   
   if ((o==cSnd || o==cAll) && sp->send!=0) {
     _SFCB_TRACE(1,("--- %s closing: %d - %d %d\n",by,sp->send,getInode(sp->send),currentProc));       
      close (sp->send); 
      sp->send=0;
   }   
   _SFCB_EXIT();
}

#define LOCAL_SFCB 

#ifdef LOCAL_SFCB

int getControlChars(char *id, char **val);

void stopLocalConnectServer()
{
   static struct sockaddr_un serverAddr;
   int sock,size=0;
   unsigned long int l;
   char *path;
   
   if (getControlChars("localSocketPath", &path)!=0) {
      mlogf(M_INFO,M_SHOW,"--- localConnectServer failed to start\n");
   }
      
   if ((sock=socket(PF_UNIX, SOCK_STREAM, 0))<0) {
      perror("socket creation error");
      return;
   }
   
   serverAddr.sun_family=AF_UNIX;
   strcpy(serverAddr.sun_path,path);
   
   if (connect(sock,(const struct sockaddr*)&serverAddr,
      sizeof(serverAddr.sun_family)+strlen(serverAddr.sun_path))<0) {
      perror("connect error");
      return;
   }
   
   l=write(sock, &size, sizeof(size)); 
   close(sock);
}

void localConnectServer()
{
   static struct sockaddr_un clientAddr,serverAddr;
   int nsocket,ssocket;
   unsigned int cl, notDone=1;
   char *path,cMsg[264];
   
   struct _msg {
      unsigned int size;
      char oper;
      pid_t pid;
      char id[64];
   } msg;
   
   mlogf(M_INFO,M_SHOW,"--- localConnectServer started\n");
   
   if (getControlChars("localSocketPath", &path)!=0) {
      mlogf(M_INFO,M_SHOW,"--- localConnectServer failed to start\n");
   }
      
   if ((ssocket=socket(PF_UNIX, SOCK_STREAM, 0))<0) {
      perror("socket creation error");
      return;
   }
   
   serverAddr.sun_family=AF_UNIX;
   strcpy(serverAddr.sun_path,path);
   unlink(path);
   
   if (bind(ssocket,(const struct sockaddr*)&serverAddr,
      sizeof(serverAddr.sun_family)+strlen(serverAddr.sun_path))<0) {
      perror("bind error");
      return;
   }
   
   listen(ssocket,1);
   
   do {
     // sfcbSockets.send;
      cl=sizeof(clientAddr);
      if ((nsocket=accept(ssocket,(struct sockaddr*)&serverAddr,&cl))<0) {
         perror("accept error");

         /* Being interrupted isn't necessarily bad; try once more */
         if (errno == EINTR) {
           if ((nsocket=accept(ssocket,(struct sockaddr*)&serverAddr,&cl))<0) {
             perror("accept error (2)");
             return;
           }
         }
         /* any other error, just return (should probably be more graceful) */
         else {
           return;
         }
      }
      
      read(nsocket, &msg.size, sizeof(msg.size));
      read(nsocket, &msg.oper, msg.size);
      
      if (msg.size!=0) {
         sprintf(cMsg,"--- Local Client connect - pid: %d user: %s\n",msg.pid,msg.id);
         mlogf(M_INFO,M_SHOW,cMsg);
         spSendCtlResult(&nsocket, &sfcbSockets.send, MSG_X_LOCAL, 0, 0, 0);
      }
      else notDone=0;   
      
      close (nsocket);
   } while (notDone);

   mlogf(M_INFO,M_SHOW,"--- localConnectServer ended\n");
}

#endif
