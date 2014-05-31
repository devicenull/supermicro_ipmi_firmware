/*
 * test_gc.c
 *
 * (C) Copyright IBM Corp. 2005
 * (C) Copyright Intel Corp. 2005
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
 *  Test for getClass() library API.
 */
#include <cimc.h>
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>

#include "show.h"
/* ********************************************************
 * environment variables  
 * CIM_HOST
 * CIM_HOST_USERID
 * CIM_HOST_PASSWD
 * CIM_HOST_PORT
 *
 *
 * ********************************************************
 * comment out v2local define to use v2 http XML interface
 * 
 * ********************************************************
 */
#define v2local

int main()
{
    CIMCEnv *ce;
    char *msg = NULL;
    int rc;
    CIMCStatus status;
    CIMCClient *client;
    CMPIObjectPath * op;
    CMPIConstClass * class;
    char 	*cim_host, *cim_host_passwd, *cim_host_userid , *cim_host_port;
    

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
    cim_host_port = getenv("CIM_HOST_PORT");
	  if (cim_host_port == NULL)
	     cim_host_port = "5988";

    printf(" Testing getClass \n") ;

#ifdef v2local
    printf(" using SfcbLocal interface : host = %s userid = %s\n",
                          cim_host,cim_host_userid) ;
    ce = NewCIMCEnv("SfcbLocal",0,&rc,&msg);
#else
    printf(" using XML HTTP interface : host = %s userid = %s port=%s\n",
                          cim_host,cim_host_userid,cim_host_port) ;
    ce = NewCIMCEnv("XML",0,&rc,&msg);
#endif
	
	  client = ce->ft->connect(ce, cim_host , "http", cim_host_port, cim_host_userid, cim_host_passwd , &status);

    op = (CMPIObjectPath *)ce->ft->newObjectPath(ce, "root/cimv2", "CIM_ComputerSystem" , &status); 
    class =(CMPIConstClass *) client->ft->getClass(client,(CIMCObjectPath *) op, CMPI_FLAG_IncludeQualifiers, NULL, &status);
	 
    /* Print the results */
    printf( "getClass() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (!status.rc) {
        printf("result:\n");
        showClass(class);
    }

    if (class) class->ft->release((CMPIConstClass *)class);
    if (op) op->ft->release(op);

    if (client) client->ft->release(client);
    if(ce) ce->ft->release(ce);
    if (status.msg) CMRelease(status.msg);
    
    return 0;
}
