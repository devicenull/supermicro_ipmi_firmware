
/*
 * $Id: context.c,v 1.12 2008/06/24 15:28:27 buccella Exp $
 *
 * © Copyright IBM Corp. 2005, 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:        Frank Scheffler
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPIContext implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native.h"


//! Forward declaration for anonymous struct.
struct native_property;


//! Function table for native_property handling functions.
/*!
  This structure holds all the function pointers necessary to handle
  linked lists of native_property structs.

  \sa propertyFT in native.h
*/
struct native_propertyFT {

   //! Adds a new native_property to a list.
   int (*addProperty) (struct native_property **,
                       int,
                       const char *, CMPIType, CMPIValueState, const CMPIValue *);

   //! Resets the values of an existing native_property, if existant.
   int (*setProperty) (struct native_property *,
                       int, const char *, CMPIType, CMPIValue *);

   //! Looks up a specifix native_property in CMPIData format.
    CMPIData(*getDataProperty) (struct native_property *,
                                const char *, CMPIStatus *);

   //! Extract an indexed native_property in CMPIData format.
    CMPIData(*getDataPropertyAt) (struct native_property *,
                                  unsigned int, CMPIString **, CMPIStatus *);

   //! Yields the number of native_property items in a list.
    CMPICount(*getPropertyCount) (struct native_property *, CMPIStatus *);

   //! Releases a complete list of native_property items.
   void (*release) (struct native_property *);
};



static struct native_propertyFT propertyFT;

//! Native extension of the CMPIContext data type.
/*!
  This structure stores the information needed to represent contexts for
  CMPI providers.
 */
struct native_context {
   CMPIContext ctx;             /*!< the inheriting data structure  */
   int mem_state;               /*!< states, whether this object is
                                   registered within the memory mangagement or
                                   represents a cloned object */
   struct native_property *entries;     /*!< context content */
   void *data;
};


static struct native_context *__new_empty_context(int);


/****************************************************************************/


static CMPIStatus __cft_release(CMPIContext * ctx)
{
   struct native_context *c = (struct native_context *) ctx;

   if (c->mem_state && c->mem_state != MEM_RELEASED) {
      propertyFT.release(c->entries);
      memUnlinkEncObj(c->mem_state);
      c->mem_state = MEM_RELEASED;
      free(ctx);
      CMReturn(CMPI_RC_OK);
   }
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIContext *__cft_clone(const CMPIContext * ctx, CMPIStatus * rc)
{
   if (rc)
      CMSetStatus(rc, CMPI_RC_ERR_NOT_SUPPORTED);
   return NULL;
}


static CMPIData __cft_getEntry(const CMPIContext * ctx,
                               const char *name, CMPIStatus * rc)
{
   struct native_context *c = (struct native_context *) ctx;

   return propertyFT.getDataProperty(c->entries, name, rc);
}


static CMPIData __cft_getEntryAt(const CMPIContext * ctx,
                                 unsigned int index,
                                 CMPIString ** name, CMPIStatus * rc)
{
   struct native_context *c = (struct native_context *) ctx;

   return propertyFT.getDataPropertyAt(c->entries, index, name, rc);
}


static unsigned int __cft_getEntryCount(const CMPIContext * ctx, CMPIStatus * rc)
{
   struct native_context *c = (struct native_context *) ctx;

   return propertyFT.getPropertyCount(c->entries, rc);
}


static CMPIStatus __cft_addEntry(const CMPIContext * ctx,
                                 const char *name,
                                 const CMPIValue * value, CMPIType type)
{
   struct native_context *c = (struct native_context *) ctx;
   
   CMReturn((propertyFT.addProperty(&c->entries,
                                    MEM_NOT_TRACKED,
                                    name,
                                    type,
                                    0,
                                    value)) ?
            CMPI_RC_ERR_ALREADY_EXISTS : CMPI_RC_OK);
}


static CMPIContextFT cft = {
   NATIVE_FT_VERSION,
   __cft_release,
   __cft_clone,
   __cft_getEntry,
   __cft_getEntryAt,
   __cft_getEntryCount,
   __cft_addEntry
};


static struct native_context *__new_empty_context(int mm_add)
{
   static CMPIContext c = {
      "CMPIContext",
      &cft
   };
   struct native_context ctx,*tCtx;
   int state;
   
   ctx.ctx = c;
   tCtx=memAddEncObj(mm_add, &ctx, sizeof(ctx), &state);
   tCtx->mem_state = state;   
   
   tCtx->entries=NULL;    
   tCtx->data=NULL;
   
   return (struct native_context*)tCtx;
}



CMPIContext *native_new_CMPIContext(int mem_state, void *data)
{
   struct native_context *ctx;
   ctx=__new_empty_context(mem_state);
   ctx->data=data;
   return (CMPIContext*)ctx; 
}


CMPIContext *native_clone_CMPIContext(const CMPIContext* ctx) 
{
   CMPIString *name;
   struct native_context *c = (struct native_context *) ctx;
   int i,s;
   CMPIContext *nCtx=native_new_CMPIContext(MEM_NOT_TRACKED,c->data);
   
   for (i=0,s=ctx->ft->getEntryCount(ctx,NULL); i<s; i++) {
      CMPIData data=ctx->ft->getEntryAt(ctx,i,&name,NULL);
      nCtx->ft->addEntry(nCtx,CMGetCharPtr(name),&data.value,data.type);
   }
   return nCtx;
}


//! Storage container for commonly needed data within native CMPI data types.
/*!
  This structure is used to build linked lists of data containers as needed
  for various native data types.
*/
struct native_property {
	char * name;		        //!< Property identifier.
	CMPIType type;		        //!< Associated CMPIType.
	CMPIValueState state; 	        //!< Current value state.
	CMPIValue value;	        //!< Current value.
	struct native_property * next;	//!< Pointer to next property.
};


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
			*propname  = sfcb_native_new_CMPIString ( prop->name,
								  NULL , 0);
		}

	} else {
	        result.type = CMPI_null;
		result.state = CMPI_nullValue;
	}

	return result;
}


/**
 * adds or replaces
 */
static int __addProperty ( struct native_property ** prop,
			   int mm_add,
			   const char * name,
			   CMPIType type,
			   CMPIValueState state, 
			   const CMPIValue * value )
{
	CMPIValue v;


	/* if it's new, add it to the end of the list */
	/* if it's an update, replace it in the list */
	if ( *prop == NULL || (strcmp ( (*prop)->name, name ) == 0) ) {
	   if (*prop == NULL) {
		 *prop = (struct native_property *)
			 calloc ( 1, sizeof ( struct native_property ) );
	   }
	   struct native_property * tmp = *prop; 
		  tmp->name = strdup ( name );

		  if ( type == CMPI_chars ) {

			  type = CMPI_string;
			  v.string = sfcb_native_new_CMPIString ( (char *) value,
								  NULL, 0 );
			  value = &v;
		  }

		  tmp->type  = type;

		  if ( type != CMPI_null ) {
			  tmp->state = state;

			  if ( mm_add == MEM_TRACKED ) {

			    sfcb_setAlignedValue(&tmp->value,value,type);
			  } else {

				  CMPIStatus rc;
				  tmp->value = sfcb_native_clone_CMPIValue ( type,
									value,
									&rc );
				  // what if clone() fails???
			  }
		  } else tmp->state = CMPI_nullValue;

		  return 0;
	}

	/* continue checking the list */
	return (  __addProperty ( &( (*prop)->next ), 
				 mm_add,
				 name, 
				 type,
				 state, 
				 value ) );
}


/**
 * returns -1 if non-existant
 */
static int __setProperty ( struct native_property * prop, 
			   int mm_add,
			   const char * name, 
			   CMPIType type,
			   CMPIValue * value )
{
	CMPIValue v;
	if ( prop == NULL ) {
		return -1;
	}

	if ( strcmp ( prop->name, name ) == 0 ) {

		CMPIStatus rc;

		if ( ! ( prop->state & CMPI_nullValue ) )
			sfcb_native_release_CMPIValue ( prop->type, &prop->value );

		if ( type == CMPI_chars ) {

			type = CMPI_string;
			v.string = sfcb_native_new_CMPIString ( (char *) value,
								NULL, 0 );
			value = &v;
		}

		prop->type  = type;

		if ( type != CMPI_null ) {
		  if (mm_add == MEM_TRACKED) {
		    sfcb_setAlignedValue(&prop->value,value,type);
		  } else {
		    prop->value = sfcb_native_clone_CMPIValue ( type, value, &rc );
		  }
		} else prop->state = CMPI_nullValue;

		return 0;
	}
	return __setProperty ( prop->next, mm_add, name, type, value);
}


static struct native_property * __getProperty ( struct native_property * prop, 
						const char * name )
{
	if ( ! prop || ! name ) {
		return NULL;
	}
	return ( strcmp ( prop->name, name ) == 0 )?
		prop: __getProperty ( prop->next, name );
}


static CMPIData __getDataProperty ( struct native_property * prop,
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_property * p = __getProperty ( prop, name );

	if ( rc ) CMSetStatus ( rc,
				( p )?
				CMPI_RC_OK:
				CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, NULL );
}


static struct native_property * __getPropertyAt
( struct native_property * prop, unsigned int pos )
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

	if ( rc ) CMSetStatus ( rc,
				( p )?
				CMPI_RC_OK:
				CMPI_RC_ERR_NO_SUCH_PROPERTY );

	return __convert2CMPIData ( p, propname );
}


static CMPICount __getPropertyCount ( struct native_property * prop,
				      CMPIStatus * rc )
{
	CMPICount c = 0;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );

	while ( prop != NULL ) {
		c++;
		prop = prop->next;
	}

	return c;
}


static void __release ( struct native_property * prop )
{
	struct native_property *np;
	for ( ; prop; prop = np ) {
	  np=prop->next;
		sfcb_native_release_CMPIValue ( prop->type, &prop->value );
		free ( prop->name );
		free ( prop );
	}
}



/**
 * Global function table to access native_property helper functions.
 */
static struct native_propertyFT propertyFT = {
	__addProperty,
	__setProperty,
	__getDataProperty,
	__getDataPropertyAt,
	__getPropertyCount,
	__release
};
