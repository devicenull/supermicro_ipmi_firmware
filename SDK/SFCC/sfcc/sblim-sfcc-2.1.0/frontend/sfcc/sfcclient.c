
/*
 * sfcclient.c
 *
 * © Copyright IBM Corp. 2007
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
 * Sfcc Client function tables.
 *
 */

#include "cimc.h"
#include "cmci.h"
#include "native.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* 
 * sfcc legacy entry points
 * this is required while sfcc clients haven't
 * switched to the new canonical entry points.
 */

static struct _ConnectionControl {
  pthread_mutex_t  ccMux;
  unsigned         ccCount;
  CIMCEnv         *ccEnv;
  CMPIStatus     (*ccRelease)(CMCIClient *cc);
} ConnectionControl = {
  PTHREAD_MUTEX_INITIALIZER, 0, NULL, NULL
};

static char *DefaultClient = "XML";

static CMPIStatus cmciRelease(CMCIClient* cc)
{
  CMPIStatus rc = {CMPI_RC_OK, NULL};
  pthread_mutex_lock(&ConnectionControl.ccMux);
  if (cc) {
    ConnectionControl.ccRelease(cc);
  }
  if (ConnectionControl.ccCount > 0) {
    if (--ConnectionControl.ccCount == 0) {
      /* restore client release function */
      //      cc->ft->release = ConnectionControl.ccRelease;
      ReleaseCIMCEnv(ConnectionControl.ccEnv);
      ConnectionControl.ccEnv = NULL;
      ConnectionControl.ccRelease = NULL;
    }
  }
  pthread_mutex_unlock(&ConnectionControl.ccMux);
  return rc;
}

CMCIClient *cmciConnect(const char *hn, const char *scheme, const char *port,
                        const char *user, const char *pwd, CMPIStatus *rc)
{
  return cmciConnect2(hn,scheme,port,user,pwd,0,NULL,NULL,NULL,rc);
}

CMCIClient *cmciConnect2(const char *hn, const char *scheme, const char *port,
			 const char *user, const char *pwd, 
			 int verifyMode, const char * trustStore,
			 const char * certFile, const char * keyFile,
			 CMPIStatus *rc)
{  
  CMCIClient     *cc = NULL;
  char           *msg;
  int             retc = 0;
  char           *client;

  pthread_mutex_lock(&ConnectionControl.ccMux);
  if (ConnectionControl.ccCount == 0) {
//printf("ConnectionControl.ccCount = 0\n");
    client = getenv("SFCC_CLIENT");
//printf("getenv(\"SFCC_CLIENT\") = %s\n", client);
    if (client == NULL) {
      if (scheme == NULL || strncasecmp(scheme,"http",4) == 0) {
	client = DefaultClient;
      } else {
	client = (char *) scheme;
      }
    }
//printf("client = %s\n", client);
    ConnectionControl.ccEnv = NewCIMCEnv(client,0,&retc, &msg);
  }
  if (retc || ConnectionControl.ccEnv == NULL) {
printf("retc || ConnectionControl.ccEnv == NULL\n");
    if(rc) {
      rc->rc=CMPI_RC_ERR_FAILED;
      rc->msg=NULL;
    }
    cc=NULL;
  } else {
//printf("retc || ConnectionControl.ccEnv != NULL\n");
    ConnectionControl.ccCount += 1;
    cc = (CMCIClient*)ConnectionControl.ccEnv->ft->connect2(ConnectionControl.ccEnv,hn,scheme,port,user,pwd,verifyMode,trustStore,certFile,keyFile,(CIMCStatus*)rc);
    if (cc) {
      if (ConnectionControl.ccRelease == NULL) {
	ConnectionControl.ccRelease = cc->ft->release;
      }
      cc->ft->release=cmciRelease;
    }
  }
  pthread_mutex_unlock(&ConnectionControl.ccMux);
  return cc;
}

CMPIObjectPath *newCMPIObjectPath( const char * namespace, 
				   const char * classname,
				   CMPIStatus * rc )
{
  return (CMPIObjectPath*)ConnectionControl.ccEnv->ft->
    newObjectPath(ConnectionControl.ccEnv,namespace,classname,
		  (CIMCStatus*)rc);
}

CMPIInstance *native_new_CMPIInstance( CMPIObjectPath *op,
					CMPIStatus * rc )
{
  return (CMPIInstance*)ConnectionControl.ccEnv->ft->
    newInstance(ConnectionControl.ccEnv,(CIMCObjectPath *)op,
		(CIMCStatus*)rc);
}

CMPIString *native_new_CMPIString( const char *ptr,
				   CMPIStatus * rc )
{
  return (CMPIString*)ConnectionControl.ccEnv->ft->
    newString(ConnectionControl.ccEnv, ptr, (CIMCStatus*)rc);
}

CMPIArgs *native_new_CMPIArgs( CMPIStatus * rc )
{
  return (CMPIArgs*)ConnectionControl.ccEnv->ft->
    newArgs(ConnectionControl.ccEnv,(CIMCStatus*)rc);
}

CMPIArray * native_new_CMPIArray ( CMPICount size,
				   CMPIType type,
				   CMPIStatus * rc)
{
  return (CMPIArray*)ConnectionControl.ccEnv->ft->
    newArray(ConnectionControl.ccEnv, size, type,(CIMCStatus*)rc);
}

CMPIDateTime * native_new_CMPIDateTime ( CMPIStatus * rc)
{
  return (CMPIDateTime*)ConnectionControl.ccEnv->ft->
    newDateTime(ConnectionControl.ccEnv,(CIMCStatus*)rc);
}

CMPIDateTime * native_new_CMPIDateTime_fromBinary ( CMPIUint64 date,
						    CMPIBoolean interval,
						    CMPIStatus * rc)
{
  return (CMPIDateTime*)ConnectionControl.ccEnv->ft->
    newDateTimeFromBinary(ConnectionControl.ccEnv, date, interval,
			  (CIMCStatus*)rc);
}

CMPIDateTime * native_new_CMPIDateTime_fromChars ( const char * date,
						   CMPIStatus * rc)
{
  return (CMPIDateTime*)ConnectionControl.ccEnv->ft->
    newDateTimeFromChars(ConnectionControl.ccEnv, date,
			 (CIMCStatus*)rc);
}

CMPIEnumeration * native_new_CMPIEnumeration ( CMPIArray * arr, 
					       CMPIStatus * rc)
{
  fprintf (stderr, "*** native_new_CMPIEnumeration not implemented *** ");
  return NULL;
}

CMPIConstClass * native_new_CMPIConstClass ( char *cn,
					     CMPIStatus * rc)
{
  fprintf (stderr, "*** native_new_CMPIConstClass not implemented *** ");
  return NULL;
}

void native_release_CMPIValue ( CMPIType type, CMPIValue * val )
{
  fprintf (stderr, "*** native_release_CMPIValue not implemented *** ");
}

void * newList ()
{
  fprintf (stderr, "*** newList not implemented *** ");
  return NULL;
}

void * newStringBuffer (int s)
{
  fprintf (stderr, "*** newStringBuffer not implemented *** ");
  return NULL;
}

char * value2Chars (CMPIType type, CMPIValue *value)
{
   char str[2048], *p;
   unsigned int size;
   CMPIString *cStr;

   str[0]=0;
   if (type & CMPI_ARRAY) {
   }
   else if (type & CMPI_ENC) {

      switch (type) {
      case CMPI_instance:
         break;

      case CMPI_ref:
	if (value->ref) {
	  cStr=value->ref->ft->toString(value->ref,NULL);
	  p = strdup((char *) cStr->hdl);
	  CMRelease(cStr);
	} else {
	  p = strdup("NULL");
	}
 
	return p;

      case CMPI_args:
         break;

      case CMPI_filter:
         break;

      case CMPI_string:
      case CMPI_numericString:
      case CMPI_booleanString:
      case CMPI_dateTimeString:
      case CMPI_classNameString:
         return strdup(value->string ? (char*)value->string->hdl : "NULL");

      case CMPI_dateTime:
         if (value->dateTime) {
         cStr=CMGetStringFormat(value->dateTime,NULL);
         p = strdup((char *) cStr->hdl);
         CMRelease(cStr);
         } else
             p = strdup("NULL");
	 return p;
      }

   }
   else if (type & CMPI_SIMPLE) {

      switch (type) {
      case CMPI_boolean:
         return strdup(value->boolean ? "true" : "false");

      case CMPI_char16:
         break;
      }

   }
   else if (type & CMPI_INTEGER) {

      switch (type) {
      case CMPI_uint8:
         sprintf(str, "%u", value->uint8);
         return strdup(str);
      case CMPI_sint8:
         sprintf(str, "%d", value->sint8);
         return strdup(str);
      case CMPI_uint16:
         sprintf(str, "%u", value->uint16);
         return strdup(str);
      case CMPI_sint16:
         sprintf(str, "%d", value->sint16);
         return strdup(str);
      case CMPI_uint32:
         sprintf(str, "%lu", value->uint32);
         return strdup(str);
      case CMPI_sint32:
         sprintf(str, "%ld", value->sint32);
         return strdup(str);
      case CMPI_uint64:
         sprintf(str, "%llu", value->uint64);
         return strdup(str);
      case CMPI_sint64:
         sprintf(str, "%lld", value->sint64);
         return strdup(str);
      }

   }
   else if (type & CMPI_REAL) {

      switch (type) {
      case CMPI_real32:
         sprintf(str, "%g", value->real32);
         return strdup(str);
      case CMPI_real64:
         sprintf(str, "%g", value->real64);
         return strdup(str);
      }

   }
   return strdup(str);
}
