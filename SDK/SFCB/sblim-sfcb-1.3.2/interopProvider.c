
/*
 * interopProvider.c
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
 * InternalProvider for sfcb.
 *
*/


#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fileRepository.h"
#include "utilft.h"
#include "trace.h"
#include "queryOperation.h"
#include "providerMgr.h"
#include "internalProvider.h"
#include "native.h"
#include "objectpath.h"
#include <time.h>

#define LOCALCLASSNAME "InteropProvider"

/* ------------------------------------------------------------------------- */

extern CMPIInstance *relocateSerializedInstance(void *area);
extern char *sfcb_value2Chars(CMPIType type, CMPIValue * value);
extern CMPIObjectPath *getObjectPath(char *path, char **msg);


extern void closeProviderContext(BinRequestContext* ctx);
extern void setStatus(CMPIStatus *st, CMPIrc rc, char *msg);
extern int testNameSpace(char *ns, CMPIStatus *st);

/* ------------------------------------------------------------------------- */

static const CMPIBroker *_broker;
static int firstTime=1;


typedef struct filter {
   CMPIInstance *fci;
   QLStatement *qs;
   int useCount;
   char *query;
   char *lang;
   char *type;
   char *sns;
} Filter;

typedef struct handler {
   CMPIInstance *hci;
   CMPIObjectPath *hop;
   int useCount;
} Handler;

typedef struct subscription {
   CMPIInstance *sci;
   Filter *fi;
   Handler *ha;
} Subscription;

static UtilHashTable *filterHt = NULL;
static UtilHashTable *handlerHt = NULL;
static UtilHashTable *subscriptionHt = NULL;

/* ------------------------------------------------------------------------- */

static int interOpNameSpace(
	const CMPIObjectPath * cop,
	CMPIStatus * st) 
 {   
   char *ns = (char*)CMGetNameSpace(cop,NULL)->hdl;   
   if (strcasecmp(ns,"root/interop") && strcasecmp(ns,"root/pg_interop")) {
      if (st) setStatus(st, CMPI_RC_ERR_FAILED, "Object must reside in root/interop");
      return 0;
   }
   return 1;
}
   
/* ------------------------------------------------------------------------- */

static CMPIContext* prepareUpcall(CMPIContext *ctx)
{
    /* used to invoke the internal provider in upcalls, otherwise we will
     * be routed here (interOpProvider) again*/
    CMPIContext *ctxLocal;
    ctxLocal = native_clone_CMPIContext(ctx);
    CMPIValue val;
    val.string = sfcb_native_new_CMPIString("$DefaultProvider$", NULL,0);
    ctxLocal->ft->addEntry(ctxLocal, "rerouteToProvider", &val, CMPI_string);
    return ctxLocal;
}

/* ------------------------------------------------------------------------- */

static Subscription *addSubscription(
	const CMPIInstance * ci,
	const char * key,
	Filter * fi,
	Handler * ha)
{
   Subscription *su;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "addSubscription");
   
   if (subscriptionHt == NULL) {
      subscriptionHt=UtilFactory->newHashTable(61,UtilHashTable_charKey);
      subscriptionHt->ft->setReleaseFunctions(subscriptionHt, free, NULL);
   }

   _SFCB_TRACE(1,("-- Subscription: %s\n",key));
   
   su=subscriptionHt->ft->get(subscriptionHt,key);
   if (su) _SFCB_RETURN(NULL);
   
   su=(Subscription*)malloc(sizeof(Subscription));
   su->sci=CMClone(ci,NULL);
   su->fi=fi;
   fi->useCount++;
   su->ha=ha;
   ha->useCount++;
   subscriptionHt->ft->put(subscriptionHt,key,su);
   
   _SFCB_RETURN(su);
}

/* ------------------------------------------------------------------------- */

static Subscription *getSubscription(
	char * key)
{
   Subscription *su;

   _SFCB_ENTER(TRACE_INDPROVIDER, "getSubscription");

   if (subscriptionHt==NULL) return NULL;
   su = subscriptionHt->ft->get(subscriptionHt,key);

   _SFCB_RETURN(su);
}

/* ------------------------------------------------------------------------- */

static void removeSubscription(
	Subscription * su,
	char * key)
{
   _SFCB_ENTER(TRACE_INDPROVIDER, "removeSubscription");

   if (subscriptionHt) {
      subscriptionHt->ft->remove(subscriptionHt,key);
      if (su) {
        if (su->fi) su->fi->useCount--;
        if (su->ha) su->ha->useCount--;
      }
   }
   if (su) {
      CMRelease(su->sci);
      free (su);
   }

   _SFCB_EXIT();
}

/* ------------------------------------------------------------------------- */

static Filter *addFilter(
	const CMPIInstance * ci,
	const char * key,
	QLStatement * qs, 
	const char * query,
	const char * lang,
	const char * sns)
{
   Filter * fi;
      
   _SFCB_ENTER(TRACE_INDPROVIDER, "addFilter");
   
   _SFCB_TRACE(1,("--- Filter: >%s<",key));
   _SFCB_TRACE(1,("--- query: >%s<",query));
   
   if (filterHt==NULL) {
      filterHt=UtilFactory->newHashTable(61,UtilHashTable_charKey);
      filterHt->ft->setReleaseFunctions(filterHt, free, NULL);
   }
      
   fi=filterHt->ft->get(filterHt,key);
   if (fi) _SFCB_RETURN(NULL);
   
   fi=(Filter*)malloc(sizeof(Filter));
   fi->fci=CMClone(ci,NULL);
   fi->useCount=0;
   fi->qs=qs;
   fi->query=strdup(query);
   fi->lang=strdup(lang);
   fi->sns=strdup(sns);
   fi->type=NULL;
   filterHt->ft->put(filterHt,key,fi);
   _SFCB_RETURN(fi);
}

/* ------------------------------------------------------------------------- */

static Filter *getFilter(
	char * key)
{
   Filter * fi;

   _SFCB_ENTER(TRACE_INDPROVIDER, "getFilter");
   _SFCB_TRACE(1,("--- Filter: >%s<",key));
   
   if (filterHt==NULL) return NULL;
   fi = filterHt->ft->get(filterHt,key);

   _SFCB_RETURN(fi);
}

/* ------------------------------------------------------------------------- */

static void removeFilter(
	Filter * fi,
	char * key)
{
   _SFCB_ENTER(TRACE_INDPROVIDER, "removeFilter");

   if (filterHt) {
      filterHt->ft->remove(filterHt,key);
   }
   if (fi) {
      CMRelease(fi->fci);
      CMRelease(fi->qs);
      free(fi->query);
      free(fi->lang);
      free(fi->sns);
      free (fi);
   }   

   _SFCB_EXIT();
}

/* ------------------------------------------------------------------------- */

static Handler *addHandler(
	CMPIInstance *ci,
	CMPIObjectPath * op)
{
   Handler *ha; 
   char *key;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "addHandler");
   
   if (handlerHt==NULL) {
      handlerHt=UtilFactory->newHashTable(61,UtilHashTable_charKey);
       handlerHt->ft->setReleaseFunctions(handlerHt, free, NULL);
   }
      
   key=normalizeObjectPathCharsDup(op);
      
   _SFCB_TRACE(1,("--- Handler: %s",key));
   
   if ((ha=handlerHt->ft->get(handlerHt,key))!=NULL) {
      _SFCB_TRACE(1,("--- Handler already registered %p",ha));
      if(key) free(key);
      _SFCB_RETURN(NULL);
   }

   ha=(Handler*)malloc(sizeof(Handler));
   ha->hci=CMClone(ci,NULL);
   ha->hop=CMClone(op,NULL);
   ha->useCount=0;
   handlerHt->ft->put(handlerHt,key,ha);
   
   _SFCB_RETURN(ha);
}

/* ------------------------------------------------------------------------- */

static Handler *getHandler(
	char * key)
{
   Handler *ha;

   _SFCB_ENTER(TRACE_INDPROVIDER, "getHandler");

   if (handlerHt==NULL) return NULL;
   ha=handlerHt->ft->get(handlerHt,key);

   _SFCB_RETURN(ha);
}

/* ------------------------------------------------------------------------- */

static void removeHandler(
	Handler * ha,
	char * key)
{
   _SFCB_ENTER(TRACE_INDPROVIDER, "removeHandler");

   if (handlerHt) {
      handlerHt->ft->remove(handlerHt,key);
   }
   if (ha) {
      CMRelease(ha->hci);
      CMRelease(ha->hop);
      free (ha);
   }

   _SFCB_EXIT();
}

/* ------------------------------------------------------------------------- */

extern int isChild(const char *ns, const char *parent, const char* child);

static int isa(const char *sns, const char *child, const char *parent)
{
   int rv;
   _SFCB_ENTER(TRACE_INDPROVIDER, "isa");
   
   if (strcasecmp(child,parent)==0) return 1;
   rv=isChild(sns,parent,child);
   _SFCB_RETURN(rv);
}

/* ------------------------------------------------------------------------- */

extern CMPISelectExp *TempCMPISelectExp(QLStatement *qs);

/*
 * This generic function serves 4 kinds of request, specified by optype:
 * OPS_ActivateFilter     28
 * OPS_DeactivateFilter   29
 * OPS_DisableIndications 30
 * OPS_EnableIndications  31
 */
CMPIStatus genericSubscriptionRequest(
	const char * principal,
	const char * cn,
	const char * type,
	Filter * fi,
	int optype,
	int * rrc)
{
   CMPIObjectPath *path;
   CMPIStatus st={CMPI_RC_OK,NULL},rc;
   IndicationReq sreq = BINREQ(optype, 6);
   BinResponseHdr **resp=NULL;
   BinRequestContext binCtx;
   OperationHdr req = {OPS_IndicationLookup, 2};
   int irc=0,err,cnt,i;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "genericSubscriptionRequest");
   _SFCB_TRACE(4, ("principal %s, class %s, type %s, optype %d",principal, cn, type, optype));
   
   if (rrc) *rrc=0;
   path = TrackedCMPIObjectPath(fi->sns, cn, &rc);
   
   sreq.principal = setCharsMsgSegment(principal);
   sreq.objectPath = setObjectPathMsgSegment(path);
   sreq.query = setCharsMsgSegment(fi->query);
   sreq.language = setCharsMsgSegment(fi->lang);
   sreq.type = setCharsMsgSegment((char*)type);
   fi->type=strdup(type);
   sreq.sns = setCharsMsgSegment(fi->sns);
   sreq.filterId=fi;

   req.nameSpace = setCharsMsgSegment(fi->sns);
   req.className = setCharsMsgSegment((char*) cn);

   memset(&binCtx,0,sizeof(BinRequestContext));
   binCtx.oHdr = &req;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=0;

   _SFCB_TRACE(1, ("--- getProviderContext for %s-%s",fi->sns,cn));
   
   irc = getProviderContext(&binCtx, &req);

   if (irc == MSG_X_PROVIDER) {      
      _SFCB_TRACE(1, ("--- Invoking Providers"));
      /* one good provider makes success */
      resp = invokeProviders(&binCtx,&err,&cnt);
      if (err == 0) {
	setStatus(&st,0,NULL);
      } else {
	setStatus(&st,resp[err-1]->rc,NULL);
	for (i=0; i<binCtx.pCount; i++) {
	  if (resp[i]->rc == 0) {
	    setStatus(&st,0,NULL);
	    break;
	  }
	}
      }
   }
   
   else {
      if (rrc) *rrc=irc;
      if (irc==MSG_X_PROVIDER_NOT_FOUND) setStatus(&st,CMPI_RC_ERR_FAILED,
         "No eligible indication provider found");
      else {
         char msg[512];
         snprintf(msg,511,"Failing to find eligible indication provider. Rc: %d",irc);
         setStatus(&st,CMPI_RC_ERR_FAILED,msg);
      }   
   }   
   
   if (resp) {
      cnt = binCtx.pCount;
      while (cnt--) {
        if (resp[cnt]) {
	  free(resp[cnt]);
        }
      }
      free(resp);
      closeProviderContext(&binCtx);
  }
   if(fi->type) {
      free(fi->type);
   }
 
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

int fowardSubscription(
	const CMPIContext * ctx,
	Filter * fi,
	int optype,
	CMPIStatus * st)
{
   CMPIStatus rc;
   char *principal = NULL;
   char **fClasses = fi->qs->ft->getFromClassList(fi->qs);
   CMPIData principalP = ctx->ft->getEntry(ctx,CMPIPrincipal,&rc);
   int irc;
   int activated = 0;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "fowardSubscription");
   
   if (rc.rc == CMPI_RC_OK) { 
      principal = (char*)principalP.value.string->hdl;
      _SFCB_TRACE(1,("--- principal=\"%s\"", principal));
   }

   /* Go thru all the indication classes specified in the filter query and activate each */
   for ( ; *fClasses; fClasses++) {
      _SFCB_TRACE(1,("--- indication class=\"%s\" namespace=\"%s\"", *fClasses, fi->sns));

      /* Check if this is a process indication */
      if (isa(fi->sns, *fClasses, "CIM_ProcessIndication") ||
          isa(fi->sns, *fClasses, "CIM_InstCreation") ||
          isa(fi->sns, *fClasses, "CIM_InstDeletion") ||
          isa(fi->sns, *fClasses, "CIM_InstModification")) {
         *st = genericSubscriptionRequest(principal, *fClasses, *fClasses, fi, optype, &irc);
         if (st->rc == CMPI_RC_OK) activated++; 
      }

      /* Warn if this indication class is unknown and continue processing the rest, if any */ 
      else {
         _SFCB_TRACE(1,("--- Unsupported/unrecognized indication class"));
      }
   }

   /* Make sure at least one of the indication classes were successfully activated */
   if (!activated) {
      setStatus(st, CMPI_RC_ERR_NOT_SUPPORTED, 
                    "No supported indication classes in filter query"\
                     " or no provider found");
      _SFCB_RETURN(-1);
   } 
     
   setStatus(st, CMPI_RC_OK, NULL);
   _SFCB_RETURN(0);
}

/* ------------------------------------------------------------------------- */

extern UtilStringBuffer *instanceToString(CMPIInstance * ci, char **props);

CMPIStatus switchIndications(const CMPIContext *ctx,
	const CMPIInstance *ci,
	int optype)
{
	CMPIStatus st={CMPI_RC_OK,NULL};
	Filter *fi;
	CMPIObjectPath *op;
	char *key;
	
	_SFCB_ENTER(TRACE_INDPROVIDER, "enableIndications()");
	
	op = CMGetProperty(ci, "filter", &st).value.ref;
	key = normalizeObjectPathCharsDup(op);
	fi = getFilter(key);
	if(key) free(key);
	
	fowardSubscription(ctx, fi, optype, &st);

	_SFCB_RETURN(st);
}

static CMPIStatus processSubscription(
	const CMPIBroker *broker,
	const CMPIContext *ctx, 
	const CMPIInstance *ci,
	const CMPIObjectPath *cop)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   Handler *ha;
   Filter *fi;
   Subscription *su;
   CMPIObjectPath *op;
   char *key,*skey;
   CMPIDateTime *dt;
   CMPIValue val;
    
   _SFCB_ENTER(TRACE_INDPROVIDER, "processSubscription()");
   
   _SFCB_TRACE(1,("--- checking for existing subscription"));
   skey = normalizeObjectPathCharsDup(cop);
   if (getSubscription(skey)) {
      _SFCB_TRACE(1,("--- subscription already exists"));
      if(skey) free(skey);
      setStatus(&st, CMPI_RC_ERR_ALREADY_EXISTS, NULL);
      _SFCB_RETURN(st); 
   }      
      
   _SFCB_TRACE(1,("--- getting new subscription filter"));
   op = CMGetProperty(ci, "filter", &st).value.ref;
   key = normalizeObjectPathCharsDup(op);
   fi = getFilter(key);
   if(key) free(key);
   
   if (fi == NULL) {
      _SFCB_TRACE(1,("--- cannot find specified subscription filter"));
      setStatus(&st, CMPI_RC_ERR_NOT_FOUND, "Filter not found");
      _SFCB_RETURN(st);
   }
   
   _SFCB_TRACE(1,("--- getting new subscription handle"));
   op = CMGetProperty(ci, "handler", &st).value.ref;
   key = normalizeObjectPathCharsDup(op);
   ha = getHandler(key);
   if(key) free(key);
      
   if (ha == NULL) {
      _SFCB_TRACE(1,("--- cannot find specified subscription handler"));
      setStatus(&st, CMPI_RC_ERR_NOT_FOUND, "Handler not found");
      _SFCB_RETURN(st);
   }

   _SFCB_TRACE(1,("--- setting subscription start time"));
   dt = CMNewDateTime(_broker,NULL);
   CMSetProperty((CMPIInstance *)ci, "SubscriptionStartTime", &dt, CMPI_dateTime);
   
   su=addSubscription(ci, skey, fi, ha); 
   fowardSubscription(ctx, fi, OPS_ActivateFilter, &st);
   
   if (st.rc != CMPI_RC_OK) removeSubscription(su, skey);
   //activation succesful, try to enable it
   else {
   	  //check property
   	  CMPIData d = CMGetProperty(ci, "SubscriptionState", &st);
   	  if(d.state == CMPI_goodValue) {
   	     if(d.value.uint16 == 2) { //==enabled
   	        fowardSubscription(ctx, fi, OPS_EnableIndications, &st);
   	     }
   	  } else {
      /* property not set, assume "enable" by default */
         val.uint16 = 2;
         st = CMSetProperty((CMPIInstance*)ci, "SubscriptionState", &val, CMPI_uint16);
         fowardSubscription(ctx, fi, OPS_EnableIndications, &st);
      }
   }   
      
   _SFCB_RETURN(st); 
}

/* ------------------------------------------------------------------ *
 * InterOp initialization
 * ------------------------------------------------------------------ */

void initInterOp(
	const CMPIBroker *broker,
	const CMPIContext *ctx)
{
   CMPIObjectPath *op;
   CMPIEnumeration *enm;
   CMPIInstance *ci;
   CMPIStatus st;
   CMPIObjectPath *cop;
   CMPIContext *ctxLocal;
   char *key,*query,*lng,*sns;
   QLStatement *qs=NULL;
   int rc;
    
   _SFCB_ENTER(TRACE_INDPROVIDER, "initInterOp");
   
   firstTime=0;
    
   _SFCB_TRACE(1,("--- checking for cim_indicationfilter"));
   op=CMNewObjectPath(broker,"root/interop","cim_indicationfilter",&st);
   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->enumerateInstances(_broker, ctxLocal, op, NULL, &st);
   CMRelease(ctxLocal);
   
   if(enm) {
      while(enm->ft->hasNext(enm, &st) && (ci=(enm->ft->getNext(enm, &st)).value.inst)) {
         cop=CMGetObjectPath(ci,&st);
         query=(char*)CMGetProperty(ci,"query",&st).value.string->hdl;
         lng=(char*)CMGetProperty(ci,"querylanguage",&st).value.string->hdl;
         sns=(char*)CMGetProperty(ci,"SourceNamespace",&st).value.string->hdl;
         qs=parseQuery(MEM_NOT_TRACKED,query,lng,sns,&rc);
         key=normalizeObjectPathCharsDup(cop);
         addFilter(ci,key,qs,query,lng,sns);
      }
      CMRelease(enm);
   }  

   _SFCB_TRACE(1,("--- checking for cim_listenerdestination"));
   op=CMNewObjectPath(broker,"root/interop","cim_listenerdestination",&st);
   enm = _broker->bft->enumerateInstances(_broker, ctx, op, NULL, &st);
   
   if(enm) {
      while(enm->ft->hasNext(enm, &st) && (ci=(enm->ft->getNext(enm, &st)).value.inst)) {
         cop=CMGetObjectPath(ci,&st); 
         addHandler(ci,cop);
      }
      CMRelease(enm);
   } 
   _SFCB_TRACE(1,("--- checking for cim_indicationsubscription"));
   op=CMNewObjectPath(broker,"root/interop","cim_indicationsubscription",&st);
   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->enumerateInstances(_broker, ctxLocal, op, NULL, &st);
   CMRelease(ctxLocal);
   
   if(enm) {
      while(enm->ft->hasNext(enm, &st) && (ci=(enm->ft->getNext(enm, &st)).value.inst)) {      
         CMPIObjectPath *hop;    
         cop=CMGetObjectPath(ci,&st);
         hop=CMGetKey(cop,"handler",NULL).value.ref;
         processSubscription(broker,ctx,ci,cop);
      }
      CMRelease(enm);   
   }
      
   _SFCB_EXIT(); 
}

/* --------------------------------------------------------------------------*/
/*                       Instance Provider Interface                         */
/* --------------------------------------------------------------------------*/
 
CMPIStatus InteropProviderCleanup(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx, CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderCleanup");
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderEnumInstanceNames(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * ref)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIEnumeration *enm;
   CMPIContext *ctxLocal;
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderEnumInstanceNames");

   if (interOpNameSpace(ref,NULL)!=1) _SFCB_RETURN(st);
   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->enumerateInstanceNames(_broker, ctxLocal, ref, &st);
   CMRelease(ctxLocal);
                                      
   while(enm && enm->ft->hasNext(enm, &st)) {
       CMReturnObjectPath(rslt, (enm->ft->getNext(enm, &st)).value.ref);   
   }
   if(enm) CMRelease(enm);
   _SFCB_RETURN(st);   
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderEnumInstances(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * ref,
	const char ** properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIEnumeration *enm;
   CMPIContext *ctxLocal;
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderEnumInstances");

   if (interOpNameSpace(ref,NULL)!=1) _SFCB_RETURN(st);
   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->enumerateInstances(_broker, ctxLocal, ref, properties, &st);
   CMRelease(ctxLocal);
                                      
   while(enm && enm->ft->hasNext(enm, &st)) {
       CMReturnInstance(rslt, (enm->ft->getNext(enm, &st)).value.inst);   
   }   
   if(enm) CMRelease(enm);
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderGetInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const char ** properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIContext *ctxLocal;
   CMPIInstance *ci;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderGetInstance");

   ctxLocal = prepareUpcall((CMPIContext *)ctx);

   ci = _broker->bft->getInstance(_broker, ctxLocal, cop, properties, &st);
   if (st.rc==CMPI_RC_OK) {
      CMReturnInstance(rslt, ci);
   }
   
   CMRelease(ctxLocal);

   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderCreateInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const CMPIInstance * ci)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIString *cn = CMGetClassName(cop, NULL);
   const char *cns = cn->ft->getCharPtr(cn,NULL);
   CMPIString *ns = CMGetNameSpace(cop, NULL);
   const char *nss = ns->ft->getCharPtr(ns,NULL);
   CMPIContext *ctxLocal;
   CMPIInstance *ciLocal;
   CMPIValue valSNS;

   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderCreateInstance");
  
   if (interOpNameSpace(cop,&st)!=1) _SFCB_RETURN(st);
   
   ciLocal = ci->ft->clone(ci, NULL);
   memLinkInstance(ciLocal);
   
   if(isa(nss, cns, "cim_indicationsubscription")) {
   
      _SFCB_TRACE(1,("--- create cim_indicationsubscription"));
      
      st=processSubscription(_broker,ctx,ciLocal,cop);
   }
   else if (isa(nss, cns, "cim_indicationfilter")) {
      QLStatement *qs=NULL;
      int rc,i,n,m;
      char *key=NULL,*ql,lng[16];
      CMPIString *lang=ciLocal->ft->getProperty(ciLocal,"querylanguage",&st).value.string;
      CMPIString *query=ciLocal->ft->getProperty(ciLocal,"query",&st).value.string;
      CMPIString *sns=ciLocal->ft->getProperty(ciLocal,"SourceNamespace",&st).value.string;
      
      _SFCB_TRACE(1,("--- create cim_indicationfilter"));
   
      if (lang == NULL || lang->hdl == NULL
          || query == NULL ||  query->hdl == NULL) {
         setStatus(&st,CMPI_RC_ERR_FAILED,"Query and/or Language property not found");
         _SFCB_RETURN(st);         
      }
      
      if(sns == NULL || sns->hdl == NULL) {
      	  /* if sourcenamespace is NULL, the namespace of the filter
      	   * registration is assumed */
	sns = sfcb_native_new_CMPIString("root/interop", NULL,0);
          valSNS.string = sns;
          ciLocal->ft->setProperty(ciLocal, "SourceNamespace", &valSNS,
                                   CMPI_string);
      }
      
      for (ql=(char*)lang->hdl,i=0,n=0,m=strlen(ql); i<m; i++) {
         if (ql[i]>' ') lng[n++]=ql[i];
         if (n>=15) break;
      }
      lng[n]=0;      
       
      _SFCB_TRACE(2,("--- CIM query language %s %s",lang->hdl,lng));
      if (strcasecmp(lng,"wql") && strcasecmp(lng,"cql") && strcasecmp(lng,"cim:cql")) {
         setStatus(&st,CMPI_RC_ERR_QUERY_LANGUAGE_NOT_SUPPORTED,NULL);
         _SFCB_RETURN(st);  
      }   
              
      key=normalizeObjectPathCharsDup(cop);
      if (getFilter(key)) {
         free(key);
         setStatus(&st,CMPI_RC_ERR_ALREADY_EXISTS,NULL);
         _SFCB_RETURN(st); 
      }
      
      qs=parseQuery(MEM_NOT_TRACKED,(char*)query->hdl,lng,(char*)sns->hdl,&rc);
      if (rc) {
         free(key);
         setStatus(&st,CMPI_RC_ERR_INVALID_QUERY,"Query parse error");
         CMRelease(qs);
         _SFCB_RETURN(st);         
      }

      addFilter(ciLocal,key,qs,(char*)query->hdl,lng,(char*)sns->hdl);
   }
   
   else {
      setStatus(&st,CMPI_RC_ERR_NOT_SUPPORTED,"Class not supported");
      _SFCB_RETURN(st);         
   }
    
   if (st.rc==CMPI_RC_OK) {
      ctxLocal = prepareUpcall((CMPIContext *)ctx);
      CMReturnObjectPath(rslt, _broker->bft->createInstance(_broker, ctxLocal, cop, ciLocal, &st));
      CMRelease(ctxLocal);
   }
    
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderModifyInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const CMPIInstance * ci,
	const char ** properties)
{
	CMPIStatus st = { CMPI_RC_OK, NULL };
	CMPIString *cn = CMGetClassName(cop, NULL);
	const char *cns = cn->ft->getCharPtr(cn,NULL);
	CMPIContext *ctxLocal;
	
	_SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderModifyInstance");
   
   	if(isa("root/interop", cns, "cim_indicationsubscription")) {
		char *key = normalizeObjectPathCharsDup(cop);
		_SFCB_TRACE(1,("--- modify cim_indicationsubscription %s",key));
		Subscription *su;
		CMPIInstance *oldInst;
		 
		/*check if SubscriptionState changed
		  enable/disableIndication */
		su=getSubscription(key);
		free(key);
		if(!su) {
			st.rc = CMPI_RC_ERR_NOT_FOUND;
			return st;      	
		}
		oldInst = su->sci;
		
		CMPIData oldState = CMGetProperty(oldInst, "SubscriptionState", &st);
		CMPIData newState = CMGetProperty(ci, "SubscriptionState", &st);
		
		if(newState.state == CMPI_goodValue) {
			if(newState.value.uint16 == 2 && oldState.value.uint16 != 2) {
				switchIndications(ctx, ci, OPS_EnableIndications);
			}  
			else if(newState.value.uint16 == 4 && oldState.value.uint16 != 4) {
				switchIndications(ctx, ci, OPS_DisableIndications);
			}
		}
		/*replace the instance in the hashtable*/
		CMRelease(su->sci);
		su->sci=CMClone(ci,NULL);
   	  
	}
	else setStatus(&st,CMPI_RC_ERR_NOT_SUPPORTED,"Class not supported");
	
	if (st.rc==CMPI_RC_OK) {
		ctxLocal = prepareUpcall((CMPIContext *)ctx);
		st = _broker->bft->modifyInstance(_broker, ctxLocal, cop, ci, properties);
		CMRelease(ctxLocal);		
	}
	_SFCB_RETURN(st);   
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderDeleteInstance(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIString *cn = CMGetClassName(cop, NULL);
   const char *cns = cn->ft->getCharPtr(cn,NULL);
   CMPIString *ns = CMGetNameSpace(cop, NULL);
   const char *nss = ns->ft->getCharPtr(ns,NULL);
   char *key = normalizeObjectPathCharsDup(cop);
   Filter *fi;
   Subscription *su;
   CMPIContext *ctxLocal;

   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderDeleteInstance");
   
   if(isa(nss, cns, "cim_indicationsubscription")) {
      _SFCB_TRACE(1,("--- delete cim_indicationsubscription %s",key));
      if ((su=getSubscription(key))) {
         fi=su->fi;
         if (fi->useCount==1) {
            char **fClasses=fi->qs->ft->getFromClassList(fi->qs);
            for ( ; *fClasses; fClasses++) {
	      char *principal=ctx->ft->getEntry(ctx,CMPIPrincipal,NULL).value.string->hdl;
	      genericSubscriptionRequest(principal, *fClasses, cns, fi, OPS_DeactivateFilter, NULL);
            }
         }   
         removeSubscription(su,key);
      }
      else setStatus(&st,CMPI_RC_ERR_NOT_FOUND,NULL);
   }
   
   else if (isa(nss, cns, "cim_indicationfilter")) {
      _SFCB_TRACE(1,("--- delete cim_indicationfilter %s",key));
      
      if ((fi=getFilter(key))) {
         if (fi->useCount) setStatus(&st,CMPI_RC_ERR_FAILED,"Filter in use");
         else removeFilter(fi,key);
      }
      else setStatus(&st,CMPI_RC_ERR_NOT_FOUND,NULL);
   }
   
   else setStatus(&st,CMPI_RC_ERR_NOT_SUPPORTED,"Class not supported");
      
   if (st.rc==CMPI_RC_OK) {
      ctxLocal = prepareUpcall((CMPIContext *)ctx);
      st = _broker->bft->deleteInstance(_broker, ctxLocal, cop);
      CMRelease(ctxLocal);
   }
   
   if (key) free(key);
   
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderExecQuery(
	CMPIInstanceMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const char * lang,
	const char * query)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderExecQuery");
   _SFCB_RETURN(st);
}

/* --------------------------------------------------------------------------*/
/*                        Method Provider Interface                          */
/* --------------------------------------------------------------------------*/

CMPIStatus InteropProviderMethodCleanup(
	CMPIMethodMI * mi,
	const CMPIContext * ctx, CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderMethodCleanup");
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderInvokeMethod(
	CMPIMethodMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * ref,
	const char * methodName,
	const CMPIArgs * in,
	CMPIArgs * out)
{ 
   CMPIStatus st = { CMPI_RC_OK, NULL };
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderInvokeMethod");
   
   if (interOpNameSpace(ref,&st)!=1) _SFCB_RETURN(st);
   
   _SFCB_TRACE(1,("--- Method: %s",methodName)); 

   if (strcasecmp(methodName, "_deliver") == 0) {
      HashTableIterator *i;
      Subscription *su;
      char *suName;
      CMPIArgs *hin=CMNewArgs(_broker,NULL);
      CMPIInstance *ind=CMGetArg(in,"indication",NULL).value.inst;
      void *filterId=(void*)CMGetArg(in,"filterid",NULL).value.
#if SIZEOF_INT == SIZEOF_VOIDP	
	uint32;
#else
        uint64;
#endif
      char *ns=(char*)CMGetArg(in,"namespace",NULL).value.string->hdl;
      
      CMAddArg(hin,"indication",&ind,CMPI_instance);
      CMAddArg(hin,"nameSpace",ns,CMPI_chars);
      
      if (subscriptionHt) for (i = subscriptionHt->ft->getFirst(subscriptionHt, 
                 (void**)&suName, (void**)&su); i; 
              i = subscriptionHt->ft->getNext(subscriptionHt,i, 
                 (void**)&suName, (void**)&su)) {
         if ((void*)su->fi==filterId) {
            CMPIString *str=CDToString(_broker,su->ha->hop,NULL);
            CMPIString *ns=CMGetNameSpace(su->ha->hop,NULL);
            _SFCB_TRACE(1,("--- invoke handler %s %s",(char*)ns->hdl,(char*)str->hdl));
            
            CBInvokeMethod(_broker,ctx,su->ha->hop,"_deliver",hin,NULL,&st);
            _SFCB_TRACE(1,("--- invoke handler status: %d",st.rc));
         }     
      }   
   }
   
   else if (strcasecmp(methodName, "_addHandler") == 0) {
      CMPIInstance *ci=in->ft->getArg(in,"handler",&st).value.inst;
      CMPIObjectPath *op=in->ft->getArg(in,"key",&st).value.ref;
      CMPIString *str=CDToString(_broker,op,NULL);
      CMPIString *ns=CMGetNameSpace(op,NULL);
      _SFCB_TRACE(1,("--- _addHandler %s %s",(char*)ns->hdl,(char*)str->hdl));
      addHandler(ci,op);     
   }
   
   else if (strcasecmp(methodName, "_removeHandler") == 0) {
      CMPIObjectPath *op=in->ft->getArg(in,"key",&st).value.ref;
      char *key=normalizeObjectPathCharsDup(op);
      Handler *ha=getHandler(key);
      if (ha) {
         if (ha->useCount) {
            setStatus(&st,CMPI_RC_ERR_FAILED,"Handler in use");
         } 
         else removeHandler(ha,key);   
      }
      else {
         setStatus(&st, CMPI_RC_ERR_NOT_FOUND, "Handler objectnot found");
      }
      if(key) free(key);
   }

   else if (strcasecmp(methodName, "_startup") == 0) {
      initInterOp(_broker,ctx);
   }

   else {
      _SFCB_TRACE(1,("--- Invalid request method: %s",methodName));
      setStatus(&st, CMPI_RC_ERR_METHOD_NOT_FOUND, "Invalid request method");
   }
   
   _SFCB_RETURN(st);
}

/* --------------------------------------------------------------------------*/
/*                     Association Provider Interface                        */
/* --------------------------------------------------------------------------*/

CMPIStatus InteropProviderAssociationCleanup(
	CMPIAssociationMI * mi,
	const CMPIContext * ctx,
	CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderAssociationCleanup");
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderAssociators(
	CMPIAssociationMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const char * assocClass,
	const char * resultClass,
	const char * role,
	const char * resultRole,
	const char ** propertyList)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIEnumeration *enm;
   CMPIContext *ctxLocal;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderAssociators");
   
   if (interOpNameSpace(cop,&st)!=1) _SFCB_RETURN(st);

   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->associators(_broker, ctxLocal, cop, assocClass, resultClass,
                                   role, resultRole, propertyList, &st);
   CMRelease(ctxLocal);
                                   
   while(enm && enm->ft->hasNext(enm, &st)) {
       CMReturnInstance(rslt, (enm->ft->getNext(enm, &st)).value.inst);   
   }
   
   if(enm) CMRelease(enm);
   
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderAssociatorNames(
	CMPIAssociationMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const char * assocClass,
	const char * resultClass,
	const char * role,
	const char * resultRole)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIEnumeration *enm;
   CMPIContext *ctxLocal;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderAssociatorNames");

   if (interOpNameSpace(cop,&st)!=1) _SFCB_RETURN(st);

   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->associatorNames(_broker, ctxLocal, cop, assocClass, resultClass,
                                   role, resultRole, &st);
   CMRelease(ctxLocal);
                                   
   while(enm && enm->ft->hasNext(enm, &st)) {
       CMReturnObjectPath(rslt, (enm->ft->getNext(enm, &st)).value.ref);
   }
   
   if(enm) CMRelease(enm);
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderReferences(
	CMPIAssociationMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const char * resultClass,
	const char * role,
	const char ** propertyList)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIEnumeration *enm;
   CMPIContext *ctxLocal;   
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderReferences");

   if (interOpNameSpace(cop,NULL)!=1) _SFCB_RETURN(st);
   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->references(_broker, ctxLocal, cop, resultClass, role, propertyList, &st);
   CMRelease(ctxLocal);
                                      
   while(enm && enm->ft->hasNext(enm, &st)) {
       CMReturnInstance(rslt, (enm->ft->getNext(enm, &st)).value.inst);   
   }   
   if(enm) CMRelease(enm);   
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------------- */

CMPIStatus InteropProviderReferenceNames(
	CMPIAssociationMI * mi,
	const CMPIContext * ctx,
	const CMPIResult * rslt,
	const CMPIObjectPath * cop,
	const char * resultClass,
	const char * role)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIEnumeration *enm;
   CMPIContext *ctxLocal;   
   _SFCB_ENTER(TRACE_INDPROVIDER, "InteropProviderReferenceNames");
   
   if (interOpNameSpace(cop,NULL)!=1) _SFCB_RETURN(st);
   ctxLocal = prepareUpcall((CMPIContext *)ctx);
   enm = _broker->bft->referenceNames(_broker, ctxLocal, cop, resultClass, role, &st);
   CMRelease(ctxLocal);
                                      
   while(enm && enm->ft->hasNext(enm, &st)) {
       CMReturnObjectPath(rslt, (enm->ft->getNext(enm, &st)).value.ref);   
   }
   if(enm) CMRelease(enm);
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------ *
 * Instance MI Factory
 *
 * NOTE: This is an example using the convenience macros. This is OK
 *       as long as the MI has no special requirements, i.e. to store
 *       data between calls.
 * ------------------------------------------------------------------ */

CMInstanceMIStub(InteropProvider, InteropProvider, _broker, CMNoHook); 

CMAssociationMIStub(InteropProvider, InteropProvider, _broker, CMNoHook);

CMMethodMIStub(InteropProvider, InteropProvider, _broker, CMNoHook);

