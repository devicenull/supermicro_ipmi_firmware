/*
 * test_sp.c
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
 *  Test for setProperty() library API. 
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
    CMPIValue value;
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

    /* Test setProperty() */
    printf("\n----------------------------------------------------------\n");
    printf("Testing setProperty() ...\n");
    objectpath = newCMPIObjectPath("root/iicmv1", "CIM_PhysicalPackage", NULL);
    CMAddKey(objectpath, "CreationClassName", "CIM_PhysicalPackage", CMPI_chars);
    CMAddKey(objectpath, "Tag", "IBM Asset Tag:0000002", CMPI_chars);

    value.string = newCMPIString("Sorta Fast Fan", NULL);
    status = cc->ft->setProperty( cc, objectpath, "Model", 
                                  &value, CMPI_chars);

    /* Print the results */
    printf( "setProperty() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    if (objectpath) CMRelease(objectpath);
    if (value.string) CMRelease(value.string);
    if (status.msg) CMRelease(status.msg);
    if (cc) CMRelease(cc);

    return 0;
}

