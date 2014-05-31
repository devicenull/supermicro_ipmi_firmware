/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     XTOK_XML = 258,
     ZTOK_XML = 259,
     XTOK_CIM = 260,
     ZTOK_CIM = 261,
     XTOK_MESSAGE = 262,
     ZTOK_MESSAGE = 263,
     XTOK_SIMPLEREQ = 264,
     ZTOK_SIMPLEREQ = 265,
     XTOK_GETCLASS = 266,
     XTOK_ENUMCLASSNAMES = 267,
     XTOK_ENUMCLASSES = 268,
     XTOK_CREATECLASS = 269,
     XTOK_CREATEINSTANCE = 270,
     XTOK_DELETECLASS = 271,
     XTOK_DELETEINSTANCE = 272,
     XTOK_MODIFYINSTANCE = 273,
     XTOK_GETINSTANCE = 274,
     XTOK_ENUMINSTANCENAMES = 275,
     XTOK_ENUMINSTANCES = 276,
     XTOK_EXECQUERY = 277,
     XTOK_ASSOCIATORS = 278,
     XTOK_REFERENCES = 279,
     XTOK_ASSOCIATORNAMES = 280,
     XTOK_REFERENCENAMES = 281,
     XTOK_SETQUALIFIER = 282,
     XTOK_SETPROPERTY = 283,
     XTOK_ENUMQUALIFIERS = 284,
     XTOK_GETQUALIFIER = 285,
     XTOK_DELETEQUALIFIER = 286,
     XTOK_GETPROPERTY = 287,
     ZTOK_IMETHODCALL = 288,
     XTOK_METHODCALL = 289,
     ZTOK_METHODCALL = 290,
     XTOK_NAMESPACEPATH = 291,
     ZTOK_NAMESPACEPATH = 292,
     XTOK_LOCALNAMESPACEPATH = 293,
     ZTOK_LOCALNAMESPACEPATH = 294,
     XTOK_NAMESPACE = 295,
     ZTOK_NAMESPACE = 296,
     ZTOK_IPARAMVALUE = 297,
     XTOK_HOST = 298,
     ZTOK_HOST = 299,
     XTOK_VALUE = 300,
     ZTOK_VALUE = 301,
     XTOK_CDATA = 302,
     ZTOK_CDATA = 303,
     XTOK_VALUEARRAY = 304,
     ZTOK_VALUEARRAY = 305,
     XTOK_VALUEREFERENCE = 306,
     ZTOK_VALUEREFERENCE = 307,
     XTOK_VALUEREFARRAY = 308,
     ZTOK_VALUEREFARRAY = 309,
     XTOK_CLASSNAME = 310,
     ZTOK_CLASSNAME = 311,
     XTOK_INSTANCENAME = 312,
     ZTOK_INSTANCENAME = 313,
     XTOK_KEYBINDING = 314,
     ZTOK_KEYBINDING = 315,
     XTOK_KEYVALUE = 316,
     ZTOK_KEYVALUE = 317,
     XTOK_IP_LOCALONLY = 318,
     XTOK_IP_INCLUDEQUALIFIERS = 319,
     XTOK_IP_INCLUDECLASSORIGIN = 320,
     XTOK_IP_DEEPINHERITANCE = 321,
     XTOK_IP_CLASSNAME = 322,
     XTOK_IP_INSTANCE = 323,
     XTOK_IP_MODIFIEDINSTANCE = 324,
     XTOK_IP_INSTANCENAME = 325,
     XTOK_IP_OBJECTNAME = 326,
     XTOK_IP_ASSOCCLASS = 327,
     XTOK_IP_RESULTCLASS = 328,
     XTOK_IP_ROLE = 329,
     XTOK_IP_RESULTROLE = 330,
     XTOK_IP_QUERY = 331,
     XTOK_IP_QUERYLANG = 332,
     XTOK_IP_CLASS = 333,
     XTOK_IP_QUALIFIERDECLARATION = 334,
     XTOK_IP_QUALIFIERNAME = 335,
     XTOK_IP_PROPERTYNAME = 336,
     XTOK_IP_NEWVALUE = 337,
     XTOK_IP_PROPERTYLIST = 338,
     XTOK_VALUENAMEDINSTANCE = 339,
     ZTOK_VALUENAMEDINSTANCE = 340,
     XTOK_QUALIFIER = 341,
     ZTOK_QUALIFIER = 342,
     XTOK_QUALIFIERDECLARATION = 343,
     ZTOK_QUALIFIERDECLARATION = 344,
     XTOK_SCOPE = 345,
     ZTOK_SCOPE = 346,
     XTOK_PROPERTY = 347,
     ZTOK_PROPERTY = 348,
     XTOK_PROPERTYARRAY = 349,
     ZTOK_PROPERTYARRAY = 350,
     XTOK_PROPERTYREFERENCE = 351,
     ZTOK_PROPERTYREFERENCE = 352,
     XTOK_PARAM = 353,
     ZTOK_PARAM = 354,
     XTOK_PARAMARRAY = 355,
     ZTOK_PARAMARRAY = 356,
     XTOK_PARAMREF = 357,
     ZTOK_PARAMREF = 358,
     XTOK_PARAMREFARRAY = 359,
     ZTOK_PARAMREFARRAY = 360,
     XTOK_METHOD = 361,
     ZTOK_METHOD = 362,
     XTOK_CLASS = 363,
     ZTOK_CLASS = 364,
     XTOK_INSTANCE = 365,
     ZTOK_INSTANCE = 366,
     XTOK_PARAMVALUE = 367,
     ZTOK_PARAMVALUE = 368,
     XTOK_INSTANCEPATH = 369,
     ZTOK_INSTANCEPATH = 370,
     XTOK_LOCALINSTANCEPATH = 371,
     ZTOK_LOCALINSTANCEPATH = 372,
     XTOK_LOCALCLASSPATH = 373,
     ZTOK_LOCALCLASSPATH = 374
   };
#endif
/* Tokens.  */
#define XTOK_XML 258
#define ZTOK_XML 259
#define XTOK_CIM 260
#define ZTOK_CIM 261
#define XTOK_MESSAGE 262
#define ZTOK_MESSAGE 263
#define XTOK_SIMPLEREQ 264
#define ZTOK_SIMPLEREQ 265
#define XTOK_GETCLASS 266
#define XTOK_ENUMCLASSNAMES 267
#define XTOK_ENUMCLASSES 268
#define XTOK_CREATECLASS 269
#define XTOK_CREATEINSTANCE 270
#define XTOK_DELETECLASS 271
#define XTOK_DELETEINSTANCE 272
#define XTOK_MODIFYINSTANCE 273
#define XTOK_GETINSTANCE 274
#define XTOK_ENUMINSTANCENAMES 275
#define XTOK_ENUMINSTANCES 276
#define XTOK_EXECQUERY 277
#define XTOK_ASSOCIATORS 278
#define XTOK_REFERENCES 279
#define XTOK_ASSOCIATORNAMES 280
#define XTOK_REFERENCENAMES 281
#define XTOK_SETQUALIFIER 282
#define XTOK_SETPROPERTY 283
#define XTOK_ENUMQUALIFIERS 284
#define XTOK_GETQUALIFIER 285
#define XTOK_DELETEQUALIFIER 286
#define XTOK_GETPROPERTY 287
#define ZTOK_IMETHODCALL 288
#define XTOK_METHODCALL 289
#define ZTOK_METHODCALL 290
#define XTOK_NAMESPACEPATH 291
#define ZTOK_NAMESPACEPATH 292
#define XTOK_LOCALNAMESPACEPATH 293
#define ZTOK_LOCALNAMESPACEPATH 294
#define XTOK_NAMESPACE 295
#define ZTOK_NAMESPACE 296
#define ZTOK_IPARAMVALUE 297
#define XTOK_HOST 298
#define ZTOK_HOST 299
#define XTOK_VALUE 300
#define ZTOK_VALUE 301
#define XTOK_CDATA 302
#define ZTOK_CDATA 303
#define XTOK_VALUEARRAY 304
#define ZTOK_VALUEARRAY 305
#define XTOK_VALUEREFERENCE 306
#define ZTOK_VALUEREFERENCE 307
#define XTOK_VALUEREFARRAY 308
#define ZTOK_VALUEREFARRAY 309
#define XTOK_CLASSNAME 310
#define ZTOK_CLASSNAME 311
#define XTOK_INSTANCENAME 312
#define ZTOK_INSTANCENAME 313
#define XTOK_KEYBINDING 314
#define ZTOK_KEYBINDING 315
#define XTOK_KEYVALUE 316
#define ZTOK_KEYVALUE 317
#define XTOK_IP_LOCALONLY 318
#define XTOK_IP_INCLUDEQUALIFIERS 319
#define XTOK_IP_INCLUDECLASSORIGIN 320
#define XTOK_IP_DEEPINHERITANCE 321
#define XTOK_IP_CLASSNAME 322
#define XTOK_IP_INSTANCE 323
#define XTOK_IP_MODIFIEDINSTANCE 324
#define XTOK_IP_INSTANCENAME 325
#define XTOK_IP_OBJECTNAME 326
#define XTOK_IP_ASSOCCLASS 327
#define XTOK_IP_RESULTCLASS 328
#define XTOK_IP_ROLE 329
#define XTOK_IP_RESULTROLE 330
#define XTOK_IP_QUERY 331
#define XTOK_IP_QUERYLANG 332
#define XTOK_IP_CLASS 333
#define XTOK_IP_QUALIFIERDECLARATION 334
#define XTOK_IP_QUALIFIERNAME 335
#define XTOK_IP_PROPERTYNAME 336
#define XTOK_IP_NEWVALUE 337
#define XTOK_IP_PROPERTYLIST 338
#define XTOK_VALUENAMEDINSTANCE 339
#define ZTOK_VALUENAMEDINSTANCE 340
#define XTOK_QUALIFIER 341
#define ZTOK_QUALIFIER 342
#define XTOK_QUALIFIERDECLARATION 343
#define ZTOK_QUALIFIERDECLARATION 344
#define XTOK_SCOPE 345
#define ZTOK_SCOPE 346
#define XTOK_PROPERTY 347
#define ZTOK_PROPERTY 348
#define XTOK_PROPERTYARRAY 349
#define ZTOK_PROPERTYARRAY 350
#define XTOK_PROPERTYREFERENCE 351
#define ZTOK_PROPERTYREFERENCE 352
#define XTOK_PARAM 353
#define ZTOK_PARAM 354
#define XTOK_PARAMARRAY 355
#define ZTOK_PARAMARRAY 356
#define XTOK_PARAMREF 357
#define ZTOK_PARAMREF 358
#define XTOK_PARAMREFARRAY 359
#define ZTOK_PARAMREFARRAY 360
#define XTOK_METHOD 361
#define ZTOK_METHOD 362
#define XTOK_CLASS 363
#define ZTOK_CLASS 364
#define XTOK_INSTANCE 365
#define ZTOK_INSTANCE 366
#define XTOK_PARAMVALUE 367
#define ZTOK_PARAMVALUE 368
#define XTOK_INSTANCEPATH 369
#define ZTOK_INSTANCEPATH 370
#define XTOK_LOCALINSTANCEPATH 371
#define ZTOK_LOCALINSTANCEPATH 372
#define XTOK_LOCALCLASSPATH 373
#define ZTOK_LOCALCLASSPATH 374




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 136 "cimXmlOps.y"
typedef union YYSTYPE {
   int                           intValue;
   char                          boolValue;
   char*                         className;
   void*                         tokCim;

   XtokMessage                   xtokMessage;
   XtokNameSpace                 xtokNameSpace;
   char*                         xtokLocalNameSpacePath;
   XtokNameSpacePath             xtokNameSpacePath;
   XtokHost                      xtokHost;
   XtokInstancePath              xtokInstancePath;
   XtokLocalInstancePath         xtokLocalInstancePath;
   XtokLocalClassPath            xtokLocalClassPath;

   XtokValue                     xtokValue;
   XtokValueArray                xtokValueArray;
   XtokValueReference            xtokValueReference;
   XtokValueRefArray             xtokValueRefArray;

   XtokInstanceName              xtokInstanceName;
   XtokKeyBinding                xtokKeyBinding;
   XtokKeyBindings               xtokKeyBindings;
   XtokKeyValue                  xtokKeyValue;

   XtokClass                     xtokClass;
   XtokInstance                  xtokInstance;
   XtokInstanceData              xtokInstanceData;
   XtokNamedInstance             xtokNamedInstance;

   XtokProperty                  xtokProperty;
   XtokPropertyData              xtokPropertyData;

   XtokMethod                    xtokMethod;
   XtokMethodData                xtokMethodData;
   XtokQualifier                 xtokQualifier;
   XtokQualifierDeclaration      xtokQualifierDeclaration;
   XtokQualifierDeclarationData  xtokQualifierDeclarationData;
   XtokQualifiers                xtokQualifiers;

   XtokParamValues               xtokParamValues;
   XtokParamValue                xtokParamValue;
   XtokParam                     xtokParam;
   XtokMethodCall                xtokMethodCall;

   XtokGetClassParmsList         xtokGetClassParmsList;
   XtokGetClassParms             xtokGetClassParms;
   XtokGetClass                  xtokGetClass;

   XtokEnumClassNames            xtokEnumClassNames;
   XtokEnumClassNamesParmsList   xtokEnumClassNamesParmsList;
   XtokEnumClassNamesParms       xtokEnumClassNamesParms;

   XtokEnumClasses               xtokEnumClasses;
   XtokEnumClassesParmsList      xtokEnumClassesParmsList;
   XtokEnumClassesParms          xtokEnumClassesParms;

   XtokGetInstance               xtokGetInstance;
   XtokGetInstanceParmsList      xtokGetInstanceParmsList;
   XtokGetInstanceParms          xtokGetInstanceParms;

   XtokDeleteClass               xtokDeleteClass;
   XtokDeleteClassParm           xtokDeleteClassParm;

   XtokDeleteInstance            xtokDeleteInstance;
   XtokDeleteInstanceParm        xtokDeleteInstanceParm;

   XtokCreateClass               xtokCreateClass;
   XtokCreateClassParm           xtokCreateClassParm;
 
   XtokCreateInstance            xtokCreateInstance;
   XtokCreateInstanceParm        xtokCreateInstanceParm;

   XtokModifyInstance            xtokModifyInstance;
   XtokModifyInstanceParmsList   xtokModifyInstanceParmsList;
   XtokModifyInstanceParms       xtokModifyInstanceParms;

   XtokEnumInstanceNames         xtokEnumInstanceNames;

   XtokEnumInstances             xtokEnumInstances;
   XtokEnumInstancesParmsList    xtokEnumInstancesParmsList;
   XtokEnumInstancesParms        xtokEnumInstancesParms;

   XtokExecQuery                 xtokExecQuery;
   
   XtokAssociators               xtokAssociators;
   XtokAssociatorsParmsList      xtokAssociatorsParmsList;
   XtokAssociatorsParms          xtokAssociatorsParms;

   XtokReferences                xtokReferences;
   XtokReferencesParmsList       xtokReferencesParmsList;
   XtokReferencesParms           xtokReferencesParms;

   XtokAssociatorNames           xtokAssociatorNames;
   XtokAssociatorNamesParmsList  xtokAssociatorNamesParmsList;
   XtokAssociatorNamesParms      xtokAssociatorNamesParms;

   XtokReferenceNames            xtokReferenceNames;
   XtokReferenceNamesParmsList   xtokReferenceNamesParmsList;
   XtokReferenceNamesParms       xtokReferenceNamesParms;

   XtokSetQualifier              xtokSetQualifier;
   XtokSetQualifierParm          xtokSetQualifierParm;
   
   XtokEnumQualifiers            xtokEnumQualifiers;
   
   XtokGetQualifier              xtokGetQualifier;
   XtokGetQualifierParm          xtokGetQualifierParm;

   XtokDeleteQualifier           xtokDeleteQualifier;
   XtokDeleteQualifierParm       xtokDeleteQualifierParm;
   
   XtokGetProperty               xtokGetProperty;
   XtokGetPropertyParm           xtokGetPropertyParm;
   
   XtokSetProperty               xtokSetProperty;
   XtokSetPropertyParms          xtokSetPropertyParms;
   XtokSetPropertyParmsList      xtokSetPropertyParmsList;
   
   XtokNewValue                  xtokNewValue;
   
   XtokScope                     xtokScope;
   
} YYSTYPE;
/* Line 1403 of yacc.c.  */
#line 401 "cimXmlOps.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif





