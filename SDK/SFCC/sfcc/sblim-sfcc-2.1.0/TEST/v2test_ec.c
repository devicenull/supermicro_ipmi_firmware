#include <cimc.h>
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "cmcimacs.h"

extern char *value2Chars(CMPIType type, CMPIValue * value);
/*
 * comment out this define to use v2 http XML interface
 */
#define v2local

int main()
{
    CIMCEnv *ce;
    char *msg = NULL;
    int rc;
    CIMCStatus status;
    CIMCClient *client;
    CIMCObjectPath *op;
    CMPIConstClass* clas ;
    CIMCEnumeration *enm;
    CIMCString *path;
    CMPIString * classname ;
    CIMCData data;
    CMPIData cdata ;
    char 	*cim_host, *cim_host_passwd, *cim_host_userid , *cim_host_port;
    int i = 0 ;
    int retc = 0;
    int count = 0;
    int numproperties = 0;
    CMPIString * propertyname;
    char *cv;
    
    /* Setup a connection to the CIMOM */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL)
	        cim_host = "localhost";
    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL)
	        cim_host_userid = "root";
    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL)
	        cim_host_passwd = "password";
    cim_host_port = getenv("CIM_HOST_PORT");
	  if (cim_host_port == NULL)
	     cim_host_port = "5988";
	     
    printf(" Testing enumerateClasses \n") ;
    	        
#ifdef v2local
    printf(" using SfcbLocal interface : host = %s userid = %s\n",
                          cim_host,cim_host_userid) ;
    ce = NewCIMCEnv("SfcbLocal",0,&rc,&msg);
#else
    printf(" using XML HTTP interface : host = %s userid = %s port=%s\n",
                          cim_host,cim_host_userid,cim_host_port) ;
    ce = NewCIMCEnv("XML",0,&rc,&msg);
#endif

    if(ce == NULL) {
      printf(" local connect failed call to NewCIMCEnv rc = %d , message = [%s] \n",retc,msg) ;
      return ;	
    }
    
    client = ce->ft->connect(ce, cim_host , "http", cim_host_port, cim_host_userid, cim_host_passwd , &status);

    op = ce->ft->newObjectPath(ce, "root/cimv2", NULL , &status); 
    
    enm = client->ft->enumClasses(client,
                                  op,
                                  CMPI_FLAG_DeepInheritance|CMPI_FLAG_IncludeQualifiers,
                                  &status);
     
    if (!status.rc) {
       printf("results:\n");
       count = enm->ft->hasNext(enm, NULL) ;
       while (count > 0) {
          data = enm->ft->getNext(enm, NULL);
          showClass(data.value.cls);          
          /*
           * see if we have any more
           */
          count = enm->ft->hasNext(enm, NULL) ;
       }          
    } else {
       printf(" we have a status.rc value of %d  \n",status.rc);
       
    }
    
    if(ce) ce->ft->release(ce);
    if(client) client->ft->release(client);
    if(op) op->ft->release(op);
    if(enm) enm->ft->release(enm);
    if(status.msg) CMRelease(status.msg);

    return 0;

}

/* end */

