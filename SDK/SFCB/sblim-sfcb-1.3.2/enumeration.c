
/*
 * enumeration.c
 *
 * (C) Copyright IBM Corp. 2005
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
 * CMPIEnumeration implementation.
 *
*/


#include "native.h"
#include "sfcbenum.h"

#ifdef LARGE_VOL_SUPPORT
#include <pthread.h>
inline static   void CMSetStatusWithMsg(CMPIStatus *st, CMPIrc rcp,char *string)
      { (st)->rc=(rcp); (st)->msg=(string); }

void enumLock(CMPIEnumeration *   );
void enumUnLock(CMPIEnumeration * );
static CMPIData __eft_getNext(const CMPIEnumeration * , CMPIStatus *);
static CMPIBoolean __eft_hasNext(const CMPIEnumeration * , CMPIStatus * );
#define TIMEOUTVALUE   2000
#define TIMEOUTVALUE2 20000
#endif

extern void adjustArrayElementRefCount(CMPIArray * array, int n);
static struct native_enum *__new_enumeration(int, CMPIArray *, CMPIStatus *);


/*****************************************************************************/

static CMPIStatus __eft_release(CMPIEnumeration * enumeration)
{
   struct native_enum *e = (struct native_enum *) enumeration;

   if (e->mem_state && e->mem_state != MEM_RELEASED) {
      e->data->ft->release(e->data);
      memUnlinkEncObj(e->mem_state);
      e->mem_state = MEM_RELEASED;
      free(enumeration);
      CMReturn(CMPI_RC_OK);
   }
   
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIEnumeration *__eft_clone(const CMPIEnumeration * enumeration,
                                    CMPIStatus * rc)
{
   CMPIStatus tmp;
   struct native_enum *e = (struct native_enum *) enumeration;
   CMPIArray *data = CMClone(e->data, &tmp);

   if (tmp.rc != CMPI_RC_OK) {
      if (rc) CMSetStatus(rc, CMPI_RC_ERR_FAILED);
      return NULL;
   }

   return (CMPIEnumeration *) __new_enumeration(MEM_NOT_TRACKED, data, rc);
}

#ifndef LARGE_VOL_SUPPORT

static CMPIData __eft_getNext(const CMPIEnumeration * enumeration, CMPIStatus * rc)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   return CMGetArrayElementAt(e->data, e->current++, rc);
}


static CMPIBoolean __eft_hasNext(const CMPIEnumeration * enumeration, CMPIStatus * rc)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   return (e->current < CMGetArrayCount(e->data, rc));
}
#endif

static CMPIArray *__eft_toArray(const CMPIEnumeration * enumeration, CMPIStatus * rc)
{
   struct native_enum *e = (struct native_enum *) enumeration;
	if ( rc ) CMSetStatus ( rc, CMPI_RC_OK );
   return e->data;
}

static CMPIEnumerationFT eft = {
   NATIVE_FT_VERSION,
   __eft_release,
   __eft_clone,
   __eft_getNext,
   __eft_hasNext,
   __eft_toArray
};

static struct native_enum *__new_enumeration(int mm_add,
                                             CMPIArray * array, CMPIStatus * rc)
{
   static CMPIEnumeration e = {
      "CMPIEnumeration",
      &eft
   };

   struct native_enum enm,*tEnm;
   int state;
   
   enm.enumeration = e;
   enm.current=0;
   enm.data=NULL;
   
   tEnm=memAddEncObj(mm_add, &enm, sizeof(enm), &state);
   tEnm->mem_state = state;
   tEnm->refCount=0;
#ifdef  LARGE_VOL_SUPPORT
   tEnm->enum_state=0;
   tEnm->last_valid=-1;
   if((pthread_mutex_init(&tEnm->enumlock,NULL)) != 0){
      CMSetStatusWithMsg(rc,CMPI_RC_ERR_FAILED," Failed pthread mutex init");
      return NULL;
  }
#endif
   tEnm->data = //(mm_add == MEM_NOT_TRACKED) ? CMClone(array, rc) : 
      array;
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return (struct native_enum*)tEnm;

}

void setEnumArray(CMPIEnumeration * enumeration,CMPIArray * array)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   e->data=array; 
}     


CMPIEnumeration *sfcb_native_new_CMPIEnumeration(CMPIArray * array, CMPIStatus * rc)
{
   return (CMPIEnumeration *) __new_enumeration(MEM_TRACKED, array, rc);
}

CMPIEnumeration *NewCMPIEnumeration(CMPIArray * array, CMPIStatus * rc)
{
   return (CMPIEnumeration *) __new_enumeration(MEM_NOT_TRACKED, array, rc);
}

/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
/* ************************************************************************************** */
#ifdef LARGE_VOL_SUPPORT
static CMPIData __eft_getNext(const CMPIEnumeration * enumeration, CMPIStatus * rc)
{
   CMPIData retData ;
   struct native_enum *e = (struct native_enum *) enumeration;
   enumLock(e) ;
                
   retData = CMGetArrayElementAt(e->data, e->current++, rc);
   enumUnLock(e) ;
   return(retData) ;
}


static CMPIBoolean __eft_hasNext(const CMPIEnumeration * enumeration, CMPIStatus * rc)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   int hasNextTO = 0 ;     /* timeout */
   int savelast  = 0 ;
   
   if(!(e->enum_state)){
      return (e->current < CMGetArrayCount(e->data, rc));
   } else {
   	
      /*  
       * wait here till we get some enumeration data 
       */ 
      hasNextTO = 0 ;
      while(e->enum_state == ENUM_INIT){
	       usleep(10000) ;
	       hasNextTO++ ;
		     if(hasNextTO > TIMEOUTVALUE){
		        CMSetStatus(rc,CMPI_RC_ERROR);
		        return(0);
		     }
	    }
   
      if(e->enum_state == ENUM_COMPLETE)
         return (e->current < CMGetArrayCount(e->data, rc));
   
      savelast = e->last_valid ;
      if(e->current <= e->last_valid){
         return (e->current < CMGetArrayCount(e->data, rc));
      } else {    	 
   	     hasNextTO = 0 ;
         while(savelast == e->last_valid){
	         usleep(1000) ;
	         hasNextTO++ ;
	         if(e->enum_state == ENUM_COMPLETE) break ;
	         if(hasNextTO > TIMEOUTVALUE2){
	            CMSetStatus(rc,CMPI_RC_ERROR);
	            return(0);
	         }
	       }
	       return (e->current < CMGetArrayCount(e->data, rc));     
      }
   }
}

void setEnumState(CMPIEnumeration * enumeration , int new_state)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   e->enum_state = new_state ; 	
}

int getEnumState(CMPIEnumeration * enumeration)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   return e->enum_state ; 	
}

CMPIArray * getEnumDatap(CMPIEnumeration * enumeration)
{
   struct native_enum *e = (struct native_enum *) enumeration;
   return e->data ; 	
}

void enumLock(CMPIEnumeration * enumeration )
{
   struct native_enum *e = (struct native_enum *) enumeration;
   pthread_mutex_lock(&e->enumlock) ;
  	
}

void enumUnLock(CMPIEnumeration * enumeration )
{
   struct native_enum *e = (struct native_enum *) enumeration;
   pthread_mutex_unlock(&e->enumlock) ;
   
}

void incLastValid(CMPIEnumeration * enumeration )
{
   struct native_enum *e = (struct native_enum *) enumeration;
   e->last_valid++ ;   	
}

int getLastValid(CMPIEnumeration * enumeration )
{
   struct native_enum *e = (struct native_enum *) enumeration;
   return(e->last_valid) ;   	
}
#endif

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
