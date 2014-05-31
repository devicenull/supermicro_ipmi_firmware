
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
 * InteropServer provider for sfcb .
 *
*/



#include "utilft.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include "constClass.h"
#include "providerRegister.h"
#include "trace.h"
#include "control.h"
#include "config.h"

#define NEW(x) ((x *) malloc(sizeof(x)))

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpiftx.h"
#include "cmpimacs.h"
#include "cmpimacsx.h"


static const CMPIBroker *_broker;
static CMPIStatus invClassSt = { CMPI_RC_ERR_INVALID_CLASS, NULL };
static CMPIStatus notSuppSt = { CMPI_RC_ERR_NOT_SUPPORTED, NULL };
static CMPIStatus okSt = { CMPI_RC_OK, NULL };

//------------------------------------------------------------------


static char *getSfcbUuid()
{
   static char *uuid=NULL;
   static char *u=NULL;
   
   if (uuid==NULL) {
      FILE *uuidFile;
      char *fn=alloca(strlen(SFCB_STATEDIR)+strlen("/uuid")+8);
      strcpy(fn,SFCB_STATEDIR);
      strcat(fn,"/uuid");
      uuidFile=fopen(fn,"r");
      if (uuidFile) {
         char u[512];
         if (fgets(u,512,uuidFile)!=NULL) {
            int l=strlen(u);
            if (l) u[l-1]=0;
            uuid=(char*)malloc(l+32);
            strcpy(uuid,"sfcb:");
            strcat(uuid,u);
            fclose(uuidFile);
            return uuid;
         }    
         fclose(uuidFile);
      }
      else if (u==NULL) {
         char hostName[512];
         gethostname(hostName,511);
         u=(char*)malloc(strlen(hostName)+32);
         strcpy(u,"sfcb:NO-UUID-FILE-");
         strcat(u,hostName);
      }  
      return u; 
   }   
   return uuid;
}

//------------------------------------------------------------------


static int genNameSpaceData(const char *ns, const char *dn, int dbl, 
			    const CMPIResult * rslt, CMPIObjectPath *op, 
			    CMPIInstance *ci,int nsOpt)
{
   if (ci) {
      if (nsOpt) CMSetProperty(ci,"Name",dn,CMPI_chars);
      else CMSetProperty(ci,"Name",ns+dbl+1,CMPI_chars);
      CMReturnInstance(rslt,ci);
   } else if (op) {
      if (nsOpt) CMAddKey(op,"Name",dn,CMPI_chars);
      else CMAddKey(op,"Name",ns+dbl+1,CMPI_chars);
      CMReturnObjectPath(rslt,op);
   }  
   return 0;
}   


static void gatherNameSpacesData(const char *dn, int dbl, 
				 const CMPIResult * rslt, 
				 CMPIObjectPath *op, CMPIInstance *ci, int nsOpt)
{
   DIR *dir, *de_test;
   struct dirent *de;
   char *n;
   int l;
   
   dir=opendir(dn);
   if (dir) while ((de=readdir(dir))!=NULL) {
     if (strcmp(de->d_name,".")==0) continue;
     if (strcmp(de->d_name,"..")==0) continue;
     l=strlen(dn)+strlen(de->d_name)+4;
     n=(char*)malloc(l+8);
     strcpy(n,dn);
     strcat(n,"/");
     strcat(n,de->d_name);
     de_test = opendir(n);
     if (de_test == NULL) {
       free(n);
       continue;
     }
     closedir(de_test);
     
     genNameSpaceData(n,de->d_name,dbl,rslt,op,ci,nsOpt);
     if (nsOpt!=1) { 
        if (nsOpt==0) gatherNameSpacesData(n,dbl,rslt,op,ci,nsOpt);
     }
     free(n);
   }
   closedir(dir);     
} 


static void gatherOldNameSpacesData(const char *dn, int dbl, 
				    const CMPIResult * rslt, 
				    CMPIObjectPath *op, 
				    CMPIInstance *ci)
{
   
   char *ns = (char*)CMGetNameSpace(op,NULL)->hdl; 
   char *nns=alloca(strlen(dn)+strlen(ns)+8);
   
   strcpy(nns,dn);
   strcat(nns,"/");
   strcat(nns,ns);
   gatherNameSpacesData(nns,dbl,rslt,op,ci,1);     
}     

static CMPIStatus NameSpaceProviderGetInstance(CMPIInstanceMI * mi,
					       const CMPIContext * ctx,
					       const CMPIResult * rslt,
					       const CMPIObjectPath * cop,
					       const char **properties,
					       int nsOpt)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char *dirn,*dn,hostName[512];
   DIR *dir;
   CMPIObjectPath *op;
   CMPIInstance *ci;
   CMPIString *name;
   unsigned short info=0,dbl;
   char *ns;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "NameSpaceProviderGetInstance");
   
   if (getControlChars("registrationDir",&dirn)) {
     dirn = "/var/lib/sfcb/registration";
   }
   
   name=CMGetKey(cop,"name",NULL).value.string;
   
   if (name && name->hdl) {
      dn=(char*)alloca(strlen(dirn)+32+strlen((char*)name->hdl));
      strcpy(dn,dirn);
      if (dirn[strlen(dirn)-1]!='/') strcat(dn,"/");
      strcat(dn,"repository/");
      if (nsOpt) {
	ns = CMGetCharPtr(CMGetNameSpace(cop,NULL));
	if (ns) {
	  strcat(dn,ns);
	  strcat(dn,"/");
	}
      }
      dbl=strlen(dn);
      strcat(dn,(char*)name->hdl);
      
      if ((dir=opendir(dn))!=NULL) {
	if (nsOpt) {
	  op=CMNewObjectPath(_broker,"root/interop","__Namespace",NULL);
	  ci=CMNewInstance(_broker,op,NULL);
	} else {
	  op=CMNewObjectPath(_broker,"root/interop","CIM_Namespace",NULL);
	  ci=CMNewInstance(_broker,op,NULL);
	  
	  CMSetProperty(ci,"CreationClassName","CIM_Namespace",CMPI_chars);
	  CMSetProperty(ci,"ObjectManagerCreationClassName","CIM_ObjectManager",CMPI_chars);
	  CMSetProperty(ci,"ObjectManagerName",getSfcbUuid(),CMPI_chars);
	  CMSetProperty(ci,"SystemCreationClassName","CIM_ComputerSystem",CMPI_chars);
	  hostName[0]=0;
	  gethostname(hostName,511);
	  CMSetProperty(ci,"SystemName",hostName,CMPI_chars);
	  CMSetProperty(ci,"ClassInfo",&info,CMPI_uint16);
	}
	CMSetProperty(ci,"Name",dn+dbl,CMPI_chars);
	CMReturnInstance(rslt,ci);
	closedir(dir);
      }
      else st.rc=CMPI_RC_ERR_NOT_FOUND;   
   }
   else st.rc=CMPI_RC_ERR_NO_SUCH_PROPERTY;  
   
   _SFCB_RETURN(st);
}

static CMPIStatus NameSpaceProviderEnumInstances(CMPIInstanceMI * mi, 
						 const CMPIContext * ctx, 
						 const CMPIResult * rslt,
						 const CMPIObjectPath * ref, 
						 const char **properties,
						 int nsOpt)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char *dir,*dn,hostName[512];
   CMPIObjectPath *op;
   CMPIInstance *ci;
   unsigned short info=0;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "NameSpaceProviderEnumInstances");

   if (getControlChars("registrationDir",&dir)) {
     dir = "/var/lib/sfcb/registration";
   }
   
   dn=(char*)alloca(strlen(dir)+32);
   strcpy(dn,dir);
   if (dir[strlen(dir)-1]!='/') strcat(dn,"/");
   strcat(dn,"repository");
   
   if (nsOpt) {
      op=CMNewObjectPath(_broker,"root/interop","__Namespace",&st);
      if (op) {
	ci=CMNewInstance(_broker,op,&st);
	if (ci) {
	  op=CMGetObjectPath(ci,NULL);
	  CMSetNameSpaceFromObjectPath(op,ref);
	  gatherOldNameSpacesData(dn,strlen(dn),rslt,op,ci);  
	}
      }
      _SFCB_RETURN(st);
   }
   
   op=CMNewObjectPath(_broker,"root/interop","CIM_Namespace",NULL);
   ci=CMNewInstance(_broker,op,NULL);
   
   CMSetProperty(ci,"CreationClassName","CIM_Namespace",CMPI_chars);
   CMSetProperty(ci,"ObjectManagerCreationClassName","CIM_ObjectManager",CMPI_chars);
   CMSetProperty(ci,"ObjectManagerName",getSfcbUuid(),CMPI_chars);
   CMSetProperty(ci,"SystemCreationClassName","CIM_ComputerSystem",CMPI_chars);
   hostName[0]=0;
   gethostname(hostName,511);
   CMSetProperty(ci,"SystemName",hostName,CMPI_chars);
   CMSetProperty(ci,"ClassInfo",&info,CMPI_uint16);
   
   gatherNameSpacesData(dn,strlen(dn),rslt,NULL,ci,0);
   
   _SFCB_RETURN(st);
}

static CMPIStatus NameSpaceProviderEnumInstanceNames(CMPIInstanceMI * mi,
						     const CMPIContext * ctx,
						     const CMPIResult * rslt,
						     const CMPIObjectPath * ref,
						     int nsOpt)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char *dir,*dn,hostName[512];
   CMPIObjectPath *op;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "NameSpaceProviderEnumInstanceNames");

   if (getControlChars("registrationDir",&dir)) {
     dir = "/var/lib/sfcb/registration";
   }
   
   dn=(char*)alloca(strlen(dir)+32);
   strcpy(dn,dir);
   if (dir[strlen(dir)-1]!='/') strcat(dn,"/");
   strcat(dn,"repository");
   
   if (nsOpt) {
      char *ns=(char*)CMGetNameSpace(ref,NULL)->hdl; 
      op=CMNewObjectPath(_broker,ns,"__Namespace",NULL);
      gatherOldNameSpacesData(dn,strlen(dn),rslt,op,NULL);  
      _SFCB_RETURN(st);
   }
   
   op=CMNewObjectPath(_broker,"root/interop","CIM_Namespace",NULL);
   
   CMAddKey(op,"CreationClassName","CIM_Namespace",CMPI_chars);
   CMAddKey(op,"ObjectManagerCreationClassName","CIM_ObjectManager",CMPI_chars);
   CMAddKey(op,"ObjectManagerName",getSfcbUuid(),CMPI_chars);
   CMAddKey(op,"SystemCreationClassName","CIM_ComputerSystem",CMPI_chars);
   hostName[0]=0;
   gethostname(hostName,511);
   CMAddKey(op,"SystemName",hostName,CMPI_chars);
   
   gatherNameSpacesData(dn,strlen(dn),rslt,op,NULL,nsOpt);
   
   _SFCB_RETURN(st);
}

//------------------------------------------------------------------

static CMPIStatus ObjectManagerProviderEnumInstanceNames(CMPIInstanceMI * mi,
							 const CMPIContext * ctx,
							 const CMPIResult * rslt,
							 const CMPIObjectPath * ref)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char hostName[512];
   CMPIObjectPath *op;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ObjectManagerProviderEnumInstanceNames");

   op=CMNewObjectPath(_broker,"root/interop","CIM_ObjectManager",NULL);
   
   CMAddKey(op,"CreationClassName","CIM_ObjectManager",CMPI_chars);
   CMAddKey(op,"SystemCreationClassName","CIM_ComputerSystem",CMPI_chars);
   hostName[0]=0;
   gethostname(hostName,511);
   CMAddKey(op,"SystemName",hostName,CMPI_chars);
   CMAddKey(op,"Name",getSfcbUuid(),CMPI_chars);
   
   CMReturnObjectPath(rslt,op);
   
   _SFCB_RETURN(st);
}

static CMPIStatus ObjectManagerProviderEnumInstances(CMPIInstanceMI * mi,
						     const CMPIContext * ctx,
						     const CMPIResult * rslt,
						     const CMPIObjectPath * ref,
						     const char **properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char str[512];
   CMPIObjectPath *op;
   CMPIInstance *ci;
   CMPIUint16 state;
   CMPIBoolean bul=0;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ObjectManagerProviderEnumInstanceNames");

   op=CMNewObjectPath(_broker,"root/interop","CIM_ObjectManager",NULL);
   ci=CMNewInstance(_broker,op,NULL);
   
   CMSetProperty(ci,"CreationClassName","CIM_ObjectManager",CMPI_chars);
   CMSetProperty(ci,"SystemCreationClassName","CIM_ComputerSystem",CMPI_chars);
   str[0]=0;
   gethostname(str,511);
   CMSetProperty(ci,"SystemName",str,CMPI_chars);
   CMSetProperty(ci,"Name",getSfcbUuid(),CMPI_chars);
   CMSetProperty(ci,"GatherStatisticalData",&bul,CMPI_boolean);
   CMSetProperty(ci,"ElementName","sfcb",CMPI_chars);
   CMSetProperty(ci,"Description",PACKAGE_STRING,CMPI_chars);
   state=5;
   CMSetProperty(ci,"EnabledState",&state,CMPI_uint16);
   CMSetProperty(ci,"RequestedState",&state,CMPI_uint16);
   state=2;
   CMSetProperty(ci,"EnabledDefault",&state,CMPI_uint16);
   
    CMReturnInstance(rslt,ci);
   
   _SFCB_RETURN(st);
}


static CMPIStatus ObjectManagerProviderGetInstance(CMPIInstanceMI * mi,
						   const CMPIContext * ctx,
						   const CMPIResult * rslt,
						   const CMPIObjectPath * ref, 
						   const char **properties)
{
  CMPIStatus st = { CMPI_RC_OK, NULL };
  CMPIString *name=CMGetKey(ref,"name",NULL).value.string;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ObjectManagerProviderGetInstance");

   if (name && name->hdl) {
      if (strcasecmp((char*)name->hdl,getSfcbUuid())==0)  
         return ObjectManagerProviderEnumInstances(mi,ctx,rslt,ref,properties);
      else st.rc=CMPI_RC_ERR_NOT_FOUND;   
   }
   else st.rc=CMPI_RC_ERR_NO_SUCH_PROPERTY;  
   
   _SFCB_RETURN(st);
}

// ---------------------------------------------------------------

static CMPIStatus ComMechProviderEnumInstanceNames(CMPIInstanceMI * mi,
						   const CMPIContext * ctx,
						   const CMPIResult * rslt,
						   const CMPIObjectPath * ref)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char hostName[512];
   CMPIObjectPath *op;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ComMechProviderEnumInstanceNames");

   op=CMNewObjectPath(_broker,"root/interop","CIM_ObjectManagerCommunicationMechanism",NULL);
   
   CMAddKey(op,"SystemCreationClassName","CIM_ObjectManager",CMPI_chars);
   CMAddKey(op,"CreationClassName","CIM_ObjectManagerCommunicationMechanism",CMPI_chars);
   hostName[0]=0;
   gethostname(hostName,511);
   CMAddKey(op,"SystemName",hostName,CMPI_chars);
   CMAddKey(op,"Name",getSfcbUuid(),CMPI_chars);
   
   CMReturnObjectPath(rslt,op);
   
   _SFCB_RETURN(st);
}

static CMPIStatus ComMechProviderEnumInstances(CMPIInstanceMI * mi,
					       const CMPIContext * ctx,
					       const CMPIResult * rslt,
					       const CMPIObjectPath * ref, 
					       const char **properties)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   char hostName[512];
   CMPIObjectPath *op;
   CMPIInstance *ci;
   CMPIUint16 mech;
   CMPIBoolean bul=0;
   int i;
   
   CMPIArray *fps;
   CMPIUint16 fpa[6]={2,3,5,6,7,9};
   CMPIArray *as;
   CMPIUint16 aa[1]={3};
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ComMechProviderEnumInstanceNames");

   op=CMNewObjectPath(_broker,"root/interop","CIM_ObjectManagerCommunicationMechanism",NULL);
   ci=CMNewInstance(_broker,op,NULL);
      
   CMSetProperty(ci,"SystemCreationClassName","CIM_ObjectManager",CMPI_chars);
   CMSetProperty(ci,"CreationClassName","CIM_ObjectManagerCommunicationMechanism",CMPI_chars);
   hostName[0]=0;
   gethostname(hostName,511);
   CMSetProperty(ci,"SystemName",hostName,CMPI_chars);
   CMSetProperty(ci,"Name",getSfcbUuid(),CMPI_chars);
   /* Version of CIM-XML that is supported */
   CMSetProperty(ci,"Version","1.0",CMPI_chars);
   
   mech=2;
   CMSetProperty(ci,"CommunicationMechanism",&mech,CMPI_uint16);
   
   fps=CMNewArray(_broker,sizeof(fpa)/sizeof(CMPIUint16),CMPI_uint16,NULL);
   for (i=0; i<sizeof(fpa)/sizeof(CMPIUint16); i++)
      CMSetArrayElementAt(fps,i,&fpa[i],CMPI_uint16);
   CMSetProperty(ci,"FunctionalProfilesSupported",&fps,CMPI_uint16A);
   
   as=CMNewArray(_broker,sizeof(aa)/sizeof(CMPIUint16),CMPI_uint16,NULL);
   for (i=0; i<sizeof(aa)/sizeof(CMPIUint16); i++)
      CMSetArrayElementAt(as,i,&aa[i],CMPI_uint16);
   CMSetProperty(ci,"AuthenticationMechanismsSupported",&as,CMPI_uint16A);
   
   CMSetProperty(ci,"MultipleOperationsSupported",&bul,CMPI_boolean);
      
   CMReturnInstance(rslt,ci);
   
   _SFCB_RETURN(st);
}

static CMPIStatus ComMechProviderGetInstance(CMPIInstanceMI * mi,
                                             const CMPIContext * ctx,
                                             const CMPIResult * rslt,
                                             const CMPIObjectPath * ref, 
					     const char **properties)
{
  CMPIStatus st = { CMPI_RC_OK, NULL };
  CMPIString *name=CMGetKey(ref,"name",NULL).value.string;
   
   _SFCB_ENTER(TRACE_PROVIDERS, "ComMechProviderGetInstance");

   if (name && name->hdl) {
      if (strcasecmp((char*)name->hdl,getSfcbUuid())==0)  
         return ComMechProviderEnumInstances(mi,ctx,rslt,ref,properties);
      else st.rc=CMPI_RC_ERR_NOT_FOUND;   
   }
   else st.rc=CMPI_RC_ERR_NO_SUCH_PROPERTY;  
   
   _SFCB_RETURN(st);
}

// ---------------------------------------------------------------

static CMPIStatus ServerProviderCleanup(CMPIInstanceMI * mi, const CMPIContext * ctx, CMPIBoolean terminate)
{
   CMPIStatus st = { CMPI_RC_OK, NULL };
   
   return (st);
}

static CMPIStatus ServerProviderGetInstance(CMPIInstanceMI * mi,
					    const CMPIContext * ctx,
					    const CMPIResult * rslt,
					    const CMPIObjectPath * ref,
					    const char **properties)
{
   CMPIString *cls=CMGetClassName(ref,NULL);
   
   if (strcasecmp((char*)cls->hdl,"cim_namespace")==0) 
     return NameSpaceProviderGetInstance(mi, ctx, rslt, ref, properties, 0);
   else if (strcasecmp((char*)cls->hdl,"__namespace")==0) 
     return NameSpaceProviderGetInstance(mi, ctx, rslt, ref, properties, 1);
   if (strcasecmp((char*)cls->hdl,"cim_objectmanager")==0) 
      return ObjectManagerProviderGetInstance(mi, ctx, rslt, ref, properties);
   if (strcasecmp((char*)cls->hdl,"cim_objectmanagercommunicationMechanism")==0) 
      return ComMechProviderGetInstance(mi, ctx, rslt, ref, properties);
   
   return invClassSt;
}

static CMPIStatus ServerProviderEnumInstanceNames(CMPIInstanceMI * mi,
						  const CMPIContext * ctx,
						  const CMPIResult * rslt,
						  const CMPIObjectPath * ref)
{
   CMPIString *cls=CMGetClassName(ref,NULL);
   
   if (strcasecmp((char*)cls->hdl,"cim_namespace")==0) 
      return NameSpaceProviderEnumInstanceNames(mi, ctx, rslt, ref,0);
   if (strcasecmp((char*)cls->hdl,"__namespace")==0) 
      return NameSpaceProviderEnumInstanceNames(mi, ctx, rslt, ref,1);
   if (strcasecmp((char*)cls->hdl,"cim_objectmanager")==0) 
      return ObjectManagerProviderEnumInstanceNames(mi, ctx, rslt, ref);
   if (strcasecmp((char*)cls->hdl,"cim_objectmanagercommunicationMechanism")==0) 
      return ComMechProviderEnumInstanceNames(mi, ctx, rslt, ref);
  
   return okSt;
}                                                

static CMPIStatus ServerProviderEnumInstances(CMPIInstanceMI * mi, 
					      const CMPIContext * ctx, 
					      const CMPIResult * rslt,
					      const CMPIObjectPath * ref, 
					      const char **properties)
{
   CMPIString *cls=CMGetClassName(ref,NULL);
   
   if (strcasecmp((char*)cls->hdl,"cim_namespace")==0) 
      return NameSpaceProviderEnumInstances(mi, ctx, rslt, ref, properties, 0);
   if (strcasecmp((char*)cls->hdl,"__namespace")==0) 
      return NameSpaceProviderEnumInstances(mi, ctx, rslt, ref, properties, 1);
   if (strcasecmp((char*)cls->hdl,"cim_objectmanager")==0) 
      return ObjectManagerProviderEnumInstances(mi, ctx, rslt, ref, properties);
   if (strcasecmp((char*)cls->hdl,"cim_objectmanagercommunicationMechanism")==0) 
      return ComMechProviderEnumInstances(mi, ctx, rslt, ref, properties);
   
   return okSt;
}                                                

static CMPIStatus ServerProviderCreateInstance(CMPIInstanceMI * mi,
					       const CMPIContext * ctx,
					       const CMPIResult * rslt,
					       const CMPIObjectPath * cop,
					       const CMPIInstance * ci)
{
   return notSuppSt;
}

static CMPIStatus ServerProviderModifyInstance(CMPIInstanceMI * mi,
					       const CMPIContext * ctx,
					       const CMPIResult * rslt,
					       const CMPIObjectPath * cop,
					       const CMPIInstance * ci, 
					       const char **properties)
{
   return notSuppSt;
}

static CMPIStatus ServerProviderDeleteInstance(CMPIInstanceMI * mi,
					       const CMPIContext * ctx,
					       const CMPIResult * rslt,
					       const CMPIObjectPath * ref)
{
   return notSuppSt;
}

static CMPIStatus ServerProviderExecQuery(CMPIInstanceMI * mi,
					  const CMPIContext * ctx,
					  const CMPIResult * rslt,
					  const CMPIObjectPath * cop,
					  const char *lang, 
					  const char *query)
{
   return notSuppSt;
}


 
CMInstanceMIStub(ServerProvider, ServerProvider, _broker, CMNoHook);

