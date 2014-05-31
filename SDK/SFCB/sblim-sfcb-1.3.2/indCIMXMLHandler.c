
/*
 * indCIMXMLHandler.c
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
 *
 * Description:
 *
 * CIM XML handler for indications.
 *
*/



#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trace.h"
#include "fileRepository.h"
#include "providerMgr.h"
#include "internalProvider.h" 
#include "cimXmlRequest.h" 

extern void closeProviderContext(BinRequestContext* ctx);
extern int exportIndication(char *url, char *payload, char **resp, char **msg);
extern void dumpSegments(void*);
extern int exportIndication(char *url, char *payload, char **resp, char **msg);
extern UtilStringBuffer *segments2stringBuffer(RespSegment *rs);
extern UtilStringBuffer *newStringBuffer(int);
extern void setStatus(CMPIStatus *st, CMPIrc rc, char *msg);

extern ExpSegments exportIndicationReq(CMPIInstance *ci, char *id);


static const CMPIBroker *_broker;
 
static int interOpNameSpace(const CMPIObjectPath *cop, CMPIStatus *st) 
 {   
   char *ns=(char*)CMGetNameSpace(cop,NULL)->hdl;   
   if (strcasecmp(ns,"root/interop") && strcasecmp(ns,"root/pg_interop")) {
      setStatus(st,CMPI_RC_ERR_FAILED,"Object must reside in root/interop");
      return 0;
   }
   return 1;
}
   
/* ------------------------------------------------------------------ *
 * Instance MI Cleanup
 * ------------------------------------------------------------------ */

CMPIStatus IndCIMXMLHandlerCleanup(CMPIInstanceMI * mi, const CMPIContext * ctx, CMPIBoolean terminating)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerCleanup");
   _SFCB_RETURN(st);
}

/* ------------------------------------------------------------------ *
 * Instance MI Functions
 * ------------------------------------------------------------------ */


CMPIStatus IndCIMXMLHandlerEnumInstanceNames(CMPIInstanceMI * mi,
                                             const CMPIContext * ctx,
                                             const CMPIResult * rslt,
                                             const CMPIObjectPath * ref)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerEnumInstanceNames");
   st=InternalProviderEnumInstanceNames(NULL,ctx,rslt,ref);
   _SFCB_RETURN(st);
}

CMPIStatus IndCIMXMLHandlerEnumInstances(CMPIInstanceMI * mi,
                                         const CMPIContext * ctx,
                                         const CMPIResult * rslt,
                                         const CMPIObjectPath * ref,
                                         const char **properties)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerEnumInstances");
   st=SafeInternalProviderEnumInstances(NULL,ctx,rslt,ref,properties,1);
   _SFCB_RETURN(st);
}


CMPIStatus IndCIMXMLHandlerGetInstance(CMPIInstanceMI * mi,
                                       const CMPIContext * ctx,
                                       const CMPIResult * rslt,
                                       const CMPIObjectPath * cop,
                                       const char **properties)
{
   CMPIStatus st;
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerGetInstance");
   st=InternalProviderGetInstance(NULL,ctx,rslt,cop,properties);
   _SFCB_RETURN(st);
}

CMPIStatus IndCIMXMLHandlerCreateInstance(CMPIInstanceMI * mi,
                                          const CMPIContext * ctx,
                                          const CMPIResult * rslt,
                                          const CMPIObjectPath * cop,
                                          const CMPIInstance * ci)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIArgs *in,*out=NULL;
   CMPIObjectPath *op;
   CMPIData rv;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerCreateInstance");
   
   if (interOpNameSpace(cop,&st)==0) _SFCB_RETURN(st);

   internalProviderGetInstance(cop,&st);
   if (st.rc==CMPI_RC_ERR_FAILED) _SFCB_RETURN(st);
   if (st.rc==CMPI_RC_OK) {
      setStatus(&st,CMPI_RC_ERR_ALREADY_EXISTS,NULL);
      _SFCB_RETURN(st); 
   }

            CMPIString *str=CDToString(_broker,cop,NULL);
            CMPIString *ns=CMGetNameSpace(cop,NULL);
            _SFCB_TRACE(1,("--- handler %s %s",(char*)ns->hdl,(char*)str->hdl));
            
   in=CMNewArgs(_broker,NULL);
   CMAddArg(in,"handler",&ci,CMPI_instance);
   CMAddArg(in,"key",&cop,CMPI_ref);
   op=CMNewObjectPath(_broker,"root/interop","cim_indicationsubscription",&st);
   rv=CBInvokeMethod(_broker,ctx,op,"_addHandler",in,out,&st);

   if (st.rc==CMPI_RC_OK) 
      st=InternalProviderCreateInstance(NULL,ctx,rslt,cop,ci);
      
   _SFCB_RETURN(st);
}

CMPIStatus IndCIMXMLHandlerModifyInstance(CMPIInstanceMI * mi,
					  const CMPIContext * ctx,
					  const CMPIResult * rslt,
					  const CMPIObjectPath * cop,
					  const CMPIInstance * ci, 
					  const char **properties)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerSetInstance");   
   _SFCB_RETURN(st);
}

CMPIStatus IndCIMXMLHandlerDeleteInstance(CMPIInstanceMI * mi,
                                          const CMPIContext * ctx,
                                          const CMPIResult * rslt,
                                          const CMPIObjectPath * cop)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIArgs *in,*out=NULL;
   CMPIObjectPath *op;
   CMPIData rv;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerDeleteInstance");  
    
   if (interOpNameSpace(cop,&st)==0) _SFCB_RETURN(st);
   
   internalProviderGetInstance(cop,&st);
   if (st.rc) _SFCB_RETURN(st);
   
   in=CMNewArgs(_broker,NULL);
   CMAddArg(in,"key",&cop,CMPI_ref);
   op=CMNewObjectPath(_broker,"root/interop","cim_indicationsubscription",&st);
   rv=CBInvokeMethod(_broker,ctx,op,"_removeHandler",in,out,&st);

   if (st.rc==CMPI_RC_OK) {
      st=InternalProviderDeleteInstance(NULL,ctx,rslt,cop);
   }
   
   _SFCB_RETURN(st);
}

CMPIStatus IndCIMXMLHandlerExecQuery(CMPIInstanceMI * mi,
                                     const CMPIContext * ctx,
                                     const CMPIResult * rslt,
                                     const CMPIObjectPath * cop,
                                     const char *lang, const char *query)
{
   CMPIStatus st = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerExecQuery");   
   _SFCB_RETURN(st);
}


/* ---------------------------------------------------------------------------*/
/*                        Method Provider Interface                           */
/* ---------------------------------------------------------------------------*/

CMPIStatus IndCIMXMLHandlerMethodCleanup(CMPIMethodMI * mi, 
					 const CMPIContext * ctx, 
					 CMPIBoolean terminating)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerMethodCleanup");   
   _SFCB_RETURN(st);
}

CMPIStatus IndCIMXMLHandlerInvokeMethod(CMPIMethodMI * mi,
					const CMPIContext * ctx,
					const CMPIResult * rslt,
					const CMPIObjectPath * ref,
					const char *methodName,
					const CMPIArgs * in, CMPIArgs * out)
{ 
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIInstance *hci,*ind;
   static int id=1;
   char strId[64];
   ExpSegments xs;
   UtilStringBuffer *sb;
   char *resp;
   char *msg;
   CMPIString *dest;
   
   _SFCB_ENTER(TRACE_INDPROVIDER, "IndCIMXMLHandlerInvokeMethod");  
    
   if (interOpNameSpace(ref,&st)==0) _SFCB_RETURN(st);
   
   if (strcasecmp(methodName, "_deliver") == 0) {     
      if ((hci=internalProviderGetInstance(ref,&st))==NULL) {
         setStatus(&st,CMPI_RC_ERR_NOT_FOUND,NULL);
         _SFCB_RETURN(st); 
      }
      dest=CMGetProperty(hci,"destination",NULL).value.string;
      _SFCB_TRACE(1,("--- destination: %s\n",(char*)dest->hdl));
      ind=CMGetArg(in,"indication",NULL).value.inst;
      sprintf(strId,"%d",id++);
      xs=exportIndicationReq(ind,strId);
      sb=segments2stringBuffer(xs.segments);
      exportIndication((char*)dest->hdl,(char*)sb->ft->getCharPtr(sb), &resp, &msg);

      CMRelease(sb);
      /* there's a UtilStringBuffer burried in xs... */
      RespSegment rs = xs.segments[5];
      UtilStringBuffer* usb = (UtilStringBuffer*)rs.txt;
      CMRelease(usb);
      if (resp) free(resp);
      if (msg) free(msg);
   }
   
   else {
      printf("--- ClassProvider: Invalid request %s\n", methodName);
      st.rc = CMPI_RC_ERR_METHOD_NOT_FOUND;
   }
   return st;
   _SFCB_RETURN(st);
}




CMInstanceMIStub(IndCIMXMLHandler, IndCIMXMLHandler, _broker, CMNoHook);
CMMethodMIStub(IndCIMXMLHandler, IndCIMXMLHandler, _broker, CMNoHook);

