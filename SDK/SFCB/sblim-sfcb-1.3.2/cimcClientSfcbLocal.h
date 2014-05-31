
/*
 * cmpic.h
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPI Local Client function tables.
 *
*/

#ifndef _CMPIC_H_
#define _CMPIC_H_

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include "constClass.h"

#ifdef __cplusplus
extern "C" {
#endif



   //---------------------------------------------------
   //--
   //	_ClientFt Function Table
   //--
   //---------------------------------------------------


   /** This structure is a table of pointers to client CIMOM services.
   */
#define native_new_CMPIObjectPath   newCMPIObjectPath

CMPIObjectPath * newCMPIObjectPath ( const char * my_nameSpace, 
					     const char * classname,
					     CMPIStatus * rc );

struct _ClientEnv;
typedef struct _ClientEnv ClientEnv;
   
struct _ClientEnc;
typedef struct _ClientEnc ClientEnc;

struct _ClientConnection;
typedef struct _ClientConnection ClientConnection;
   
typedef struct clientData {
   char *hostName;
   char *port;
   char *user;
   char *pwd;
   char *scheme;
   int  status;
} ClientData;
  
#define Client_VERIFY_NONE 0
#define Client_VERIFY_PEER 1

typedef struct credentialData {
  int    verifyMode;
  char * trustStore;
  char * certFile;
  char * keyFile;
} CredentialData;

typedef struct _Client {
   void *hdl;
   void *ft; //ClientFT *ft;
} Client;

typedef struct _ClientFT {
     int ftVersion;
     CMPIStatus (*release)
              (Client* cl);
     Client *(*clone) ( Client * cl, CMPIStatus * st );
     CMPIConstClass* (*getClass)
                (Client* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);
     CMPIEnumeration* (*enumClassNames)
                (Client* cl,
                 CMPIObjectPath* op, CMPIFlags flags, CMPIStatus* rc);
     CMPIEnumeration* (*enumClasses)
                (Client* cl,
                 CMPIObjectPath* op, CMPIFlags flags, CMPIStatus* rc);
     CMPIInstance* (*getInstance)
                (Client* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);
     CMPIObjectPath* (*createInstance)
                (Client* cl,
                 CMPIObjectPath* op, CMPIInstance* inst, CMPIStatus* rc);
     CMPIStatus (*setInstance)
                (Client* cl,
                 CMPIObjectPath* op, CMPIInstance* inst, CMPIFlags flags, char ** properties);
     CMPIStatus (*deleteInstance)
                (Client* cl,
                 CMPIObjectPath* op);
     CMPIEnumeration* (*execQuery)
                (Client* cl,
                 CMPIObjectPath* op, const char *query, const char *lang, CMPIStatus* rc);
     CMPIEnumeration* (*enumInstanceNames)
                (Client* cl,
                 CMPIObjectPath* op, CMPIStatus* rc);
     CMPIEnumeration* (*enumInstances)
                (Client* cl,
                 CMPIObjectPath* op, CMPIFlags flags, char** properties, CMPIStatus* rc);
     CMPIEnumeration* (*associators)
                (Client* cl,
                 CMPIObjectPath* op, const char *assocClass, const char *resultClass,
                 const char *role, const char *resultRole, CMPIFlags flags, 
                 char** properties, CMPIStatus* rc);
     CMPIEnumeration* (*associatorNames)
                (Client* cl,
                 CMPIObjectPath* op, const char *assocClass, const char *resultClass,
                 const char *role, const char *resultRole, CMPIStatus* rc);
     CMPIEnumeration* (*references)
                (Client* cl,
                 CMPIObjectPath* op, const char *resultClass ,const char *role ,
     CMPIFlags flags, char** properties, CMPIStatus* rc);
     CMPIEnumeration* (*referenceNames)
                (Client* cl,
                 CMPIObjectPath* op, const char *resultClass ,const char *role,
                 CMPIStatus* rc);
     CMPIData (*invokeMethod)
                (Client* cl,
                 CMPIObjectPath* op,const char *method,
     CMPIArgs* in, CMPIArgs* out, CMPIStatus* rc);
     CMPIStatus (*setProperty)
                (Client* cl,
                 CMPIObjectPath* op, const char *name , CMPIValue* value,
                 CMPIType type);
     CMPIData (*getProperty)
                (Client *cl, 
                 CMPIObjectPath *op, const char *name, CMPIStatus *rc);
} ClientFT;


typedef struct _ClientEnvFT {
   char *env;
   void* (*release)(ClientEnv *ce);
   Client* (*connect)
               (ClientEnv *ce, const char *hn, const char *scheme, const char *port, 
                const char *user, const char *pwd, CMPIStatus *rc);
   Client* (*connect2)
               (ClientEnv *ce, const char *hn, const char *scheme, const char *port, 
                const char *user, const char *pwd, 
                int verifyMode, const char * trustStore,
                const char * certFile, const char * keyFile,
                CMPIStatus *rc);
   CMPIInstance* (*newInstance)
               (ClientEnv *ce, const CMPIObjectPath* op, CMPIStatus* rc);
   CMPIObjectPath* (*newObjectPath)
               (ClientEnv *ce, const char *ns, const char *cn, CMPIStatus* rc);
   CMPIArgs* (*newArgs)
               (ClientEnv *ce, CMPIStatus* rc);
   CMPIString* (*newString)
               (ClientEnv *ce, const char *data, CMPIStatus* rc);
   CMPIArray* (*newArray)
               (ClientEnv *ce, CMPICount max, CMPIType type, CMPIStatus* rc);
   CMPIDateTime* (*newDateTime)
               (ClientEnv *ce, CMPIStatus* rc);
   CMPIDateTime* (*newDateTimeFromBinary)
               (ClientEnv *ce, CMPIUint64 binTime, CMPIBoolean interval, CMPIStatus* rc);
   CMPIDateTime* (*newDateTimeFromChars)
               (ClientEnv *ce, const char *utcTime, CMPIStatus* rc);
} ClientEnvFT;

struct _ClientEnv {
   void *hdl;
   ClientEnvFT *ft;
};

int sameCMPIObjectPath ( const CMPIObjectPath *cop1, const CMPIObjectPath *cop2);
   
#ifdef __cplusplus
 };
#endif

#endif // _CMPIC_H_
