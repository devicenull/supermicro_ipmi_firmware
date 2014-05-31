
/*
 * native.h
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
 * Header file for the encapsulated CMPI data type implementation.
 *
*/

#ifndef CMPI_NATIVE_DATA_H
#define CMPI_NATIVE_DATA_H

#define NATIVE_FT_VERSION 1

//#define CMPI_VERSION 90

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "cmpidt.h"
#include "cmpift.h"
#include "constClass.h"
#include "cmpimacs.h"
#include "msgqueue.h"

#include "support.h"


typedef struct _nativeEncObject {
   void *hdl;
   void *ft;
   int refCount;
} nativeEncObject;

struct _NativeCMPIBrokerFT {
   CMPIBrokerFT brokerFt;
   CMPIArray *(*getKeyNames) (const CMPIBroker * broker,
                              CMPIContext * context, CMPIObjectPath * cop,
                              CMPIStatus * rc);
   CMPIString *(*getMessage) (const CMPIBroker * mb, const char *msgId,
                              const char *defMsg, CMPIStatus * rc,
                              unsigned int count, va_list);
    CMPIBoolean(*classPathIsA) (const CMPIBroker * broker, CMPIObjectPath * cop,
                                const char *type, CMPIStatus * rc);
};

typedef struct _NativeCMPIBrokerFT NativeCMPIBrokerFT;


/****************************************************************************/

void sfcb_native_release_CMPIValue(CMPIType, CMPIValue * val);
CMPIValue sfcb_native_clone_CMPIValue(CMPIType, const CMPIValue * val, CMPIStatus *);

CMPIString *sfcb_native_new_CMPIString(const char *, CMPIStatus *, int reown);

CMPIArray *internal_new_CMPIArray(int mode, CMPICount size, CMPIType type,
                                  CMPIStatus *);
CMPIArray *NewCMPIArray(CMPICount size, CMPIType type, CMPIStatus *);
CMPIArray *TrackedCMPIArray(CMPICount size, CMPIType type, CMPIStatus *);

void sfcb_native_array_increase_size(const CMPIArray *, CMPICount);
CMPIResult *native_new_CMPIResult(int, int, CMPIStatus *);
CMPIArray *native_result2array(CMPIResult *);

CMPIEnumeration *sfcb_native_new_CMPIEnumeration(CMPIArray *, CMPIStatus *);

CMPIInstance *NewCMPIInstance(CMPIObjectPath *, CMPIStatus *);
CMPIInstance *TrackedCMPIInstance(const CMPIObjectPath *, CMPIStatus *);
CMPIInstance *internal_new_CMPIInstance(int mode, const CMPIObjectPath *,
                                        CMPIStatus *, int override);

CMPIObjectPath *NewCMPIObjectPath(const char *, const char *, CMPIStatus *);
CMPIObjectPath *TrackedCMPIObjectPath(const char *, const char *, CMPIStatus *);
CMPIObjectPath *interal_new_CMPIObjectPath(int mode, const char *, const char *,
                                           CMPIStatus *);

CMPIArgs *NewCMPIArgs(CMPIStatus *);
CMPIArgs *TrackedCMPIArgs(CMPIStatus *);

CMPIDateTime *sfcb_native_new_CMPIDateTime(CMPIStatus *);
CMPIDateTime *sfcb_native_new_CMPIDateTime_fromBinary(CMPIUint64,
                                                 CMPIBoolean, CMPIStatus *);
CMPIDateTime *sfcb_native_new_CMPIDateTime_fromChars(const char *, CMPIStatus *);
CMPISelectExp *native_new_CMPISelectExp(const char *,
                                        const char *,
                                        CMPIArray **, CMPIStatus *);
                                        
CMPIContext *native_new_CMPIContext(int mem_state, void*);
void native_release_CMPIContext(CMPIContext *);
CMPIContext *native_clone_CMPIContext(const CMPIContext *ctx);

extern CMPIBrokerExtFT *CMPI_BrokerExt_Ftab;

MsgSegment setObjectPathMsgSegment(const CMPIObjectPath * op);
CMPIInstance *relocateSerializedInstance(void *area);
void getSerializedInstance(const CMPIInstance * ci, void *area);
unsigned long getInstanceSerializedSize(const CMPIInstance * ci);
void getSerializedObjectPath(const CMPIObjectPath * op, void *area);
unsigned long getObjectPathSerializedSize(const CMPIObjectPath * op);
CMPIConstClass *getConstClass(const char *ns, const char *cn);

CMPIObjectPath *relocateSerializedObjectPath(void *area);
void dateTime2chars(CMPIDateTime * dt, CMPIStatus * rc, char *str_time);

void sfcb_setAlignedValue(CMPIValue * target, const CMPIValue *source, 
			  CMPIType type);


/****************************************************************************/

extern CMPIBrokerEncFT native_brokerEncFT;
extern CMPIBrokerEncFT *BrokerEncFT;
extern CMPIBrokerFT *RequestFT;

//struct native_propertyFT propertyFT;

#endif

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
