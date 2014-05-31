
/*
 * classProvider.c
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
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * Class provider for sfcb .
 *
*/



#include "utilft.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <zlib.h>

//#include "classRegister.h"
#include "constClass.h"
#include "providerRegister.h"
#include "trace.h"
#include "control.h"

#define NEW(x) ((x *) calloc(1,sizeof(x)))

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpiftx.h"
#include "cmpimacs.h"
#include "cmpimacsx.h"
#include "objectImpl.h"
#include "mrwlock.h"

#define LOCALCLASSNAME "ClassProvider"
#define Iterator HashTableIterator*

static int cacheLimit=10;

static const CMPIBroker *_broker;

extern char * configfile;
extern ProviderRegister *pReg;

typedef struct _Class_Register_FT Class_Register_FT;
struct _ClassRegister {
   void *hdl;
   Class_Register_FT *ft;
   ClVersionRecord *vr;
   int assocs,topAssocs;
   char *fn;
   gzFile *f;
};
typedef struct _ClassRegister ClassRegister;

typedef struct _ClassRecord {
   struct _ClassRecord *nextCached,*prevCached;
   char *parent;
   z_off_t position;
   long length;
   CMPIConstClass *cachedCls;
   unsigned int flags;
   #define CREC_isAssociation 1
} ClassRecord;

typedef struct _ClassBase {
   UtilHashTable *ht;
   UtilHashTable *it;
   MRWLOCK mrwLock;
   ClassRecord *firstCached,*lastCached;
   int cachedCount;
} ClassBase;

struct _Class_Register_FT {
   int version;
   void (*release) (ClassRegister * br);
   ClassRegister *(*clone) (ClassRegister * br);
   
   CMPIConstClass *(*getClass) (ClassRegister * br, const char *clsName, void **id);
   int (*putClass) (ClassRegister * br,  const char *className, ClassRecord * cls);
   void (*removeClass) (ClassRegister * br, const char *className);
   void (*releaseClass) (ClassRegister *br, void *id);

   UtilList *(*getChildren) (ClassRegister * br, const char *className);
   void (*addChild)(ClassRegister *cr, const char *p, const char *child);
   
   Iterator (*getFirstClassRecord)(ClassRegister *cr, char **cn, ClassRecord **crec);
   Iterator (*getNextClassRecord)(ClassRegister *cr, Iterator i, char **cn, 
      ClassRecord **crec);
      
   Iterator (*getFirstClass)(ClassRegister *cr, char **cn, CMPIConstClass **cls, void **id);
   Iterator (*getNextClass)(ClassRegister *cr, Iterator i, char **cn, 
      CMPIConstClass **cls, void **id);
      
   void (*rLock)(ClassRegister * cr);
   void (*wLock)(ClassRegister * cr);
   void (*rUnLock)(ClassRegister * cr);
   void (*wUnLock)(ClassRegister * cr);
};

extern Class_Register_FT *ClassRegisterFT;


static CMPIConstClass *getClass(ClassRegister * cr, const char *clsName, void **id);

typedef struct nameSpaces {
   int next,max,blen;
   char *base;
   char *names[1];
} NameSpaces;

static UtilHashTable *nsHt=NULL;
static int nsBaseLen;

static void buildInheritanceTable(ClassRegister * cr)
{
   Iterator i;
   char *cn;
   ClassRecord *crec;

   for (i = cr->ft->getFirstClassRecord(cr, &cn, &crec); i;
        i = cr->ft->getNextClassRecord(cr, i, &cn, &crec)) {
      if (crec->parent == NULL) continue;
      cr->ft->addChild(cr,crec->parent,cn);
   }
}

static void release(ClassRegister * cr)
{
   ClassBase *cb = (ClassBase *) cr->hdl;
   free(cr->fn);
   cb->ht->ft->release(cb->ht);
   free(cr);
}

static ClassRegister *regClone(ClassRegister * cr)
{
   return NULL;
}

void rLock(ClassRegister * cr) {
   ClassBase *cb = (ClassBase *) cr->hdl;
   MReadLock(&cb->mrwLock);
}

void wLock(ClassRegister * cr) {
   ClassBase *cb = (ClassBase *) cr->hdl;
   MWriteLock(&cb->mrwLock);
}

void rUnLock(ClassRegister * cr) {
   ClassBase *cb = (ClassBase *) cr->hdl;
   MReadUnlock(&cb->mrwLock);
}

void wUnLock(ClassRegister * cr) {
   ClassBase *cb = (ClassBase *) cr->hdl;
   MWriteUnlock(&cb->mrwLock);
}

static Iterator getFirstClassRecord(ClassRegister *cr, char **cn, ClassRecord **crec)
{
   ClassBase *cb = (ClassBase *) cr->hdl;
   return cb->ht->ft->getFirst(cb->ht,(void**)cn, (void**)crec);
}

static Iterator getNextClassRecord(ClassRegister *cr, Iterator i, char **cn, 
      ClassRecord **crec)
{
   ClassBase *cb = (ClassBase *) cr->hdl;
   return cb->ht->ft->getNext(cb->ht, i, (void**)cn, (void**)crec);
}      

static Iterator getFirstClass(ClassRegister *cr, char **cn, CMPIConstClass **cls, void **id)
{
   char *buf;
   CMPIConstClass *cc;
   ClassBase *cb = (ClassBase *) cr->hdl;
   ClassRecord *crec;
   int r;
   
   Iterator i = cb->ht->ft->getFirst(cb->ht, (void**)cn, (void**)&crec);
   if (i==0) return i;
 
   if (crec->cachedCls) {
      *id=crec;
      *cls=crec->cachedCls;
      return i;
   }
   *id=NULL;
   
   r=gzseek(cr->f, crec->position, SEEK_SET);
   
   buf = (char *) malloc(crec->length);
   r=gzread(cr->f, buf, crec->length);

   cc = NEW(CMPIConstClass);
   cc->hdl = buf;
   cc->ft = CMPIConstClassFT;
   cc->ft->relocate(cc);
   *cls=cc;
   return i;
}

static Iterator getNextClass(ClassRegister *cr, Iterator ip, char **cn, 
      CMPIConstClass **cls, void **id)
{
   char *buf;
   CMPIConstClass *cc;
   ClassBase *cb = (ClassBase *) cr->hdl;
   ClassRecord *crec;
   int r;
   
   Iterator i = cb->ht->ft->getNext(cb->ht, ip, (void**)cn, (void**)&crec);
   if (i==0) return i;
 
   if (crec->cachedCls) {
      *id=crec;
      *cls=crec->cachedCls;
      return i;
   }
   *id=NULL;
   
   r=gzseek(cr->f, crec->position, SEEK_SET);
   
   buf = (char *) malloc(crec->length);
   r=gzread(cr->f, buf, crec->length);  
   
   cc = NEW(CMPIConstClass);
   cc->hdl = buf;
   cc->ft = CMPIConstClassFT;
   cc->ft->relocate(cc);
   *cls=cc;
   return i;
}      

static UtilList *getChildren(ClassRegister * cr, const char *className)
{
   ClassBase *cb = (ClassBase *) (cr + 1);
   return cb->it->ft->get(cb->it, className);
}

static void addChild(ClassRegister *cr, const char *p, const char *child)
{
   UtilList *ul = ((ClassBase*)(cr+1))->it->ft->get(((ClassBase*)(cr+1))->it, p);
   if (ul == NULL) {
      ul = UtilFactory->newList();
      ((ClassBase*)(cr+1))->it->ft->put(((ClassBase*)(cr+1))->it, p, ul);
   }
   ul->ft->prepend(ul, child);
}


static void pruneCache(ClassRegister * cr)
{
   ClassBase *cb = (ClassBase *) (cr + 1);
   ClassRecord *crec;
   
   while (cb->cachedCount>cacheLimit) {
      crec=cb->lastCached;
//    fprintf(stderr,"--- removing %s from cache\n",crec->cachedCls->ft->getCharClassName(crec->cachedCls));
      DEQ_FROM_LIST(crec,cb->firstCached,cb->lastCached,nextCached,prevCached);
      crec->cachedCls->ft->release(crec->cachedCls);
      crec->cachedCls=NULL;
      cb->cachedCount--;
   }   
}


static ClassRegister *newClassRegister(char *fname)
{
   ClassRegister *cr =
       (ClassRegister *) calloc(1,sizeof(ClassRegister) + sizeof(ClassBase));
   ClassBase *cb = (ClassBase *) (cr + 1);
   char fin[1024];
   long s, total=0;
   ClObjectHdr hdr;
   ClVersionRecord *vrp=(ClVersionRecord*)&hdr;
   int vRec=0,first=1;
   
   z_off_t pos=0;
   ClassRecord *crec;
   
   cr->hdl = cb;
   cr->ft = ClassRegisterFT;
   cr->vr = NULL;
   cr->assocs = cr->topAssocs = 0;
   
   strcpy(fin, fname);
   strcat(fin, "/classSchemas");
   cr->f = gzopen(fin, "r");
   if (cr->f==NULL) {
      strcat(fin, ".gz");
      cr->f = gzopen(fin, "r");
   }
   
   
   cb->ht = UtilFactory->newHashTable(61,
            UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);
   cb->it = UtilFactory->newHashTable(61,
            UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);
   MRWInit(&cb->mrwLock);
      
   if (cr->f == NULL)  return cr;

   cr->fn = strdup(fin);
   cr->vr=NULL;
   pos=gztell(cr->f);

   while ((s = gzread(cr->f, &hdr, sizeof(hdr))) == sizeof(hdr)) {
      CMPIConstClass *cc=NULL;
      char *buf=NULL;
      const char *cn,*pn;
      
      if (first) {
         if (vrp->size==sizeof(ClVersionRecord) && vrp->type==HDR_Version) vRec=1;
         else if (vrp->size==sizeof(ClVersionRecord)<<24 && vrp->type==HDR_Version) {
            mlogf(M_ERROR,M_SHOW,"--- %s is in wrong endian format - directory skipped\n",fin);
            return NULL;
         }
      }
      
      if (vRec==0 && hdr.type!=HDR_Class) {
         mlogf(M_ERROR,M_SHOW,"--- %s contains non-class record(s) - directory skipped\n",fin);
         return NULL;
     }
      
      buf = (char *) malloc(hdr.size);
      if (buf==NULL) {
         mlogf(M_ERROR,M_SHOW,"--- %s contains record(s) that are too long - directory skipped\n",fin);
         return NULL;
      }
      
      s=hdr.size;
      *((ClObjectHdr *) buf) = hdr;
      
      if (gzread(cr->f, buf + sizeof(hdr), hdr.size - sizeof(hdr)) == hdr.size - sizeof(hdr)) {
         if (vRec) {
            cr->vr=(ClVersionRecord*)buf;
            if (strcmp(cr->vr->id,"sfcb-rep")) {
               mlogf(M_ERROR,M_SHOW,"--- %s contains invalid version record - directory skipped\n",fin);
               return NULL;
            }   
            pos=gztell(cr->f);
            vRec=0;
         }
         
         if (first) {
            int v=-1;
            first=0;
            if (ClVerifyObjImplLevel(cr->vr)) continue;
            if (cr->vr) v=cr->vr->objImplLevel;
            mlogf(M_ERROR,M_SHOW,"--- %s contains unsupported object implementation format (%d) - directory skipped\n",
               fin,v);
            return NULL;
         }
         
         cc = NEW(CMPIConstClass);
         cc->hdl = buf;
         cc->ft = CMPIConstClassFT;
         cc->ft->relocate(cc);
         cn=(char*)cc->ft->getCharClassName(cc);
         
         if (strncmp(cn,"DMY_",4)!=0) {
            total+=sizeof(ClassRecord);   
            crec=(ClassRecord*)calloc(1,sizeof(ClassRecord));  
            
            if ((pn=cc->ft->getCharSuperClassName(cc))) {
               crec->parent=strdup(pn);
            }
            crec->position=pos;
            crec->length=s;
            cr->ft->putClass(cr, strdup(cn), crec);
            
            if (cc->ft->isAssociation(cc)) {
               crec->flags|=CREC_isAssociation;
               cr->assocs++;
               if (pn == NULL) cr->topAssocs++;
            }   
         } 
         first=0;
      }
      else {
         mlogf(M_ERROR,M_SHOW,"--- %s contains invalid record(s) - directory skipped\n",fin);
         return NULL;
      }
      pos=gztell(cr->f);
      free(buf);
      free(cc);
   }
 
   if (cr->vr) {
      mlogf(M_INFO,M_SHOW,"--- Caching ClassProvider for %s (%d.%d-%d) using %ld bytes\n", 
          fin, cr->vr->version, cr->vr->level, cr->vr->objImplLevel, total);
   }
   else mlogf(M_INFO,M_SHOW,"--- Caching ClassProvider for %s (no-version) using %ld bytes\n", fin, total);

   buildInheritanceTable(cr);
   
   return cr;
}

static UtilHashTable *gatherNameSpaces(char *dn, UtilHashTable *ns, int first)
{
   DIR *dir, *dir_test;
   struct dirent *de;
   char *n=NULL;
   int l;
   ClassRegister *cr;
   
   if (ns==NULL) {
      ns= UtilFactory->newHashTable(61,
             UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);
      nsBaseLen=strlen(dn)+1;       
   }          
       
   dir=opendir(dn);
   if (dir) while ((de=readdir(dir))!=NULL) {
     if (strcmp(de->d_name,".")==0) continue;
     if (strcmp(de->d_name,"..")==0) continue;
     l=strlen(dn)+strlen(de->d_name)+4;
     n=(char*)malloc(l+8);
     strcpy(n,dn);
     strcat(n,"/");
     strcat(n,de->d_name);
     dir_test = opendir(n);
     if (dir_test == NULL) {
       free(n);
       continue;
     }
     closedir(dir_test);
     cr=newClassRegister(n);
     if (cr) {
       ns->ft->put(ns, n+nsBaseLen, cr);
       gatherNameSpaces(n,ns,0);
     } else {
        free(n);
     }
   }
   else if (first) {
      mlogf(M_ERROR,M_SHOW,"--- Repository %s not found\n",dn);
   }
   closedir(dir);  
   return ns;     
} 

static UtilHashTable *buildClassRegisters()
{
   char *dir;
   char *dn;

   setupControl(configfile);

   if (getControlChars("registrationDir",&dir)) {
     dir = "/var/lib/sfcb/registration";
   }
   
   dn=(char*)alloca(strlen(dir)+32);
   strcpy(dn,dir);
   if (dir[strlen(dir)-1]!='/') strcat(dn,"/");
   strcat(dn,"repository");
   return gatherNameSpaces(dn,NULL,1);   
}    


static ClassRegister *getNsReg(const CMPIObjectPath *ref, int *rc)
{
   char *ns;
   CMPIString *nsi=CMGetNameSpace(ref,NULL);
   ClassRegister *cReg;
   *rc=0;
   
   if (nsHt==NULL) nsHt=buildClassRegisters();
   
   if (nsi && nsi->hdl) {
      ns=(char*)nsi->hdl;
      if (strcasecmp(ns,"root/pg_interop")==0)
         cReg=nsHt->ft->get(nsHt,"root/interop");
      else cReg=nsHt->ft->get(nsHt,ns);
      return cReg;
   }
   
   *rc=1;
   return NULL;
}

static int putClass(ClassRegister * cr, const char *cn, ClassRecord * crec)
{
   ClassBase *cb = (ClassBase *) cr->hdl;
   return cb->ht->ft->put(cb->ht, cn, crec);
}


static CMPIConstClass *getClass(ClassRegister * cr, const char *clsName, void **id)
{
   ClassRecord *crec;
   int r;
   CMPIConstClass *cc;
   char *buf;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "getClass");
   _SFCB_TRACE(1,("--- classname %s cReg %p",clsName,cr));
   ClassBase *cb = (ClassBase *) cr->hdl;
   
   crec = cb->ht->ft->get(cb->ht, clsName);
   if (crec==NULL) {
      _SFCB_RETURN(NULL);
   }
   
   if (crec->cachedCls==NULL) {
//   fprintf(stderr,"--- reading class %s\n",clsName);
      r=gzseek(cr->f, crec->position, SEEK_SET);
      buf = (char *) malloc(crec->length);
      r=gzread(cr->f, buf, crec->length);  
   
      cc = NEW(CMPIConstClass);
      cc->hdl = buf;
      cc->ft = CMPIConstClassFT;
      cc->ft->relocate(cc);
      crec->cachedCls=cc;
      cb->cachedCount++;
      if (cb->cachedCount>=cacheLimit) pruneCache(cr);
      ENQ_TOP_LIST(crec,cb->firstCached,cb->lastCached,nextCached,prevCached);
   }
   else {
      if (crec!=cb->firstCached) {
         DEQ_FROM_LIST(crec,cb->firstCached,cb->lastCached,nextCached,prevCached);
         ENQ_TOP_LIST(crec,cb->firstCached,cb->lastCached,nextCached,prevCached);
      }
//      fprintf(stderr,"--- in cache class %s\n",clsName);
   }   
   _SFCB_RETURN(crec->cachedCls);   
}

static Class_Register_FT ift = {
   1,
   release,
   regClone,
   getClass,
   putClass,
   NULL,
   NULL,
   getChildren,
   addChild,
   getFirstClassRecord,
   getNextClassRecord,
   getFirstClass,
   getNextClass,
   rLock, 
   wLock, 
   rUnLock,
   wUnLock
};

Class_Register_FT *ClassRegisterFT = &ift;



/* ------------------------------------------------------------------ *
 * Class MI Cleanup
 * ------------------------------------------------------------------ */

static CMPIStatus ClassProviderCleanup(CMPIClassMI * mi, CMPIContext * ctx)
{
/* 
   ClassBase *cb;
   UtilHashTable *ct;
   HashTableIterator *i;
   CMPIConstClass *cc;
   char *cn;

      
   if (cReg==NULL) CMReturn(CMPI_RC_OK);
   cb = (ClassBase *) (cReg + 1);
   ct = cb->ht;

   for (i = ct->ft->getFirst(ct, (void **) &cn, (void **) &cc); i;
        i = ct->ft->getNext(ct, i, (void **) &cn, (void **) &cc)) {
       free(cc->hdl);
       free(cc);
   }
*/
   CMReturn(CMPI_RC_OK);
}

/* ------------------------------------------------------------------ *
 * Class MI Functions
 * ------------------------------------------------------------------ */

static void loopOnChildNames(ClassRegister *cReg, char *cn, CMPIResult * rslt)
{
   CMPIObjectPath *op;
   UtilList *ul = getChildren(cReg,cn);
   char *child;
   if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  child = (char *) ul->ft->getNext(ul)) {
      op=CMNewObjectPath(_broker,NULL,child,NULL);
      CMReturnObjectPath(rslt,op);
      loopOnChildNames(cReg,child,rslt);
   }     
}
 

static CMPIStatus ClassProviderEnumClassNames(CMPIClassMI * mi,
                                          CMPIContext * ctx,
                                          CMPIResult * rslt,
                                          CMPIObjectPath * ref)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char *cn=NULL;
   CMPIFlags flgs=0;
   CMPIString *cni;
   ClassBase *cb;
   Iterator it;
   char *key;
   int rc,n;
   ClassRecord *crec;
   CMPIObjectPath *op;
   ClassRegister *cReg;
   char *ns;

   _SFCB_ENTER(TRACE_PROVIDERS, "ClassProviderEnumClassNames");
   
   cReg=getNsReg(ref, &rc);
   if (cReg==NULL) {
      CMPIStatus st = { CMPI_RC_ERR_INVALID_NAMESPACE, NULL };
      _SFCB_RETURN(st);
   }
      
   ns=(char*)CMGetNameSpace(ref,NULL)->hdl;
   flgs=ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
   cni=ref->ft->getClassName(ref,NULL);
   if (cni) cn=(char*)cni->hdl;
   cb = (ClassBase *) cReg->hdl;  

   cReg->ft->wLock(cReg);
      
   if (cn && strcasecmp(cn,"$ClassProvider$")==0) cn=NULL;
   
   if (cn==NULL) {
      n=0;
      for (it = cReg->ft->getFirstClassRecord(cReg, &key, &crec);
           key && it && crec;
           it = cReg->ft->getNextClassRecord(cReg, it, &key, &crec)) {
         if ((flgs & CMPI_FLAG_DeepInheritance) || crec->parent==NULL) { 
            if (((flgs & FL_assocsOnly)==0) || crec->flags & CREC_isAssociation) {
               op=CMNewObjectPath(_broker,ns,key,NULL);
               CMReturnObjectPath(rslt,op);
            }   
         }   
      }     
   } else {
     CMPIConstClass *cls = getClass(cReg,cn,NULL);
     if (cls == NULL) {
       st.rc = CMPI_RC_ERR_INVALID_CLASS;
     } else if ((flgs & CMPI_FLAG_DeepInheritance)==0) {
       UtilList *ul = getChildren(cReg,cn);
       char *child;
       if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  child = (char *) ul->ft->getNext(ul)) {
         op=CMNewObjectPath(_broker,ns,child,NULL);
         CMReturnObjectPath(rslt,op);
       }     
     } else if (flgs & CMPI_FLAG_DeepInheritance) {
       if (((flgs & FL_assocsOnly)==0) || crec->flags & CREC_isAssociation)
         loopOnChildNames(cReg, cn, rslt);
     }
   }
     
   cReg->ft->wUnLock(cReg);
      
   _SFCB_RETURN(st);
}

static void loopOnChildren(ClassRegister *cReg, char *cn, CMPIResult * rslt)
{
   UtilList *ul = getChildren(cReg,cn);
   char *child;
   void *cid;

   if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  child = (char *) ul->ft->getNext(ul)) {
      CMPIConstClass *cl = getClass(cReg,child,&cid);
      CMReturnInstance(rslt, (CMPIInstance *) cl);
      if (cid==NULL) CMRelease(cl);
      loopOnChildren(cReg,child,rslt);
   }     
}
 
 

static CMPIStatus ClassProviderEnumClasses(CMPIClassMI * mi,
                                      CMPIContext * ctx,
                                      CMPIResult * rslt,
                                      CMPIObjectPath * ref)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char *cn=NULL;
   CMPIFlags flgs=0;
   CMPIString *cni;
   ClassBase *cb;
   Iterator it;
   char *key;
   int rc;
   CMPIConstClass *cls;
   ClassRegister *cReg;
   void *cid;

   _SFCB_ENTER(TRACE_PROVIDERS, "ClassProviderEnumClasss");
   
   cReg=getNsReg(ref, &rc);
   if (cReg==NULL) {
      CMPIStatus st = { CMPI_RC_ERR_INVALID_NAMESPACE, NULL };
      _SFCB_RETURN(st);
   }
           
   cReg->ft->wLock(cReg);
   
   flgs=ctx->ft->getEntry(ctx,CMPIInvocationFlags,NULL).value.uint32;
   cni=ref->ft->getClassName(ref,NULL);
   if (cni) {
      cn=(char*)cni->hdl;
      if (cn && *cn==0) cn=NULL;
   }   
   cb = (ClassBase *) cReg->hdl;
 
      
   if (cn==NULL) {
      for (it = cReg->ft->getFirstClass(cReg, &key, &cls,&cid);
           key && it && cls;
           it = cReg->ft->getNextClass(cReg, it, &key, &cls,&cid)) {
         if ((flgs & CMPI_FLAG_DeepInheritance) || cls->ft->getCharSuperClassName(cls)==NULL) {  
            CMReturnInstance(rslt, (CMPIInstance *) cls);
            if (cid==NULL) CMRelease(cls);
         }   
      }     
   } else {
     cls = getClass(cReg,cn,NULL);
     if (cls == NULL) {
       st.rc = CMPI_RC_ERR_INVALID_CLASS;
     } else if ((flgs & CMPI_FLAG_DeepInheritance)==0) {
       UtilList *ul = getChildren(cReg,cn);
       char *child;
       if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  child = (char *) ul->ft->getNext(ul)) {
         cls = getClass(cReg,child,&cid);
         CMReturnInstance(rslt, (CMPIInstance *) cls);
         if (cid==NULL) CMRelease(cls);
       }     
     } else if (cn && (flgs & CMPI_FLAG_DeepInheritance)) {
       loopOnChildren(cReg, cn, rslt);
     }
   }
     
   cReg->ft->wUnLock(cReg);
   
   _SFCB_RETURN(st);
}


static CMPIStatus ClassProviderGetClass(CMPIClassMI * mi,
                                    CMPIContext * ctx,
                                    CMPIResult * rslt,
                                    CMPIObjectPath * ref, char **properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIString *cn = CMGetClassName(ref, NULL);
   CMPIConstClass *cl, *clLocal;
   ClassRegister *cReg;
   int rc;
   void *cid;

   _SFCB_ENTER(TRACE_PROVIDERS, "ClassProviderGetClass");
   _SFCB_TRACE(1,("--- ClassName=\"%s\"",(char *) cn->hdl));
   
   cReg=getNsReg(ref, &rc);
   if (cReg==NULL) {
      CMPIStatus st = { CMPI_RC_ERR_INVALID_NAMESPACE, NULL };
      _SFCB_RETURN(st);
   }

   cReg->ft->wLock(cReg);
   
   /* Make a cloned copy of the cached results to prevent
      thread interference. */
   clLocal = getClass(cReg, (char *) cn->hdl,&cid);

   if (clLocal) {
      _SFCB_TRACE(1,("--- Class found"));
      cl = clLocal->ft->clone(clLocal, NULL);
      memLinkInstance((CMPIInstance*)cl);
      CMReturnInstance(rslt, (CMPIInstance *) cl);
   } else {
      _SFCB_TRACE(1,("--- Class not found"));
      st.rc = CMPI_RC_ERR_NOT_FOUND;
   }
   
   cReg->ft->wUnLock(cReg);
   
   _SFCB_RETURN(st);
}

static CMPIStatus ClassProviderCreateClass(CMPIClassMI * mi,
                                       CMPIContext * ctx,
                                       CMPIResult * rslt,
                                       CMPIObjectPath * ref, CMPIConstClass * cc)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   return st;
}

static CMPIStatus ClassProviderSetClass(CMPIClassMI * mi,
                                    CMPIContext * ctx,
                                    CMPIResult * rslt,
                                    CMPIObjectPath * cop,
                                    CMPIConstClass * ci)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   return st;
}

static CMPIStatus ClassProviderDeleteClass(CMPIClassMI * mi,
                                       CMPIContext * ctx,
                                       CMPIResult * rslt, CMPIObjectPath * cop)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   return st;
}

/* ---------------------------------------------------------------------------*/
/*                        Method Provider Interface                           */
/* ---------------------------------------------------------------------------*/

extern CMPIBoolean isAbstract(CMPIConstClass * cc);

static int repCandidate(ClassRegister *cReg, char *cn)
{ 
   void *cid;
   CMPIConstClass *cl = getClass(cReg,cn,&cid);
   if (isAbstract(cl)) return 0;
   ProviderInfo *info;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "repCandidate");
  
   if (strcasecmp(cn,"cim_indicationfilter")==0 ||  
       strcasecmp(cn,"cim_indicationsubscription")==0) _SFCB_RETURN(0);
         
   while (cn != NULL) {
      info = pReg->ft->getProvider(pReg, cn, INSTANCE_PROVIDER);
      if (info) _SFCB_RETURN(0);
      cn = (char*)cl->ft->getCharSuperClassName(cl);
      if (cn==NULL) break;
      cl = getClass(cReg,cn,&cid);
   }
   _SFCB_RETURN(1);
}

static void loopOnChildChars(ClassRegister *cReg, char *cn, CMPIArray *ar, int *i, int ignprov)
{
   UtilList *ul = getChildren(cReg,cn);
   char *child;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "loopOnChildChars");
   _SFCB_TRACE(1,("--- class %s",cn));
   
   if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  
         child=(char*)ul->ft->getNext(ul)) {
      if (ignprov || repCandidate(cReg, child)) {
         CMSetArrayElementAt(ar, *i, child, CMPI_chars);
         *i=(*i)+1;
      }   
      loopOnChildChars(cReg, child,ar,i,ignprov);
   }     
   _SFCB_EXIT();
}

static void loopOnChildCount(ClassRegister *cReg, char *cn, int *i, int ignprov)
{
   UtilList *ul = getChildren(cReg,cn);
   char *child;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "loopOnChildCount");
   
   if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  
         child=(char*)ul->ft->getNext(ul)) {
      if (ignprov || repCandidate(cReg, child)) *i=(*i)+1;
      loopOnChildCount(cReg, child,i,ignprov);
   }     
   _SFCB_EXIT();
}


static CMPIStatus ClassProviderMethodCleanup(CMPIMethodMI * mi, 
				      const CMPIContext * ctx, 
				      CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   return st;
}

static CMPIStatus ClassProviderInvokeMethod(CMPIMethodMI * mi,
                                     const CMPIContext * ctx,
                                     const CMPIResult * rslt,
                                     const CMPIObjectPath * ref,
                                     const char *methodName,
                                     const CMPIArgs * in, CMPIArgs * out)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIArray *ar;
   int rc;
   ClassRegister *cReg;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ClassProviderInvokeMethod");

   cReg=getNsReg(ref, &rc);
   if (cReg==NULL) {
      CMPIStatus st = { CMPI_RC_ERR_INVALID_NAMESPACE, NULL };
      _SFCB_RETURN(st);
   }

   if (strcasecmp(methodName, "getchildren") == 0) {
      CMPIData cn = CMGetArg(in, "class", NULL);
      _SFCB_TRACE(1,("--- getchildren %s",(char*)cn.value.string->hdl));
      
      cReg->ft->wLock(cReg);
   
      if (cn.type == CMPI_string && cn.value.string && cn.value.string->hdl) {
         char *child;
         int l=0, i=0;
         UtilList *ul = getChildren(cReg, (char *) cn.value.string->hdl);
         if (ul) l = ul->ft->size(ul);
         ar = CMNewArray(_broker, l, CMPI_string, NULL);
         if (ul) for (child = (char *) ul->ft->getFirst(ul); child;  child = (char *)
               ul->ft->getNext(ul)) {
            CMSetArrayElementAt(ar, i++, child, CMPI_chars);
         }
         st = CMAddArg(out, "children", &ar, CMPI_stringA);
      }
      else {
      }
      
      cReg->ft->wUnLock(cReg);
   
   }
   
   else if (strcasecmp(methodName, "getallchildren") == 0) {
      int ignprov=0;
      CMPIStatus st;
      CMPIData cn = CMGetArg(in, "class", &st);
      
      cReg->ft->wLock(cReg);
   
      if (st.rc!=CMPI_RC_OK) {
         cn = CMGetArg(in, "classignoreprov", NULL);
         ignprov=1;
      }
      _SFCB_TRACE(1,("--- getallchildren %s",(char*)cn.value.string->hdl));
      if (cn.type == CMPI_string && cn.value.string && cn.value.string->hdl) {
         int n=0,i=0;
         loopOnChildCount(cReg,(char *)cn.value.string->hdl,&n,ignprov);
         _SFCB_TRACE(1,("--- count %d",n));
         ar = CMNewArray(_broker, n, CMPI_string, NULL);
         if (n) {
            _SFCB_TRACE(1,("--- loop %s",(char*)cn.value.string->hdl));
            loopOnChildChars(cReg, (char *)cn.value.string->hdl,ar,&i,ignprov);         
         }   
         st = CMAddArg(out, "children", &ar, CMPI_stringA);
      }
      else {
      }
      
      cReg->ft->wUnLock(cReg);   
   }

   else if (strcasecmp(methodName, "getassocs") == 0) {
      ar = CMNewArray(_broker, cReg->topAssocs, CMPI_string, NULL);
      ClassBase *cb = (ClassBase *) (cReg + 1);
      UtilHashTable *ct = cb->ht;
      HashTableIterator *i;
      char *cn;
      ClassRecord *crec;
      int n;

      cReg->ft->wLock(cReg);
   
       for (n = 0, i = ct->ft->getFirst(ct, (void **) &cn, (void **) &crec); i;
           i = ct->ft->getNext(ct, i, (void **) &cn, (void **) &crec)) {
         if(crec->flags & CREC_isAssociation && crec->parent == NULL) {
	   /* add top-level association class */
	   CMSetArrayElementAt(ar, n++, cn, CMPI_chars);
         }
      }
      CMAddArg(out, "assocs", &ar, CMPI_stringA);
      
      cReg->ft->wUnLock(cReg);
   }

   else if (strcasecmp(methodName, "ischild") == 0) {
      char *parent=(char*)CMGetClassName(ref,NULL)->hdl;
      UtilList *ul = getChildren(cReg, parent);
      char *chldn=(char*)CMGetArg(in, "child", NULL).value.string->hdl;
      char *child;
      
      cReg->ft->wLock(cReg);
   
      st.rc = CMPI_RC_ERR_FAILED;
      if (ul) for (child=(char*)ul->ft->getFirst(ul); child; 
                   child=(char*)ul->ft->getNext(ul)) {
         if (strcasecmp(child,chldn)==0 ) {
            st.rc=CMPI_RC_OK;
            break;
         }   
      }
      
      cReg->ft->wUnLock(cReg);
   }
   
   else if (strcasecmp(methodName, "_startup") == 0) {
      st.rc=CMPI_RC_OK;
  }
   
   else {
      mlogf(M_ERROR,M_SHOW,"--- ClassProvider: Invalid invokeMethod request %s\n", methodName);
      st.rc = CMPI_RC_ERR_METHOD_NOT_FOUND;
   }
   _SFCB_RETURN(st);
}


CMClassMIStub(ClassProvider, ClassProvider, _broker, CMNoHook);

CMMethodMIStub(ClassProvider, ClassProvider, _broker, CMNoHook);
 
//
//
