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

    /* Setup a connection to the CIMOM */
    cim_host = getenv("CIM_HOST");
    if (cim_host == NULL)
    {
	cim_host = "localhost";
    }

    cim_host_userid = getenv("CIM_HOST_USERID");
    if (cim_host_userid == NULL)
    {
	cim_host_userid = "root";
    }

    cim_host_passwd = getenv("CIM_HOST_PASSWD");
    if (cim_host_passwd == NULL)
    {
	cim_host_passwd = "password";
    }

    cc = cmciConnect(cim_host, NULL, "5988",
			       cim_host_userid, cim_host_passwd, NULL);
   
    /* Test enumInstances() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumInstances() with qualifiers...\n");
    objectpath = newCMPIObjectPath("root/cimv2", "CIM_ComputerSystem", NULL);
    enumeration = cc->ft->enumInstances(cc, objectpath,
                                        CMPI_FLAG_IncludeQualifiers,
                                        NULL, &status);

    /* Print the results */
    printf( "enumInstances() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc)
    {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL))
        {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);

            showInstance(data.value.inst);

            count = data.value.inst->ft->getQualifierCount(
                data.value.inst,NULL);

            fprintf(stderr,"Qualifier count: %d\n",count);

            qd = data.value.inst->ft->getQualifier(
                data.value.inst, "Description",NULL);

            qd = data.value.inst->ft->getPropertyQualifier(
                data.value.inst, "NameFormat","ValueMap",NULL);

            showProperty(qd, "NameFormat - ValueMap");

            fprintf(stderr,"Cloning\n");
            CMPIInstance *clone=CMClone( data.value.inst,NULL); 
             fprintf(stderr,"Releasing\n");
           CMRelease(clone);  
            
            qd = data.value.inst->ft->getPropertyQualifier(
                data.value.inst, "NameFormat","Values",&status);

            showProperty(qd, "NameFormat - Values");
        }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
    CMRelease(cc);
       
    return 0;
}
