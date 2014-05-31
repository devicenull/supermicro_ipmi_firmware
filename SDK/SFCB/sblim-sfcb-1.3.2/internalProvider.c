
/*
 * internalProvider.c
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
 * Based on concepts developed by Viktor Mihajlovski <mihajlov@de.ibm.com>
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
#include "providerRegister.h"
#include "fileRepository.h"
#include "utilft.h"
#include "trace.h"
#include "constClass.h"
#include "internalProvider.h"
#include "native.h"
#include "objectpath.h"

#include "cimslp.h"

#define LOCALCLASSNAME "InternalProvider"

static char * interopNs = "root/interop";
static char * pg_interopNs = "root/pg_interop";

extern ProviderInfo *interOpProvInfoPtr;
extern ProviderInfo *forceNoProvInfoPtr;

extern CMPIInstance *relocateSerializedInstance(void *area);
extern char *sfcb_value2Chars(CMPIType type, CMPIValue * value);
extern CMPIObjectPath *getObjectPath(char *path, char **msg);
extern CMPIBroker *Broker;
extern UtilStringBuffer *newStringBuffer(int s);
extern void setStatus(CMPIStatus *st, CMPIrc rc, const char *msg);

static const CMPIBroker *_broker;

static const char * repositoryNs (const char * nss)
{
  if (strcasecmp(nss,pg_interopNs)==0) {
    return interopNs;
  } else {
    return nss;
  }   
}

/*
static int cpy2lower(char *in, char *out)
{
   int i = 0;
   while ((out[i] = tolower(in[i++])) != 0);
   return i - 1;
}
*/


static CMPIInstance *instifyBlob(void * blob) {
	CMPIInstance *inst;
    int id;
	
	if (blob==NULL) {
		return NULL;
	}   
	else {
		inst=relocateSerializedInstance(blob);
        memAdd(blob, &id);
		return inst;
	}	
}

static CMPIInstance* ipGetBlob(const char *ns, const char *cls, char *id, int *len) {
	void *blob=getBlob(ns, cls, id, len);
   	return instifyBlob(blob);
}

static CMPIInstance* ipGetFirst(BlobIndex *bi, int *len, char** keyb, size_t *keybl) {
	void *blob=getFirst(bi, len, keyb, keybl);
	return instifyBlob(blob);
}

static CMPIInstance* ipGetNext(BlobIndex *bi, int *len, char** keyb, size_t *keybl) {
	void *blob=getNext(bi, len, keyb, keybl);
	return instifyBlob(blob);	
}

const char **getKeyList(const CMPIObjectPath *cop)
{
	CMPIString * s;
	const char ** list;
	int i = cop->ft->getKeyCount(cop, NULL);
	list = malloc((i+1) * sizeof(char*));
	list[i] = NULL;
	while(i) {
		i--;
		cop->ft->getKeyAt(cop, i, &s, NULL);
		list[i] = s->ft->getCharPtr(s, NULL);
	}
	return list;
}


static char **nsTab=NULL;
static int nsTabLen=0;

static int testNameSpace(const char *ns, CMPIStatus *st)
{
    char **nsp=nsTab;
    
    if (interOpProvInfoPtr==forceNoProvInfoPtr) {
       if (strcasecmp(ns,interopNs)==0) {
	 st->msg=sfcb_native_new_CMPIString("Interop namespace disabled",NULL,0);
          st->rc=CMPI_RC_ERR_FAILED;
          return 0;
       }   
    }
    
    while (nsTabLen && *nsp) {
       if (strcasecmp(*nsp,ns)==0) return 1;
       nsp++;
    }
    if (existingNameSpace(ns)) {
      nsTab=nsp=realloc(nsTab,sizeof(nsp)*(nsTabLen+2));
      nsp[nsTabLen++]=strdup(ns);
      nsp[nsTabLen]=NULL;
      return 1;
    }
    
    st->rc=CMPI_RC_ERR_INVALID_NAMESPACE;
    return 0;
}

static BlobIndex *_getIndex(const char *ns, const char *cn)
 {
   BlobIndex *bi;
   if (getIndex(ns,cn,strlen(ns)+strlen(cn)+64,0,&bi))
      return bi;
   else return NULL;
 }


extern int isChild(const char *ns, const char *parent, const char* child);

static int isa(const char *sns, const char *child, const char *parent)
{
   int rv;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "isa");
   
   if (strcasecmp(child,parent)==0) return 1;
   rv=isChild(sns,parent,child);
   _SFCB_RETURN(rv);
}


/* ------------------------------------------------------------------ *
 * Instance MI Cleanup
 * ------------------------------------------------------------------ */

CMPIStatus InternalProviderCleanup(CMPIInstanceMI * mi, 
				   const CMPIContext * ctx,
				   CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderCleanup");
   
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------ *
 * Instance MI Functions
 * ------------------------------------------------------------------ */ 
 

CMPIStatus InternalProviderEnumInstanceNames(CMPIInstanceMI * mi,
                                             const CMPIContext * ctx,
                                             const CMPIResult * rslt,
                                             const CMPIObjectPath * ref)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIStatus sti = { CMPI_RC_OK, NULL };
   BlobIndex *bi;
   CMPIString *cn = CMGetClassName(ref, NULL);
   CMPIString *ns = CMGetNameSpace(ref, NULL);
   CMPIObjectPath *cop;
   const char *nss=ns->ft->getCharPtr(ns,NULL);
   const char *cns=cn->ft->getCharPtr(cn,NULL);
   const char *bnss=repositoryNs(nss);
   size_t ekl;
   int i,ac=0;
   char copKey[8192]="";
   char *kp;
   char *msg;
   CMPIArgs *in,*out;
   CMPIObjectPath *op;
   CMPIArray *ar;
   CMPIData rv;

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderEnumInstanceNames");
   _SFCB_TRACE(1,("%s %s",nss,cns));
   
   in=CMNewArgs(Broker,NULL);
   out=CMNewArgs(Broker,NULL);
   CMAddArg(in,"class",cns,CMPI_chars);
   op=CMNewObjectPath(Broker,bnss,"$ClassProvider$",&sti);
   rv=CBInvokeMethod(Broker,ctx,op,"getallchildren",in,out,&sti);     
   ar=CMGetArg(out,"children",NULL).value.array;
   if (ar) ac=CMGetArrayCount(ar,NULL);
   
   for (i=0; cns; i++) {
      if ((bi=_getIndex(bnss,cns))!=NULL) {
	if (ipGetFirst(bi,NULL,&kp,&ekl)) {
	  while(1) {
            strcpy(copKey,nss);
            strcat(copKey,":");
            strcat(copKey,cns);
            strcat(copKey,".");
            strncat(copKey,kp,ekl);
	    
            cop=getObjectPath(copKey,&msg);
            if (cop) CMReturnObjectPath(rslt, cop);
            else {
	      CMPIStatus st = { CMPI_RC_ERR_FAILED, NULL };
	      return st;
            }
	    if (bi->next < bi->dSize && ipGetNext(bi,NULL,&kp,&ekl)) {
	      continue;
	    }
	    break;
	  }
	}
	freeBlobIndex(&bi,1);
      }
      if (i<ac) cns=(char*)CMGetArrayElementAt(ar,i,NULL).value.string->hdl;
      else cns=NULL;  
   }
   _SFCB_RETURN(st);
}

UtilStringBuffer *instanceToString(CMPIInstance * ci, char **props);

static CMPIStatus enumInstances(CMPIInstanceMI * mi, 
				const CMPIContext * ctx, void *rslt,
				const CMPIObjectPath * ref, 
				const char **properties,
				void(*retFnc)(void*,CMPIInstance*), 
				int ignprov)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIStatus sti = { CMPI_RC_OK, NULL };
   BlobIndex *bi;
   CMPIString *cn = CMGetClassName(ref, NULL);
   CMPIString *ns = CMGetNameSpace(ref, NULL);
   const char *nss=ns->ft->getCharPtr(ns,NULL);
   const char *cns=cn->ft->getCharPtr(cn,NULL);
   const char *bnss=repositoryNs(nss);
   int len,i,ac=0;
   CMPIInstance *ci;
   CMPIArgs *in,*out;
   CMPIObjectPath *op;
   CMPIArray *ar;
   CMPIData rv;
   const char **keyList;

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "enumInstances");
   _SFCB_TRACE(1,("--- %s %s",nss,cns));
  
   in=CMNewArgs(Broker,NULL);
   out=CMNewArgs(Broker,NULL);
   if (ignprov) CMAddArg(in,"classignoreprov",cns,CMPI_chars);
   else CMAddArg(in,"class",cns,CMPI_chars);
   
   op=CMNewObjectPath(Broker,bnss,"$ClassProvider$",&sti);
   _SFCB_TRACE(1,("--- getallchildren"));
   rv=CBInvokeMethod(Broker,ctx,op,"getallchildren",in,out,&sti);     
   _SFCB_TRACE(1,("--- getallchildren rc: %d",sti.rc));
   
   ar=CMGetArg(out,"children",NULL).value.array;
   if (ar) ac=CMGetArrayCount(ar,NULL);
   _SFCB_TRACE(1,("--- getallchildren ar: %p count: %d",ar,ac));
 
   for (i=0; cns; i++) {
       _SFCB_TRACE(1,("--- looking for %s",cns));
      if ((bi=_getIndex(bnss,cns))!=NULL) {
	for (ci=ipGetFirst(bi,&len,NULL,0); ci; ci=ipGetNext(bi,&len,NULL,0)) {
            if(properties) {
               keyList = getKeyList(ci->ft->getObjectPath(ci, NULL));
               ci->ft->setPropertyFilter(ci, properties, keyList);
               if(keyList) {
                  free(keyList);
               }
            }
            _SFCB_TRACE(1,("--- returning instance %p",ci));
            retFnc(rslt,ci);
         }
      } 
      freeBlobIndex(&bi,1);
      if (i<ac) cns=(char*)CMGetArrayElementAt(ar,i,NULL).value.string->hdl;
      else cns=NULL;  
   }
   
   _SFCB_RETURN(st);
}

static void return2result(void *ret, CMPIInstance *ci)
{
   CMPIResult * rslt=(CMPIResult*)ret; 
   CMReturnInstance(rslt, ci);
}

static void return2lst(void *ret, CMPIInstance *ci)
{
   UtilList *ul=(UtilList*)ret; 
   ul->ft->append(ul,ci); 
}

CMPIStatus InternalProviderEnumInstances(CMPIInstanceMI * mi, 
					 const CMPIContext * ctx, 
					 const CMPIResult * rslt,
                                         const CMPIObjectPath * ref, 
					 const char **properties)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderEnumInstances");
   st=enumInstances(mi,ctx,(void*)rslt,ref,properties,return2result,0);
   _SFCB_RETURN(st);
}

UtilList *SafeInternalProviderAddEnumInstances(UtilList *ul, CMPIInstanceMI * mi, 
					       const CMPIContext * ctx, const CMPIObjectPath * ref,
					       const char **properties, CMPIStatus *rc, int ignprov)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "SafeInternalProviderAddEnumInstances");
   st=enumInstances(mi,ctx,(void*)ul,ref,properties,return2lst,ignprov);
   if (rc) *rc=st;
   _SFCB_RETURN(ul);
}

CMPIStatus SafeInternalProviderEnumInstances(CMPIInstanceMI * mi, const CMPIContext * ctx, const CMPIResult * rslt,
                                         const CMPIObjectPath * ref, const char **properties, int ignprov)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "SafeInternalProviderEnumInstances");
   st=enumInstances(mi,ctx,(void*)rslt,ref,properties,return2result,ignprov);
   _SFCB_RETURN(st);
}

CMPIInstance *internalProviderGetInstance(const CMPIObjectPath * cop, CMPIStatus *rc)
{
   int len;
   CMPIString *cn = CMGetClassName(cop, NULL);
   CMPIString *ns = CMGetNameSpace(cop, NULL);
   char *key = normalizeObjectPathChars(cop);
   CMPIInstance *ci=NULL;
   const char *nss=ns->ft->getCharPtr(ns,NULL);
   const char *cns=cn->ft->getCharPtr(cn,NULL);
   const char *bnss=repositoryNs(nss);
   CMPIStatus st = { CMPI_RC_OK, NULL };

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "internalProviderGetInstance");
   _SFCB_TRACE(1,("--- Get instance for %s %s %s",nss,cns,key));
   
   if (testNameSpace(bnss,rc)==0) {
      _SFCB_TRACE(1,("--- Invalid namespace %s",nss));
      _SFCB_RETURN(NULL);
   }

   ci=ipGetBlob(bnss,cns,key,&len);
   
   if (ci==NULL) {
      _SFCB_TRACE(1,("--- Instance not found"));
      st.rc=CMPI_RC_ERR_NOT_FOUND;
   }   

   *rc=st;
   _SFCB_RETURN(ci);
}

CMPIStatus InternalProviderGetInstance(CMPIInstanceMI * mi,
                                       const CMPIContext * ctx,
                                       const CMPIResult * rslt,
                                       const CMPIObjectPath * cop,
                                       const char **properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIInstance *ci;
   const char ** keyList;

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderGetInstance");
   
   ci=internalProviderGetInstance(cop,&st);
   if(st.rc==CMPI_RC_OK && properties) {
      keyList = getKeyList(ci->ft->getObjectPath(ci, NULL));
      ci->ft->setPropertyFilter(ci, properties, keyList);
      if(keyList) {
         free(keyList);
      }
   }
   
   if (st.rc==CMPI_RC_OK) {
      CMReturnInstance(rslt, ci);
   }
   
   _SFCB_RETURN(st);    
}

CMPIStatus InternalProviderCreateInstance(CMPIInstanceMI * mi,
                                          const CMPIContext * ctx,
                                          const CMPIResult * rslt,
                                          const CMPIObjectPath * cop,
                                          const CMPIInstance * ci)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   unsigned long len;
   void *blob;
   CMPIString *cn = CMGetClassName(cop, NULL);
   CMPIString *ns = CMGetNameSpace(cop, NULL);
   char *key = normalizeObjectPathChars(cop);
   const char *nss=ns->ft->getCharPtr(ns,NULL);
   const char *cns=cn->ft->getCharPtr(cn,NULL);
   const char *bnss=repositoryNs(nss);

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderCreateInstance");
   
   if (testNameSpace(bnss,&st)==0) {
      return st;
   }

   if (existingBlob(bnss,cns,key)) {
      CMPIStatus st = { CMPI_RC_ERR_ALREADY_EXISTS, NULL };
      return st;
   }

   len=getInstanceSerializedSize(ci);
   blob=malloc(len+64);
   getSerializedInstance(ci,blob);
   
   if (addBlob(bnss,cns,key,blob,(int)len)) {
      CMPIStatus st = { CMPI_RC_ERR_FAILED, NULL };
      st.msg=sfcb_native_new_CMPIString("Unable to write to repository",NULL,0);
      free(blob);
      return st;
   }
   free(blob);
   
   if (rslt) { 
     CMReturnObjectPath(rslt, cop);
     if (isa(nss, cns, "cim_registeredprofile")) {
        CMPIArray* atArray;
        atArray = CMGetProperty(ci, "AdvertiseTypes", &st).value.array;
#ifdef HAVE_SLP
        if (st.rc == CMPI_RC_OK || 
            atArray != NULL || 
            CMGetArrayElementAt(atArray, 0, &st).value.uint16 == 3) {
	  if (slppid > 1) /* sanity check */
            kill(slppid, SIGHUP);  /* restart SLP to update RegisteredProfiles */
        }
#endif
     }
   }

   _SFCB_RETURN(st);
}

CMPIStatus InternalProviderModifyInstance(CMPIInstanceMI * mi,
					  const CMPIContext * ctx,
					  const CMPIResult * rslt,
					  const CMPIObjectPath * cop,
					  const CMPIInstance * ci, 
					  const char **properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   unsigned long len;
   void *blob;
   CMPIString *cn = CMGetClassName(cop, NULL);
   CMPIString *ns = CMGetNameSpace(cop, NULL);
   char *key = normalizeObjectPathChars(cop);
   const char *nss=ns->ft->getCharPtr(ns,NULL);
   const char *cns=cn->ft->getCharPtr(cn,NULL);
   const char *bnss=repositoryNs(nss);
   const char **keyList;

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderSetInstance");
   
   if (testNameSpace(bnss,&st)==0) {
      return st;
   }

   if (existingBlob(bnss,cns,key)==0) {
      CMPIStatus st = { CMPI_RC_ERR_NOT_FOUND, NULL };
      return st;
   }
   
   if(properties) {
     keyList = getKeyList(ci->ft->getObjectPath(ci, NULL));
     ci->ft->setPropertyFilter((CMPIInstance*)ci, properties, keyList);
     if(keyList) {
        free(keyList);
     }
   }   

   len=getInstanceSerializedSize(ci);
   blob=malloc(len+64);
   getSerializedInstance(ci,blob);
   addBlob(bnss,cns,key,blob,(int)len);
   free(blob);

   if (isa(nss, cns, "cim_registeredprofile")) {
      CMPIArray* atArray;
      atArray = CMGetProperty(ci, "AdvertiseTypes", &st).value.array;
      if (st.rc == CMPI_RC_OK || 
          atArray != NULL || 
          CMGetArrayElementAt(atArray, 0, &st).value.uint16 == 3) {

        kill(slppid, SIGHUP);  /* restart SLP to update RegisteredProfiles */
      }
   }

   _SFCB_RETURN(st);
}

CMPIStatus InternalProviderDeleteInstance(CMPIInstanceMI * mi,
                                          const CMPIContext * ctx,
                                          const CMPIResult * rslt,
                                          const CMPIObjectPath * cop)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIString *cn = CMGetClassName(cop, NULL);
   CMPIString *ns = CMGetNameSpace(cop, NULL);
   char *key = normalizeObjectPathChars(cop);
   const char *nss=ns->ft->getCharPtr(ns,NULL);
   const char *cns=cn->ft->getCharPtr(cn,NULL);
   const char *bnss=repositoryNs(nss);

   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderDeleteInstance");
   
   if (testNameSpace(bnss,&st)==0) {
      return st;
   }

   if (existingBlob(bnss,cns,key)==0) {
      CMPIStatus st = { CMPI_RC_ERR_NOT_FOUND, NULL };
      return st;
   }

   deleteBlob(bnss,cns,key);

   if (isa(nss, cns, "cim_registeredprofile")) {
          kill(slppid, SIGHUP); /* restart SLP to update RegisteredProfiles */
   }

   _SFCB_RETURN(st);
}

CMPIStatus InternalProviderExecQuery(CMPIInstanceMI * mi,
                                     const CMPIContext * ctx,
                                     const CMPIResult * rslt,
                                     const CMPIObjectPath * cop,
                                     const char *lang, const char *query)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   return st;
}


/* ------------------------------------------------------------------ *
 * Association MI Functions
 * ------------------------------------------------------------------ */

static int ASSOC      = 0;
static int ASSOC_NAME = 1;
static int REF        = 2;
static int REF_NAME   = 3;

CMPIConstClass *getConstClass(const char *ns, const char *cn);

static CMPIConstClass *assocForName(const char *nameSpace, const char *assocClass, 
                         const char *role, const char *resultRole)
{
   CMPIConstClass *cc =getConstClass(nameSpace, assocClass);
   
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "assocForName");
   _SFCB_TRACE(1,("--- nameSpace: %s assocClass: %s cc: %p",nameSpace,assocClass,cc));
   
   if (cc!=NULL && cc->ft->isAssociation(cc) != 0 &&
       (role==NULL || (cc->ft->getProperty(cc,role,NULL).state & CMPI_notFound)==0) &&
       (resultRole==NULL || (cc->ft->getProperty(cc,resultRole,NULL).state & CMPI_notFound)==0)) {
      _SFCB_RETURN(cc); 
   }   
   else _SFCB_RETURN(NULL);
}
 
static int objectPathEquals(UtilStringBuffer *pn, CMPIObjectPath *op, UtilStringBuffer **retName, int eq)
{
   int rc=0;
   UtilStringBuffer *opn=normalizeObjectPathStrBuf(op);
   if (strcmp(pn->ft->getCharPtr(pn),opn->ft->getCharPtr(opn))==0) rc=1;
   if (retName && rc==eq) *retName=opn;
   else opn->ft->release(opn);
   return rc;
}

CMPIStatus getRefs(const CMPIContext * ctx,  const CMPIResult * rslt,
                                       const CMPIObjectPath * cop,
                                       const char *assocClass,
                                       const char *resultClass,
                                       const char *role,
                                       const char *resultRole,
                                       const char **propertyList,
                                       int associatorFunction)
{
   UtilList *refs= UtilFactory->newList();
   char *ns=(char*)CMGetNameSpace(cop,NULL)->hdl;
   CMPIStatus st = { CMPI_RC_OK, NULL };
   
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "getRefs");
   
   if (assocClass != NULL) {
      CMPIObjectPath *path;
      if (assocForName(ns,assocClass,role,resultRole) == NULL) {
	 /* for an unknown class we just return nothing */
         _SFCB_RETURN(st);
      }
      path=CMNewObjectPath(_broker,ns,assocClass,NULL);
      SafeInternalProviderAddEnumInstances(refs, NULL, ctx, path, propertyList, &st, 1);  
   }
    
   else {
      CMPIData rv;
      CMPIObjectPath *op=CMNewObjectPath(Broker,ns,"$ClassProvider$",&st);
      CMPIArgs *in=CMNewArgs(Broker,NULL);
      CMPIArgs *out=CMNewArgs(Broker,NULL);
      rv=CBInvokeMethod(Broker,ctx,op,"getassocs",in,out,&st);
      if (out) {
	int i,m;
         CMPIArray *ar = CMGetArg(out, "assocs", &st).value.array;
         for (i = 0, m = CMGetArrayCount(ar, NULL); i < m; i++) {
            char *name=CMGetArrayElementAt(ar,i,NULL).value.string->hdl;
            if (name) {
	      CMPIObjectPath *cop=CMNewObjectPath(Broker,ns,name,NULL);
	      if (cop) {
		SafeInternalProviderAddEnumInstances(refs, NULL, ctx, cop, propertyList, &st, 1);  
	      }
	    }
            _SFCB_TRACE(1,("--- assoc %s",name));
         }
      }
   }

   
   if (role) {
            // filter out the associations not matching the role property
      CMPIInstance *ci;      
      UtilStringBuffer *pn=normalizeObjectPathStrBuf(cop);
      for (ci=refs->ft->getFirst(refs); ci; ci=refs->ft->getNext(refs)) {
         CMPIData data=CMGetProperty(ci,role,NULL);
         if ((data.state & CMPI_notFound) || 
	     data.type!=CMPI_ref ||
	     objectPathEquals(pn,data.value.ref,NULL,0)==0) {
	   refs->ft->removeCurrent(refs);
         }
      }
      pn->ft->release(pn);
   } 
   
   else {
            // filter out associations not referencing pathName
      CMPIInstance *ci;
      int matched,i,m;      
      UtilStringBuffer *pn=normalizeObjectPathStrBuf(cop);
      for (ci=refs->ft->getFirst(refs); ci; ci=refs->ft->getNext(refs)) {
         for (matched=0,i=0,m=CMGetPropertyCount(ci,NULL); i<m; i++) {
            CMPIData data=CMGetPropertyAt(ci,i,NULL,NULL);
            if (data.type==CMPI_ref && objectPathEquals(pn,data.value.ref,NULL,0)) {
               matched=1;
               break;
            }
         }
         if (matched==0) refs->ft->removeCurrent(refs);
      }
      pn->ft->release(pn);
   }
   
   if (associatorFunction==REF) {
      CMPIInstance *ci;
      for (ci=refs->ft->getFirst(refs); ci; ci=refs->ft->getNext(refs)) {
         CMReturnInstance(rslt,ci);
      }
      refs->ft->release(refs);
      _SFCB_RETURN(st);
   } 
   
   else if (associatorFunction==REF_NAME) {
      CMPIInstance *ci;
      for (ci=refs->ft->getFirst(refs); ci; ci=refs->ft->getNext(refs)) {
         CMPIObjectPath *ref=CMGetObjectPath(ci,NULL);
         CMReturnObjectPath(rslt,ref);
      }
      refs->ft->release(refs);
      _SFCB_RETURN(st);
   } 
   
   else {
            // Use hashtable to avoid dup'd associators
      CMPIInstance *ci;
      UtilHashTable *assocs = UtilFactory->newHashTable(61,UtilHashTable_charKey);
      UtilStringBuffer *pn=normalizeObjectPathStrBuf(cop);
      for (ci=refs->ft->getFirst(refs); ci; ci=refs->ft->getNext(refs)) {
                // Q: for ASSOC_NAME we should not require the
                // object exist if we go by the book, should we?
                // The current approach retrieves the instances
                // via the CIMOM handle
         if (resultRole) {
            CMPIData data=CMGetProperty(ci,resultRole,NULL);
            UtilStringBuffer *an=NULL;
            if ((data.state & CMPI_notFound)==0 && data.type==CMPI_ref && 
		objectPathEquals(pn,data.value.ref,&an,0)==0) {
               if (resultClass ==NULL || CMClassPathIsA(Broker,data.value.ref,resultClass,NULL)) {
                  CMPIInstance *aci=CBGetInstance(Broker,ctx,data.value.ref,propertyList,&st);
                  assocs->ft->put(assocs,an->ft->getCharPtr(an),aci);
               }                  
            }
         } 
         
         else {
                   // must loop over the properties to find ref instances
            int i,m;
            for (i=0,m=CMGetPropertyCount(ci,NULL); i<m; i++) {
               CMPIData data=CMGetPropertyAt(ci,i,NULL,NULL);
               if (data.type==CMPI_ref) {
                  CMPIObjectPath *ref=data.value.ref;
                  CMPIString *tns=CMGetNameSpace(ref,NULL);
                  if (tns==NULL || tns->hdl==NULL) CMSetNameSpace(ref,ns);
                  UtilStringBuffer *an=NULL;
                  if (objectPathEquals(pn,ref,&an,0)==0) {
         
                     if (resultClass==NULL || CMClassPathIsA(Broker,ref,resultClass,NULL)) {
                        CMPIInstance *aci=CBGetInstance(Broker,ctx,ref,propertyList,&st);
                        if (aci) assocs->ft->put(assocs,an->ft->getCharPtr(an),aci);
                     }
                  }
               }
            }
         }
      }
      
      {
         HashTableIterator *it;
         char *an;
         CMPIInstance *aci;
         for (it=assocs->ft->getFirst(assocs,(void**)&an,(void**)&aci); it;
              it=assocs->ft->getNext(assocs,it,(void**)&an,(void**)&aci)) {
            if (associatorFunction == ASSOC) 
               CMReturnInstance(rslt,aci);
            else {
               CMPIObjectPath *op=CMGetObjectPath(aci,NULL);
               CMReturnObjectPath(rslt,op);
            }
         }
      }  
      
      refs->ft->release(refs);
      assocs->ft->release(assocs);
      pn->ft->release(pn);
      _SFCB_RETURN(st);
        
   }
}

CMPIStatus InternalProviderAssociationCleanup(CMPIAssociationMI * mi, const CMPIContext * ctx, CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderAssociationCleanup");
   
   _SFCB_RETURN(st);
}

                            
                            
CMPIStatus InternalProviderAssociators(CMPIAssociationMI * mi,
                                       const CMPIContext * ctx,
                                       const CMPIResult * rslt,
                                       const CMPIObjectPath * cop,
                                       const char *assocClass,
                                       const char *resultClass,
                                       const char *role,
                                       const char *resultRole,
                                       const char **propertyList)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderAssociators");
   st=getRefs(ctx, rslt, cop, assocClass, resultClass,
                        role, resultRole, propertyList, ASSOC);
   _SFCB_RETURN(st);
}

CMPIStatus InternalProviderAssociatorNames(CMPIAssociationMI * mi,
                                           const CMPIContext * ctx,
                                           const CMPIResult * rslt,
                                           const CMPIObjectPath * cop,
                                           const char *assocClass,
                                           const char *resultClass,
                                           const char *role,
                                           const char *resultRole)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderAssociatorNames");
   st=getRefs(ctx, rslt, cop, assocClass, resultClass,
                        role, resultRole, NULL, ASSOC_NAME);
   _SFCB_RETURN(st);
}

CMPIStatus InternalProviderReferences(CMPIAssociationMI * mi,
                                      const CMPIContext * ctx,
                                      const CMPIResult * rslt,
                                      const CMPIObjectPath * cop,
                                      const char *assocClass,
                                      const char *role, const char **propertyList)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderReferences");
   st=getRefs(ctx, rslt, cop, assocClass, NULL,
                        role, NULL, propertyList, REF);
   _SFCB_RETURN(st);
}


CMPIStatus InternalProviderReferenceNames(CMPIAssociationMI * mi,
                                          const CMPIContext * ctx,
                                          const CMPIResult * rslt,
                                          const CMPIObjectPath * cop,
                                          const char *assocClass,
                                          const char *role)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderReferenceNames");
   st=getRefs(ctx, rslt, cop, assocClass, NULL,
                        role, NULL, NULL, REF_NAME);
   _SFCB_RETURN(st);
}



/* ------------------------------------------------------------------ *
 * Method MI Functions
 * ------------------------------------------------------------------ */

CMPIStatus InternalProviderMethodCleanup(CMPIMethodMI * mi,
					 const CMPIContext * ctx,
					 CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };   
   return st;
}

CMPIStatus InternalProviderInvokeMethod(CMPIMethodMI * mi,
					const CMPIContext * ctx,
					const CMPIResult * rslt,
					const CMPIObjectPath * ref,
					const char *methodName,
					const CMPIArgs * in, CMPIArgs * out)
{
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderInvokeMethod");
   CMReturnWithChars(_broker, CMPI_RC_ERR_FAILED, "DefaultProvider does not support invokeMethod operations");
}

/* ------------------------------------------------------------------ *
 * Property MI Functions
 * ------------------------------------------------------------------ */

CMPIStatus InternalProviderPropertyCleanup(CMPIPropertyMI * mi,
                     const CMPIContext * ctx,
                     CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };   
   return st;
}

CMPIStatus InternalProviderSetProperty(CMPIPropertyMI * mi,
                    const CMPIContext * ctx,
                    const CMPIResult * rslt,
                    const CMPIObjectPath * ref,
                    const char *propName,
                    const CMPIData data)
{
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderSetProperty");
   CMReturnWithChars(_broker, CMPI_RC_ERR_FAILED, "DefaultProvider does not support property operations");
}

CMPIStatus InternalProviderGetProperty(CMPIPropertyMI * mi,
                    const CMPIContext * ctx,
                    const CMPIResult * rslt,
                    const CMPIObjectPath * ref,
                    const char *propName)
{
   _SFCB_ENTER(TRACE_INTERNALPROVIDER, "InternalProviderGetProperty");
   CMReturnWithChars(_broker, CMPI_RC_ERR_FAILED, "DefaultProvider does not support property operations");
}

/* ------------------------------------------------------------------ *
 * Instance MI Factory
 *
 * NOTE: This is an example using the convenience macros. This is OK
 *       as long as the MI has no special requirements, i.e. to store
 *       data between calls.
 * ------------------------------------------------------------------ */

CMInstanceMIStub(InternalProvider, InternalProvider, _broker, CMNoHook);

CMAssociationMIStub(InternalProvider, InternalProvider, _broker, CMNoHook);

CMMethodMIStub(InternalProvider, InternalProvider, _broker, CMNoHook);

CMPropertyMIStub(InternalProvider, InternalProvider, _broker, CMNoHook);
