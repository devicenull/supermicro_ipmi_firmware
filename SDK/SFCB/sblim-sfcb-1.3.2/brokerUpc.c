/*
 * $Id: brokerUpc.c,v 1.27 2008/04/09 18:43:14 smswehla Exp $
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
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * This module implements CMPI up-call functions.
 *
*/

 
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "support.h"
#include "native.h"
#include "utilft.h"
#include "providerMgr.h"
#include "providerRegister.h"
#include "objectImpl.h"
#include "msgqueue.h"
#include "utilft.h"
#include "config.h"

#ifdef HAVE_INDICATIONS
#define SFCB_INCL_INDICATION_SUPPORT 1
#endif

#ifdef SFCB_INCL_INDICATION_SUPPORT 
#include "selectexp.h"
extern int indicationEnabled;
#endif

extern MsgSegment setArgsMsgSegment(const CMPIArgs * args);
extern CMPIArgs *relocateSerializedArgs(void *area);
extern UtilStringBuffer *instanceToString(CMPIInstance * ci, char **props);
extern MsgSegment setInstanceMsgSegment(const CMPIInstance * ci);
extern void memLinkObjectPath(CMPIObjectPath *op);

extern ProviderInfo *activProvs;
#ifdef SFCB_INCL_INDICATION_SUPPORT
extern NativeSelectExp *activFilters;
#endif
extern CMPIObjectPath *TrackedCMPIObjectPath(const char *nameSpace,
                                      const char *className, CMPIStatus * rc);
extern void setStatus(CMPIStatus *st, CMPIrc rc, char *msg);

extern int initProvider(ProviderInfo *info, unsigned int sessionId);


void closeProviderContext(BinRequestContext * ctx);

//---------------------------------------------------
//---
//-     Thread support
//---
//---------------------------------------------------


static CMPI_MUTEX_TYPE mtx=NULL;

void lockUpCall(const CMPIBroker* mb)
{
   if (mtx==NULL) mtx=mb->xft->newMutex(0);
   mb->xft->lockMutex(mtx);
}

void unlockUpCall(const CMPIBroker* mb)
{
   mb->xft->unlockMutex(mtx);
   mb->xft->destroyMutex(mtx);
   mtx = NULL;
}

static CMPIContext* prepareAttachThread(const CMPIBroker* mb, const CMPIContext* ctx) 
{
   CMPIContext *nctx;
   _SFCB_ENTER(TRACE_INDPROVIDER | TRACE_UPCALLS, "prepareAttachThread");
   nctx=native_clone_CMPIContext(ctx); 
   _SFCB_RETURN(nctx);
}

static CMPIStatus attachThread(const CMPIBroker* mb, const CMPIContext* ctx) 
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER | TRACE_UPCALLS, "attachThread");
   _SFCB_RETURN(st);
}

static CMPIStatus detachThread(const CMPIBroker* mb, const CMPIContext* ctx) 
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER | TRACE_UPCALLS, "detachThread");
    ctx->ft->release((CMPIContext*)ctx);
   _SFCB_RETURN(st);
}


static CMPIStatus deliverIndication(const CMPIBroker* mb, const CMPIContext* ctx,
       const char *ns, const CMPIInstance* ind) 
{
#ifdef SFCB_INCL_INDICATION_SUPPORT 
   
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIArgs *in=NULL;
   CMPIObjectPath *op=NULL;
   
   _SFCB_ENTER(TRACE_INDPROVIDER | TRACE_UPCALLS, "deliverIndication");
   
   if (indicationEnabled==0) {      
      _SFCB_TRACE(1,("--- Provider not enabled for indications"));
      printf("Provider not enabled for indications\n");
      setStatus(&st,CMPI_RC_ERR_FAILED, "Provider not enabled for indications");
      _SFCB_RETURN(st);
   }

   NativeSelectExp *se=activFilters;
   
   while (se) {
     if (se->exp.ft->evaluate(&se->exp,ind,&st)) {
     /*apply a propertyfilter in case the query is not "SELECT * FROM ..." */
        if(se->qs->spNames && se->qs->spNames[0]) {
           ind->ft->setPropertyFilter((CMPIInstance*)ind, (const char**)se->qs->spNames, NULL);
        }
        op=CMNewObjectPath(mb,"root/interop","cim_indicationsubscription",NULL);
        in=CMNewArgs(mb,NULL);
        CMAddArg(in,"nameSpace",ns,CMPI_chars);
        CMAddArg(in,"indication",&ind,CMPI_instance);
        CMAddArg(in,"filterid",&se->filterId,
#if SIZEOF_INT == SIZEOF_VOIDP
		     CMPI_uint32
#else
		     CMPI_uint64
#endif		     
		 );
         CBInvokeMethod(mb,ctx,op,"_deliver",in,NULL,&st);
      }
      se=se->next;
   }   
   
   _SFCB_RETURN(st);
#else 

   _SFCB_ENTER(TRACE_INDPROVIDER | TRACE_UPCALLS, "deliverIndication");
   CMPIStatus rci = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   _SFCB_RETURN(rci);
   
#endif
}

static void buildStatus(BinResponseHdr *resp, CMPIStatus *st)
{
   if (st==NULL) return;
   st->rc=resp->rc;
   if (resp->rc && resp->count==1 &&  resp->object[0].type==MSG_SEG_CHARS && resp->object[0].length) {
     st->msg=sfcb_native_new_CMPIString((char*)resp->object[0].data,NULL, 0);
   }
   else st->msg=NULL;   
}

//---------------------------------------------------
//---
//-     Instance support
//---
//---------------------------------------------------



static CMPIStatus setErrorStatus(int code)
{
   CMPIStatus st;
   char *msg;
   char m[256];

   switch (code) {
   case MSG_X_NOT_SUPPORTED:
      msg = "Operation not supported";
      code = CMPI_RC_ERR_NOT_SUPPORTED;
      break;
   case MSG_X_INVALID_CLASS:
      msg = "Class not found";
      code = CMPI_RC_ERR_INVALID_CLASS;
      break;
   case MSG_X_INVALID_NAMESPACE:
      msg = "Invalid namespace";
      code = CMPI_RC_ERR_INVALID_NAMESPACE;
      break;
   case MSG_X_PROVIDER_NOT_FOUND:
      msg = "Provider not found or not loadable";
      code = CMPI_RC_ERR_FAILED;
      break;
   case MSG_X_FAILED:
      msg = "Provider Manager failed";
      code = CMPI_RC_ERR_FAILED;
      break;
   default:
      sprintf(m, "Provider Manager internal error - %d\n", code);
      code = CMPI_RC_ERR_FAILED;
      msg = m;
   }
   st.rc = code;
   st.msg = sfcb_native_new_CMPIString(msg, NULL, 0);
   return st;
}

static void setContext(BinRequestContext * binCtx, OperationHdr * oHdr,
                       BinRequestHdr * bHdr, int size, 
		       const CMPIContext    * ctx,
		       const CMPIObjectPath * cop)
{
   CMPIData ctxData;
   memset(binCtx,0,sizeof(BinRequestContext));
   oHdr->nameSpace = setCharsMsgSegment((char *)
                          ClObjectPathGetNameSpace((ClObjectPath *) cop->hdl));
   oHdr->className = setCharsMsgSegment((char *)
                          ClObjectPathGetClassName((ClObjectPath *) cop->hdl));
   ctxData=CMGetContextEntry(ctx,CMPIPrincipal,NULL);
   if (ctxData.value.string) {
     bHdr->object[0] = setCharsMsgSegment(CMGetCharPtr(ctxData.value.string));
   } else {
     bHdr->object[0] = setCharsMsgSegment("$$");
   }
   bHdr->object[1] = setObjectPathMsgSegment(cop);
   ctxData=CMGetContextEntry(ctx,CMPIInvocationFlags,NULL);
   bHdr->flags=ctxData.value.Int;
   bHdr->sessionId=ctx->ft->getEntry(ctx,"CMPISessionId",NULL).value.uint32;

   binCtx->oHdr = oHdr;
   binCtx->bHdr = bHdr;
   binCtx->bHdrSize = size;
   binCtx->chunkedMode = 0;
}

static void cpyResult(CMPIResult *result, CMPIArray *ar, int *c)
{
   CMPIArray *r;
   int j,m;
   
   r = native_result2array(result);
   if(r) {
      for (j=0,m=CMGetArrayCount(r,NULL); j<m; j++, *c=(*c)+1) {
         CMPIData data=CMGetArrayElementAt(r,j,NULL);
         if (*c) sfcb_native_array_increase_size(ar, 1);
         CMSetArrayElementAt(ar, *c, &data.value, data.type);
      }
   }
}      

static void cpyResponse(BinResponseHdr *resp, CMPIArray *ar, int *c, CMPIType type)
{
   int j; 
   void *tObj;
   
   for (j = 0; j < resp->count; *c=(*c)+1, j++) {
      if (*c) sfcb_native_array_increase_size(ar, 1);
      if (type==CMPI_ref) {
         CMPIObjectPath *obj = relocateSerializedObjectPath(resp->object[j].data);
         tObj=obj->ft->clone(obj,NULL);
      }
      else {
         CMPIInstance *obj = relocateSerializedInstance(resp->object[j].data);
         tObj=obj->ft->clone(obj,NULL);
      }
      memLinkInstance(tObj);
      CMSetArrayElementAt(ar, *c, &tObj, type);
   }
}   

static void checkReroute(const CMPIBroker * broker,
                         const CMPIContext * context,
                         OperationHdr *oHdr)
{
   CMPIData data;
   CMPIStatus st;
   
   data = context->ft->getEntry(context, "rerouteToProvider", &st);
   if(st.rc == CMPI_RC_OK) {
      /* property found! */
      oHdr->className = setCharsMsgSegment(data.value.string->hdl);
   }
}

/*
 *  Operations requests handler for generic enumeration calls
 *  should apply to enumInstances/InstanceNames
 *                  emunClasses/ClassNames
 *                  associators/associatorNames
 *                  references/referenceNames 
 */

static CMPIEnumeration *genericEnumRequest(const CMPIBroker * broker,
                                      const CMPIContext * context,
                                      const CMPIObjectPath * cop,
                                      const char **props,
                                      const char *assocclass,
                                      const char *resultclass,
                                      const char *role,
                                      const char *resultrole,                                      
                                      const int optype,
                                      BinRequestHdr *bhdr,
                                      OperationHdr *oHdr,
                                      int sreqSize,
                                      const int retType,
                                      CMPIStatus * rc)
{
   BinRequestContext binCtx;
   
   CMPIStatus st = { CMPI_RC_OK, NULL },rci = {CMPI_RC_OK, NULL};
   CMPIEnumeration *enm = NULL;
   CMPIArray *ar = NULL;
   BinRequestHdr *bhdrTmp=NULL;
   CMPIObjectPath *copLocalCall;
   int irc, c, i=-1;

   _SFCB_ENTER(TRACE_UPCALLS, "genericEnumRequest");
   
   if (cop && cop->hdl) {
   
      lockUpCall(broker);
      
      if(props) {
      	 while(props[++i]);
      	 bhdrTmp=calloc(1,sreqSize+i*sizeof(MsgSegment));
      	 memcpy(bhdrTmp, bhdr, sreqSize);
      	 bhdr=bhdrTmp;
      	 sreqSize+=i*sizeof(MsgSegment);
      	 i=-1;
      	 while(props[++i]) {
      	    bhdr->object[i+2] = setCharsMsgSegment(props[i]);
      	 }
      	 bhdr->count+=i;      	 
      }
      setContext(&binCtx, oHdr, bhdr, sreqSize, context, cop);

      checkReroute(broker, context, oHdr);

      irc = getProviderContext(&binCtx, oHdr);

      if (irc == MSG_X_PROVIDER) {
         BinResponseHdr *resp;
         ProviderInfo *pInfo;
         int local;
         ar = TrackedCMPIArray(0, retType, NULL);
         for (resp=NULL, c=0, i = 0; i < binCtx.pCount; i++,binCtx.pDone++) {
            local=0;
            binCtx.provA = binCtx.pAs[i];
            for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
               if (pInfo->provIds.ids == binCtx.provA.ids.ids) {
               	  copLocalCall = (CMPIObjectPath*)cop;
               	  unlockUpCall(broker);
                  #ifndef HAVE_OPTIMIZED_ENUMERATION
                     _SFCB_TRACE(TRACE_UPCALLS, ("--- Unoptimized Enums - looking if classname needs to be replaced%s",
                                      pInfo->className));
                     if (pInfo->className && pInfo->className[0] != '$') {
                        /* not a special provider, perform class name substitution if call is for a
                           parent class of the class the provider is registered for */
                        char * classname = CMGetCharPtr(CMGetClassName(cop,NULL));
                        char * namespace = CMGetCharPtr(CMGetNameSpace(cop,NULL));
                        if (classname && namespace && strcasecmp(pInfo->className,classname)) {
                           CMPIObjectPath * provPath = CMNewObjectPath(broker,namespace,pInfo->className,NULL);
                           if (provPath && CMClassPathIsA(broker,provPath,classname,NULL)) {
	                      _SFCB_TRACE(TRACE_UPCALLS, ("--- Replacing class name %s",pInfo->className));
	                      copLocalCall = provPath;
                           }
                        }
                     }
                  #endif
                  CMPIResult *result = native_new_CMPIResult(0,1,NULL);
                  local=1;
                  
		  if (pInfo->initialized == 0) {
		    initProvider(pInfo,binCtx.bHdr->sessionId);
		  }
		  switch(optype) {
		    case OPS_Associators:
		      rci = pInfo->associationMI->ft->associators(pInfo->associationMI,
		                      context, result, copLocalCall, assocclass,
		                      resultclass, role, resultrole, props);
		      break;
		    case OPS_AssociatorNames:
		      rci = pInfo->associationMI->ft->associatorNames(pInfo->associationMI,
		                      context, result, copLocalCall, assocclass,
		                      resultclass, role, resultrole);
		      break;
            case OPS_References:
              rci = pInfo->associationMI->ft->references(pInfo->associationMI,
                              context, result, copLocalCall, resultclass,
                              role, props);
              break;
            case OPS_ReferenceNames:
              rci = pInfo->associationMI->ft->referenceNames(pInfo->associationMI,
                              context, result, copLocalCall, resultclass, role);
              break;
		    case OPS_EnumerateInstances:
		      rci = pInfo->instanceMI->ft->enumerateInstances(pInfo->instanceMI,
		                      context, result, copLocalCall, props);
		      break;
		    case OPS_EnumerateInstanceNames:
		      rci = pInfo->instanceMI->ft->enumerateInstanceNames(pInfo->instanceMI,
		                      context, result, copLocalCall);		                      
		      break;
		  }
                  lockUpCall(broker);
                  if (rci.rc == CMPI_RC_OK) cpyResult(result, ar, &c);  
                  else st=rci; 
                  break;
               }
            } 
            if (local) continue;
            resp = invokeProvider(&binCtx);
            
            resp->rc--;
            rci.rc = resp->rc;
            if (resp->rc == CMPI_RC_OK) cpyResponse(resp, ar, &c, retType);
            else st=rci; 
            free (resp);
         }
         closeProviderContext(&binCtx);
         enm = sfcb_native_new_CMPIEnumeration(ar, NULL);
      }  
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
   }
   else st.rc = CMPI_RC_ERR_FAILED;

   if(bhdrTmp) free(bhdrTmp);
   if (rc) *rc = st;
   
   _SFCB_TRACE(1,("--- rc: %d",st.rc));
   _SFCB_RETURN(enm);
}

//---------------------------------------------------
//---
//-     Operations requests handlers - Instances
//---
//---------------------------------------------------


static CMPIInstance *getInstance(const CMPIBroker * broker,
                                 const CMPIContext * context,
                                 const CMPIObjectPath * cop,
                                 const char **props, CMPIStatus * rc)
{
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   GetInstanceReq *sreq = NULL;
   OperationHdr oHdr = { OPS_GetInstance, 2 };
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIInstance *inst = NULL,*tInst=NULL;
   int irc, ps, sreqSize=sizeof(GetInstanceReq);
   ProviderInfo *pInfo;
   const char **p=props;

   _SFCB_ENTER(TRACE_UPCALLS, "getInstance");
   
   if (cop && cop->hdl) {
   
      lockUpCall(broker);

      for (ps=0,p=props; p && *p; p++,ps++) {
         sreqSize+=sizeof(MsgSegment);
      }
      sreq=(GetInstanceReq*)calloc(1,sreqSize);
      sreq->hdr.count=ps+2;
      sreq->hdr.operation=OPS_GetInstance;
   
      setContext(&binCtx, &oHdr, &sreq->hdr, sreqSize, context, cop);
      _SFCB_TRACE(1,("--- for %s %s",(char*)oHdr.nameSpace.data,(char*)oHdr.className.data));
      
      checkReroute(broker, context, &oHdr);
      
      for (ps=0,p=props; p && *p; p++,ps++) {
         sreq->properties[ps]=setCharsMsgSegment(*p);
      }

      irc = getProviderContext(&binCtx, &oHdr);
      
      if (irc == MSG_X_PROVIDER) {
      for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
         if (pInfo->provIds.ids == binCtx.provA.ids.ids) {	   
            CMPIResult *result = native_new_CMPIResult(0,1,NULL);
            CMPIArray *r;
            unlockUpCall(broker);
	    if (pInfo->initialized == 0) {
	      initProvider(pInfo,binCtx.bHdr->sessionId);
	    }
            st = pInfo->instanceMI->ft->getInstance(pInfo->instanceMI,context,result,cop,props);
            if (rc) *rc=st;
            r = native_result2array(result);
            if (st.rc==0) 
               inst=CMGetArrayElementAt(r, 0, NULL).value.inst;
            return inst;   
         }
      }

         resp = invokeProvider(&binCtx);
         closeProviderContext(&binCtx);
         resp->rc--;
         buildStatus(resp,&st);
         if (resp->rc == CMPI_RC_OK) {
            inst = relocateSerializedInstance(resp->object[0].data);
            tInst=inst->ft->clone(inst,NULL);
            memLinkInstance(tInst);
         }
	 free(resp);
      }
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
   }
   else st.rc = CMPI_RC_ERR_FAILED;
   
   if(sreq) {
      free(sreq);
   }

   if (rc) *rc = st;
   
   _SFCB_TRACE(1,("--- rc: %d",st.rc));
   
   _SFCB_RETURN(tInst);
}

static CMPIObjectPath *createInstance(const CMPIBroker * broker,
                                      const CMPIContext * context,
                                      const CMPIObjectPath * cop,
                                      const CMPIInstance * inst, CMPIStatus * rc)
{
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   CreateInstanceReq sreq = BINREQ(OPS_CreateInstance, 3);
   OperationHdr oHdr = { OPS_CreateInstance, 2 };
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIObjectPath *op = NULL,*tOp=NULL;
   int irc;
   ProviderInfo *pInfo;

   _SFCB_ENTER(TRACE_UPCALLS, "createInstance");
   
   if (cop && cop->hdl && inst && inst->hdl) {
   
      lockUpCall(broker);
   
      setContext(&binCtx, &oHdr, &sreq.hdr, sizeof(sreq), context, cop);
      _SFCB_TRACE(1,("--- for %s %s",(char*)oHdr.nameSpace.data,(char*)oHdr.className.data)); 

      sreq.instance = setInstanceMsgSegment(inst);
      
      checkReroute(broker, context, &oHdr);
      
      irc = getProviderContext(&binCtx, &oHdr);

      if (irc == MSG_X_PROVIDER) {
      for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
         if (pInfo->provIds.ids == binCtx.provA.ids.ids) {
            CMPIResult *result = native_new_CMPIResult(0,1,NULL);
            CMPIArray *r;
            unlockUpCall(broker);
	    if (pInfo->initialized == 0) {
	      initProvider(pInfo,binCtx.bHdr->sessionId);
	    }
            st = pInfo->instanceMI->ft->createInstance(pInfo->instanceMI,context,result,cop,inst);
            if (rc) *rc=st;
            r = native_result2array(result);
            if (st.rc==0) 
               op=CMGetArrayElementAt(r, 0, NULL).value.ref;
            return op;   
         }
      }
      
         resp = invokeProvider(&binCtx);
         closeProviderContext(&binCtx);
         resp->rc--;
         buildStatus(resp,&st);
         if (resp->rc == CMPI_RC_OK) {
            op = relocateSerializedObjectPath(resp->object[0].data);
            tOp=op->ft->clone(op,NULL);
            memLinkObjectPath(tOp);
         }
	 free(resp);
      }
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
   }
   else st.rc = CMPI_RC_ERR_FAILED;

   if (rc) *rc = st;
   
   _SFCB_TRACE(1,("--- rc: %d",st.rc));
   
   _SFCB_RETURN(tOp);
}

static CMPIStatus modifyInstance(const CMPIBroker * broker,
                              const CMPIContext * context,
                              const CMPIObjectPath * cop,
                              const CMPIInstance * inst, const char **props)
{
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   const char **p=props;
   ModifyInstanceReq *sreq=NULL;
   OperationHdr oHdr = { OPS_ModifyInstance, 2 };
   CMPIStatus st = { CMPI_RC_OK, NULL };
   int ps,irc,sreqSize=sizeof(ModifyInstanceReq); //-sizeof(MsgSegment);
   ProviderInfo *pInfo;

   _SFCB_ENTER(TRACE_UPCALLS, "modifyInstance");
   
   if (cop && cop->hdl && inst && inst->hdl) {
   
      lockUpCall(broker);
   
      for (ps=0,p=props; p && *p; p++,ps++) 
         sreqSize+=sizeof(MsgSegment);      
      sreq=(ModifyInstanceReq*)calloc(1,sreqSize);
      sreq->hdr.count=ps+3;
      sreq->hdr.operation=OPS_ModifyInstance;
      
      setContext(&binCtx, &oHdr, &sreq->hdr, sreqSize, context, cop);
      _SFCB_TRACE(1,("--- for %s %s",(char*)oHdr.nameSpace.data,(char*)oHdr.className.data));
      
      checkReroute(broker, context, &oHdr); 
      
      sreq->instance = setInstanceMsgSegment(inst);
      for (ps=0,p=props; p && *p; p++,ps++)   
         sreq->properties[ps]=setCharsMsgSegment(*p);

      irc = getProviderContext(&binCtx, &oHdr);

      if (irc == MSG_X_PROVIDER) {
      for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
         if (pInfo->provIds.ids == binCtx.provA.ids.ids) {
            CMPIResult *result = native_new_CMPIResult(0,1,NULL);
            unlockUpCall(broker);
	    if (pInfo->initialized == 0) {
	      initProvider(pInfo,binCtx.bHdr->sessionId);
	    }
            st = pInfo->instanceMI->ft->modifyInstance(pInfo->instanceMI,context,result,cop,inst,props);
            if (sreq) free(sreq);
            return st;   
         }
      }
      
         resp = invokeProvider(&binCtx);
         closeProviderContext(&binCtx);
         resp->rc--;
         buildStatus(resp,&st);
	 free(resp);
      }
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
   }
   else st.rc = CMPI_RC_ERR_FAILED;
   if (sreq) free(sreq);

   _SFCB_TRACE(1,("--- rc: %d",st.rc));
   _SFCB_RETURN(st);
}

static CMPIStatus deleteInstance(const CMPIBroker * broker,
                                 const CMPIContext * context, const CMPIObjectPath * cop)
{
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   DeleteInstanceReq sreq = BINREQ(OPS_DeleteInstance, 2);
   OperationHdr oHdr = { OPS_DeleteInstance, 2 };
   CMPIStatus st = { CMPI_RC_OK, NULL };
   int irc;
   ProviderInfo *pInfo;

   _SFCB_ENTER(TRACE_UPCALLS, "deleteInstance");
   
   if (cop && cop->hdl) {
   
      lockUpCall(broker);
   
      setContext(&binCtx, &oHdr, &sreq.hdr, sizeof(sreq), context, cop);
      _SFCB_TRACE(1,("--- for %s %s",(char*)oHdr.nameSpace.data,(char*)oHdr.className.data));
      
      checkReroute(broker, context, &oHdr); 
      
      irc = getProviderContext(&binCtx, &oHdr);

      if (irc == MSG_X_PROVIDER) {
      for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
         if (pInfo->provIds.ids == binCtx.provA.ids.ids) {
            CMPIResult *result = native_new_CMPIResult(0,1,NULL);
            unlockUpCall(broker);
	    if (pInfo->initialized == 0) {
	      initProvider(pInfo,binCtx.bHdr->sessionId);
	    }
            st = pInfo->instanceMI->ft->deleteInstance(pInfo->instanceMI,context,result,cop);
            return st;   
         }
      }
      
         resp = invokeProvider(&binCtx);
         closeProviderContext(&binCtx);
         resp->rc--;
         buildStatus(resp,&st);
	 free(resp);
      }
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
   }
   else st.rc = CMPI_RC_ERR_FAILED;
   
   _SFCB_TRACE(1,("--- rc: %d",st.rc));  
   _SFCB_RETURN(st);
}


static CMPIEnumeration *execQuery(const CMPIBroker * broker,
                                  const CMPIContext * context,
                                  const CMPIObjectPath * cop, const char *query,
                                  const char *lang, CMPIStatus * rc)
{
   BinRequestContext binCtx;
   ExecQueryReq sreq = BINREQ(OPS_ExecQuery, 4);
   OperationHdr oHdr = { OPS_ExecQuery, 2 };
   CMPIStatus st = { CMPI_RC_OK, NULL },rci = { CMPI_RC_OK, NULL};
   CMPIEnumeration *enm = NULL;
   CMPIArray *ar = NULL;
   int irc, i, c;

   _SFCB_ENTER(TRACE_UPCALLS, "execQuery");
   
   if (cop && cop->hdl) {
   
      lockUpCall(broker);
   
      setContext(&binCtx, &oHdr, &sreq.hdr, sizeof(sreq), context, cop);
      _SFCB_TRACE(1,("--- for %s %s",(char*)oHdr.nameSpace.data,(char*)oHdr.className.data)); 
      
      sreq.query = setCharsMsgSegment(query);
      sreq.queryLang = setCharsMsgSegment(lang);
      
      irc = getProviderContext(&binCtx, &oHdr);

      if (irc == MSG_X_PROVIDER) {
         BinResponseHdr *resp;
         ProviderInfo *pInfo;
         int local;
         ar = TrackedCMPIArray(0, CMPI_instance, NULL);
         for (resp=NULL, c=0, i = 0; i < binCtx.pCount; i++,binCtx.pDone++) {
            local=0;
            binCtx.provA = binCtx.pAs[i];
            for (pInfo = activProvs; pInfo; pInfo = pInfo->next) {
               if (pInfo->provIds.ids == binCtx.provA.ids.ids) {
                  CMPIResult *result = native_new_CMPIResult(0,1,NULL);
                  local=1;
                  unlockUpCall(broker);
		  if (pInfo->initialized == 0) {
		    initProvider(pInfo,binCtx.bHdr->sessionId);
		  }
                  rci = pInfo->instanceMI->ft->execQuery(
                     pInfo->instanceMI, context, result, cop, query, lang);
                  lockUpCall(broker);
                  if (rci.rc == CMPI_RC_OK) cpyResult(result, ar, &c);  
                  else st=rci; 
                  break;
               }
            } 
            if (local) continue;  
            
            resp = invokeProvider(&binCtx);
            
            resp->rc--;
            rci.rc = resp->rc;
            if (resp->rc == CMPI_RC_OK) cpyResponse(resp, ar, &c, CMPI_instance);
            else st=rci; 
            free (resp);
         }
         closeProviderContext(&binCtx);
         enm = sfcb_native_new_CMPIEnumeration(ar, NULL);
      }  
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
   }
   else st.rc = CMPI_RC_ERR_FAILED;
   
   if (rc) *rc = st;
   
   _SFCB_TRACE(1,("--- rc: %d",st.rc));
   _SFCB_RETURN(enm);
}

static CMPIEnumeration *enumInstances(const CMPIBroker * broker,
                                      const CMPIContext * context,
                                      const CMPIObjectPath * cop,
                                      const char **props, CMPIStatus * rc)
{
   EnumInstancesReq sreq = BINREQ(OPS_EnumerateInstances, 2);
   OperationHdr oHdr = { OPS_EnumerateInstances, 2 };
   
   return genericEnumRequest(broker, context, cop, props, NULL, NULL, NULL,
                             NULL, OPS_EnumerateInstances, &sreq.hdr, &oHdr,
                             sizeof(sreq), CMPI_instance, rc);
}

static CMPIEnumeration *enumInstanceNames(const CMPIBroker * broker,
                                      const CMPIContext * context,
                                      const CMPIObjectPath * cop, CMPIStatus * rc)
{
   EnumInstanceNamesReq sreq = BINREQ(OPS_EnumerateInstanceNames, 2);
   OperationHdr oHdr = { OPS_EnumerateInstanceNames, 2 };
   
   return genericEnumRequest(broker, context, cop, NULL, NULL, NULL, NULL,
                             NULL, OPS_EnumerateInstanceNames, &sreq.hdr, &oHdr,
                             sizeof(sreq), CMPI_ref, rc);
}

//---------------------------------------------------
//---
//-     Operations requests handlers - Associations
//---
//---------------------------------------------------



static CMPIEnumeration *associators(const CMPIBroker * broker,
                                    const CMPIContext * context,
                                    const CMPIObjectPath * cop,
                                    const char *assocclass,
                                    const char *resultclass,
                                    const char *role,
                                    const char *resultrole, const char **props,
                                    CMPIStatus * rc)
{
   AssociatorsReq sreq = BINREQ(OPS_Associators, 6);
   OperationHdr oHdr = { OPS_Associators, 6 };
   
   sreq.resultClass = setCharsMsgSegment(resultclass);
   sreq.role = setCharsMsgSegment(role);
   sreq.assocClass = setCharsMsgSegment(assocclass);
   sreq.resultRole = setCharsMsgSegment(resultrole);
   
   return genericEnumRequest(broker, context, cop, props, assocclass,
                             resultclass, role, resultrole, OPS_Associators,
                             &sreq.hdr, &oHdr, sizeof(sreq), CMPI_instance, rc);
   
}

static CMPIEnumeration *associatorNames(const CMPIBroker * broker,
                                        const CMPIContext * context,
                                        const CMPIObjectPath * cop,
                                        const char *assocclass,
                                        const char *resultclass,
                                        const char *role,
                                        const char *resultrole, CMPIStatus * rc)
{
   AssociatorNamesReq sreq = BINREQ(OPS_AssociatorNames, 6);
   OperationHdr oHdr = { OPS_AssociatorNames, 6 };
   
   sreq.resultClass = setCharsMsgSegment(resultclass);
   sreq.role = setCharsMsgSegment(role);
   sreq.assocClass = setCharsMsgSegment(assocclass);
   sreq.resultRole = setCharsMsgSegment(resultrole);
   
   return genericEnumRequest(broker, context, cop, NULL, assocclass, resultclass,
                             role, resultrole, OPS_AssociatorNames, &sreq.hdr, &oHdr,
                             sizeof(sreq), CMPI_ref, rc);
}

static CMPIEnumeration *references(const CMPIBroker * broker,
                                   const CMPIContext * context,
                                   const CMPIObjectPath * cop,
                                   const char *resultclass,
                                   const char *role, const char **props,
                                   CMPIStatus * rc)
{
   ReferencesReq sreq = BINREQ(OPS_References, 4);
   OperationHdr oHdr = { OPS_References, 4 };
   
   sreq.role = setCharsMsgSegment(role);
   sreq.resultClass = setCharsMsgSegment(resultclass);
   
   return genericEnumRequest(broker, context, cop, props, NULL,
                             resultclass, role, NULL, OPS_References,
                             &sreq.hdr, &oHdr, sizeof(sreq), CMPI_instance, rc);
}

static CMPIEnumeration *referenceNames(const CMPIBroker * broker,
                                       const CMPIContext * context,
                                       const CMPIObjectPath * cop,
                                       const char *resultclass,
                                       const char *role, CMPIStatus * rc)
{
   ReferenceNamesReq sreq = BINREQ(OPS_ReferenceNames, 4);
   OperationHdr oHdr = { OPS_ReferenceNames, 4 };
   
   sreq.role = setCharsMsgSegment(role);
   sreq.resultClass = setCharsMsgSegment(resultclass);
   
   return genericEnumRequest(broker, context, cop, NULL, NULL, resultclass,
                             role, NULL, OPS_ReferenceNames, &sreq.hdr, &oHdr,
                             sizeof(sreq), CMPI_ref, rc);
}



//---------------------------------------------------
//---
//-     Operations requests handlers - Method
//---
//---------------------------------------------------

struct native_args {
   CMPIArgs args;   
   int mem_state;   
};


static CMPIData invokeMethod(const CMPIBroker * broker, const CMPIContext * context,
                             const CMPIObjectPath * cop, const char *method,
                             const CMPIArgs * in, CMPIArgs * out, CMPIStatus * rc)
{
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   InvokeMethodReq *sreq; // = BINREQ(OPS_InvokeMethod, 5);
   OperationHdr oHdr = { OPS_InvokeMethod, 2 };
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIArgs *tOut;
   int irc,i,s,x=0,size,n;
   CMPIString *name;
   CMPIData rv = { 0, CMPI_nullValue, {0} };

   if (cop && cop->hdl) {
     
      if (in == NULL) {
	in = CMNewArgs(broker,NULL);
      }
   
      for (i=0,s=CMGetArgCount(in,NULL); i<s; i++) {
         CMPIData d=CMGetArgAt(in,i,NULL,NULL);
         if (d.type==CMPI_instance) x++;
      }
   
      size=sizeof(InvokeMethodReq)+(x*sizeof(MsgSegment));
      sreq=(InvokeMethodReq*)calloc(1,size);
      sreq->hdr.count=5+x;
      sreq->hdr.operation=OPS_InvokeMethod;
      
      lockUpCall(broker);
      
      setContext(&binCtx, &oHdr, &sreq->hdr, size, context, cop);
      
      sreq->in = setArgsMsgSegment(in);
      sreq->out = setArgsMsgSegment(NULL);
      sreq->method = setCharsMsgSegment(method);
      
      if (x) for (n=5,i=0,s=CMGetArgCount(in,NULL); i<s; i++) {
         CMPIData d=CMGetArgAt(in,i,NULL,NULL);
         BinRequestHdr *req=(BinRequestHdr*)sreq;
         if (d.type==CMPI_instance) {
            req->object[n++]=setInstanceMsgSegment(d.value.inst);
         }
      }

      irc = getProviderContext(&binCtx, &oHdr);

      if (irc == MSG_X_PROVIDER) {

         resp = invokeProvider(&binCtx);
         closeProviderContext(&binCtx);
         
         resp->rc--;
         buildStatus(resp,&st);
         if (resp->rc == CMPI_RC_OK) {
            if (out) {
               tOut = relocateSerializedArgs(resp->object[0].data);
              for (i=0,s=tOut->ft->getArgCount(tOut,NULL); i<s; i++) {
                  CMPIData data=tOut->ft->getArgAt(tOut,i,&name,NULL);
                  out->ft->addArg(out,CMGetCharPtr(name),&data.value,data.type);
               }
            }
            if (resp->rvValue) {
               if (resp->rv.type==CMPI_chars) {
                  resp->rv.value.chars=(long)resp->rvEnc.data+(char*)resp;
               }
               else if (resp->rv.type==CMPI_dateTime) {
                  resp->rv.value.dateTime =
                          sfcb_native_new_CMPIDateTime_fromChars(
                                                   (long)resp->rvEnc.data
                                                 + (char*)resp,NULL);
               }
            }
            rv=resp->rv;
         }
	 free(resp);
      }
      else st = setErrorStatus(irc);
      
      unlockUpCall(broker);
      if (sreq) free(sreq);
   }
   else st.rc = CMPI_RC_ERR_FAILED;

   if (rc) *rc = st;
   return rv;
   
}



//---------------------------------------------------
//---
//-     Operations requests handlers - Property
//---
//---------------------------------------------------



static CMPIStatus setProperty(const CMPIBroker * broker,
                              const CMPIContext * context,
                              const CMPIObjectPath * cop, const char *name,
                              const CMPIValue * value, CMPIType type)
{
   CMPIStatus rci = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   return rci;
}

static CMPIData getProperty(const CMPIBroker * broker, const CMPIContext * context,
                            const CMPIObjectPath * cop, const char *name,
                            CMPIStatus * rc)
{
   CMPIStatus rci = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   CMPIData rv = { 0, CMPI_nullValue, {0} };
   if (rc)
      *rc = rci;
   return rv;
}


static CMPIBrokerFT request_FT = {
   0, 0,
   "RequestHandler",
   prepareAttachThread,
   attachThread,
   detachThread,
   deliverIndication,
   enumInstanceNames,
   getInstance,
   createInstance,
   modifyInstance,
   deleteInstance,
   execQuery,
   enumInstances,
   associators,
   associatorNames,
   references,
   referenceNames,
   invokeMethod,
   setProperty,
   getProperty
};

CMPIBrokerFT *RequestFT = &request_FT;

extern CMPIBrokerExtFT brokerExt_FT;

static CMPIBroker _broker = {
   NULL,
   &request_FT,
   &native_brokerEncFT,
   &brokerExt_FT
};

CMPIBroker *Broker = &_broker;
