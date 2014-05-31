/*!
  \file instance.c
  \brief Native CMPIInstance implementation.

  This is the native CMPIInstance implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIInstance, however, it works independently from the management broker.
  
  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Frank Scheffler
  $Revision: 1.2 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "native.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif


/****************************************************************************/

static void __release_list ( char ** list )
{
	if ( list ) {

		char ** tmp = list;

		while ( *tmp ) free ( *tmp++ );
		free ( list );
	}
}


static char ** __duplicate_list ( char ** list )
{
	char ** result = NULL;
	
	if ( list ) {
		size_t size = 1;
		char ** tmp = list;

		while ( *tmp++ ) ++size;

		result = malloc ( size * sizeof ( char * ) );

		for ( tmp = result; *list; tmp++ )
			*tmp = strdup ( *list++ );
	}

	return result;
}


static int __contained_list ( char ** list, const char * name )
{
	if ( list ) {

		while ( *list )
			if ( strcasecmp ( *list++, name ) == 0 ) return 1;
	}
	return 0;
}


/****************************************************************************/


static CMPIStatus __ift_release ( CMPIInstance * instance )
{
	struct native_instance * i = (struct native_instance *) instance;

	if (i) {
	    if (i->classname) free(i->classname);
	    if (i->nameSpace) free(i->nameSpace);
	    __release_list ( i->property_list );
	    __release_list ( i->key_list );
            propertyFT.release(i->props);
            qualifierFT.release(i->qualifiers);
            free(i);
            CMReturn ( CMPI_RC_OK );
	}   
 
	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIInstance * __ift_clone ( CMPIInstance * instance, CMPIStatus * rc )
{
	struct native_instance * i   = (struct native_instance *) instance;
	struct native_instance * new = (struct native_instance *) 
			       calloc ( 1, sizeof ( struct native_instance ) );

	new->instance.ft=i->instance.ft;
        if (i->classname) new->classname     = strdup ( i->classname );
	if (i->nameSpace) new->nameSpace     = strdup ( i->nameSpace );
	new->property_list = __duplicate_list ( i->property_list );
	new->key_list      = __duplicate_list ( i->key_list );
	new->qualifiers    = qualifierFT.clone ( i->qualifiers, rc );
	new->props         = propertyFT.clone ( i->props, rc );

	return (CMPIInstance *) new;
}


static CMPIData __ift_getProperty ( CMPIInstance * instance,
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;

	return propertyFT.getDataProperty ( i->props, name, rc );
}


static CMPIData __ift_getPropertyAt ( CMPIInstance * instance, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;

	return propertyFT.getDataPropertyAt ( i->props, index, name, rc );
}


static unsigned int __ift_getPropertyCount ( CMPIInstance * instance, 
					     CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;
  
	return propertyFT.getPropertyCount ( i->props, rc );

}

static CMPIData __ift_getQualifier ( CMPIInstance * instance, 
				      const char * name,
				      CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;

	return qualifierFT.getDataQualifier ( i->qualifiers, name, rc );
}

static CMPIData __ift_getQualifierAt ( CMPIInstance * instance, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;

	return qualifierFT.getDataQualifierAt ( i->qualifiers, index, name, rc );
}

static unsigned int __ift_getQualifierCount ( CMPIInstance * instance, 
					     CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;
  
	return qualifierFT.getQualifierCount ( i->qualifiers, rc );
}

static CMPIData __ift_getPropertyQualifier ( CMPIInstance * instance, 
				      const char * pname, const char *qname,
				      CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;
	struct native_property *p=propertyFT.getProperty ( i->props, pname );

	if (p) return qualifierFT.getDataQualifier ( p->qualifiers, qname, rc );        
	CMSetStatus ( rc, CMPI_RC_ERR_NO_SUCH_PROPERTY );
}

static CMPIData __ift_getPropertyQualifierAt ( CMPIInstance * instance, 
				      const char * pname, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;
	struct native_property *p=propertyFT.getProperty ( i->props, pname );

	if (p) return qualifierFT.getDataQualifierAt ( p->qualifiers, index, name, rc );
	CMSetStatus ( rc, CMPI_RC_ERR_NO_SUCH_PROPERTY );
}

static unsigned int __ift_getPropertyQualifierCount ( CMPIInstance * instance, 
				             const char * pname, 
					     CMPIStatus * rc )
{
	struct native_instance * i = (struct native_instance *) instance;
	struct native_property *p=propertyFT.getProperty ( i->props, pname );
  
	if (p) return qualifierFT.getQualifierCount ( p->qualifiers, rc );
	CMSetStatus ( rc, CMPI_RC_ERR_NO_SUCH_PROPERTY );
}


static CMPIStatus __ift_setProperty ( CMPIInstance * instance,
				      const char * name,
				      CMPIValue * value,
				      CMPIType type )
{
	struct native_instance * i = (struct native_instance *) instance;

	if ( i->filtered == 0 ||
	     i->property_list == NULL ||
	     __contained_list ( i->property_list, name ) ||
	     __contained_list ( i->key_list, name ) ) {

		if ( propertyFT.setProperty ( i->props,
					      name, 
					      type,
					      value ) ) {
			propertyFT.addProperty ( &i->props, 
						 name, 
						 type, 
                                                 (value == NULL) ?
                                                    CMPI_nullValue : 0, 
						 value );
               }

	}
	CMReturn ( CMPI_RC_OK );
}


static CMPIObjectPath * __ift_getObjectPath ( CMPIInstance * instance,
					      CMPIStatus * rc )
{
	int j;
	struct native_instance * i = (struct native_instance *) instance;   

	CMPIObjectPath * cop   = newCMPIObjectPath ( i->nameSpace,
						     i->classname,
						     rc );

	if ( rc && rc->rc != CMPI_RC_OK )
		return NULL;

	j = propertyFT.getPropertyCount ( i->props, NULL );

	while ( j-- ) {
		CMPIStatus tmp;
		CMPIString * keyName;
		CMPIData d = propertyFT.getDataPropertyAt ( i->props,
							    j,   
							    &keyName,
							    &tmp );
		if ( d.state & CMPI_keyValue ) {
			CMAddKey ( cop,
				   CMGetCharPtr ( keyName ),
				   &d.value, 
				   d.type );
		}
		if (keyName) CMRelease(keyName);
	}
	return cop;
}


static CMPIStatus __ift_setPropertyFilter ( CMPIInstance * instance,
					    char ** propertyList,
					    char ** keys )
{  

	struct native_instance * i = (struct native_instance *) instance;

	if ( i->filtered ) {

		__release_list ( i->property_list );
		__release_list ( i->key_list );
	}

	i->filtered = 1;
	i->property_list = __duplicate_list ( propertyList );
	i->key_list      = __duplicate_list ( keys );

	CMReturn ( CMPI_RC_OK );
}   



CMPIInstance * native_new_CMPIInstance ( CMPIObjectPath * cop,
					 CMPIStatus * rc )
{
	static CMPIInstanceFT ift = {
		NATIVE_FT_VERSION,
		__ift_release,
		__ift_clone,
		__ift_getProperty,
		__ift_getPropertyAt,
		__ift_getPropertyCount,
		__ift_setProperty,
		__ift_getObjectPath,
		__ift_setPropertyFilter,
                __ift_getQualifier,
                __ift_getQualifierAt,
                __ift_getQualifierCount,
                __ift_getPropertyQualifier,
                __ift_getPropertyQualifierAt,
                __ift_getPropertyQualifierCount
	};
	static CMPIInstance i = {
		"CMPIInstance",
		&ift
	};

	struct native_instance * instance =
		(struct native_instance *) 
		calloc ( 1, sizeof ( struct native_instance ) );

	CMPIStatus tmp1, tmp2, tmp3;
	CMPIString * str;

	instance->instance     = i;

	if (cop) {
	   int j = CMGetKeyCount ( cop, &tmp1 );
    
      str = CMGetClassName ( cop, &tmp2 );
	   instance->classname = strdup(CMGetCharPtr ( str ));
      CMRelease(str);
      
      str = CMGetNameSpace ( cop, &tmp3 );
      instance->nameSpace = (str && str->hdl) ? strdup(CMGetCharPtr ( str )) : NULL;
      CMRelease(str);

	   if ( tmp1.rc != CMPI_RC_OK ||
	        tmp2.rc != CMPI_RC_OK ||
	        tmp3.rc != CMPI_RC_OK ) {
		   CMSetStatus ( rc, CMPI_RC_ERR_FAILED );
	   } 
           else {
	     while ( j-- && ( tmp1.rc == CMPI_RC_OK ) ) {
			CMPIString * keyName;
			CMPIData tmp = CMGetKeyAt ( cop, j, &keyName, &tmp1 );
			propertyFT.addProperty ( &instance->props,
						 CMGetCharPtr ( keyName ),
						 tmp.type,
						 tmp.state,
						 &tmp.value );
			if (keyName) CMRelease(keyName);
		}

                
		CMSetStatus ( rc, tmp1.rc );
	   }
        }   

	return (CMPIInstance *) instance;
}

void setInstNsAndCn(CMPIInstance *ci, char *ns, char *cn)
{
   struct native_instance * i = (struct native_instance *) ci;
   
   if (cn) i->classname=strdup(cn);
   if (ns) i->nameSpace=strdup(ns);
}

int addInstQualifier( CMPIInstance* ci, char * name,
				      CMPIValue * value,
				      CMPIType type)
{
   struct native_instance * i = (struct native_instance *) ci;

	if ( qualifierFT.setQualifier ( i->qualifiers,
				      name, 
				      type,
				      value ) ) {
		qualifierFT.addQualifier ( &i->qualifiers, 
					 name, 
					 type, 
					 0, 
					 value );
		}
                
	return ( CMPI_RC_OK );
}

int addInstPropertyQualifier( CMPIInstance* ci, char * pname, char *qname,
				      CMPIValue * value,
				      CMPIType type)
{
   struct native_instance * i = (struct native_instance *) ci;
   struct native_property *p=propertyFT.getProperty ( i->props ,pname );
   
   if (p) {
	if ( qualifierFT.setQualifier ( p->qualifiers,
				      qname, 
				      type,
				      value ) ) {
		qualifierFT.addQualifier ( &p->qualifiers, 
					 qname, 
					 type, 
					 0, 
					 value );
		}
                
	return ( CMPI_RC_OK );
   }
   return CMPI_RC_ERR_NO_SUCH_PROPERTY;
}                                      
                                      
                                      
/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
