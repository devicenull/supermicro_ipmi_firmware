/*!
  \file native.h
  \brief Header file for the native encapsulated CMPI data type implementation.

  This file defines all the data types and functions necessary to use
  native encapsulated CMPI data objects. These are clones of the regular
  CMPI data types like CMPIObjectPath, CMPIInstance etc., however, they
  can be instantiated and manipulated without a full blown CIMOM. Instead,
  they use an autononmous CIMOM clone that provides all the functions
  to create these objects as defined by the CMPIBrokerEncFT.

  (C) Copyright IBM Corp. 2003

  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.

  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Frank Scheffler
  $Revision: 1.2 $
*/

#ifndef _REMOTE_CMPI_NATIVE_DATA_H
#define _REMOTE_CMPI_NATIVE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define NATIVE_FT_VERSION 1

#include "cmcidt.h"
#include "cmcift.h"
#include "cmci.h"
// #include "cimXmlParser.h"


/* ************************************************** */
/*                                                    */
/* ************************************************** */

#ifndef LARGE_VOL_SUPPORT 
struct native_enum {
	CMPIEnumeration enumeration;

	CMPICount current;
	CMPIArray * data;
};
#else
struct native_enum {
	CMPIEnumeration enumeration;

	CMPICount current;
	CMPIArray * data;
	CMCIConnection * econ;
	CMPIObjectPath * ecop;
};
#endif

//! Forward declaration for anonymous struct.
struct native_property;
struct native_qualifier;

struct native_constClass {
	CMPIConstClass ccls;

	char * classname;

	struct native_property * props;
        struct native_qualifier *qualifiers;
};

struct native_instance {
	CMPIInstance instance;

	char * classname;
	char * nameSpace;

	int filtered;
	char ** property_list;
	char ** key_list;

	struct native_property * props;
        struct native_qualifier *qualifiers;
};

struct native_property {
	char * name;		                //!< Property identifier.
	CMPIType type;		                //!< Associated CMPIType.
	CMPIValueState state; 	                //!< Current value state.
	CMPIValue value;	                //!< Current value.
	struct native_qualifier *qualifiers;	//!< Qualifiers.
	struct native_property * next;	        //!< Pointer to next property.
};


//! Function table for native_property handling functions.
/*!
  This structure holds all the function pointers necessary to handle
  linked lists of native_property structs.

  \sa propertyFT in native.h
*/
struct native_propertyFT 
{
        //! Function table version
        int ftVersion;

	//! Releases a complete list of native_property items.
	void (* release) ( struct native_property * );

	//! Clones a complete list of native_property items.
	struct native_property * (* clone) ( struct native_property *,
					     CMPIStatus * );
                                             
	//! Adds a new native_property to a list.
	struct native_property* (* getProperty) ( struct native_property *,
			      const char *);

	//! Adds a new native_property to a list.
	int (* addProperty) ( struct native_property **,
			      const char *,
			      CMPIType,
			      CMPIValueState,
			      CMPIValue * );

	//! Resets the values of an existing native_property, if existant.
	int (* setProperty) ( struct native_property *,
			      const char *,
			      CMPIType,
			      CMPIValue * );

	//! Looks up a specifix native_property in CMPIData format.
	CMPIData (* getDataProperty) ( struct native_property *,
				       const char *,
				       CMPIStatus * );

	//! Extract an indexed native_property in CMPIData format.
	CMPIData (* getDataPropertyAt) ( struct native_property *,
					 unsigned int,
					 CMPIString **,
					 CMPIStatus * );

	//! Yields the number of native_property items in a list.
	CMPICount (* getPropertyCount) ( struct native_property *,
					 CMPIStatus * );

	//! Looks up a specifix native_property and return qualifier chain.
	struct native_qualifier *(*__getDataPropertyQualifiers ) ( struct native_property *,
				       const char *,
				       CMPIStatus * );

};


struct native_qualifierFT 
{
        //! Function table version
        int ftVersion;
        
	//! Releases a complete list of native_qualifier items.
	void (* release) ( struct native_qualifier * );

	//! Clones a complete list of native_qualifier items.
	struct native_qualifier * (* clone) ( struct native_qualifier *,
					     CMPIStatus * );
                                             
	//! Adds a new native_qualifier to a list.
	int (* addQualifier) ( struct native_qualifier **,
			      const char *,
			      CMPIType,
			      CMPIValueState,
			      CMPIValue * );

	//! Resets the values of an existing native_qualifier, if existant.
	int (* setQualifier) ( struct native_qualifier *,
			      const char *,
			      CMPIType,
			      CMPIValue * );

	//! Looks up a specifix native_qualifier in CMPIData format.
	CMPIData (* getDataQualifier) ( struct native_qualifier *,
				       const char *,
				       CMPIStatus * );

	//! Extract an indexed native_qualifier in CMPIData format.
	CMPIData (* getDataQualifierAt) ( struct native_qualifier *,
					 unsigned int,
					 CMPIString **,
					 CMPIStatus * );

	//! Yields the number of native_qualifier items in a list.
	CMPICount (* getQualifierCount) ( struct native_qualifier *,
					 CMPIStatus * );
};

extern int addInstQualifier( CMPIInstance* ci, char * name,
				      CMPIValue * value,
				      CMPIType type);


/****************************************************************************/

void native_release_CMPIValue ( CMPIType, CMPIValue * val );
CMPIValue native_clone_CMPIValue ( CMPIType, CMPIValue * val, CMPIStatus * );
CMPIString * native_new_CMPIString ( const char *, CMPIStatus * );
CMPIArray * native_new_CMPIArray ( CMPICount size,
				   CMPIType type,
				   CMPIStatus * );
void native_array_increase_size ( CMPIArray *, CMPICount );
CMPIEnumeration * native_new_CMPIEnumeration ( CMPIArray *, CMPIStatus * );
CMPIInstance * native_new_CMPIInstance ( CMPIObjectPath *, CMPIStatus * );
CMPIObjectPath * native_new_CMPIObjectPath ( const char *,
					     const char *,
					     CMPIStatus * );
CMPIArgs * native_new_CMPIArgs ( CMPIStatus * );
CMPIDateTime * native_new_CMPIDateTime ( CMPIStatus * );
CMPIDateTime * native_new_CMPIDateTime_fromBinary ( CMPIUint64,
						    CMPIBoolean,
						    CMPIStatus * );
CMPIDateTime * native_new_CMPIDateTime_fromChars ( const char *,
						   CMPIStatus * );

struct xtokValueReference;
CMPIValue str2CMPIValue(CMPIType type, char *val, struct xtokValueReference *ref);
void setInstNsAndCn(CMPIInstance *ci, char *ns, char *cn);
CMPIStatus simpleArrayAdd(CMPIArray * array, CMPIValue * val, CMPIType type);
const char *getNameSpaceChars(CMPIObjectPath * cop);
CMPIValue *getKeyValueTypePtr(char *type, char *value, struct xtokValueReference *ref,
                              CMPIValue * val, CMPIType * typ);

#define newCMPIString native_new_CMPIString
#define newCMPIObjectPath native_new_CMPIObjectPath
#define newCMPIInstance native_new_CMPIInstance
#define newCMPIArray native_new_CMPIArray
#define newCMPIEnumeration native_new_CMPIEnumeration
#define newCMPIArgs native_new_CMPIArgs
/****************************************************************************/

extern struct native_propertyFT const propertyFT;
extern struct native_qualifierFT const qualifierFT;

#ifdef __cplusplus
 }
#endif

#endif

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
