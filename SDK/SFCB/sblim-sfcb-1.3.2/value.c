
/*
 * value.c
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
 * CMPIValue implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native.h"
#include "constClass.h"
#include "qualifier.h"
#include "cmpidtx.h"
#include "instance.h"
#include "objectpath.h"

void sfcb_native_release_CMPIValue(CMPIType type, CMPIValue * val)
{
   switch (type) {

   case CMPI_instance:
      CMRelease(val->inst);
      break;

   case CMPI_class:
      CMRelease(val->inst);
      break;
      
   case CMPI_qualifierDecl:
      CMRelease((CMPIQualifierDecl*)val->dataPtr.ptr);
      break;      

   case CMPI_ref:
      CMRelease(val->ref);
      break;

   case CMPI_args:
      CMRelease(val->args);
      break;

   case CMPI_filter:
      CMRelease(val->filter);
      break;

   case CMPI_enumeration:
      CMRelease(val->Enum);
      break;

   case CMPI_string:
      CMRelease(val->string);
      break;

   case CMPI_chars:
      free(val->chars);
      break;

   case CMPI_dateTime:
      CMRelease(val->dateTime);
      break;

   default:
      if (type & CMPI_ARRAY) {
         CMRelease(val->array);
      }
   }
}


CMPIValue sfcb_native_clone_CMPIValue(const CMPIType type,
                                 const CMPIValue * val, CMPIStatus * rc)
{
   CMPIValue v;
   CMPIConstClass *cl;

   if (type & CMPI_ENC) {

      switch (type) {

      case CMPI_instance:
         v.inst = CMClone(val->inst, rc);
         break;

      case CMPI_class:
         cl=(CMPIConstClass*)val->inst;
         v.inst = (CMPIInstance*)CMClone(cl, rc);
         break;
         
      case CMPI_qualifierDecl:
      	v.dataPtr.ptr = CMClone((CMPIQualifierDecl*)val->dataPtr.ptr, rc);
      	break;

      case CMPI_ref:
         v.ref = CMClone(val->ref, rc);
         break;

      case CMPI_args:
         v.args = CMClone(val->args, rc);
         break;

      case CMPI_filter:
         v.filter = CMClone(val->filter, rc);
         break;

      case CMPI_enumeration:
         v.Enum = CMClone(val->Enum, rc);
         break;

      case CMPI_string:
         v.string = CMClone(val->string, rc);
         break;

      case CMPI_chars:
         v.chars = strdup(val->chars);
         CMSetStatus(rc, CMPI_RC_OK);
         break;

      case CMPI_dateTime:
         v.dateTime = CMClone(val->dateTime, rc);
         break;
         
      case CMPI_ptr:
         v = *val;
         break;         
      }

   }
   else if (type & CMPI_ARRAY) {

      v.array = CMClone(val->array, rc);
   }
   else {

      sfcb_setAlignedValue(&v, val, type);
      CMSetStatus(rc, CMPI_RC_OK);
   }

   return v;
}

//extern CMPIString *__oft_toString(CMPIObjectPath * cop, CMPIStatus * rc);

char *sfcb_value2Chars(CMPIType type, CMPIValue * value)
{
   char str[256], *p;
   unsigned int size;
   CMPIString *cStr;
   
   str[0] = 0;
   if (value) {
     if (type & CMPI_ARRAY) {

     }
     else if (type & CMPI_ENC) {
       if (value->chars) {
	 /* non - null encapsulated value */
	 switch (type) {
	 case CMPI_instance:
	   break;

	 case CMPI_ref:
	   cStr = value->ref->ft->toString(value->ref, NULL); //  __oft_toString(value->ref, NULL);
	   return strdup((char *) cStr->hdl);
	   break;

	 case CMPI_args:
	   break;

	 case CMPI_filter:
	   break;

	 case CMPI_string:
	 case CMPI_numericString:
	 case CMPI_booleanString:
	 case CMPI_dateTimeString:
	 case CMPI_classNameString:
	   if (value->string->hdl) {
	     size = strlen((char *) value->string->hdl);
	     p = malloc(size + 8);
	     sprintf(p, "\"%s\"", (char *) value->string->hdl);
	     return p;
	   }
	   break;

	 case CMPI_dateTime:
	   cStr=CMGetStringFormat(value->dateTime,NULL);
	   size = strlen((char *) cStr->hdl);
	   p = malloc(size + 8);
	   sprintf(p, "\"%s\"", (char *) cStr->hdl);
	   return p;
	   break;
	 }
       }
     }
     else if (type & CMPI_SIMPLE) {

       switch (type) {
       case CMPI_boolean:
         return strdup(value->boolean ? "true" : "false");

       case CMPI_char16:
         break;
       }

     }
     else if (type & CMPI_INTEGER) {

       switch (type) {
       case CMPI_uint8:
         sprintf(str, "%u", value->uint8);
         return strdup(str);
       case CMPI_sint8:
         sprintf(str, "%d", value->sint8);
         return strdup(str);
       case CMPI_uint16:
         sprintf(str, "%u", value->uint16);
         return strdup(str);
       case CMPI_sint16:
         sprintf(str, "%d", value->sint16);
         return strdup(str);
       case CMPI_uint32:
         sprintf(str, "%u", value->uint32);
         return strdup(str);
       case CMPI_sint32:
         sprintf(str, "%d", value->sint32);
         return strdup(str);
       case CMPI_uint64:
         sprintf(str, "%llu", value->uint64);
         return strdup(str);
       case CMPI_sint64:
         sprintf(str, "%lld", value->sint64);
         return strdup(str);
       }

     }
     else if (type & CMPI_REAL) {

       switch (type) {
       case CMPI_real32:
         sprintf(str, "%g", value->real32);
         return strdup(str);
       case CMPI_real64:
         sprintf(str, "%g", value->real64);
         return strdup(str);
       }

   }
   }
   return strdup(str);
}

void sfcb_setAlignedValue(CMPIValue * target, const CMPIValue *source, 
			  CMPIType type)
{
  /* copies a value making sure the alignments are right */
  if (target && source) {
    memset(target,0,sizeof(CMPIValue));
    switch (type) {
    case CMPI_boolean:
    case CMPI_uint8:
    case CMPI_sint8:
      target->Byte = source->Byte;
      break;
    case CMPI_char16:
    case CMPI_uint16:
    case CMPI_sint16:
      target->Short = source->Short;
      break;
    case CMPI_uint32:
    case CMPI_sint32:
      target->Int = source->Int;
      break;
    case CMPI_real32:
      target->Float = source->Float;
      break;
    case CMPI_uint64:
    case CMPI_sint64:
      target->Long = source->Long;
      break;
    case CMPI_real64:
      target->Double = source->Double;
      break;
    default:
      if (type == CMPI_ptr) {
	*target = *source;
      } else {
	target->inst = source->inst;
      }
    }  
  }
}

static int extended_strcmp(char *s1, char *s2)
{
    if(s1 == NULL && s2 == NULL) return 0;
    if(s1 == NULL) return -1;
    if(s2 == NULL) return 1;
    
    return strcmp(s1, s2);
}

int sfcb_comp_CMPIValue(CMPIValue *val1, CMPIValue *val2, CMPIType type)
{
   int c;
   CMPIValue tempVal1, tempVal2;
   CMPIString *s1, *s2;
   
   /* check if we have null pointers for our ENC data types, the pointers */
   if(val1->array == NULL && val2->array == NULL) return 0; /*identical*/
   if(val1->array == NULL) return -1; /* val1 is less than val2 */
   if(val2->array == NULL) return 1;  /* val2 is less than val1 */
   
   if(type & CMPI_ARRAY) {
      c = (val1->array)->ft->getSize(val1->array, NULL);
      if(c != (val2->array)->ft->getSize(val2->array, NULL)) {
      	 /* member count not equal -> CMPIValue cannot be equal */
         return 1;
      }
      while(c--) {
         tempVal1 = ((val1->array)->ft->getElementAt(val1->array, c-1, NULL)).value;
         tempVal2 = ((val2->array)->ft->getElementAt(val2->array, c-1, NULL)).value;
         
         if(sfcb_comp_CMPIValue(&tempVal1, &tempVal2, type & ~CMPI_ARRAY)) {
            /* one element does not seem to be identical*/
            return 1;
         }
      }
      /* all tests passed - arrays identical */
      return 0;
   }
   if(val1 && val2) {
      switch(type) {
         case CMPI_boolean:
         case CMPI_uint8:
         case CMPI_sint8:
            return(val1->Byte - val2->Byte);
         case CMPI_char16:
         case CMPI_uint16:
         case CMPI_sint16:
            return(val1->Short - val2->Short);
         case CMPI_uint32:
         case CMPI_sint32:
            return(val1->Int - val2->Int);
         case CMPI_real32:
            return(val1->Float - val2->Float);
         case CMPI_uint64:
         case CMPI_sint64:
            return(val1->Long - val2->Long);
         case CMPI_real64:
            return(val1->Double - val2->Double);
         case CMPI_instance:
            return (instanceCompare(val1->inst, val2->inst));
         case CMPI_ref:
            return (objectpathCompare(val1->ref, val2->ref));
         case CMPI_string:
            return extended_strcmp(val1->string->ft->getCharPtr(val1->string, NULL),
                          val2->string->ft->getCharPtr(val2->string, NULL));
         case CMPI_dateTime:
            s1 = val1->dateTime->ft->getStringFormat(val1->dateTime, NULL);
            s2 = val2->dateTime->ft->getStringFormat(val2->dateTime, NULL);
            return extended_strcmp(s1->ft->getCharPtr(s1, NULL),
                          s2->ft->getCharPtr(s2, NULL));
         default:
            return 0;
      }
   }
   return 0;
}
