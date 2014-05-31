
/*
 * $Id: array.c,v 1.21 2008/04/14 18:42:34 gdread Exp $
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
 * Author:       Frank Scheffler 
 * Contributors: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 * CMPIArray implementation.

  * In contrast to a regular array, there exists an additional increase()
  * method, which is only used by the native CMPIResult implementation to
  * grow an array stepwise.
 *
*/


#include <stdlib.h>
#include <string.h>

#include "native.h"
#include "array.h"
#include "objectImpl.h"

extern CMPIObjectPath *getObjectPath(char *path, char **msg);
extern const char *ClObjectGetClString(ClObjectHdr * hdr, ClString * id);
extern int localClientMode;

static struct native_array *__new_empty_array(int, CMPICount,
                                              CMPIType, CMPIStatus *);


/*****************************************************************************/

static void __make_NULL(struct native_array *a, int from, int to, int release)
{
   for (; from <= to; from++) {
      a->data[from].state = CMPI_nullValue;

      if (release) {
         sfcb_native_release_CMPIValue(a->type, &a->data[from].value);
      }
   }
}


static CMPIStatus __aft_release(CMPIArray * array)
{
   struct native_array *a = (struct native_array *) array;

   if (a->mem_state && a->mem_state != MEM_RELEASED) {
      int i = a->size;
      if (a->mem_state==MEM_NOT_TRACKED) while (i--) {
	      if (!(a->data[i].state & CMPI_nullValue) && a->refCount==0) {
		      sfcb_native_release_CMPIValue(a->type, &a->data[i].value);
	      }
      }
      memUnlinkEncObj(a->mem_state);
      a->mem_state = MEM_RELEASED;
      if (a->data) free(a->data);
      free(array);
      CMReturn(CMPI_RC_OK);
   }
   
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIArray *__aft_clone(const CMPIArray * array, CMPIStatus * rc)
{
   CMPIStatus tmp;
   struct native_array *a = (struct native_array *) array;
   struct native_array *new = __new_empty_array(MEM_NOT_TRACKED,
                                                a->size,
                                                a->type,
                                                &tmp);

   int i = a->size;
   while (i-- && tmp.rc == CMPI_RC_OK) {
      new->data[i].state = a->data[i].state;
      if (!(new->data[i].state & CMPI_nullValue)) {

         new->data[i].value =
             sfcb_native_clone_CMPIValue(a->type, &a->data[i].value, &tmp);
      }
   }

   if (rc)
      CMSetStatus(rc, tmp.rc);

   return (CMPIArray *) new;
}


static CMPICount __aft_getSize(const CMPIArray * array, CMPIStatus * rc)
{
   struct native_array *a = (struct native_array *) array;

   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return a->size;
}


static CMPIType __aft_getSimpleType(const CMPIArray * array, CMPIStatus * rc)
{
   struct native_array *a = (struct native_array *) array;

   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return a->type;
}


static CMPIData __aft_getElementAt(const CMPIArray * array,
                                   CMPICount index, CMPIStatus * rc)
{
   struct native_array *a = (struct native_array *) array;

   CMPIData result = { a->type, CMPI_badValue, {0} };

   if (index < a->size) {

      result.state = a->data[index].state;
      result.value = a->data[index].value;
   }

   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return result;
}

static CMPIStatus setElementAt ( CMPIArray * array, CMPICount index, const CMPIValue * val,
       CMPIType type, int opt )
{
   struct native_array * a = (struct native_array *) array;

   if ( a->dynamic && index==a->size ) {
      sfcb_native_array_increase_size(array, 1); 
   }

   if ( index < a->size ) {
      CMPIValue v;

      if ( type == CMPI_chars && a->type == CMPI_string ) {
	 if (val) {
	     v.string = sfcb_native_new_CMPIString ( (char *) val, NULL, 0 );
	     type = CMPI_string;
	     val  = &v;
	 } else {
	     type = CMPI_null;
	 }
      }

      if ((type & (CMPI_ENC|CMPI_ARRAY)) && (val==NULL || val->chars==NULL)) {
	 type = CMPI_null;
      }

      if ( opt || type == a->type ) {
         CMPIStatus rc = {CMPI_RC_OK, NULL};
         if ( ! ( a->data[index].state & CMPI_nullValue ) ) {
            __make_NULL ( a, index, index, a->mem_state == MEM_NOT_TRACKED );
         }

	 if (opt) {
	     sfcb_setAlignedValue(&(a->data[index].value),val,type);
	 } else if (a->mem_state != MEM_NOT_TRACKED) {
	     sfcb_setAlignedValue(&(a->data[index].value),val,type);
	 } else {
	     a->data[index].value = 
		 sfcb_native_clone_CMPIValue ( type, val, &rc );
	 }
	 if (localClientMode) switch (a->type) {
	 case CMPI_instance:
	 case CMPI_ref:
	 case CMPI_class:
	 case CMPI_string:
		 ((nativeEncObject*)a->data[index].value.inst)->refCount++;
	 default: ;
	 }
	 if (rc.rc == CMPI_RC_OK) {
		 a->data[index].state = 0;
	 }		 
	 return rc;
      }

      if ( type == CMPI_null ) {
         if ( ! ( a->data[index].state & CMPI_nullValue ) ) {
            __make_NULL ( a, index, index, a->mem_state == MEM_NOT_TRACKED );
         }
         CMReturn ( CMPI_RC_OK );
      }
   }
   CMReturn ( CMPI_RC_ERR_FAILED );
}

static CMPIStatus __aft_setElementAt (  CMPIArray * array, CMPICount index, const CMPIValue * val,
          CMPIType type )
{ 
   return setElementAt(array,index,val,type,0);
}

CMPIStatus arraySetElementNotTrackedAt(CMPIArray * array,
                                     CMPICount index,
                                     CMPIValue * val, CMPIType type)
{
   struct native_array *a = (struct native_array *) array;

   if (index < a->size) {
      CMPIValue v;

      if (type == CMPI_chars && a->type == CMPI_string) {
	 v.string = sfcb_native_new_CMPIString((char *) val, NULL, 0);
         type = CMPI_string;
         val = &v;
      }

      if (type == a->type) {
         CMPIStatus rc = { CMPI_RC_OK, NULL };
         a->data[index].state = 0;
	 sfcb_setAlignedValue(&(a->data[index].value),val,type);
         if (localClientMode) {
            switch (a->type) {
            case CMPI_instance:
            case CMPI_ref:
            case CMPI_class:
            case CMPI_string:
               ((nativeEncObject*)a->data[index].value.inst)->refCount++;
            default: ;
            }
         }
         return rc;
      }

      if (type == CMPI_null) {
         if (!(a->data[index].state & CMPI_nullValue)) {
            __make_NULL(a, index, index, a->mem_state == MEM_NOT_TRACKED);
         }
         CMReturn(CMPI_RC_OK);
      }
   }

   CMReturn(CMPI_RC_ERR_FAILED);
}



void sfcb_native_array_increase_size(const CMPIArray * array, CMPICount increment)
{
   struct native_array *a = (struct native_array *) array;

   if ((a->size+increment)>a->max) {
      if (a->size==0) a->max=8;
      else while ((a->size+increment)>a->max) a->max*=2;
      a->data = (struct native_array_item *)
         realloc(a->data, a->max * sizeof(struct native_array_item));
      memset(&a->data[a->size], 0, sizeof(struct native_array_item) * increment);
      __make_NULL(a , a->size , a->max - 1, 0);
   }
   a->size += increment;
}


void native_array_reset_size(const CMPIArray * array, CMPICount increment)
{
   struct native_array *a = (struct native_array *) array;
   a->size = increment;
}

static CMPIArrayFT aft = {
   NATIVE_FT_VERSION,
   __aft_release,
   __aft_clone,
   __aft_getSize,
   __aft_getSimpleType,
   __aft_getElementAt,
   __aft_setElementAt
};

static struct native_array *__new_empty_array(int mm_add,
                                              CMPICount size,
                                              CMPIType type, CMPIStatus * rc)
{
   static CMPIArray a = {
      "CMPIArray",
      &aft
   };
   struct native_array array,*tArray;
   int state;

   array.array = a;
   tArray=memAddEncObj(mm_add, &array, sizeof(array), &state);
   tArray->mem_state = state;   
   tArray->refCount=0;   
   
   type &= ~CMPI_ARRAY;
   tArray->type  = ( type == CMPI_chars )? CMPI_string: type;
   tArray->size  = size;
 
   if (tArray->size==0) {
      tArray->max=8;
      tArray->dynamic=1;
   }
   else {
      tArray->max=tArray->size;
      tArray->dynamic=0;
   }    
   
   tArray->data = (struct native_array_item *)
      malloc ( tArray->max * sizeof ( struct native_array_item ) );

   __make_NULL(tArray, 0, tArray->max - 1, 0);

   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return (struct native_array*)tArray;
}


CMPIArray *TrackedCMPIArray(CMPICount size, CMPIType type, CMPIStatus * rc)
{
   void *array = internal_new_CMPIArray(MEM_TRACKED, size, type, rc);
   return (CMPIArray *) array;
}

CMPIArray *internal_new_CMPIArray(int mode, CMPICount size, CMPIType type,
                                  CMPIStatus * rc)
{
   void *array = __new_empty_array(mode, size, type, rc);
   return (CMPIArray *) array;
}

CMPIArray *NewCMPIArray(CMPICount size, CMPIType type, CMPIStatus * rc)
{
   void *array = internal_new_CMPIArray(MEM_NOT_TRACKED, size, type, rc);
   return (CMPIArray *) array;
}



CMPIArray *internal_native_make_CMPIArray(CMPIData * av, CMPIStatus * rc,
                                 ClObjectHdr * hdr, int mode)
{
   void *array =
       __new_empty_array(mode, av->value.sint32, av->type, rc);
   int i, m;
   CMPIValue value;

   for (i = 0, m = (int) av->value.sint32; i < m; i++)
      if (av[i + 1].state != CMPI_nullValue) {
	  if (av[i + 1].type == CMPI_string) {
	      char *chars = (char *) ClObjectGetClString(hdr, (ClString *) & av[i + 1].value.chars);
	      arraySetElementNotTrackedAt((CMPIArray *) array, i, (CMPIValue *) chars, CMPI_chars);
	  } else if (av[i + 1].type == CMPI_ref) {
	      CMPIValue value;
	      char *msg = "";
	      char *chars = (char *) ClObjectGetClString(hdr, (ClString *) & av[i + 1].value.chars);
	      value.ref = getObjectPath(chars,&msg);	      
	      arraySetElementNotTrackedAt((CMPIArray *) array, i, &value, CMPI_ref);
	  } else if (av[i + 1].type == CMPI_instance) {
              value.inst = (void *)ClObjectGetClObject(hdr, (ClString *) & av[i + 1].value.inst);
              if(value.inst) {
              	 relocateSerializedInstance(value.inst);
              }
              arraySetElementNotTrackedAt((CMPIArray *) array, i, &value, CMPI_instance);
	  } else if (av[i + 1].type == CMPI_dateTime) {
          const char *str =
          ClObjectGetClString(hdr, (ClString *) & av[i + 1].value.chars);
          value.dateTime = sfcb_native_new_CMPIDateTime_fromChars(str, NULL);
          arraySetElementNotTrackedAt((CMPIArray *) array, i, &value, CMPI_dateTime);
      } else {
	      arraySetElementNotTrackedAt((CMPIArray *) array, i, &av[i + 1].value,
					  av[i + 1].type);
	  }
      }           
   return (CMPIArray *) array;
}

CMPIArray *native_make_CMPIArray(CMPIData * av, CMPIStatus * rc,
                                 ClObjectHdr * hdr)
{
    return internal_native_make_CMPIArray(av, rc, hdr, MEM_TRACKED);
}

CMPIStatus sfcb_simpleArrayAdd(CMPIArray * array, CMPIValue * val, CMPIType type)
{
   struct native_array * a = (struct native_array *) array;
   if (a->dynamic) {
      if (a->size==0) {
         a->type=type;
         if (a->type==CMPI_chars) a->type=CMPI_string;
      }   
      return setElementAt(array,a->size,val,type,1);
   }   
   CMReturn ( CMPI_RC_ERR_FAILED );
} 

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 4 ***/
/*** End:              ***/
