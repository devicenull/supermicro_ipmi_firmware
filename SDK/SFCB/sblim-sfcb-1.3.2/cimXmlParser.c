
/*
 * $Id: cimXmlParser.c,v 1.29 2008/08/15 19:05:13 buccella Exp $
 *
 *  © Copyright IBM Corp. 2005, 2007
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
 *
 * CIM XML lexer for sfcb to be used in connection with cimXmlOps.y.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <cmpidt.h>

#include "cimXmlParser.h"
#include "cimXmlOps.h"
#include "cimXmlRequest.h"
#include "trace.h"


static int attrsOk(XmlBuffer * xb, const XmlElement * e, XmlAttr * r,
                   const char *tag, int etag);
static char *getValue(XmlBuffer * xb, const char *v);
extern int yyparse(void *);

typedef struct tags {
   const char *tag;
   int (*process) (YYSTYPE *, ParserControl * parm);
   int etag;
} Tags;



static void Throw(XmlBuffer * xb, char *msg)
{
   printf("*** Error: %s\n", msg);
   exit(1);
}

static XmlBuffer *newXmlBuffer(char *s)
{
   XmlBuffer *xb = (XmlBuffer *) malloc(sizeof(XmlBuffer));
   xb->base = xb->cur = (char *) strdup(s);
   xb->last = xb->cur + strlen(xb->cur);
   xb->nulledChar = 0;
   xb->eTagFound = 0;
   xb->etag = 0;
   return xb;
}

static void skipWS(XmlBuffer * xb)
{
   static int c = 0;
   c++;
   while (*xb->cur <= ' ' && xb->last > xb->cur)
      xb->cur++;
}

static int getChars(XmlBuffer * xb, const char *s)
{
   int l = strlen(s);
   if (strncmp(xb->cur, s, l) == 0) {
      xb->cur += l;
      return 1;
   }
   return 0;
}

static int getChar(XmlBuffer * xb, const char c)
{
   if (*xb->cur++ == c)
      return *(xb->cur - 1);
   xb->cur--;
   return 0;
}

/* skip nulled char and whitespace, 
   see if we're on a new tag and return a pointer, 
   or NULL otherwise
 */
static char *nextTag(XmlBuffer * xb)
{
   if (xb->nulledChar) {
      xb->nulledChar = 0;
      return xb->cur + 1;
   }
   skipWS(xb);
   if (*xb->cur == '<')
      return xb->cur + 1;
   return NULL;
}

/* compares two strings
   return 1: n begins with t and following char is alphanumeric
   return 0: otherwise
 */
static int nextEquals(const char *n, const char *t)
{
   int l = strlen(t);
   if (strncmp(n, t, l) == 0) {
      if (!isalnum(*(n + l))) {
         return 1;
      }
   }
   return 0;
}

/* scan until we find the end of the tag ('>') */
static char skipTag(XmlBuffer * xb)
{
   while (*xb->cur != '>' && xb->last > xb->cur)
      xb->cur++;
   xb->cur++;
   return *xb->cur;
}

/*
static int xmlHdr(XmlBuffer *xb) {
   XmlElement elm[3]={{"version"},{"encoding"},{NULL}};
   XmlAttr vals[2];
   xb->eTagFound = 0;

   skipWS(xb);
   if (getChars(xb,"<?xml")) {
     if (attrsOk(xb,elm,vals,"?xml",ZTOK_CIM)) return 1;
   }
   return 0;
}

static int skipElement(XmlBuffer *xb, const char *t) {
   char eTag[256]="</";
   strcat(eTag,t);
   strcat(eTag,">");
   char *e=strstr(xb->cur,eTag);
   if (e) {
      xb->cur=e+strlen(eTag);
      return 1;
   }
   return 0;
}

static int testAttr(XmlBuffer *xb, char *w, char *val, int xCase) {
   char *end=strchr(xb->cur,'>'),*tcur=xb->cur,dlm,*a=NULL;
   int l=strlen(val),rv=0;

   *end=0;
   a=strstr(xb->cur,w);
   if (a) {
     xb->cur=a+strlen(w);
     skipWS(xb);
     dlm=*(xb->cur);
     if ((xCase && strncmp(xb->cur+1,val,l)==0) || (xCase==0 && strncasecmp(xb->cur+1,val,l)==0)) {
       if (*(xb->cur+l+1)==dlm) rv=1;
     }
   }
   *end='>';
   xb->cur=tcur;
   return rv;
}

static int endTag(XmlBuffer *xb, const char *t, const XmlAttr *a) {
   char *start=xb->cur;
   if (a && xb->eTagFound) return 1;
   skipWS(xb);
   if (getChars(xb,"</")) {
      skipWS(xb);
      if (getWord(xb,t,1)) {
         skipWS(xb);
         if (getChar(xb,'>')) return 1;
      }
   }
   xb->cur=start;
   return 0;
}

*/

static int getWord(XmlBuffer * xb, const char *w, int xCase)
{
   int l = strlen(w);
   if ((xCase && strncmp(xb->cur, w, l) == 0)
       || (xCase == 0 && strncasecmp(xb->cur, w, l) == 0)) {
      if (!isalnum(*(xb->cur + l))) {
         xb->cur += l;
         return 1;
      }
   }
   return 0;
}

static int tagEquals(XmlBuffer * xb, const char *t)
{
   char *start = NULL;
   int sz = 0;
   if (*xb->cur == 0) {
      xb->cur++;
      sz = 1;
   }                            // why is this needed ?
   else
      start = xb->cur;
   skipWS(xb);
   if (sz || getChar(xb, '<')) {
      skipWS(xb);
      if (getWord(xb, t, 1))
         return 1;
   }
   else {
      printf("OOOPS\n");
   }
   xb->cur = start;
   return 0;
}

static int attrsOk(XmlBuffer * xb, const XmlElement * e, XmlAttr * r,
                   const char *tag, int etag)
{
   unsigned int n;
   char *ptr, wa[32];
   char msg1[] = { "Unknown attribute in list for " };
   char msg2[] = { "Bad attribute list for " };
   char word[11];

   for (n = 0; (e + n)->attr; n++)
      wa[n] = 0;

   xb->eTagFound = 0;
   for (skipWS(xb); isalpha(*xb->cur); skipWS(xb)) {
//      for (n=0; n < a.size(); n++) {
      for (n = 0; (e + n)->attr; n++) {
         if (wa[n] == 1)
            continue;
         if (getWord(xb, (e + n)->attr, 0)) {
            if (!isalnum(*xb->cur)) {
               skipWS(xb);
               if (getChar(xb, '=')) {
                  (r + n)->attr = getValue(xb, (e + n)->attr);
                  wa[n] = 1;
                  goto ok;
               }
               else
                  Throw(xb, "'=' expected in attribute list");
            }
         }
      }
      strncpy(word, xb->cur, 10);
      word[10] = 0;
      ptr = (char *) alloca(strlen(tag) + strlen(msg1) + 8 + 20);
      strcpy(ptr, msg1);
      strcat(ptr, tag);
      strcat(ptr, " (");
      strcat(ptr, word);
      strcat(ptr, ")");
      Throw(xb, ptr);
    ok:;
   }

   if (getChars(xb, "/>")) {
      xb->eTagFound = 1;
      xb->etag = etag;
      return 1;
   }

   else if (getChar(xb, '>'))
      return 1;

   else if (getChars(xb, "?>") && strcmp(tag, "?xml") == 0) {
      xb->eTagFound = 1;
      xb->etag = etag;
      return 1;
   }

   ptr = (char *) alloca(strlen(tag) + strlen(msg2) + 48);
   strcpy(ptr, msg2);
   strcat(ptr, tag);
   strcat(ptr, ": ");
   strncpy(word, xb->cur, 10);
   strcat(ptr, word);
   strcat(ptr, tag);
   Throw(xb, ptr);
   return -1;
}

/* Is this Broken?  I guess we don't allow escaping the quotes */

static char *getValue(XmlBuffer * xb, const char *v)
{
   skipWS(xb);
   char dlm = 0;
   char *start = NULL;
   if ((dlm = getChar(xb, '"')) || (dlm = getChar(xb, '\''))) {
      start = xb->cur;
      while (*xb->cur != dlm) {
         xb->cur++;
      }
      *xb->cur = 0;
      xb->cur++;
      return start;
   }
   return NULL;
}

static struct _xmlescape {
  char * escaped;
  char   unescaped;
  int    len;
} xmlEscapeTab[] = {
    { "&apos;", '\'', 6 },
    { "&quot;", '\"', 6 },
    { "&amp;", '&', 5 },
    { "&gt;", '>', 4 },
    { "&lt;", '<', 4 },
    { "&#x20;", ' ', 6 },
    { "&#xa;", '\n', 5 },
};

static int xmlUnescape(char *buf, char *end)
{
  int i;
  for (i=0; i < sizeof(xmlEscapeTab)/sizeof(struct _xmlescape);i++) {
    if (end-buf >= xmlEscapeTab[i].len && 
	strncmp(buf,xmlEscapeTab[i].escaped,xmlEscapeTab[i].len) == 0) {
      *buf = xmlEscapeTab[i].unescaped;
      memmove(buf+1,buf+xmlEscapeTab[i].len,end-buf-xmlEscapeTab[i].len+1);
      return xmlEscapeTab[i].len-1;
    }
  }
  return 0;
}

static char *getContent(XmlBuffer * xb)
{
   char *start = xb->cur,*end, *help;

    /* if this is an ending tag, return NULL (no content) */
    if (xb->eTagFound)
      return NULL;

   /* scan through until we hit an '<' or end of buffer*/
   for (; *xb->cur != '<' && xb->cur < xb->last; xb->cur++);

   /* store the char we found, set it to null to use as a marker in the code below */
   xb->nulledChar = *(xb->cur);
   *(xb->cur) = 0;

   /* skip leading blanks */
   while (*start && *start<=' ') start++;
   if (start == NULL) return "";  // should check for *start == NULL instead? start==NULL shouldn't happen, ever.
   end=xb->cur;
   /* strip trailing blanks */
   while (*(end-1)<=' ') {
     *(end-1) = 0;
     end -= 1;
   }
   /* unescape */
   help = start;
   while (help < end) {
     if (*help == '&') {
       end -= xmlUnescape(help, end);
     }
     help += 1;
   }
   return start;
}



typedef struct types {
   char *str;
   CMPIType type;
} Types;

static Types types[] = {
   {"boolean", CMPI_boolean},
   {"string", CMPI_string},
   {"char16", CMPI_char16},
   {"uint8", CMPI_uint8},
   {"sint8", CMPI_sint8},
   {"uint16", CMPI_uint16},
   {"sint16", CMPI_sint16},
   {"uint32", CMPI_uint32},
   {"sint32", CMPI_sint32},
   {"uint64", CMPI_uint64},
   {"sint64", CMPI_sint64},
   {"datetime", CMPI_dateTime},
   {"real32", CMPI_real32},
   {"real64", CMPI_real64},
   {NULL}
};

static int num_types = sizeof(types)/sizeof(Types) - 1;


//static XmlBuffer* xmb;

static int procCim(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"CIMVERSION"},
      {"DTDVERSION"},
      {NULL}
   };
   XmlAttr attr[2];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "CIM")) {
      if (attrsOk(parm->xmb, elm, attr, "CIM", ZTOK_CIM))
         return XTOK_CIM;
   }
   return 0;
}

static int procMessage(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"ID"},
      {"PROTOCOLVERSION"},
      {NULL}
   };
   XmlAttr attr[2];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "MESSAGE")) {
      if (attrsOk(parm->xmb, elm, attr, "MESSAGE", ZTOK_MESSAGE)) {
         lvalp->xtokMessage.id = attr[0].attr;
         parm->reqHdr.id = attr[0].attr;
         return XTOK_MESSAGE;
      }
   }
   return 0;
}

static int procSimplereq(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "SIMPLEREQ")) {
      if (attrsOk(parm->xmb, elm, attr, "SIMPLEREQ", ZTOK_SIMPLEREQ))
         return XTOK_SIMPLEREQ;
   }
   return 0;
}

static int unsupported(ParserControl * parm)
{
   longjmp(parm->env,5);
   return 0;
}

static int procMethodCall(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {NULL}
   };
   XmlAttr attr[1];

   parm->reqHdr.methodCall = 1;
   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "METHODCALL")) {
      if (attrsOk(parm->xmb, elm, attr, "METHODCALL", ZTOK_METHODCALL)) {
         lvalp->xtokMethodCall.method=parm->reqHdr.iMethod = attr[0].attr;
         return XTOK_METHODCALL;
         // return unsupported(parm);
      }   
   }
   return 0;
}

static int procImethodCall(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {NULL}
   };
   XmlAttr attr[1];

   parm->reqHdr.methodCall = 0;
   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "IMETHODCALL")) {
      if (attrsOk(parm->xmb, elm, attr, "IMETHODCALL", ZTOK_IMETHODCALL)) {
         parm->reqHdr.iMethod = attr[0].attr;
         if (strcasecmp(attr[0].attr, "getInstance") == 0)
            return XTOK_GETINSTANCE;
         if (strcasecmp(attr[0].attr, "getClass") == 0)
            return XTOK_GETCLASS;
         if (strcasecmp(attr[0].attr, "enumerateClassNames") == 0)
            return XTOK_ENUMCLASSNAMES;
         if (strcasecmp(attr[0].attr, "enumerateClasses") == 0)
            return XTOK_ENUMCLASSES;
         if (strcasecmp(attr[0].attr, "enumerateInstanceNames") == 0)
            return XTOK_ENUMINSTANCENAMES;
         if (strcasecmp(attr[0].attr, "enumerateInstances") == 0)
            return XTOK_ENUMINSTANCES;
         if (strcasecmp(attr[0].attr, "deleteInstance") == 0)
            return XTOK_DELETEINSTANCE;
         if (strcasecmp(attr[0].attr, "createInstance") == 0)
            return XTOK_CREATEINSTANCE;
         if (strcasecmp(attr[0].attr, "modifyInstance") == 0)
            return XTOK_MODIFYINSTANCE;
         if (strcasecmp(attr[0].attr, "associatorNames") == 0)
            return XTOK_ASSOCIATORNAMES;
         if (strcasecmp(attr[0].attr, "associators") == 0)
            return XTOK_ASSOCIATORS;
         if (strcasecmp(attr[0].attr, "referenceNames") == 0)
            return XTOK_REFERENCENAMES;
         if (strcasecmp(attr[0].attr, "references") == 0)
            return XTOK_REFERENCES;
         if (strcasecmp(attr[0].attr, "execQuery") == 0)
            return XTOK_EXECQUERY;
         if (strcasecmp(attr[0].attr, "createClass") == 0)
            return XTOK_CREATECLASS;
         if (strcasecmp(attr[0].attr, "deleteClass") == 0)
            return XTOK_DELETECLASS;
         if (strcasecmp(attr[0].attr, "deleteClass") == 0)
            return unsupported(parm);
         if (strcasecmp(attr[0].attr, "getProperty") == 0)
            return XTOK_GETPROPERTY;
         if (strcasecmp(attr[0].attr, "setProperty") == 0)
            return XTOK_SETPROPERTY;
         if (strcasecmp(attr[0].attr, "getQualifier") == 0)
            return XTOK_GETQUALIFIER;
         if (strcasecmp(attr[0].attr, "setQualifier") == 0)
            return XTOK_SETQUALIFIER;
         if (strcasecmp(attr[0].attr, "deleteQualifier") == 0)
            return XTOK_DELETEQUALIFIER;
         if (strcasecmp(attr[0].attr, "enumerateQualifiers") == 0)
            return XTOK_ENUMQUALIFIERS;
      }
   }
   return 0;
}

static int procLocalNameSpacePath(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "LOCALNAMESPACEPATH")) {
      if (attrsOk
          (parm->xmb, elm, attr, "LOCALNAMESPACEPATH",
           ZTOK_LOCALNAMESPACEPATH)) {
         return XTOK_LOCALNAMESPACEPATH;
      }
   }
   return 0;
}

static int procLocalClassPath(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "LOCALCLASSPATH")) {
      if (attrsOk
          (parm->xmb, elm, attr, "LOCALCLASSPATH",
           ZTOK_LOCALCLASSPATH)) {
         return XTOK_LOCALCLASSPATH;
      }
   }
   return 0;
}

static int procLocalInstancePath(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "LOCALINSTANCEPATH")) {
      if (attrsOk
          (parm->xmb, elm, attr, "LOCALINSTANCEPATH",
           ZTOK_LOCALINSTANCEPATH)) {
         return XTOK_LOCALINSTANCEPATH;
      }
   }
   return 0;
}

static int procNameSpace(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {NULL}
   };
   XmlAttr attr[1];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "NAMESPACE")) {
      if (attrsOk(parm->xmb, elm, attr, "NAMESPACE", ZTOK_NAMESPACE)) {
         lvalp->xtokNameSpace.ns = attr[0].attr;
         return XTOK_NAMESPACE;
      }
   }
   return 0;
}

static int procParamValue(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {"PARAMTYPE"},
      {"EmbeddedObject"},
      {NULL}
   };
   XmlAttr attr[3];
   int i, m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PARAMVALUE")) {
      if (attrsOk(parm->xmb, elm, attr, "PARAMVALUE", ZTOK_PARAMVALUE)) {
         lvalp->xtokParamValue.name = attr[0].attr;
         lvalp->xtokParamValue.type = 0;
         if (attr[1].attr) {
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokParamValue.type = types[i].type;
                  break;
               }
            }
	    // is this right?  why isn't reference covered by the types array in the loop above?
            if (lvalp->xtokParamValue.type==0) {
               if (strcasecmp(attr[1].attr, "reference") == 0) 
                  lvalp->xtokParamValue.type = CMPI_ref;
            }
         }
         if (attr[2].attr) {
            if(strcasecmp(attr[2].attr, "instance") == 0
               || strcasecmp(attr[2].attr, "object") == 0) {
               lvalp->xtokParamValue.type = CMPI_instance;

               /* unescape */
               char *help = parm->xmb->cur;
               char *end = strstr(help, "</PARAMVALUE");
               while (help < end) {
                 if (*help == '&') {
                   int fewer = xmlUnescape(help, end);
                   end -= fewer;
                   memset(end, ' ', fewer);
                 }
                 help += 1;
               }

            } else {
               Throw(NULL, "Invalid value for attribute EmbeddedObject");
            }
         }
         return XTOK_PARAMVALUE;
      }
   }
   return 0;
}

typedef struct iParm {
   char *name;
   int tag;
} IParm;

static IParm iParms[] = {
   {"classname", XTOK_IP_CLASSNAME},
   {"newinstance", XTOK_IP_INSTANCE},
   {"modifiedinstance", XTOK_IP_MODIFIEDINSTANCE},
   {"instancename", XTOK_IP_INSTANCENAME},
   {"objectname", XTOK_IP_OBJECTNAME},
   {"localonly", XTOK_IP_LOCALONLY},
   {"IncludeQualifiers", XTOK_IP_INCLUDEQUALIFIERS},
   {"IncludeClassOrigin", XTOK_IP_INCLUDECLASSORIGIN},
   {"deepInheritance", XTOK_IP_DEEPINHERITANCE},
   {"assocclass", XTOK_IP_ASSOCCLASS},
   {"resultclass", XTOK_IP_RESULTCLASS},
   {"role", XTOK_IP_ROLE},
   {"resultrole", XTOK_IP_RESULTROLE},
   {"propertylist", XTOK_IP_PROPERTYLIST},
   {"querylanguage", XTOK_IP_QUERYLANG},
   {"query", XTOK_IP_QUERY},
   {"newclass", XTOK_IP_CLASS},
   {"qualifierdeclaration", XTOK_IP_QUALIFIERDECLARATION},
   {"qualifiername", XTOK_IP_QUALIFIERNAME},
   {"propertyname", XTOK_IP_PROPERTYNAME},
   {"newValue", XTOK_IP_NEWVALUE}
};

static int procIParamValue(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {NULL}
   };
   XmlAttr attr[1];
   int i, m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "IPARAMVALUE")) {
      if (attrsOk(parm->xmb, elm, attr, "IPARAMVALUE", ZTOK_IPARAMVALUE)) {
         for (i = 0, m = sizeof(iParms) / sizeof(IParm); i < m; i++) {
            if (strcasecmp(attr[0].attr, iParms[i].name) == 0) {
               return iParms[i].tag;
            }
         }
      }
   }
   return 0;
}

static int procClassName(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {NULL}
   };
   XmlAttr attr[1];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "CLASSNAME")) {
      if (attrsOk(parm->xmb, elm, attr, "CLASSNAME", ZTOK_CLASSNAME)) {
         lvalp->className = attr[0].attr;
         return XTOK_CLASSNAME;
      }
   }
   return 0;
}

static int procInstanceName(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"CLASSNAME"},
      {NULL}
   };
   XmlAttr attr[1];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "INSTANCENAME")) {
      if (attrsOk(parm->xmb, elm, attr, "INSTANCENAME", ZTOK_INSTANCENAME)) {
         lvalp->xtokInstanceName.className = attr[0].attr;
         return XTOK_INSTANCENAME;
      }
   }
   return 0;
}

static int procKeyBinding(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {NULL}
   };
   XmlAttr attr[1];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "KEYBINDING")) {
      if (attrsOk(parm->xmb, elm, attr, "KEYBINDING", ZTOK_KEYBINDING)) {
         lvalp->xtokInstanceName.className = attr[0].attr;
         return XTOK_KEYBINDING;
      }
   }
   return 0;
}

static int procInstance(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"CLASSNAME"},
      {NULL}
   };
   XmlAttr attr[1];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "INSTANCE")) {
      if (attrsOk(parm->xmb, elm, attr, "INSTANCE", ZTOK_INSTANCE)) {
         lvalp->xtokInstance.className = attr[0].attr;
         return XTOK_INSTANCE;
      }
   }
   return 0;
}

static int procClass(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"NAME"},
      {"SUPERCLASS"},
      {NULL}
   };
   XmlAttr attr[2];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "CLASS")) {
      if (attrsOk(parm->xmb, elm, attr, "CLASS", ZTOK_CLASS)) {
         lvalp->xtokClass.className = attr[0].attr;
         lvalp->xtokClass.superClass = attr[1].attr;
         return XTOK_CLASS;
      }
   }
   return 0;
}

static int procKeyValue(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {"VALUETYPE"},
      {"TYPE"},
      {NULL}
   };
   XmlAttr attr[2] = {
      {NULL}
   };
   char *val;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "KEYVALUE")) {
      if (attrsOk(parm->xmb, elm, attr, "KEYVALUE", ZTOK_KEYVALUE)) {
         val = getContent(parm->xmb);
         lvalp->xtokKeyValue.valueType = attr[0].attr;
         lvalp->xtokKeyValue.value = val;
         return XTOK_KEYVALUE;
      }
   }
   return 0;
}

static int procHost(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "HOST")) {
      if (attrsOk(parm->xmb, elm, attr, "HOST", ZTOK_HOST)) {
         lvalp->xtokHost.host = getContent(parm->xmb);
         return XTOK_HOST;
      }
   }
   return 0;
}

static int procValue(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "VALUE")) {
      char *v;
      if (attrsOk(parm->xmb, elm, attr, "VALUE", ZTOK_VALUE)) {
         v=getContent(parm->xmb); /* v is a pointer to where the content starts*/
         lvalp->xtokValue.value = v;
         return XTOK_VALUE;
      }
   }
   return 0;
}

static int procCdata(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   
   if (tagEquals(parm->xmb, "![CDATA[")) {
      parm->xmb->cur[0] = '>';   	
      char *v;
      if (attrsOk(parm->xmb, elm, attr, "![CDATA[", ZTOK_CDATA)) {
      	 
      	 v=strstr(parm->xmb->cur, "]]>");
      	 if(v) {
      	 	v[0] = '<';
      	 	v[1] = '/';
      	 } else {
      	 	return 0;
      	 }
         return XTOK_CDATA;
      }
   }
   return 0;
}

static int procValueArray(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "VALUE.ARRAY")) {
      if (attrsOk(parm->xmb, elm, attr, "VALUE.ARRAY",
           ZTOK_VALUEARRAY)) {
         return XTOK_VALUEARRAY;
      }
   }
   return 0;
}

static int procValueRefArray(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "VALUE.REFARRAY")) {
      if (attrsOk(parm->xmb, elm, attr, "VALUE.REFARRAY",
           ZTOK_VALUEREFARRAY)) {
         return XTOK_VALUEREFARRAY;
      }
   }
   return 0;
}

static int procValueNamedInstance(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "VALUE.NAMEDINSTANCE")) {
      if (attrsOk(parm->xmb, elm, attr, "VALUE.NAMEDINSTANCE",
           ZTOK_VALUENAMEDINSTANCE)) {
         lvalp->xtokValue.value = getContent(parm->xmb);
         return XTOK_VALUENAMEDINSTANCE;
      }
   }
   return 0;
}

static int procInstancePath(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "INSTANCEPATH")) {
      if (attrsOk(parm->xmb, elm, attr, "INSTANCEPATH",
           ZTOK_INSTANCEPATH)) {
         lvalp->xtokValue.value = getContent(parm->xmb);
         return XTOK_INSTANCEPATH;
      }
   }
   return 0;
}

static int procNameSpacePath(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "NAMESPACEPATH")) {
      if (attrsOk(parm->xmb, elm, attr, "NAMESPACEPATH",
           ZTOK_NAMESPACEPATH)) {
         lvalp->xtokValue.value = getContent(parm->xmb);
         return XTOK_NAMESPACEPATH;
      }
   }
   return 0;
}

static int procValueReference(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
      {NULL}
   };
   XmlAttr attr[1];
   if (tagEquals(parm->xmb, "VALUE.REFERENCE")) {
      if (attrsOk(parm->xmb, elm, attr, "VALUE.REFERENCE",
           ZTOK_VALUEREFERENCE)) {
         lvalp->xtokValue.value = getContent(parm->xmb);
         return XTOK_VALUEREFERENCE;
      }
   }
   return 0;
}

static int procXml(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = { {"version"},
   {"encoding"},
   {"standalone"},
   {NULL}
   };
   XmlAttr attr[3];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "?xml")) {
      if (attrsOk(parm->xmb, elm, attr, "?xml", ZTOK_XML)) {
         return XTOK_XML;
      }
   }
   return 0;
}

static int procQualifierDeclaration(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
		{"NAME"},
		{"TYPE"},
		{"ISARRAY"},
		{"ARRAYSIZE"},
		{"OVERRIDABLE"},
		{"TOSUBCLASS"},
		{"TOINSTANCE"},
		{"TRANSLATABLE"},
		{NULL}
   };
   XmlAttr attr[8];
   int i, m;
   
   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "QUALIFIER.DECLARATION")) {
      if (attrsOk(parm->xmb, elm, attr, "QUALIFIER.DECLARATION", ZTOK_QUALIFIERDECLARATION)) {
         memset(&lvalp->xtokQualifierDeclaration, 0, sizeof(XtokQualifierDeclaration));
         lvalp->xtokQualifierDeclaration.name = attr[0].attr;
         lvalp->xtokQualifierDeclaration.type = (CMPIType) - 1;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokQualifierDeclaration.type = types[i].type;
                  break;
               }
            }
         if (attr[2].attr) {
            lvalp->xtokQualifierDeclaration.isarray = !strcasecmp(attr[2].attr, "true");
			lvalp->xtokQualifierDeclaration.isarrayIsSet = 1;            
         }
         else
         	lvalp->xtokQualifierDeclaration.isarrayIsSet = 0; //needed, because attr is mandatory
         if (attr[3].attr)
         	lvalp->xtokQualifierDeclaration.arraySize = atoi(attr[3].attr);
         if (attr[4].attr)
            lvalp->xtokQualifierDeclaration.overridable = !strcasecmp(attr[4].attr, "true");
         else
         	lvalp->xtokQualifierDeclaration.overridable = 1; //default true
         if (attr[5].attr)
            lvalp->xtokQualifierDeclaration.tosubclass = !strcasecmp(attr[5].attr, "true");
         else
         	lvalp->xtokQualifierDeclaration.tosubclass = 1; //default true
         if (attr[6].attr)
            lvalp->xtokQualifierDeclaration.toinstance = !strcasecmp(attr[6].attr, "true");
         if (attr[7].attr)
            lvalp->xtokQualifierDeclaration.translatable = !strcasecmp(attr[7].attr, "true");
         return XTOK_QUALIFIERDECLARATION;
      }
   }
   return 0;
}

static int procQualifier(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = { {"NAME"},
   {"TYPE"},
   {"PROPAGATED"},
   {"OVERRIDABLE"},
   {"TOSUBCLASS"},
   {"TOINSTANCE"},
   {"TRANSLATABLE"},
   {NULL}
   };
   XmlAttr attr[8];
   int i, m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "QUALIFIER")) {
      if (attrsOk(parm->xmb, elm, attr, "QUALIFIER", ZTOK_QUALIFIER)) {
         memset(&lvalp->xtokQualifier, 0, sizeof(XtokQualifier));
         lvalp->xtokQualifier.name = attr[0].attr;
         lvalp->xtokQualifier.type = (CMPIType) - 1;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokQualifier.type = types[i].type;
                  break;
               }
            }
         if (attr[2].attr)
            lvalp->xtokQualifier.propagated = !strcasecmp(attr[2].attr, "true");
         if (attr[3].attr)
            lvalp->xtokQualifier.overridable =
                !strcasecmp(attr[3].attr, "true");
         if (attr[4].attr)
            lvalp->xtokQualifier.tosubclass = !strcasecmp(attr[4].attr, "true");
         if (attr[5].attr)
            lvalp->xtokQualifier.toinstance = !strcasecmp(attr[5].attr, "true");
         if (attr[6].attr)
            lvalp->xtokQualifier.translatable =
                !strcasecmp(attr[6].attr, "true");
         return XTOK_QUALIFIER;
      }
   }
   return 0;
}

static int procScope(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = {
   {"CLASS"},
   {"ASSOCIATION"},
   {"REFERENCE"},
   {"PROPERTY"},
   {"METHOD"},
   {"PARAMETER"},
   {"INDICATION"},   
   {NULL}
   };
   XmlAttr attr[7];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "SCOPE")) {
      if (attrsOk(parm->xmb, elm, attr, "SCOPE", ZTOK_SCOPE)) {
               	
         memset(&lvalp->xtokScope, 0, sizeof(XtokScope));

         if (attr[0].attr)
            lvalp->xtokScope.class = !strcasecmp(attr[0].attr, "true");
         if (attr[1].attr)
            lvalp->xtokScope.association = !strcasecmp(attr[1].attr, "true");            
         if (attr[2].attr)
            lvalp->xtokScope.reference = !strcasecmp(attr[2].attr, "true");
         if (attr[3].attr)
            lvalp->xtokScope.property = !strcasecmp(attr[3].attr, "true");
         if (attr[4].attr)
            lvalp->xtokScope.method = !strcasecmp(attr[4].attr, "true");
         if (attr[5].attr)
            lvalp->xtokScope.parameter = !strcasecmp(attr[5].attr, "true");
         if (attr[6].attr)
            lvalp->xtokScope.indication = !strcasecmp(attr[6].attr, "true");            
         return XTOK_SCOPE;
      }
   }
   return 0;
}

static int procProperty(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = { {"NAME"},
   {"TYPE"},
   {"CLASSORIGIN"},
   {"PROPAGATED"},
   {"EmbeddedObject"},
   {NULL}
   };
   XmlAttr attr[5];
   int i, m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PROPERTY")) {
      attr[1].attr = NULL;
      lvalp->xtokProperty.valueType = (CMPIType) 1;
      if (attrsOk(parm->xmb, elm, attr, "PROPERTY", ZTOK_PROPERTY)) {
         memset(&lvalp->xtokProperty, 0, sizeof(XtokProperty));
         lvalp->xtokProperty.name = attr[0].attr;
         lvalp->xtokProperty.valueType = (CMPIType) - 1;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokProperty.valueType = types[i].type;
                  break;
               }
            }
         lvalp->xtokProperty.classOrigin = attr[2].attr;
         if (attr[3].attr)
            lvalp->xtokProperty.propagated = !strcasecmp(attr[3].attr, "true");
         if (attr[4].attr) {
            if(strcasecmp(attr[4].attr, "instance") == 0
               || strcasecmp(attr[4].attr, "object") == 0) {
               lvalp->xtokParamValue.type = CMPI_instance;
            } else {
               Throw(NULL, "Invalid value for attribute EmbeddedObject");
            }
         }
         return XTOK_PROPERTY;
      }
   }
   return 0;
}

static int procPropertyArray(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elmPA[] = { {"NAME"},
   {"TYPE"},
   {"CLASSORIGIN"},
   {"PROPAGATED"},
   {"ARRAYSIZE"},
   {"EmbeddedObject"},
   {NULL}
   };
   XmlAttr attr[6];
   int i,m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PROPERTY.ARRAY")) {
      if (attrsOk(parm->xmb, elmPA, attr, "PROPERTY.ARRAY", ZTOK_PROPERTYARRAY)) {
         memset(&lvalp->xtokProperty, 0, sizeof(XtokProperty));
         lvalp->xtokProperty.valueType = (CMPIType) - 1;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokProperty.valueType = types[i].type;
                  break;
               }
            }         
         lvalp->xtokProperty.valueType |= CMPI_ARRAY;
         lvalp->xtokProperty.name = attr[0].attr;
         lvalp->xtokProperty.classOrigin = attr[2].attr;
         if (attr[3].attr)
            lvalp->xtokProperty.propagated = !strcasecmp(attr[3].attr, "true");
         if (attr[5].attr) {
            if(strcasecmp(attr[5].attr, "instance") == 0
               || strcasecmp(attr[5].attr, "object") == 0) {
               lvalp->xtokParamValue.type = CMPI_instance;
            } else {
               Throw(NULL, "Invalid value for attribute EmbeddedObject");
            }
         }      	
         return XTOK_PROPERTYARRAY;
      }
   }
   return 0;
}

static int procPropertyReference(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = { {"NAME"},
   {"REFERENCECLASS"},
   {"CLASSORIGIN"},
   {"PROPAGATED"},
   {NULL}
   };
   XmlAttr attr[4];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PROPERTY.REFERENCE")) {
      attr[1].attr = NULL;
      if (attrsOk(parm->xmb, elm, attr, "PROPERTY.REFERENCE", ZTOK_PROPERTYREFERENCE)) {
         memset(&lvalp->xtokProperty, 0, sizeof(XtokProperty));
         lvalp->xtokProperty.valueType = CMPI_ref;
         lvalp->xtokProperty.name = attr[0].attr;
         lvalp->xtokProperty.referenceClass = attr[1].attr;
         lvalp->xtokProperty.classOrigin = attr[2].attr;
         if (attr[3].attr)
            lvalp->xtokProperty.propagated = !strcasecmp(attr[3].attr, "true");
         return XTOK_PROPERTYREFERENCE;
      }
   }
   return 0;
}

static int procMethod(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = { {"NAME"},
   {"TYPE"},
   {"CLASSORIGIN"},
   {"PROPAGATED"},
   {NULL}
   };
   XmlAttr attr[4];
   int i, m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "METHOD")) {
      attr[1].attr = NULL;
      if (attrsOk(parm->xmb, elm, attr, "METHOD", ZTOK_METHOD)) {
         memset(&lvalp->xtokMethod, 0, sizeof(XtokMethod));
         lvalp->xtokMethod.name = attr[0].attr;
         lvalp->xtokMethod.type = CMPI_null;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokMethod.type = types[i].type;
                  break;
               }
            }
         lvalp->xtokMethod.classOrigin = attr[2].attr;
         if (attr[3].attr)
            lvalp->xtokMethod.propagated = !strcasecmp(attr[3].attr, "true");
         return XTOK_METHOD;
      }
   }
   return 0;
}

static int procParam(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = 
   { {"NAME"},
     {"TYPE"},
     {NULL}
   };
   XmlAttr attr[2];
   int i,m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PARAMETER")) {
      attr[1].attr = NULL;
      if (attrsOk(parm->xmb, elm, attr, "PARAMETER", ZTOK_PARAM)) {
         memset(&lvalp->xtokParam, 0, sizeof(XtokParam));
         lvalp->xtokParam.pType = ZTOK_PARAM;
         lvalp->xtokParam.name = attr[0].attr;
         lvalp->xtokParam.type = CMPI_null;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokParam.type = types[i].type;
                  break;
               }
            }
         return XTOK_PARAM;
      }
   }
   return 0;
}

static int procParamArray(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = 
   { {"NAME"},
     {"TYPE"},
     {"ARRAYSIZE"},
     {NULL}
   };
   XmlAttr attr[3];
   int i,m;

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PARAMETER.ARRAY")) {
      attr[1].attr = NULL;
      if (attrsOk(parm->xmb, elm, attr, "PARAMETER.ARRAY", ZTOK_PARAM)) {
         memset(&lvalp->xtokParam, 0, sizeof(XtokParam));
         lvalp->xtokParam.pType = ZTOK_PARAMARRAY;
         lvalp->xtokParam.name = attr[0].attr;
         lvalp->xtokParam.type = CMPI_null;
         if (attr[1].attr)
            for (i = 0, m = num_types; i < m; i++) {
               if (strcasecmp(attr[1].attr, types[i].str) == 0) {
                  lvalp->xtokParam.type = types[i].type;
                  lvalp->xtokParam.type |=CMPI_ARRAY;
                  break;
               }
            }
         lvalp->xtokParam.arraySize = atoi(attr[2].attr);
         return XTOK_PARAM;
      }
   }
   return 0;
}

static int procParamRef(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = 
   { {"NAME"},
     {"REFERENCECLASS"},
     {NULL}
   };
   XmlAttr attr[2];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PARAMETER.REFERENCE")) {
      attr[1].attr = NULL;
      if (attrsOk(parm->xmb, elm, attr, "PARAMETER.REFERENCE", ZTOK_PARAM)) {
         memset(&lvalp->xtokParam, 0, sizeof(XtokParam));
         lvalp->xtokParam.pType = ZTOK_PARAMREF;
         lvalp->xtokParam.name = attr[0].attr;
         lvalp->xtokParam.refClass = attr[1].attr;
         lvalp->xtokParam.type = CMPI_ref;
         return XTOK_PARAM;
      }
   }
   return 0;
}

static int procParamRefArray(YYSTYPE * lvalp, ParserControl * parm)
{
   static XmlElement elm[] = 
   { {"NAME"},
     {"REFERENCECLASS"},
     {"ARRAYSIZE"},
     {NULL}
   };
   XmlAttr attr[3];

   memset(attr, 0, sizeof(attr));
   if (tagEquals(parm->xmb, "PARAMETER.REFARRAY")) {
      attr[1].attr = NULL;
      if (attrsOk(parm->xmb, elm, attr, "PARAMETER.REFARRAY", ZTOK_PARAM)) {
         memset(&lvalp->xtokParam, 0, sizeof(XtokParam));
         lvalp->xtokParam.pType = ZTOK_PARAMREFARRAY;
         lvalp->xtokParam.name = attr[0].attr;
         lvalp->xtokParam.refClass = attr[1].attr;
         lvalp->xtokParam.arraySize = atoi(attr[2].attr);
         lvalp->xtokParam.type = CMPI_refA;
         return XTOK_PARAM;
      }
   }
   return 0;
}


static Tags tags[] = {
   {"CIM", procCim, ZTOK_CIM},
   {"MESSAGE", procMessage, ZTOK_MESSAGE},
   {"SIMPLEREQ", procSimplereq, ZTOK_SIMPLEREQ},
   {"IMETHODCALL", procImethodCall, ZTOK_IMETHODCALL},
   {"METHODCALL", procMethodCall, ZTOK_METHODCALL},
   {"LOCALNAMESPACEPATH", procLocalNameSpacePath, ZTOK_LOCALNAMESPACEPATH},
   {"LOCALINSTANCEPATH", procLocalInstancePath, ZTOK_LOCALINSTANCEPATH},
   {"LOCALCLASSPATH", procLocalClassPath, ZTOK_LOCALCLASSPATH},
   {"NAMESPACEPATH", procNameSpacePath, ZTOK_NAMESPACEPATH},
   {"NAMESPACE", procNameSpace, ZTOK_NAMESPACE},
   {"IPARAMVALUE", procIParamValue, ZTOK_IPARAMVALUE},
   {"PARAMVALUE", procParamValue, ZTOK_PARAMVALUE},
   {"CLASSNAME", procClassName, ZTOK_CLASSNAME},
   {"VALUE.ARRAY", procValueArray, ZTOK_VALUEARRAY},
   {"VALUE.NAMEDINSTANCE", procValueNamedInstance, ZTOK_VALUENAMEDINSTANCE},
   {"VALUE.REFERENCE", procValueReference, ZTOK_VALUEREFERENCE},
   {"VALUE.REFARRAY", procValueRefArray, ZTOK_VALUEREFARRAY},
   {"VALUE", procValue, ZTOK_VALUE},
   {"HOST", procHost, ZTOK_HOST},
   {"KEYVALUE", procKeyValue, ZTOK_KEYVALUE},
   {"KEYBINDING", procKeyBinding, ZTOK_KEYBINDING},
   {"INSTANCEPATH", procInstancePath, ZTOK_INSTANCEPATH},
   {"INSTANCENAME", procInstanceName, ZTOK_INSTANCENAME},
   {"INSTANCE", procInstance, ZTOK_INSTANCE},
   {"PROPERTY.REFERENCE", procPropertyReference, ZTOK_PROPERTYREFERENCE},
   {"PROPERTY.ARRAY", procPropertyArray, ZTOK_PROPERTYARRAY},
   {"PROPERTY", procProperty, ZTOK_PROPERTY},
   {"QUALIFIER.DECLARATION", procQualifierDeclaration, ZTOK_QUALIFIERDECLARATION},   
   {"QUALIFIER", procQualifier, ZTOK_QUALIFIER},
   {"SCOPE", procScope, ZTOK_SCOPE},   
   {"PARAMETER.ARRAY", procParamArray, ZTOK_PARAMARRAY},
   {"PARAMETER.REFERENCE", procParamRef, ZTOK_PARAMREF},
   {"PARAMETER.REFARRAY", procParamRefArray, ZTOK_PARAMREFARRAY},
   {"PARAMETER", procParam, ZTOK_PARAM},
   {"METHOD", procMethod, ZTOK_METHOD},
   {"CLASS", procClass, ZTOK_CLASS},
   {"?xml", procXml, ZTOK_XML},
   {"![CDATA[", procCdata, ZTOK_CDATA},
   {"", procCdata, ZTOK_CDATA},      
};

int yylex(YYSTYPE * lvalp, ParserControl * parm)
{
   int i, m, rc;
   char *next;

   _SFCB_ENTER(TRACE_XMLPARSING, "yylex");
   
   for (;;) {
      next = nextTag(parm->xmb);
      if (next == NULL) {
         _SFCB_RETURN(0);
      }   
      _SFCB_TRACE(1, ("--- token: %.32s\n",next));

      /* found a tag, reset eTagFound flag, return the token for the tag that was found */
      if (parm->xmb->eTagFound) {
         parm->xmb->eTagFound = 0;
         _SFCB_RETURN(parm->xmb->etag);
      }

      /* found an ending tag, loop from 0-#ofTags, return the token for the matching opening tag */
      if (*next == '/') {
	for (i = 0, m = sizeof(tags)/sizeof(Tags); i < m; i++) {
            if (nextEquals(next + 1, tags[i].tag) == 1) {
               skipTag(parm->xmb);
               _SFCB_RETURN(tags[i].etag);
            }
         }
      }

      else {
	/* skip comment section */
         if (strncmp(parm->xmb->cur, "<!--", 4) == 0) {
            parm->xmb->cur = strstr(parm->xmb->cur, "-->") + 3;
            continue;
         }
         for (i = 0, m = sizeof(tags)/sizeof(Tags); i < m; i++) {
            if (nextEquals(next, tags[i].tag) == 1) {

               // fprintf(stderr, " yylex| processing for: %.10s\n", next);
               rc=tags[i].process(lvalp, parm); /* call a procXXX fn based on tag name */
               _SFCB_RETURN(rc);
            }
         }
      }
      break;
   }
   _SFCB_RETURN(0);
}

int yyerror(char *s)
{
   printf("*-* yyerror: %s\n", s);
   return 5;
   //   exit(5);
}

RequestHdr scanCimXmlRequest(char *xmlData)
{
   ParserControl control;

   XmlBuffer *xmb = newXmlBuffer(xmlData);
   control.xmb = xmb;
   control.reqHdr.xmlBuffer = xmb;
   control.reqHdr.cimRequest = NULL;
   control.reqHdr.iMethod = NULL;
   control.paramValues.last = control.paramValues.first = NULL;
   control.properties.last = control.properties.first = NULL;
   control.qualifiers.last = control.qualifiers.first = NULL;
   control.methods.last = control.methods.first = NULL;
   control.Qs=0;
   control.Ps=0;
   control.Ms=0;
   control.MPs=0;
   control.MQs=0;
   control.MPQs=0;

   if (setjmp(control.env)) {
//      printf("--- setjmp caught !!\n");
      control.reqHdr.opType=0;
      return control.reqHdr;
   }

   control.reqHdr.rc = yyparse(&control);

   return control.reqHdr;
}

static void freeReference(XtokValueReference *op);

static void freeNameSpacePath(XtokNameSpacePath *op)
{
  if (op->nameSpacePath) {
    free(op->nameSpacePath);
  }
}

static void freeKeyBinding(XtokKeyBinding *op)
{
  if (op->type && strcmp(op->type,"ref")==0) {
    freeReference(&op->ref);
  }
}

static void freeKeyBindings(XtokKeyBindings *op)
{
   int i;  
   if (op->keyBindings) {
     for (i=0; i<op->next; i++) {
       freeKeyBinding(op->keyBindings+i);
     }
     free(op->keyBindings);
  }
}

static void freeInstanceName(XtokInstanceName *op)
{
  freeKeyBindings(&op->bindings);
}

static void freeLocalInstancePath(XtokLocalInstancePath *op)
{
  if (op->path) {
    free(op->path);
  }
  freeInstanceName(&op->instanceName);
}

static void freeInstancePath(XtokInstancePath *op)
{
  freeNameSpacePath(&op->path);
  freeInstanceName(&op->instanceName );
}

static void freeReference(XtokValueReference *op)
{
  switch (op->type) {
  case typeValRef_InstanceName:
    freeInstanceName(&op->instanceName);
    break;
  case typeValRef_InstancePath:
    freeInstancePath(&op->instancePath);
    break;
  case typeValRef_LocalInstancePath:
    freeLocalInstancePath(&op->localInstancePath);
    break;
  }
}

static void freeRefArray(XtokValueRefArray *op)
{
  int i;
  if (op->values) {
    for (i=0; i<op->next; i++) {
      freeReference(op->values+i);
    }
    free(op->values);
  }
}

static void freeInstance(XtokInstance *op);

static void freeValue(XtokValue *op) {
  if(op->type == typeValue_Instance && op->instance) {
    freeInstance(op->instance);
    free(op->instance);
  }
  /*else if(op->type == typeValue_Class && op->class) {
    freeClass(op->class);
    free(op->class);
  }*/ 
}

static void freeArray(XtokValueArray *op)
{
  int i;
  if (op->values) {
    for(i=0; i<op->next; i++) {
      freeValue(op->values+i);
    }
    free(op->values);
  }
}

static void freeParamValue(XtokParamValue *op)
{
  if ((op->type & CMPI_refA) == CMPI_refA) {
    freeRefArray(&op->valueRefArray);
  } else if (op->type & CMPI_ARRAY) {
    freeArray(&op->valueArray);
  } else if ((op->type & CMPI_ref) == CMPI_ref) {
    freeReference(&op->valueRef);
  } else if ((op->type & CMPI_instance) == CMPI_instance 
             || (op->type & CMPI_class) == CMPI_class) {
    freeValue(&op->value);
  }
}

static void freeParamValues(XtokParamValues *op)
{
  XtokParamValue * pv = op->first , *dv;
  while (pv) {
    freeParamValue(pv);
    dv = pv;
    pv = pv->next;
    free(dv);
  }
}

static void freeQualifier(XtokQualifier *op)
{
  if (op->type & CMPI_ARRAY) {
    freeArray(&op->valueArray);
  }
}

static void freeQualifiers(XtokQualifiers* op)
{
  XtokQualifier * qv = op->first , *dv;
  while (qv) {
    freeQualifier(qv);
    dv = qv;
    qv = qv->next;
    free(dv);
  }
}

static void freeInstance(XtokInstance *op);

static void freeProperty(XtokProperty *op)
{
  if (op->valueType & CMPI_ARRAY) {    
    freeArray(&op->val.list);
  } else if ((op->valueType & CMPI_ref) == CMPI_ref) {
    freeReference(&op->val.ref);
  }
  if(op->val.val.type==typeValue_Instance && op->val.val.instance) {
    freeInstance(op->val.val.instance);
    free(op->val.val.instance);
  }
  freeQualifiers(&op->val.qualifiers);
}

static void freeProperties(XtokProperties* op)
{
  XtokProperty * pv = op->first , *dv;
  while (pv) {
    freeProperty(pv);
    dv = pv;
    pv = pv->next;
    free(dv);
  }
}

static void freeParam(XtokParam* op)
{
  freeQualifiers(&op->qualifiers);
}

static void freeParams(XtokParams* op)
{
  XtokParam * pv = op->first , *dv;
  while (pv) {
    freeParam(pv);
    dv = pv;
    pv = pv->next;
    free(dv);
  }
}

static void freeMethod(XtokMethod* op)
{
  freeQualifiers(&op->qualifiers);
  freeParams(&op->params);
}

static void freeMethods(XtokMethods* op)
{
  XtokMethod * mv = op->first , *dv;
  while (mv) {
    freeMethod(mv);
    dv = mv;
    mv = mv->next;
    free(dv);
  }
}

static void freeInstance(XtokInstance *op)
{
  freeProperties(&op->properties);
  freeQualifiers(&op->qualifiers);
}

static  void freeNewValue(XtokNewValue *op)
{
    freeValue(&op->val);
    freeReference(&op->ref);
    freeArray(&op->arr);
}

static void freeMethodCall(XtokMethodCall* op)
{
  if(op->instName) {
    freeInstanceName(&op->instanceName);
  }
  freeParamValues(&op->paramValues);
}

static void freeGetClass(XtokGetClass* op)
{
  freeArray(&op->propertyList);
}

static void freeCreateClass(XtokCreateClass* op)
{
  freeProperties(&op->cls.properties);
  freeQualifiers(&op->cls.qualifiers);
  freeMethods(&op->cls.methods);
}

static void freeGetInstance(XtokGetInstance* op)
{
  freeInstanceName(&op->instanceName);
  freeArray(&op->propertyList);
}

static void freeCreateInstance(XtokCreateInstance* op)
{
  freeInstance(&op->instance);
}

static void freeModifyInstance(XtokModifyInstance* op)
{
  freeInstance(&op->namedInstance.instance);
  freeInstanceName(&op->namedInstance.path);
  freeArray(&op->propertyList);
}

static void freeDeleteInstance(XtokDeleteInstance* op)
{
  freeInstanceName(&op->instanceName);
}

static void freeEnumInstances(XtokEnumInstances* op)
{
  freeArray(&op->propertyList);
}

static void freeAssociatorNames(XtokAssociatorNames* op)
{
  if(op->objNameSet) {
    freeInstanceName(&op->objectName);
  }
}

static void freeReferenceNames(XtokReferenceNames* op)
{
  if(op->objNameSet) {
    freeInstanceName(&op->objectName);
  }
}

static void freeAssociators(XtokAssociators* op)
{
  if(op->objNameSet) {
    freeInstanceName(&op->objectName);
  }
  freeArray(&op->propertyList);
}

static void freeReferences(XtokReferences* op)
{
  if(op->objNameSet) {
    freeInstanceName(&op->objectName);
  }
  freeArray(&op->propertyList);
}

static void freeSetQualifier(XtokSetQualifier* op)
{
  if (op->qualifierdeclaration.data.type & CMPI_ARRAY) {
    freeArray(&op->qualifierdeclaration.data.valueArray);
  }
}

static void freeGetProperty(XtokGetProperty* op)
{
  freeInstanceName(&op->instanceName);
}

static void freeSetProperty(XtokSetProperty* op)
{
  freeInstanceName(&op->instanceName);
  freeNewValue(&op->newVal);
}

void freeCimXmlRequest(RequestHdr hdr)
{
  if (hdr.cimRequest) {
    OperationHdr *op = (OperationHdr *)hdr.cimRequest;
    if (op->nameSpace.data) {
      free(op->nameSpace.data);
    }
    switch(hdr.opType) {
    case OPS_InvokeMethod:
      freeMethodCall((XtokMethodCall *)hdr.cimRequest);
      break;
    case OPS_GetClass:
      freeGetClass((XtokGetClass *)hdr.cimRequest);
      break;
    case OPS_CreateClass:
      freeCreateClass((XtokCreateClass *)hdr.cimRequest);
      break;
    case OPS_GetInstance:
      freeGetInstance((XtokGetInstance *)hdr.cimRequest);
      break;
    case OPS_CreateInstance:
      freeCreateInstance((XtokCreateInstance *)hdr.cimRequest);
      break;
    case OPS_ModifyInstance:
      freeModifyInstance((XtokModifyInstance *)hdr.cimRequest);
      break;
    case OPS_DeleteInstance:
      freeDeleteInstance((XtokDeleteInstance *)hdr.cimRequest);
      break;
    case OPS_EnumerateInstances:
      freeEnumInstances((XtokEnumInstances *)hdr.cimRequest);
      break;
    case OPS_AssociatorNames:
      freeAssociatorNames((XtokAssociatorNames *)hdr.cimRequest);
      break;
    case OPS_ReferenceNames:
      freeReferenceNames((XtokReferenceNames *)hdr.cimRequest);
      break;
    case OPS_Associators:
      freeAssociators((XtokAssociators *)hdr.cimRequest);
      break;
    case OPS_References:
      freeReferences((XtokReferences *)hdr.cimRequest);
      break;
    case OPS_SetQualifier:
      freeSetQualifier((XtokSetQualifier *)hdr.cimRequest);
      break;
    case OPS_GetProperty:
      freeGetProperty((XtokGetProperty *)hdr.cimRequest);
      break;
    case OPS_SetProperty:
      freeSetProperty((XtokSetProperty *)hdr.cimRequest);
      break;
    }
    free (hdr.cimRequest);
  }
}
