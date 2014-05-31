
/*
 * cimXmlRequest.c
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
 * Author:       Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 * CMPI broker encapsulated functionality.
 *
 * CIM operations request handler .
 *
*/



#include "cmpidt.h"
#include "cimXmlRequest.h"
#include "cimXmlParser.h"
#include "cimXmlGen.h"
#include "msgqueue.h"
#include "cmpidt.h"
#include "constClass.h"
#include "objectImpl.h"

#include "native.h"
#include "trace.h"
#include "utilft.h"
#include "string.h"

#include "queryOperation.h"
#include "config.h"

#ifdef SFCB_IX86
#define SFCB_ASM(x) asm(x)
#else
#define SFCB_ASM(x)
#endif

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX	64
#endif


extern const char *instGetClassName(CMPIInstance * ci);
extern CMPIData opGetKeyCharsAt(CMPIObjectPath * cop, unsigned int index,
                                const char **name, CMPIStatus * rc);
extern CMPIData __ift_internal_getPropertyAt(const CMPIInstance * ci, 
					     CMPICount i, char ** name,
					     CMPIStatus * rc, int readonly);


const char *opGetClassNameChars(CMPIObjectPath * cop);

#define DATA2XML(data,obj,name,refname,btag,etag,sb,qsb,inst,param)    \
  data2xml((data),(obj),(name),(refname),(btag),sizeof(btag)-1,(etag), \
	   sizeof(etag)-1,(sb),(qsb),(inst),(param))


char *XMLEscape(char *in, int *outlen)
{
   int i, l, o, n;
   char *out;
   char *rep;

   _SFCB_ENTER(TRACE_CIMXMLPROC, "XMLEscape");

   if (in == NULL)
      return (NULL);
   l = strlen(in);
   out = (char *) malloc((l * 6) + 1);
   o = 0;

   for (i = 0; i < l; i++) {
      char ch = in[i];
      switch (ch) {
      case '>':
	n = 4;
	rep = "&gt;";
	break;
      case '<':
	n = 4;
	rep = "&lt;";
	break;
      case '&':
	n = 5;
	rep = "&amp;";
	break;
      case '"':
	n = 6;
	rep = "&quot;";
	break;
      case '\'':
	n = 6;
	rep = "&apos;";
	break;
      default:
	/* avoid memcopy */
	out[o] = ch;
	o += 1;
	continue;
      }
      memcpy(out+o, rep, n);
      o += n;
   }
   out[o] = '\0';
   if (outlen) *outlen=o;

   _SFCB_RETURN(out);
}

/** Takes a string representation of type and value, creates corresponding 
  * CMPI data type.
  *
  * @param type is the string representation of a CMPI type ("boolean", 
  *             "numeric", "ref").  Any other value is treated as CMPI_chars
  * @param value is the string representation of the CMPI value to be assigned
  * @param ref is
  * @param val is the CMPI value created (is modified)
  * @param typ is the CMPI type (is modified)
  * @param scopingNS is
  *
  * @return The created CMPIValue
  *
  */

CMPIValue *getKeyValueTypePtr(char *type, char *value, XtokValueReference *ref,
                              CMPIValue * val, CMPIType * typ, char *scopingNS)
{
   if (type) {
      if (strcasecmp(type, "string") == 0);
      else if (strcasecmp(type, "boolean") == 0) {
         *typ = CMPI_boolean;
         if (strcasecmp(value, "true") == 0)
            val->boolean = 1;
         else
            val->boolean = 0;
         return val;
      }
      else if (strcasecmp(type, "numeric") == 0) {
         if (value[0] == '+' || value[0] == '-') {
            *typ = CMPI_sint64;
            sscanf(value, "%lld", &val->uint64);
         }
         else {
            sscanf(value, "%llu", &val->sint64);
            *typ = CMPI_uint64;
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
            in=&ref->instancePath.instanceName;
            hn=ref->instancePath.path.host.host;
            ns=ref->instancePath.path.nameSpacePath;
            break;   
         case typeValRef_LocalInstancePath: 
            in=&ref->localInstancePath.instanceName;
            ns=ref->localInstancePath.path;
            break;   
         case typeValRef_InstanceName: 
            in=&ref->instanceName;
            ns=scopingNS;
            break;   
         default:
            mlogf(M_ERROR,M_SHOW,"%s(%d): unexpected reference type %d %x\n", __FILE__, __LINE__, 
               (int) type, (int) type);
            abort();   
         }

         cn=in->className;
         op=TrackedCMPIObjectPath(ns,cn,NULL);
         CMSetHostname(op,hn);

         for (i = 0, m = in->bindings.next; i < m; i++) {
            valp = getKeyValueTypePtr(
               in->bindings.keyBindings[i].type,
               in->bindings.keyBindings[i].value,
               &in->bindings.keyBindings[i].ref,
               &v, &type, scopingNS);
            CMAddKey(op,in->bindings.keyBindings[i].name,valp,type);
         }
         *typ = CMPI_ref;
         val->ref=op;
         return val;
      }
   }
   
   *typ = CMPI_chars;
   return (CMPIValue *) value;
}

static char *dataType(CMPIType type)
{
   switch (type & ~CMPI_ARRAY) {
   case CMPI_chars:
   case CMPI_string:
      return "string";
   case CMPI_sint64:
      return "sint64";
   case CMPI_uint64:
      return "uint64";
   case CMPI_sint32:
      return "sint32";
   case CMPI_uint32:
      return "uint32";
   case CMPI_sint16:
      return "sint16";
   case CMPI_uint16:
      return "uint16";
   case CMPI_uint8:
      return "uint8";
   case CMPI_sint8:
      return "sint8";
   case CMPI_boolean:
      return "boolean";
   case CMPI_char16:
      return "char16";
   case CMPI_real32:
      return "real32";
   case CMPI_real64:
      return "real64";
   case CMPI_dateTime:
      return "datetime"; 
   case CMPI_ref:
      return "*";
   case CMPI_instance:
      return "%";
   }
   mlogf(M_ERROR,M_SHOW,"%s(%d): invalid data type %d %x\n", __FILE__, __LINE__, (int) type,
          (int) type);
   SFCB_ASM("int $3");
   abort();
   return "*??*";
}

static char *keyType(CMPIType type)
{
   switch (type) {
   case CMPI_chars:
   case CMPI_string:
   case CMPI_dateTime:
      return "string";
   case CMPI_sint64:
   case CMPI_uint64:
   case CMPI_sint32:
   case CMPI_uint32:
   case CMPI_sint16:
   case CMPI_uint16:
   case CMPI_uint8:
   case CMPI_sint8:
      return "numeric";
   case CMPI_boolean:
      return "boolean";
   case CMPI_ref:
      return "*";
   }
   mlogf(M_ERROR,M_SHOW,"%s(%d): invalid key data type %d %x\n", __FILE__, __LINE__,
          (int) type, (int) type);
   SFCB_ASM("int $3");
   abort();
   return "*??*";
}

CMPIType guessType(char *val)
{
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
   else if (strcasecmp(val,"true") == 0) return CMPI_boolean;
   else if (strcasecmp(val,"false") == 0) return CMPI_boolean;
   return CMPI_string;
}

CMPIValue makeFromEmbeddedObject(XtokValue value, char *ns)
{
	XtokProperty *p = NULL;
	CMPIValue val;
	memset(&val, 0, sizeof(CMPIValue));
	CMPIObjectPath *path;
	
	if(value.type == typeValue_Instance) {
		CMPIInstance *inst;
		XtokInstance *xtokInstance = value.instance;
		path = TrackedCMPIObjectPath(ns, xtokInstance->className, NULL);
		inst = TrackedCMPIInstance(path, NULL);
		
		for (p = xtokInstance->properties.first; p; p = p->next) {
			if (p->val.val.value) {
			val = str2CMPIValue(p->valueType, p->val.val, &p->val.ref, NULL);
			CMSetProperty(inst, p->name, &val, p->valueType);
			}
		}
		val.inst = inst;
	} else if(value.type == typeValue_Class) {
		//not yet supported
	}
	return val;
}

CMPIValue str2CMPIValue(CMPIType type, XtokValue val, XtokValueReference *ref, char* ns)
{
   CMPIValue value;
   CMPIType t;

   if (type==0) {
      type=guessType(val.value);
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
       if (max > 0) {
          t = guessType(arr->values[0].value);
       }
     }
     /* build an array by looping thru the elements */
     value.array = TrackedCMPIArray(max,t,NULL);
     if (value.array != NULL) {
       for (i=0; i<max; i++) {
	 v = str2CMPIValue(t, arr->values[i], refarr->values+i,ns);
	 CMSetArrayElementAt(value.array, i, &v, t); 
       }
       return value;
     }
   }
   
   switch (type) {
   case CMPI_char16:
      value.char16 = *val.value;
      break;
   case CMPI_string:
      value.string = sfcb_native_new_CMPIString(val.value, NULL,0);
      break;
   case CMPI_sint64:
      sscanf(val.value, "%lld", &value.sint64);
      break;
   case CMPI_uint64:
      sscanf(val.value, "%llu", &value.uint64);
      break;
   case CMPI_sint32:
      sscanf(val.value, "%d", &value.sint32);
      break;
   case CMPI_uint32:
      sscanf(val.value, "%u", &value.uint32);
      break;
   case CMPI_sint16:
      sscanf(val.value, "%hd", &value.sint16);
      break;
   case CMPI_uint16:
      sscanf(val.value, "%hu", &value.uint16);
      break;
   case CMPI_uint8:
      sscanf(val.value, "%u", &value.uint32);
      value.uint8 = value.uint32;
      break;
   case CMPI_sint8:
      sscanf(val.value, "%d", &value.sint32);
      value.sint8 = value.sint32;
      break;
   case CMPI_boolean:
      value.boolean = strcasecmp(val.value, "false");
      if (value.boolean) value.boolean = 1;
      break;
   case CMPI_real32:
      sscanf(val.value, "%f", &value.real32);
      break;
   case CMPI_real64:
      sscanf(val.value, "%lf", &value.real64);
      break;
   case CMPI_dateTime:
      value.dateTime = sfcb_native_new_CMPIDateTime_fromChars(val.value, NULL);
      break;
   case CMPI_ref:
      getKeyValueTypePtr("ref", NULL, ref, &value, &t, ns);
      break;
   case CMPI_instance:
      value=makeFromEmbeddedObject(val, ns);
      break;
  default:
      mlogf(M_ERROR,M_SHOW,"%s(%d): invalid value %d-%p\n", __FILE__, __LINE__, (int) type, val);
      abort();
   }
   return value;
}

int value2xml(CMPIData d, UtilStringBuffer * sb, int wv)
{
   char str[256];
   char *sp = str;
   int splen = 0;
   int freesp = 0;

   if (d.type & CMPI_ARRAY) {
      sb->ft->appendChars(sb, "**[]**");
      return 1;
   }
   
   else {
      if (wv) SFCB_APPENDCHARS_BLOCK(sb, "<VALUE>");
      if ((d.type & (CMPI_UINT|CMPI_SINT))==CMPI_UINT) {
         unsigned long long ul = 0LL;
         switch (d.type) {
         case CMPI_uint8:
            ul = d.value.uint8;
            break;
         case CMPI_uint16:
            ul = d.value.uint16;
            break;
         case CMPI_uint32:
            ul = d.value.uint32;
            break;
         case CMPI_uint64:
            ul = d.value.uint64;
            break;
         }
         splen = sprintf(str, "%llu", ul);
      }
      
      else if (d.type & CMPI_SINT) {
         long long sl = 0LL;
          switch (d.type) {
         case CMPI_sint8:
            sl = d.value.sint8;
            break;
         case CMPI_sint16:
            sl = d.value.sint16;
            break;
         case CMPI_sint32:
            sl = d.value.sint32;
            break;
         case CMPI_sint64:
            sl = d.value.sint64;
            break;
         }
         splen = sprintf(str, "%lld", sl);
      }
      
      else if (d.type==CMPI_real32) {
	splen = sprintf(str, "%.7e", d.value.real32);
      }
      
      else if (d.type==CMPI_real64) {
         splen = sprintf(str, "%.16e", d.value.real64);
      }

      else if (d.type == CMPI_boolean)
         splen = sprintf(str, "%s", d.value.boolean ? "TRUE" : "FALSE");
      else if (d.type == CMPI_char16)
         splen = sprintf(str, "%c", d.value.char16);
      else if (d.type == CMPI_chars) {
 	 sp = XMLEscape(d.value.chars, &splen);
         if (sp) freesp = 1;
      }
      else if (d.type == CMPI_string) {
 	 sp = XMLEscape((char *) d.value.string->hdl, &splen);
         if (sp) freesp = 1;
      }
      else if (d.type == CMPI_dateTime) {
         if (d.value.dateTime) {
            CMPIString *sdf = CMGetStringFormat(d.value.dateTime, NULL);
            sp = (char *) sdf->hdl;
	    splen = 25;
         }
         else {
	   splen = 0;
	 }
      }
      else if(d.type == CMPI_instance) {
         SFCB_APPENDCHARS_BLOCK(sb, "<![CDATA[");
         instance2xml(d.value.inst, sb, 0);
         SFCB_APPENDCHARS_BLOCK(sb, "]]>");
	 splen = 0;
      }
      else {
         mlogf(M_ERROR,M_SHOW,"%s(%d): invalid value2xml %d-%x\n", __FILE__, __LINE__,
                (int) d.type, (int) d.type);
         abort();
      }
      if (splen) {
	sb->ft->appendBlock(sb, sp, splen);
      }
      if (wv) SFCB_APPENDCHARS_BLOCK(sb, "</VALUE>\n");
   }
   if (freesp) free(sp);
   return 0;
}

static int keyBinding2xml(CMPIObjectPath * op, UtilStringBuffer * sb);

static int lnsPath2xml(CMPIObjectPath * ci, UtilStringBuffer * sb)
{
   _SFCB_ENTER(TRACE_CIMXMLPROC, "lnsPath2xml");
   char * ns = CMGetCharPtr(CMGetNameSpace(ci,NULL));
   
   if (ns && *ns) {
     char *nsc = strdup(ns);
     char *nsp;
     ns = nsc;
     SFCB_APPENDCHARS_BLOCK(sb, "<LOCALNAMESPACEPATH>\n");
     do {
       nsp = strchr(ns,'/');
       if (nsp) {
	 *nsp = 0;
       }
       SFCB_APPENDCHARS_BLOCK(sb, "<NAMESPACE NAME=\"");
       sb->ft->appendChars(sb, ns);
       SFCB_APPENDCHARS_BLOCK(sb, "\"/>\n");
       ns = nsp + 1;
     } while (nsp);
     free(nsc);
     SFCB_APPENDCHARS_BLOCK(sb, "</LOCALNAMESPACEPATH>\n");  
   }
   _SFCB_RETURN(0);
}

static int nsPath2xml(CMPIObjectPath * ci, UtilStringBuffer * sb)
{
   _SFCB_ENTER(TRACE_CIMXMLPROC, "nsPath2xml");
   char * hn = CMGetCharPtr(CMGetHostname(ci,NULL));

   SFCB_APPENDCHARS_BLOCK(sb, "<NAMESPACEPATH>\n");
   SFCB_APPENDCHARS_BLOCK(sb, "<HOST>");
   if (hn && *hn) {
      sb->ft->appendChars(sb, hn);
   } else {
      SFCB_APPENDCHARS_BLOCK(sb, "localhost");
   }
   SFCB_APPENDCHARS_BLOCK(sb, "</HOST>\n");
   
   lnsPath2xml(ci, sb);
   SFCB_APPENDCHARS_BLOCK(sb, "</NAMESPACEPATH>\n");
   _SFCB_RETURN(0);
}

int instanceName2xml(CMPIObjectPath * cop, UtilStringBuffer * sb)
{
   _SFCB_ENTER(TRACE_CIMXMLPROC, "instanceName2xml");
   SFCB_APPENDCHARS_BLOCK(sb, "<INSTANCENAME CLASSNAME=\"");
   sb->ft->appendChars(sb, opGetClassNameChars(cop));
   SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
   keyBinding2xml(cop, sb);
   SFCB_APPENDCHARS_BLOCK(sb, "</INSTANCENAME>\n");

   _SFCB_RETURN(0);
}

static int refValue2xml(CMPIObjectPath * ci, UtilStringBuffer * sb)
{
  _SFCB_ENTER(TRACE_CIMXMLPROC, "refValue2xml");
  
  SFCB_APPENDCHARS_BLOCK(sb, "<VALUE.REFERENCE>\n");
  if (ci && ci->hdl) {
    char * ns = CMGetCharPtr(CMGetNameSpace(ci,NULL));
    char * hn = CMGetCharPtr(CMGetHostname(ci,NULL));
    if (hn && *hn && ns && *ns) {
      SFCB_APPENDCHARS_BLOCK(sb, "<INSTANCEPATH>\n");
      nsPath2xml(ci, sb);
    } else if (ns && *ns) {
      SFCB_APPENDCHARS_BLOCK(sb, "<LOCALINSTANCEPATH>\n");
      lnsPath2xml(ci, sb);
    }
    instanceName2xml(ci, sb);
    if (hn && *hn && ns && *ns) {
      SFCB_APPENDCHARS_BLOCK(sb, "</INSTANCEPATH>\n");
    } else if (ns && *ns) {
      SFCB_APPENDCHARS_BLOCK(sb, "</LOCALINSTANCEPATH>\n");
    }
  }
  SFCB_APPENDCHARS_BLOCK(sb, "</VALUE.REFERENCE>\n");
  _SFCB_RETURN(0);
}

static void method2xml(CMPIType type, CMPIString *name, char *bTag, char *eTag,
   UtilStringBuffer * sb, UtilStringBuffer * qsb)
{
   _SFCB_ENTER(TRACE_CIMXMLPROC, "method2xml");
   sb->ft->appendChars(sb, bTag);
   sb->ft->appendChars(sb, (char *) name->hdl);
   SFCB_APPENDCHARS_BLOCK(sb, "\" TYPE=\"");
   sb->ft->appendChars(sb, dataType(type));
   SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
   if (qsb) sb->ft->appendChars(sb, (char *) qsb->hdl);
   sb->ft->appendChars(sb, eTag);

   _SFCB_EXIT();
}

void data2xml(CMPIData * data, void *obj, CMPIString * name, CMPIString * refName, 
		     char *bTag, int bTagLen, char *eTag, int eTagLen,
                     UtilStringBuffer * sb, UtilStringBuffer * qsb, int inst, int param)
{
   _SFCB_ENTER(TRACE_CIMXMLPROC, "data2xml");

   char *type;

   if (data->type & CMPI_ARRAY) { 
      CMPIArray *ar = data->value.array;
      CMPIData d;
      int j, ac = ar ? CMGetArrayCount(ar, NULL) : 0;

      sb->ft->appendBlock(sb, bTag,bTagLen);
      sb->ft->appendChars(sb, (char *) name->hdl);
      if (param) SFCB_APPENDCHARS_BLOCK(sb, "\" PARAMTYPE=\"");
      else if (bTag) SFCB_APPENDCHARS_BLOCK(sb, "\" TYPE=\"");
      if (data->type == CMPI_refA) {
         SFCB_APPENDCHARS_BLOCK(sb, "reference");
      }
      else if(((data->type & ~CMPI_ARRAY) == CMPI_instance) 
               || ((data->type & ~CMPI_ARRAY) == CMPI_class)) {
         SFCB_APPENDCHARS_BLOCK(sb, "string");
      } else {
          sb->ft->appendChars(sb, dataType(data->type));
      }
      SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
      if (qsb) sb->ft->appendChars(sb, (char *) qsb->hdl);
      if (data->state == 0) {
         if (data->type == CMPI_refA) {
            SFCB_APPENDCHARS_BLOCK(sb, "<VALUE.REFARRAY>\n");
         }
         else {
            SFCB_APPENDCHARS_BLOCK(sb, "<VALUE.ARRAY>\n");
         }
         for (j = 0; j < ac; j++) {
            d = CMGetArrayElementAt(ar, j, NULL);
	    if ((d.state & CMPI_nullValue)==0) {
	      if (d.type == CMPI_ref) {
		refValue2xml(d.value.ref,sb);
	      } else {
		value2xml(d, sb, 1);
	      }
	    }
         }
         if (data->type == CMPI_refA) {
            SFCB_APPENDCHARS_BLOCK(sb, "</VALUE.REFARRAY>\n");
         } else {
            SFCB_APPENDCHARS_BLOCK(sb, "</VALUE.ARRAY>\n");
         }
      }
   }
   
   else {
      type = dataType(data->type);
      if (*type == '*') {
	sb->ft->appendBlock(sb, bTag, bTagLen);
         sb->ft->appendChars(sb, (char *) name->hdl);
	 if (param) {
	   SFCB_APPENDCHARS_BLOCK(sb, "\" PARAMTYPE=\"reference");
	 } else if (refName) {
        SFCB_APPENDCHARS_BLOCK(sb, "\" REFERENCECLASS=\"");
        sb->ft->appendChars(sb, (char *) refName->hdl);
     }
         SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
         if (qsb) sb->ft->appendChars(sb, (char *) qsb->hdl);
         if (inst && data->value.ref) {
	   refValue2xml(data->value.ref,sb);
         }
      }
      
      else if (*type == '%') {                  
	     sb->ft->appendBlock(sb, bTag, bTagLen);
         sb->ft->appendChars(sb, (char *) name->hdl);
         SFCB_APPENDCHARS_BLOCK(sb, "\" EmbeddedObject=\"object");
         if (param) SFCB_APPENDCHARS_BLOCK(sb, "\" PARAMTYPE=\"string\">\n");
         else SFCB_APPENDCHARS_BLOCK(sb, "\" TYPE=\"string\">\n");
         if(data->value.inst) {
	         SFCB_APPENDCHARS_BLOCK(sb, "<VALUE>");
	         SFCB_APPENDCHARS_BLOCK(sb, "<![CDATA[");
	         instance2xml(data->value.inst, sb, 0);
	         SFCB_APPENDCHARS_BLOCK(sb, "]]>");
	         SFCB_APPENDCHARS_BLOCK(sb, "</VALUE>\n");
         }
     }
      
      else {
	     sb->ft->appendBlock(sb, bTag, bTagLen);
         sb->ft->appendChars(sb, (char *) name->hdl);
         if (param) SFCB_APPENDCHARS_BLOCK(sb, "\" PARAMTYPE=\"");
         else  if (bTag) SFCB_APPENDCHARS_BLOCK(sb, "\" TYPE=\"");
         sb->ft->appendChars(sb, type);
         SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
         if (qsb) sb->ft->appendChars(sb, (char *) qsb->hdl);
         if (data->state == 0) value2xml(*data, sb, 1);
      }
   }
   sb->ft->appendBlock(sb, eTag, eTagLen);
}

static void quals2xml(unsigned long quals, UtilStringBuffer * sb)
{
   if (quals & ClClass_Q_Abstract)
      SFCB_APPENDCHARS_BLOCK(sb, "<QUALIFIER NAME=\"Abstract\" TYPE=\"boolean\">\n"
                          "<VALUE>TRUE</VALUE>\n</QUALIFIER>\n");
   if (quals & ClClass_Q_Association)
      SFCB_APPENDCHARS_BLOCK(sb,
                          "<QUALIFIER NAME=\"Association\" TYPE=\"boolean\">\n"
                          "<VALUE>TRUE</VALUE>\n</QUALIFIER>\n");
   if (quals & ClClass_Q_Indication)
      SFCB_APPENDCHARS_BLOCK(sb,
                          "<QUALIFIER NAME=\"Indication\" TYPE=\"boolean\">\n"
                          "<VALUE>TRUE</VALUE>\n</QUALIFIER>\n");
   if (quals & (ClProperty_Q_Key << 8))
      SFCB_APPENDCHARS_BLOCK(sb, "<QUALIFIER NAME=\"Key\" TYPE=\"boolean\">\n"
                          "<VALUE>TRUE</VALUE>\n</QUALIFIER>\n");
   if (quals & (ClProperty_Q_EmbeddedObject << 8))
      SFCB_APPENDCHARS_BLOCK(sb,
                          "<QUALIFIER NAME=\"EmbeddedObject\" TYPE=\"boolean\">\n"
                          "<VALUE>TRUE</VALUE>\n</QUALIFIER>\n");                          
}

static void param2xml(CMPIParameter *pdata, CMPIConstClass * cls, ClParameter *parm, CMPIString *pname, 
      UtilStringBuffer * sb, unsigned int flags)
{
   ClClass *cl = (ClClass *) cls->hdl;
   int i, m;
   CMPIData data;
   CMPIString qname;
   char *etag="</PARAMETER>\n";
   UtilStringBuffer *qsb = NULL;
   
   if (flags & FL_includeQualifiers) {   
      m = ClClassGetMethParamQualifierCount(cl,parm);
      if (m) qsb = UtilFactory->newStrinBuffer(1024);
      for (i = 0; i < m; i++) {
         ClClassGetMethParamQualifierAt(cl, parm, i, &data, (char**)&qname.hdl);
         DATA2XML(&data,cls,&qname,NULL,"<QUALIFIER NAME=\"","</QUALIFIER>\n",qsb,NULL,0,0);
      }
   }
   
   if (pdata->type==CMPI_ref) {
      SFCB_APPENDCHARS_BLOCK(sb, "<PARAMETER.REFERENCE NAME=\"");
      sb->ft->appendChars(sb, (char*)pname->hdl);
      if (pdata->refName) {         
         SFCB_APPENDCHARS_BLOCK(sb, "\" REFERENCECLASS=\"");
         sb->ft->appendChars(sb, pdata->refName);
      }
      SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
      etag="</PARAMETER.REFERENCE>\n";
   }
   else if (pdata->type==CMPI_refA) {
      SFCB_APPENDCHARS_BLOCK(sb, "<PARAMETER.REFARRAY NAME=\"");
      sb->ft->appendChars(sb, (char*)pname->hdl);
      if (pdata->refName) {         
         SFCB_APPENDCHARS_BLOCK(sb, "\" REFERENCECLASS=\"");
         sb->ft->appendChars(sb, pdata->refName);
      }
      SFCB_APPENDCHARS_BLOCK(sb, "\">");
      etag="</PARAMETER.REFARRAY>\n";
   }   
   else {
      if (pdata->type&CMPI_ARRAY) {
      char size[128];
         SFCB_APPENDCHARS_BLOCK(sb, "<PARAMETER.ARRAY NAME=\"");
         sb->ft->appendChars(sb, (char*)pname->hdl);
         if(pdata->arraySize) {
         	sprintf(size,"\" ARRAYSIZE=\"%d",pdata->arraySize);
         	sb->ft->appendChars(sb, size);
         }
         etag="</PARAMETER.ARRAY>\n";
      }
      else {
         SFCB_APPENDCHARS_BLOCK(sb, "<PARAMETER NAME=\"");
         sb->ft->appendChars(sb, (char*)pname->hdl);
      }
      SFCB_APPENDCHARS_BLOCK(sb, "\" TYPE=\"");
      sb->ft->appendChars(sb, dataType(pdata->type));
      SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
   } 
 
   if (qsb) sb->ft->appendChars(sb, (char *) qsb->hdl);
   sb->ft->appendChars(sb, etag);
}

int cls2xml(CMPIConstClass * cls, UtilStringBuffer * sb, unsigned int flags)
{
   ClClass *cl = (ClClass *) cls->hdl;
   int i, m, q, qm, p, pm;
   char *type, *superCls;
   CMPIString *name, *qname, *refName;
   CMPIData data, qdata;
   CMPIType mtype;
   unsigned long quals;
   UtilStringBuffer *qsb = UtilFactory->newStrinBuffer(1024);

   _SFCB_ENTER(TRACE_CIMXMLPROC, "cls2xml"); 
   
   SFCB_APPENDCHARS_BLOCK(sb, "<CLASS NAME=\"");
   sb->ft->appendChars(sb, cls->ft->getCharClassName(cls));
   superCls = (char *) cls->ft->getCharSuperClassName(cls);
   if (superCls) {
      SFCB_APPENDCHARS_BLOCK(sb, "\" SUPERCLASS=\"");
      sb->ft->appendChars(sb, superCls);
   }
   SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
   if (flags & FL_includeQualifiers) quals2xml(cl->quals, sb);
   
   if (flags & FL_includeQualifiers)
      for (i = 0, m = ClClassGetQualifierCount(cl); i < m; i++) {
         data = cls->ft->getQualifierAt(cls, i, &name, NULL);
            DATA2XML(&data,cls,name,NULL,"<QUALIFIER NAME=\"","</QUALIFIER>\n",sb,NULL,0,0);
      }

   for (i = 0, m = ClClassGetPropertyCount(cl); i < m; i++) {
      qsb->ft->reset(qsb);
      data = getPropertyQualsAt(cls, i, &name, &quals, &refName, NULL);
      if (flags & FL_includeQualifiers) quals2xml(quals << 8, qsb);
      if (flags & FL_includeQualifiers)
         for (q = 0, qm = ClClassGetPropQualifierCount(cl, i); q < qm; q++) {
	   qdata = internalGetPropQualAt(cls, i, q, &qname, NULL);
            DATA2XML(&qdata,cls,qname,NULL,"<QUALIFIER NAME=\"","</QUALIFIER>\n",qsb,NULL,0,0);
            CMRelease(qname);
	    sfcb_native_release_CMPIValue(qdata.type,&qdata.value);
         }
      if (data.type & CMPI_ARRAY) DATA2XML(&data,cls,name,NULL,"<PROPERTY.ARRAY NAME=\"",
          "</PROPERTY.ARRAY>\n", sb, qsb, 0,0);
      else {
         type = dataType(data.type);
         if (*type == '*') {
            if (data.state &CMPI_nullValue) {
               SFCB_APPENDCHARS_BLOCK(sb, "<PROPERTY.REFERENCE NAME=\"");
               sb->ft->appendChars(sb, (char*)name->hdl);
               if(refName) {
                  SFCB_APPENDCHARS_BLOCK(sb, "\" REFERENCECLASS=\"");
                  sb->ft->appendChars(sb, (char*)refName->hdl);
               }
               SFCB_APPENDCHARS_BLOCK(sb, "\"></PROPERTY.REFERENCE>\n");
            }
            else DATA2XML(&data,cls,name,refName,"<PROPERTY.REFERENCE NAME=\"",
                   "</PROPERTY.REFERENCE>\n", sb, qsb, 0,0);
         }   
         else  DATA2XML(&data,cls,name,NULL,"<PROPERTY NAME=\"", "</PROPERTY>\n",
            sb, qsb, 0,0);
      }
      CMRelease(name);
   }
   
   for (i = 0, m = ClClassGetMethodCount(cl); i < m; i++) {
      ClMethod *meth;
      ClParameter *parm;
      char * sname, *smname;
      CMPIString *name, *mname;
      
      qsb->ft->reset(qsb);
      ClClassGetMethodAt(cl, i, &mtype, &smname, &quals);
      mname=sfcb_native_new_CMPIString(smname,NULL,2);
      meth=((ClMethod*)ClObjectGetClSection(&cl->hdr,&cl->methods))+i;
      
      if (flags & FL_includeQualifiers) {
         for (q = 0, qm = ClClassGetMethQualifierCount(cl, i); q < qm; q++) {
            ClClassGetMethQualifierAt(cl, meth, q, &qdata, &sname);
	    name=sfcb_native_new_CMPIString(sname,NULL,2);
            DATA2XML(&qdata,cls,name,NULL,"<QUALIFIER NAME=\"","</QUALIFIER>\n",qsb,NULL,0,0);
         }
      }   
      
      for (p = 0, pm = ClClassGetMethParameterCount(cl, i); p < pm; p++) {
         CMPIParameter pdata;
         ClClassGetMethParameterAt(cl, meth, p, &pdata, &sname);
	 name=sfcb_native_new_CMPIString(sname,NULL,2);
         parm=((ClParameter*)ClObjectGetClSection(&cl->hdr,&meth->parameters))+p;
         param2xml(&pdata,cls,parm,name,qsb,flags);
      }
 
      method2xml(mtype,mname,"<METHOD NAME=\"", "</METHOD>\n",sb, qsb);
   }
   
   SFCB_APPENDCHARS_BLOCK(sb, "</CLASS>\n");

   qsb->ft->release(qsb);

   _SFCB_RETURN(0);
}

int instance2xml(CMPIInstance * ci, UtilStringBuffer * sb, unsigned int flags)
{
   ClInstance *inst = (ClInstance *) ci->hdl;
   int i, m = ClInstanceGetPropertyCount(inst);
   char *type;
   UtilStringBuffer *qsb = UtilFactory->newStrinBuffer(1024);

   _SFCB_ENTER(TRACE_CIMXMLPROC, "instance2xml");
   
   SFCB_APPENDCHARS_BLOCK(sb, "<INSTANCE CLASSNAME=\"");
   sb->ft->appendChars(sb, instGetClassName(ci));
   SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
   
   if (flags & FL_includeQualifiers) quals2xml(inst->quals, sb);

   for (i = 0; i < m; i++) {
      CMPIString name;
      CMPIData data;
      qsb->ft->reset(qsb);
      if(ClInstanceIsPropertyAtFiltered(inst, i)) {
          continue;
      }
      data = __ift_internal_getPropertyAt(ci, i, (char**)&name.hdl, NULL, 1);
      
      if (data.type & CMPI_ARRAY) {
         DATA2XML(&data,ci,&name,NULL,"<PROPERTY.ARRAY NAME=\"", "</PROPERTY.ARRAY>\n",
                  sb, qsb, 1,0);
      }   
      else {
         type = dataType(data.type);
         if (*type == '*')   DATA2XML(&data,ci,&name,NULL,"<PROPERTY.REFERENCE NAME=\"",
                     "</PROPERTY.REFERENCE>\n", sb, qsb, 1,0);
         else DATA2XML(&data,ci,&name,NULL,"<PROPERTY NAME=\"", "</PROPERTY>\n", sb, qsb, 1,0);
      }
      
      if (data.type & (CMPI_ENC|CMPI_ARRAY)) {// don't get confused using generic release 
         if ((data.state&~CMPI_keyValue)==0 && data.type != CMPI_instance) 
            data.value.inst->ft->release(data.value.inst);
      }   
   }
   SFCB_APPENDCHARS_BLOCK(sb, "</INSTANCE>\n");

   qsb->ft->release(qsb);
   
   _SFCB_RETURN(0);
}

int args2xml(CMPIArgs * args, UtilStringBuffer * sb)
{
   int i, m;

   _SFCB_ENTER(TRACE_CIMXMLPROC, "args2xml");
   
   if (args==NULL) _SFCB_RETURN(0);
   
   m = CMGetArgCount(args,NULL);
   if (m==0) _SFCB_RETURN(0);

   
   for (i = 0; i < m; i++) {
      CMPIString *name;
      CMPIData data;
      data = CMGetArgAt(args, i, &name, NULL);
      
      DATA2XML(&data,args,name,NULL,"<PARAMVALUE NAME=\"", "</PARAMVALUE>\n", sb, NULL, 1,1);
      
      if ((data.type & (CMPI_ENC|CMPI_ARRAY)) && data.value.inst) {
	// don't get confused using generic release 
	data.value.inst->ft->release(data.value.inst);
      }   
      CMRelease(name);
   }
   
   _SFCB_RETURN(0);
}

static int keyBinding2xml(CMPIObjectPath * op, UtilStringBuffer * sb)
{
   int i, m;
   _SFCB_ENTER(TRACE_CIMXMLPROC, "keyBinding2xml");

   for (i = 0, m = CMGetKeyCount(op, NULL); i < m; i++) {
      const char *name;
      char *type;
      CMPIData data;
      data = opGetKeyCharsAt(op, i, &name, NULL);
      SFCB_APPENDCHARS_BLOCK(sb, "<KEYBINDING NAME=\"");
      sb->ft->appendChars(sb, name);
      SFCB_APPENDCHARS_BLOCK(sb, "\">\n");
      type = keyType(data.type);
      if (*type == '*' && data.value.ref) {
	refValue2xml(data.value.ref,sb);
      }
      else {
         SFCB_APPENDCHARS_BLOCK(sb, "<KEYVALUE VALUETYPE=\"");
         sb->ft->appendChars(sb, type);
         SFCB_APPENDCHARS_BLOCK(sb, "\">");
         value2xml(data, sb, 0);
         SFCB_APPENDCHARS_BLOCK(sb, "</KEYVALUE>\n");
      }
      SFCB_APPENDCHARS_BLOCK(sb, "</KEYBINDING>\n");
   }

   _SFCB_RETURN(0);
}

static int className2xml(CMPIObjectPath * op, UtilStringBuffer * sb)
{
   SFCB_APPENDCHARS_BLOCK(sb, "<CLASSNAME NAME=\"");
   sb->ft->appendChars(sb, opGetClassNameChars(op));
   SFCB_APPENDCHARS_BLOCK(sb, "\"/>\n");
   return 0;
}

int enum2xml(CMPIEnumeration * enm, UtilStringBuffer * sb, CMPIType type,
                    int xmlAs, unsigned int flags)
{
   CMPIObjectPath *cop;
   CMPIInstance *ci;
   CMPIConstClass *cl;
   CMPIString *hs;
   static char hss[HOST_NAME_MAX+1];

   _SFCB_ENTER(TRACE_CIMXMLPROC, "enum2xml");
   
   while (CMHasNext(enm, NULL)) {
      if (type == CMPI_ref) {
         cop = CMGetNext(enm, NULL).value.ref;
         if (xmlAs==XML_asClassName) className2xml(cop,sb);
         else if (xmlAs==XML_asObjectPath) {
            SFCB_APPENDCHARS_BLOCK(sb, "<OBJECTPATH>\n");
            SFCB_APPENDCHARS_BLOCK(sb, "<INSTANCEPATH>\n");
            nsPath2xml(cop, sb);
            instanceName2xml(cop, sb);
            SFCB_APPENDCHARS_BLOCK(sb, "</INSTANCEPATH>\n");
            SFCB_APPENDCHARS_BLOCK(sb, "</OBJECTPATH>\n");
         }
         else instanceName2xml(cop, sb);
      }
      else if (type == CMPI_class) {
         cl = (CMPIConstClass*)CMGetNext(enm, NULL).value.inst;
         cls2xml(cl,sb,flags); 
      }
      else if (type == CMPI_instance) {
         ci = CMGetNext(enm, NULL).value.inst;
         cop = CMGetObjectPath(ci, NULL);
	 hs = CMGetHostname(cop,NULL);
	 if (hs == NULL || CMGetCharPtr(hs)==NULL) {
	   /* required for value.objectwithpath */
	   if(!hss[0] && gethostname(hss, HOST_NAME_MAX)) {
          CMSetHostname(cop,"localhost");
       } else {
          CMSetHostname(cop,hss);
       }
	 }
         if (xmlAs==XML_asObj) {
            SFCB_APPENDCHARS_BLOCK(sb, "<VALUE.OBJECTWITHPATH>\n");
            SFCB_APPENDCHARS_BLOCK(sb, "<INSTANCEPATH>\n");
            nsPath2xml(cop, sb);
         }
         else
            SFCB_APPENDCHARS_BLOCK(sb, "<VALUE.NAMEDINSTANCE>\n");
         instanceName2xml(cop, sb);
         if (xmlAs==XML_asObj)
            SFCB_APPENDCHARS_BLOCK(sb, "</INSTANCEPATH>\n");
         instance2xml(ci, sb, flags);
         if (xmlAs==XML_asObj)
            SFCB_APPENDCHARS_BLOCK(sb, "</VALUE.OBJECTWITHPATH>\n");
         else
            SFCB_APPENDCHARS_BLOCK(sb, "</VALUE.NAMEDINSTANCE>\n");
         cop->ft->release(cop);   
      }
   }
   
   _SFCB_RETURN(0);
}

#ifdef HAVE_QUALREP
int qualifierDeclaration2xml(CMPIQualifierDecl * q, UtilStringBuffer * sb)
{
   ClQualifierDeclaration *qual = (ClQualifierDeclaration *) q->hdl;
   CMPIData data;

   _SFCB_ENTER(TRACE_CIMXMLPROC, "qualifierDeclaration2xml");

   if(qual->qualifierData.used) {
      data = q->ft->getQualifierDeclData(q, NULL);
   }
   else {
      data.state = CMPI_badValue;
   }

   SFCB_APPENDCHARS_BLOCK(sb, "<QUALIFIER.DECLARATION NAME=\"");
   sb->ft->appendChars(sb, q->ft->getCharQualifierName(q));
   SFCB_APPENDCHARS_BLOCK(sb, "\"");
   if (qual->type) {
		SFCB_APPENDCHARS_BLOCK(sb," TYPE=\"");
		sb->ft->appendChars(sb, dataType(qual->type));
		SFCB_APPENDCHARS_BLOCK(sb,"\"");
   }
   if(data.state != CMPI_goodValue) { //no default value - ISARRAY needs to be specified
		if (qual->type & CMPI_ARRAY)
			SFCB_APPENDCHARS_BLOCK(sb," ISARRAY=\"true\"");
		else
			SFCB_APPENDCHARS_BLOCK(sb," ISARRAY=\"false\"");
   }
   if (qual->arraySize) {
   		int strtmpSize = sizeof(qual->arraySize)*8 / 3.32 + 1; //estimate max. no of possible digits
   		char strtmp[strtmpSize];
   		sprintf(strtmp, "%d", qual->arraySize);
		SFCB_APPENDCHARS_BLOCK(sb," ARRAYSIZE=\"");
		sb->ft->appendChars(sb, strtmp);
		SFCB_APPENDCHARS_BLOCK(sb,"\"");
   }
   if (qual->flavor & ClQual_F_Overridable) //default true - so we have to mention it, if it's false
		SFCB_APPENDCHARS_BLOCK(sb," OVERRIDABLE=\"true\"");
   else
		SFCB_APPENDCHARS_BLOCK(sb," OVERRIDABLE=\"false\"");
   if (qual->flavor & ClQual_F_ToSubclass) //default true - so we have to mention it, if it's false
		SFCB_APPENDCHARS_BLOCK(sb," TOSUBCLASS=\"true\"");
   else
		SFCB_APPENDCHARS_BLOCK(sb," TOSUBCLASS=\"false\"");   
   if (qual->flavor & ClQual_F_ToInstance) SFCB_APPENDCHARS_BLOCK(sb," TOINSTANCE=\"true\"");
   if (qual->flavor & ClQual_F_Translatable) SFCB_APPENDCHARS_BLOCK(sb," TRANSLATABLE=\"true\"");
   SFCB_APPENDCHARS_BLOCK(sb, ">\n");
   
   if (qual->scope) {
		SFCB_APPENDCHARS_BLOCK(sb, "<SCOPE");
		if(qual->scope & ClQual_S_Class) SFCB_APPENDCHARS_BLOCK(sb," CLASS=\"true\"");
		if(qual->scope & ClQual_S_Association) SFCB_APPENDCHARS_BLOCK(sb," ASSOCIATION=\"true\"");
		if(qual->scope & ClQual_S_Reference) SFCB_APPENDCHARS_BLOCK(sb," REFERENCE=\"true\"");
		if(qual->scope & ClQual_S_Property) SFCB_APPENDCHARS_BLOCK(sb," PROPERTY=\"true\"");
		if(qual->scope & ClQual_S_Method) SFCB_APPENDCHARS_BLOCK(sb," METHOD=\"true\"");
		if(qual->scope & ClQual_S_Parameter) SFCB_APPENDCHARS_BLOCK(sb," PARAMETER=\"true\"");
		if(qual->scope & ClQual_S_Indication) SFCB_APPENDCHARS_BLOCK(sb," INDICATION=\"true\"");
		SFCB_APPENDCHARS_BLOCK(sb, "></SCOPE>\n");
   }
   
	if(data.state == CMPI_goodValue) {
		if(data.type & CMPI_ARRAY) {
			SFCB_APPENDCHARS_BLOCK(sb, "<VALUE.ARRAY>\n");
			int i;
			for(i=0; i< data.value.array->ft->getSize(data.value.array, NULL); i++)
				value2xml(data.value.array->ft->getElementAt(data.value.array, i, NULL), sb, 1);
			SFCB_APPENDCHARS_BLOCK(sb, "</VALUE.ARRAY>\n");			
		} else {
			value2xml(data, sb, 1);
		}
	}
   SFCB_APPENDCHARS_BLOCK(sb, "</QUALIFIER.DECLARATION>\n");

   _SFCB_RETURN(0);
}

int qualiEnum2xml(CMPIEnumeration * enm, UtilStringBuffer * sb)
{
   CMPIQualifierDecl *q;

   _SFCB_ENTER(TRACE_CIMXMLPROC, "qualiEnum2xml");
   while (CMHasNext(enm, NULL)) {
         q = CMGetNext(enm, NULL).value.dataPtr.ptr; 
         qualifierDeclaration2xml(q, sb);
   }
   _SFCB_RETURN(0);
}
#endif
