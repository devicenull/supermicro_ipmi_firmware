
/*
 * httpAdapter.c
 *
 * © Copyright IBM Corp. 2005, 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * httpAdapter implementation.
 *
*/
 

#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <dlfcn.h>

#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>

#include "cmpidt.h"
#include "msgqueue.h"
#include "utilft.h"
#include "trace.h"
#include "cimXmlRequest.h"
#include "support.h"

#include <pthread.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "httpComm.h"
#include "sfcVersion.h"
#include "control.h"

unsigned long exFlags = 0;
static char *name;
static int debug;
static int doBa;
static int doUdsAuth;
static int doFork = 0;
int noChunking = 0;
int sfcbSSLMode = 0;
int httpLocalOnly = 0;  /* 1 = only listen on loopback interface */
static int hBase;
static int hMax;
static int httpProcIdX;
static int stopAccepting=0;
static int running=0;
static long keepaliveTimeout=15;
static long keepaliveMaxRequest=10;
static long numRequest;

#if defined USE_SSL
static SSL_CTX *ctx;
static X509    *x509 = NULL;
#define CC_VERIFY_IGNORE  0
#define CC_VERIFY_ACCEPT  1
#define CC_VERIFY_REQUIRE 2
int ccVerifyMode = CC_VERIFY_IGNORE;
static int get_cert(int,X509_STORE_CTX*);
static int ccValidate(X509*, char**, int);
#endif

static key_t httpProcSemKey;
static key_t httpWorkSemKey;
static int httpProcSem;
static int httpWorkSem;

extern char *decode64(char *data);
extern void libraryName(const char* dir, const char *location, char *fullName);
extern void *loadLibib(const char *libname);
extern int getControlChars(char *id, char **val);
extern void *loadLibib(const char *libname);

extern RespSegments genFirstChunkResponses(BinRequestContext*,BinResponseHdr**,int,int);
extern RespSegments genLastChunkResponses(BinRequestContext*, BinResponseHdr**, int);
extern RespSegments genChunkResponses(BinRequestContext*, BinResponseHdr**, int);
extern RespSegments genFirstChunkErrorResponse(BinRequestContext * binCtx, int rc, char *msg);
extern char *getErrTrailer(int id, int rc, char *m);
extern void dumpTiming(int pid);
extern char * configfile;
extern int inet_aton(const char *cp, struct in_addr *inp);

int sfcBrokerPid=0;
static unsigned int sessionId;
extern char *opsName[];

typedef int (*Authenticate)(char* principal, char* pwd);

typedef struct _buffer {
   char *data, *content;
   int length, size, ptr, content_length,trailers;
   char *httpHdr, *authorization, *content_type, *host, *useragent;
   char *principal;
   char *protocol;
#if defined USE_SSL
   X509 *certificate;
#endif
} Buffer;

#ifdef HAVE_IPV6
 #define USE_INET6
#endif

//#define USE_THREADS
//for use by the process thread in handleHttpRequest()
struct processThreadParams {
   int breakloop;
   CommHndl conn_fd;
   int connFd;
   fd_set httpfds;
   struct timeval httpTimeout;
   int isReady;
};

void initHttpProcCtl(int p, int sslmode)
{
   httpProcSemKey=ftok(SFCB_BINARY,'H' + sslmode);
   httpWorkSemKey=ftok(SFCB_BINARY,'W' + sslmode);
   union semun sun;
   int i;

   mlogf(M_INFO,M_SHOW,"--- Max Http%s procs: %d\n",sslmode?"s":"",p);
   if ((httpProcSem=semget(httpProcSemKey,1,0600))!=-1) 
      semctl(httpProcSem,0,IPC_RMID,sun);
      
   if ((httpProcSem=semget(httpProcSemKey,1+p,IPC_CREAT | IPC_EXCL | 0600))==-1) {
      char *emsg=strerror(errno);
      mlogf(M_ERROR,M_SHOW,"\n--- Http Proc semaphore create key: 0x%x failed: %s\n",httpProcSemKey,emsg);
      mlogf(M_ERROR,M_SHOW,"     use \"ipcrm -S 0x%x\" to remove semaphore\n\n",httpProcSemKey);
      abort();
   }
   sun.val=p;
   semctl(httpProcSem,0,SETVAL,sun);
   
   sun.val=0;
   for (i=1; i<=p; i++)
      semctl(httpProcSem,p,SETVAL,sun);

   if ((httpWorkSem=semget(httpWorkSemKey,1,0600))!=-1) 
      semctl(httpWorkSem,0,IPC_RMID,sun);
      
   if ((httpWorkSem=semget(httpWorkSemKey,1,IPC_CREAT | IPC_EXCL | 0600))==-1) {
      char *emsg=strerror(errno);
      mlogf(M_ERROR,M_SHOW,"\n--- Http ProcWork semaphore create key: 0x%x failed: %s\n",httpWorkSemKey,emsg);
      mlogf(M_ERROR,M_SHOW,"     use \"ipcrm -S 0x%x\" to remove semaphore\n\n",httpProcSemKey);
      abort();
   }
   sun.val=1;
   semctl(httpWorkSem,0,SETVAL,sun);
}

int remProcCtl()
{
   semctl(httpProcSem,0,IPC_RMID,0);
   semctl(httpWorkSem,0,IPC_RMID,0);
   return 0;
}


int baValidate(char *cred, char **principal)
{
   char *auth,*pw=NULL;
   int i,err=0;
   static void *authLib=NULL;
   static Authenticate authenticate=NULL;
   char dlName[512];
   int ret = 0;

   if (strncasecmp(cred,"basic ",6)) return 0;
   auth=decode64(cred+6);
   for (i=0; i<strlen(auth); i++)
      if (auth[i]==':') {
          auth[i]=0;
          pw=&auth[i+1];
          break;
      }

   if (err==0 && authLib==NULL) {
      char *ln;
      err=1;
      if (getControlChars("basicAuthlib", &ln)==0) {
         libraryName(NULL,ln,dlName);
        if ((authLib=dlopen(dlName, RTLD_LAZY))) {
            authenticate= dlsym(authLib, "_sfcBasicAuthenticate");
            if (authenticate) err=0;
         }
      }
      if (err) mlogf(M_ERROR,M_SHOW,"--- Authentication exit %s not found\n",dlName);
   }

   if (err) {
     ret = 1;
   } 
   else {
     *principal=strdup(auth);
     if (authenticate(auth,pw)) 
       ret = 1;
   }

   free(auth);

   return ret;
}

static void handleSigChld(int sig)
{
   const int oerrno = errno;
   pid_t pid;
   int status;

   for (;;) {
      pid = wait4(0, &status, WNOHANG, NULL);
      if ((int) pid == 0)
         break;
      if ((int) pid < 0) {
         if (errno == EINTR || errno == EAGAIN) {
    //        fprintf(stderr, "pid: %d continue \n", pid);
            continue;
         }
         if (errno != ECHILD)
            perror("child wait");
         break;
      }
      else {
         running--;
         // fprintf(stderr, "%s: SIGCHLD signal %d - %s(%d)\n", name, pid,
         //        __FILE__, __LINE__);
      }
   }
   errno = oerrno;
}

static void handleSigUsr1(int sig)
{
   stopAccepting=1;
}

static void freeBuffer(Buffer * b)
{
   Buffer emptyBuf = { NULL, NULL, 0, 0, 0, 0, 0 ,0};
   if (b->data)
      free(b->data);
   if (b->content)
      free(b->content);
   *b=emptyBuf;   
}

static char *getNextHdr(Buffer * b)
{
   int i;
   char c;

   for (i = b->ptr; b->ptr < b->length; ++b->ptr) {
      c = b->data[b->ptr];
      if (c == '\n' || c == '\r') {
         b->data[b->ptr] = 0;
         ++b->ptr;
         if (c == '\r' && b->ptr < b->length && b->data[b->ptr] == '\n') {
            b->data[b->ptr] = 0;
            ++b->ptr;
         }
         return &(b->data[i]);
      }
   }
   return NULL;
}

static void add2buffer(Buffer * b, char *str, size_t len)
{
   if (b->size == 0) {
      b->size = len + 500;
      b->length = 0;
      b->data = (char *) malloc(b->size);
   }
   else if (b->length + len >= b->size) {
      b->size = b->length + len + 500;
      b->data = (char *) realloc((void *) b->data, b->size);
   }
   memmove(&((b->data)[b->length]), str, len);
   b->length += len;
   (b->data)[b->length] = 0;
}

static void genError(CommHndl conn_fd, Buffer * b, int status, char *title,
                     char *more)
{
   char head[1000];
   char server[] = "Server: sfcHttpd\r\n";
   char clength[] = "Content-Length: 0\r\n";
   char cclose[] = "Connection: close\r\n";
   char end[] = "\r\n";

   _SFCB_ENTER(TRACE_HTTPDAEMON, "genError");

   snprintf(head, sizeof(head), "%s %d %s\r\n", b->protocol, status, title);
//   fprintf(stderr,"+++ genError: %s\n",head);
   _SFCB_TRACE(1,("--- genError response: %s",head));
   commWrite(conn_fd, head, strlen(head));
   if (more) {
      commWrite(conn_fd, more, strlen(more));
   }

   commWrite(conn_fd, server, strlen(server));
   commWrite(conn_fd, clength, strlen(clength));
   if (keepaliveTimeout==0 || numRequest >= keepaliveMaxRequest ) {
     _SFCB_TRACE(1,("--- closing after error\n"));
     commWrite(conn_fd, cclose, strlen(cclose));
   }
   commWrite(conn_fd, end, strlen(end));
   commFlush(conn_fd);
}

static int readData(CommHndl conn_fd, char *into, int length)
{
   int c = 0, r;

   while (c < length) {
      r = commRead(conn_fd, into + c, length - c);
      if (r < 0 && (errno == EINTR || errno == EAGAIN)) {
         continue;
      }
      c += r;
   }
   return c;
}

static void getPayload(CommHndl conn_fd, Buffer * b)
{
   int c = b->length - b->ptr;
   b->content = (char *) malloc(b->content_length + 8);
   if (c) memcpy(b->content, (b->data) + b->ptr, c);

   if (c > b->content_length) {
     mlogf(M_INFO,M_SHOW,"--- HTTP Content-Length is lying; content truncated\n");
     c = b->content_length;
   }

   readData(conn_fd, (b->content) + c, b->content_length - c);
   *((b->content) + b->content_length) = 0;
}

void dumpResponse(RespSegments * rs)
{
   int i;
   if (rs) {
      for (i = 0; i < 7; i++) {
         if (rs->segments[i].txt) {
            if (rs->segments[i].mode == 2) {
               UtilStringBuffer *sb = (UtilStringBuffer *) rs->segments[i].txt;
               printf("%s", sb->ft->getCharPtr(sb));
            }
            else
               printf("%s", rs->segments[i].txt);
         }
      }
      printf("<\n");
   }
}

static void writeResponse(CommHndl conn_fd, RespSegments rs)
{

   static char head[] = {"HTTP/1.1 200 OK\r\n"};
   static char cont[] = {"Content-Type: application/xml; charset=\"utf-8\"\r\n"};
   static char cach[] = {"Cache-Control: no-cache\r\n"};
   static char op[]   = {"CIMOperation: MethodResponse\r\n"};
   static char cclose[] = "Connection: close\r\n";
   static char end[]  = {"\r\n"};
   char str[256];
   int len, i, ls[8];

   _SFCB_ENTER(TRACE_HTTPDAEMON, "writeResponse");
   
   for (len = 0, i = 0; i < 7; i++) {
      if (rs.segments[i].txt) {
         if (rs.segments[i].mode == 2) {
            UtilStringBuffer *sb = (UtilStringBuffer *) rs.segments[i].txt;
            if (sb == NULL)
               ls[i] = 0;
            else
               len += ls[i] = sb->ft->getSize(sb);
         }
         else
            len += ls[i] = strlen(rs.segments[i].txt);
      }
   }

   commWrite(conn_fd, head, strlen(head));
   commWrite(conn_fd, cont, strlen(cont));
   sprintf(str, "Content-Length: %d\r\n", len);
   commWrite(conn_fd, str, strlen(str));
   commWrite(conn_fd, cach, strlen(cach));
   commWrite(conn_fd, op, strlen(op));
   if (keepaliveTimeout == 0 || numRequest >= keepaliveMaxRequest) {
     commWrite(conn_fd, cclose, strlen(cclose));
   }
   commWrite(conn_fd, end, strlen(end));

   for (len = 0, i = 0; i < 7; i++) {
      if (rs.segments[i].txt) {
         if (rs.segments[i].mode == 2) {
            UtilStringBuffer *sb = (UtilStringBuffer *) rs.segments[i].txt;
            if (sb) {
               commWrite(conn_fd, (void*)sb->ft->getCharPtr(sb), ls[i]);
               sb->ft->release(sb);
            }
         }
         else {
            commWrite(conn_fd, rs.segments[i].txt, ls[i]);
            if (rs.segments[i].mode == 1)
               free(rs.segments[i].txt);
         }
      }
   }
   commFlush(conn_fd);
   
   _SFCB_EXIT();
}


static void writeChunkHeaders(BinRequestContext *ctx)
{
   static char head[] = {"HTTP/1.1 200 OK\r\n"};
   static char cont[] = {"Content-Type: application/xml; charset=\"utf-8\"\r\n"};
   static char cach[] = {"Cache-Control: no-cache\r\n"};
   static char op[]   = {"CIMOperation: MethodResponse\r\n"};
   static char tenc[] = {"Transfer-encoding: chunked\r\n"};
   static char trls[] = {"Trailer: CIMError, CIMStatusCode, CIMStatusCodeDescription\r\n"};
   static char cclose[] = "Connection: close\r\n";

   _SFCB_ENTER(TRACE_HTTPDAEMON, "writeChunkHeaders");
   
   commWrite(*(ctx->commHndl), head, strlen(head));
   commWrite(*(ctx->commHndl), cont, strlen(cont));
   commWrite(*(ctx->commHndl), cach, strlen(cach));
   commWrite(*(ctx->commHndl), op, strlen(op));
   commWrite(*(ctx->commHndl), tenc, strlen(tenc));
   commWrite(*(ctx->commHndl), trls, strlen(trls));
   if (keepaliveTimeout == 0 || numRequest >= keepaliveMaxRequest) {
     commWrite(*(ctx->commHndl), cclose, strlen(cclose));
   }
   commFlush(*(ctx->commHndl));
   
   _SFCB_EXIT();
}

static void writeChunkResponse(BinRequestContext *ctx, BinResponseHdr *rh)
{
   int i,len,ls[8];
   char str[256];
   RespSegments rs;
   _SFCB_ENTER(TRACE_HTTPDAEMON, "writeChunkResponse");
   switch (ctx->chunkedMode) {
   case 1:
      _SFCB_TRACE(1,("---  writeChunkResponse case 1"));
      if (rh->rc!=1) {
         RespSegments rs;
         rs=genFirstChunkErrorResponse(ctx, rh->rc-1, NULL);
         writeResponse(*ctx->commHndl, rs);
         _SFCB_EXIT();
     }  
      writeChunkHeaders(ctx);
  /*    if (rh->rc!=1) {
         _SFCB_TRACE(1,("---  writeChunkResponse case 1 error"));
         rh->moreChunks=0;
         break;
      } */     
      rs=genFirstChunkResponses(ctx, &rh, rh->count,rh->moreChunks);
      ctx->chunkedMode=2;
      break;
   case 2:
      _SFCB_TRACE(1,("---  writeChunkResponse case 2"));
      if (rh->rc!=1) {
         _SFCB_TRACE(1,("---  writeChunkResponse case 2 error"));
         rh->moreChunks=0;
         break;
      }   
      if (rh->moreChunks || ctx->pDone<ctx->pCount)
         rs=genChunkResponses(ctx, &rh, rh->count);
      else {
         rs=genLastChunkResponses(ctx, &rh, rh->count);
      }
      break;
   }

   if (rh->rc==1) {
   
      for (len = 0, i = 0; i < 7; i++) {
         if (rs.segments[i].txt) {
            if (rs.segments[i].mode == 2) {
               UtilStringBuffer *sb = (UtilStringBuffer *) rs.segments[i].txt;
               if (sb == NULL) ls[i] = 0;
               else len += ls[i] = sb->ft->getSize(sb);
            }
            else len += ls[i] = strlen(rs.segments[i].txt);
         }
      }
      /*  
       * make sure we do not have a 0 len , this would 
       * indicate the end of the chunk data. 
       */
      if(len != 0) {
        sprintf(str, "\r\n%x\r\n",len);     
        commWrite(*(ctx->commHndl), str, strlen(str));
        _SFCB_TRACE(1,("---  writeChunkResponse chunk amount %x ",len));
      }
      
      for (len = 0, i = 0; i < 7; i++) {
         if (rs.segments[i].txt) {
            if (rs.segments[i].mode == 2) {
               UtilStringBuffer *sb = (UtilStringBuffer *) rs.segments[i].txt;
               if (sb) {
                  commWrite(*(ctx->commHndl), (void*)sb->ft->getCharPtr(sb), ls[i]);
                  sb->ft->release(sb);
               }         
            }
            else {
               commWrite(*(ctx->commHndl), rs.segments[i].txt, ls[i]);
               if (rs.segments[i].mode == 1)
                  free(rs.segments[i].txt);
            }

         }
      }
   }
    
   if (rh->moreChunks==0 && ctx->pDone>=ctx->pCount) {
      char *eStr = "\r\n0\r\n";
      char status[512];
      char *desc=NULL;
      
      _SFCB_TRACE(1,("---  writing trailers"));
      
      sprintf(status,"CIMStatusCode: %d\r\n",(int)(rh->rc-1));
      if (rh->rc!=1) desc=getErrTrailer(73,rh->rc-1,NULL);

      commWrite(*(ctx->commHndl), eStr, strlen(eStr));
      commWrite(*(ctx->commHndl), status, strlen(status));
      if (desc) {
         commWrite(*(ctx->commHndl), desc, strlen(desc));
         free(desc);
      }   
      eStr="\r\n";
      commWrite(*(ctx->commHndl), eStr, strlen(eStr));
   }
   commFlush(*(ctx->commHndl));
   _SFCB_EXIT();
}

static ChunkFunctions httpChunkFunctions = {
   writeChunkResponse,
};


#define hdrBufsize 5000
#define hdrLimmit 5000

static int  getHdrs(CommHndl conn_fd, Buffer * b, char *cmd)
{
   int first=1,total=0,isReady;
   struct timeval httpTimeout;
   fd_set httpfds;
   int state=0;
   
   FD_ZERO(&httpfds);
   FD_SET(conn_fd.socket,&httpfds);
   httpTimeout.tv_sec=5;
   httpTimeout.tv_usec=0;
   isReady = select(conn_fd.socket+1,&httpfds,NULL,NULL,&httpTimeout);
   if (isReady == 0) return 3;
	
   for (;;) {
      char buf[hdrBufsize];
      int r = commRead(conn_fd, buf, sizeof(buf));
      
      if (r < 0 && (errno == EINTR || errno == EAGAIN)) continue;
      if (r <= 0) break;
      
      add2buffer(b, buf, r);
      total+=r;
//      fprintf(stderr,"+++ buf: >%s<\n",buf);
      if (r && first) {
         if (strncasecmp(buf,cmd,strlen(cmd)) != 0) { 
	   /* not what we expected - still continue to read to
	      not confuse the client */
	   state = 1;
	 }
         first=0;
      }
      
      if (strstr(b->data, "\r\n\r\n") != NULL ||
          strstr(b->data, "\n\n") != NULL) {
         break;
      }
      
      if (total>=hdrLimmit) {
         mlogf(M_ERROR,M_SHOW,"-#- Possible DOS attempt detected\n");
	 state = 2;
         break;
      }
   }
   return state;
}

int pauseCodec(char *name)
{
   int rc=0;
   char *n;
  
   if (noHttpPause) return 0;
   if (httpPauseStr==NULL) {
      if (httpPauseStr) {
         char *p;
         p=httpPauseStr=strdup(httpPauseStr);
         while (*p) { *p=tolower(*p); p++; }
      }
   }   
   if (httpPauseStr) {
      char *p;
      int l=strlen(name);
      p=n=strdup(name);      
      while (*p) { *p=tolower(*p); p++; }
      if ((p=strstr(httpPauseStr,n))!=NULL) {
         if ((p==httpPauseStr || *(p-1)==',') && (p[l]==',' || p[l]==0)) rc=1;
      }
      free(n);
      return rc;
  }
   noHttpPause=1;
   return 0;
}

static int doHttpRequest(CommHndl conn_fd)
{
   char *cp;
   Buffer inBuf = { NULL, NULL, 0, 0, 0, 0, 0 ,0};
   RespSegments response;
   static RespSegments nullResponse = { NULL, 0, 0, NULL, { {0, NULL} } };
   int len, hl, rc,uset=0;
   char *hdr, *path;
   int discardInput=0;
   MsgSegment msgs[2];
   struct rusage us,ue;
   struct timeval sv, ev;
   CimXmlRequestContext ctx;
   int breakloop;

   _SFCB_ENTER(TRACE_HTTPDAEMON, "doHttpRequest");

   if (pauseCodec("http")) for (breakloop=0;breakloop==0;) {
      fprintf(stdout,"-#- Pausing for codec http %d\n",currentProc);
      sleep(5);
   }
      
   inBuf.authorization = "";
   inBuf.protocol="HTTP/1.1";
   inBuf.content_type = NULL;
   inBuf.content_length = -1;
   inBuf.host = NULL;
   inBuf.useragent = "";
   int badReq = 0;

   rc=getHdrs(conn_fd, &inBuf,"POST ");
   
   if (rc==1) { 
      genError(conn_fd, &inBuf, 501, "Not Implemented", NULL);
      /* we continue to parse headers and empty the socket
	 to be graceful with the client */
      discardInput=1;
   }   
   else if (rc==2) {
     genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
      discardInput=2;
      _SFCB_TRACE(1, ("--- potential DOS attempt discovered."));      
   }
   else if (rc==3) {
      genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
      _SFCB_TRACE(1, ("--- exiting after request timeout."));
      commClose(conn_fd);
      exit(1);
   }
            
   if (inBuf.size == 0) {
     /* no buffer data - end of file - quit */
     _SFCB_TRACE(1,("--- HTTP connection EOF, quit %d ", currentProc));   
     commClose(conn_fd);
     exit(1);
   }
   
   inBuf.httpHdr = getNextHdr(&inBuf);
   for (badReq = 1;;) {
      if (inBuf.httpHdr == NULL)
         break;
      path = strpbrk(inBuf.httpHdr, " \t\r\n");
      if (path == NULL)
         break;
      *path++ = 0;
      _SFCB_TRACE(1,("--- Header: %s",inBuf.httpHdr));
      path += strspn(path, " \t\r\n");
      inBuf.protocol = strpbrk(path, " \t\r\n");
      *inBuf.protocol++ = 0;
#ifdef SFCB_CANONICAL_URI
      if (strcmp(path, "/cimom") != 0)
         break;
#endif
      if (inBuf.protocol == NULL)
         break;
      badReq = 0;
   }

   if (badReq) {
     genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
      _SFCB_TRACE(1, ("--- exiting after malformed header."));      
      commClose(conn_fd);
      exit(1);
   }

   while ((hdr = getNextHdr(&inBuf)) != NULL) {
      _SFCB_TRACE(1,("--- Header: %s",hdr));
      if (hdr[0] == 0)
         break;
      else if (strncasecmp(hdr, "Authorization:", 14) == 0) {
         cp = &hdr[14];
         cp += strspn(cp, " \t");
         inBuf.authorization = cp;
      }
      else if (strncasecmp(hdr, "Content-Length:", 15) == 0) {
         cp = &hdr[15];
         cp += strspn(cp, " \t");
         inBuf.content_length = atol(cp);
      }
      else if (strncasecmp(hdr, "Content-Type:", 13) == 0) {
         cp = &hdr[13];
         cp += strspn(cp, " \t");
         inBuf.content_type = cp;
      }
      else if (strncasecmp(hdr, "Host:", 5) == 0) {
         cp = &hdr[5];
         cp += strspn(cp, " \t");
         inBuf.host = cp;
         if (strchr(inBuf.host, '/') != NULL || inBuf.host[0] == '.') {
            if (!discardInput) {
   	         genError(conn_fd, &inBuf, 400, "Bad Request", NULL);
	            discardInput=2;
            }
	      }
      }
      else if (strncasecmp(hdr, "User-Agent:", 11) == 0) {
         cp = &hdr[11];
         cp += strspn(cp, " \t");
         inBuf.useragent = cp;
      }
      else if (strncasecmp(hdr, "TE:", 3) == 0) {
         char *cp = &hdr[3];
         cp += strspn(cp, " \t");
         if (strncasecmp(cp,"trailers",8)==0)
         inBuf.trailers=1;
      }
      else if (strncasecmp(hdr, "Expect:", 7) == 0) {
         if (!discardInput) {
            genError(conn_fd, &inBuf, 417, "Expectation Failed", NULL);  //more);
	         discardInput=2;
         }
      }
   }

#if defined USE_SSL
   if (doBa && sfcbSSLMode) {
     if (ccVerifyMode != CC_VERIFY_IGNORE ) {
       if (x509) {
	 inBuf.certificate = x509;
	 if (ccValidate(inBuf.certificate,&inBuf.principal,0)) {
	   /* successful certificate validation overrides basic auth */
	   doBa = 0;
	 }
       } else if (ccVerifyMode == CC_VERIFY_REQUIRE) {
	 /* this should never happen ;-) famous last words */
	 mlogf(M_ERROR,M_SHOW,
	       "\n--- Client certificate not accessible - closing connection\n");
	 commClose(conn_fd);
	 exit(1);
       }
     }     
   }
#endif

   int authorized = 0; 
   if (!discardInput && doUdsAuth) {
	   struct ucred cr; 
	   int cl = sizeof(cr); 
	   if (getsockopt(conn_fd.socket, SOL_SOCKET, SO_PEERCRED, &cr, &cl) == 0) {
		   if (cr.uid == 0) {
			   authorized = 1;
		   }
	   }
   }
   if (!authorized && !discardInput && doBa) {
     if (!(inBuf.authorization && baValidate(inBuf.authorization,&inBuf.principal))) {
       char more[]="WWW-Authenticate: Basic realm=\"cimom\"\r\n";
       genError(conn_fd, &inBuf, 401, "Unauthorized", more);
       /* we continue to parse headers and empty the socket
	  to be graceful with the client */
       discardInput=1;
     }
#if defined USE_SSL
     else if (sfcbSSLMode && ccVerifyMode != CC_VERIFY_IGNORE) {
       /* associate certificate with principal for next request */
       ccValidate(inBuf.certificate,&inBuf.principal,1);
     }
#endif
   }

   len = inBuf.content_length;
   if (len < 0) {
     if (!discardInput) {
        genError(conn_fd, &inBuf, 411, "Length Required", NULL);
     }
     _SFCB_TRACE(1, ("--- exiting after missing content length."));      
     commClose(conn_fd);
     exit(1);
   }

   hdr = (char *) malloc(strlen(inBuf.authorization) + 64);
   len += hl =
       sprintf(hdr, "<!-- xml -->\n<!-- auth: %s -->\n", inBuf.authorization);

   getPayload(conn_fd, &inBuf);
   if (discardInput) {
     free(hdr);
     freeBuffer(&inBuf);
     _SFCB_RETURN(discardInput-1);
   }

   msgs[0].data = hdr;
   msgs[0].length = hl;
   msgs[1].data = inBuf.content;
   msgs[1].length = len - hl;

   ctx.cimXmlDoc = inBuf.content;
   ctx.principal = inBuf.principal;
   ctx.host = inBuf.host;
   ctx.teTrailers = inBuf.trailers;
   ctx.cimXmlDocLength = len - hl;
   ctx.commHndl = &conn_fd;
   
   if (msgs[1].length > 0) {
      ctx.chunkFncs=&httpChunkFunctions;
      ctx.sessionId=sessionId;
      
#ifdef SFCB_DEBUG
      if ((_sfcb_trace_mask & TRACE_RESPONSETIMING) ) {
	gettimeofday(&sv,NULL);
	getrusage(RUSAGE_SELF,&us);
	uset=1;
      }
      
      if ((_sfcb_trace_mask & TRACE_XMLIN) ) {
	_sfcb_trace(1,__FILE__,__LINE__,
		    _sfcb_format_trace("-#- xmlIn %d bytes:\n%*s",inBuf.content_length,
				       inBuf.content_length,(char*)inBuf.content));
	_sfcb_trace(1,__FILE__,__LINE__,
		    _sfcb_format_trace("-#- xmlIn end\n"));
      }  
#endif 
      
      response = handleCimXmlRequest(&ctx);
   } 
   else {
     response = nullResponse;
   }
   free(hdr);

   _SFCB_TRACE(1, ("--- Generate http response"));
   if (response.chunkedMode==0) writeResponse(conn_fd, response);
   cleanupCimXmlRequest(&response);

#ifdef SFCB_DEBUG
   if (uset && (_sfcb_trace_mask & TRACE_RESPONSETIMING) ) {
      gettimeofday(&ev,NULL);
      getrusage(RUSAGE_SELF,&ue);
      _sfcb_trace(1,__FILE__,__LINE__,
		  _sfcb_format_trace("-#- Operation %.5u %s-%s real: %f user: %f sys: %f \n",
				     sessionId, opsName[ctx.operation], ctx.className,
				     timevalDiff(&sv,&ev),
				     timevalDiff(&us.ru_utime,&ue.ru_utime),
				     timevalDiff(&us.ru_stime,&ue.ru_stime)));
   }
#endif

   freeBuffer(&inBuf);
   _SFCB_RETURN(0);
}


#ifdef USE_THREADS
/**
 * from handleHttpRequest()
 *
 */
void* processThreadFunc(void* params) {
  //cast the param struct 
  struct processThreadParams* p = (struct processThreadParams*)params;

   _SFCB_ENTER(TRACE_HTTPDAEMON, "handleHttpRequest-processthread");

  processName="CIMXML-Processor";
  
  localMode=0;
  if (doFork) {
    _SFCB_TRACE(1,("--- Forked xml handler %d", currentProc))
  }

  _SFCB_TRACE(1,("--- Started xml handler %d %d", currentProc,
		 resultSockets.receive));

  if (getenv("SFCB_PAUSE_HTTP")) for (p->breakloop=0;p->breakloop==0;) {
    fprintf(stderr,"-#- Pausing - pid: %d\n",currentProc);
    sleep(5);
  }   

  p->conn_fd.socket=p->connFd;
  p->conn_fd.file=fdopen(p->connFd,"a");
  p->conn_fd.buf = NULL;
  if (p->conn_fd.file == NULL) {
    mlogf(M_ERROR,M_SHOW,"--- failed to create socket stream - continue with raw socket: %s\n",strerror(errno));
  } else {
    p->conn_fd.buf = malloc(SOCKBUFSZ);
    if (p->conn_fd.buf) {
      setbuffer(p->conn_fd.file,p->conn_fd.buf,SOCKBUFSZ);
    } else {
      mlogf(M_ERROR,M_SHOW,"--- failed to create socket buffer - continue unbuffered: %s\n",strerror(errno));
    }
  }
  if (sfcbSSLMode) {
#if defined USE_SSL
    BIO *sslb;
    BIO *sb=BIO_new_socket(p->connFd,BIO_NOCLOSE);
    if (!(->conn_fd.ssl = SSL_new(ctx)))
      intSSLerror("Error creating SSL object");
    SSL_set_bio(p->conn_fd.ssl, sb, sb);
    if (SSL_accept(p->conn_fd.ssl) <= 0)
      intSSLerror("Error accepting SSL connection");
    sslb = BIO_new(BIO_f_ssl());
    BIO_set_ssl(sslb,p->conn_fd.ssl,BIO_CLOSE);
    p->conn_fd.bio=BIO_new(BIO_f_buffer());
    BIO_push(p->conn_fd.bio,sslb);
    if (BIO_set_write_buffer_size(p->conn_fd.bio,SOCKBUFSZ)) { 
    } else {
      p->conn_fd.bio=NULL;
    }
#endif
  } else {
#if defined USE_SSL
    p->conn_fd.bio=NULL;
    p->conn_fd.ssl=NULL;
#endif
  }
      
  numRequest = 0;
  FD_ZERO(&p->httpfds);
  FD_SET(p->conn_fd.socket,&p->httpfds);
  do {
    numRequest += 1;
        
    if (doHttpRequest(p->conn_fd)) {
      /* eof reached - leave */
      break;
    }
    if (keepaliveTimeout==0 || numRequest >= keepaliveMaxRequest ) {
      /* no persistence wanted or exceeded - quit */
      break;
    }
    /* wait for next request or timeout */
    p->httpTimeout.tv_sec=keepaliveTimeout;
    p->httpTimeout.tv_usec=keepaliveTimeout;
    p->isReady = select(p->conn_fd.socket+1,&p->httpfds,NULL,NULL,&p->httpTimeout);
    if (p->isReady == 0) {
      _SFCB_TRACE(1,("--- HTTP connection timeout, quit %d ", currentProc));
      break;
    } else if (p->isReady < 0) {
      _SFCB_TRACE(1,("--- HTTP connection error, quit %d ", currentProc));
      break;
    }
  } while (1);

  commClose(p->conn_fd);
  if (!doFork) return;

  _SFCB_TRACE(1, ("--- Xml handler exiting %d", currentProc));
  dumpTiming(currentProc);

return;
}
#endif

/**
 * called by httpDaemon
 *
 */
static void handleHttpRequest(int connFd)
{
   int r;
   CommHndl conn_fd;
   int isReady;
   fd_set httpfds;
   struct sembuf procReleaseUnDo = {0,1,SEM_UNDO};
   struct timeval httpTimeout;
   int breakloop;

   _SFCB_ENTER(TRACE_HTTPDAEMON, "handleHttpRequest");

   _SFCB_TRACE(1, ("--- Forking xml handler"));

#ifdef USE_THREADS
   if (doFork) {
//       semAcquire(httpWorkSem,0);
//       semAcquire(httpProcSem,0);
//       for (httpProcIdX=0; httpProcIdX<hMax; httpProcIdX++)
//          if (semGetValue(httpProcSem,httpProcIdX+1)==0) break;
//       procReleaseUnDo.sem_num=httpProcIdX+1; 

      sessionId++;
      //r = fork();
      struct processThreadParams ptparams = {breakloop, conn_fd, connFd, httpfds, httpTimeout, isReady};
      _SFCB_TRACE(1, ("--- Spawning HTTP process thread"));
      processThreadFunc(&ptparams);

      //child's thread of execution
//       if (r==0) {
//          currentProc=getpid();
//          processName="CIMXML-Processor";
//          semRelease(httpProcSem,0);
//          semAcquireUnDo(httpProcSem,0);
//          semReleaseUnDo(httpProcSem,httpProcIdX+1);
//          semRelease(httpWorkSem,0);
//          atexit(uninitGarbageCollector);                      |
//          atexit(sunsetControl);
//       }
      // parent's thread of execution
      //else if (r>0) {
         running++;
         _SFCB_EXIT();
      //}
   }
   //else r = 0;

   // fork() failed
//    if (r < 0) {
//       char *emsg=strerror(errno);
//       mlogf(M_ERROR,M_SHOW,"--- fork handler: %s\n",emsg);
//       exit(1);
//    }

   // child's thread of execution || doFork=0
   // moved this hunk to processThreadFunc for process thread to work on; this block remains for when doFork=0
   //if (r == 0) {
   if (doFork == 0) {

      localMode=0;
      if (doFork) {
         _SFCB_TRACE(1,("--- Forked xml handler %d", currentProc))
      }

      _SFCB_TRACE(1,("--- Started xml handler %d %d", currentProc,
                   resultSockets.receive));

      if (getenv("SFCB_PAUSE_HTTP")) for (breakloop=0;breakloop==0;) {
         fprintf(stderr,"-#- Pausing - pid: %d\n",currentProc);
         sleep(5);
      }   

      conn_fd.socket=connFd;
      conn_fd.file=fdopen(connFd,"a");

     if (conn_fd.file == NULL) {
	mlogf(M_ERROR,M_SHOW,"--- failed to create socket stream - continue with raw socket: %s\n",strerror(errno));
      } else {
	conn_fd.buf = malloc(SOCKBUFSZ);
	if (conn_fd.buf) {
	  setbuffer(conn_fd.file,conn_fd.buf,SOCKBUFSZ);
	} else {
	  mlogf(M_ERROR,M_SHOW,"--- failed to create socket buffer - continue unbuffered: %s\n",strerror(errno));
	}
      }
      if (sfcbSSLMode) {
#if defined USE_SSL
	BIO *sslb;
	BIO *sb;
	long flags = fcntl(connFd,F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(connFd,F_SETFL,flags);
	sb=BIO_new_socket(connFd,BIO_NOCLOSE);
	if (!(conn_fd.ssl = SSL_new(ctx)))
	  intSSLerror("Error creating SSL object");
	SSL_set_bio(conn_fd.ssl, sb, sb);
	while(1) {
	  int sslacc, sslerr;
	  sslacc = SSL_accept(conn_fd.ssl);
	  if (sslacc == 1) {
	    /* accepted */
	    break;
	  }
	  sslerr = SSL_get_error(conn_fd.ssl,sslacc);
	  if (sslerr == SSL_ERROR_WANT_WRITE || 
	      sslerr == SSL_ERROR_WANT_READ) {
	    /* still in handshake */
	    FD_ZERO(&httpfds);
	    FD_SET(connFd,&httpfds);
	    httpTimeout.tv_sec=5;
	    httpTimeout.tv_usec=0;
	    if (sslerr == SSL_ERROR_WANT_WRITE) {
	      isReady = select(connFd+1,NULL,&httpfds,NULL,&httpTimeout);
	    } else {
	      isReady = select(connFd+1,&httpfds,NULL,NULL,&httpTimeout);
	    }
	    if (isReady == 0) {
	      intSSLerror("Timeout error accepting SSL connection");
	    } else if (isReady < 0) {
	      intSSLerror("Error accepting SSL connection");
	    }
	  } else {
	    /* unexpected error */
	    intSSLerror("Error accepting SSL connection");
	  } 
	}
	flags ^= O_NONBLOCK;
	fcntl(connFd,F_SETFL,flags);
	sslb = BIO_new(BIO_f_ssl());
	BIO_set_ssl(sslb,conn_fd.ssl,BIO_CLOSE);
	conn_fd.bio=BIO_new(BIO_f_buffer());
	BIO_push(conn_fd.bio,sslb);
	if (BIO_set_write_buffer_size(conn_fd.bio,SOCKBUFSZ)) { 
	} else {
	  conn_fd.bio=NULL;
	}
#endif
      } else {
#if defined USE_SSL
	conn_fd.bio=NULL;
	conn_fd.ssl=NULL;
#endif
      }
      
      numRequest = 0;
      FD_ZERO(&httpfds);
      FD_SET(conn_fd.socket,&httpfds);
      do {
	numRequest += 1;
        
        if (doHttpRequest(conn_fd)) {
	  /* eof reached - leave */
	  break;
	}
	if (keepaliveTimeout==0 || numRequest >= keepaliveMaxRequest ) {
	  /* no persistence wanted or exceeded - quit */
	  break;
	}
	/* wait for next request or timeout */
	httpTimeout.tv_sec=keepaliveTimeout;
	httpTimeout.tv_usec=keepaliveTimeout;
	isReady = select(conn_fd.socket+1,&httpfds,NULL,NULL,&httpTimeout);
	if (isReady == 0) {
	  _SFCB_TRACE(1,("--- HTTP connection timeout, quit %d ", currentProc));
	  break;
	} else if (isReady < 0) {
	  _SFCB_TRACE(1,("--- HTTP connection error, quit %d ", currentProc));
	  break;
	}
      } while (1);

      commClose(conn_fd);
      if (!doFork) return;

      _SFCB_TRACE(1, ("--- Xml handler exiting %d", currentProc));
      dumpTiming(currentProc);
      return;
   }

#else /* end threads section, start forked process section */

   if (doFork) {
      semAcquire(httpWorkSem,0);
      semAcquire(httpProcSem,0);
      for (httpProcIdX=0; httpProcIdX<hMax; httpProcIdX++)
         if (semGetValue(httpProcSem,httpProcIdX+1)==0) break;
      procReleaseUnDo.sem_num=httpProcIdX+1; 

      sessionId++;
      r = fork();

      /* child's thread of execution */
      if (r==0) {
         currentProc=getpid();
         processName="CIMXML-Processor";
         semRelease(httpProcSem,0);
         semAcquireUnDo(httpProcSem,0);
         semReleaseUnDo(httpProcSem,httpProcIdX+1);
         semRelease(httpWorkSem,0);
	 atexit(uninitGarbageCollector);
	 atexit(sunsetControl);
      }
      /* parent's thread of execution */
      else if (r>0) {
         running++;
         _SFCB_EXIT();
      }
   }
   else r = 0;

   /* fork() failed */
   if (r < 0) {
      char *emsg=strerror(errno);
      mlogf(M_ERROR,M_SHOW,"--- fork handler: %s\n",emsg);
      exit(1);
   }

   if (r == 0) {
      localMode=0;
      /* child's thread of execution || doFork=0 */
      if (doFork) {
         _SFCB_TRACE(1,("--- Forked xml handler %d", currentProc))
      }

      _SFCB_TRACE(1,("--- Started xml handler %d %d", currentProc,
                   resultSockets.receive));

      if (getenv("SFCB_PAUSE_HTTP")) for (breakloop=0;breakloop==0;) {
         fprintf(stderr,"-#- Pausing - pid: %d\n",currentProc);
         sleep(5);
      }   

      conn_fd.socket=connFd;
      conn_fd.file=fdopen(connFd,"a");
      conn_fd.buf = NULL;
      if (conn_fd.file == NULL) {
	mlogf(M_ERROR,M_SHOW,"--- failed to create socket stream - continue with raw socket: %s\n",strerror(errno));
      } else {
	conn_fd.buf = malloc(SOCKBUFSZ);
	if (conn_fd.buf) {
	  setbuffer(conn_fd.file,conn_fd.buf,SOCKBUFSZ);
	} else {
	  mlogf(M_ERROR,M_SHOW,"--- failed to create socket buffer - continue unbuffered: %s\n",strerror(errno));
	}
      }
      if (sfcbSSLMode) {
#if defined USE_SSL
	BIO *sslb;
	BIO *sb;
	long flags = fcntl(connFd,F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(connFd,F_SETFL,flags);
	sb=BIO_new_socket(connFd,BIO_NOCLOSE);
	if (!(conn_fd.ssl = SSL_new(ctx)))
	  intSSLerror("Error creating SSL object");
	SSL_set_bio(conn_fd.ssl, sb, sb);
	while(1) {
	  int sslacc, sslerr;
	  sslacc = SSL_accept(conn_fd.ssl);
	  if (sslacc == 1) {
	    /* accepted */
	    break;
	  }
	  sslerr = SSL_get_error(conn_fd.ssl,sslacc);
	  if (sslerr == SSL_ERROR_WANT_WRITE || 
	      sslerr == SSL_ERROR_WANT_READ) {
	    /* still in handshake */
	    FD_ZERO(&httpfds);
	    FD_SET(connFd,&httpfds);
	    httpTimeout.tv_sec=5;
	    httpTimeout.tv_usec=0;
	    if (sslerr == SSL_ERROR_WANT_WRITE) {
	      isReady = select(connFd+1,NULL,&httpfds,NULL,&httpTimeout);
	    } else {
	      isReady = select(connFd+1,&httpfds,NULL,NULL,&httpTimeout);
	    }
	    if (isReady == 0) {
	      intSSLerror("Timeout error accepting SSL connection");
	    } else if (isReady < 0) {
	      intSSLerror("Error accepting SSL connection");
	    }
	  } else {
	    /* unexpected error */
	    intSSLerror("Error accepting SSL connection");
	  } 
	}
	flags ^= O_NONBLOCK;
	fcntl(connFd,F_SETFL,flags);
	sslb = BIO_new(BIO_f_ssl());
	BIO_set_ssl(sslb,conn_fd.ssl,BIO_CLOSE);
	conn_fd.bio=BIO_new(BIO_f_buffer());
	BIO_push(conn_fd.bio,sslb);
	if (BIO_set_write_buffer_size(conn_fd.bio,SOCKBUFSZ)) { 
	} else {
	  conn_fd.bio=NULL;
	}
#endif
      } else {
#if defined USE_SSL
	conn_fd.bio=NULL;
	conn_fd.ssl=NULL;
#endif
      }
      
      numRequest = 0;
      FD_ZERO(&httpfds);
      FD_SET(conn_fd.socket,&httpfds);
      do {
	numRequest += 1;
        
        if (doHttpRequest(conn_fd)) {
	  /* eof reached - leave */
	  break;
	}
	if (keepaliveTimeout==0 || numRequest >= keepaliveMaxRequest ) {
	  /* no persistence wanted or exceeded - quit */
	  break;
	}
	/* wait for next request or timeout */
	httpTimeout.tv_sec=keepaliveTimeout;
	httpTimeout.tv_usec=keepaliveTimeout;
	isReady = select(conn_fd.socket+1,&httpfds,NULL,NULL,&httpTimeout);
	if (isReady == 0) {
	  _SFCB_TRACE(1,("--- HTTP connection timeout, quit %d ", currentProc));
	  break;
	} else if (isReady < 0) {
	  _SFCB_TRACE(1,("--- HTTP connection error, quit %d ", currentProc));
	  break;
	}
      } while (1);

      commClose(conn_fd);
      if (!doFork) return;

      _SFCB_TRACE(1, ("--- Xml handler exiting %d", currentProc));
      dumpTiming(currentProc);
      exit(0);
   }

#endif  //end forked section

}


int httpDaemon(int argc, char *argv[], int sslMode, int sfcbPid)
{

#ifdef USE_INET6
   struct sockaddr_in6 sin;
#else
   struct sockaddr_in sin;
#endif
   struct sockaddr_un sun; 

   socklen_t sz,sin_len,sun_len;
   int i,ru,rc;
   char *cp;
   long procs, port;
   int listenFd=-1, udsListenFd=-1, connFd; 
   int enableUds=0,enableHttp=0;
   fd_set httpfds;
   int maxfdp1; 

   static char *udsPath=NULL;

   name = argv[0];
   debug = 1;
   doFork = 1;

   _SFCB_ENTER(TRACE_HTTPDAEMON, "httpDaemon");

   setupControl(configfile);
   sfcbSSLMode=sslMode;
   if (sslMode) processName="HTTPS-Daemon";
   else processName="HTTP-Daemon";

   if (sfcbSSLMode) {
      if (getControlNum("httpsPort", &port))
         port = 5989;
      hBase=stBase;
      hMax=stMax;
   }
   else {
      if (getControlNum("httpPort", &port))
         port = 5988;
      if (getControlChars("httpSocketPath", &udsPath)) 
		 udsPath = "/tmp/sfcbHttpSocket"; 
      hBase=htBase;
      hMax=htMax;
   }

   if (sslMode) {
     if (getControlNum("httpsProcs", &procs))
       procs = 10;
   } else {
     if (getControlNum("httpProcs", &procs))
       procs = 10;
     if (getControlBool("enableHttp", &enableHttp))
       enableHttp=1;
	 if (getControlBool("enableUds", &enableUds))
       enableUds=1;
	 if (!enableUds)
		 udsPath = NULL; 
	 if (!enableHttp)
		 port = -1; 
   }
   initHttpProcCtl(procs,sslMode);

   if (getControlBool("doBasicAuth", &doBa))
      doBa=0;

   if (getControlBool("doUdsAuth", &doUdsAuth))
      doUdsAuth=0;

   if (getControlNum("keepaliveTimeout", &keepaliveTimeout))
     keepaliveTimeout = 15;

   if (getControlNum("keepaliveMaxRequest", &keepaliveMaxRequest))
     keepaliveMaxRequest = 10;

   i = 1;
   while (i < argc && argv[i][0] == '-') {
      if (strcmp(argv[i], "-D") == 0)
         debug = 1;
      else if (strcmp(argv[i], "-nD") == 0)
         debug = 0;
      else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
         ++i;
         port = (unsigned short) atoi(argv[i]);
      }
      else if (strcmp(argv[i], "-tm") == 0) {
         if (isdigit(*argv[i + 1])) {
            ++i;
         }
      }
      else if (strcmp(argv[i], "-F") == 0)
         doFork = 1;
      else if (strcmp(argv[i], "-nF") == 0)
         doFork = 0;
      else if (strcmp(argv[i], "-H") == 0);
      ++i;
   }

   if (getControlBool("useChunking", &noChunking))
      noChunking=0;
   noChunking=noChunking==0;

   cp = strrchr(name, '/');
   if (cp != NULL)
      ++cp;
   else  cp = name;
   name = cp;

   if (sslMode) mlogf(M_INFO,M_SHOW,"--- %s HTTPS Daemon V" sfcHttpDaemonVersion " started - %d - port %ld\n", 
         name, currentProc,port);
   else mlogf(M_INFO,M_SHOW,"--- %s HTTP  Daemon V" sfcHttpDaemonVersion " started - %d - port %ld, %s\n", 
         name, currentProc,port,udsPath);


   if (doBa) mlogf(M_INFO,M_SHOW,"--- Using Basic Authentication\n");
   if (doUdsAuth) mlogf(M_INFO,M_SHOW,"--- Using Unix Socket Peer Cred Authentication\n");

   if (keepaliveTimeout == 0) {
     mlogf(M_INFO,M_SHOW,"--- Keep-alive timeout disabled\n");
   } else {
     mlogf(M_INFO,M_SHOW,"--- Keep-alive timeout: %ld seconds\n",keepaliveTimeout);
     mlogf(M_INFO,M_SHOW,"--- Maximum requests per connection: %ld\n",keepaliveMaxRequest);
   }

   ru = 1;
   if (enableUds) {
      udsListenFd = socket(PF_UNIX, SOCK_STREAM, 0); 
   }
   if (enableHttp || sslMode) {
#ifdef USE_INET6
      listenFd = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
      if (listenFd < 0) { 
          mlogf(M_INFO,M_SHOW,"--- Using IPv4 address\n");
          listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
      }
#else
      listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif
      setsockopt(listenFd, SOL_SOCKET, SO_REUSEADDR, (char *) &ru, sizeof(ru));
   }

   sin_len = sizeof(sin);
   sun_len = sizeof(sun); 

   memset(&sin,0,sin_len);
   memset(&sun,0,sun_len);

   if (udsListenFd >= 0) {
     if (getControlChars("httpSocketPath", &udsPath)) {
         mlogf(M_ERROR,M_SHOW,"--- No unix socket path defined for HTTP\n"); 
         sleep(1);
         kill(sfcbPid,3);
     }
     sun.sun_family=AF_UNIX;
     strcpy(sun.sun_path,udsPath);
   }

   if (listenFd >= 0) {
      if (getControlBool("httpLocalOnly", &httpLocalOnly))
	    httpLocalOnly=0;

#ifdef USE_INET6
     sin.sin6_family = AF_INET6;
     if (httpLocalOnly)
	   sin.sin6_addr = in6addr_loopback;
     else
	   sin.sin6_addr = in6addr_any;
     sin.sin6_port = htons(port);
#else
     sin.sin_family = AF_INET;
     if (httpLocalOnly) {
	   char* loopback_int = "127.0.0.1";
	   inet_aton(loopback_int, &sin.sin_addr); /* not INADDR_LOOPBACK ? */
     }
     else
	   sin.sin_addr.s_addr = INADDR_ANY;
     sin.sin_port = htons(port);
#endif 
   }

  if (listenFd >= 0) {
     if (bind(listenFd, (struct sockaddr *) &sin, sin_len) ||
             listen(listenFd, 10)) {
            mlogf(M_ERROR,M_SHOW,"--- Cannot listen on port %ld (%s)\n", port, strerror(errno));
            sleep(1);
            kill(sfcbPid,3);
     }
  }
  if (udsListenFd >= 0) {
     unlink(udsPath); 
     if (bind(udsListenFd, (struct sockaddr *) &sun, sun_len) ||
             listen(udsListenFd, 10)) {
            mlogf(M_ERROR,M_SHOW,"--- Cannot listen on unix socket %s (%s)\n", udsPath, strerror(errno));
            sleep(1);
            kill(sfcbPid,3);
     }
  }

  if (!debug) {
      int rc = fork();
      if (rc == -1) {
         char *emsg=strerror(errno);
         mlogf(M_ERROR,M_SHOW,"--- fork daemon: %s",emsg);
         exit(1);
      }
      else if (rc)
         exit(0);
   }
//   memInit();
    currentProc=getpid();

/* don't want these if we're using threads */
#ifndef USE_THREADS
    setSignal(SIGCHLD, handleSigChld,0);
    setSignal(SIGUSR1, handleSigUsr1,0);
    setSignal(SIGINT, SIG_IGN,0);
    setSignal(SIGTERM, SIG_IGN,0);
    setSignal(SIGHUP, SIG_IGN,0);
#endif

    commInit();

#if defined USE_SSL
    if (sfcbSSLMode) {
       char *fnc,*fnk, *fnt, *fnl;
       ctx = SSL_CTX_new(SSLv23_method());
       getControlChars("sslCertificateFilePath", &fnc);
       _SFCB_TRACE(1,("---  sslCertificateFilePath = %s",fnc));
       if (SSL_CTX_use_certificate_chain_file(ctx, fnc) != 1)
	 intSSLerror("Error loading certificate from file");
       getControlChars("sslKeyFilePath", &fnk);
       _SFCB_TRACE(1,("---  sslKeyFilePath = %s",fnk));
       if (SSL_CTX_use_PrivateKey_file(ctx, fnk, SSL_FILETYPE_PEM) != 1)
	 intSSLerror("Error loading private key from file");
       getControlChars("sslClientCertificate", &fnl);
       _SFCB_TRACE(1,("---  sslClientCertificate = %s",fnl));
       if (strcasecmp(fnl,"ignore") == 0) {
	 ccVerifyMode = CC_VERIFY_IGNORE;
	 SSL_CTX_set_verify(ctx,SSL_VERIFY_NONE,0);
       } else if (strcasecmp(fnl,"accept") == 0) {
	 ccVerifyMode = CC_VERIFY_ACCEPT;
	 SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,get_cert);
       } else if (strcasecmp(fnl,"require") == 0) {
	 ccVerifyMode = CC_VERIFY_REQUIRE;
	 SSL_CTX_set_verify(ctx,
			    SSL_VERIFY_PEER|SSL_VERIFY_FAIL_IF_NO_PEER_CERT,
			    get_cert);
       } else {
	 intSSLerror("sslClientCertificate must be one of: ignore, accept or require");	 
       } 
       getControlChars("sslClientTrustStore", &fnt);
       _SFCB_TRACE(1,("---  sslClientTrustStore = %s",fnt));
       if (ccVerifyMode != CC_VERIFY_IGNORE &&
	   SSL_CTX_load_verify_locations(ctx, fnt, NULL) != 1)
	 intSSLerror("Error locating the client trust store");

       /* SSLv2 is pretty old; no one should be needing it any more */
       SSL_CTX_set_options(ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | 
                           SSL_OP_SINGLE_DH_USE); 
       /* disable weak ciphers */
       if (SSL_CTX_set_cipher_list(ctx, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH") != 1)
           intSSLerror("Error setting cipher list (no valid ciphers)"); 
	      
    }
#endif

   maxfdp1 = (listenFd > udsListenFd? listenFd : udsListenFd) + 1; 
   for (;;) {
      char *emsg;
      // listen(listenFd, 1);
	  FD_ZERO(&httpfds); 
	  if (listenFd >= 0) {
         FD_SET(listenFd, &httpfds); 
	  }
	  if (udsListenFd >= 0) {
         FD_SET(udsListenFd, &httpfds); 
      }
	  rc = select(maxfdp1, &httpfds, NULL, NULL, NULL); 
      if (rc < 0) {
         if (errno == EINTR || errno == EAGAIN) {
            if (stopAccepting) break;
            continue;
         }
      }
	  if (listenFd >= 0 && FD_ISSET(listenFd, &httpfds)) {
		  sz = sin_len; 
		  if ((connFd = accept(listenFd, (__SOCKADDR_ARG) &sin, &sz))<0) {
			 if (errno == EINTR || errno == EAGAIN) {
				if (stopAccepting) break;
				continue;
			 }   
			 emsg=strerror(errno);
			 mlogf(M_ERROR,M_SHOW,"--- accept error %s\n",emsg);
			 _SFCB_ABORT();
		  }
		  _SFCB_TRACE(1, ("--- Processing http request"));

		  handleHttpRequest(connFd);
		  close(connFd);
	  }
	  if (udsListenFd >= 0 && FD_ISSET(udsListenFd, &httpfds)) {
		  sz = sun_len; 
		  if ((connFd = accept(udsListenFd, (__SOCKADDR_ARG) &sun, &sz))<0) {
			 if (errno == EINTR || errno == EAGAIN) {
				if (stopAccepting) break;
				continue;
			 }   
			 emsg=strerror(errno);
			 mlogf(M_ERROR,M_SHOW,"--- accept error %s\n",emsg);
			 _SFCB_ABORT();
		  }
		  _SFCB_TRACE(1, ("--- Processing http request"));

		  handleHttpRequest(connFd);
		  close(connFd);
	  }
   }
   
   remProcCtl();   
   
//   printf("--- %s draining %d\n",processName,running);
   for (;;) {
      if (running==0) {
         mlogf(M_INFO,M_SHOW,"--- %s terminating %d\n",processName,getpid());
         exit(0);
      }   
      sleep(1);
   }   

}

#if defined USE_SSL
static int get_cert(int preverify_ok,X509_STORE_CTX* x509_ctx)
{
  if (preverify_ok) {
    x509 = X509_STORE_CTX_get_current_cert(x509_ctx);
  }
  return preverify_ok;
}

typedef int (*Validate)(X509 *certificate, char ** principal, int mode);

static int ccValidate(X509 *certificate, char ** principal, int mode)
{
  int   result=0;
  char *ln;
  char  dlName[512];
  void *authLib;
  Validate validate;
  _SFCB_ENTER(TRACE_HTTPDAEMON, "ccValidate");
  
  if (getControlChars("certificateAuthlib", &ln)==0) {
    libraryName(NULL,ln,dlName);
    if ((authLib=dlopen(dlName, RTLD_LAZY))) {
      validate= dlsym(authLib, "_sfcCertificateAuthenticate");
      if (validate) {
	result = validate(certificate,principal,mode);
      } else {
	mlogf(M_ERROR,M_SHOW,
	      "--- Certificate authentication exit %s not found\n",dlName);
	result = 0;
      } 
    }
  } else {
    mlogf(M_ERROR,M_SHOW,
	  "--- Certificate authentication exit not configured\n",dlName);	
  }
  _SFCB_RETURN(result);
}
#endif
