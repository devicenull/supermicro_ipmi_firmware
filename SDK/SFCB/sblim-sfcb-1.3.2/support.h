
/*
 * $Id: support.h,v 1.14 2007/04/27 09:11:07 sschuetz Exp $
 *
 * support.h
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
 * Description: Various memory management and provider support routines.
 *
 */

/** @file support.h
 *  @brief Memory managment system and support routines for providers.
 *
 *  @author Frank Scheffler
 *
 *  @sa support.c
 *  @sa native.h
 */

#ifndef CMPI_TOOL_H
#define CMPI_TOOL_H

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpiftx.h"
#include "cmpimacs.h"
#include <time.h>
#include <sys/time.h>
#include <dlfcn.h>

#define ENQ_BOT_LIST(i,f,l,n,p) { if (l) l->n=i; else f=i; \
                                  i->p=l; i->n=NULL; l=i;}
#define ENQ_TOP_LIST(i,f,l,n,p) { if (f) f->p=i; else l=i; \
                                   i->p=NULL; i->n=f; f=i;}
#define DEQ_FROM_LIST(i,f,l,n,p) \
                    { if (i->n) i->n->p=i->p; else l=i->p; \
                      if (i->p) i->p->n=i->n; else f=i->n;}

CMPIClassMI *loadClassMI(const char *provider,
			 void *library,
			 CMPIBroker * broker,
			 CMPIContext * ctx);

CMPIMethodMI *loadMethodMI(const char *provider,
			   void *library,
			   CMPIBroker * broker, 
			   CMPIContext * ctx);

CMPIPropertyMI *loadPropertyMI(const char *provider,
			       void *library,
			       CMPIBroker * broker, 
			       CMPIContext * ctx);

CMPIInstanceMI *loadInstanceMI(const char *provider,
			       void *library,
			       CMPIBroker * broker, 
			       CMPIContext * ctx);

CMPIIndicationMI *loadIndicationMI(const char *provider,
				   void *library,
				   CMPIBroker * broker,
				   CMPIContext * ctx);

CMPIAssociationMI *loadAssociationMI(const char *provider,
				     void *library,
				     CMPIBroker * broker,
				     CMPIContext * ctx);

CMPIQualifierDeclMI *loadQualifierDeclMI(const char *provider,
                                         void *library,
                                         CMPIBroker * broker,
                                         CMPIContext * ctx);                                         

/** @def MEM_NOT_TRACKED
 *
 *  Cloned object state in which memory is not tracked. 
 */

/** @def MEM_TRACKED
 *
 *  State in which memory is being tracked.
 */

/** @def MEM_RELEASED
 *
 *  State in which previously tracked memory has been released.
 */

/** @def MT_SIZE_STEP
 *
 *  The initial size of trackable memory pointers per thread. This size is
 *  incremented by the same amount once the limit is reached.
 */

/** @struct ObjectFT
 *  @brief Function table for generic object wrapper
 *
 *  This structure contains the function table for operating on generic objects. 
 */

/** @struct Object
 *  @brief Generic object structure
 *
 *  This structure is a generic wrapper for the underlying object implementation.
 */

/** @var typedef struct _managed_thread managed_thread
 *  @brief Per-Thread heap management structure.
 *
 *  @sa _managed_thread
 */

/** @struct _managed_thread
 *  @brief Per-Thread heap management structure.
 *
 *  This struct is used for managing the heap bound to the current thread.
 */

/** @var typedef struct heapControl HeapControl
 *  @brief Heap management structure.
 *
 *  @sa heapControl
 */

/** @struct heapControl
 *  @brief Heap management structure.
 *
 *  This struct is returned using a global pthread_key_t and stores all allocated
 *  objects that are going to be freed, once the thread is flushed or dies.
 */

#define MEM_NOT_TRACKED -2
#define MEM_TRACKED   1
#define MEM_RELEASED -1

#define MT_SIZE_STEP 100

typedef struct {
   int ftVersion;                    /**< function table version */
   CMPIStatus(*release) (void *obj); /**< function pointer to generic object release function 
				      *   @param obj generic object to be freed */
} ObjectFT;


typedef struct {
   void *hdl;                        /**< pointer to underlying object implementation */
   ObjectFT *ft;                     /**< pointer to generic object function table */
} Object;

typedef struct _managed_thread managed_thread;

typedef struct heapControl {
   unsigned memSize;                 /**< current maximum number of tracked object pointers */
   unsigned memUsed;                 /**< number of currently tracked object pointers */
   void **memObjs;                   /**< pointers to object allocations */
   unsigned memEncUsed;              /**< current maximum number of tracked encapsulated object pointers */
   unsigned memEncSize;              /**< number of currently tracked encapsulated object pointers */
   Object **memEncObjs;              /**< pointers to encapsulated object allocations */
} HeapControl;
 
struct _managed_thread {
   void *broker;
   void *ctx;                        /**< pointer to current thread context */
   void *data;
   HeapControl hc;                   /**< heap control structure for this thread */
   int   cleanupDone;                /**< cleanup state */
};



void *tool_mm_load_lib(const char *libname);

void tool_mm_flush();
void *tool_mm_alloc(int, size_t);
void *tool_mm_realloc(void *, size_t);
int tool_mm_add(void *);
void tool_mm_set_broker(void *, void *);
int tool_mm_remove(void *);
void *tool_mm_get_broker(void **);

int memAdd(void *ptr, int *memId);
void *memAlloc(int add, size_t size, int *memId);
void *memAddEncObj(int mode, void *ptr, size_t size, int *memId);
void memUnlinkEncObj(int memId);
void memLinkEncObj(void *ptr, int *memId);
void memLinkInstance(CMPIInstance *ci);

void * markHeap();
void releaseHeap(void * heap);


typedef struct cntlVals {
   int type;
   char *id;
   char *val;
} CntlVals;

void cntlSkipws(char **p);
int cntlParseStmt(char *in, CntlVals * rv);
char *cntlGetVal(CntlVals * rv);
char *cntlGetStr(CntlVals * rv);

int uninit_sfcBroker();
void uninitGarbageCollector();

extern double timevalDiff(struct timeval *sv, struct timeval *ev);

#define PADDING_LEN(s) ( (s)%sizeof(int) ? sizeof(int) - (s)%sizeof(int) : 0)
#define PADDED_LEN(s) ((s) + PADDING_LEN(s))

#endif
