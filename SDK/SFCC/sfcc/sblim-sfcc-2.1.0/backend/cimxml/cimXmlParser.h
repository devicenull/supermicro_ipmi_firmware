
/*
 * cimXmlParser.h
 *
 * © Copyright IBM Corp. 2005, 2007
 * © Copyright Intel Corp. 2005
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
 * CIM XML lexer header file for sfcc
 *
*/

#ifndef XMLSCAN_H
#define XMLSCAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cmcidt.h"
#include "cmcift.h"
#include "cmcimacs.h"
#include "native.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum typeValRef {
   typeValRef_InstanceName,
   typeValRef_InstancePath,
   typeValRef_LocalInstancePath
} TypeValRef;

typedef enum typeProperty {
   typeProperty_Value,
   typeProperty_Reference,
   typeProperty_Array
} TypeProperty;

typedef enum typeValue {
   typeValue_Instance,
   typeValue_Class,
   typeValue_charP
} TypeValue;

typedef struct xmlBuffer {
   char *base;
   char *last;
   char *cur;
   char eTagFound;
   int etag;
   char nulledChar;
} XmlBuffer;

typedef struct xmlElement {
   char *attr;
} XmlElement;

typedef struct xmlAttr {
   char *attr;
} XmlAttr;




typedef struct xtokNameSpace {
   char *ns;
   char *cns;                   // must be free'd
} XtokNameSpace;

typedef struct xtokMessage {
   char *id;
} XtokMessage;

struct xtokInstance;

typedef struct xtokValueData {
   union {
    char *value;
    struct xtokInstance *inst;
   };
   TypeValue type;
} XtokValueData;

typedef struct xtokValue {
   XtokValueData data;
   TypeValue type;
} XtokValue;

typedef struct xtokValueArray {
   int max,next;
   char **values;
} XtokValueArray;

typedef struct xtokClassName {
   char *value;
} XtokClassName;

typedef struct xtokLocalNameSpacePath {
   char *value;
} XtokLocalNameSpacePath;

typedef struct xtokHost {
   char *host;
} XtokHost;

typedef struct xtokNameSpacePath {
    XtokHost host;
    XtokLocalNameSpacePath nameSpacePath;
} XtokNameSpacePath;



struct xtokKeyBinding;
struct xtokValueReference;

typedef struct xtokKeyValue {
   char *valueType, *value;
} XtokKeyValue;

typedef struct xtokKeyBindings {
   struct xtokKeyBinding *last, *first;
} XtokKeyBindings;

typedef struct xtokInstanceName {
   char *className;
   XtokKeyBindings bindings;
} XtokInstanceName;

typedef struct xtokInstancePath {
   XtokNameSpacePath path;
   XtokInstanceName instanceName;
   int type;
} XtokInstancePath;

typedef struct xtokLocalInstancePath {
   XtokLocalNameSpacePath path;
   XtokInstanceName instanceName;
   int type;
} XtokLocalInstancePath;

typedef struct xtokLocalClassPath {
   char *path;
   char *className;
   int type;
} XtokLocalClassPath;

typedef struct xtokClassPath {
   XtokNameSpacePath name;
   XtokClassName className;
   int  type;
} XtokClassPath;

typedef struct xtokValueReferenceData {
   union {
      XtokInstancePath instancePath;
      XtokLocalInstancePath localInstancePath;
      XtokInstanceName instanceName;
   };
   TypeValRef type;
} XtokValueReferenceData;

typedef struct xtokValueReference {
   XtokValueReferenceData data;
   TypeValRef type;
   CMPIObjectPath *op;
} XtokValueReference;

typedef struct xtokValueRefArray {
   int max,next;
   XtokValueReference *values;
} XtokValueRefArray;

typedef struct xtokQualifierData {
   union {
      XtokValue value;
      XtokValueArray array;
   };
  char isArray;
} XtokQualifierData;

typedef struct xtokQualifier {
   struct xtokQualifier *next;
   char *name;
   CMPIType type;
   XtokQualifierData data;
   char propagated, overridable, tosubclass, toinstance, translatable;
} XtokQualifier;

typedef struct xtokQualifiers {
   XtokQualifier *last, *first; // must be free'd
} XtokQualifiers;

typedef struct xtokKeyBindingContent {
  union {
     XtokKeyValue keyValue;
     XtokValueReference ref;
  };
  char *type;
} XtokKeyBindingContent;


typedef struct xtokKeyBinding {
  struct xtokKeyBinding *next;
  char *name, *type;
  XtokKeyBindingContent val;
} XtokKeyBinding;


typedef struct xtokPropertyData {
   union {
      XtokValue value;
      XtokValueReference ref;
      XtokValueArray array;
   };
   XtokQualifiers qualifiers;
   int null;
} XtokPropertyData;

typedef struct xtokProperty {
   struct xtokProperty *next;
   char *name;
   char *classOrigin;
   char propagated;
   char *referenceClass;
   CMPIType valueType;
   XtokPropertyData val;
   TypeProperty propType;
} XtokProperty;

typedef struct xtokProperties {
   XtokProperty *last, *first;  // must be free'd
} XtokProperties;

typedef struct xtokReturnValueData {
   union {
      XtokValue value;
      XtokValueReference ref;
   };
   CMPIType type;
} XtokReturnValueData;

typedef struct xtokReturnValue {
   XtokReturnValueData data;
   CMPIType type;
} XtokReturnValue;

typedef struct xtokInstance {
   char *className;
   XtokProperties properties;
   XtokQualifiers qualifiers;
} XtokInstance;

typedef struct xtokInstanceWithPath {
   XtokInstancePath path;
   XtokInstance inst;
} XtokInstanceWithPath;

typedef struct xtokInstanceData {
   XtokProperties properties;
   XtokQualifiers qualifiers;
} XtokInstanceData;

typedef struct xtokNamedInstance {
   XtokInstanceName path;
   XtokInstance instance;
} XtokNamedInstance;

typedef struct xtokObjectPath {
   XtokInstancePath path;
} XtokObjectPath;

typedef struct xtokPropertyList {
   XtokValueArray list;
} XtokPropertyList;


typedef struct xtokParamValueData {
  union {
     XtokValue value;
     XtokValueReference valueRef;
     XtokValueArray valueArray;
     XtokValueRefArray valueRefArray;
  };
  CMPIType type;
} XtokParamValueData;

typedef struct xtokParamValue {
  struct xtokParamValue *next;
  char *name;
  CMPIType type;
  XtokParamValueData data;
} XtokParamValue;

typedef struct xtokParamValues {
   XtokParamValue *last, *first;  // must be free'd
} XtokParamValues;


typedef struct xtokParam {
   struct xtokParam *next;
   XtokQualifiers qualifiers;
   XtokQualifier qualifier;
   int qPart;
   int pType;
   char *name;
   char *refClass;
   char *arraySize;
   CMPIType type;
} XtokParam;

typedef struct xtokParams {
   XtokParam *last, *first;  // must be free'd
} XtokParams;


typedef struct xtokMethod {
   struct xtokMethod *next;
   XtokQualifiers qualifiers;
   XtokParams params;
   char *name;
   char *classOrigin;
   int propagated;
   CMPIType type;
} XtokMethod;

typedef struct xtokMethodData {
   XtokQualifiers qualifiers;
   XtokParams params;
} XtokMethodData;

typedef struct xtokMethods {
   XtokMethod *last, *first;  // must be free'd
} XtokMethods;

typedef struct xtokMethodRespContent {
   XtokParamValues values;  // must be free'd
   XtokReturnValue retValue;
   char* name;
} XtokMethodRespContent;

typedef struct xtokSimpleRespContent {
    XtokMethodRespContent resp;
} XtokSimpleRespContent;

typedef struct xtokClass {
   char *className;
   char *superClass;
   XtokProperties properties;
   XtokQualifiers qualifiers;
   XtokMethods    methods;
} XtokClass;

typedef struct xtokClassWithPath {
   XtokClassPath path;
   XtokClass cls;
} XtokClassWithPath;


typedef struct xtokObjectWithPathData {
   union {
       XtokInstanceWithPath inst;
       XtokClassWithPath cls;
   };
   int type;
} XtokObjectWithPathData;

typedef struct xtokObjectWithPath {
   XtokObjectWithPathData object;
   int type;
} XtokObjectWithPath;

typedef struct xtokErrorResp {
   char *code;
   char *description;
} XtokErrorResp;

typedef struct xtokGetPropRetContent {
  union {
     XtokValue value;
     XtokValueReference ref;
     XtokValueArray arr;
  };
} XtokGetPropRetContent;

#include <setjmp.h>

typedef struct responseHdr {
   XmlBuffer *xmlBuffer;
   int rc;
   int opType;
   int simple;
   char *id;
   char *iMethod;
   int methodCall;
   void *cimRequest;
   unsigned long cimRequestLength;
   int errCode;
   char *description;
   CMPIArray *rvArray;
   CMPIArgs *outArgs;
} ResponseHdr;


typedef struct parser_heap {
  size_t  capacity;
  size_t  numBlocks;
  void  **blocks;
} ParserHeap;
 
typedef struct parser_control {
   XmlBuffer *xmb;
   ResponseHdr respHdr;
   char *da_nameSpace;
   ParserHeap *heap;
#ifdef LARGE_VOL_SUPPORT
   CMCIConnection * econ ; /* enumeration connection */
#endif
} ParserControl;


/* Tokens.  */
#define XTOK_XML 258
#define ZTOK_XML 259
#define XTOK_CIM 260
#define ZTOK_CIM 261
#define XTOK_MESSAGE 262
#define ZTOK_MESSAGE 263
#define XTOK_SIMPLERSP 264
#define ZTOK_SIMPLERSP 265
#define XTOK_SIMPLEEXPREQ 266
#define ZTOK_SIMPLEEXPREQ 267
#define XTOK_EXPORTINDICATION 268
#define ZTOK_EXPMETHODCALL 269
#define XTOK_EP_INSTANCE 270
#define ZTOK_EXPPARAMVALUE 271
#define XTOK_IMETHODRESP 272
#define ZTOK_IMETHODRESP 273
#define XTOK_METHODRESP 274
#define ZTOK_METHODRESP 275
#define XTOK_ERROR 276
#define ZTOK_ERROR 277
#define XTOK_IRETVALUE 278
#define ZTOK_IRETVALUE 279
#define XTOK_RETVALUE 280
#define ZTOK_RETVALUE 281
#define XTOK_NAMESPACEPATH 282
#define ZTOK_NAMESPACEPATH 283
#define XTOK_LOCALNAMESPACEPATH 284
#define ZTOK_LOCALNAMESPACEPATH 285
#define XTOK_NAMESPACE 286
#define ZTOK_NAMESPACE 287
#define ZTOK_IPARAMVALUE 288
#define XTOK_RETURNVALUE 289
#define ZTOK_RETURNVALUE 290
#define XTOK_HOST 291
#define ZTOK_HOST 292
#define XTOK_VALUE 293
#define ZTOK_VALUE 294
#define XTOK_VALUEARRAY 295
#define ZTOK_VALUEARRAY 296
#define XTOK_VALUEREFERENCE 297
#define ZTOK_VALUEREFERENCE 298
#define XTOK_CLASSNAME 299
#define ZTOK_CLASSNAME 300
#define XTOK_INSTANCENAME 301
#define ZTOK_INSTANCENAME 302
#define XTOK_KEYBINDING 303
#define ZTOK_KEYBINDING 304
#define XTOK_KEYVALUE 305
#define ZTOK_KEYVALUE 306
#define XTOK_VALUENAMEDINSTANCE 307
#define ZTOK_VALUENAMEDINSTANCE 308
#define XTOK_QUALIFIER 309
#define ZTOK_QUALIFIER 310
#define XTOK_PROPERTY 311
#define ZTOK_PROPERTY 312
#define XTOK_PROPERTYARRAY 313
#define ZTOK_PROPERTYARRAY 314
#define XTOK_PROPERTYREFERENCE 315
#define ZTOK_PROPERTYREFERENCE 316
#define XTOK_PARAM 317
#define ZTOK_PARAM 318
#define XTOK_PARAMARRAY 319
#define ZTOK_PARAMARRAY 320
#define XTOK_PARAMREF 321
#define ZTOK_PARAMREF 322
#define XTOK_PARAMREFARRAY 323
#define ZTOK_PARAMREFARRAY 324
#define XTOK_METHOD 325
#define ZTOK_METHOD 326
#define XTOK_CLASS 327
#define ZTOK_CLASS 328
#define XTOK_INSTANCE 329
#define ZTOK_INSTANCE 330
#define XTOK_VALUEOBJECTWITHPATH 331
#define ZTOK_VALUEOBJECTWITHPATH 332
#define XTOK_PARAMVALUE 333
#define ZTOK_PARAMVALUE 334
#define XTOK_INSTANCEPATH 335
#define ZTOK_INSTANCEPATH 336
#define XTOK_OBJECTPATH 337
#define ZTOK_OBJECTPATH 338
#define XTOK_LOCALINSTANCEPATH 339
#define ZTOK_LOCALINSTANCEPATH 340
#define XTOK_LOCALCLASSPATH 341
#define ZTOK_LOCALCLASSPATH 342
#define XTOK_CLASSPATH 343
#define ZTOK_CLASSPATH 344
#define XTOK_VALUEREFARRAY 345
#define ZTOK_VALUEREFARRAY 346
#define XTOK_CDATA 347
#define ZTOK_CDATA 348


typedef union parseUnion
{

   XtokMessage                   xtokMessage;
   XtokErrorResp                 xtokErrorResp;

   XtokNameSpace                 xtokNameSpace;
   XtokLocalNameSpacePath        xtokLocalNameSpacePath;
   XtokNameSpacePath             xtokNameSpacePath;
   XtokHost                      xtokHost;
   XtokInstancePath              xtokInstancePath;
   XtokLocalInstancePath         xtokLocalInstancePath;
   XtokClassPath                 xtokClassPath;
   XtokClassName                 xtokClassName;
   
   XtokValue                     xtokValue;
   XtokValueData                 xtokValueData;
   XtokValueArray                xtokValueArray;
   XtokValueReference            xtokValueReference;
   XtokValueRefArray             xtokValueRefArray;
   XtokValueReferenceData        xtokValueReferenceData;
   XtokObjectWithPath            xtokObjectWithPath;
   XtokObjectWithPathData        xtokObjectWithPathData;
   XtokClassWithPath             xtokClassWithPath;
   XtokInstanceWithPath          xtokInstanceWithPath;
   XtokObjectPath                xtokObjectPath;
   XtokInstanceName              xtokInstanceName;
   XtokKeyBinding                xtokKeyBinding;
   XtokKeyBindingContent         xtokKeyBindingContent;
   XtokKeyValue                  xtokKeyValue;

   XtokClass                     xtokClass;
   XtokInstance                  xtokInstance;
   XtokInstanceData              xtokInstanceData;
   XtokNamedInstance             xtokNamedInstance;

   XtokProperty                  xtokProperty;
   XtokPropertyData              xtokPropertyData;
   
   XtokGetPropRetContent         xtokGetPropRetContent;

   XtokMethod                    xtokMethod;
   XtokMethodData                xtokMethodData;
   XtokQualifier                 xtokQualifier;
   XtokQualifierData             xtokQualifierData;

   XtokParamValue                xtokParamValue;
   XtokParamValueData            xtokParamValueData;
   XtokParam                     xtokParam;

   XtokReturnValue               xtokReturnValue;
   XtokReturnValueData           xtokReturnValueData;
   XtokMethodRespContent         xtokMethodRespContent;
   XtokSimpleRespContent         xtokSimpleRespContent;
} parseUnion;


ParserHeap* parser_heap_init();
void parser_heap_term(ParserHeap* ph);
void* parser_malloc(ParserHeap *ph, size_t sz);
void* parser_calloc(ParserHeap *ph, size_t num, size_t sz);
void* parser_realloc(ParserHeap *ph, void *p, size_t sz);
void* parser_strdup(ParserHeap *ph, const char *s);

extern ResponseHdr scanCimXmlResponse(const char *xmlData, CMPIObjectPath *cop);
extern void freeCimXmlResponse(ResponseHdr * hdr);
extern int sfccLex(parseUnion * lvalp, ParserControl * parm);

#ifdef __cplusplus
 }
#endif

#endif
