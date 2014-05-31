
/*
 * $Id: support.c,v 1.30 2008/06/25 23:15:36 buccella Exp $
 *
 *  © Copyright IBM Corp. 2005, 2007
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
 * Description: Various memory management and support routines.
 *
 */

/** @file support.c
 *  @brief Memory managment system and support routines for providers
 *
 *  @author Frank Scheffler
 *
 *  @sa support.h
 *  @sa native.h
 */

#include <stdio.h>
#include <dlfcn.h>
#include "support.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include "native.h"
#include "trace.h"
#include "config.h"
#include <pthread.h>

#ifdef SFCB_IX86
#define SFCB_ASM(x) asm(x)
#else
#define SFCB_ASM(x)
#endif

int collectStat=0;            /**< flag determining whether statistic collection is enabled */
unsigned long exFlags = 0;    /**< flag determining whether extra options are enabled (currently only determines interop support) */
int localClientMode=0;        /**< flag determining whether local client connections are enabled */

void *loadLibib(const char *libname)
{
   char filename[255];
   sprintf(filename, "lib%s.so", libname);
   return dlopen(filename, RTLD_LAZY);
}


static void *getGenericEntryPoint(void *library, const char *ptype)
{
   char entry_point[255];
   void *sym;
   sprintf(entry_point, "_Generic_Create_%sMI", ptype);
   sym = dlsym(library, entry_point);
   return sym;
}


static void *getFixedEntryPoint(const char *provider,
                                     void *library, const char *ptype)
{
   char entry_point[255];
   void *sym;
   sprintf(entry_point, "%s_Create_%sMI", provider, ptype);
   sym = dlsym(library, entry_point);
   return sym;
}


typedef CMPIInstanceMI *(*GENERIC_InstanceMI) (CMPIBroker * broker,
                                               CMPIContext * ctx,
                                               const char * provider,
					       CMPIStatus * status);
typedef CMPIInstanceMI *(*FIXED_InstanceMI) (CMPIBroker * broker,
                                             CMPIContext * ctx,
					     CMPIStatus * status);


CMPIInstanceMI *loadInstanceMI(const char *provider, void *library,
                                     CMPIBroker * broker, CMPIContext * ctx)
{
   CMPIInstanceMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadInstanceMI");
   
   GENERIC_InstanceMI g = (GENERIC_InstanceMI) 
      getGenericEntryPoint(library,"Instance");
   if (g == NULL) {
      FIXED_InstanceMI f = (FIXED_InstanceMI) 
         getFixedEntryPoint(provider, library, "Instance");
      if (f == NULL) _SFCB_RETURN(NULL);
      if (broker) {
	mi=(f) (broker, ctx, &status);
	if (mi && status.rc == CMPI_RC_OK)
         _SFCB_RETURN(mi);
      }   
      _SFCB_RETURN((CMPIInstanceMI*)(void*)-1);
   }
   if (broker) {
     mi=(g) (broker, ctx, provider, &status);
     if (mi && status.rc == CMPI_RC_OK)
        _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIInstanceMI*)(void*)-1);
};


typedef CMPIAssociationMI *(*GENERIC_AssociationMI) (CMPIBroker * broker,
                                                     CMPIContext * ctx,
                                                     const char * provider,
						     CMPIStatus * status);
typedef CMPIAssociationMI *(*FIXED_AssociationMI) (CMPIBroker * broker,
                                                   CMPIContext * ctx,
						   CMPIStatus * status);


CMPIAssociationMI *loadAssociationMI(const char *provider,
                                           void *library,
                                           CMPIBroker * broker,
                                           CMPIContext * ctx)
{
   CMPIAssociationMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadAssociationMI");
   
   GENERIC_AssociationMI g = (GENERIC_AssociationMI) 
      getGenericEntryPoint(library, "Association");
      
   if (g == NULL) {
      FIXED_AssociationMI f = (FIXED_AssociationMI) 
         getFixedEntryPoint(provider, library, "Association");
      if (f == NULL) _SFCB_RETURN(NULL);
      if (broker) {
	mi=(f) (broker, ctx, &status);
	if (mi && status.rc == CMPI_RC_OK)
	  _SFCB_RETURN(mi);
      }   
      _SFCB_RETURN((CMPIAssociationMI*)(void*)-1);
   }
   if (broker) {
     mi=(g) (broker, ctx, provider, &status);
     if (mi && status.rc == CMPI_RC_OK)
       _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIAssociationMI*)(void*)-1);
};


typedef CMPIMethodMI *(*GENERIC_MethodMI) (CMPIBroker * broker,
                                           CMPIContext * ctelsex,
                                           const char * provider,
					   CMPIStatus * status);
typedef CMPIMethodMI *(*FIXED_MethodMI) (CMPIBroker * broker,
                                         CMPIContext * ctx,
					 CMPIStatus * status);


CMPIMethodMI *loadMethodMI(const char *provider, void *library,
                                 CMPIBroker * broker, CMPIContext * ctx)
{
   CMPIMethodMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadMethodMI");
   
   GENERIC_MethodMI g =
       (GENERIC_MethodMI) getGenericEntryPoint(library, "Method");
   if (g == NULL) {
      FIXED_MethodMI f =
          (FIXED_MethodMI) getFixedEntryPoint(provider, library,"Method");
      if (f == NULL) _SFCB_RETURN(NULL);
      if (broker) {
	mi=(f)(broker, ctx, &status);
	if (mi && status.rc == CMPI_RC_OK)
	  _SFCB_RETURN(mi);
      }   
      _SFCB_RETURN((CMPIMethodMI*)(void*)-1);
   }
   if (broker)  {
     mi=(g)(broker,ctx, provider, &status);
     if (mi && status.rc == CMPI_RC_OK)
       _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIMethodMI*)(void*)-1);
}


typedef CMPIPropertyMI *(*GENERIC_PropertyMI) (CMPIBroker * broker,
                                               CMPIContext * ctx,
                                               const char * provider,
					       CMPIStatus * status);
typedef CMPIPropertyMI *(*FIXED_PropertyMI) (CMPIBroker * broker,
                                             CMPIContext * ctx,
					     CMPIStatus * status);

CMPIPropertyMI *loadPropertyMI(const char *provider, void *library,
                                     CMPIBroker * broker, CMPIContext * ctx)
{
   CMPIPropertyMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadPropertyMI");

   GENERIC_PropertyMI g =
       (GENERIC_PropertyMI) getGenericEntryPoint(library,"Property");
   if (g == NULL) {
      FIXED_PropertyMI f =
          (FIXED_PropertyMI) getFixedEntryPoint(provider, library,
                                                     "Property");
      if (f == NULL) _SFCB_RETURN(NULL);
      if (broker) {
	mi=(f)(broker, ctx, &status);
	if (mi && status.rc == CMPI_RC_OK)
	  _SFCB_RETURN(mi);
      }   
      _SFCB_RETURN((CMPIPropertyMI*)(void*)-1);
   }

   if (broker)  {
     mi=(g)(broker,ctx, provider, &status);
     if (mi && status.rc == CMPI_RC_OK)
       _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIPropertyMI*)(void*)-1);
};


typedef CMPIIndicationMI *(*GENERIC_IndicationMI) (CMPIBroker * broker,
                                                   CMPIContext * ctx,
                                                   const char * provider,
						   CMPIStatus * status);
typedef CMPIIndicationMI *(*FIXED_IndicationMI) (CMPIBroker * broker,
                                                 CMPIContext * ctx,
						 CMPIStatus * status);


CMPIIndicationMI *loadIndicationMI(const char *provider,
                                         void *library,
                                         CMPIBroker * broker, CMPIContext * ctx)
{
   CMPIIndicationMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadIndicationMI");

   GENERIC_IndicationMI g =
       (GENERIC_IndicationMI) getGenericEntryPoint(library,
                                                        "Indication");
   if (g == NULL) {
      FIXED_IndicationMI f =
          (FIXED_IndicationMI) getFixedEntryPoint(provider, library,
                                                       "Indication");
      if (f == NULL) _SFCB_RETURN(NULL);
      if (broker) {
	mi=(f)(broker, ctx, &status);
	if (mi && status.rc == CMPI_RC_OK)
	  _SFCB_RETURN(mi);
      }   
      _SFCB_RETURN((CMPIIndicationMI*)(void*)-1);
   }

   if (broker)  {
     mi=(g)(broker,ctx, provider, &status);
     if (mi && status.rc == CMPI_RC_OK)
       _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIIndicationMI*)(void*)-1);
};



typedef CMPIClassMI *(*FIXED_ClassMI) (CMPIBroker * broker,
				       CMPIContext * ctx,
				       CMPIStatus * status);

CMPIClassMI *loadClassMI(const char *provider,
                                         void *library,
                                         CMPIBroker * broker, CMPIContext * ctx)
{
   CMPIClassMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadClassMI");
   FIXED_ClassMI f =
          (FIXED_ClassMI) getFixedEntryPoint(provider, library,
                                                       "Class");
   if (f == NULL) _SFCB_RETURN(NULL);
   
   if (broker)  {
     mi=(f)(broker,ctx, &status);
     if (mi && status.rc == CMPI_RC_OK)
       _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIClassMI*)(void*)-1);
};

typedef CMPIQualifierDeclMI *(*FIXED_QualifierDeclMI) (CMPIBroker * broker,
						       CMPIContext * ctx,
						       CMPIStatus * status);

CMPIQualifierDeclMI *loadQualifierDeclMI(const char *provider,
                                         void *library,
                                         CMPIBroker * broker, CMPIContext * ctx)
{
   CMPIQualifierDeclMI *mi; 
   CMPIStatus status = {CMPI_RC_OK, NULL};

   _SFCB_ENTER(TRACE_PROVIDERDRV, "loadQualifierDeclMI");
   FIXED_QualifierDeclMI f =
          (FIXED_QualifierDeclMI) getFixedEntryPoint(provider, library,
                                                       "QualifierDecl");
   if (f == NULL) _SFCB_RETURN(NULL);
   
   if (broker)  {
     mi=(f)(broker,ctx, &status);
     if (mi && status.rc == CMPI_RC_OK)
       _SFCB_RETURN(mi);
   }
   _SFCB_RETURN((CMPIQualifierDeclMI*)(void*)-1);
};

/****************************************************************************/



/** Exits the program with a memory allocation error message in case the given 
 *  condition holds.
 */
#define __ALLOC_ERROR(cond) \
  if ( cond ) { \
    error_at_line ( -1, errno, __FILE__, __LINE__, \
		    "unable to allocate requested memory." ); \
  }


/**
 * flag to ensure MM is initialized only once
 */
static int __once = 0;

/**
 * global key to get access to thread-specific memory management data
 */
static CMPI_THREAD_KEY_TYPE __mm_key;

void tool_mm_set_broker(void *broker, void *ctx);
void *tool_mm_get_broker(void **ctx);
void *memAddEncObj(int mode, void *ptr, size_t size, int *memId);
int memAdd(void *ptr, int *memId);



void *tool_mm_load_lib(const char *libname)
{
   char filename[255];
   sprintf(filename, "lib%s.so", libname);
   return dlopen(filename, RTLD_LAZY);
}

static void __flush_mt(managed_thread * mt)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "__flush_mt");

   while (mt->hc.memUsed) {
      --mt->hc.memUsed;
      if (mt->hc.memObjs[mt->hc.memUsed]) free(mt->hc.memObjs[mt->hc.memUsed]);
      mt->hc.memObjs[mt->hc.memUsed] = NULL;
   };
 
   while (mt->hc.memEncUsed) {
      --mt->hc.memEncUsed;
      _SFCB_TRACE(1,("memEnc %d %d %p\n", currentProc,mt->hc.memEncUsed,mt->hc.memEncObjs[mt->hc.memEncUsed]))
      if (mt->hc.memEncObjs[mt->hc.memEncUsed]) {
	if (mt->hc.memEncObjs[mt->hc.memEncUsed]->ft == NULL) {
	  fprintf(stderr,"***** NULL ft in enc obj record *****\n");
	  abort();
	}
         mt->hc.memEncObjs[mt->hc.memEncUsed]->ft->release(mt->hc.memEncObjs[mt->hc.memEncUsed]);
      }   
      mt->hc.memEncObjs[mt->hc.memEncUsed] = NULL;
   };
   _SFCB_EXIT();
}

/**
 * Cleans up a previously initialized thread once it dies/exits.
 */
 
static void __cleanup_mt(void *ptr)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "__cleanup_mt");
   managed_thread *mt = (managed_thread *) ptr;
   
   //   fprintf(stderr,"---- %d/%d cleanup_mt %x, %x\n", getpid(), pthread_self(), mt , (mt?mt->hc.memEncObjs:NULL));
   
   if (mt && mt->cleanupDone == 0) {
     mt->cleanupDone = 1;
     __flush_mt(mt);
     
     if (mt->hc.memObjs) free(mt->hc.memObjs);
     if (mt->hc.memEncObjs) free(mt->hc.memEncObjs);
     if (mt) free(mt);
   }
   _SFCB_EXIT();
}

static managed_thread *__memInit(int dontforce);

/**
 * Initializes the current thread by adding it to the memory management sytem.
 */

void uninitGarbageCollector()
{

   /* do an init (don't force) just in case; otherwise, we risk using garbage */
   managed_thread *mt =  __memInit(1);

   if (mt==NULL) return;
   
   __cleanup_mt(mt);
   CMPI_BrokerExt_Ftab->setThreadSpecific(__mm_key,NULL);
   CMPI_BrokerExt_Ftab->destroyThreadKey(__mm_key);
}

static managed_thread *__init_mt()
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "managed_thread");
   managed_thread *mt = (managed_thread *) calloc(1, sizeof(managed_thread)+8);

   //   fprintf(stderr,"---- %d/%d init_mt \n", getpid(), pthread_self());

   __ALLOC_ERROR(!mt);

   mt->hc.memEncUsed = mt->hc.memUsed = 0;
   mt->hc.memEncSize = mt->hc.memSize = MT_SIZE_STEP;
   mt->hc.memObjs = (void **) malloc(MT_SIZE_STEP * sizeof(void *));
   mt->hc.memEncObjs = (Object **) malloc(MT_SIZE_STEP * sizeof(void *));
   
   mt->data = NULL;

   CMPI_BrokerExt_Ftab->setThreadSpecific(__mm_key, mt);

   _SFCB_RETURN(mt);
}

/**
 * Initializes the memory mangement system.
 */
 
static void __init_mm()
{
  //   fprintf(stderr,"---- %d/%d initmm\n", getpid(), pthread_self());
   _SFCB_ENTER(TRACE_MEMORYMGR, "__init_mm");
   CMPI_BrokerExt_Ftab->createThreadKey(&__mm_key, __cleanup_mt);
   _SFCB_EXIT();
}

/** 
 * dontforce = 1: don't create new thread memory if none found
 */
static managed_thread *__memInit(int dontforce)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "__memInit");
   managed_thread *mt;

   CMPI_BrokerExt_Ftab->threadOnce(&__once, __init_mm);

   mt = (managed_thread *)
       CMPI_BrokerExt_Ftab->getThreadSpecific(__mm_key);
   if (mt==NULL && dontforce==0) mt=__init_mt();
   return mt;
}


/**
 * Allocates zeroed memory and eventually puts it under memory mangement.
 *
 * Description:
 *
 *   Calls calloc to get the requested block size, then adds it to
 *   the control system depending on add, defined as MEM_TRACKED and
 *   MEM_NOT_TRACKED.
 */

void *memAlloc(int add, size_t size, int *memId)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "mem_alloc");
   void *result = calloc(1, size);
   if (!result) {
      _SFCB_TRACE(1,("--- memAlloc %u %d\n", size, currentProc))
      abort();
   }
   __ALLOC_ERROR(!result);

   if (add != MEM_TRACKED) {
      memAdd(result,memId);
   }
   _SFCB_TRACE(1, ("--- Area: %p size: %d", result, size));
   _SFCB_RETURN(result);
}



/**
 * Adds ptr to the list of managed objects for the current thread.
 *
 * Description:
 *
 *   First checks if the current thread is already under memory management
 *   control, eventually adds it. Then checks if ptr is already stored, if
 *   not finally adds it. Additionally the array size for stored void
 *   pointers may have to be enlarged by MT_SIZE_STEP.
 */

int memAdd(void *ptr, int *memId)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "memAdd");
   if (localClientMode) return(1);
   managed_thread *mt=__memInit(0);

   mt->hc.memObjs[mt->hc.memUsed++] = ptr;
   *memId=mt->hc.memUsed;

   if (mt->hc.memUsed == mt->hc.memSize) {
      mt->hc.memSize += MT_SIZE_STEP;
      mt->hc.memObjs = (void **) realloc(mt->hc.memObjs, mt->hc.memSize * sizeof(void *));
      __ALLOC_ERROR(!mt->hc.memObjs);
   }

   _SFCB_RETURN(1);
}

void *memAddEncObj(int mode, void *ptr, size_t size, int *memId)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "memAddEncObj");

   void *object = malloc(size);
   memcpy(object, ptr, size);

   if (localClientMode || mode != MEM_TRACKED) {
      *memId=MEM_NOT_TRACKED;
      _SFCB_RETURN(object);
   }   
   
   managed_thread *mt=__memInit(0);

   mt->hc.memEncObjs[mt->hc.memEncUsed++] = (Object *) object;
   *memId=mt->hc.memEncUsed;

   if (mt->hc.memEncObjs[*memId - 1]->ft == NULL) {
     abort();
   }
   
   if (mt->hc.memEncUsed == mt->hc.memEncSize) {
      mt->hc.memEncSize += MT_SIZE_STEP;
      mt->hc.memEncObjs =
          (Object **) realloc(mt->hc.memEncObjs, mt->hc.memEncSize * sizeof(void *));
      __ALLOC_ERROR(!mt->hc.memEncObjs);
   }

   _SFCB_RETURN(object); 
}

void memLinkEncObj(void *object, int *memId)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "memLinkEncObj");
   if (localClientMode) return;
   managed_thread *mt=__memInit(0);

   mt->hc.memEncObjs[mt->hc.memEncUsed++] = (Object *) object;
   *memId=mt->hc.memEncUsed;
   
   if (mt->hc.memEncUsed == mt->hc.memEncSize) {
      mt->hc.memEncSize += MT_SIZE_STEP;
      mt->hc.memEncObjs =
          (Object **) realloc(mt->hc.memEncObjs, mt->hc.memEncSize * sizeof(void *));
      __ALLOC_ERROR(!mt->hc.memEncObjs);
   }

   _SFCB_EXIT(); 
}

void memUnlinkEncObj(int memId)
{
   if (localClientMode) return;
   managed_thread *mt=__memInit(1); /* if none found we shouldn't delete */
   
   if (mt && memId!=MEM_RELEASED && memId!=MEM_NOT_TRACKED)
      mt->hc.memEncObjs[memId-1] = NULL;
}




void tool_mm_flush()
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "tool_mm_flush");
   managed_thread *mt;

   CMPI_BrokerExt_Ftab->threadOnce(&__once, __init_mm);

   mt = (managed_thread *)
       CMPI_BrokerExt_Ftab->getThreadSpecific(__mm_key);

   if (mt != NULL) {
      __flush_mt(mt);
   }
   _SFCB_EXIT();
}




void tool_mm_set_broker(void *broker, void *ctx)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "tool_mm_set_broker");
   managed_thread *mt=__memInit(0);

   mt->broker = broker;
   mt->ctx = ctx;
   _SFCB_EXIT();
}

void *tool_mm_get_broker(void **ctx)
{
   _SFCB_ENTER(TRACE_MEMORYMGR, "tool_mm_get_broker");
   managed_thread *mt=__memInit(0);

   if (ctx)
      *ctx = mt->ctx;
   _SFCB_RETURN(mt->broker);
}


void *getThreadDataSlot()
{
   managed_thread *mt=__memInit(0);
   return &mt->data;
}

void *markHeap()
{
   managed_thread *mt;
   if (localClientMode) return NULL;
   
   HeapControl *hc=(HeapControl*)calloc(1,sizeof(HeapControl)+8);
   _SFCB_ENTER(TRACE_MEMORYMGR, "markHeap");
   
   mt=__memInit(0);
   
   memcpy(hc,&mt->hc,sizeof(HeapControl));
   
   mt->hc.memEncUsed = mt->hc.memUsed = 0;
   mt->hc.memEncSize = mt->hc.memSize = MT_SIZE_STEP;
   mt->hc.memObjs = (void **) malloc(MT_SIZE_STEP * sizeof(void *));
   mt->hc.memEncObjs = (Object **) malloc(MT_SIZE_STEP * sizeof(void *));
   
   _SFCB_RETURN(hc);
}

void releaseHeap(void *hc)
{
   managed_thread *mt;
   if (localClientMode) return;
   
   mt = (managed_thread *) CMPI_BrokerExt_Ftab->getThreadSpecific(__mm_key);
   _SFCB_ENTER(TRACE_MEMORYMGR, "releaseHeap");
   
   mt=__memInit(0);
   
   __flush_mt(mt);
   
   if (mt->hc.memObjs) free(mt->hc.memObjs);
   if (mt->hc.memEncObjs) free(mt->hc.memEncObjs);
   
   memcpy(&mt->hc,hc,sizeof(HeapControl));
   
   free(hc);
   _SFCB_EXIT();
}



#include "utilft.h"
#include "providerRegister.h"

ProviderRegister *pReg = NULL;

int init_sfcBroker(char *home)
{
   pReg = newProviderRegister(home);
   return 0;
}


int uninit_sfcBroker()
{
   if (pReg) pReg->ft->release(pReg);
   return 0;
}

//*********************************************************************
//*
//*     Copyright (c) 1999, Bob Withers - bwit@pobox.com
//*
//* This code may be freely used for any purpose, either personal
//* or commercial, provided the authors copyright notice remains
//* intact.
//*********************************************************************

static char cvt[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz0123456789+/";

char *encode64(char *data)
{
   int i, o = 0;
   char c;
   int len = strlen(data);
   char *ret = (char *) malloc(len * 2);

   for (i = 0; i < len; ++i) {
      c = (data[i] >> 2) & 0x3f;
      ret[o++] = cvt[(int) c];
      c = (data[i] << 4) & 0x3f;
      if (++i < len)
         c |= (data[i] >> 4) & 0x0f;

      ret[o++] = cvt[(int) c];
      if (i < len) {
         c = (data[i] << 2) & 0x3f;
         if (++i < len)
            c |= (data[i] >> 6) & 0x03;

         ret[o++] = cvt[(int) c];
      }
      else {
         ++i;
         ret[o++] = '=';
      }

      if (i < len) {
         c = data[i] & 0x3f;
         ret[o++] = cvt[(int) c];
      }
      else
         ret[o++] = '=';
   }
   ret[o] = 0;
   return (ret);
}

static int find(char *str, char c)
{
   char *p = strchr(str, c);
   return p - str;
}

char *decode64(char *din)
{
   unsigned char *data=(unsigned char*)din;
   int i, o = 0, len = strlen((char*)data);
   unsigned char c, c1;
   unsigned char *ret = (unsigned char *) malloc(len * 2);

 for (i = 0; i < len; ++i) {
      c = (char) find(cvt, data[i]);
      ++i;
      c1 = (char) find(cvt, data[i]);
      c = (c << 2) | ((c1 >> 4) & 0x3);
      ret[o++] = c;
      if (++i < len) {
         c = data[i];
         if ('=' == c)
            break;
         c = (char) find(cvt, c);
         c1 = ((c1 << 4) & 0xf0) | ((c >> 2) & 0xf);
         ret[o++] = c1;
      }

      if (++i < len) {
         c1 = data[i];
         if ('=' == c1)
            break;
         c1 = (char) find(cvt, c1);
         c = ((c << 6) & 0xc0) | c1;
         ret[o++] = c;
      }
   }

   ret[o] = 0;
   return ((char*)ret);
}

void dump(char *msg, void *a, int len)
{
   unsigned char *b = (unsigned char *) a, *bb;
   int i, j, k, l;
   printf("(%p-%d) %s\n", a, len, msg);
   static char ht[] = "0123456789ABCDEF";

   for (bb = b, k = 0, j = 1, i = 0; i < len; i++, j++) {
      if (j == 1 && k == 0)
         printf("%p: ", b + i);
      printf("%c%c", ht[b[i] >> 4], ht[b[i] & 15]);
      if (j == 4) {
         j = 0;
         printf(" ");
         k++;
      }
      if (k == 8) {
         printf(" *");
         for (l = 0; l < 32; l++) {
            if (bb[l] >= ' ' && bb[l] <= 'z')
               printf("%c", bb[l]);
            else
               printf(".");
         }
         bb = &bb[32];
         k = 0;
         printf("*\n");
      }
   }
   printf("\n");
}


/* --------------------------------------------
 * ------
 * --    Sfcb control statement scanner support
 * ------
 * --------------------------------------------
 */


void cntlSkipws(char **p)
{
   while (**p && **p <= ' ' && **p != '\n')
      (*p)++;
}

int cntlParseStmt(char *in, CntlVals * rv)
{
   rv->type = 0;
   cntlSkipws(&in);
   if (*in == 0 || *in == '#' || *in == '\n') {
      rv->type = 3;
   }
   else if (*in == '[') {
      char *p = strpbrk(in + 1, "] \t\n");
      if (*p == ']') {
         rv->type = 1;
         *p = 0;
         rv->id = in + 1;
      }
   }
   else {
      char *p = strpbrk(in, ": \t\n");
      if (*p == ':') {
         rv->type = 2;
         *p = 0;
         rv->id = in;
         in = ++p;
         cntlSkipws(&in);
         rv->val = in;
      }
   }
   return rv->type;
}

char *cntlGetVal(CntlVals * rv)
{
   char *p, *v;
   if (rv->val == NULL)
      return NULL;
   cntlSkipws(&rv->val);
   v = rv->val;
   p = strpbrk(rv->val, " \t\n");
   if (p) {
      if (*p != '\n')
         rv->val = p + 1;
      else
         rv->val = NULL;
      *p = 0;
   }
   else rv->val = NULL;
   return v;
}

char *cntlGetStr(CntlVals * rv)
{
   char *v;
   if (rv->val == NULL) {
      return NULL;
   }
   cntlSkipws(&rv->val);
   v = rv->val;
   return v;
}

void dumpTiming(int pid)
{
   char buffer[4096];
   FILE *f;
   int l;
   
   if (collectStat==0) return;
   
   sprintf(buffer,"/proc/%d/stat",pid);
   f=fopen(buffer,"r");
   l=fread(buffer,1,4095,f);
   fclose(f);   
   buffer[l]=0;
   f=fopen("sfcbStat","a");
   fprintf(f,"%s %s\n",processName,buffer);
   fclose(f);
}

void setStatus(CMPIStatus *st, CMPIrc rc, char *msg)
{
   st->rc=rc;
   if (rc!=0 && msg) st->msg=sfcb_native_new_CMPIString(msg,NULL,0);
   else st->msg=NULL;
}   

void showStatus(CMPIStatus *st, char *msg)
{
   char *m=NULL;
   if (st->msg) m=(char*)st->msg->hdl;
   mlogf(M_INFO,M_SHOW,"--- showStatus (%s): %d %s\n",msg,st->rc,m);
}   

double timevalDiff(struct timeval *sv, struct timeval *ev)
{
   double s,e;

   s=sv->tv_usec;
   s/=1000000;
   s+=sv->tv_sec;
   e=ev->tv_usec;
   e/=1000000;
   e+=ev->tv_sec;
   return e-s;
}
   
