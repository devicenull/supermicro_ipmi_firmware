/*!
  \file enumeration.c
  \brief Native CMPIEnumeration implementation.

  This is the native CMPIEnumeration implementation as used for remote
  providers. It reflects the well-defined interface of a regular
  CMPIEnumeration, however, it works independently from the management broker.
  
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
*/

#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"

struct _CMCIConnectionFT;                           // new 
typedef struct _CMCIConnectionFT CMCIConnectionFT;  // new
#include <curl/curl.h>         // new
#include "cimXmlParser.h"      // new
#include "utilStringBuffer.h"  // new

#ifndef LARGE_VOL_SUPPORT

#include "native.h"
#include <time.h>              // new
#include <sys/time.h>          // new
#include "conn.h"              // new

#else 



#include "native.h"
#include <time.h>              // new
#include <sys/time.h>          // new
#include "esinfo.h"            // new
#include "conn.h"              // new

#endif

static CMPIStatus __eft_release ( CMPIEnumeration * );
static CMPIEnumeration * __eft_clone ( CMPIEnumeration * , CMPIStatus *);
static CMPIData __eft_getNext ( CMPIEnumeration * ,CMPIStatus * );
static CMPIArray * __eft_toArray ( CMPIEnumeration * ,CMPIStatus * );
static CMPIBoolean __eft_hasNext ( CMPIEnumeration * , CMPIStatus * );
static struct native_enum * __new_enumeration ( CMPIArray *, CMPIStatus * );

#ifdef DMALLOC
#include "dmalloc.h"
#endif

/*****************************************************************************/

static CMPIStatus __eft_release ( CMPIEnumeration * enumeration )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	CMPIStatus st;
	if (e) {
		if (e->data)
		  st = CMRelease(e->data);
		free ( enumeration );
		return st;
	}

	CMReturn ( CMPI_RC_ERR_FAILED );
}


static CMPIEnumeration * __eft_clone ( CMPIEnumeration * enumeration,
				       CMPIStatus * rc )
{
	CMPIStatus tmp;
	struct native_enum * e = (struct native_enum *) enumeration;
	CMPIArray * data       = CMClone ( e->data, &tmp );

	if ( tmp.rc != CMPI_RC_OK ) {
		CMSetStatus ( rc, CMPI_RC_ERR_FAILED );
		return NULL;
	}

	return 
		(CMPIEnumeration *) __new_enumeration ( data, rc );
}


static CMPIData __eft_getNext ( CMPIEnumeration * enumeration,
				CMPIStatus * rc )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	return CMGetArrayElementAt ( e->data, e->current++, rc );
}

#ifndef LARGE_VOL_SUPPORT

static CMPIBoolean __eft_hasNext ( CMPIEnumeration * enumeration,
				   CMPIStatus * rc )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	return ( e->current < CMGetArrayCount ( e->data, rc ) );
}

#endif

static CMPIArray * __eft_toArray ( CMPIEnumeration * enumeration,
				   CMPIStatus * rc )
{
	struct native_enum * e = (struct native_enum *) enumeration;
	CMSetStatus ( rc, CMPI_RC_OK );
	return e->data;
}


static struct native_enum * __new_enumeration ( CMPIArray * array,
						CMPIStatus * rc )
{
	static const CMPIEnumerationFT eft = {
		NATIVE_FT_VERSION,
		__eft_release,
		__eft_clone,
		__eft_getNext,
		__eft_hasNext,
		__eft_toArray
	};
	static const CMPIEnumeration e = {
		"CMPIEnumeration",
		(CMPIEnumerationFT*)&eft
	};

	struct native_enum * enumeration = (struct native_enum *)
		calloc ( 1, sizeof ( struct native_enum ) );

  enumeration->enumeration = e;
  enumeration->data = array; 	/* CMClone ( array, rc ) ? */
#ifdef LARGE_VOL_SUPPORT
  enumeration->econ = NULL ;
  enumeration->ecop = NULL ;
#endif
	CMSetStatus ( rc, CMPI_RC_OK );
	return enumeration;
}


CMPIEnumeration * native_new_CMPIEnumeration ( CMPIArray * array,
					       CMPIStatus * rc )
{
	return (CMPIEnumeration *) __new_enumeration ( array, rc );
}


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#ifdef LARGE_VOL_SUPPORT
#define TIMEOUTVALUE 2000
static CMPIBoolean __eft_hasNext ( CMPIEnumeration * enumeration,
				   CMPIStatus * rc )
{
	int hasNextTO = 0 ;     /* timeout */
	struct native_enum * e = (struct native_enum *) enumeration;
	
  if(e->econ){ 
	   CMCIConnection *con = e->econ ;
	
	   /*
	    * need to be cautious here because the array might not be 
	    * allocated yet , so we pause till we are past PARSTATE_INIT.
	    * cycle "roughly" 20 seconds ?? if we didn't get past 
	    * PARSTATE_INIT , something is wrong.   
	    */
		
	    while(con->asynRCntl.escanInfo.parsestate == PARSTATE_INIT){
	       usleep(10000) ;
	       hasNextTO++ ;
		     if(hasNextTO > TIMEOUTVALUE){
		     	  CMSetStatus(rc,CMPI_RC_ERROR);
		 	      return(0);
		     }
	    }
	             
      /*
       * if we caught up with the parsing (current > or = ArrayCount) 
       * then we delay a bit if we haven't reached PARSTATE_COMPLETE.
       * if parsing sees the server timeout , also exit so we don't hang here 
       * forever.
       *
       */
      hasNextTO = 0 ;
      if(hasNextTO < TIMEOUTVALUE){
         if(((CMGetArrayCount ( e->data, rc )) <= (e->current)) && 
      	                con->asynRCntl.escanInfo.parsestate != PARSTATE_COMPLETE)  {
            while((CMGetArrayCount ( e->data, rc )) <= (e->current)){
  	           usleep(10000) ;
	             hasNextTO++ ;
		           if(hasNextTO > TIMEOUTVALUE){
		           	  CMSetStatus(rc,CMPI_RC_ERROR);
		 	            return(0);
		           }
		           if((con->asynRCntl.escanInfo.parsestate == PARSTATE_COMPLETE)||
		           	  (con->asynRCntl.escanInfo.parsestate == PARSTATE_SERVER_TIMEOUT)){
		              CMSetStatus(rc,CMPI_RC_ERROR);
		           	 	return(0) ;
		           } 
            }
         }
      }
  }
                     
	return ( e->current < CMGetArrayCount ( e->data, rc ) );

}

#endif
/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
