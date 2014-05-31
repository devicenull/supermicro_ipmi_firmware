/*
 * test_leak.c
 *
 * (C) Copyright Intel Corp. 2005
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        David Howell <david.p.howell@intel.com>
 *
 * Description:
 *	Do a set of client library calls that work (abort if not) in
 *	a loop, getting the starting value of the heap top before the
 *      set and after. Loop over this many times to see if there is 
 *	any new growth of he heap between iterations, shouldn't be.
 *
 *	Enable verbose (below) for a call by call heap growth report.
 */
#include <cmci.h>
#include <native.h>
#include <unistd.h>
#include <stdlib.h>
#include "show.h"

#define NUM_ITERS	125
#define verbose		0

int main()
{
    CMCIClient		*cc;
    CMPIObjectPath	*objectpath, *objectpath2;
    CMPIEnumeration	*enumeration;
    CMPIInstance	*instance1;
    CMPIStatus		status;
    int			i;
    char		*cim_host, *cim_host_passwd, *cim_host_userid;
    void		*curbrk;

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

    objectpath = newCMPIObjectPath("root/iicmv1",
				   "IICM_MAPAdminDomain", NULL);
    CMAddKey(objectpath, "CreationClassName",
			 "IICM_MAPAdminDomain", CMPI_chars);
    CMAddKey(objectpath, "Name", "admin1", CMPI_chars);

    objectpath2 = newCMPIObjectPath( "root/iicmv1", "CIM_Slot", NULL);
    CMAddKey(objectpath2, "CreationClassName", "CIM_Slot", CMPI_chars);
    CMAddKey(objectpath2, "Tag", "IBM Asset Tag:0000004", CMPI_chars);

    for (i = 0; i < NUM_ITERS; i++) {
        curbrk = (void *)sbrk(0);

        instance1 = cc->ft->getInstance(cc, objectpath, 0, NULL, &status);
	if (status.rc != 0) {
    	    printf( "getInstance() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(instance1);
	if (verbose)
	    printf("after getInstance heap %p start %p\n", sbrk(0), curbrk);

	enumeration = cc->ft->enumInstances(cc, objectpath, 0, NULL, &status);
	if (status.rc != 0) {
    	    printf( "enumInstance() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(enumeration);
	if (verbose)
	    printf("after enumInstances heap %p start %p\n", sbrk(0), curbrk);

	
        enumeration = cc->ft->associators(cc, objectpath, 
                      "CIM_SystemComponent", NULL, NULL, NULL, 0, NULL, &status);
	if (status.rc != 0) {
    	    printf( "associators() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(enumeration);
	if (verbose)
	    printf("after associators heap %p start %p\n", sbrk(0), curbrk);

        enumeration = cc->ft->associatorNames( cc, 
            objectpath2,                    // The parent object?
            "CIM_PackageInSlot",            // assocClass
            NULL,                           // resultClass
            NULL,                           // role
            NULL,                           // resultRole
            &status);                       // return code
	if (status.rc != 0) {
    	    printf( "associatorNames() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(enumeration);
	if (verbose)
	    printf("after associatorNames heap %p start %p\n", sbrk(0), curbrk);

        enumeration = cc->ft->enumInstanceNames(cc, objectpath2, &status);
	if (status.rc != 0) {
    	    printf( "enumInstanceNames() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(enumeration);
	if (verbose)
	    printf("after enumInstanceNames heap %p start %p\n", sbrk(0), curbrk);

        enumeration = cc->ft->references(cc, objectpath, 
					 NULL, NULL, 0, NULL, &status);
	if (status.rc != 0) {
    	    printf( "references() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(enumeration);
	if (verbose)
	    printf("after references heap %p start %p\n", sbrk(0), curbrk);

        enumeration = cc->ft->referenceNames(cc, objectpath2, NULL, NULL, &status);
	if (status.rc != 0) {
    	    printf( "referenceNames() rc=%d, msg=%s\n", 
                    status.rc, (status.msg)? (char *)status.msg->hdl : NULL);
	     abort();
	}
	CMRelease(enumeration);
	if (verbose)
	    printf("after referenceNames heap %p start %p\n", sbrk(0), curbrk);

	if (curbrk != (void *)sbrk(0)) {
	     fprintf(stderr, "+++ heap top changed from %p to %p\n",
							curbrk, sbrk(0));
	}

	if (verbose || (i % 10) == 0)
	     printf ("+++ iteration %d complete, curbrk %p\n", i, curbrk);
    }

    if (objectpath) CMRelease(objectpath);
    if (objectpath2) CMRelease(objectpath2);
    if (status.msg) CMRelease(status.msg);
    if (cc) CMRelease(cc);
    
    return 0;
}

