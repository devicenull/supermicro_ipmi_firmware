#include <cimc.h>
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "cmcimacs.h"

extern char *value2Chars(CMPIType type, CMPIValue * value);
void showProperty( CMPIData , char * );
void showInstance( CMPIInstance * );
static char * CMPIState_str(CMPIValueState);
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
CMPIObjectPath *op = NULL ;
CIMCEnumeration *enm = NULL;
CIMCClient *client = NULL;
CIMCInstance *instance = NULL;
CIMCData data;
char 	*cim_host, *cim_host_passwd, *cim_host_userid, *cim_host_port;
int count = 0;

    /*
     * Setup a connection to the CIMOM by checking environment 
     * if not found we default those values
     */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL)
       cim_host = "localhost";
    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL)
       cim_host_userid = "root";
    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL)
       cim_host_passwd = "password";
    if (cim_host_port == NULL)
	     cim_host_port = "5988";
	     
    printf(" Testing enumerateInstanceNames \n") ;
    
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
      printf(" local connect failed call to NewCIMCEnv message = [%s] \n",msg) ;
      return ;	
    }
    printf("do connect \n") ;
    client = ce->ft->connect(ce, cim_host , "http", cim_host_port, cim_host_userid, cim_host_passwd , &status);
    if(client == NULL) 
    {
       printf(" failed the call to connect \n") ;	
    }
    
    printf("do newObjectPath \n") ;
    op = (CMPIObjectPath *)ce->ft->newObjectPath(ce, "root/cimv2", "CIM_ManagedElement" , &status);     
    if(op == NULL) 
    {
       printf(" failed the call to newObjectPath \n") ;	
    }
        
    printf("do enumInstances \n") ;
    enm = client->ft->enumInstanceNames(client, (CIMCObjectPath *) op , &status);
    if(enm == NULL) 
    {
       printf(" failed the call to client->ft->enumInstances \n") ;	
    }
       
    /* Print the results */
   
    if (!status.rc) {
       printf("results:\n");
       count = enm->ft->hasNext(enm, NULL) ;
       while (count > 0) {
       	  
          data = enm->ft->getNext(enm, NULL);

          showObjectPath(data.value.ref);
          /*
           * see if we have any more
           */
           count = enm->ft->hasNext(enm, NULL) ;   
       }
    } else {
       printf("  ERROR received from enumInstanceNames  status.rc = %d\n",status.rc) ;
       if(msg)
       	 printf("  ERROR msg = %s\n",msg) ;
    }
    
    if(enm) enm->ft->release(enm);
    if(op) op->ft->release(op);
    if(client) client->ft->release(client);
    if(ce) ce->ft->release(ce);
    if(status.msg) CMRelease(status.msg);	
    	
}

/* */
