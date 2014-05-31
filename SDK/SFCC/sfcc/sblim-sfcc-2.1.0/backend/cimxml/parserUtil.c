/*
 * $id$
 *
 *  © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sven Schuetz <sven@de.ibm.com> (created file)
 *               all content by Adrian Schuur
 *
 * Description:
 *
 * CIM XML parser helper functions
 *
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parserUtil.h"
#include "cimXmlParser.h"
#include "sfcUtil/utilft.h"

static int ct = 0;
static int dontLex = 0;

extern CMPIConstClass * native_new_CMPIConstClass ( char  *cn, CMPIStatus * rc );
extern int addClassProperty( CMPIConstClass * ccls, char * name,
                 CMPIValue * value, CMPIType type,
                 CMPIValueState state);
extern CMPIType guessType(char *val);
extern int addInstPropertyQualifier( CMPIInstance* ci, char * pname,
                     char *qname, CMPIValue * value,
                     CMPIType type);
extern int addClassPropertyQualifier( CMPIConstClass* cc, char * pname,
                      char *qname, CMPIValue * value,
                      CMPIType type);
extern int addClassQualifier( CMPIConstClass* cc, char * name,
                      CMPIValue * value,
                      CMPIType type);
extern char *XmlToAsciiStr(char *XmlStr);

#if DEBUG
extern int do_debug;
#endif

static inline int isBoolean(CMPIData data)
{
   if (data.type == CMPI_chars) {
      if (strcasecmp(data.value.chars,"true") == 0) return 0xffff;
      if (strcasecmp(data.value.chars,"false") == 0) return 0;
   }
   return -1;
}

void createPath(CMPIObjectPath **op, XtokInstanceName *p)
{
   int i;
   CMPIValue val,*valp;
   CMPIType type;
   XtokKeyBinding *b;

   *op = newCMPIObjectPath(NULL, p->className, NULL);
   for (b = p->bindings.first; b; b = b->next) {
      valp = getKeyValueTypePtr(b->type,
                              b->val.keyValue.value,
                              &b->val.ref,
                              &val, &type);
      CMAddKey(*op, b->name, valp, type);
      if (type == CMPI_ref) {
    CMRelease(valp->ref);
      }
   }
}

void setInstProperties(CMPIInstance *ci, XtokProperties *ps)
{
   XtokProperty *np = NULL,*p = ps ? ps->first : NULL;
   CMPIValue val;
   CMPIObjectPath *op;
   CMPIStatus status;
   CMPIType   type;
   XtokQualifier *nq = NULL,*q;
   XtokQualifiers *qs;
   int rc, n, setq;

   while (p) {
      setq=1;
      switch (p->propType) {
      case typeProperty_Value:
         type = p->valueType;
         if (p->val.value.data.value != NULL && p->val.null==0) {
            if (type == CMPI_string || type == CMPI_chars) {
                char *charsStr = XmlToAsciiStr(p->val.value.data.value);
                val = str2CMPIValue(type, charsStr, NULL);
                free (charsStr);
            }
            else
                val = str2CMPIValue(type, p->val.value.data.value, NULL);
            CMSetProperty(ci, p->name, &val, type);
        native_release_CMPIValue(type, &val);
         }
         else {
            CMSetProperty(ci, p->name, NULL, type);
            setq = 0;
         }
         break;
      case typeProperty_Reference: 
         val=str2CMPIValue(CMPI_ref, NULL, &p->val.ref);
         CMSetProperty(ci, p->name, &val.ref, CMPI_ref);
     CMRelease(val.ref);
         break;
      case typeProperty_Array:
         type = p->valueType;
         if (p->val.array.next > 0) {
            CMPIArray *arr = newCMPIArray(0, type, &status);
            if (p->val.array.max > 0) {
            int i;
               for (i = 0; i < p->val.array.next; ++i)
               {
                   char *valStr = p->val.array.values[i];
                   if (type == CMPI_string || type == CMPI_chars) {
                       char *charsStr = XmlToAsciiStr(valStr);
                       val = str2CMPIValue(type, charsStr, NULL);
                       free (charsStr);
                   }
                   else
                       val = str2CMPIValue(type, valStr, NULL);
                   CMSetArrayElementAt(arr, i, &val, type);
               native_release_CMPIValue(type, &val);
               }
            }
            val.array = arr;
            CMSetProperty(ci, p->name, &val, type | CMPI_ARRAY);
            CMRelease(arr);         /* cloned in property */
         }
         else {
            CMSetProperty(ci, p->name, NULL, p->valueType | CMPI_ARRAY);
            setq = 0;
         }
         break;
      }

      if (setq) {
         qs=&p->val.qualifiers;
         q=qs ? qs->first : NULL;  
         n=0;   
         while (q) {
            if (q->type & CMPI_ARRAY) {
               CMPIArray *arr = NULL;
               arr = newCMPIArray(0, type, NULL);
               type  = q->type & ~CMPI_ARRAY;
               int i;
               if (q->data.array.max) {
                   for (i = 0; i < q->data.array.next; ++i) {
                  val = str2CMPIValue(type, q->data.array.values[i], NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
               }
               rc = addInstPropertyQualifier(ci, p->name, q->name,
                         (CMPIValue *)&arr, q->type); 
               native_release_CMPIValue(q->type,(CMPIValue*)&arr);
            }
            else {
               val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
               rc= addInstPropertyQualifier(ci, p->name, q->name, &val, q->type);
               native_release_CMPIValue(q->type,&val);
            }   
            nq = q->next; 
            q = nq;
         }
      }

      np = p->next;
      p = np;

   }

   if (ps)
      ps->first = ps->last =  NULL;
}

void setInstQualifiers(CMPIInstance *ci, XtokQualifiers *qs)
{
   XtokQualifier *nq = NULL,*q = qs ? qs->first : NULL;
   CMPIValue val;
   int rc;
   
   while (q) {
      if (q->type & CMPI_ARRAY) {
               CMPIType type=q->type&~CMPI_ARRAY;
               CMPIArray *arr = newCMPIArray(0, type, NULL);
               int i;
          if (q->data.array.max) {
              for (i = 0; i < q->data.array.next; ++i) {
                  val = str2CMPIValue(type, q->data.array.values[i], NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
               rc = addInstQualifier(ci, q->name, (CMPIValue*)&arr, q->type);
               native_release_CMPIValue(q->type,(CMPIValue*)&arr);
      }
      }
      else {
         val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
         rc = addInstQualifier(ci, q->name, &val, q->type);
         native_release_CMPIValue( q->type,&val);
      }
      nq = q->next;
      q = nq;
   }
   if (qs) {
      qs->first = qs->last = NULL;
   }
}

void setClassProperties(CMPIConstClass *cls, XtokProperties *ps)
{
   XtokProperty *np = NULL,*p = ps ? ps->first : NULL;
   CMPIValue val;
   CMPIArray       *arr;
   XtokQualifier   *nq,*q;
   XtokQualifiers *qs;
   int rc, n;

      val.uint64=0l;
   while (p) {
      switch (p->propType) {
      case typeProperty_Value:
         addClassProperty(cls, p->name, &val, p->valueType, CMPI_nullValue);
         break;
      case typeProperty_Reference:
         addClassProperty(cls, p->name, &val, CMPI_ref, CMPI_nullValue);
     break;
      case typeProperty_Array:
         val.array = arr;
     addClassProperty(cls, p->name, &val,
                               p->valueType | CMPI_ARRAY, CMPI_nullValue);
         break;
      }

      qs=&p->val.qualifiers;
      q=qs ? qs->first : NULL;  
      n=0;   
      while (q) {
         if (q->type & CMPI_ARRAY) {
            CMPIType type=q->type&~CMPI_ARRAY;
            arr = newCMPIArray(0, type, NULL);
            int i;
            if (q->data.array.max) {
                for (i = 0; i < q->data.array.next; ++i) {
               val = str2CMPIValue(type, q->data.array.values[i], NULL);
               CMSetArrayElementAt(arr, i, &val, type);
               native_release_CMPIValue(type,&val);
            }
            }
            val.array = arr;
            rc = addClassPropertyQualifier(cls, p->name, q->name, &val, q->type); 
            native_release_CMPIValue(q->type,(CMPIValue*)&arr);
         }
         else {
            val = str2CMPIValue(q->type, q->data.value.data.value, NULL);
            rc= addClassPropertyQualifier(cls, p->name, q->name, &val, q->type);
            native_release_CMPIValue(q->type,&val);
         }   
         nq = q->next; 
         q = nq;
      }

      np = p->next;
      p = np;
   }
   if (ps) ps->first = ps->last = NULL;
}

void setClassQualifiers(CMPIConstClass *cls, XtokQualifiers *qs)
{
   XtokQualifier *nq = NULL,*q = qs ? qs->first : NULL;
   CMPIValue val;
   int rc;
   
   while (q) {
      if (q->type & CMPI_ARRAY) {
               CMPIType type=q->type&~CMPI_ARRAY;
               CMPIArray *arr = newCMPIArray(0, type, NULL);
               int i;
          if (q->data.array.max > 0) {
              for (i = 0; i < q->data.array.next; ++i) {
                   char *valStr = q->data.array.values[i];
                   if (type == CMPI_string || type == CMPI_chars)
                   {
                       char *charsStr = XmlToAsciiStr(valStr);
                       val = str2CMPIValue(type, charsStr, NULL);
                       free (charsStr);
                   }
                   else
                       val = str2CMPIValue(type, valStr, NULL);
                  CMSetArrayElementAt(arr, i, &val, type);
                  native_release_CMPIValue(type,&val);
               }
               rc = addClassQualifier(cls, q->name, (CMPIValue*)&arr, q->type);
               native_release_CMPIValue(q->type,(CMPIValue*)&arr);
      }
      }
      else {
          char *valStr = q->data.value.data.value;
          if (q->type == CMPI_string || q->type == CMPI_chars)
          {
              char *charsStr = XmlToAsciiStr(valStr);
              val = str2CMPIValue(q->type, charsStr, NULL);
              free (charsStr);
          }
          else
              val = str2CMPIValue(q->type, valStr, NULL);
         rc = addClassQualifier(cls, q->name, &val, q->type);
         native_release_CMPIValue( q->type,&val);
      }
      nq = q->next;
      q = nq;
   }
   if (qs) {
      qs->first = qs->last = NULL;
   }
}


void addProperty(ParserControl *parm, XtokProperties *ps, XtokProperty *p)
{
   XtokProperty *np;
   np = (XtokProperty*)PARSER_MALLOC(sizeof(XtokProperty));
   memcpy(np, p, sizeof(XtokProperty));
   np->next = NULL;
   if (ps->last) ps->last->next = np;
   else ps->first = np;
   ps->last = np;
}

void addParamValue(ParserControl *parm, XtokParamValues *vs, XtokParamValue *v)
{
   XtokParamValue *nv;
   nv = (XtokParamValue*)PARSER_MALLOC(sizeof(XtokParamValue));
   memcpy(nv, v, sizeof(XtokParamValue));
   nv->next = NULL;
   if (vs->last)
      vs->last->next = nv;
   else
      vs->first = nv;
   vs->last = nv;
}

void addKeyBinding(ParserControl *parm, XtokKeyBindings *ks, XtokKeyBinding *k)
{
   XtokKeyBinding *nk;
   nk = (XtokKeyBinding*)PARSER_MALLOC(sizeof(XtokKeyBinding));
   memcpy(nk, k, sizeof(XtokKeyBinding));
   nk->next = NULL;
   if (ks->last) ks->last->next = nk;
   else ks->first = nk;
   ks->last = nk;
}

void addQualifier(ParserControl *parm, XtokQualifiers *qs, XtokQualifier *q)
{
   XtokQualifier *nq;
   nq = (XtokQualifier*)PARSER_MALLOC(sizeof(XtokQualifier));
   memcpy(nq, q, sizeof(XtokQualifier));
   nq->next = NULL;
   if (qs->last) qs->last->next = nq;
   else qs->first = nq;
   qs->last = nq;
}

void addMethod(ParserControl *parm, XtokMethods *ms, XtokMethod *m)
{
   XtokMethod *nm;
   nm = (XtokMethod*)PARSER_MALLOC(sizeof(XtokMethod));
   memcpy(nm, m, sizeof(XtokMethod));
   nm->next = NULL;
   if (ms->last)
      ms->last->next = nm;
   else
      ms->first = nm;
   ms->last = nm;
}

void addParam(ParserControl *parm, XtokParams *ps, XtokParam *p)
{
   XtokParam *np;
   np = (XtokParam*)PARSER_MALLOC(sizeof(XtokParam));
   memcpy(np, p, sizeof(XtokParam));
   np->next = NULL;
   if (ps->last)
      ps->last->next = np;
   else
      ps->first = np;
   ps->last = np;
}

void setError(ParserControl *parm, XtokErrorResp *e)
{
#if DEBUG
   if (do_debug)
       fprintf(stderr, "error:: code:%s description:%s\n", 
                                 e->code, e->description);
#endif
   parm->respHdr.errCode = atoi(e->code);
   parm->respHdr.description = XmlToAsciiStr(e->description);
}

void setReturnArgs(ParserControl *parm, XtokParamValues *ps)
{
    CMPIValue value;
    XtokParamValue *outParam=NULL;
    CMPIArgs *args = NULL;
   
    /* Process OUT parameters */
    outParam = ps->first;

    if (outParam) { 
        args = newCMPIArgs(NULL);

        while (outParam) {
            value = str2CMPIValue(outParam->type, outParam->data.value.data.value, &outParam->data.valueRef);

            /* Add it to the args list */
            args->ft->addArg ( args, outParam->name, &value, outParam->type);
            native_release_CMPIValue(outParam->type,&value);
            outParam = outParam->next;
        }
        parm->respHdr.outArgs = args;
        /* Note : Freeing of list will be done by
        * parser_heap_term() routine.
        */
    }
}
