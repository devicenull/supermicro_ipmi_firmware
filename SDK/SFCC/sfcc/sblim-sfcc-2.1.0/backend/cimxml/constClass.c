/*!
  \file constClass.c
  \brief Native CMPIConstClass implementation.

  This is the native CMPIConstClass implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIConstClass, however, it works independently from the management broker.
  
  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Adrian Schuur

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

extern CMPIConstClass * native_new_CMPIConstClass ( char  *cn,
						    CMPIStatus * rc );
extern int addClassProperty( CMPIConstClass * ccls, char * name,
			     CMPIValue * value, CMPIType type,
					  CMPIValueState state);

/****************************************************************************/


static CMPIStatus __ccft_release ( CMPIConstClass * ccls )
{
	struct native_constClass * cc = (struct native_constClass *) ccls;

	if ( cc ) {

		free ( cc->classname );
		propertyFT.release ( cc->props );
		qualifierFT.release ( cc->qualifiers );
		free ( cc );

		CMReturn ( CMPI_RC_OK );
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIConstClass * __ccft_clone ( CMPIConstClass * ccls, CMPIStatus * rc )
{
	struct native_constClass * cc   = (struct native_constClass *) ccls;
	struct native_constClass * new = (struct native_constClass *) 
		calloc ( 1, sizeof ( struct native_constClass ) );

	new->ccls      = cc->ccls;
	new->classname = strdup ( cc->classname );
	new->props     = propertyFT.clone ( cc->props, rc );

	return (CMPIConstClass *) new;
}


static CMPIString *__ccft_getClassName ( CMPIConstClass * ccls,
				    CMPIStatus * rc )
{
	struct native_constClass * cc = (struct native_constClass *) ccls;

	return native_new_CMPIString ( cc->classname, NULL );
}


static CMPIData __ccft_getProperty ( CMPIConstClass * ccls,
				    const char * name,
				    CMPIStatus * rc )
{
	struct native_constClass * cc = (struct native_constClass *) ccls;

	return propertyFT.getDataProperty ( cc->props, name, rc );
}

static CMPIData __ccft_getPropertyAt ( CMPIConstClass * ccls, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_constClass * cc = (struct native_constClass *) ccls;

	return propertyFT.getDataPropertyAt ( cc->props, index, name, rc );
}


static unsigned int __ccft_getPropertyCount ( CMPIConstClass * ccls, 
					     CMPIStatus * rc )
{
	struct native_constClass * cc = (struct native_constClass *) ccls;
  
	return propertyFT.getPropertyCount ( cc->props, rc );
}


static CMPIData __ccft_getQualifier ( CMPIConstClass * ccls, 
				      const char * name,
				      CMPIStatus * rc )
{
	struct native_constClass * c = (struct native_constClass *) ccls;

	return qualifierFT.getDataQualifier ( c->qualifiers, name, rc );
}

static CMPIData __ccft_getQualifierAt ( CMPIConstClass * ccls, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_constClass * c = (struct native_constClass *) ccls;

	return qualifierFT.getDataQualifierAt ( c->qualifiers, index, name, rc );
}

static unsigned int __ccft_getQualifierCount ( CMPIConstClass * ccls, 
					     CMPIStatus * rc )
{
	struct native_constClass * c = (struct native_constClass *) ccls;
  
	return qualifierFT.getQualifierCount ( c->qualifiers, rc );
}

static CMPIData __ccft_getPropertyQualifier ( CMPIConstClass * ccls, 
				      const char * pname, const char *qname,
				      CMPIStatus * rc )
{
	struct native_constClass * c = (struct native_constClass *) ccls;
	struct native_property *p=propertyFT.getProperty ( c->props, pname );

	if (p) return qualifierFT.getDataQualifier ( p->qualifiers, qname, rc );        
	CMSetStatus ( rc, CMPI_RC_ERR_NO_SUCH_PROPERTY );
}

static CMPIData __ccft_getPropertyQualifierAt ( CMPIConstClass * ccls, 
				      const char * pname, 
				      unsigned int index,
				      CMPIString ** name,
				      CMPIStatus * rc )
{
	struct native_constClass * c = (struct native_constClass *) ccls;
	struct native_property *p=propertyFT.getProperty ( c->props, pname );

	if (p) return qualifierFT.getDataQualifierAt ( p->qualifiers, index, name, rc );
	CMSetStatus ( rc, CMPI_RC_ERR_NO_SUCH_PROPERTY );
}

static unsigned int __ccft_getPropertyQualifierCount ( CMPIConstClass * ccls, 
				             const char * pname, 
					     CMPIStatus * rc )
{
	struct native_constClass * c = (struct native_constClass *) ccls;
	struct native_property *p=propertyFT.getProperty ( c->props, pname );
  
	if (p) return qualifierFT.getQualifierCount ( p->qualifiers, rc );
	CMSetStatus ( rc, CMPI_RC_ERR_NO_SUCH_PROPERTY );
}





CMPIConstClass * native_new_CMPIConstClass ( char  *cn, CMPIStatus * rc )
{
	static CMPIConstClassFT ccft = {
		NATIVE_FT_VERSION,
		__ccft_release,
		__ccft_clone,
		__ccft_getClassName,
		__ccft_getProperty,
		__ccft_getPropertyAt,
		__ccft_getPropertyCount,
                __ccft_getQualifier,
                __ccft_getQualifierAt,
                __ccft_getQualifierCount,
                __ccft_getPropertyQualifier,
                __ccft_getPropertyQualifierAt,
                __ccft_getPropertyQualifierCount
	};
	static CMPIConstClass cc = {
		"CMPIConstClass",
		&ccft
	};

	struct native_constClass * ccls =
		(struct native_constClass *) 
		calloc ( 1, sizeof ( struct native_constClass ) );

	ccls->ccls = cc;

	ccls->classname = strdup (cn );

	return (CMPIConstClass *) ccls;
}


int addClassProperty( CMPIConstClass * ccls,
				      char * name,
				      CMPIValue * value,
				      CMPIType type, 
				      CMPIValueState state)
{
	struct native_constClass * cc = (struct native_constClass *) ccls;


	if ( propertyFT.setProperty ( cc->props,
				      name, 
				      type,
				      value ) ) {
		propertyFT.addProperty ( &cc->props, 
					 name, 
					 type, 
					 state, 
					 value );
	}

	return ( CMPI_RC_OK );
}

int addClassQualifier( CMPIConstClass* cc, char * name,
				      CMPIValue * value,
				      CMPIType type)
{
   struct native_constClass * c = (struct native_constClass *) cc;

	if ( qualifierFT.setQualifier ( c->qualifiers,
				      name, 
				      type,
				      value ) ) {
		qualifierFT.addQualifier ( &c->qualifiers, 
					 name, 
					 type, 
					 0, 
					 value );
		}
                
	return ( CMPI_RC_OK );
}

int addClassPropertyQualifier( CMPIConstClass* cc, char * pname, char *qname,
				      CMPIValue * value,
				      CMPIType type)
{
   struct native_constClass * c = (struct native_constClass *) cc;
   struct native_property *p=propertyFT.getProperty ( c->props ,pname );
   
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
