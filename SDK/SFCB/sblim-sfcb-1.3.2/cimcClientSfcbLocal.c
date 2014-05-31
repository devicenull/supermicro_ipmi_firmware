
/*
 * $Id: cimcClientSfcbLocal.c,v 1.24 2008/10/03 00:24:47 buccella Exp $
 *
 * © Copyright IBM Corp. 2006, 2007
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
 * sfcb's implementation of the sfcc CIMC API
 *
 */

#include "cimcClientSfcbLocal.h"
#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"

#include "providerMgr.h"
#include "queryOperation.h"

 
#define CIMCSetStatusWithChars(st,rcp,chars) \
      { if (st) { (st)->rc=(rcp); \
        (st)->msg=NewCMPIString((chars),NULL); }}
        
#define NewCMPIString sfcb_native_new_CMPIString
   
extern unsigned long _sfcb_trace_mask; 

extern void closeProviderContext(BinRequestContext* binCtx);
extern MsgSegment setObjectPathMsgSegment(const CMPIObjectPath * op);
extern MsgSegment setArgsMsgSegment(CMPIArgs * args);
extern MsgSegment setInstanceMsgSegment(const CMPIInstance * inst);
extern CMPIConstClass *relocateSerializedConstClass(void *area);
extern CMPIObjectPath *relocateSerializedObjectPath(void *area);
extern CMPIInstance *relocateSerializedInstance(void *area);
extern CMPIArgs *relocateSerializedArgs(void *area);

extern CMPIString *NewCMPIString(const char*, CMPIStatus*);
extern CMPIObjectPath *NewCMPIObjectPath(const char*ns, const char* cn, CMPIStatus*);
extern CMPIInstance *NewCMPIInstance(CMPIObjectPath * cop, CMPIStatus * rc);
extern CMPIArray *NewCMPIArray(CMPICount size, CMPIType type, CMPIStatus * rc);
extern CMPIEnumeration *sfcb_native_new_CMPIEnumeration(CMPIArray * array, CMPIStatus * rc);
extern CMPIString *NewCMPIString(const char *ptr, CMPIStatus * rc);
extern CMPIEnumeration *NewCMPIEnumeration(CMPIArray * array, CMPIStatus * rc);

extern CMPIArgs* NewCMPIArgs(CMPIStatus* rc);
extern CMPIDateTime *NewCMPIDateTime(CMPIStatus *rc);
extern CMPIDateTime *NewCMPIDateTimeFromBinary(CMPIUint64 binTime, CMPIBoolean interval, CMPIStatus *rc);
extern CMPIDateTime *NewCMPIDateTimeFromChars(const char *utcTime, CMPIStatus *rc);

extern void setInstanceLocalMode(int mode);

extern int localClientMode;

extern void sunsetControl();

#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#undef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

#ifdef DMALLOC
#include "dmalloc.h"
#endif

#if DEBUG
int do_debug = 0;	/* simple enable debug messages flag */

static void set_debug()
{
    static int firsttime = 1;
    char *dbg;
    if (firsttime == 1) {
	firsttime--;
        do_debug = ((dbg = getenv("CMPISFCC_DEBUG")) != NULL &&
		    strcasecmp(dbg, "true") == 0);
    }
}
#endif

#ifdef LARGE_VOL_SUPPORT
/*
 * 
 */
 
extern int  getEnumState(CMPIEnumeration * ) ;
extern void setEnumState(CMPIEnumeration * ,int) ;
#include "sfcbenum.h"

typedef struct thrdInfo {
	CMPIEnumeration * enm;
	Client * mb ;
	CMPIObjectPath * cop ;
	CMPIFlags flags ;
	char ** properties ;
	CMPIStatus * rc ;
} thrdInfo ;
      
void sfcb_native_array_increase_size(const CMPIArray *, CMPICount);
static CMPIEnumeration * enumInstances(Client *,CMPIObjectPath *,	CMPIFlags,char **,CMPIStatus *);
                  void * enumInstancesThrd( thrdInfo * ) ;
static CMPIEnumeration * enumInstanceNames(Client *,CMPIObjectPath *,CMPIStatus *);
                  void * enumInstanceNamesThrd( thrdInfo * ) ;
static CMPIEnumeration * enumClassNames(Client *,CMPIObjectPath *,CMPIFlags,CMPIStatus *);
                  void * enumClassNamesThrd(thrdInfo * );
static CMPIEnumeration * enumClasses(Client *,CMPIObjectPath *,CMPIFlags,CMPIStatus *) ;
                  void * enumClassesThrd( thrdInfo * ) ;

#endif

typedef const struct _ClientConnectionFT {
    CMPIStatus (*release) (ClientConnection *);
} ClientConnectionFT;

struct _ClientEnc {
   Client          enc;
   ClientData      data;
   CredentialData  certData;
   ClientConnection *connection;
};

struct _ClientConnection {
    ClientConnectionFT *ft;        
};

char *getResponse(ClientConnection *con, CMPIObjectPath *cop);
ClientConnection *initConnection(ClientData *cld);

/* --------------------------------------------------------------------------*/


static CMPIStatus releaseConnection(ClientConnection *con)
{
  CMPIStatus rc = {CMPI_RC_OK,NULL};
  free(con);
  return rc;
}


static ClientConnectionFT conFt={
  releaseConnection
};

/* --------------------------------------------------------------------------*/

ClientConnection *initConnection(ClientData *cld)
{
   ClientConnection *c=(ClientConnection*)calloc(1,sizeof(ClientConnection));
   c->ft=&conFt;
   return c;
}

/*--------------------------------------------------------------------------*/
/* --------------------------------------------------------------------------*/

static Client * cloneClient ( Client * cl, CMPIStatus * st )
{
  CMPIStatus rc;
  CIMCSetStatusWithChars(&rc, CMPI_RC_ERR_NOT_SUPPORTED, "Clone function not supported");
  if (st) *st=rc;
  return NULL;
}  

static CMPIStatus releaseClient(Client * mb)
{
  CMPIStatus rc={CMPI_RC_OK,NULL};
  ClientEnc		* cl = (ClientEnc*)mb;
  
  if (cl->data.hostName) {
    free(cl->data.hostName);
  }
  if (cl->data.user) {
    free(cl->data.user);
  }
  if (cl->data.pwd) {
    free(cl->data.pwd);
  }
  if (cl->data.scheme) {
    free(cl->data.scheme);
  }
  if (cl->data.port) {
    free(cl->data.port);
  }
  if (cl->certData.trustStore) {
    free(cl->certData.trustStore);
  }
  if (cl->certData.certFile) {
    free(cl->certData.certFile);
  }
  if (cl->certData.keyFile) {
    free(cl->certData.keyFile);
  }
 
  if (cl->connection) CMRelease(cl->connection);

  free(cl);
  return rc;
}

void freeResps(BinResponseHdr **resp, int count)
{
   if (resp && count) while (count)
      free(resp[(count--)-1]);
   if (resp) free(resp);
}

extern void setEnumArray(CMPIEnumeration * enumeration,CMPIArray * array);
static CMPIEnumeration *cpyEnumResponses(BinRequestContext * binCtx,
                                 BinResponseHdr ** resp,
                                 int arrLen)
{
   int i, c, j;
   union o {
     CMPIInstance *inst;
     CMPIObjectPath *path;
     CMPIConstClass *cls;
   } object;
   CMPIArray *ar,*art;
   CMPIEnumeration *enm;
   CMPIStatus rc;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "genEnumResponses");
   
   ar = NewCMPIArray(arrLen, binCtx->type, NULL);
   art = NewCMPIArray(0, binCtx->type, NULL);
   
   for (c = 0, i = 0; i < binCtx->rCount; i++) {
      for (j = 0; j < resp[i]->count; c++, j++) {
         if (binCtx->type == CMPI_ref)
            object.path = relocateSerializedObjectPath(resp[i]->object[j].data);
         else if (binCtx->type == CMPI_instance)
            object.inst = relocateSerializedInstance(resp[i]->object[j].data);
         else if (binCtx->type == CMPI_class) {
            object.cls = relocateSerializedConstClass(resp[i]->object[j].data);
         }
    //     object.inst=CMClone(object.inst,NULL);
         rc=CMSetArrayElementAt(ar,c, (CMPIValue*)&object.inst, binCtx->type);
      }
   }

   enm = NewCMPIEnumeration(art, NULL);
   setEnumArray(enm,ar);
   art->ft->release(art);
   
   _SFCB_RETURN(enm);
}

static void ctxErrResponse( BinRequestContext * ctx, CMPIStatus *rc)
{
   MsgXctl *xd = ctx->ctlXdata;
   char msg[256],*m;
   int r=CMPI_RC_ERR_FAILED;

   switch (ctx->rc) {
   case MSG_X_NOT_SUPPORTED:
      m = "Operation not supported yy";
      break;
   case MSG_X_INVALID_CLASS:
      m = "Class not found";
      r = CMPI_RC_ERR_INVALID_CLASS;
      break;
   case MSG_X_INVALID_NAMESPACE:
      m = "Invalid namespace";
      r = CMPI_RC_ERR_INVALID_NAMESPACE;
      break;
   case MSG_X_PROVIDER_NOT_FOUND:
      m = "Provider not found or not loadable";
      r = CMPI_RC_ERR_NOT_FOUND;
      break;
   case MSG_X_FAILED:
      m = xd->data;
      break;
   default:
      sprintf(msg, "Internal error - %d\n", ctx->rc);
      m = msg;
   }
   
   if (rc) CIMCSetStatusWithChars(rc,r,m);
}

static void closeSockets(BinRequestContext *binCtx)
{
   int c;
   for (c=0;c<binCtx->pCount; c++) {
      close(binCtx->pAs[c].socket);
   }
}

#ifndef LARGE_VOL_SUPPORT
static CMPIEnumeration * enumInstanceNames(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumInstanceNames");
   EnumInstanceNamesReq sreq = BINREQ(OPS_EnumerateInstanceNames, 2);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateInstanceNames,0,2};
   CMPIEnumeration *enm = NULL;

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = 0;
   binCtx.type=CMPI_ref;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);
   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);
      _SFCB_TRACE(1, ("--- Back from Provider"));

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      if (resp) freeResps(resp,binCtx.pCount);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   
   _SFCB_RETURN(NULL);
}


static CMPIEnumeration * enumInstances(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   EnumInstancesReq *sreq;
   int pCount=0, irc, l = 0, err = 0,sreqSize=sizeof(EnumInstancesReq);
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateInstances,0,2};
   CMPIEnumeration *enm = NULL;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumInstances");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   sreq->hdr.operation=OPS_EnumerateInstances;
   sreq->hdr.count=pCount+2;

   sreq->objectPath = setObjectPathMsgSegment(cop);
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.bHdr->flags = flags;
   binCtx.type=CMPI_instance;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);
   
   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         free(sreq);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      if (resp) freeResps(resp,binCtx.pCount);
         free(sreq);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   free(sreq);
   
   _SFCB_RETURN(NULL);

}

#endif
/* --------------------------------------------------------------------------*/

static CMPIInstance * getInstance(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   CMPIInstance *inst;
   int irc, i, sreqSize=sizeof(GetInstanceReq)-sizeof(MsgSegment);
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   GetInstanceReq *sreq;
   int pCount=0;
   OperationHdr oHdr={OPS_GetInstance,0,2};
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "getInstance");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   sreq->hdr.operation=OPS_GetInstance;
   sreq->hdr.count=pCount+2;

   sreq->objectPath = setObjectPathMsgSegment(cop);
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   for (i=0; i<pCount; i++)
      sreq->properties[i]=setCharsMsgSegment(properties[i]);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.bHdr->flags = flags;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProvider(&binCtx);
      
      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      resp->rc--;
      if (resp->rc == CMPI_RC_OK) {
         inst = relocateSerializedInstance(resp->object[0].data);
         inst=inst->ft->clone(inst,NULL);
         free(sreq);
         free(resp);
         _SFCB_RETURN(inst);
      }
      free(sreq);
      if (rc) CIMCSetStatusWithChars(rc, resp->rc, (char*)resp->object[0].data);
      free(resp);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   free(sreq);
   closeProviderContext(&binCtx);

   _SFCB_RETURN(NULL);
}


/* --------------------------------------------------------------------------*/

static CMPIObjectPath * createInstance(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIInstance * inst,
	CMPIStatus * rc)
{
   int irc;
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   CreateInstanceReq sreq = BINREQ(OPS_CreateInstance, 3);
   CMPIObjectPath *path;
   OperationHdr oHdr={OPS_CreateInstance,0,3};

   _SFCB_ENTER(TRACE_CIMXMLPROC, "createInst");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));
                  
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.path = setObjectPathMsgSegment(cop);
   sreq.instance = setInstanceMsgSegment(inst);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProvider(&binCtx);
            
      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      resp->rc--;
      if (resp->rc == CMPI_RC_OK) {
         path = relocateSerializedObjectPath(resp->object[0].data);
         path=path->ft->clone(path,NULL);
         free(resp);
         _SFCB_RETURN(path);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp->rc, (char*)resp->object[0].data);
      free(resp);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);

   _SFCB_RETURN(NULL);
}


/* --------------------------------------------------------------------------*/

static CMPIStatus modifyInstance(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIInstance * inst,
	CMPIFlags flags,
	char ** properties)
{
   int pCount=0, irc, i, sreqSize=sizeof(ModifyInstanceReq)-sizeof(MsgSegment);
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   ModifyInstanceReq *sreq;
   OperationHdr oHdr={OPS_ModifyInstance,0,2};
   CMPIStatus rc={0,NULL};
    
   _SFCB_ENTER(TRACE_CIMXMLPROC, "setInstance");
   
   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   
   for (i=0; i<pCount; i++)
      sreq->properties[i]=setCharsMsgSegment(properties[i]);

   sreq->hdr.operation=OPS_ModifyInstance;
   sreq->hdr.count=pCount+3;

   sreq->instance = setInstanceMsgSegment(inst);
   sreq->path = setObjectPathMsgSegment(cop);
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProvider(&binCtx);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      resp->rc--;
      if (resp->rc == CMPI_RC_OK) {
         free(sreq);
         free(resp);
         _SFCB_RETURN(rc);
      }
      free(sreq);
      CIMCSetStatusWithChars(&rc, resp->rc, (char*)resp->object[0].data);
      free(resp);
      _SFCB_RETURN(rc);
   }
   else ctxErrResponse(&binCtx,&rc);
   free(sreq);
   closeProviderContext(&binCtx);

   _SFCB_RETURN(rc);
}

static CMPIStatus deleteInstance(
	Client * mb,
	CMPIObjectPath * cop)
{
   int irc;
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   DeleteInstanceReq sreq = BINREQ(OPS_DeleteInstance, 2);
   OperationHdr oHdr={OPS_DeleteInstance,0,2};
   CMPIStatus rc={0,NULL};
    
   _SFCB_ENTER(TRACE_CIMXMLPROC, "deleteInstance");
   
   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProvider(&binCtx);
      
      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      resp->rc--;
      if (resp->rc == CMPI_RC_OK) {
         free(resp);
         _SFCB_RETURN(rc);
      }
      CIMCSetStatusWithChars(&rc, resp->rc, (char*)resp->object[0].data);
      free(resp);
      _SFCB_RETURN(rc);
   }
   else ctxErrResponse(&binCtx,&rc);
   closeProviderContext(&binCtx);
   
   _SFCB_RETURN(rc);
}


static CMPIEnumeration * execQuery(
	Client * mb,
	CMPIObjectPath * cop,
	const char * query,
	const char * lang,
	CMPIStatus * rc)
{
   ExecQueryReq sreq = BINREQ(OPS_ExecQuery, 4);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   QLStatement *qs=NULL;
   char **fCls;
   OperationHdr oHdr={OPS_ExecQuery,0,4};
	CMPIObjectPath * path;
   CMPIEnumeration *enm;
    
   _SFCB_ENTER(TRACE_CIMXMLPROC, "execQuery");

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   qs=parseQuery(MEM_TRACKED,query,lang,NULL,&irc);
   
   fCls=qs->ft->getFromClassList(qs);
   if (fCls==NULL || *fCls==NULL) {
     mlogf(M_ERROR,M_SHOW,"--- from clause missing\n");
     abort();
   }
   oHdr.className = setCharsMsgSegment(*fCls);

   path = NewCMPIObjectPath((char*)ns->hdl, *fCls, NULL);
   
   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(path);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.query = setCharsMsgSegment(query);
   sreq.queryLang = setCharsMsgSegment(lang);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = 0;
   binCtx.rHdr = NULL;
   binCtx.type=CMPI_instance;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;
   
   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);
   CMRelease(ns);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      freeResps(resp,binCtx.pCount);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
    
   _SFCB_RETURN(NULL);
}	
	
static CMPIEnumeration * associators(
	Client	* mb,
	CMPIObjectPath	* cop,
	const char	* assocClass,
	const char	* resultClass,
	const char	* role,
	const char	* resultRole,
	CMPIFlags	flags,
	char		** properties,
	CMPIStatus	* rc)
{
   AssociatorsReq *sreq; 
   int irc, pCount=0, i, l = 0, err = 0, sreqSize=sizeof(AssociatorsReq)-sizeof(MsgSegment);
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_Associators,0,6};
   CMPIEnumeration *enm = NULL;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "associators");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   memset(&binCtx,0,sizeof(BinRequestContext));

   if (pCount) sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   sreq->hdr.operation=OPS_Associators;
   sreq->hdr.count=pCount+6;
   
   sreq->objectPath = setObjectPathMsgSegment(cop);
   sreq->resultClass = setCharsMsgSegment(resultClass);
   sreq->role = setCharsMsgSegment(role);
   sreq->assocClass = setCharsMsgSegment(assocClass);
   sreq->resultRole = setCharsMsgSegment(resultRole);
   sreq->hdr.flags = flags;
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   for (i=0; i<pCount; i++)
      sreq->properties[i]=setCharsMsgSegment(properties[i]);

   oHdr.className = sreq->assocClass;

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.bHdr->flags = flags;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.type=CMPI_instance;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;
   
   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
	      free(sreq);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      if (resp) freeResps(resp,binCtx.pCount);
	   free(sreq);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   free(sreq);
   closeProviderContext(&binCtx);
    
   _SFCB_RETURN(NULL);
}


static CMPIEnumeration * references(
	Client * mb,
	CMPIObjectPath * cop,
	const char * resultClass,
	const char * role ,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   AssociatorsReq *sreq; 
   int irc, pCount=0, i,l = 0, err = 0, sreqSize=sizeof(AssociatorsReq)-sizeof(MsgSegment);
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_References,0,4};
   CMPIEnumeration *enm = NULL;

   _SFCB_ENTER(TRACE_CIMXMLPROC, "references");

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   memset(&binCtx,0,sizeof(BinRequestContext));
   
   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   sreq->hdr.operation=OPS_References;
   sreq->hdr.count=pCount+4;

   
   sreq->objectPath = setObjectPathMsgSegment(cop);
   sreq->resultClass = setCharsMsgSegment(resultClass);
   sreq->role = setCharsMsgSegment(role);
   sreq->hdr.flags = flags;
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   for (i=0; i<pCount; i++)
      sreq->properties[i]=setCharsMsgSegment(properties[i]);

   oHdr.className = sreq->resultClass;

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.bHdr->flags = flags;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.type=CMPI_instance;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;
   
   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
	      free(sreq);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      if (resp) freeResps(resp,binCtx.pCount);
	   free(sreq);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   free(sreq);
   closeProviderContext(&binCtx);
    
   _SFCB_RETURN(NULL);
}


static CMPIEnumeration * associatorNames(
	Client	* mb,
	CMPIObjectPath	* cop,
	const char	* assocClass,
	const char	* resultClass,
	const char	* role,
	const char	* resultRole,
	CMPIStatus	* rc)
{
   AssociatorNamesReq sreq = BINREQ(OPS_AssociatorNames, 6);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_AssociatorNames,0,6};
   CMPIEnumeration *enm = NULL;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "associatorNames");

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);

   memset(&binCtx,0,sizeof(BinRequestContext));
   
   sreq.objectPath = setObjectPathMsgSegment(cop);

   sreq.resultClass = setCharsMsgSegment(resultClass);
   sreq.role = setCharsMsgSegment(role);
   sreq.assocClass = setCharsMsgSegment(assocClass);
   sreq.resultRole = setCharsMsgSegment(resultRole);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className = sreq.assocClass;

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = 0;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.type=CMPI_ref;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      freeResps(resp,binCtx.pCount);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
    
   _SFCB_RETURN(NULL);
}
	
	
	
static CMPIEnumeration * referenceNames(
	Client * mb,
	CMPIObjectPath * cop,
	const char * resultClass,
	const char * role,
	CMPIStatus * rc)
{
   ReferenceNamesReq sreq = BINREQ(OPS_ReferenceNames, 4);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_ReferenceNames,0,4};
   CMPIEnumeration *enm = NULL;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "referenceNames");

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);

   memset(&binCtx,0,sizeof(BinRequestContext));
   
   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.resultClass = setCharsMsgSegment(resultClass);
   sreq.role = setCharsMsgSegment(role);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   oHdr.className = sreq.resultClass;
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = 0;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.type=CMPI_ref;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      freeResps(resp,binCtx.pCount);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
    
   _SFCB_RETURN(NULL);
}

static CMPIData invokeMethod(
	Client * mb,
	CMPIObjectPath * cop,
	const char * method,
	CMPIArgs * in,
	CMPIArgs * out,
	CMPIStatus * rc)
{
   InvokeMethodReq sreq = BINREQ(OPS_InvokeMethod, 5);
   int irc, i, outc;
   BinResponseHdr *resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_InvokeMethod,0,5};
   CMPIArgs   *argsout;
   CMPIData retval={0,CMPI_notFound,{0l}};
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "referenceNames");

   
   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.in = setArgsMsgSegment(in);
   sreq.out = setArgsMsgSegment(NULL);
   sreq.method = setCharsMsgSegment(method);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   CMRelease(ns);
   CMRelease(cn);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Provider"));
      resp = invokeProvider(&binCtx);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      resp->rc--;
      if (resp->rc == CMPI_RC_OK) {
         argsout = relocateSerializedArgs(resp->object[0].data);
         outc = CMGetArgCount(argsout, NULL);
         for (i=0; i < outc; i++) {
            CMPIString *name;
            CMPIData data = CMGetArgAt(argsout,i, &name,NULL);
            CMAddArg(out,(char*)name->hdl,&data.value,data.type);
         }
         if (resp->rvValue) {
            if (resp->rv.type==CMPI_chars) {
               resp->rv.value.chars=(long)resp->rvEnc.data+(char*)resp;
            }
            else if (resp->rv.type==CMPI_dateTime) {
               resp->rv.value.dateTime=
                  NewCMPIDateTimeFromChars((long)resp->rvEnc.data
                                          +(char*)resp,NULL);
            }
         }
         retval = resp->rv;
         free(resp);
         _SFCB_RETURN(retval);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp->rc, (char*)resp->object[0].data);
      free(resp);
      _SFCB_RETURN(retval);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   
   _SFCB_RETURN(retval);
}

static CMPIStatus setProperty(
	Client * mb,
	CMPIObjectPath * cop,
	const char * name,
	CMPIValue * value,
	CMPIType type)
{
   CMPIStatus rc;
   CMSetStatus(&rc, CMPI_RC_ERR_NOT_SUPPORTED);

   
   return rc;
}

static CMPIData getProperty(
	Client * mb,
	CMPIObjectPath * cop,
	const char * name,
	CMPIStatus * rc)
{
   CMPIData retval={0,CMPI_notFound,{0l}};
   if (rc) CMSetStatus(rc, CMPI_RC_ERR_NOT_SUPPORTED);
   return retval;
}


/* --------------------------------------------------------------------------*/
static CMPIConstClass * getClass(
   Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
   CMPIConstClass *cls;   
   int irc,i,sreqSize=sizeof(GetClassReq)-sizeof(MsgSegment);
   BinRequestContext binCtx;
   BinResponseHdr *resp;
   GetClassReq *sreq;
   int pCount=0;
   OperationHdr oHdr={OPS_GetClass,0,2};
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "getClass");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   sreq->hdr.operation=OPS_GetClass;
   sreq->hdr.count=pCount+2;

   sreq->objectPath = setObjectPathMsgSegment(cop);
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   for (i=0; i<pCount; i++)
      sreq->properties[i]=setCharsMsgSegment(properties[i]);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.bHdr->flags = flags;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);

   _SFCB_TRACE(1, ("--- Provider context gotten"));
   if (irc == MSG_X_PROVIDER) {
      resp = invokeProvider(&binCtx);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      resp->rc--;
      if (resp->rc == CMPI_RC_OK) { 
         cls = relocateSerializedConstClass(resp->object[0].data);
         cls = cls->ft->clone(cls,NULL);
         free(resp);
         free(sreq);
         _SFCB_RETURN(cls);
      }
      free(sreq);
      if (rc) CIMCSetStatusWithChars(rc, resp->rc, (char*)resp->object[0].data);
      free(resp);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   free(sreq);
   closeProviderContext(&binCtx);

   _SFCB_RETURN(NULL);
}

#ifndef LARGE_VOL_SUPPORT
static CMPIEnumeration* enumClassNames(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	CMPIStatus * rc)
{
   EnumClassNamesReq sreq = BINREQ(OPS_EnumerateClassNames, 2);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateClassNames,0,2};
   CMPIEnumeration *enm = NULL;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumClassNames");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));
   
   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.hdr.flags = flags;

   binCtx.oHdr = (OperationHdr*) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = flags;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.type=CMPI_ref;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

  _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx,(OperationHdr*)&oHdr);

   CMRelease(ns);
   CMRelease(cn);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);
      _SFCB_TRACE(1, ("--- Back from Provider"));

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      freeResps(resp,binCtx.pCount);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   
   _SFCB_RETURN(NULL);
}

static CMPIEnumeration * enumClasses(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	CMPIStatus * rc)
{
   EnumClassesReq sreq = BINREQ(OPS_EnumerateClasses, 2);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateClasses,0,2};
   CMPIEnumeration *enm = NULL;
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumClasses");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.hdr.flags = flags;

   binCtx.oHdr = (OperationHdr*)&oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = flags;
   binCtx.type=CMPI_class;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr*)&oHdr);

   CMRelease(ns);
   CMRelease(cn);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      if (err == 0) {
         enm=cpyEnumResponses(&binCtx,resp,l);
         freeResps(resp,binCtx.pCount);
         _SFCB_RETURN(enm);
      }
      if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      freeResps(resp,binCtx.pCount);
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);

   _SFCB_RETURN(NULL);

}

#endif



static ClientFT clientFt = {
   1, //NATIVE_FT_VERSION,
   releaseClient,
   cloneClient,
   getClass,
   enumClassNames,
   enumClasses,
   getInstance,
   createInstance,
   modifyInstance,
   deleteInstance,
   execQuery,
   enumInstanceNames,
   enumInstances,
   associators,
   associatorNames,
   references,
   referenceNames,
   invokeMethod,
   setProperty,
   getProperty
};

/* --------------------------------------------------------------------------*/

extern int getControlChars(char *, char **);
extern int setupControl(char *fn);
extern int errno;

int localConnect(ClientEnv* ce, CMPIStatus *st) 
{
   static struct sockaddr_un serverAddr;
   int sock,rc,sfcbSocket;
   void *idData;
   unsigned long int l;
   char *user;
   static char *socketName=NULL;

   
   struct _msg {
      unsigned int size;
      char oper;
      pid_t pid;
      char id[64];
   } msg;
   
   if ((sock=socket(PF_UNIX, SOCK_STREAM, 0))<0) {
      if (st) {
         st->rc=CMPI_RC_ERR_FAILED;
         st->msg=ce->ft->newString(ce,strerror(errno),NULL);
      }
      return -1;
   }
   
   if (socketName==NULL) {
      setupControl(NULL);
      rc=getControlChars("localSocketPath", &socketName);    
      sunsetControl();
      if (rc) {
         fprintf(stderr,"--- Failed to open sfcb local socket (%d)\n",rc);
         return -2;
      }
   }   

   serverAddr.sun_family=AF_UNIX;
   strcpy(serverAddr.sun_path,socketName);
   
   if (connect(sock,(struct sockaddr*)&serverAddr,
      sizeof(serverAddr.sun_family)+strlen(serverAddr.sun_path))<0) {
      if (st) {
         st->rc=CMPI_RC_ERR_FAILED;
         st->msg=ce->ft->newString(ce,strerror(errno),NULL);
      }
      return -1;
   }
   
   msg.size=sizeof(msg)-sizeof(msg.size);
   msg.oper=1;
   msg.pid=getpid();
   user=getenv("USER");
   strncpy(msg.id, (user) ? user : "", sizeof(msg.id)-1);
   msg.id[sizeof(msg.id)-1]=0;
   
   l=write(sock, &msg, sizeof(msg)); 
   
   rc = spRecvCtlResult(&sock, &sfcbSocket,&idData, &l);
  
   sfcbSockets.send=sfcbSocket;
   localMode=0;
   close(sock);
   
   return (rc < 0) ? rc : sfcbSocket;
}

static void* release(ClientEnv* ce)
{
   void *lib=ce->hdl;
   close(sfcbSockets.send);
   free(ce);
   uninitGarbageCollector();
   return lib;
}

static Client *CMPIConnect2(ClientEnv* ce, const char *hn, const char *scheme, const char *port,
			 const char *user, const char *pwd, 
			 int verifyMode, const char * trustStore,
			 const char * certFile, const char * keyFile,
			 CMPIStatus *rc);
                         
static Client *CMPIConnect(ClientEnv* ce, const char *hn, const char *scheme, const char *port,
                        const char *user, const char *pwd, CMPIStatus *rc)
{
  return CMPIConnect2(ce, hn, scheme, port, user, pwd, Client_VERIFY_PEER, NULL,
		      NULL, NULL, rc);
}

static Client *CMPIConnect2(ClientEnv* ce, const char *hn, const char *scheme, const char *port,
			 const char *user, const char *pwd, 
			 int verifyMode, const char * trustStore,
			 const char * certFile, const char * keyFile,
			 CMPIStatus *rc)
{  
   ClientEnc *cc = (ClientEnc*)calloc(1, sizeof(ClientEnc));

   if (rc) rc->rc=0;
   
   cc->enc.hdl		= &cc->data;
   cc->enc.ft		= &clientFt;

   cc->data.hostName	= hn ? strdup(hn) : strdup("localhost");
   cc->data.user	= user ? strdup(user) : NULL;
   cc->data.pwd		= pwd ? strdup(pwd) : NULL;
   cc->data.scheme	= scheme ? strdup(scheme) : strdup("http");

   if (port != NULL)
      cc->data.port = strdup(port); 
   else
      cc->data.port = strcmp(cc->data.scheme, "https") == 0 ? 
	strdup("5989") : strdup("5988");

   cc->certData.verifyMode = verifyMode;
   cc->certData.trustStore = trustStore ? strdup(trustStore) : NULL;
   cc->certData.certFile = certFile ? strdup(certFile) : NULL;
   cc->certData.keyFile = keyFile ? strdup(keyFile) : NULL;

   if (localConnect(ce,rc)<0) return NULL;

   return (Client*)cc;
}

static CMPIInstance* newInstance(ClientEnv* ce, const CMPIObjectPath* op, CMPIStatus* rc)
{
   return NewCMPIInstance((CMPIObjectPath*)op,rc);
}

static CMPIString* newString(ClientEnv* ce, const char *ptr, CMPIStatus * rc)
{
   return NewCMPIString(ptr, rc);
}

static CMPIObjectPath* newObjectPath(ClientEnv* ce, const char *ns, const char *cn, CMPIStatus* rc)
{
   return NewCMPIObjectPath(ns,cn,rc);
}

static CMPIArgs* newArgs(ClientEnv* ce, CMPIStatus* rc)
{
   return NewCMPIArgs(rc);
}

static CMPIArray* newArray(ClientEnv* ce, CMPICount max, CMPIType type, CMPIStatus* rc)
{
   return NewCMPIArray(max, type, rc);
}

static CMPIDateTime *newDateTime(ClientEnv* ce, CMPIStatus *rc) 
{
   return NewCMPIDateTime(rc); 
}

static CMPIDateTime *newDateTimeFromBinary(ClientEnv* ce, CMPIUint64 binTime, CMPIBoolean interval, 
      CMPIStatus *rc) 
{
   return NewCMPIDateTimeFromBinary(binTime,interval,rc); 
}

static CMPIDateTime *newDateTimeFromChars(ClientEnv* ce, const char *utcTime, CMPIStatus *rc) 
{
   return NewCMPIDateTimeFromChars(utcTime,rc); 
}

ClientEnv* _Create_SfcbLocal_Env(char *id)
{
 
   static ClientEnvFT localFT = {
      "SfcbLocal",
      release,
      CMPIConnect,
      CMPIConnect2,     
      newInstance,      
      newObjectPath,
      newArgs,
      newString,
      newArray,
      newDateTime,
      newDateTimeFromBinary,
      newDateTimeFromChars,
    };
   
   //    localClientMode=1;
    setInstanceLocalMode(1);
    
    ClientEnv *env = (ClientEnv*)malloc(sizeof(ClientEnv));
    env->hdl=NULL;
    env->ft=&localFT;
    
    return env;
 }
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
#ifdef LARGE_VOL_SUPPORT



static CMPIEnumeration * enumInstances(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	char ** properties,
	CMPIStatus * rc)
{
  pthread_t       enumThrdId  = 0 ;
  int             pthrd_error   = 0 ;
	thrdInfo passedInfo  ;
	CMPIEnumeration *enm = NULL;
	int enumState = 0 ;
	int timeOut = 0 ;
	 _SFCB_ENTER(TRACE_CIMXMLPROC, "enumInstances");
	
  /*
   * allocate an enum 
   */
	
  enm = NewCMPIEnumeration(NULL, NULL);
	  
  setEnumState(enm, ENUM_INIT) ;
  
  /*
   * fill in info to send to the thread
   */
  passedInfo.enm = enm ;
  passedInfo.mb = mb ;
  passedInfo.cop = cop ;
  passedInfo.flags = flags ;
  passedInfo.properties = properties ;
  passedInfo.rc = rc ;

  pthrd_error = pthread_create(&enumThrdId,
                                  NULL,
                                  (void*)&enumInstancesThrd,
                                  (void*)&passedInfo);


  /*
   * for now delay here till we see state changed
   */
  while ((enumState = getEnumState(enm)) == ENUM_INIT){
     // usleep(200000) ;	
     usleep(5000) ;
     timeOut++ ;
     if(timeOut > 1000){
  	 	  pthread_cancel(enumThrdId);
  	 	  CIMCSetStatusWithChars(rc,CMPI_RC_ERR_FAILED," Failed timeout - EnumState is still ENUM_INIT "); 	     	
  	    break;     	
     }
  }
    
	_SFCB_RETURN(enm);
}

void * enumInstancesThrd( thrdInfo * passedInInfo)
{ 
	 Client * mb = passedInInfo->mb ;
	 CMPIObjectPath * cop = passedInInfo->cop ;
	 CMPIFlags flags = passedInInfo->flags ;
	 char ** properties = passedInInfo->properties ;
	 CMPIStatus * rc = passedInInfo->rc ;
   CMPIEnumeration *enm = passedInInfo->enm;

   EnumInstancesReq *sreq;
   int pCount=0, irc, l = 0, err = 0,sreqSize=sizeof(EnumInstancesReq);
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateInstances,0,2};

   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumInstanceThrd");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   if (properties) {
      char **p;
      for (p=properties; *p; p++) pCount++;
   }
   
   sreqSize+=pCount*sizeof(MsgSegment);
   sreq=calloc(1,sreqSize);
   sreq->hdr.operation=OPS_EnumerateInstances;
   sreq->hdr.count=pCount+2;

   sreq->objectPath = setObjectPathMsgSegment(cop);
   sreq->principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq->hdr;
   binCtx.bHdr->flags = flags;
   binCtx.type=CMPI_instance;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sreqSize;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0; 
   binCtx.pAs=NULL;
   binCtx.largeVolLocal= 1;
   binCtx.enm=enm;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);
   
   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));

      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      setEnumState(enm , ENUM_COMPLETE) ;
      
      if(err != 0) {   
         if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                  (char*)resp[err-1]->object[0].data);
      }
      
      if (resp) freeResps(resp,binCtx.pCount);
      
      free(sreq);
      
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   free(sreq);
   
   _SFCB_RETURN(NULL);

}
/* ************************************************************************************* */

static CMPIEnumeration * enumInstanceNames(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIStatus * rc)
{
  pthread_t       enumThrdId  = 0 ;
  int             pthrd_error   = 0 ;
	thrdInfo passedInfo  ;
	CMPIEnumeration *enm = NULL;
	int enumState = 0 ;
	int timeOut = 0 ;
	
	 _SFCB_ENTER(TRACE_CIMXMLPROC, "enumInstanceNames");
	
  /*
   * allocate an enum 
   */
	
  enm = NewCMPIEnumeration(NULL, NULL);
	  
  setEnumState(enm, ENUM_INIT) ;
  
  /*
   * fill in info to send to the thread
   */
  passedInfo.enm = enm ;
  passedInfo.mb = mb ;
  passedInfo.cop = cop ;
  passedInfo.flags = 0 ;
  passedInfo.properties = NULL ;
  passedInfo.rc = rc ;
  
  pthrd_error = pthread_create(&enumThrdId,
                                  NULL,
                                  (void*)&enumInstanceNamesThrd,
                                  (void*)&passedInfo);
  
  /*
   * for now delay here till we see state changed
   */
  while ((enumState = getEnumState(enm)) == ENUM_INIT){
     //usleep(200000) ;
     usleep(5000) ;
     timeOut++ ;
     if(timeOut > 1000){
  	 	  pthread_cancel(enumThrdId);
  	 	  CIMCSetStatusWithChars(rc,CMPI_RC_ERR_FAILED," Failed timeout - EnumState is still ENUM_INIT "); 	     	
  	    break;     	
     }	
  }

	_SFCB_RETURN(enm);
}

void * enumInstanceNamesThrd( thrdInfo * passedInInfo)
{
   Client * mb = passedInInfo->mb ;
	 CMPIObjectPath * cop = passedInInfo->cop ;
	 CMPIStatus * rc = passedInInfo->rc ;
   CMPIEnumeration *enm = passedInInfo->enm;
   
   EnumInstanceNamesReq sreq = BINREQ(OPS_EnumerateInstanceNames, 2);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateInstanceNames,0,2};

   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumInstanceNamesThrd");

   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);

   binCtx.oHdr = (OperationHdr *) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = 0;
   binCtx.type=CMPI_ref;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;
   binCtx.largeVolLocal= 1; 
   binCtx.enm=enm;


   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr *) &oHdr);
   CMRelease(ns);
   CMRelease(cn);
   
   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);
      _SFCB_TRACE(1, ("--- Back from Provider"));

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      setEnumState(enm , ENUM_COMPLETE) ;
      
      if (err != 0) {
         if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                    (char*)resp[err-1]->object[0].data);
      }
      
      if (resp) freeResps(resp,binCtx.pCount);
      	
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   
   _SFCB_RETURN(NULL);
}

////////////////////////////////////////////

static CMPIEnumeration * enumClassNames(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	CMPIStatus * rc)
{
	pthread_t       enumThrdId  = 0 ;
  int             pthrd_error   = 0 ;
	thrdInfo passedInfo  ;
	CMPIEnumeration *enm = NULL;
	int enumState = 0 ;
	int timeOut = 0 ;
	
	 _SFCB_ENTER(TRACE_CIMXMLPROC, "enumClassNames");
	
  /*
   * allocate an enum 
   */
	
  enm = NewCMPIEnumeration(NULL, NULL);
	  
  setEnumState(enm, ENUM_INIT) ;
  
  /*
   * fill in info to send to the thread
   */
  passedInfo.enm = enm ;
  passedInfo.mb = mb ;
  passedInfo.cop = cop ;
  passedInfo.flags = flags;
  passedInfo.properties = NULL ;
  passedInfo.rc = rc ;
  
  pthrd_error = pthread_create(&enumThrdId,
                                  NULL,
                                  (void*)&enumClassNamesThrd,
                                  (void*)&passedInfo);
  
  /*
   * for now delay here till we see state changed
   */
  while ((enumState = getEnumState(enm)) == ENUM_INIT){
     usleep(5000) ;	
  	 timeOut++;
  	 if(timeOut > 1000){
  	 	pthread_cancel(enumThrdId);
  	 	CIMCSetStatusWithChars(rc,CMPI_RC_ERR_FAILED," Failed timeout - EnumState is still ENUM_INIT "); 	     	
  	  break;
  	 }
  }
  
	_SFCB_RETURN(enm);
	
}


void * enumClassNamesThrd(thrdInfo * passedInInfo)
{
	 Client * mb = passedInInfo->mb ;
   CMPIObjectPath * cop = passedInInfo->cop ;
   CMPIFlags flags = passedInInfo->flags ;
   // char ** properties = NULL ;
   CMPIStatus * rc = passedInInfo->rc ;
   CMPIEnumeration *enm = passedInInfo->enm;
   
   EnumClassNamesReq sreq = BINREQ(OPS_EnumerateClassNames, 2);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateClassNames,0,2};
   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumClassNamesThrd");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));
   
   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.hdr.flags = flags;

   binCtx.oHdr = (OperationHdr*) &oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = flags;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.type=CMPI_ref;
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;
   binCtx.largeVolLocal= 1; 
   binCtx.enm=enm;


  _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx,(OperationHdr*)&oHdr);

   CMRelease(ns);
   CMRelease(cn);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);
      _SFCB_TRACE(1, ("--- Back from Provider"));

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);

      setEnumState(enm , ENUM_COMPLETE) ;
     
      if (err != 0) {
         if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                   (char*)resp[err-1]->object[0].data);
      }
      
      if (resp) freeResps(resp,binCtx.pCount);
      	
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);
   
   _SFCB_RETURN(NULL);
}

static CMPIEnumeration * enumClasses(
	Client * mb,
	CMPIObjectPath * cop,
	CMPIFlags flags,
	CMPIStatus * rc)
{
	pthread_t       enumThrdId  = 0 ;
  int             pthrd_error   = 0 ;
	thrdInfo passedInfo  ;
	CMPIEnumeration *enm = NULL;
	int enumState = 0 ;
	int timeOut = 0 ;
	
	 _SFCB_ENTER(TRACE_CIMXMLPROC, "enumClasses");
	
  /*
   * allocate an enum 
   */
	
  enm = NewCMPIEnumeration(NULL, NULL);
	  
  setEnumState(enm, ENUM_INIT) ;
  
  /*
   * fill in info to send to the thread
   */
  passedInfo.enm = enm ;
  passedInfo.mb = mb ;
  passedInfo.cop = cop ;
  passedInfo.flags = flags;
  passedInfo.properties = NULL ;
  passedInfo.rc = rc ;
  
  pthrd_error = pthread_create(&enumThrdId,
                                  NULL,
                                  (void*)&enumClassesThrd,
                                  (void*)&passedInfo);
  
  /*
   * for now delay here till we see state changed
   */
  while ((enumState = getEnumState(enm)) == ENUM_INIT){
     usleep(5000) ;
     timeOut++ ;
     if(timeOut > 1000){
  	 	  pthread_cancel(enumThrdId);
  	 	  CIMCSetStatusWithChars(rc,CMPI_RC_ERR_FAILED," Failed timeout - EnumState is still ENUM_INIT "); 	     	
  	    break;     	
     }
  }
  
	_SFCB_RETURN(enm);
	
}

void * enumClassesThrd( thrdInfo * passedInInfo)
{
   Client * mb = passedInInfo->mb ;
   CMPIObjectPath * cop = passedInInfo->cop ;
   CMPIFlags flags = passedInInfo->flags ;
   CMPIStatus * rc = passedInInfo->rc ;
   CMPIEnumeration *enm = passedInInfo->enm;
   
   EnumClassesReq sreq = BINREQ(OPS_EnumerateClasses, 2);
   int irc, l = 0, err = 0;
   BinResponseHdr **resp;
   BinRequestContext binCtx;
   OperationHdr oHdr={OPS_EnumerateClasses,0,2};

   
   _SFCB_ENTER(TRACE_CIMXMLPROC, "enumClassesThrd");
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *ns=cop->ft->getNameSpace(cop,NULL);
   CMPIString *cn=cop->ft->getClassName(cop,NULL);
   
   oHdr.nameSpace=setCharsMsgSegment((char*)ns->hdl);
   oHdr.className=setCharsMsgSegment((char*)cn->hdl);

   memset(&binCtx,0,sizeof(BinRequestContext));

   sreq.objectPath = setObjectPathMsgSegment(cop);
   sreq.principal = setCharsMsgSegment(((ClientEnc*)mb)->data.user);
   sreq.hdr.flags = flags;

   binCtx.oHdr = (OperationHdr*)&oHdr;
   binCtx.bHdr = &sreq.hdr;
   binCtx.bHdr->flags = flags;
   binCtx.type=CMPI_class;
   binCtx.rHdr = NULL;
   binCtx.bHdrSize = sizeof(sreq);
   binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   binCtx.pAs=NULL;
   binCtx.largeVolLocal= 1; 
   binCtx.enm=enm;

   _SFCB_TRACE(1, ("--- Getting Provider context"));
   irc = getProviderContext(&binCtx, (OperationHdr*)&oHdr);

   CMRelease(ns);
   CMRelease(cn);

   if (irc == MSG_X_PROVIDER) {
      _SFCB_TRACE(1, ("--- Calling Providers"));
      resp = invokeProviders(&binCtx, &err, &l);

      closeSockets(&binCtx);
      closeProviderContext(&binCtx);
      
      setEnumState(enm , ENUM_COMPLETE) ;
       
      if (err != 0) {
         if (rc) CIMCSetStatusWithChars(rc, resp[err-1]->rc, 
                    (char*)resp[err-1]->object[0].data);
      }
      
      if (resp) freeResps(resp,binCtx.pCount);
      	
      _SFCB_RETURN(NULL);
   }
   else ctxErrResponse(&binCtx,rc);
   closeProviderContext(&binCtx);

   _SFCB_RETURN(NULL);

}

#endif
