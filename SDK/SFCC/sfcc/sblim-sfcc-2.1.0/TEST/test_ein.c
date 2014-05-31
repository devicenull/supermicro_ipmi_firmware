/*
 * test_ein.c
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
 *  Test for enumInstanceNames() library API.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "show.h"

int main()
{
    CMCIClient		* cc;
    CMPIObjectPath	* objectpath;
    CMPIEnumeration	* enumeration;
    CMPIStatus		status;
    char		*cim_host, *cim_host_passwd, *cim_host_userid;
    int i;

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
    cc = cmciConnect(cim_host, NULL, "5988",
			       cim_host_userid, cim_host_passwd, NULL);

    /* Test enumInstanceNames() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing enumInstanceNames() ...\n");
    //    objectpath = newCMPIObjectPath("root/iicmv1", "CIM_PhysicalPackage", NULL);
    objectpath = newCMPIObjectPath("root/cimv2", "CIM_Process", NULL);


    for (i=0; i< 10; i++) {
    enumeration = cc->ft->enumInstanceNames(cc, objectpath, &status);
    /* Print the results */
    printf( "enumInstanceNames() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) {
        printf("result(s):\n");
        while (enumeration->ft->hasNext(enumeration, NULL)) {
            CMPIData data = enumeration->ft->getNext(enumeration, NULL);
            showObjectPath(data.value.ref);
        }
    }
    }

    if (enumeration) CMRelease(enumeration);
    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
    if (status.msg) CMRelease(status.msg);
 
    return 0;
}
