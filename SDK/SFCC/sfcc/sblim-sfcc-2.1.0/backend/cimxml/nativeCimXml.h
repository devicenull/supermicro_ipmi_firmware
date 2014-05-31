
/*
 * $\Id$
 *
 * © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Sven Schuetz <sven@de.ibm.com>
 *
 * Description:   native objects of the cimxml backend
 *
 * 
 *
 */
 
#ifndef _NATIVECIMXML_H_
#define _NATIVECIMXML_H_

#define NATIVECIMXML_FT_VERSION 1

#include "cimc.h"

CIMCIndicationListener *newCIMCIndicationListener (int sslMode,
                                                   int *portNumber,
                                                   void (*fp) (CIMCInstance *indInstance),
                                                   CIMCStatus *rc);


struct native_indicationlistener {
	CIMCIndicationListener il;

	int sslMode;
	int port;
    
    void (*sendIndicationInstance) (CIMCInstance *indInstance);
};


#endif
