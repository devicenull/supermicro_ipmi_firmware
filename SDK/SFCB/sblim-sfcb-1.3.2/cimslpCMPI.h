/*
 * cimslpCMPI.h
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
 * Functions getting slp relevant information from the CIMOM utilizing sfcc
 *
*/

#ifndef _cimslpCMPI_h
#define _cimslpCMPI_h

#include <cmci.h>
#include <unistd.h>

typedef struct {
	char * url_syntax;
	char * service_hi_name;
	char * service_hi_description;
	char * service_id;
	char * CommunicationMechanism;
	char * OtherCommunicationMechanismDescription;
	char ** InteropSchemaNamespace;
	char * ProtocolVersion;
	char ** FunctionalProfilesSupported;
	char ** FunctionalProfileDescriptions;
	char * MultipleOperationsSupported;
	char ** AuthenticationMechanismsSupported;
	char ** AuthenticationMechansimDescriptions;
	char ** Namespace;
	char ** Classinfo;
	char ** RegisteredProfilesSupported;
} cimSLPService;

typedef struct {
	char * commScheme; //http or https
	char * cimhost;
	char * port;
	char * cimuser;
	char * cimpassword;
	char * trustStore;
	char * certFile;
	char * keyFile;
} cimomConfig;

extern char *value2Chars(CMPIType type, CMPIValue * value);

void initializeService(cimSLPService  *rs);
cimSLPService getSLPData(cimomConfig cfg);
char * myGetProperty(CMPIInstance *instance, char * propertyName);
char ** myGetPropertyArray(CMPIInstance *instance, char *propertyName);
char ** myGetPropertyArrayFromArray(CMPIInstance **instances, char *propertyName);
CMPIInstance ** myGetInstances(CMCIClient *cc, char * path, char * objectname);
CMPIConstClass * myGetClass(CMCIClient *cc, char * path, char * objectname);
char * transformValue(char * cssf, CMPIConstClass * ccls, char * propertyName);
char ** transformValueArray(char ** cssf, CMPIConstClass * ccls, char * propertyName);
char ** myGetRegProfiles(CMPIInstance **instances, CMCIClient *cc);
char ** getInterOpNS();
char * getUrlSyntax(char* sn, char * cs, char * port);

#endif
