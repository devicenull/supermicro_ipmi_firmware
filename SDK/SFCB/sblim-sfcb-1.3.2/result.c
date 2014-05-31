
/*
 * result.c
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
 * Author:        Frank Scheffler
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPIResult implementation.
 *
*/


#include "native.h"
#include "providerMgr.h"
#include "constClass.h"
#include "queryOperation.h"
#include "utilft.h"
#include "objectImpl.h"
#include "mlog.h"

extern void native_array_reset_size(CMPIArray * array, CMPICount increment);
extern MsgSegment setInstanceMsgSegment(CMPIInstance * ci);
extern MsgSegment setConstClassMsgSegment(CMPIConstClass * cl);
extern int sendResponse(int requestor, BinResponseHdr * hdr);
extern int spSendAck(int to);
extern int spRcvAck(int from);
extern int getConstClassSerializedSize(CMPIConstClass*);
extern void getSerializedConstClass(CMPIConstClass * cl, void *area);
extern int getControlNum(char *id, long *val);
extern int spSendResult2(int *to, int *from, 
   void *d1, unsigned long s1, void *d2, unsigned long s2);



struct native_result {
   CMPIResult result;
   int mem_state;

   int requestor,size,legacy;
   CMPICount current;
   CMPIArray *array;

   BinResponseHdr *resp;
   unsigned long sMax,sNext;
   char *data;
   unsigned long dMax,dNext;
   QLStatement *qs;
};
typedef struct native_result NativeResult;

static struct native_result *__new_empty_result(int, CMPIStatus *);


static void prepResultBuffer(NativeResult *nr)
{
   _SFCB_ENTER(TRACE_PROVIDERDRV, "prepResultBuffer");
   
   if (getControlNum("chunkSize",(long*)&nr->dMax)) nr->dMax=50000;

   nr->dNext=0;
   nr->data=(char*)malloc(nr->dMax);
  
   nr->sMax=nr->dMax/400;
   nr->sNext=0;
   nr->resp=(BinResponseHdr*)calloc(1,sizeof(BinResponseHdr)+
            ((nr->sMax-1)*sizeof(MsgSegment)));
   
   _SFCB_EXIT();
}

static int xferResultBuffer(NativeResult *nr, int to, int more, int rc)
{
   long l=sizeof(BinResponseHdr)+((nr->sNext-1)*sizeof(MsgSegment));
   int i,dmy=-1,s1=l;
 
   _SFCB_ENTER(TRACE_PROVIDERDRV, "xferResultBuffer");
   
   if (nr->data==NULL) prepResultBuffer(nr);
   
   for (i=0; i<nr->sMax; i++) {
      nr->resp->object[i].data=(void*)l;
      l+=nr->resp->object[i].length;
   } 
     
   nr->resp->moreChunks=more;
   nr->resp->rc = rc;
   nr->resp->count = nr->sNext;
   
   rc=spSendResult2(&to,&dmy,nr->resp,s1,nr->data,nr->dNext);
   if (more) spRcvAck(to);
   
   _SFCB_RETURN(rc);
}

int xferLastResultBuffer(CMPIResult *result, int to, int rc)
{
    NativeResult *r = (NativeResult*) result;
 
   _SFCB_ENTER(TRACE_PROVIDERDRV, "xferLastResultBuffer");
    rc=xferResultBuffer(r,to,0,rc);
   _SFCB_RETURN(rc);
}

static void* nextResultBufferPos(NativeResult *nr, int type, int length)
{
   long pos,npos;

   _SFCB_ENTER(TRACE_PROVIDERDRV, "nextResultBufferPos");
   
   if (nr->data==NULL) prepResultBuffer(nr);

   if (nr->dNext+length>=nr->dMax) {
      if (nr->requestor) {
         xferResultBuffer(nr,nr->requestor, 1,1);
         nr->dNext=0;
         nr->sNext=0;
   }
      else {
         while (nr->dNext+length>=nr->dMax) nr->dMax*=2;
         nr->data=(char*)realloc(nr->data,nr->dMax);
      }
   }
   
   if (nr->sNext==nr->sMax) {
      nr->sMax*=2;
      nr->resp=(BinResponseHdr*)realloc(nr->resp,sizeof(BinResponseHdr)+
            ((nr->sMax-1)*sizeof(MsgSegment)));
   }
   
   nr->resp->object[nr->sNext].data=(void*)nr->dNext;
   nr->resp->object[nr->sNext].length=length;
   nr->resp->object[nr->sNext++].type=type;
   pos=nr->dNext;
   nr->dNext+=length;

   npos=(long)(nr->data+pos);
   _SFCB_RETURN((void*)npos);
}

/*****************************************************************************/

static CMPIStatus __rft_release(CMPIResult * result)
{
   NativeResult *nr = (NativeResult*) result;

   free(nr->data);
   free(nr->resp);
   free(result);

   CMReturn(CMPI_RC_OK);
}


static CMPIResult *__rft_clone(const CMPIResult * result, CMPIStatus * rc)
{
   NativeResult *or = (NativeResult*)result;
   NativeResult *nr = __new_empty_result(MEM_NOT_TRACKED, rc);
   CMPIArray *a = or->array;

   if (rc && rc->rc != CMPI_RC_OK)
      return NULL;

   *nr=*or;  
   
   if (or->data) {
      nr->data=(char*)malloc(or->dMax);
      memcpy(nr->data,or->data,or->dMax);
   }
   if (or->resp) {
      int s=sizeof(BinResponseHdr)+((or->sMax-1)*sizeof(MsgSegment));
      nr->resp=(BinResponseHdr*)malloc(s);
      memcpy(nr->resp,or->resp,s);
   }         

   if (a) nr->array = a->ft->clone(a, rc);

   return (CMPIResult*)nr;
}


static CMPIStatus returnData(const CMPIResult * result,
                                   const CMPIValue * val, CMPIType type)
{
   NativeResult *r = (NativeResult*) result;
   
   if (r->current == 0 && r->array==NULL) {
      CMPIStatus rc;
      r->array = TrackedCMPIArray(1, type, &rc);
      r->current = 0;
      if (rc.rc != CMPI_RC_OK) return rc;
   }
   else sfcb_native_array_increase_size(r->array, 1);

   return CMSetArrayElementAt(r->array, r->current++, val, type); 
}

static CMPIStatus __rft_returnData(const CMPIResult * result,
                                   const CMPIValue * val, CMPIType type)
{
//   NativeResult *r = (NativeResult*) result;

   if (type==CMPI_ref) {
      mlogf(M_ERROR,M_SHOW,"--- CMPIResult does not yet support returning references\n");
      abort();   
   }
   
   return returnData(result,val,type);
}

extern UtilStringBuffer *instanceToString(const CMPIInstance * ci, char **props);

static CMPIStatus __rft_returnInstance(const CMPIResult * result,
                                       const CMPIInstance * instance)
{
   int size,isInst=isInstance(instance);
   void *ptr;
   NativeResult *r = (NativeResult*) result;
   int releaseInstance=0;
   CMPIStatus st={CMPI_RC_OK,NULL};
   
   _SFCB_ENTER(TRACE_PROVIDERDRV, "__rft_returnInstance");
   
   if (r->qs) {
      int irc;

      if (r->qs->where) {
         r->qs->propSrc.data=(CMPIInstance *)instance;
         irc=r->qs->where->ft->evaluate(r->qs->where,&r->qs->propSrc);
         if (irc==1) {
            if (r->qs->allProps==0) {
               instance=
                  r->qs->ft->cloneAndFilter(r->qs,(CMPIInstance *)instance,r->qs->cop,r->qs->keys);
               releaseInstance=1;
            }     
         }   
         else CMReturn(CMPI_RC_OK);
      }
      else {
         if (r->qs->allProps==0) {
            instance=r->qs->ft->cloneAndFilter(r->qs,(CMPIInstance *)instance,r->qs->cop,r->qs->keys);
            releaseInstance=1;
         }
      }        
   }

   if (r->legacy) {
      CMPIValue v;
      CMPIStatus rc;
      _SFCB_TRACE(1,("--- Legacy Mode"));
      v.inst = (CMPIInstance *) instance;
      rc=returnData(result, &v, CMPI_instance);
      if (releaseInstance) instance->ft->release((CMPIInstance*)instance);
      _SFCB_RETURN(rc);
   }

   if (isInst) {
      size=getInstanceSerializedSize(instance);
      ptr=nextResultBufferPos(r, MSG_SEG_INSTANCE, size);
      _SFCB_TRACE(1,("--- Moving instance %d",size));
      getSerializedInstance(instance,ptr);
   }
   else {
      size=getConstClassSerializedSize((CMPIConstClass*)instance);
      ptr=nextResultBufferPos(r, MSG_SEG_CONSTCLASS, size);
      _SFCB_TRACE(1,("--- Moving class %d",size));
      getSerializedConstClass((CMPIConstClass*)instance,ptr);
   }

   if (releaseInstance) instance->ft->release((CMPIInstance*)instance);
   _SFCB_RETURN(st);
}


static CMPIStatus __rft_returnObjectPath(const CMPIResult * result,
                                         const CMPIObjectPath * cop)
{
   int size;
   void *ptr;
   NativeResult *r = (NativeResult*) result;

   if (r->legacy) {
      CMPIValue v;
      v.ref = (CMPIObjectPath*)cop;
      return returnData(result, &v, CMPI_ref);
   }   
  
   size=getObjectPathSerializedSize(cop);
   ptr=nextResultBufferPos(r, MSG_SEG_OBJECTPATH, size);
   getSerializedObjectPath(cop,ptr);

   CMReturn(CMPI_RC_OK);
}


static CMPIStatus __rft_returnDone(const CMPIResult * result)
{
   CMReturn(CMPI_RC_OK);
}


static NativeResult *__new_empty_result(int mode, CMPIStatus * rc)
{
   static CMPIResultFT rft = {
      NATIVE_FT_VERSION,
      __rft_release,
      __rft_clone,
      __rft_returnData,
      __rft_returnInstance,
      __rft_returnObjectPath,
      __rft_returnDone
   };
   static CMPIResult r = {
      "CMPIResult",
      &rft
   };
   int state;

   NativeResult result,*tRslt;
   memset(&result,0,sizeof(NativeResult));
   result.result=r;

   tRslt=memAddEncObj(mode, &result, sizeof(result),&state);
   tRslt->mem_state=state;

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   return tRslt;
}


CMPIResult *native_new_CMPIResult(int requestor, int legacy, CMPIStatus * rc)
{
   CMPIResult *res= (CMPIResult *) __new_empty_result(MEM_TRACKED, rc);
   ((NativeResult*)res)->requestor=requestor;
   ((NativeResult*)res)->legacy=legacy;
   return res;
}

void setResultQueryFilter(CMPIResult * result, QLStatement *qs)
{
   NativeResult *r = (NativeResult*) result;
   r->qs=qs;   
}

CMPIArray *native_result2array(CMPIResult * result)
{
   struct native_result *r = (struct native_result *) result;

   return r->array;
}


/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
