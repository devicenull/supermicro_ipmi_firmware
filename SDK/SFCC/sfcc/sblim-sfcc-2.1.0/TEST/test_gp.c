/*
 * test_gp.c
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
 *  Test for getProperty() library API. 
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "show.h"

int main()
{
    CMCIClient *cc;
    CMPIObjectPath * objectpath;
    CMPIStatus status;
    CMPIData   data;
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

    /* Test getProperty() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing getProperty() ...\n");
    objectpath = newCMPIObjectPath("root/cimv2", "Linux_ComputerSystem", NULL);
    CMAddKey(objectpath, "CreationClassName", "Linux_ComputerSystem", CMPI_chars);
    CMAddKey(objectpath, "Name", "localhost.localdomain", CMPI_chars);

    data = cc->ft->getProperty(cc, objectpath, "ElementName", &status);
    /* Print the results */
    printf( "getProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) 
    {
        printf("result(s):\n");
        printf("ElementName=%s\n", (char*)(data.value.string)->hdl);
	/*	native_release_CMPIValue(data.type,&data.value);*/
    }

    data = cc->ft->getProperty(cc, objectpath, "Width", &status);
    /* Print the results */
    printf( "getProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
    if (!status.rc) 
    {
        printf("result(s):\n");
        printf("Width=%s\n", (char*)(data.value.string)->hdl);
	/*	native_release_CMPIValue(data.type,&data.value);*/
    }

    if (objectpath) CMRelease(objectpath);
    if (status.msg) CMRelease(status.msg);
    if (cc) CMRelease(cc);
  
    return 0;
}

