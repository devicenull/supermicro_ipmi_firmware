/*!
  \file value.c
  \brief Native CMPIValue helper module.

  This module provides means to clone and release CMPIValues.

  © Copyright IBM Corp. 2003, 2007
  © Copyright Intel Corp. 2006
 
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
#include "native.h"
#include "cimXmlParser.h"
#include "utilft.h"
#include "parserUtil.h"

#ifdef DMALLOC
#include "dmalloc.h"
#endif
 
char *keytype2Chars(CMPIType type);
CMPIType guessType(char *val);
char *value2Chars(CMPIType type, CMPIValue * value);
extern char *pathToChars(CMPIObjectPath * cop, CMPIStatus * rc, char *str,
								 int uri);


void native_release_CMPIValue ( CMPIType type, CMPIValue * val )
{
	if ( val )
		switch ( type ) {
	
		case CMPI_instance:
			if (val->inst)
			    CMRelease ( val->inst );
			break;
	
		case CMPI_class:
			if (val->inst)
			    CMRelease ( val->inst );
			break;
	
		case CMPI_ref:
			if (val->ref)
			    CMRelease ( val->ref );
			break;
	
		case CMPI_args:
			if (val->args)
			    CMRelease ( val->args );
			break;
	
		case CMPI_enumeration:
			if (val->Enum)
			    CMRelease ( val->Enum );
			break;
	
		case CMPI_string:
			if (val->string)
			    CMRelease ( val->string );
			break;
	
		case CMPI_chars:
			if (val->chars)
			    free ( val->chars );
			break;
	
		case CMPI_dateTime:
			if (val->dateTime)
			    CMRelease ( val->dateTime );
			break;
	
		default:
			if ( type & CMPI_ARRAY ) {
				if (val->array)
				    CMRelease ( val->array );
			}
		}
}


CMPIValue native_clone_CMPIValue ( CMPIType type,
				   CMPIValue * val,
				   CMPIStatus * rc )
{
	CMPIValue v;

        v.uint64 = 0LL;		/* Necessary, don't delete */
	if ( type & CMPI_ARRAY ) {
		CMPIArray *array = val->array;
                if (array)
		v.array = CMClone ( array, rc );
	} 
	else
	if ( (type & CMPI_ENC) && val->inst ) {
		switch ( type ) {

		case CMPI_instance:
                        if (val->inst)
			v.inst = CMClone ( val->inst, rc );
			break;

		case CMPI_ref:
                        if (val->ref)
			v.ref = CMClone ( val->ref, rc );
			break;

		case CMPI_args:
                        if (val->args)
			v.args = CMClone ( val->args, rc );
			break;

		case CMPI_enumeration:
                        if (val->Enum)
			v.Enum = CMClone ( val->Enum, rc );
			break;

		case CMPI_string:
                        if (val->string)
			v.string = CMClone ( val->string, rc );
			break;

		case CMPI_chars:
                        if (val->chars)
			v.chars = strdup ( val->chars );
			break;

		case CMPI_dateTime:
                        if (val->dateTime)
			v.dateTime = CMClone ( val->dateTime, rc );
			break;
		}

	} else {
                if (val)
		v = *val;
	}

	CMSetStatus ( rc, CMPI_RC_OK );
	return v;
}

static char *value2CharsUri(CMPIType type, CMPIValue * value, int uri)
{
   char str[2048], *p;
   unsigned int size;
   CMPIString *cStr;

   str[0]=0;
   if (type & CMPI_ARRAY) {
   }
   else if (type & CMPI_ENC) {

      switch (type) {
      case CMPI_instance:
         break;

      case CMPI_ref:
         return strdup(value->ref ? pathToChars(value->ref, NULL, str, uri) : "NULL");

      case CMPI_args:
         break;

      case CMPI_filter:
         break;

      case CMPI_string:
      case CMPI_numericString:
      case CMPI_booleanString:
      case CMPI_dateTimeString:
      case CMPI_classNameString:
         return strdup(value->string ? (char*)value->string->hdl : "NULL");

      case CMPI_dateTime:
         if (value->dateTime) {
         cStr=CMGetStringFormat(value->dateTime,NULL);
         p = strdup((char *) cStr->hdl);
         CMRelease(cStr);
         } else
             p = strdup("NULL");
	 return p;
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
         sprintf(str, "%lu", value->uint32);
         return strdup(str);
      case CMPI_sint32:
         sprintf(str, "%ld", value->sint32);
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
   return strdup(str);
}

char *value2Chars(CMPIType type, CMPIValue * value)
{
   return value2CharsUri(type,value,0);
}

char *keytype2Chars(CMPIType type)
{
   if (type == CMPI_boolean) return "boolean";
   if (type & (CMPI_INTEGER | CMPI_REAL)) return "numeric";
   return "string";
}

CMPIValue *getKeyValueTypePtr(char *type, char *value, XtokValueReference *ref,
                              CMPIValue * val, CMPIType * typ)
{
   if (type) {
      if (strcasecmp(type, "string") == 0);
      else if (strcasecmp(type, "boolean") == 0) {
         *typ = CMPI_boolean;
         val->boolean = (strcasecmp(type, "true") == 0) ? 1 : 0;
         return val;
      }
      else if (strcasecmp(type, "numeric") == 0) {
         if (value[0] == '+' || value[0] == '-') {
            *typ = CMPI_sint64;
            sscanf(value, "%lld", &val->uint64);
         }
         else {
            *typ = CMPI_uint64;
            sscanf(value, "%llu", &val->sint64);
         }
         return val;
      }
      else if (strcasecmp(type, "ref") == 0) {
         CMPIObjectPath *op;
         char *hn="",*ns="",*cn;
         CMPIType type;
         CMPIValue v, *valp;
         int i,m;
         XtokInstanceName *in; 
         
         switch(ref->type) {
         case typeValRef_InstancePath: 
            in=&ref->data.instancePath.instanceName;
            hn=ref->data.instancePath.path.host.host;
            ns=ref->data.instancePath.path.nameSpacePath.value;
            break;   
         case typeValRef_InstanceName: 
            in=&ref->data.instanceName;
            break;   
         case typeValRef_LocalInstancePath:
            in = &ref->data.localInstancePath.instanceName;
            ns = ref->data.localInstancePath.path.value;
            break;
         default:
//            mlogf(M_ERROR,M_SHOW,"%s(%d): unexpected reference type %d %x\n", __FILE__, __LINE__, 
            printf("%s(%d): unexpected reference type %d %x\n", __FILE__, __LINE__, 
               (int) type, (int) type);
            abort();   
         }

         cn=in->className;
         op=newCMPIObjectPath(ns,cn,NULL);
         CMSetHostname(op,hn);

         XtokKeyBinding *b;
         for (b = in->bindings.first; b; b = b->next) {
            valp = getKeyValueTypePtr(
               b->type,
               b->val.keyValue.value,
               &b->val.ref,
               &v, &type);
            CMAddKey(op,b->name,valp,type);
         }
         *typ = CMPI_ref;
         val->ref=op;
         return val;
      }
   }
   
   *typ = CMPI_chars;
   return (CMPIValue *) value;
}

CMPIInstance *getInstFromEmbedded(XtokInstance *inst)
{
    CMPIInstance *newInst;
    newInst = native_new_CMPIInstance(NULL, NULL);
    setInstNsAndCn(newInst, NULL, inst->className);
    setInstProperties(newInst, &inst->properties);
    return newInst;
}

CMPIType guessType(char *val)
{
   /* TODO: Currently doesn't guess right for real values (3.175e+00) */
   if (((*val=='-' || *val=='+') && strlen(val)>1) || isdigit(*val)) {
      char *c;
      for (c=val+1; ; c++) {
         if (*c==0) {
            if (!isdigit(*val)) return CMPI_sint64;
            return CMPI_uint64;
         }
         if (!isdigit(*c)) break;
      }
   }
   else if (strcasecmp(val,"true") == 0 || strcasecmp(val,"false") == 0)
     return CMPI_boolean;
   return CMPI_string;
}

CMPIValue str2CMPIValue(CMPIType type, char *val, XtokValueReference *ref)
{
   CMPIValue value,*valp;
 //  char *val=p->value;
   CMPIType t;

   if (type==0) {
      type=guessType(val);
   }
   
   if (type & CMPI_ARRAY) {
     /* array type received -- needs special handling */
     int i, max;
     CMPIValue v;
     XtokValueArray *arr = (XtokValueArray*)ref;
     XtokValueRefArray *refarr = (XtokValueRefArray*)arr;
     max=arr->next;
     if ((type & CMPI_ref) == CMPI_ref) {
       t = CMPI_ref;
     } else if (type & ~CMPI_ARRAY) {
       t = type & ~CMPI_ARRAY;
     } else {
       /* the guess type can go wrong */
       t = guessType(arr->values[0]);
     }
     /* build an array by looping thru the elements */
     value.array = native_new_CMPIArray(max,t,NULL);
     if (value.array != NULL) {
       for (i=0; i<max; i++) {
	 v = str2CMPIValue(t, arr->values[i], refarr->values+i);
	 CMSetArrayElementAt(value.array, i, &v, t);
	 native_release_CMPIValue(t, &v); 
       }
       return value;
     }
   }   
   
   switch (type) {
   case CMPI_char16:
      value.char16 = *val;
      break;
   case CMPI_chars:
      value.chars = strdup(val);
      break;
   case CMPI_string:
      value.string = native_new_CMPIString(val, NULL);
      break;
   case CMPI_sint64:
      sscanf(val, "%lld", &value.sint64);
      break;
   case CMPI_uint64:
      sscanf(val, "%llu", &value.uint64);
      break;
   case CMPI_sint32:
      sscanf(val, "%ld", &value.sint32);
      break;
   case CMPI_uint32:
      sscanf(val, "%lu", &value.uint32);
      break;
   case CMPI_sint16:
      sscanf(val, "%hd", &value.sint16);
      break;
   case CMPI_uint16:
      sscanf(val, "%hu", &value.uint16);
      break;
   case CMPI_uint8:
      sscanf(val, "%lu", &value.uint32);
      value.uint8 = value.uint32;
      break;
   case CMPI_sint8:
      sscanf(val, "%ld", &value.sint32);
      value.sint8 = value.sint32;
      break;
   case CMPI_boolean:
      value.boolean = strcasecmp(val, "false");
      break;
   case CMPI_real32:
      sscanf(val, "%f", &value.real32);
      break;
   case CMPI_real64:
      sscanf(val, "%lf", &value.real64);
      break;
   case CMPI_dateTime:
      value.dateTime = native_new_CMPIDateTime_fromChars(val, NULL);
      break;
   case CMPI_ref:
      valp=getKeyValueTypePtr("ref", NULL, ref, &value, &t);
      break;
   case CMPI_instance:
      value.inst=getInstFromEmbedded((XtokInstance*)val);
      break;
  default:
 //     mlogf(M_ERROR,M_SHOW,"%s(%d): invalid value %d-%s\n", __FILE__, __LINE__, (int) type, val);
      printf("%s(%d): invalid value %d-%s\n", __FILE__, __LINE__, (int) type, val);
      abort();
   }
   return value;
}
/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
