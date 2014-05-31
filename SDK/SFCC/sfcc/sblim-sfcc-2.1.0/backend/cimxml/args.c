/*!
  \file args.c
  \brief Native CMPIArgs implementation.

  This is the native CMPIArgs implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIArgs object, however, it works independently from the management broker.

  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  (C) Copyright IBM Corp. 2003
  (C) Copyright Intel Corp. 2003

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
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "native.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

//! Native extension of the CMPIArgs data type.
/*!
  This structure stores the information needed to represent arguments for
  CMPI providers, i.e. within invokeMethod() calls.
 */
struct native_args {
	CMPIArgs args;	        /*!< the inheriting data structure  */
	struct native_property * data;	/*!< argument content */
};


static struct native_args * __new_empty_args ( CMPIStatus * );

/****************************************************************************/


static CMPIStatus __aft_release ( CMPIArgs * args )
{
	struct native_args * a = (struct native_args *) args;

	if ( a ) {

		propertyFT.release ( a->data );
		free ( a );

		CMReturn ( CMPI_RC_OK );
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIArgs * __aft_clone ( CMPIArgs * args, CMPIStatus * rc )
{
	struct native_args * a   = (struct native_args *) args;
	struct native_args * new = __new_empty_args ( rc );

	if ( rc->rc == CMPI_RC_OK ) {
		new->data = propertyFT.clone ( a->data, rc );
	}

	return (CMPIArgs *) new;
}


static CMPIStatus __aft_addArg ( CMPIArgs * args,
				 const char * name,
				 CMPIValue * value,
				 CMPIType type )
{
	struct native_args * a = (struct native_args *) args;

	CMReturn ( ( propertyFT.addProperty ( &a->data,
					      name,
					      type,
					      0,
					      value ) )?
		   CMPI_RC_ERR_ALREADY_EXISTS:
		   CMPI_RC_OK );
}


static CMPIData __aft_getArg ( CMPIArgs * args,
			       const char * name,
			       CMPIStatus * rc )
{
	struct native_args * a = (struct native_args *) args;

	return propertyFT.getDataProperty ( a->data, name, rc );
}


static CMPIData __aft_getArgAt ( CMPIArgs * args,
				 unsigned int index,
				 CMPIString ** name,
				 CMPIStatus * rc )
{
	struct native_args * a = (struct native_args *) args;

	return propertyFT.getDataPropertyAt ( a->data, index, name, rc );
}


static unsigned int __aft_getArgCount ( CMPIArgs * args, CMPIStatus * rc )
{
	struct native_args * a = (struct native_args *) args;

	return propertyFT.getPropertyCount ( a->data, rc );
}


static struct native_args * __new_empty_args ( CMPIStatus * rc )
{
	static const CMPIArgsFT aft = {
		NATIVE_FT_VERSION,
		__aft_release,
		__aft_clone,
		__aft_addArg,
		__aft_getArg,
		__aft_getArgAt,
		__aft_getArgCount
	};
	static CMPIArgs a = {
		"CMPIArgs",
		(CMPIArgsFT*)&aft
	};

	struct native_args * args = (struct native_args *)
		calloc ( 1, sizeof ( struct native_args ) );

	args->args      = a;
        args->data = 0;

	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
	return args;
}


CMPIArgs * native_new_CMPIArgs ( CMPIStatus * rc )
{
	return (CMPIArgs *) __new_empty_args ( rc );
}

/*****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
