/*!
  \file string.c
  \brief Native CMPIString implementation.

  This is the native CMPIString implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIString, however, it works independently from the management broker.
  
  It is part of a native broker implementation that simulates CMPI data
  types rather than interacting with the entities in a full-grown CIMOM.

  (C) Copyright IBM Corp. 2003
 
  THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE 
  ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE 
  CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 
  You can obtain a current copy of the Eclipse Public License from
  http://www.opensource.org/licenses/eclipse-1.0.php

  \author Frank Scheffler
  $Revision: 1.3 $

  \todo Once CMGetCharPtr() macro uses the appropriate function call instead
  of casting the internal hdl, store "CMPIString" type in there.
*/

#include <stdlib.h>
#include <string.h>
#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "native.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif

//CMPIString *strTab[5000];
//int strTabNext=0;

struct native_string {
	CMPIString string;
};

static struct native_string * __new_string ( const char *, CMPIStatus * );


/*****************************************************************************/

static CMPIStatus __sft_release ( CMPIString * string )
{
	struct native_string * s = (struct native_string *) string;
int i;
//	for (i=0; i<strTabNext; i++) if (strTab[i]==string) { strTab[i]=NULL; break; }
        if ( s ) {

		if (s->string.hdl != NULL)
		    free ( s->string.hdl );
		free ( s );

		CMReturn ( CMPI_RC_OK );
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIString * __sft_clone ( CMPIString * string, CMPIStatus * rc )
{
	return (CMPIString * )
		__new_string ( string->ft->getCharPtr ( string, rc ), rc );
}


static char * __sft_getCharPtr ( CMPIString * string, CMPIStatus * rc )
{
	return (char *) string->hdl;
}


static struct native_string * __new_string ( const char * ptr, 
					     CMPIStatus * rc )
{
	static CMPIStringFT const sft = {
		NATIVE_FT_VERSION,
		__sft_release,
		__sft_clone,
		__sft_getCharPtr
	};

	struct native_string * string =
		(struct native_string *)
		calloc ( 1, sizeof ( struct native_string ) );

        string->string.hdl = ( ptr )? strdup ( ptr ): NULL;
	string->string.ft  = (CMPIStringFT*) &sft;

	CMSetStatus ( rc, CMPI_RC_OK );
	return string;
}


CMPIString * native_new_CMPIString ( const char * ptr, CMPIStatus * rc )
{
	return (CMPIString *) __new_string ( ptr, rc );
}


/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
