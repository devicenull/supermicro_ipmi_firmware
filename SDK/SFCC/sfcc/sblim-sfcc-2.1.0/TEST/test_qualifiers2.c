/*
 * test_ei.c
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
 *  Test for enumInstances() library API.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "show.h"

int main( int argc, char * argv[] )
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIEnumeration * enumeration;
    CMPIStatus status;
    char 	*cim_host, *cim_host_passwd, *cim_host_userid;
    CMPIData qd;
    int count;

    /* Setup a conncetion to the CIMOM */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL)
	cim_host = "localhost";
    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL)
	cim_host_userid = "root";
    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL)
	cim_host_passwd = "password";
    cc = cmciConnect(cim_host, NULL, "5988",
			       cim_host_userid, cim_host_passwd, NULL);
   
    /* Test enumClasses() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumClasses() ...\n");
    objectpath = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
    enumeration = cc->ft->enumClasses(cc, objectpath, CMPI_FLAG_IncludeQualifiers, &status);

    /* Print the results */
    printf( "enumClasses() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showClass(data.value.cls);
            count=data.value.cls->ft->getQualifierCount(data.value.cls,NULL);
            fprintf(stderr,"Qualifier count: %d\n",count);
            qd=data.value.cls->ft->getQualifier(data.value.cls,"Description",NULL);
            qd=data.value.cls->ft->getPropertyQualifier(data.value.cls,
               "PrimaryOwnerContact","MaxLen",NULL);
            fprintf(stderr,"Cloning\n");
            CMPIConstClass *clone=CMClone( data.value.cls,NULL); 
             fprintf(stderr,"Releasing\n");
           CMRelease(clone);  
            
	   count=data.value.cls->ft->getPropertyQualifierCount(data.value.cls,"NameFormat",NULL);
            fprintf(stderr,"Property Qualifier count(NameFormat): %d\n",count);
            qd=data.value.cls->ft->getPropertyQualifier(data.value.cls,
               "NameFormat","ValueMap",&status);
            showProperty(qd,"NameFormat - ValueMap"); 
        }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
    CMRelease(cc);
       
    return 0;
}
