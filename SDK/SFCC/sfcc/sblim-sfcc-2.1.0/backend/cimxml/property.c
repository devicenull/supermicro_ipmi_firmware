/*!
  \file property.c
  \brief Native property implementation.

  This module implements a native property, which is not public to any
  provider programmer. It is used to implement various other data types
  natively, such as instances, object-paths and args.

  It provides means to maintain linked lists of named properties including
  functionality to add, remove, clone and release them.

  (C) Copyright IBM Corp. 2003

  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.

  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Frank Scheffler
  $Revision: 1.4 $
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

//! Storage container for commonly needed data within native CMPI data types.
/*!
  This structure is used to build linked lists of data containers as needed
  for various native data types.
*/
/****************************************************************************/

static CMPIData __convert2CMPIData ( struct native_property * prop,
				     CMPIString ** propname )
{
	CMPIData result;

	if ( prop != NULL ) {
		result.type  = prop->type;
		result.state = prop->state;
		result.value = prop->value;

		if ( propname ) {
			*propname  = native_new_CMPIString ( prop->name,
							     NULL );
		}

	} else {
		result.state = CMPI_nullValue;
	}

	return result;
}


/**
 * returns non-zero if already existant
 */
static int __addProperty ( struct native_property ** prop,
			   const char * name,
			   CMPIType type,
			   CMPIValueState state,
			   CMPIValue * value )
{
   CMPIStatus rc;

   if ( *prop == NULL ) {
      struct native_property * tmp = *prop =
         (struct native_property *) calloc ( 1, sizeof ( struct native_property ) );

      tmp->qualifiers = NULL;
      tmp->name = strdup ( name );
      tmp->type  = type;
      tmp->state = state;
      
      if ( type != CMPI_null && state != CMPI_nullValue) {
         if ( type == CMPI_chars ) {
            tmp->type = CMPI_string;
            tmp->value.string = native_new_CMPIString ( (char *) value, &rc );
         }
         else tmp->value = native_clone_CMPIValue ( type, value, &rc );
      }      
      else {
         tmp->state = CMPI_nullValue;
         tmp->value.uint64=0;
      }

      return 0;
   }
   
   return ( strcasecmp ( (*prop)->name, name ) == 0 ||
          __addProperty ( &( (*prop)->next ), name, type, state, value ) );
}


/**
 * returns -1 if non-existant
 */
static int __setProperty ( struct native_property * prop,
			   const char * name,
			   CMPIType type,
			   CMPIValue * value )
{
   CMPIStatus rc;
   
   if ( prop == NULL ) return -1;

   if ( strcasecmp ( prop->name, name ) == 0 ) {

      if ( ! ( prop->state & CMPI_nullValue ) )
         native_release_CMPIValue ( prop->type, &prop->value );

      prop->type  = type;
      if ( type == CMPI_chars ) {
         prop->type = CMPI_string;
         prop->value.string = native_new_CMPIString ( (char *) value, &rc );
      }

      else { 
         if ( type != CMPI_null && value != NULL) 
             prop->value = native_clone_CMPIValue ( type, value, &rc );
         else prop->state = CMPI_nullValue;
      }
      return 0;
   }
   return __setProperty ( prop->next, name, type, value);
}


static struct native_property * __getProperty ( struct native_property * prop,
						const char * name )
{
	if ( ! prop || ! name ) {
		return NULL;
	}
	return ( strcasecmp ( prop->name, name ) == 0 )?
		prop: __getProperty ( prop->next, name );
}


static CMPIData __getDataProperty ( struct native_property * prop,
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_property * p = __getProperty ( prop, name );

	CMSetStatus( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, NULL );
}

static struct native_qualifier *__getDataPropertyQualifiers ( struct native_property * prop,
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_property * p = __getProperty ( prop, name );

	CMSetStatus( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return p ? p->qualifiers : NULL;
}


static struct native_property * __getPropertyAt( struct native_property * prop, 
unsigned int pos )
{
	if ( ! prop ) {
		return NULL;
	}

	return ( pos == 0 )?
		prop: __getPropertyAt ( prop->next, --pos );
}


static CMPIData __getDataPropertyAt ( struct native_property * prop,
				      unsigned int pos,
				      CMPIString ** propname,
				      CMPIStatus * rc )
{
	struct native_property * p = __getPropertyAt ( prop, pos );

	CMSetStatus ( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, propname );
}


static CMPICount __getPropertyCount ( struct native_property * prop,
				      CMPIStatus * rc )
{
	CMPICount c = 0;

	CMSetStatus ( rc, CMPI_RC_OK );

	while ( prop != NULL ) {
		c++;
		prop = prop->next;
	}

	return c;
} 


static void __release ( struct native_property * prop )
{
	struct native_property * next;
	for ( ; prop; prop = next ) {
		free ( prop->name );
 		native_release_CMPIValue ( prop->type, &prop->value );
                qualifierFT.release(prop->qualifiers);
                next=prop->next;
		free ( prop );
 	}
}


static struct native_property * __clone ( struct native_property * prop,
					  CMPIStatus * rc )
{
	struct native_property * result;
	CMPIStatus tmp;

	if ( prop == NULL ) {

		CMSetStatus ( rc, CMPI_RC_OK );
		return NULL;
	}

	result = (struct native_property * )
		 calloc ( 1, sizeof ( struct native_property ) );

	result->name  = strdup ( prop->name );
	result->type  = prop->type;
	result->state = prop->state;
	result->value = native_clone_CMPIValue ( prop->type,
						 &prop->value,
						 &tmp );

	if ( tmp.rc != CMPI_RC_OK ) {

		result->state = CMPI_nullValue;
	}

	result->qualifiers    = qualifierFT.clone ( prop->qualifiers, rc );
        
	result->next  = __clone ( prop->next, rc );
	return result;
}


/**
 * Global function table to access native_property helper functions.
 */
struct native_propertyFT const propertyFT = {
        NATIVE_FT_VERSION,
	__release,
	__clone,
        __getProperty,
	__addProperty,
	__setProperty,
	__getDataProperty,
	__getDataPropertyAt,
	__getPropertyCount,
        __getDataPropertyQualifiers
};

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
