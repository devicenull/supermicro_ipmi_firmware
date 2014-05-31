
/*
 * $Id: providerDrv.c,v 1.65 2008/10/03 19:15:45 buccella Exp $
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
 * CMPI style provider driver.
 *
*/

#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#include "trace.h"
#include "cmpidt.h"
#include "providerMgr.h"
#include "providerRegister.h"
#include "utilft.h"
#include "cimXmlParser.h"
#include "support.h"
#include "msgqueue.h"
#include "constClass.h"
#include "native.h"
#include "queryOperation.h"
#include "selectexp.h"
#include "control.h"
#include "config.h"
#include "constClass.h"

#ifdef HAVE_QUALREP
#include "qualifier.h"
#endif

#ifdef HAVE_INDICATIONS
#define SFCB_INCL_INDICATION_SUPPORT 1
#endif

#define PROVCHARS(p) (p && *((char*)p)) ? (char*)p : NULL

char * opsName[];


#ifdef SFCB_DEBUG

#define TIMING_PREP \
   int uset=0; \
   struct rusage us,ue,cs,ce; \
   struct timeval sv,ev;

#define TIMING_START(req,pInfo)						\
      if (pInfo && req->sessionId && (_sfcb_trace_mask & TRACE_RESPONSETIMING) ) {\
         gettimeofday(&sv,NULL);\
         getrusage(RUSAGE_SELF,&us);\
         getrusage(RUSAGE_CHILDREN,&cs);\
         uset=1;\
      }      

#define TIMING_STOP(req,pInfo)					\
      if (uset && (_sfcb_trace_mask & TRACE_RESPONSETIMING) ) { \
	gettimeofday(&ev,NULL); \
	getrusage(RUSAGE_SELF,&ue); \
	getrusage(RUSAGE_CHILDREN,&ce); \
	_sfcb_trace(1, __FILE__, __LINE__, \
		    _sfcb_format_trace("-#- Provider  %.5u %s-%s real: %f user: %f sys: %f children user: %f children sys: %f \n",  \
				       req->sessionId, \
				       opsName[req->operation], \
				       pInfo->providerName, \
				       timevalDiff(&sv,&ev), \
				       timevalDiff(&us.ru_utime,&ue.ru_utime), \
				       timevalDiff(&us.ru_stime,&ue.ru_stime), \
				       timevalDiff(&cs.ru_utime,&ce.ru_utime), \
				       timevalDiff(&cs.ru_stime,&ce.ru_stime) \
				       )); \
      }
#else

#define TIMING_PREP 
#define TIMING_START(req,pInfo)
#define TIMING_STOP(req,pInfo)

#endif

extern CMPIBroker *Broker;

extern unsigned long exFlags;
extern ProviderRegister *pReg;
extern ProviderInfo *classProvInfoPtr;

extern void processProviderInvocationRequests(char *);
extern CMPIObjectPath *relocateSerializedObjectPath(void *area);
extern MsgSegment setInstanceMsgSegment(CMPIInstance * op);
extern MsgSegment setArgsMsgSegment(CMPIArgs * args);
extern MsgSegment setConstClassMsgSegment(CMPIConstClass * cl);
extern void getSerializedConstClass(CMPIConstClass * cl, void *area);
extern void getSerializedArgs(CMPIArgs * cl, void *area);
extern CMPIConstClass *relocateSerializedConstClass(void *area);
extern CMPIInstance *relocateSerializedInstance(void *area);
extern CMPIConstClass *relocateSerializedConstClass(void *area);
extern CMPIArgs *relocateSerializedArgs(void *area);
extern MsgSegment setArgsMsgSegment(CMPIArgs * args);
extern void dump(char *msg, void *a, int l);
extern void showClHdr(void *ihdr);
extern ProvIds getProvIds(ProviderInfo *info);
extern int xferLastResultBuffer(CMPIResult *result, int to, int rc);
extern void setResultQueryFilter(CMPIResult * result, QLStatement *qs);
extern CMPIArray *getKeyListAndVerifyPropertyList(CMPIObjectPath*, 
                char **props, int *ok,CMPIStatus * rc);
extern void dumpTiming(int pid);
static BinResponseHdr *errorCharsResp(int rc, char *msg);
static int sendResponse(int requestor, BinResponseHdr * hdr);
                
extern CMPISelectExp *NewCMPISelectExp(const char *queryString, const char *language, 
           const char *sns, CMPIArray ** projection, CMPIStatus * rc);
NativeSelectExp *activFilters=NULL;
extern void setStatus(CMPIStatus *st, CMPIrc rc, char *msg);
extern CMPIString *NewCMPIString(const char *ptr, CMPIStatus * rc);

static ProviderProcess *provProc=NULL,*curProvProc=NULL;
static int provProcMax=0;
static int idleThreadStartHandled=0;

ProviderInfo *activProvs = NULL;
int indicationEnabled=0;

unsigned long provSampleInterval=10;
unsigned long provTimeoutInterval=25;
unsigned      provAutoGroup=0;
static int stopping=0;

void uninitProvProcCtl();
extern void uninitSocketPairs();
extern void sunsetControl();
extern void uninitGarbageCollector();


typedef struct parms { 
   int requestor;
   BinRequestHdr *req;
   ProviderInfo *pInfo;
   struct parms  *next,*prev; 
} Parms;
static Parms *activeThreadsFirst=NULL,*activeThreadsLast=NULL;

/* old version support */
typedef CMPIStatus (*authorizeFilterPreV1)
  (CMPIIndicationMI* mi, const CMPIContext* ctx, CMPIResult *result,
   const CMPISelectExp* se, const char *ns, const CMPIObjectPath* op, const char *user);

typedef CMPIStatus (*mustPollPreV1)
  (CMPIIndicationMI* mi, const CMPIContext* ctx,  CMPIResult *result,
   const CMPISelectExp* se, const char *ns, const CMPIObjectPath* op);

typedef CMPIStatus (*activateFilterPreV1)
  (CMPIIndicationMI* mi, const CMPIContext* ctx,  CMPIResult *result,
   const CMPISelectExp* se, const char *ns, const CMPIObjectPath* op, CMPIBoolean first);

typedef CMPIStatus (*deActivateFilterPreV1)
  (CMPIIndicationMI* mi, const CMPIContext* ctx,  CMPIResult *result,
   const CMPISelectExp* se, const  char *ns, const CMPIObjectPath* op, CMPIBoolean last);


void libraryName(const char *dir, const char *location, char *fullName)
{
#if defined(CMPI_PLATFORM_WIN32_IX86_MSVC)
  if (dir) {
    sprintf(fullName,"%s\\%s.dll",dir,location);
  } else {
    sprintf(fullName,"%s.dll",location);
  }
#elif defined(CMPI_PLATFORM_LINUX_GENERIC_GNU)
  if (dir) {
   sprintf(fullName,"%s/lib%s.so",dir,location);
  } else {
    sprintf(fullName,"lib%s.so",location);
  }
#elif defined(CMPI_OS_HPUX)
  if (dir) {
   sprintf(fullName,"%s/lib%s.so",dir,location);
  } else {
    sprintf(fullName,"lib%s.so",location);
  }
#elif defined(CMPI_OS_OS400)
  if (dir) {
   strcpy(fullName, location);
#elif defined(CMPI_OS_DARWIN)
  if (dir) {
   sprintf(fullName,"%s/lib%s.dylib",dir,location);
  } else {
    sprintf(fullName,"lib%s.dylib",location);
  }
#else
  if (dir) {
   sprintf(fullName,"%s/lib%s.so",dir,location);
  } else {
    sprintf(fullName,"lib%s.so",location);
  }
#endif
}

int testStartedProc(int pid, int *left) 
{
   ProviderProcess *pp=provProc;
   ProviderInfo *info;
   int i,stopped=0;
   
   *left=0;
   for (i=0; i<provProcMax; i++) {
      if ((pp+i)->pid==pid) {
         stopped=1;
         (pp+i)->pid=0;
         info=(pp+i)->firstProv;
         pReg->ft->resetProvider(pReg,pid);
      }   
      if ((pp+i)->pid!=0) (*left)++;
   }
   
   if (pid==classProvInfoPtr->pid) {
      stopped=1;
      classProvInfoPtr->pid=0;
   }
   if (classProvInfoPtr->pid!=0) (*left)++;
   
   return stopped;
}         

int stopNextProc()
{
   ProviderProcess *pp=provProc;
   int i,done=0,t;
   
   for (i=provProcMax-1; i; i--) {
      if ((pp+i)->pid) {
         kill((pp+i)->pid,SIGUSR1);
         return (pp+i)->pid;
      }
   }
   
   if (done==0) {
      if (classProvInfoPtr && classProvInfoPtr->pid) {
         t=classProvInfoPtr->pid;
         kill(classProvInfoPtr->pid,SIGUSR1);
         done=1;
         return t;
      }
   }   
   
   return 0;
}

static void stopProc(void *p)
{
   ProviderInfo *pInfo;
   CMPIContext *ctx = NULL;

   ctx = native_new_CMPIContext(MEM_NOT_TRACKED,NULL);
//   for (pInfo=curProvProc->firstProv; pInfo; pInfo=pInfo->next) {
   for (pInfo=activProvs; pInfo; pInfo=pInfo->next) {
      if (pInfo->classMI) pInfo->classMI->ft->cleanup(pInfo->classMI, ctx);
      if (pInfo->instanceMI) pInfo->instanceMI->ft->cleanup(pInfo->instanceMI, ctx, 1);
      if (pInfo->associationMI) pInfo->associationMI->ft->cleanup(pInfo->associationMI, ctx, 1);
      if (pInfo->methodMI) pInfo->methodMI->ft->cleanup(pInfo->methodMI, ctx, 1);
      if (pInfo->indicationMI) pInfo->indicationMI->ft->cleanup(pInfo->indicationMI, ctx, 1);
      //dlclose(pInfo->library);
   }
   mlogf(M_INFO,M_SHOW,"---  stopped %s %d\n",processName,getpid());
   ctx->ft->release(ctx);
   
   uninit_sfcBroker();
   uninitProvProcCtl();
   uninitSocketPairs();
   sunsetControl();
   uninitGarbageCollector();
   
   exit(0);
} 
  

static void handleSigSegv(int sig) 
{
   Parms *threads=activeThreadsFirst;
   char msg[1024];
   BinResponseHdr *resp;
   
   mlogf(M_ERROR,M_SHOW, "-#- %s - %d provider exiting due to a SIGSEGV signal\n",
           processName, currentProc);
   while (threads) {
      snprintf(msg,1023, "*** Provider %s(%d) exiting due to a SIGSEGV signal\n",
              threads->pInfo->providerName, currentProc);
      resp = errorCharsResp(CMPI_RC_ERR_FAILED, msg);
      sendResponse(threads->requestor, resp);
      threads=threads->next;
   }        
           
}

static void handleSigUsr1(int sig)
{
   pthread_t t;
   pthread_attr_t tattr;

   stopping=1;  
   pthread_attr_init(&tattr);
   pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);      
   pthread_create(&t, &tattr, (void *(*)(void *))stopProc,NULL);
}



/* -------------
 * ---
 *      Provider Loading support
 * ---
 * -------------
 */

void initProvProcCtl(int p)
{
   int i;

   mlogf(M_INFO,M_SHOW,"--- Max provider procs: %d\n",p);
   provProcMax=p;
   provProc=(ProviderProcess*)calloc(p,sizeof(*provProc));
   for (i=0; i<p; i++) provProc[i].id=i;
}

void uninitProvProcCtl()
{
   free(provProc);
}

static pthread_mutex_t idleMtx=PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t activeMtx=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  idleCnd=PTHREAD_COND_INITIALIZER;

void* providerIdleThread()
{
   struct timespec idleTime;
   time_t next;
   int rc,val,doNotExit,noBreak=1;
   ProviderInfo *pInfo;
   ProviderProcess *proc;
   CMPIContext *ctx = NULL;
   CMPIStatus crc;

   _SFCB_ENTER(TRACE_PROVIDERDRV, "providerIdleThread");
   
   idleThreadStartHandled=1;

   for (;;) {
      idleTime.tv_sec=time(&next)+provSampleInterval;
      idleTime.tv_nsec=0;

      _SFCB_TRACE(1, ("--- providerIdleThread cycle restarted %d",currentProc));
      pthread_mutex_lock(&idleMtx);
      rc=pthread_cond_timedwait(&idleCnd,&idleMtx,&idleTime);
      if (stopping) return NULL;
      if (rc==ETIMEDOUT) {
         time_t now;
         time(&now);
         pInfo = activProvs;
         doNotExit=0;
         crc.rc=0;
         noBreak=1;
         if (pInfo) {
            proc=curProvProc;
            if (proc) {
               semAcquireUnDo(sfcbSem,(proc->id*3)+provProcGuardId+provProcBaseId);
               if ((val=semGetValue(sfcbSem,(proc->id*3)+provProcInuseId+provProcBaseId))==0) {            
                  if ((now-proc->lastActivity)>provTimeoutInterval) {
                     ctx = native_new_CMPIContext(MEM_TRACKED,NULL);
                     noBreak=0;
                     for (crc.rc=0,pInfo = activProvs; pInfo; pInfo = pInfo->next) {
                        if (pInfo->library==NULL) continue;
                        if (pInfo->indicationMI!=NULL) continue;
                        if (crc.rc==0 && pInfo->instanceMI) 
                           crc = pInfo->instanceMI->ft->cleanup(pInfo->instanceMI, ctx,0);
                        if (crc.rc==0 && pInfo->associationMI) 
                           crc = pInfo->associationMI->ft->cleanup(pInfo->associationMI, ctx,0);
                        if (crc.rc==0 && pInfo->methodMI) 
                           crc = pInfo->methodMI->ft->cleanup(pInfo->methodMI, ctx,0);
                        _SFCB_TRACE(1, ("--- Cleanup rc: %d %s-%d",crc.rc,processName,currentProc));
                        if (crc.rc==CMPI_RC_NEVER_UNLOAD) doNotExit=1;
                        if (crc.rc==CMPI_RC_DO_NOT_UNLOAD) doNotExit=noBreak=1;
                        if (crc.rc==0) {
                           _SFCB_TRACE(1, ("--- Unloading provider %s-%d",pInfo->providerName,currentProc));
                           dlclose(pInfo->library);
                           pInfo->library=NULL;
                           pInfo->instanceMI=NULL;
                           pInfo->associationMI=NULL;
                           pInfo->methodMI=NULL;
                           pInfo->initialized=0;
			   pthread_mutex_destroy(&pInfo->initMtx);
                        }   
                        else doNotExit=1;
                     }  
                     if (doNotExit==0) {
                        dumpTiming(currentProc);
                        _SFCB_TRACE(1, ("--- Exiting %s-%d",processName,currentProc));
                        exit(0);
                     }
                  }
               }
               semRelease(sfcbSem,(proc->id*3)+provProcGuardId+provProcBaseId);
            }      
         }
      }
      pthread_mutex_unlock(&idleMtx);
      if (noBreak==0) break;
   }
   _SFCB_TRACE(1, ("--- Stopping idle-monitoring due to provider request %s-%d",processName,currentProc));
   
   _SFCB_RETURN(NULL);
}


static int getInstanceMI(ProviderInfo *info, CMPIInstanceMI **mi, CMPIContext *ctx)
{
    int rc;
   _SFCB_ENTER(TRACE_PROVIDERDRV, "getInstanceMI");
   
   if (info->instanceMI == NULL) info->instanceMI =
          loadInstanceMI(info->providerName, info->library, Broker, ctx);
   *mi = info->instanceMI;
   rc = info->instanceMI ? 1 : 0;
   _SFCB_RETURN(rc);
}

static int getAssociationMI(ProviderInfo *info, CMPIAssociationMI **mi, CMPIContext *ctx)
{
   int rc;
   _SFCB_ENTER(TRACE_PROVIDERDRV, "getAssociationMI");
    
   if (info->associationMI == NULL) info->associationMI =
          loadAssociationMI(info->providerName, info->library, Broker, ctx);
   *mi = info->associationMI;
   rc = info->associationMI ? 1 : 0;
   _SFCB_RETURN(rc);
}

static int getIndicationMI(ProviderInfo *info, CMPIIndicationMI **mi, CMPIContext *ctx)
{
   int rc;
    _SFCB_ENTER(TRACE_PROVIDERDRV, "getIndicationMI");
    
   if (info->indicationMI == NULL) info->indicationMI =
          loadIndicationMI(info->providerName, info->library, Broker, ctx);
   *mi = info->indicationMI;
   rc = info->indicationMI ? 1 : 0;
   _SFCB_RETURN(rc);
}

static int getMethodMI(ProviderInfo *info, CMPIMethodMI **mi, CMPIContext *ctx)
{
   int rc;
    _SFCB_ENTER(TRACE_PROVIDERDRV, "getMethodMI");
    
   if (info->methodMI == NULL) info->methodMI =
          loadMethodMI(info->providerName, info->library, Broker, ctx);
   *mi = info->methodMI;
   rc = info->methodMI ? 1 : 0;
   _SFCB_RETURN(rc);
}

static int getClassMI(ProviderInfo *info, CMPIClassMI **mi, CMPIContext *ctx)
{
   int rc;
    _SFCB_ENTER(TRACE_PROVIDERDRV, "getClassMI");
    
   if (info->classMI == NULL) info->classMI =
          loadClassMI(info->providerName, info->library, Broker, ctx);
   *mi = info->classMI;
   rc = info->classMI ? 1 : 0;
   _SFCB_RETURN(rc);
}

static int getPropertyMI(ProviderInfo *info, CMPIPropertyMI **mi, CMPIContext *ctx)
{
   int rc;
    _SFCB_ENTER(TRACE_PROVIDERDRV, "getPropertyMI");
    
   if (info->propertyMI == NULL) info->propertyMI =
          loadPropertyMI(info->providerName, info->library, Broker, ctx);
   *mi = info->propertyMI;
   rc = info->propertyMI ? 1 : 0;
   _SFCB_RETURN(rc);
}

#ifdef HAVE_QUALREP
static int getQualifierDeclMI(ProviderInfo *info, CMPIQualifierDeclMI **mi, CMPIContext *ctx)
{
   int rc;
    _SFCB_ENTER(TRACE_PROVIDERDRV, "getQualiferMI");
    
   if (info->qualifierDeclMI == NULL) info->qualifierDeclMI =
          loadQualifierDeclMI(info->providerName, info->library, Broker, ctx);
   *mi = info->qualifierDeclMI;
   rc = info->qualifierDeclMI ? 1 : 0;
   _SFCB_RETURN(rc);
}
#endif


/* see if we're under the max allowed number of loaded providers.
   if so, fork() for the new provider
   I think this can be broken down into smaller functions
*/
static int getProcess(ProviderInfo * info, ProviderProcess ** proc)
{
   int i;
   static int seq=0;

   _SFCB_ENTER(TRACE_PROVIDERDRV, "getProcess");

   if (provAutoGroup && info->group == NULL) {
     /* implicitly put all providers in a module in a virtual group */
     info->group = strdup(info->location);
   }
   
   if (info->group) {
     for (i = 0; i < provProcMax; i++) {
       if ((provProc+i) && provProc[i].pid &&
           provProc[i].group && strcmp(provProc[i].group,info->group)==0) {
         semAcquire(sfcbSem,(provProc[i].id*3)+provProcGuardId+provProcBaseId);
         semRelease(sfcbSem,(provProc[i].id*3)+provProcInuseId+provProcBaseId);
         semRelease(sfcbSem,(provProc[i].id*3)+provProcGuardId+provProcBaseId);
         info->pid=provProc[i].pid;
         info->providerSockets=provProc[i].providerSockets;
         _SFCB_TRACE(1,("--- Process %d shared by %s and %s",provProc[i].pid,info->providerName,
			provProc[i].firstProv->providerName));
         if (provProc[i].firstProv) info->next=provProc[i].firstProv;
         else info->next = NULL;
         provProc[i].firstProv=info;
         info->proc=provProc+i;
         if (info->unload<provProc[i].unload) provProc[i].unload=info->unload;
         _SFCB_RETURN(provProc[i].pid);
       }
     }
   }

   for (i = 0; i < provProcMax; i++) {
      if (provProc[i].pid == 0) {
         *proc = provProc + i;
         providerSockets=sPairs[(*proc)->id];
  
         (*proc)->providerSockets = info->providerSockets = providerSockets;
         (*proc)->group = info->group;
         (*proc)->unload=info->unload;
         (*proc)->firstProv=info;
         info->proc = *proc;
         info->next = NULL;

         (*proc)->pid = info->pid = fork();

         if (info->pid < 0) {
            perror("provider fork");
            _SFCB_ABORT();
         }

         if (info->pid == 0) {

            currentProc=getpid();
            setSignal(SIGCHLD, SIG_DFL,0);
            setSignal(SIGTERM, SIG_IGN,0);
            setSignal(SIGHUP, SIG_IGN,0);
            setSignal(SIGUSR1, handleSigUsr1,0);
            
            setSignal(SIGSEGV, handleSigSegv,SA_ONESHOT);
            
            curProvProc=(*proc);
            resultSockets=sPairs[(*proc)->id+ptBase];

            _SFCB_TRACE(1,("--- Forked started for %s %d %d-%lu",
                         info->providerName, currentProc,providerSockets.receive,
                         getInode(providerSockets.receive)));
            processName=info->providerName;
            providerProcess=1;
            info->proc=*proc;
            info->pid=currentProc; 
                         
            semSetValue(sfcbSem,((*proc)->id*3)+provProcGuardId+provProcBaseId,0);
            semSetValue(sfcbSem,((*proc)->id*3)+provProcInuseId+provProcBaseId,0);
            semSetValue(sfcbSem,((*proc)->id*3)+provProcAliveId+provProcBaseId,0);
            semReleaseUnDo(sfcbSem,((*proc)->id*3)+provProcAliveId+provProcBaseId);
            semReleaseUnDo(sfcbSem,((*proc)->id*3)+provProcInuseId+provProcBaseId);
            semRelease(sfcbSem,((*proc)->id*3)+provProcGuardId+provProcBaseId);
            
            processProviderInvocationRequests(info->providerName);
            _SFCB_RETURN(0);
         }

         else {
           info->startSeq=++seq;
         }
         _SFCB_TRACE(1,("--- Fork provider OK %s %d %d", info->providerName,
                      info->pid, i));
         _SFCB_RETURN(info->pid);
      }
   }

   *proc = NULL;
   _SFCB_RETURN(-1);
}

// I think we should break this function into two subfunctions:
// something like isLoaded() and doForkProvider()
int forkProvider(ProviderInfo * info, OperationHdr * req, char **msg)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "forkProvider");
   ProviderProcess *proc;
   ProviderInfo * pInfo;
   int val,rc;

   if (info->pid ) {
      proc=info->proc;
      semAcquire(sfcbSem,(proc->id*3)+provProcGuardId+provProcBaseId);
      if ((val=semGetValue(sfcbSem,(proc->id*3)+provProcAliveId+provProcBaseId))) {
         semRelease(sfcbSem,(proc->id*3)+provProcInuseId+provProcBaseId);
         semRelease(sfcbSem,(proc->id*3)+provProcGuardId+provProcBaseId);
         _SFCB_TRACE(1, ("--- Provider %s still loaded",info->providerName));
         _SFCB_RETURN(CMPI_RC_OK)
      }

      semRelease(sfcbSem,(proc->id*3)+provProcGuardId+provProcBaseId);
      _SFCB_TRACE(1, ("--- Provider has been unloaded prevously, will reload"));
      
      info->pid=0;
      for (pInfo=proc->firstProv; pInfo; pInfo=pInfo->next) {
         pInfo->pid=0;
      }      
      proc->firstProv=NULL;            
      proc->pid=0; 
      proc->group=NULL; 
   }

   _SFCB_TRACE(1, ("--- Forking provider for %s", info->providerName));

   if (getProcess(info, &proc) > 0) {

      LoadProviderReq sreq = BINREQ(OPS_LoadProvider, 3);

      BinRequestContext binCtx;
      BinResponseHdr *resp;

      memset(&binCtx,0,sizeof(BinRequestContext));
      sreq.className = setCharsMsgSegment(info->className);
      sreq.libName = setCharsMsgSegment(info->location);
      sreq.provName = setCharsMsgSegment(info->providerName);
      sreq.hdr.flags = info->type;
      sreq.unload = info->unload;
      sreq.hdr.provId = getProvIds(info).ids;

      if (req) binCtx.oHdr = (OperationHdr *) req;
      binCtx.bHdr = &sreq.hdr;
      binCtx.bHdrSize = sizeof(sreq);
      binCtx.provA.socket = info->providerSockets.send;
      binCtx.provA.ids = getProvIds(info);
      binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;

      _SFCB_TRACE(1, ("--- Invoke loader"));

      resp = invokeProvider(&binCtx);
      resp->rc--;
      if (resp->rc) {
         *msg = strdup((char *) resp->object[0].data);
      }
      else *msg = NULL;

      rc=resp->rc;
      _SFCB_TRACE(1, ("--- rc: %d", resp->rc));
      
      free(resp);
      _SFCB_RETURN(rc);
   }
   _SFCB_RETURN(CMPI_RC_ERR_FAILED);
}

typedef struct provHandler {
   BinResponseHdr *(*handler) (BinRequestHdr *, ProviderInfo * info, int requestor);
} ProvHandler;


static int sendResponse(int requestor, BinResponseHdr * hdr)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "sendResponse");
   int i, rvl=0, ol, size, dmy=-1;
   long l;
   char str_time[26];
   BinResponseHdr *buf;
   
   size = sizeof(BinResponseHdr) + ((hdr->count - 1) * sizeof(MsgSegment));

   if (hdr->rvValue) {
      switch(hdr->rv.type) {
      case CMPI_string:
         if (hdr->rv.value.string) {
            if (hdr->rv.value.string->hdl) {
               hdr->rv.value.string= hdr->rv.value.string->hdl; 
            }
            else hdr->rv.value.string=NULL;
         }
         hdr->rv.type=CMPI_chars;
      case CMPI_chars:
         hdr->rvEnc=setCharsMsgSegment((char*)hdr->rv.value.string);
         rvl=hdr->rvEnc.length;
         break;
      case CMPI_dateTime:
         dateTime2chars(hdr->rv.value.dateTime, NULL, str_time);
         hdr->rvEnc.type=MSG_SEG_CHARS;
         hdr->rvEnc.length=rvl=26;
         hdr->rvEnc.data=&str_time;
         break;      
      case CMPI_ref:
         mlogf(M_ERROR,M_SHOW,"-#- not supporting refs\n");
         abort();
      default: ;
      }
   }
   
   for (l = size, i = 0; i < hdr->count; i++) {
     /* add padding length to calculation */
     l += (hdr->object[i].type == MSG_SEG_CHARS ? PADDED_LEN(hdr->object[i].length) : hdr->object[i].length);
   }

   buf = (BinResponseHdr *) malloc(l +rvl + 8);
   memcpy(buf, hdr, size);

   if (rvl) {
      ol = hdr->rvEnc.length;
      l=size;
      switch (hdr->rvEnc.type) {
      case MSG_SEG_CHARS:
         memcpy(((char *) buf) + l, hdr->rvEnc.data, ol);
         buf->rvEnc.data = (void *) l;
         l += ol;
         break;
      } 
      size=l;
   }
   
   for (l = size, i = 0; i < hdr->count; i++) {
      ol = hdr->object[i].length;
      switch (hdr->object[i].type) {
      case MSG_SEG_OBJECTPATH:
         getSerializedObjectPath((CMPIObjectPath *) hdr->object[i].data,
                                 ((char *) buf) + l);
         buf->object[i].data = (void *) l;
         l += ol;
         break;
      case MSG_SEG_INSTANCE:
         getSerializedInstance((CMPIInstance *) hdr->object[i].data,
                               ((char *) buf) + l);
         buf->object[i].data = (void *) l;
         l += ol;
         break;
      case MSG_SEG_CHARS:
         memcpy(((char *) buf) + l, hdr->object[i].data, ol);
         buf->object[i].data = (void *) l;
         buf->object[i].length = PADDED_LEN(ol);
         l += buf->object[i].length;
         break;
      case MSG_SEG_CONSTCLASS:
         getSerializedConstClass((CMPIConstClass *) hdr->object[i].data,
                                 ((char *) buf) + l);
         buf->object[i].data = (void *) l;
         l += ol;
         break;
      case MSG_SEG_ARGS:
         getSerializedArgs((CMPIArgs *) hdr->object[i].data,
                           ((char *) buf) + l);
         buf->object[i].data = (void *) l;
         l += ol;
         break;
#ifdef HAVE_QUALREP         
      case MSG_SEG_QUALIFIER:
         getSerializedQualifier((CMPIQualifierDecl *) hdr->object[i].data,
                                 ((char *) buf) + l);
         buf->object[i].data = (void *) l;
         l += ol;
         break;
#endif
      default:
         mlogf(M_ERROR,M_SHOW,"--- bad sendResponse request %d\n", hdr->object[i].type);
         _SFCB_ABORT();
      }
   }
   
   _SFCB_TRACE(1, ("--- Sending result to %d-%lu",
                   requestor,getInode(requestor)));

   spSendResult(&requestor, &dmy, buf, l);
   free(buf);
   _SFCB_RETURN(0);
}


int sendResponseChunk(CMPIArray *r,int requestor, CMPIType type)
{
   int i, count;
   BinResponseHdr *resp;
      
   _SFCB_ENTER(TRACE_PROVIDERDRV, "sendResponseChunk");
   
   count = CMGetArrayCount(r, NULL);
   resp = (BinResponseHdr *) 
      calloc(1,sizeof(BinResponseHdr) +((count - 1) * sizeof(MsgSegment)));
      
   resp->moreChunks=1;
   resp->rc = 1;
   resp->count = count;
   for (i = 0; i < count; i++)
      if (type==CMPI_instance) resp->object[i] =
          setInstanceMsgSegment(CMGetArrayElementAt(r, i, NULL).value.inst);
      else resp->object[i] =
          setObjectPathMsgSegment(CMGetArrayElementAt(r, i, NULL).value.ref);

   _SFCB_RETURN(sendResponse(requestor, resp));
}

static BinResponseHdr *errorResp(CMPIStatus * rci)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "errorResp");
   BinResponseHdr *resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
   resp->moreChunks=0;
   resp->rc = rci->rc + 1;
   resp->count = 1;
   resp->object[0] = setCharsMsgSegment(rci->msg ? (char *) rci->msg->hdl : "");
   _SFCB_RETURN(resp)
}

static BinResponseHdr *errorCharsResp(int rc, char *msg)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "errorCharsResp");
   BinResponseHdr *resp =
       (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) + strlen(msg) + 4);
   strcpy((char *) (resp + 1), msg ? msg : "");
   resp->rc = rc + 1;
   resp->count = 1;
   resp->object[0] = setCharsMsgSegment((char *) (resp + 1));
   _SFCB_RETURN(resp);
}

char **makePropertyList(int n, MsgSegment *ms)
{
   char **l;
   int i;

   //if (n==1 && ms[0].data==NULL) return NULL;
   l=(char**)malloc(sizeof(char*)*(n+1));

   for (i=0; i<n; i++)
      l[i]=(char*)ms[i].data;
   l[n]=NULL;
   return l;
}

static BinResponseHdr *deleteClass(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "deleteClass");
   TIMING_PREP
   DeleteClassReq *req = (DeleteClassReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci =  info->classMI->ft->deleteClass(info->classMI, ctx, result,path);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      resp->count = 0;
      resp->moreChunks=0;
      resp->rc = 1;
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *getClass(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   TIMING_PREP
   GetClassReq *req = (GetClassReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   char **props=NULL;
   int i;
   
   _SFCB_ENTER(TRACE_PROVIDERDRV, "getClass");
   
    char *cn=CMGetClassName(path,NULL)->hdl; 	 
    char *ns=CMGetNameSpace(path,NULL)->hdl; 	 
   _SFCB_TRACE(1, ("--- Namespace %s ClassName %s",ns,cn));
   
   if (req->hdr.flags & FL_localOnly) flgs|=CMPI_FLAG_LocalOnly;
   if (req->hdr.flags & FL_includeQualifiers) flgs|=CMPI_FLAG_IncludeQualifiers;
   if (req->hdr.flags & FL_includeClassOrigin) flgs|=CMPI_FLAG_IncludeClassOrigin;
   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>2) props=makePropertyList(req->hdr.count-2,req->properties);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->classMI->ft->getClass(info->classMI, ctx, result, path,props);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   r = native_result2array(result);
   if (rci.rc == CMPI_RC_OK) {
      count = 1;
      resp = (BinResponseHdr *) calloc(1, sizeof(BinResponseHdr) +
                                    ((count - 1) * sizeof(MsgSegment)));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = count;
      for (i = 0; i < count; i++) resp->object[i] = 
         setConstClassMsgSegment(CMGetArrayElementAt(r, i, NULL).value.dataPtr.ptr);
   }
   else resp = errorResp(&rci);
   if (props) free(props);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *createClass(BinRequestHdr * hdr, ProviderInfo * info, 
               int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "createClass");
   TIMING_PREP
   CreateClassReq *req = (CreateClassReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIConstClass *cls = relocateSerializedConstClass(req->cls.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->classMI->ft->createClass(info->classMI, ctx, result, path, cls);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      resp->count = 0;
      resp->moreChunks=0;
      resp->rc = 1;
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *enumClassNames(BinRequestHdr * hdr,
                                         ProviderInfo * info, int requestor)
{
   TIMING_PREP
   EnumClassNamesReq *req = (EnumClassNamesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   BinResponseHdr *resp;
   CMPIFlags flgs=req->hdr.flags;
   int i;
   
   _SFCB_ENTER(TRACE_PROVIDERDRV, "enumClassNames");

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));

//   rci = info->instanceMI->ft->enumInstanceNames(info->instanceMI, ctx, result,
   TIMING_START(hdr,info)
   rci = info->classMI->ft->enumClassNames(info->classMI, ctx, result,
                                               path);
   TIMING_STOP(hdr,info)
   r = native_result2array(result);

   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      if (r) count = CMGetArrayCount(r, NULL);
      else count=0;
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) +
				       ((count?count - 1:0) * sizeof(MsgSegment)));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = count;
      for (i = 0; i < count; i++) resp->object[i] =
         setObjectPathMsgSegment(CMGetArrayElementAt(r, i, NULL).value.ref);
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *enumClasses(BinRequestHdr * hdr,
                                         ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "enumClasses");
   TIMING_PREP
   EnumClassesReq *req = (EnumClassesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=req->hdr.flags;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));

   TIMING_START(hdr,info)
   rci = info->classMI->ft->enumClasses(info->classMI, ctx, result,path);
   TIMING_STOP(hdr,info)
   r = native_result2array(result);

   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);
    
   _SFCB_RETURN(resp);
}

#ifdef HAVE_QUALREP

static BinResponseHdr *enumQualifiers(BinRequestHdr * hdr,
                                         ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "enumQualifiers");
   TIMING_PREP
   EnumQualifiersReq *req = (EnumQualifiersReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPICount count;
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=req->hdr.flags;
   int i;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));

   TIMING_START(hdr,info)
   rci = info->qualifierDeclMI->ft->enumQualifiers(info->qualifierDeclMI, ctx, result,path);
   TIMING_STOP(hdr,info)
   r = native_result2array(result);

   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      if (r) count = CMGetArrayCount(r, NULL);
      else count=0;
      resp = (BinResponseHdr *) calloc(1, sizeof(BinResponseHdr) +
				       ((count?count - 1:0) * sizeof(MsgSegment)));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = count;
      for (i = 0; i < count; i++) {
      	resp->object[i] = setQualifierMsgSegment(CMGetArrayElementAt(r, i, NULL).value.dataPtr.ptr);
      }    
   }
   else resp = errorResp(&rci);
    
   _SFCB_RETURN(resp);
}

static BinResponseHdr *setQualifier(BinRequestHdr * hdr, ProviderInfo * info, 
               int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "setQualifier");
   TIMING_PREP
   SetQualifierReq *req = (SetQualifierReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIQualifierDecl *q = relocateSerializedQualifier(req->qualifier.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->qualifierDeclMI->ft->setQualifier(info->qualifierDeclMI, ctx, result, path, q);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      resp->count = 0;
      resp->moreChunks=0;
      resp->rc = 1;
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *getQualifier(BinRequestHdr * hdr, ProviderInfo * info, 
   int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "getQualifier");
   TIMING_PREP
   GetQualifierReq *req = (GetQualifierReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   BinResponseHdr *resp;
   int i;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->qualifierDeclMI->ft->getQualifier(info->qualifierDeclMI, ctx, result, path);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   r = native_result2array(result);

   if (rci.rc == CMPI_RC_OK) {
      count = 1;
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) +
                                    ((count - 1) * sizeof(MsgSegment)));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = count;
      for (i = 0; i < count; i++)
         resp->object[i] = setQualifierMsgSegment(CMGetArrayElementAt(r, i, NULL).value.dataPtr.ptr);
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *deleteQualifier(BinRequestHdr * hdr, ProviderInfo * info, 
   int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "deleteQualifier");
   TIMING_PREP
   DeleteQualifierReq *req = (DeleteQualifierReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->qualifierDeclMI->ft->deleteQualifier(info->qualifierDeclMI, ctx, result, path);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      resp->count = 0;
      resp->moreChunks=0;
      resp->rc = 1;
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}
#endif

static BinResponseHdr *getProperty(BinRequestHdr * hdr, ProviderInfo * info, 
   int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "getProperty");
   TIMING_PREP
   GetPropertyReq *req = (GetPropertyReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   CMPIData data;
   CMPIInstance *inst = internal_new_CMPIInstance(MEM_TRACKED, NULL, NULL, 1);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (info->propertyMI == NULL) info->propertyMI =
                 loadPropertyMI(info->providerName, info->library, Broker, ctx);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->propertyMI->ft->getProperty(info->propertyMI, ctx, result, path, (const char*)req->name.data);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   r = native_result2array(result);

   if (rci.rc == CMPI_RC_OK) {
      count = 1;
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) +
                                    ((count - 1) * sizeof(MsgSegment)));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = count;
      
      data = CMGetArrayElementAt(r, 0, NULL);
      inst->ft->setProperty(inst, (const char*)req->name.data, &data.value, data.type);
      resp->object[0] = setInstanceMsgSegment(inst);
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *setProperty(BinRequestHdr * hdr, ProviderInfo * info, 
               int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "setProperty");
   TIMING_PREP
   SetPropertyReq *req = (SetPropertyReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIInstance *inst = relocateSerializedInstance(req->inst.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPIString *pName;
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   CMPIData data;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);
   
   data = inst->ft->getPropertyAt(inst, 0, &pName, NULL);

   if (info->propertyMI == NULL) info->propertyMI =
                 loadPropertyMI(info->providerName, info->library, Broker, ctx);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->propertyMI->ft->setProperty(info->propertyMI, ctx, result, path, (const char*)pName->hdl, data);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      resp->count = 0;
      resp->moreChunks=0;
      resp->rc = 1;
   }
   else resp = errorResp(&rci);
   
   CMRelease(pName);
   
   _SFCB_RETURN(resp);
}

static BinResponseHdr *invokeMethod(BinRequestHdr * hdr, ProviderInfo * info, 
   int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "invokeMethod");
   TIMING_PREP
   InvokeMethodReq *req = (InvokeMethodReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   char *method = (char *) req->method.data;
   CMPIArgs *in,*tIn = relocateSerializedArgs(req->in.data);
   CMPIArgs *out = TrackedCMPIArgs(NULL);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>5) {
      int i,s,n;
      CMPIString *name;
      in=CMNewArgs(Broker,NULL);
      BinRequestHdr *r=(BinRequestHdr*)req;
      for (n=5,i=0,s=CMGetArgCount(tIn,NULL); i<s; i++) {
         CMPIData d=CMGetArgAt(tIn,i,&name,NULL);
         if (d.type==CMPI_instance) {
            d.value.inst=relocateSerializedInstance(r->object[n++].data);
         }
         CMAddArg(in,(char*)name->hdl,&d.value,d.type);
      }
   }
   else in=tIn;
   
   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->methodMI->ft->invokeMethod
                          (info->methodMI, ctx, result, path, method, in, out);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   r = native_result2array(result);
   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      memset(&resp->rv,0,sizeof(resp->rv));
      if (r) {
         count = CMGetArrayCount(r, NULL);
         resp->rvValue=1;
         if (count) {
            resp->rv=CMGetArrayElementAt(r, 0, NULL);
         }
      }  
          
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = 1;
      resp->object[0] = setArgsMsgSegment(out);
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *getInstance(BinRequestHdr * hdr, ProviderInfo * info, 
   int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "getInstance");
   TIMING_PREP
   GetInstanceReq *req = (GetInstanceReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIArray *r;
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   char **props=NULL;
   int i;

   if (req->hdr.flags & FL_localOnly) flgs|=CMPI_FLAG_LocalOnly;
   if (req->hdr.flags & FL_includeQualifiers) flgs|=CMPI_FLAG_IncludeQualifiers;
   if (req->hdr.flags & FL_includeClassOrigin) flgs|=CMPI_FLAG_IncludeClassOrigin;
   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>2) props=makePropertyList(req->hdr.count-2,req->properties);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->instanceMI->ft->getInstance(info->instanceMI, ctx, result, path, (const char**)props);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   r = native_result2array(result);

   if (rci.rc == CMPI_RC_OK) {
      if (r && CMGetArrayCount(r, NULL) > 0) {
         count = 1;

         resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) +
                                       ((count - 1) * sizeof(MsgSegment)));
         resp->moreChunks=0;
         resp->rc = 1;
         resp->count = count;
         for (i = 0; i < count; i++)
            resp->object[i] =
              setInstanceMsgSegment(CMGetArrayElementAt(r, i, NULL).value.inst);
      } else {
         rci.rc = CMPI_RC_ERR_NOT_FOUND;
         rci.msg = NULL;
         resp = errorResp(&rci);
      }
   }
   else resp = errorResp(&rci);
   if (props) free(props);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *deleteInstance(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "deleteInstance");
   TIMING_PREP
   DeleteInstanceReq *req = (DeleteInstanceReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci =  info->instanceMI->ft->deleteInstance(info->instanceMI, ctx, result,
                                            path);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
      resp->count = 0;
      resp->moreChunks=0;
      resp->rc = 1;
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *createInstance(BinRequestHdr * hdr, ProviderInfo * info, 
               int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "createInstance");
   TIMING_PREP
   CreateInstanceReq *req = (CreateInstanceReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIInstance *inst = relocateSerializedInstance(req->instance.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPIArray *r;
   CMPICount count;
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   int i;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->instanceMI->ft->createInstance(info->instanceMI, ctx, result,
                                            path, inst);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));
   r = native_result2array(result);

   if (rci.rc == CMPI_RC_OK) {
      count = 1;
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) +
                                    ((count - 1) * sizeof(MsgSegment)));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = count;
      for (i = 0; i < count; i++)
         resp->object[i] =
             setObjectPathMsgSegment(CMGetArrayElementAt(r, i, NULL).value.ref);
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *modifyInstance(BinRequestHdr * hdr, ProviderInfo * info, 
              int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "modifyInstance");
   TIMING_PREP
   ModifyInstanceReq *req = (ModifyInstanceReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->path.data);
   CMPIInstance *inst = relocateSerializedInstance(req->instance.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPICount count;
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   char **props=NULL;

   if (req->hdr.flags & FL_includeQualifiers) flgs|=CMPI_FLAG_IncludeQualifiers;
   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>3) props=makePropertyList(req->hdr.count-3,req->properties);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->instanceMI->ft->modifyInstance(info->instanceMI, ctx, result,
                                            path, inst, (const char **)props);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      count = 1;
      resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr) - sizeof(MsgSegment));
      resp->moreChunks=0;
      resp->rc = 1;
      resp->count = 0;
   }
   else resp = errorResp(&rci);
   if (props) free(props);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *enumInstances(BinRequestHdr * hdr, ProviderInfo * info, 
      int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "enumInstances");
   TIMING_PREP
   EnumInstancesReq *req = (EnumInstancesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   char **props=NULL;

#ifndef HAVE_OPTIMIZED_ENUMERATION
   if (info->className && info->className[0] != '$') {
     /* not a special provider, perform class name substitution if call is for a
        parent class of the class the provider is registered for */
     char * classname = CMGetCharPtr(CMGetClassName(path,NULL));
     char * namespace = CMGetCharPtr(CMGetNameSpace(path,NULL));
     if (classname && namespace && strcasecmp(info->className,classname)) {
       CMPIObjectPath * provPath = CMNewObjectPath(Broker,namespace,info->className,NULL);
       if (provPath && CMClassPathIsA(Broker,provPath,classname,NULL)) {
	 _SFCB_TRACE(1, ("--- Replacing class name %s",info->className));
	 path = provPath;
       }
     }
   }
#endif

   if (req->hdr.flags & FL_localOnly) flgs|=CMPI_FLAG_LocalOnly;
   if (req->hdr.flags & FL_deepInheritance) flgs|=CMPI_FLAG_DeepInheritance;
   if (req->hdr.flags & FL_includeQualifiers) flgs|=CMPI_FLAG_IncludeQualifiers;
   if (req->hdr.flags & FL_includeClassOrigin) flgs|=CMPI_FLAG_IncludeClassOrigin;
   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>2) props=makePropertyList(req->hdr.count-2,req->properties);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->instanceMI->ft->enumerateInstances(info->instanceMI, ctx, result, path, (const char**)props);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);
   if (props) free(props);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *enumInstanceNames(BinRequestHdr * hdr,
                                         ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "enumInstanceNames");
   TIMING_PREP
   EnumInstanceNamesReq *req = (EnumInstanceNamesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

#ifndef HAVE_OPTIMIZED_ENUMERATION
   if (info->className && info->className[0] != '$') {
     /* not a special provider, perform class name substitution if call is for a
        parent class of the class the provider is registered for */
     char * classname = CMGetCharPtr(CMGetClassName(path,NULL));
     char * namespace = CMGetCharPtr(CMGetNameSpace(path,NULL));
     if (classname && namespace && strcasecmp(info->className,classname)) {
       CMPIObjectPath * provPath = CMNewObjectPath(Broker,namespace,info->className,NULL);
       if (provPath && CMClassPathIsA(Broker,provPath,classname,NULL)) {
	 _SFCB_TRACE(1, ("--- Replacing class name %s",info->className));
	 path = provPath;
       }
     }
   }
#endif

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->instanceMI->ft->enumerateInstanceNames(info->instanceMI, ctx, result,
                                               path);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}

CMPIValue queryGetValue(QLPropertySource* src, char* name, QLOpd *type)
{
   CMPIInstance *ci=(CMPIInstance*)src->data;
   CMPIStatus rc;
   CMPIData d=ci->ft->getProperty(ci,name,&rc);
   CMPIValue v={(long long)0};
   
   if (rc.rc==CMPI_RC_OK) {
      if (d.type & CMPI_SINT) { 
         if (d.type==CMPI_sint32) v.sint64=d.value.sint32;
         else if (d.type==CMPI_sint16) v.sint64=d.value.sint16;
         else if (d.type==CMPI_sint8) v.sint64=d.value.sint8;
         else v.sint64=d.value.sint64;
         *type=QL_Integer;
      }
      else if (d.type & CMPI_UINT) { 
         if (d.type==CMPI_uint32) v.uint64=d.value.uint32;
         else if (d.type==CMPI_uint16) v.uint64=d.value.uint16;
         else if (d.type==CMPI_uint8) v.uint64=d.value.uint8;
         else v.uint64=d.value.uint64;
         *type=QL_UInteger;
      }
    
      else switch(d.type) {
      case CMPI_string:
         *type=QL_Chars;
         v.chars=(char*)d.value.string->hdl;
         break;
      case CMPI_boolean:
         *type=QL_Boolean;
         v.boolean=d.value.boolean;
         break;
      case CMPI_real64:
         *type=QL_Double;
         v.real64=d.value.real64;
         break;
      case CMPI_real32:
         *type=QL_Double;
         v.real64=d.value.real32;
         break;
      case CMPI_char16:
         *type=QL_Char;
         v.char16=d.value.char16;
         break;
      case CMPI_instance:
         *type=QL_Inst;
         v.inst=d.value.inst;
         break;
      default:
         *type=QL_Invalid;
      }
   }
   else *type=QL_NotFound;
   return v;
}

static BinResponseHdr *execQuery(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "execQuery");
   TIMING_PREP
   ExecQueryReq *req = (ExecQueryReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   int irc;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->instanceMI->ft->execQuery(info->instanceMI, ctx, result, path,
            PROVCHARS(req->query.data), PROVCHARS(req->queryLang.data));
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc==CMPI_RC_ERR_NOT_SUPPORTED) {
      QLStatement *qs;  
      CMPIArray *kar;
      CMPICount i,c;
      int ok=1;
 
      qs=parseQuery(MEM_TRACKED,(char*)req->query.data,(char*)req->queryLang.data,NULL,&irc);   
      if (irc) {
         rci.rc=CMPI_RC_ERR_INVALID_QUERY;
         resp = errorResp(&rci);
         _SFCB_RETURN(resp);
      }

      qs->propSrc.getValue=queryGetValue;
      qs->propSrc.sns=qs->sns;
      //      qs->cop=CMNewObjectPath(Broker,"*",qs->fClasses[0],NULL);
      qs->cop=path;

      if (qs->allProps) {	
         CMPIConstClass *cc = getConstClass(CMGetNameSpace(qs->cop,NULL)->hdl,
           CMGetClassName(qs->cop,NULL)->hdl);
         kar=cc->ft->getKeyList(cc);
      }
      else {
         kar=getKeyListAndVerifyPropertyList(qs->cop, qs->spNames, &ok, NULL);
      }

      if (ok) {
         c=kar->ft->getSize(kar,NULL);
         qs->keys=(char**)malloc((c+1)*sizeof(char*));

         for (i=0; i<c; i++)
            qs->keys[i]=(char*)kar->ft->getElementAt(kar,i,NULL).value.string->hdl;
         qs->keys[c]=NULL;

         setResultQueryFilter(result,qs);      
         _SFCB_TRACE(1, ("--- Calling enumerateInstances provider %s",info->providerName));
	 TIMING_START(hdr,info)
         rci = info->instanceMI->ft->enumerateInstances(info->instanceMI, ctx, result,
                      path,NULL); 
	 TIMING_STOP(hdr,info)
         _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));
         free(qs->keys);
      }
      else rci.rc=CMPI_RC_OK;

      kar->ft->release(kar);
      qs->ft->release(qs);
   }

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}


static BinResponseHdr *associators(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "associators");
   TIMING_PREP
   AssociatorsReq *req = (AssociatorsReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   char **props=NULL;

   if (req->hdr.flags & FL_includeQualifiers) flgs|=CMPI_FLAG_IncludeQualifiers;
   if (req->hdr.flags & FL_includeClassOrigin) flgs|=CMPI_FLAG_IncludeClassOrigin;
   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>6) props=makePropertyList(req->hdr.count-6,req->properties);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->associationMI->ft->associators(info->associationMI, ctx, result,
                                            path,
                                            PROVCHARS(req->assocClass.data),
                                            PROVCHARS(req->resultClass.data),
                                            PROVCHARS(req->role.data),
                                            PROVCHARS(req->resultRole.data),
                                            (const char**)props);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);
   if (props) free(props);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *references(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "references");
   TIMING_PREP
   ReferencesReq *req = (ReferencesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;
   char **props=NULL;

   if (req->hdr.flags & FL_includeQualifiers) flgs|=CMPI_FLAG_IncludeQualifiers;
   if (req->hdr.flags & FL_includeClassOrigin) flgs|=CMPI_FLAG_IncludeClassOrigin;
   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   if (req->hdr.count>4) props=makePropertyList(req->hdr.count-4,req->properties);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->associationMI->ft->references(info->associationMI, ctx, result,
                                           path,
                                           PROVCHARS(req->resultClass.data),
                                           PROVCHARS(req->role.data),
                                           (const char**)props);
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);
   if (props) free(props);

   _SFCB_RETURN(resp);
}

static BinResponseHdr *associatorNames(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "associatorNames");
   TIMING_PREP
   AssociatorNamesReq *req = (AssociatorNamesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->associationMI->ft->associatorNames(info->associationMI, ctx,
                                            result, path,
                                            PROVCHARS(req->assocClass.data),
                                            PROVCHARS(req->resultClass.data),
                                            PROVCHARS(req->role.data),
                                            PROVCHARS(req->resultRole.data));
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}


static BinResponseHdr *referenceNames(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "referenceNames");
   TIMING_PREP
   ReferenceNamesReq *req = (ReferenceNamesReq *) hdr;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   CMPIResult *result = native_new_CMPIResult(requestor<0 ? 0 : requestor,0,NULL);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   BinResponseHdr *resp;
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1, ("--- Calling provider %s",info->providerName));
   TIMING_START(hdr,info)
   rci = info->associationMI->ft->referenceNames(info->associationMI, ctx,
                                               result, path,
                                               PROVCHARS(req->resultClass.data),
                                               PROVCHARS(req->role.data));
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc == CMPI_RC_OK) {
      xferLastResultBuffer(result,abs(requestor),1);
      return NULL;       
   }
   else resp = errorResp(&rci);

   _SFCB_RETURN(resp);
}




#ifdef SFCB_INCL_INDICATION_SUPPORT   

static BinResponseHdr *activateFilter(BinRequestHdr *hdr, ProviderInfo* info, 
             int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV|TRACE_INDPROVIDER, "activateFilter");
   TIMING_PREP
   
   IndicationReq *req = (IndicationReq*)hdr;
   BinResponseHdr *resp=NULL;
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   NativeSelectExp *se=NULL,*prev=NULL;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIFlags flgs=0;
   int makeActive=0;
   char *type = NULL;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   _SFCB_TRACE(1,("--- pid: %d activFilters %p %s",currentProc,activFilters,processName));
   for (se = activFilters; se; se = se->next) {
     if (se->filterId == req->filterId) break;
   }

   _SFCB_TRACE(1,("--- selExp found: %p",se));
   if (se==NULL) {
      char *query=(char*)req->query.data;
      char *lang=(char*)req->language.data;
      type=(char*)req->type.data;
      char *sns=(char*)req->sns.data;
      se=(NativeSelectExp*)NewCMPISelectExp(query,lang,sns,NULL,&rci);
      makeActive=1;
      se->filterId=req->filterId;
      prev=se->next=activFilters;
      activFilters=se;
      _SFCB_TRACE(1,("--- new selExp:  %p",se));
   } 

   if (info->indicationMI==NULL) {
      CMPIStatus st;
      setStatus(&st,CMPI_RC_ERR_NOT_SUPPORTED,"Provider does not support indications");
      resp = errorResp(&st);
      _SFCB_RETURN(resp);  
   }

   _SFCB_TRACE(1, ("--- Calling authorizeFilter %s",info->providerName));
   TIMING_START(hdr,info)
   if (info->indicationMI->ft->ftVersion < 100) {
     authorizeFilterPreV1 fptr = 
       (authorizeFilterPreV1)info->indicationMI->ft->authorizeFilter;
     rci = fptr(info->indicationMI, ctx, result,
		(CMPISelectExp*)se, type, path,
		PROVCHARS(req->principal.data));
   } else {
     rci = info->indicationMI->ft->authorizeFilter(info->indicationMI, ctx,
						   (CMPISelectExp*)se, type, path,
						   PROVCHARS(req->principal.data));
   }
   TIMING_STOP(hdr,info)
   _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

   if (rci.rc==CMPI_RC_OK) {
      _SFCB_TRACE(1, ("--- Calling mustPoll %s",info->providerName));
      TIMING_START(hdr,info)
      if (info->indicationMI->ft->ftVersion < 100) {
	mustPollPreV1 fptr =
	  (mustPollPreV1)info->indicationMI->ft->mustPoll;
	rci = fptr(info->indicationMI, ctx, result,
		   (CMPISelectExp*)se, type, path);
      } else {
	rci = info->indicationMI->ft->mustPoll(info->indicationMI, ctx, 
                                               (CMPISelectExp*)se, type, path);
      }
      TIMING_STOP(hdr,info)
      _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

      _SFCB_TRACE(1, ("--- Calling activateFilter %s",info->providerName));
      TIMING_START(hdr,info)
      if (info->indicationMI->ft->ftVersion < 100) {
	activateFilterPreV1 fptr =
	  (activateFilterPreV1) info->indicationMI->ft->activateFilter;
	rci = fptr(info->indicationMI, ctx, result,
	       (CMPISelectExp*)se, type, path, 1);
      } else {
	rci = info->indicationMI->ft->activateFilter(info->indicationMI, ctx,
						     (CMPISelectExp*)se, type, path, 1);
      }
      TIMING_STOP(hdr,info)
      _SFCB_TRACE(1, ("--- Back from provider rc: %d", rci.rc));

      /*if (indicationEnabled==0 && rci.rc==CMPI_RC_OK) {
      indicationEnabled=1;
	TIMING_START(hdr,info)
      info->indicationMI->ft->enableIndications(info->indicationMI,ctx);
	TIMING_STOP(hdr,info)
      }*/

      if (rci.rc==CMPI_RC_OK) {
         resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
         resp->rc=1;
      }
   }

   if (rci.rc!=CMPI_RC_OK) {
      activFilters=prev;
      resp = errorResp(&rci);
      _SFCB_TRACE(1, ("--- Not OK rc: %d", rci.rc));
   }
   else {
       _SFCB_TRACE(1, ("--- OK activFilters: %p",activFilters));
   }
   _SFCB_TRACE(1, ("---  pid: %d activFilters %p",currentProc,activFilters));

   _SFCB_RETURN(resp);
}

static BinResponseHdr *deactivateFilter(BinRequestHdr *hdr, ProviderInfo* info, 
             int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV|TRACE_INDPROVIDER, "deactivateFilter");
   TIMING_PREP

   IndicationReq *req = (IndicationReq*)hdr;
   BinResponseHdr *resp=NULL;
   CMPIStatus rci = { CMPI_RC_OK, NULL };
   NativeSelectExp *se=NULL,**sef=&activFilters;
   CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   CMPIResult *result = native_new_CMPIResult(0,1,NULL);
   CMPIFlags flgs=0;

   ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
   ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
   ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);

   resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
   resp->rc=1;

   _SFCB_TRACE(1, ("---  pid: %d activFilters %p",currentProc,activFilters));
   if (activFilters==NULL) _SFCB_RETURN(resp); 

   for (se = activFilters; se; se = se->next) {
      if (se->filterId == req->filterId) {
         *sef=se->next;
         if (activFilters==NULL) {
            _SFCB_TRACE(1, ("--- Calling disableIndications %s",info->providerName));
            indicationEnabled=0;
	   TIMING_START(hdr,info)
            info->indicationMI->ft->disableIndications(info->indicationMI,ctx);
	   TIMING_STOP(hdr,info)
         }

         _SFCB_TRACE(1, ("--- Calling deactivateFilter %s",info->providerName));
	 TIMING_START(hdr,info)
	 if (info->indicationMI->ft->ftVersion < 100) {
	   deActivateFilterPreV1 fptr = 
	     (deActivateFilterPreV1) info->indicationMI->ft->deActivateFilter;
	   rci = fptr(info->indicationMI, ctx, result,
		  (CMPISelectExp*)se, "", path, 1);
	 } else {
	   rci = info->indicationMI->ft->deActivateFilter(info->indicationMI, ctx,
							  (CMPISelectExp*)se, "", path, 1);
	 }
	 TIMING_STOP(hdr,info)
         if (rci.rc==CMPI_RC_OK) {
            resp->rc=1;
            _SFCB_RETURN(resp);
         }

         if (resp) free(resp);
         resp = errorResp(&rci);
         _SFCB_RETURN(resp);
      }
      sef=&se->next;
   }

   _SFCB_RETURN(resp);
}

static BinResponseHdr *enableIndications(BinRequestHdr *hdr, ProviderInfo* info, 
             int requestor)
{
	_SFCB_ENTER(TRACE_PROVIDERDRV|TRACE_INDPROVIDER, "enableIndications");
	TIMING_PREP
	   
	IndicationReq *req = (IndicationReq*)hdr;
	BinResponseHdr *resp=NULL;
	CMPIStatus rci = { CMPI_RC_OK, NULL };
	//CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
	CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
	CMPIFlags flgs=0;
	
	ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
	ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
	ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);
	
	if (info->indicationMI==NULL) {
		CMPIStatus st;
		setStatus(&st,CMPI_RC_ERR_NOT_SUPPORTED,"Provider does not support indications");
		resp = errorResp(&st);
		_SFCB_RETURN(resp);  
	}
	
	if (indicationEnabled==0 && rci.rc==CMPI_RC_OK) {
		indicationEnabled=1;
		TIMING_START(hdr,info)
		info->indicationMI->ft->enableIndications(info->indicationMI,ctx);
		TIMING_STOP(hdr,info)
	}
	
	if (rci.rc==CMPI_RC_OK) {
		resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
		resp->rc=1;
	}
	if (rci.rc!=CMPI_RC_OK) {
		resp = errorResp(&rci);
		_SFCB_TRACE(1, ("--- Not OK rc: %d", rci.rc));
	}
	
	_SFCB_RETURN(resp);
}

static BinResponseHdr *disableIndications(BinRequestHdr *hdr, ProviderInfo* info, 
             int requestor)
{
	_SFCB_ENTER(TRACE_PROVIDERDRV|TRACE_INDPROVIDER, "disableIndications");
	TIMING_PREP
	   
	IndicationReq *req = (IndicationReq*)hdr;
	BinResponseHdr *resp=NULL;
	CMPIStatus rci = { CMPI_RC_OK, NULL };
	//CMPIObjectPath *path = relocateSerializedObjectPath(req->objectPath.data);
	CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
	CMPIFlags flgs=0;
	
	ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
	ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)req->principal.data,CMPI_chars);
	ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&hdr->sessionId,CMPI_uint32);
	
	if (info->indicationMI==NULL) {
		CMPIStatus st;
		setStatus(&st,CMPI_RC_ERR_NOT_SUPPORTED,"Provider does not support indications");
		resp = errorResp(&st);
		_SFCB_RETURN(resp);  
	}
	
	if (indicationEnabled==1 && rci.rc==CMPI_RC_OK) {
		indicationEnabled=0;
		TIMING_START(hdr,info)
		info->indicationMI->ft->disableIndications(info->indicationMI,ctx);
		TIMING_STOP(hdr,info)
	}
	
	if (rci.rc==CMPI_RC_OK) {
		resp = (BinResponseHdr *) calloc(1,sizeof(BinResponseHdr));
		resp->rc=1;
	}
	if (rci.rc!=CMPI_RC_OK) {
		resp = errorResp(&rci);
		_SFCB_TRACE(1, ("--- Not OK rc: %d", rci.rc));
	}
	
	_SFCB_RETURN(resp);
}

#endif   



static BinResponseHdr *opNotSupported(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   BinResponseHdr *resp;
   CMPIStatus rci = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   _SFCB_ENTER(TRACE_PROVIDERDRV, "opNotSupported");

   mlogf(M_ERROR,M_SHOW,"--- opNotSupported\n");
   resp = errorResp(&rci);
   _SFCB_RETURN(resp);
}



int initProvider(ProviderInfo *info, unsigned int sessionId)
{
   CMPIInstanceMI *mi = NULL;
   int rc=0;
   unsigned int flgs=0;
   CMPIContext *ctx = native_new_CMPIContext(MEM_TRACKED,info);
   
   _SFCB_ENTER(TRACE_PROVIDERDRV, "initProvider");

   pthread_mutex_lock(&info->initMtx);
   if (info->initialized==0) {  
   
     ctx->ft->addEntry(ctx,CMPIInvocationFlags,(CMPIValue*)&flgs,CMPI_uint32);
     ctx->ft->addEntry(ctx,CMPIPrincipal,(CMPIValue*)"$$",CMPI_chars);
     ctx->ft->addEntry(ctx,"CMPISessionId",(CMPIValue*)&sessionId,CMPI_uint32);
     if (info->parms) {
         ctx->ft->addEntry(ctx,"sfcbProviderParameters",(CMPIValue*)info->parms,CMPI_chars);
     }
     
     
     if (info->type & INSTANCE_PROVIDER) {
       rc |= (getInstanceMI(info, &mi, ctx) != 1);
     }   
     if (info->type & ASSOCIATION_PROVIDER) {
       rc |= (getAssociationMI(info, (CMPIAssociationMI **) & mi, ctx) != 1);
     }   
     if (info->type & METHOD_PROVIDER) {
       rc |= (getMethodMI(info, (CMPIMethodMI **) & mi, ctx) != 1);
     }   
     if (info->type & INDICATION_PROVIDER) {
       rc |= (getIndicationMI(info, (CMPIIndicationMI **) & mi, ctx) != 1);
     }   
     if (info->type & CLASS_PROVIDER) {
       rc |= (getClassMI(info, (CMPIClassMI **) & mi, ctx) != 1);
     }
     if (info->type & PROPERTY_PROVIDER) {
       rc |= (getPropertyMI(info, (CMPIPropertyMI **) & mi, ctx) != 1);
     }
#ifdef HAVE_QUALREP   
     if (info->type & QUALIFIER_PROVIDER) {
       rc |= (getQualifierDeclMI(info, (CMPIQualifierDeclMI **) & mi, ctx) != 1);
     }
#endif   
     
     if (rc) {
       rc = -2;
     } else {
       info -> initialized = 1;
     }
     pthread_mutex_unlock(&info->initMtx);
   }

   _SFCB_RETURN(rc);
}


static int doLoadProvider(ProviderInfo *info, char *dlName)
{
   char *dirs,*dir,*dirlast, *dircpy;
   char *fullname;
   struct stat stbuf;

   _SFCB_ENTER(TRACE_PROVIDERDRV, "doLoadProvider");

   if (getControlChars("providerDirs",&dirs) != 0) {
     mlogf(M_ERROR,M_SHOW,"*** No provider directories configured.\n");
     abort();
   }
   

   libraryName(NULL, (char *) info->location, dlName);
   
   dircpy = strdup(dirs);
   fullname = malloc(strlen(dircpy)+strlen(dlName)+2); /* sufficient */
   dir=strtok_r(dircpy," \t",&dirlast);
   info->library = NULL;
   while (dir) {
     libraryName(dir, (char *) info->location, fullname);
     if (stat(fullname,&stbuf) == 0) {
       info->library = dlopen(fullname, RTLD_NOW | RTLD_GLOBAL);
       if (info->library == NULL) {
	 mlogf(M_ERROR,M_SHOW,"*** dlopen error: %s\n",dlerror());
       } else {
	 _SFCB_TRACE(1, ("--- Loaded provider library %s for %s-%d",
			 fullname,
			 info->providerName,
			 currentProc));
       }
       break;	 
     }
     dir=strtok_r(NULL, " \t", &dirlast);
   }
   free(dircpy);
   free(fullname);

   if (info->library == NULL) {
      _SFCB_RETURN(-1);
   }   

   info->initialized=0;
   pthread_mutex_init(&info->initMtx,NULL);
   
   _SFCB_RETURN(0);
}


static BinResponseHdr *loadProvider(BinRequestHdr * hdr, ProviderInfo * info, int requestor)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadProvider");

   LoadProviderReq *req = (LoadProviderReq *) hdr;
   BinResponseHdr *resp;
   char dlName[512];

   _SFCB_TRACE(1, ("--- Loading Provide %s %s %s", (char *) req->className.data,
                (char *) req->provName.data, (char *) req->libName.data));

   info = (ProviderInfo *) calloc(1, sizeof(*info));

   info->className = strdup((char*)req->className.data);
   info->location = strdup((char*)req->libName.data);
   info->providerName = strdup((char*)req->provName.data);
   info->type = req->hdr.flags;
   info->unload = req->unload;
   info->providerSockets = providerSockets;
   info->provIds.ids = hdr->provId;

   switch (doLoadProvider(info,dlName)) {
   case -1: {
      char msg[740];
      sprintf(msg, "*** Failed to load %s for %s\n", dlName,
              info->providerName);
      mlogf(M_ERROR,M_SHOW,msg);
      resp = errorCharsResp(CMPI_RC_ERR_FAILED, msg);
      free(info);
      _SFCB_RETURN(resp);
   }
   case -2:  {
      char msg[740];
      sprintf(msg, "*** Inconsistent provider registration for %s (1)\n",
              info->providerName);
      mlogf(M_ERROR,M_SHOW,msg);
      resp = errorCharsResp(CMPI_RC_ERR_FAILED, msg);
      free(info);
      _SFCB_RETURN(resp);
   }
   default:
      if (activProvs)
         info->next = activProvs;
      activProvs = info;
      break;
   }

   resp = (BinResponseHdr *) calloc(1, sizeof(BinResponseHdr));
   resp->rc = 1;
   resp->count = 0;

   _SFCB_RETURN(resp);
}


static ProvHandler pHandlers[] = {
   {opNotSupported},            //dummy
   {getClass},                  //OPS_GetClass 1
   {getInstance},               //OPS_GetInstance 2
   {deleteClass},               //OPS_DeleteClass 3
   {deleteInstance},            //OPS_DeleteInstance 4
   {createClass},               //OPS_CreateClass 5
   {createInstance},            //OPS_CreateInstance 6
   {opNotSupported},            //OPS_ModifyClass 7
   {modifyInstance},            //OPS_ModifyInstance 8
   {enumClasses},               //OPS_EnumerateClasses 9
   {enumClassNames},            //OPS_EnumerateClassNames 10
   {enumInstances},             //OPS_EnumerateInstances 11
   {enumInstanceNames},         //OPS_EnumerateInstanceNames 12
   {execQuery},                 //OPS_ExecQuery 13
   {associators},               //OPS_Associators 14
   {associatorNames},           //OPS_AssociatorNames 15
   {references},                //OPS_References 16
   {referenceNames},            //OPS_ReferenceNames 17
   {getProperty},               //OPS_GetProperty 18
   {setProperty},               //OPS_SetProperty 19
#ifdef HAVE_QUALREP
   {getQualifier},              //OPS_GetQualifier 20
   {setQualifier},              //OPS_SetQualifier 21
   {deleteQualifier},           //OPS_DeleteQualifier 22
   {enumQualifiers},            //OPS_EnumerateQualifiers 23
#else
   {opNotSupported},            //OPS_GetQualifier 20
   {opNotSupported},            //OPS_SetQualifier 21
   {opNotSupported},            //OPS_DeleteQualifier 22
   {opNotSupported},            //OPS_EnumerateQualifiers 23
#endif
   {invokeMethod},              //OPS_InvokeMethod 24
   {loadProvider},              //OPS_LoadProvider 25
   {NULL},                      //OPS_PingProvider 26
   {NULL},                      //OPS_IndicationLookup   27
#ifdef SFCB_INCL_INDICATION_SUPPORT   
   {activateFilter},            //OPS_ActivateFilter     28
   {deactivateFilter},          //OPS_DeactivateFilter   29
   {disableIndications},        //OPS_DisableIndications  30
   {enableIndications}          //OPS_EnableIndications 31
#else
   {NULL},                      //OPS_ActivateFilter     28
   {NULL},                      //OPS_DeactivateFilter   29
   {NULL},                      //OPS_EnableIndications  30
   {NULL}                       //OPS_DisableIndications 31
#endif   
};

char *opsName[] = {
   "dummy",
   "GetClass",
   "GetInstance",
   "DeleteClass",
   "DeleteInstance",
   "CreateClass",
   "CreateInstance",
   "ModifyClass",
   "ModifyInstance",
   "EnumerateClasses",
   "EnumerateClassNames",
   "EnumerateInstances",
   "EnumerateInstanceNames",
   "ExecQuery",
   "Associators",
   "AssociatorNames",
   "References",
   "ReferenceNames",
   "GetProperty",
   "SetProperty",
   "GetQualifier",
   "SetQualifier",
   "DeleteQualifier",
   "EnumerateQualifiers",
   "InvokeMethod",
   "LoadProvider",
   "PingProvider",
   "IndicationLookup",
   "ActivateFilter",
   "DeactivateFilter",
   "EnableIndications",
   "DisableIndications",
};

static void *processProviderInvocationRequestsThread(void *prms)
{
   BinResponseHdr *resp=NULL;
   ProviderInfo *pInfo;
   ProvHandler hdlr;
   Parms *parms = (Parms *) prms;
   BinRequestHdr *req = parms->req;
   int i,requestor=0,initRc=0;

   _SFCB_ENTER(TRACE_PROVIDERDRV, "processProviderInvocationRequestsThread");
 
   for (i = 0; i < req->count; i++)
      if (req->object[i].length)
         req->object[i].data=(void*)((long)req->object[i].data+(char*)req);
      else if (req->object[i].type == MSG_SEG_CHARS)
         req->object[i].data = NULL;

   if (req->operation != OPS_LoadProvider) {
     if (req->provId == NULL) {
       mlogf(M_ERROR,M_SHOW,"-#- no provider id specified for request --- terminating process.\n");
       exit(-1);
     }

      time(&curProvProc->lastActivity);
      for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
         if (pInfo->provIds.ids == req->provId) {
            pInfo->lastActivity=curProvProc->lastActivity;
            break;
         }
      }
      if (pInfo==NULL) {
         mlogf(M_ERROR,M_SHOW,"-#- Serious provider id / provider process mismatch --- terminating process.\n");
         exit(-1);
      }
      
      if (pInfo && pInfo->library==NULL) { 
         char dlName[512];
         mlogf(M_INFO,M_SHOW,"--- Reloading provider\n");
         doLoadProvider(pInfo,dlName);
      }  
      
      if (pInfo && pInfo->initialized == 0) {
	initRc=initProvider(pInfo,req->sessionId);
	_SFCB_TRACE(1, ("--- Provider initialization rc %d",initRc));
      }

   }
   else pInfo = NULL;

   if (initRc) {
      char msg[1024];
      snprintf(msg,1023, "*** Inconsistent provider registration for %s (2)\n",
              pInfo->providerName);
      mlogf(M_ERROR,M_SHOW,msg);
      _SFCB_TRACE(1, (msg));
      resp = errorCharsResp(CMPI_RC_ERR_FAILED, msg);
   }
   
   else {
      _SFCB_TRACE(1, ("--- Provider request for %s %p %x",
        opsName[req->operation],pInfo,req->provId));

      if (req->flags & FL_chunked) requestor=parms->requestor;
      else requestor=-parms->requestor;

      hdlr = pHandlers[req->operation];
      
      pthread_mutex_lock(&activeMtx);
      parms->pInfo=pInfo;
      ENQ_BOT_LIST(parms,activeThreadsFirst,activeThreadsLast,next,prev); 
      pthread_mutex_unlock(&activeMtx);
      
      resp = hdlr.handler(req, pInfo, requestor);
      
      pthread_mutex_lock(&activeMtx);
      DEQ_FROM_LIST(parms,activeThreadsFirst,activeThreadsLast,next,prev);
      pthread_mutex_unlock(&activeMtx);

      _SFCB_TRACE(1, ("--- Provider request for %s DONE", opsName[req->operation]));
   }
   
   if (resp) {
      if (req->options & BRH_NoResp) {
        _SFCB_TRACE(1, ("--- response suppressed"));
      }
      else {
	sendResponse(parms->requestor, resp);
	if (req->operation == OPS_LoadProvider && resp->rc == 2)
	  exit(-1);
      }
   }  
    
   tool_mm_flush();
   
   free(resp);

   if (pInfo && idleThreadStartHandled==0) {
      if (idleThreadStartHandled==0 && req->operation != OPS_PingProvider) {
         if (pInfo->unload==0) {
            pthread_attr_t tattr;
            pthread_attr_init(&tattr);
            pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);   
            pthread_create(&pInfo->idleThread, &tattr, 
               (void *(*)(void *))providerIdleThread, NULL);
            idleThreadId=pInfo->idleThread;
         }   
         else pInfo->idleThread=0;
         idleThreadStartHandled=1;
      }
      time(&pInfo->lastActivity);
      curProvProc->lastActivity=pInfo->lastActivity;
   }   

   if ((req->options & BRH_Internal) ==0) close(abs(parms->requestor));
   free(parms);
   free(req);

   _SFCB_RETURN(NULL);
}

int pauseProvider(char *name)
{
   int rc=0;
   char *n;
  
   if (noProvPause) return 0;
   if (provPauseStr==NULL) return 0;
   else {
      if (provPauseStr) {
         char *p;
         p=provPauseStr=strdup(provPauseStr);
         while (*p) { *p=tolower(*p); p++; }
      }
   }   
   if (provPauseStr) {
      char *p;
      int l=strlen(name);
      p=n=strdup(name);      
      while (*p) { *p=tolower(*p); p++; }
      if ((p=strstr(provPauseStr,n))!=NULL) {
         if ((p==provPauseStr || *(p-1)==',') && (p[l]==',' || p[l]==0)) rc=1;
      }
      free(p);
      return rc;
  }
   noProvPause=1;
   return 0;
}

void processProviderInvocationRequests(char *name)
{
   unsigned long rl;
   Parms *parms;
   int rc,debugMode=0,once=1;
   pthread_t t;
   pthread_attr_t tattr;
   MqgStat mqg;

   _SFCB_ENTER(TRACE_PROVIDERDRV, "processProviderInvocationRequests");

   pthread_attr_init(&tattr);
   pthread_attr_setdetachstate(&tattr, PTHREAD_CREATE_DETACHED);

   debugMode=pauseProvider(name);
   for (;;) {
      _SFCB_TRACE(1, ("--- Waiting for provider request to %d-%lu",
                   providerSockets.receive,getInode(providerSockets.receive)));
      parms = (Parms *) malloc(sizeof(*parms));
      
      rc = spRecvReq(&providerSockets.receive, &parms->requestor,
                     (void **) &parms->req, &rl, &mqg);
      if (mqg.rdone) {
	int debug_break = 0;
         if (rc!=0)mlogf(M_ERROR,M_SHOW,"oops\n");               

         _SFCB_TRACE(1, ("--- Got something %d-%p on %d-%lu", 
            parms->req->operation,parms->req->provId,
            providerSockets.receive,getInode(providerSockets.receive)));

         if (once && debugMode && parms->req->operation != OPS_LoadProvider) for (;;) {
	   if (debug_break) break;
            fprintf(stdout,"-#- Pausing for provider: %s -pid: %d\n",name,currentProc);
            once=0;      
            sleep(5);
         }

         if (parms->req->operation == OPS_LoadProvider || debugMode) {
            processProviderInvocationRequestsThread(parms);
         }
         else {
            pthread_create(&t, &tattr, (void *(*)(void *))
                processProviderInvocationRequestsThread, (void *) parms);
         }
      }
      else {
      	free(parms);
      }   
   }
   _SFCB_EXIT();
}
