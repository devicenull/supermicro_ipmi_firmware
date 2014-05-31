/*
 * test_ci.c
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
 *  Test for createInstance() library API. 
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "show.h"

static char * _HOSTNAME = "bestorga.ibm.com";

int main()
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath, *objectpath_r;
    CMPIInstance * instance;
    CMPIStatus status;
    char hostName[512];
    char 	*cim_host, *cim_host_passwd, *cim_host_userid;

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
   
    gethostname(hostName,511);
    _HOSTNAME=strdup(hostName);

    /* Test createInstance() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing createInstance() ...\n");
    objectpath = newCMPIObjectPath("root/cimv2", "CWS_Authorization", NULL);
    instance = newCMPIInstance(objectpath, NULL);
    CMSetProperty(instance, "Username", "bestorga", CMPI_chars);
    CMSetProperty(instance, "Classname", "foobar", CMPI_chars);

    objectpath_r = cc->ft->createInstance(cc, objectpath, instance, &status);

    /* Print the results */
    printf( "createInstance() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) {
        printf("result:\n");
        showObjectPath(objectpath_r);
    }

    if (instance) CMRelease(instance);
    if (objectpath) CMRelease(objectpath);
    if (objectpath_r) CMRelease(objectpath_r);
    if (status.msg) CMRelease(status.msg);
    if (cc) CMRelease(cc);
    if (_HOSTNAME) free(_HOSTNAME);
    
    return 0;
}
