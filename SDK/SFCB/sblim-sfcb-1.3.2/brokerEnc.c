
/*
 * brokerEnc.c
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
 * Author:       Frank Scheffler 
 * Contributors: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 * 
 * This module implements CMPI broker encapsulated functions.
 *
*/


#include "native.h"
#include "trace.h"
#include "constClass.h"
#include "utilft.h"

extern const char *opGetClassNameChars(const CMPIObjectPath * cop);
extern const char *opGetNameSpaceChars(const CMPIObjectPath * cop);
extern CMPIConstClass *getConstClass(const char *ns, const char *cn);
extern CMPIObjectPathFT *CMPI_ObjectPath_FT;
extern CMPIInstanceFT *CMPI_Instance_FT;
extern CMPIString *instance2String(CMPIInstance * inst, CMPIStatus * rc);
extern int verifyPropertyList(CMPIConstClass * cls, char **list);
extern CMPISelectExp *TrackedCMPISelectExp(const char *queryString,
          const char *language, CMPIArray ** projection, CMPIStatus * rc);



static CMPIInstance *__beft_newInstance(const CMPIBroker * broker,
                                        const CMPIObjectPath * cop, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newInstance");
   CMPIInstance *inst=TrackedCMPIInstance(cop, rc);
   _SFCB_RETURN(inst);
}


static CMPIObjectPath *__beft_newObjectPath(const CMPIBroker * broker,
                                            const char *namespace,
                                            const char *classname,
                                            CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newObjectPath");
   CMPIObjectPath *path=TrackedCMPIObjectPath(namespace, classname, rc);
   _SFCB_RETURN(path);
}


static CMPIArgs *__beft_newArgs(const CMPIBroker * broker, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newArgs");
   CMPIArgs *args=TrackedCMPIArgs(rc);
   _SFCB_RETURN(args);
}


static CMPIString *__beft_newString(const CMPIBroker * broker,
                                    const char *str, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newString");
   CMPIString *s=sfcb_native_new_CMPIString(str, rc, 0);
   _SFCB_RETURN(s);
}


static CMPIArray *__beft_newArray(const CMPIBroker * broker,
                                  CMPICount size,
                                  CMPIType type, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newArray");
   CMPIArray *ar=TrackedCMPIArray(size, type, rc);
   _SFCB_RETURN(ar);
}


static CMPIDateTime *__beft_newDateTime(const CMPIBroker * broker, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newDateTime");
   CMPIDateTime *dt=sfcb_native_new_CMPIDateTime(rc);
   _SFCB_RETURN(dt);
}


static CMPIDateTime *__beft_newDateTimeFromBinary(const CMPIBroker * broker,
                                                  CMPIUint64 time,
                                                  CMPIBoolean interval,
                                                  CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newDateTimeFromBinary");
   CMPIDateTime *dt=sfcb_native_new_CMPIDateTime_fromBinary(time, interval, rc);
   _SFCB_RETURN(dt);
}


static CMPIDateTime *__beft_newDateTimeFromChars(const CMPIBroker * broker,
                                                 const char *string, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newDateTimeFromChars");
    CMPIDateTime *dt=sfcb_native_new_CMPIDateTime_fromChars(string, rc);
   _SFCB_RETURN(dt);
}


static CMPISelectExp *__beft_newSelectExp(const CMPIBroker * broker,
                                          const char *queryString,
                                          const char *language,
                                          CMPIArray ** projection,
                                          CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"newSelectExp");
   CMPISelectExp *sx=TrackedCMPISelectExp(queryString, language, projection, rc);
   _SFCB_RETURN(sx);
}


static CMPIBoolean __beft_classPathIsA(const CMPIBroker * broker,
                                       const CMPIObjectPath * cop,
                                       const char *type, CMPIStatus * rc)
{
   CMPIConstClass *cc;
   char *scn, *ns;
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   CMPIString *clsn = CMGetClassName(cop, NULL);

   _SFCB_ENTER(TRACE_ENCCALLS,"classPathIsA");

   if (clsn && clsn->hdl && type) {
      if (strcasecmp(type, (char *) clsn->hdl) == 0)
         _SFCB_RETURN(1);
   }
   else _SFCB_RETURN(0);

   ns = (char *) opGetNameSpaceChars(cop);
   cc = (CMPIConstClass *) getConstClass(ns, opGetClassNameChars(cop));

   if (cc && type) for (; (scn = (char *) cc->ft->getCharSuperClassName(cc)) != NULL;) {
      if (strcasecmp(scn, type) == 0) return 1;
      cc = (CMPIConstClass *) getConstClass(ns, scn);
      if (cc == NULL) break;
   }
   _SFCB_RETURN(0);
}

static CMPIString *__beft_toString(const CMPIBroker * broker,
                                   const void *object, CMPIStatus * rc)
{
   CMPIString *str;
   _SFCB_ENTER(TRACE_ENCCALLS,"toString");
   if (object) {
      if (((CMPIInstance *) object)->ft) {
         if (((CMPIObjectPath *) object)->ft == CMPI_ObjectPath_FT) {
            //str=__oft_toString((CMPIObjectPath *) object, rc);
            str = ((CMPIObjectPath *)object)->ft->toString((CMPIObjectPath *) object, rc);
            _SFCB_RETURN(str);
         }   
         if (((CMPIInstance *) object)->ft == CMPI_Instance_FT) {
            str=instance2String((CMPIInstance *) object, rc);
            _SFCB_RETURN(str);
         }   
      }
   }
   _SFCB_TRACE(1,("This operation is not yet supported."));
   if (rc) CMSetStatus(rc, CMPI_RC_ERR_NOT_SUPPORTED);
   _SFCB_RETURN(NULL);
}


static CMPIBoolean __beft_isOfType(const CMPIBroker * broker,
                                   const void *object,
                                   const char *type, CMPIStatus * rc)
{
   char *t = *((char **) object);

   _SFCB_ENTER(TRACE_ENCCALLS,"isOfType");

   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   _SFCB_RETURN(strcmp(t, type) == 0);
}


static CMPIString *__beft_getType(const CMPIBroker * broker,
                                  const void *object, CMPIStatus * rc)
{
   _SFCB_ENTER(TRACE_ENCCALLS,"getType");
   _SFCB_RETURN(__beft_newString(broker, *((char **) object), rc));
}


static CMPIString *__beft_getMessage(const CMPIBroker * broker,
                                     const char *msgId, const char *defMsg,
                                     CMPIStatus * rc, unsigned int count, ...)
{
  if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_SUPPORTED);
  _SFCB_ENTER(TRACE_ENCCALLS,"getMessage");
  _SFCB_TRACE(1,("This operation is not yet supported."));
  _SFCB_RETURN(__beft_newString(broker,defMsg,NULL));
}

static CMPIStatus __beft_logMessage
       (const CMPIBroker* broker,int severity ,const char *id,const char *text,
	const CMPIString *string)
{
  CMPIStatus rc = {CMPI_RC_ERR_NOT_SUPPORTED,NULL};
  _SFCB_ENTER(TRACE_ENCCALLS,"logMessage");
  _SFCB_TRACE(1,("This operation is not yet supported."));
  _SFCB_RETURN(rc);
}

static CMPIStatus __beft_trace
       (const CMPIBroker* broker, int level,const char *component,const char *text,
	const CMPIString *string)
{
  CMPIStatus rc = {CMPI_RC_ERR_NOT_SUPPORTED,NULL};
  _SFCB_ENTER(TRACE_ENCCALLS,"trace");
  _SFCB_TRACE(1,("This operation is not yet supported."));
  _SFCB_RETURN(rc);
}


CMPIArray *getKeyListAndVerifyPropertyList(CMPIObjectPath * cop, 
					   char **props,
					   int *ok,
					   CMPIStatus * rc)
{
   CMPIArray *ar;    
   CMPIConstClass *cc;
   
   cc = (CMPIConstClass *)
       getConstClass(opGetNameSpaceChars(cop), opGetClassNameChars(cop));
   if (cc) {
      ar=cc->ft->getKeyList(cc);
      *ok=verifyPropertyList(cc,props);
      return ar;
   }   
   else return NewCMPIArray(0, CMPI_string, NULL);
}

/****************************************************************************/


CMPIBrokerEncFT native_brokerEncFT = {
   NATIVE_FT_VERSION,
   __beft_newInstance,
   __beft_newObjectPath,
   __beft_newArgs,
   __beft_newString,
   __beft_newArray,
   __beft_newDateTime,
   __beft_newDateTimeFromBinary,
   __beft_newDateTimeFromChars,
   __beft_newSelectExp,
   __beft_classPathIsA,
   __beft_toString,
   __beft_isOfType,
   __beft_getType,
   __beft_getMessage,
   __beft_logMessage,
   __beft_trace
};

CMPIBrokerEncFT *BrokerEncFT = &native_brokerEncFT;

