
/*
 * providerRegister.h
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
 * Based on concepts developed by Viktor Mihajlovski <mihajlov@de.ibm.com>
 *
 * Description:
 *
 * Provider registration support.
 *
*/


#ifndef _ProviderRegister_h_
#define _ProviderRegister_h_

#include <stdio.h>
#include <sys/types.h>
#include "cmpift.h"
#include "cmpiftx.h"
#include "msgqueue.h"
#include "utilft.h"
#include "providerMgr.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

struct providerProcess;

typedef struct _ProviderInfo {
   char *className;
   unsigned long type;
   char *providerName;
   char *location;
   char *parms;
   char *group;
   char **ns;
   int id;
   pid_t pid;
   pthread_t tid;
   void *library;
   ComSockets providerSockets;
   ProvIds provIds;
   int unload,initialized;
   pthread_t idleThread;
   pthread_mutex_t initMtx;
   time_t lastActivity;
   int startSeq;
   struct _ProviderInfo *next;
   struct _ProviderInfo *nextInRegister; /*not actually next in Register,but
   pointer to the next provider serving the same class (for indications)*/
   struct providerProcess *proc;         
   struct providerThread *thread;
   CMPIInstanceMI *instanceMI;
   CMPIAssociationMI *associationMI;
   CMPIMethodMI *methodMI;
   CMPIIndicationMI *indicationMI;
   CMPIPropertyMI *propertyMI;
   CMPIClassMI *classMI;
   CMPIQualifierDeclMI *qualifierDeclMI;   
} ProviderInfo;

typedef struct providerProcess {
   char *group;
   int pid;
   int id;
   int unload;
   ProviderInfo *firstProv;
   ComSockets providerSockets;
   time_t lastActivity;
} ProviderProcess;

typedef struct providerThread {
   char *group;
   pthread_t tid;
   int id;
   int unload;
   ProviderInfo *firstProv;
   ComSockets providerSockets;
   time_t lastActivity;
} ProviderThread;

#define INSTANCE_PROVIDER       1
#define ASSOCIATION_PROVIDER    2
#define INDICATION_PROVIDER     4
#define METHOD_PROVIDER         8
#define PROPERTY_PROVIDER       16
#define CLASS_PROVIDER          32
#define QUALIFIER_PROVIDER      64
#define FORCE_PROVIDER_NOTFOUND 128

struct _Provider_Register_FT;
typedef struct _Provider_Register_FT Provider_Register_FT;

struct _ProviderRegister {
   void *hdl;
   Provider_Register_FT *ft;
};
typedef struct _ProviderRegister ProviderRegister;

typedef struct _ProviderBase {
   char *fn;
   UtilHashTable *ht;
} ProviderBase;

struct _Provider_Register_FT {
   int version;
   void (*release) (ProviderRegister * br);
   ProviderRegister *(*clone) (ProviderRegister * br);
   ProviderInfo *(*getProvider) (ProviderRegister * br,
                       const char *clsName, unsigned long type);
   int (*putProvider) (ProviderRegister * br, const char *clsName,
                       ProviderInfo * info);
   void (*removeProvider) (ProviderRegister * br, const char *clsName);
   ProviderInfo *(*locateProvider) (ProviderRegister * br,
                       const char *provName);
   void (*resetProvider) (ProviderRegister * br, int pid);
};

extern Provider_Register_FT *ProviderRegisterFT;
struct _Provider_Register_FT;

ProviderRegister *newProviderRegister(char *fn);

#ifdef __cplusplus
}
#endif
#endif                          // _ProviderRegister_h_

