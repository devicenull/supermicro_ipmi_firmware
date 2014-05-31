
/*
 * cimXmlRequest.h
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
 * CMPI broker encapsulated functionality.
 *
 * CIM operations request handler .
 *
*/



#ifndef handleCimXmlRequest_h
#define handleCimXmlRequest_h

#include "msgqueue.h"
#include "providerMgr.h"

struct commHndl;
struct chunkFunctions;

typedef struct respSegment {
   int mode;
   char *txt;
} RespSegment;

typedef struct respSegments {
   void *buffer;
   int chunkedMode,rc;
   char *errMsg;
   RespSegment segments[7];
} RespSegments;

typedef struct expSegments {
   RespSegment segments[7];
} ExpSegments;

typedef struct cimXmlRequestContext {
   char *cimXmlDoc;
   char *principal;
   char *host;
   int  teTrailers;
   unsigned int sessionId;
   unsigned long cimXmlDocLength;
   struct commHndl *commHndl;
   struct chunkFunctions *chunkFncs;
   char *className;
   int operation;
} CimXmlRequestContext;

extern RespSegments handleCimXmlRequest(CimXmlRequestContext * ctx);
extern int cleanupCimXmlRequest(RespSegments * rs);


#endif
