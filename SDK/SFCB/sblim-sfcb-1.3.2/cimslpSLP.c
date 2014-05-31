/*
 * cimslpSLP.c
 *
 * (C) Copyright IBM Corp. 2006
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Sven Schuetz <sven@de.ibm.com>
 * Contributions:
 *
 * Description:
 *
 * Functions for slp regs/deregs
 *
*/

#include <slp.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cimslpCMPI.h"
#include "cimslpSLP.h"
#include "cimslpUtil.h"
#include "trace.h"


#define SIZE 1024

int size;
char * gAttrstring = "NULL";
char* urlsyntax=NULL;

void freeCSS(cimSLPService css)
{
	freeStr(css.url_syntax);
	freeStr(css.service_hi_name);
	freeStr(css.service_hi_description);
	freeStr(css.service_id);
	freeStr(css.CommunicationMechanism);
	freeStr(css.OtherCommunicationMechanismDescription);
	freeArr(css.InteropSchemaNamespace);    
	freeStr(css.ProtocolVersion);
	freeArr(css.FunctionalProfilesSupported);
	freeArr(css.FunctionalProfileDescriptions);
	freeStr(css.MultipleOperationsSupported);
	freeArr(css.AuthenticationMechanismsSupported);
	freeArr(css.AuthenticationMechansimDescriptions);
	freeArr(css.Namespace);
	freeArr(css.Classinfo);
	freeArr(css.RegisteredProfilesSupported);
}

void onErrorFnc(SLPHandle hslp, SLPError errcode, void* cookie)
{
    *(SLPError*)cookie = errcode;

	if(errcode) {
		printf("Callback Code %i\n",errcode);
	}
}


char* buildAttrString(char * name, char * value, char * attrstring)
{
	int length;
	
	if(value == NULL) {
		return attrstring;
	}

	length = strlen(attrstring) + strlen(value) + strlen(name) + 5;

	if(length > size) {
		//make sure that string is big enough to hold the result
		//multiply with 3 so that we do not have to enlarge the next run already
		size = size + (length * 3);
		attrstring = (char *) realloc(attrstring, size * sizeof(char));
	}

	if(strlen(attrstring) != 0) {
		strcat(attrstring, ",");
	}

	sprintf(attrstring, "%s(%s=%s)",attrstring, name, value);

	return attrstring;

}


char* buildAttrStringFromArray(char * name, char ** value, char * attrstring)
{
	int length=0;
	int i;


	if(value == NULL) {
		return attrstring;
	}

	for(i = 0; value[i] != NULL; i++) {
		length += strlen(value[i]);
	}

	length = length + strlen(attrstring) + strlen(name) + 5;

	if(length > size) {
		//make sure that string is big enough to hold the result
		//multiply with 3 so that we do not have to enlarge the next run already
		size = size + (length * 3);
		attrstring = (char *) realloc(attrstring, size * sizeof(char));
	}

	if(strlen(attrstring) != 0) {
		strcat(attrstring, ",");
	}

	strcat(attrstring, "(");
	strcat(attrstring, name);
	strcat(attrstring, "=");
	for(i = 0; value[i] != NULL; i++) {
		strcat(attrstring, value[i]);
		strcat(attrstring, ",");
	}
	attrstring[strlen(attrstring) - 1] = '\0';
	strcat(attrstring, ")");

	return attrstring;

}

void deregisterCIMService()
{
	SLPHandle hslp;
	SLPError callbackerr = 0;
	SLPError err = 0;
	
	err = SLPOpen("", SLP_FALSE, &hslp);
	if(err != SLP_OK)
	{
		printf("Error opening slp handle %i\n",err);
	}	
	err = SLPDereg( hslp, urlsyntax, onErrorFnc, &callbackerr);
    if(( err != SLP_OK) || (callbackerr != SLP_OK)) {
        printf("--- Error deregistering service with slp (%i) ... it will now timeout\n",err);
    }
    SLPClose(hslp);
}

int registerCIMService(cimSLPService css, int slpLifeTime)
{
	SLPHandle hslp;
	SLPError err = 0;
	SLPError callbackerr = 0;
	char *attrstring;
	int retCode = 0;
	
	_SFCB_ENTER(TRACE_SLP, "registerCIMService");
	
	size = SIZE;
	
	if(! css.url_syntax) {
		freeCSS(css);
		return 1;
	} else {
		/*
		 * We have 2x urlsyntax:
		 * css.url_syntax, which is just the url, e.g.
		 *  http://somehost:someport
		 * urlsyntax, which is the complete service string as required by slp, e.g.
		 *  service:wbem:http://somehost:someport
		 */
		freeStr(urlsyntax);
		urlsyntax = (char*)malloc(strlen(css.url_syntax) + 14);//("service:wbem:" = 13) + \0
		sprintf(urlsyntax, "service:wbem:%s", css.url_syntax);
	}
	
	attrstring = malloc(sizeof(char) * SIZE);
	attrstring[0] = 0;

	attrstring = buildAttrString("template-type",
								"wbem", attrstring);
	attrstring = buildAttrString("template-version",
								"1.0", attrstring);
	attrstring = buildAttrString("template-description",
								"This template describes the attributes used for advertising WBEM Servers.", attrstring);
	attrstring = buildAttrString("template-url-syntax",
								css.url_syntax, attrstring);								
	attrstring = buildAttrString("service-hi-name",
								css.service_hi_name, attrstring);
	attrstring = buildAttrString("service-hi-description",
								css.service_hi_description, attrstring);
	attrstring = buildAttrString("service-id",
								css.service_id, attrstring);
	attrstring = buildAttrString("CommunicationMechanism",
								css.CommunicationMechanism, attrstring);
	attrstring = buildAttrString("OtherCommunicationMechanismDescription",
								css.OtherCommunicationMechanismDescription, attrstring);
	attrstring = buildAttrStringFromArray("InteropSchemaNamespace",
								css.InteropSchemaNamespace, attrstring);
	attrstring = buildAttrString("ProtocolVersion",
								css.ProtocolVersion, attrstring);
	attrstring = buildAttrStringFromArray("FunctionalProfilesSupported",
								css.FunctionalProfilesSupported, attrstring);
	attrstring = buildAttrStringFromArray("FunctionalProfileDescriptions",
								css.FunctionalProfileDescriptions, attrstring);
	attrstring = buildAttrString("MultipleOperationsSupported",
								css.MultipleOperationsSupported, attrstring);
	attrstring = buildAttrStringFromArray("AuthenticationMechanismsSupported",
								css.AuthenticationMechanismsSupported, attrstring);
	attrstring = buildAttrStringFromArray("AuthenticationMechansimDescriptions",
								css.AuthenticationMechansimDescriptions, attrstring);
	attrstring = buildAttrStringFromArray("Namespace",
								css.Namespace, attrstring);
	attrstring = buildAttrStringFromArray("Classinfo",
								css.Classinfo, attrstring);
	attrstring = buildAttrStringFromArray("RegisteredProfilesSupported",
								css.RegisteredProfilesSupported, attrstring);

	err = SLPOpen("", SLP_FALSE, &hslp);
	if(err != SLP_OK)
	{
		printf("Error opening slp handle %i\n",err);
		retCode = err;
	}
	
	if(strcmp(gAttrstring, attrstring)) {
		/*attrstring changed - dereg the service, and rereg with the new attributes
		actually, this is also true for the first run, as it changed from NULL to some value.
		Normally, this should be run only once if nothing changes. Then now further
		re/dreg is necessary. That's why I check if gAttrstring is "NULL" before
		deregging, as it would dereg a not existing service otherwise and probably return
		an error code
		*/
		if(strcmp(gAttrstring, "NULL")) {
			err = SLPDereg( hslp, urlsyntax, onErrorFnc, &callbackerr);
			free(gAttrstring);
		}
	}	
    err = SLPReg( hslp,
              urlsyntax,
              slpLifeTime,
              NULL,
              attrstring,
              SLP_TRUE,
              onErrorFnc,
              &callbackerr );

	#ifdef HAVE_SLP_ALONE
	printf("url_syntax: %s\n", css.url_syntax);
	printf("attrsting: %s\n", attrstring);
	#endif
	
    if(( err != SLP_OK) || (callbackerr != SLP_OK)) {
        printf("Error registering service with slp %i\n",err);
        retCode = err;
    }

    if( callbackerr != SLP_OK) {
        printf("Error registering service with slp %i\n",callbackerr);
        retCode = callbackerr;
    }
	
    //only save the last state when something changed to not waste mallocs
    if (strcmp(attrstring, gAttrstring)) {
    	gAttrstring = strdup(attrstring);
    }

	free(attrstring);
	freeCSS(css);
	
    SLPClose(hslp);
    
    _SFCB_RETURN(retCode);
}
