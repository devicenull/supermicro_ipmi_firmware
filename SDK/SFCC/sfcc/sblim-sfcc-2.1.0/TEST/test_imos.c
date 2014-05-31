/*
 * test_imos.c
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
 *  Test for invokeMethod() library API. Tests OSBase Method Provider
 *  Note: This test requires that the cmpi-base provider be installed 
 *        and registered on your CIMOM.
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
    CMPIStatus		status;
    CMPIArgs		* args, * outargs; 
    char 		*cim_host, *cim_host_passwd, *cim_host_userid;
    CMPIData		retval, outval;
    char 		*arg_chars;

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

    printf("\n----------------------------------------------------------\n");
    printf("Testing invokeMethod() ...\n");

    objectpath = newCMPIObjectPath("root/cimv2", "Linux_OperatingSystem", NULL);
    CMAddKey(objectpath, "CreationClassName", "Linux_OperatingSystem", CMPI_chars);
    CMAddKey(objectpath, "Name", "linux", CMPI_chars);

/*---------------------------------------------------------------------*/

    printf("+++T1:passing IN chars argument\n");

    args = newCMPIArgs(NULL);
    outargs = newCMPIArgs(NULL);
    arg_chars = "ls";
    args->ft->addArg(args, "cmd", (CMPIValue*)arg_chars, CMPI_chars);

    retval = cc->ft->invokeMethod(
        cc, objectpath, "execCmd", args, outargs, &status);

    /* Print the results */
    printf( "invokeMethod() rc=%d, msg=%s\n", 
            status.rc, (status.msg)? (char *)status.msg->hdl : NULL);

    outval=CMGetArg(outargs,"out",NULL);
    if (!status.rc) {
      if (outval.value.chars) {
        char *cv = value2Chars(outval.type,&(outval.value));
        printf("result(s):\n\tout value:%s\n", cv);
        if (cv != NULL) free(cv);    
      }
    }

    if (args) CMRelease(args);
    if (outargs) CMRelease(outargs);
    if (status.msg) CMRelease(status.msg);

    if (!status.rc) {
        char *cv = value2Chars(retval.type,&(retval.value));
        printf("result(s):\n\treturn value:%s\n", cv);
        if (cv != NULL) free(cv);
    }
    else
        goto done;


done:
    if (objectpath) CMRelease(objectpath);
    if (cc) CMRelease(cc);
  
    return 0;
}

