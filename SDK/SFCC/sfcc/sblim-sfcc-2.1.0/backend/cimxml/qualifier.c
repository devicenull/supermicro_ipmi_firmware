/*!
  \file qualifier.c
  \brief Native qualifier implementation.

  This module implements a native qualifier, which is not public to any
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
  $Revision: 1.3 $
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
struct native_qualifier {
	char * name;		        //!< Qualifier identifier.
	CMPIType type;		        //!< Associated CMPIType.
	CMPIValueState state; 	        //!< Current value state.
	CMPIValue value;	        //!< Current value.
	struct native_qualifier * next;	//!< Pointer to next qualifier.
};


/****************************************************************************/

static CMPIData __convert2CMPIData ( struct native_qualifier * qual,
				     CMPIString ** qualname )
{
	CMPIData result;

	if ( qual != NULL ) {
		result.type  = qual->type;
		result.state = qual->state;
		result.value = qual->value;

		if ( qualname ) {
			*qualname  = native_new_CMPIString ( qual->name,
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
static int __addQualifier ( struct native_qualifier ** qual,
			   const char * name,
			   CMPIType type,  
			   CMPIValueState state, 
			   CMPIValue * value )
{
   CMPIStatus rc;
   
   if ( *qual == NULL ) {
      struct native_qualifier * tmp = *qual =
         (struct native_qualifier *) calloc ( 1, sizeof ( struct native_qualifier ) );

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
   
   return ( strcasecmp ( (*qual)->name, name ) == 0 ||
          __addQualifier ( &( (*qual)->next ), name, type, state, value ) );
}


/**
 * returns -1 if non-existant
 */
static int __setQualifier ( struct native_qualifier * qual, 
			   const char * name, 
			   CMPIType type,
			   CMPIValue * value )
{
   CMPIStatus rc;

   if ( qual == NULL ) return -1;

   if ( strcasecmp ( qual->name, name ) == 0 ) {

      if ( ! ( qual->state & CMPI_nullValue ) )
         native_release_CMPIValue ( qual->type, &qual->value );

      qual->type  = type;
      if ( type == CMPI_chars ) {
         qual->type = CMPI_string;
         qual->value.string = native_new_CMPIString ( (char *) value, &rc );
      }
      else { 
         if ( type != CMPI_null ) 
             qual->value = native_clone_CMPIValue ( type, value, &rc );
         else qual->state = CMPI_nullValue;
      }
      return 0;
   }
   return __setQualifier ( qual->next, name, type, value);
   
}


static struct native_qualifier * __getQualifier ( struct native_qualifier * qual, 
						const char * name )
{
	if ( ! qual || ! name ) {
		return NULL;
	}
	return ( strcasecmp ( qual->name, name ) == 0 )?
		qual: __getQualifier ( qual->next, name );
}


static CMPIData __getDataQualifier ( struct native_qualifier * qual, 
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_qualifier * p = __getQualifier ( qual, name );

	CMSetStatus( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_FAILED );

	return __convert2CMPIData ( p, NULL );
}


static struct native_qualifier * __getQualifierAt ( struct native_qualifier * qual, 
                   unsigned int pos )
{
	if ( ! qual ) {
		return NULL;
	} 

	return ( pos == 0 )?
		qual: __getQualifierAt ( qual->next, --pos );
}


static CMPIData __getDataQualifierAt ( struct native_qualifier * qual, 
				      unsigned int pos,
				      CMPIString ** qualname,
				      CMPIStatus * rc )
{
	struct native_qualifier * p = __getQualifierAt ( qual, pos );

	CMSetStatus( rc, ( p ) ? CMPI_RC_OK : CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, qualname );
}


static CMPICount __getQualifierCount ( struct native_qualifier * qual,
				      CMPIStatus * rc )
{
	CMPICount c = 0;

	CMSetStatus ( rc, CMPI_RC_OK );

	while ( qual != NULL ) {
		c++;
		qual = qual->next;
	}

	return c;
}


static void __release ( struct native_qualifier * qual )
{
        struct native_qualifier *next; 
        for ( ; qual; qual = next ) {
		free ( qual->name );
		native_release_CMPIValue ( qual->type, &qual->value );
                next = qual->next;
		free ( qual );
	}
}


static struct native_qualifier * __clone ( struct native_qualifier * qual,
					  CMPIStatus * rc )
{
	struct native_qualifier * result;
	CMPIStatus tmp;

	if ( qual == NULL ) {
		CMSetStatus ( rc, CMPI_RC_OK );
		return NULL;
	}

	result = (struct native_qualifier * ) 
		 calloc ( 1, sizeof ( struct native_qualifier ) );

	result->name  = strdup ( qual->name );
	result->type  = qual->type;
	result->state = qual->state;
	result->value = native_clone_CMPIValue ( qual->type,
						 &qual->value,
						 &tmp );

	if ( tmp.rc != CMPI_RC_OK ) {

		result->state = CMPI_nullValue;
	}
  
	result->next  = __clone ( qual->next, rc );
	return result;
}


/**
 * Global function table to access native_qualifier helper functions.
 */
struct native_qualifierFT const qualifierFT = {
        NATIVE_FT_VERSION,
	__release,
	__clone,
	__addQualifier,
	__setQualifier,
	__getDataQualifier,
	__getDataQualifierAt,
	__getQualifierCount
};



/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
