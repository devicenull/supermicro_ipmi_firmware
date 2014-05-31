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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse sfcXmlparse
#define yylex   sfcXmllex
#define yyerror sfcXmlerror
#define yylval  sfcXmllval
#define yychar  sfcXmlchar
#define yydebug sfcXmldebug
#define yynerrs sfcXmlnerrs


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




/* Copy the first part of user declarations.  */
#line 1 "cimXmlOps.y"


/*
 * cimXmlOps.y
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
 *
 * CIM XML grammar for sfcb.
 *
*/


/*
**==============================================================================
**
** Includes
**
**==============================================================================
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "cimXmlParser.h"


//
// Define the global parser state object:
//

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm
#define YYERROR_VERBOSE 1

extern int yyerror(char*);
extern int yylex (void *lvalp, ParserControl *parm);


static void setRequest(void *parm, void *req, unsigned long size, int type)
{
   ((ParserControl*)parm)->reqHdr.cimRequestLength=size;
   ((ParserControl*)parm)->reqHdr.cimRequest=malloc(size);
   memcpy(((ParserControl*)parm)->reqHdr.cimRequest,req,size);
   ((ParserControl*)parm)->reqHdr.opType = type;
}

static void addProperty(XtokProperties *ps, XtokProperty *p)
{
   XtokProperty *np;
   np=(XtokProperty*)malloc(sizeof(XtokProperty));
   memcpy(np,p,sizeof(XtokProperty));
   np->next=NULL;
   if (ps->last) {
      ps->last->next=np;
   }
   else ps->first=np;
   ps->last=np;
}

static void addParamValue(XtokParamValues *vs, XtokParamValue *v)
{
   XtokParamValue *nv;
   nv=(XtokParamValue*)malloc(sizeof(XtokParamValue));
   memcpy(nv,v,sizeof(XtokParamValue));
   nv->next=NULL;
   if (vs->last) {
      vs->last->next=nv;
   }
   else vs->first=nv;
   vs->last=nv;
}

static void addQualifier(XtokQualifiers *qs, XtokQualifier *q)
{
   XtokQualifier *nq;
   nq=(XtokQualifier*)malloc(sizeof(XtokQualifier));
   memcpy(nq,q,sizeof(XtokQualifier));
   nq->next=NULL;
   if (qs->last) {
      qs->last->next=nq;
   }
   else qs->first=nq;
   qs->last=nq;
}

static void addMethod(XtokMethods *ms, XtokMethod *m)
{
   XtokMethod *nm;
   nm=(XtokMethod*)malloc(sizeof(XtokMethod));
   memcpy(nm,m,sizeof(XtokMethod));
   nm->next=NULL;
   if (ms->last) {
      ms->last->next=nm;
   }
   else ms->first=nm;
   ms->last=nm;
}

static void addParam(XtokParams *ps, XtokParam *p)
{
   XtokParam *np;
   np=(XtokParam*)malloc(sizeof(XtokParam));
   memcpy(np,p,sizeof(XtokParam));
   np->next=NULL;
   if (ps->last) {
      ps->last->next=np;
   }
   else ps->first=np;
   ps->last=np;
}



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 191 of yacc.c.  */
#line 580 "cimXmlOps.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 592 "cimXmlOps.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   565

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  120
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  95
/* YYNRULES -- Number of rules. */
#define YYNRULES  266
/* YYNRULES -- Number of states. */
#define YYNSTATES  529

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   374

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     7,    11,    15,    19,    23,    27,    31,
      35,    39,    43,    47,    51,    55,    59,    63,    67,    71,
      75,    79,    83,    87,    91,    95,    99,   103,   107,   111,
     113,   116,   118,   121,   123,   126,   129,   133,   137,   141,
     145,   148,   155,   162,   164,   167,   169,   172,   176,   180,
     184,   186,   188,   190,   193,   197,   200,   204,   206,   209,
     213,   215,   218,   220,   223,   227,   231,   234,   238,   241,
     245,   248,   252,   255,   257,   260,   262,   265,   268,   272,
     276,   279,   281,   284,   286,   289,   292,   296,   299,   303,
     306,   310,   313,   317,   320,   324,   326,   329,   331,   334,
     338,   342,   345,   349,   352,   356,   359,   363,   366,   368,
     371,   375,   377,   380,   384,   386,   389,   391,   394,   398,
     402,   405,   409,   412,   414,   417,   421,   423,   426,   430,
     435,   437,   440,   442,   445,   449,   453,   456,   460,   463,
     467,   470,   474,   477,   481,   484,   492,   500,   502,   505,
     507,   510,   514,   518,   521,   525,   528,   532,   535,   539,
     542,   546,   549,   553,   556,   560,   563,   565,   568,   570,
     573,   577,   581,   584,   588,   591,   595,   598,   602,   605,
     609,   612,   614,   617,   619,   622,   626,   630,   633,   637,
     640,   644,   647,   651,   654,   656,   659,   661,   664,   668,
     672,   675,   679,   682,   687,   691,   692,   695,   698,   701,
     705,   706,   709,   714,   715,   718,   722,   723,   726,   729,
     730,   735,   736,   738,   740,   741,   744,   749,   754,   759,
     760,   763,   764,   766,   768,   770,   774,   778,   782,   785,
     789,   794,   797,   802,   807,   812,   816,   822,   828,   831,
     834,   838,   840,   843,   847,   851,   855,   859,   861,   864,
     867,   870,   873,   877,   879,   882,   887
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     121,     0,    -1,     3,     4,   122,    -1,     5,   123,     6,
      -1,     7,   124,     8,    -1,     9,   125,    10,    -1,    11,
     142,    33,    -1,    12,   145,    33,    -1,    13,   148,    33,
      -1,    17,   163,    33,    -1,    19,   151,    33,    -1,    18,
     158,    33,    -1,    15,   156,    33,    -1,    20,   165,    33,
      -1,    21,   166,    33,    -1,    23,   170,    33,    -1,    25,
     176,    33,    -1,    24,   173,    33,    -1,    26,   179,    33,
      -1,    22,   169,    33,    -1,    34,   126,    35,    -1,    16,
     161,    33,    -1,    14,   154,    33,    -1,    29,   139,    33,
      -1,    27,   140,    33,    -1,    30,   135,    33,    -1,    31,
     137,    33,    -1,    32,   129,    33,    -1,    28,   131,    33,
      -1,   203,    -1,   203,   127,    -1,   202,    -1,   202,   127,
      -1,   128,    -1,   127,   128,    -1,   112,   113,    -1,   112,
     204,   113,    -1,   112,   205,   113,    -1,   112,   207,   113,
      -1,   112,   208,   113,    -1,   197,   130,    -1,    81,   204,
      42,    70,   212,    42,    -1,    70,   212,    42,    81,   204,
      42,    -1,   197,    -1,   197,   132,    -1,   133,    -1,   132,
     133,    -1,    70,   212,    42,    -1,    81,   204,    42,    -1,
      82,   134,    42,    -1,   204,    -1,   205,    -1,   207,    -1,
     197,   136,    -1,    80,   204,    42,    -1,   197,   138,    -1,
      80,   204,    42,    -1,   197,    -1,   197,   141,    -1,    79,
     190,    42,    -1,   197,    -1,   197,   143,    -1,   144,    -1,
     143,   144,    -1,    67,   211,    42,    -1,    63,   210,    42,
      -1,    63,    42,    -1,    64,   210,    42,    -1,    64,    42,
      -1,    65,   210,    42,    -1,    65,    42,    -1,    83,   205,
      42,    -1,    83,    42,    -1,   197,    -1,   197,   146,    -1,
     147,    -1,   146,   147,    -1,    67,    42,    -1,    67,   211,
      42,    -1,    66,   210,    42,    -1,    66,    42,    -1,   197,
      -1,   197,   149,    -1,   150,    -1,   149,   150,    -1,    67,
      42,    -1,    67,   211,    42,    -1,    66,    42,    -1,    66,
     210,    42,    -1,    63,    42,    -1,    63,   210,    42,    -1,
      64,    42,    -1,    64,   210,    42,    -1,    65,    42,    -1,
      65,   210,    42,    -1,   197,    -1,   197,   152,    -1,   153,
      -1,   152,   153,    -1,    70,   212,    42,    -1,    63,   210,
      42,    -1,    63,    42,    -1,    64,   210,    42,    -1,    64,
      42,    -1,    65,   210,    42,    -1,    65,    42,    -1,    83,
     205,    42,    -1,    83,    42,    -1,   197,    -1,   197,   155,
      -1,    78,   183,    42,    -1,   197,    -1,   197,   157,    -1,
      68,   188,    42,    -1,   197,    -1,   197,   159,    -1,   160,
      -1,   159,   160,    -1,    69,   182,    42,    -1,    64,   210,
      42,    -1,    64,    42,    -1,    83,   205,    42,    -1,    83,
      42,    -1,   197,    -1,   197,   162,    -1,    67,   211,    42,
      -1,   197,    -1,   197,   164,    -1,    70,   212,    42,    -1,
     197,    67,   211,    42,    -1,   197,    -1,   197,   167,    -1,
     168,    -1,   167,   168,    -1,    67,   211,    42,    -1,    63,
     210,    42,    -1,    63,    42,    -1,    64,   210,    42,    -1,
      64,    42,    -1,    66,   210,    42,    -1,    66,    42,    -1,
      65,   210,    42,    -1,    65,    42,    -1,    83,   205,    42,
      -1,    83,    42,    -1,   197,    76,   204,    42,    77,   204,
      42,    -1,   197,    77,   204,    42,    76,   204,    42,    -1,
     197,    -1,   197,   171,    -1,   172,    -1,   171,   172,    -1,
      71,   212,    42,    -1,    72,   211,    42,    -1,    72,    42,
      -1,    73,   211,    42,    -1,    73,    42,    -1,    74,   204,
      42,    -1,    74,    42,    -1,    75,   204,    42,    -1,    75,
      42,    -1,    64,   210,    42,    -1,    64,    42,    -1,    65,
     210,    42,    -1,    65,    42,    -1,    83,   205,    42,    -1,
      83,    42,    -1,   197,    -1,   197,   174,    -1,   175,    -1,
     174,   175,    -1,    71,   212,    42,    -1,    73,   211,    42,
      -1,    73,    42,    -1,    74,   204,    42,    -1,    74,    42,
      -1,    64,   210,    42,    -1,    64,    42,    -1,    65,   210,
      42,    -1,    65,    42,    -1,    83,   205,    42,    -1,    83,
      42,    -1,   197,    -1,   197,   177,    -1,   178,    -1,   177,
     178,    -1,    71,   212,    42,    -1,    72,   211,    42,    -1,
      72,    42,    -1,    73,   211,    42,    -1,    73,    42,    -1,
      74,   204,    42,    -1,    74,    42,    -1,    75,   204,    42,
      -1,    75,    42,    -1,   197,    -1,   197,   180,    -1,   181,
      -1,   180,   181,    -1,    71,   212,    42,    -1,    73,   211,
      42,    -1,    73,    42,    -1,    74,   204,    42,    -1,    74,
      42,    -1,    84,   212,   188,    85,    -1,   108,   184,   109,
      -1,    -1,   184,   196,    -1,   184,   193,    -1,   184,   185,
      -1,   106,   186,   107,    -1,    -1,   186,   196,    -1,   186,
      98,   187,    99,    -1,    -1,   187,   196,    -1,   110,   189,
     111,    -1,    -1,   189,   196,    -1,   189,   193,    -1,    -1,
      88,   192,   191,    89,    -1,    -1,   204,    -1,   205,    -1,
      -1,    90,    91,    -1,    92,   194,   195,    93,    -1,    96,
     194,   195,    97,    -1,    94,   194,   195,    95,    -1,    -1,
     194,   196,    -1,    -1,   204,    -1,   207,    -1,   205,    -1,
      86,   204,    87,    -1,    86,   205,    87,    -1,    38,   198,
      39,    -1,    40,    41,    -1,   198,    40,    41,    -1,    36,
     200,   197,    37,    -1,    43,    44,    -1,   114,   199,   212,
     115,    -1,   116,   197,   212,   117,    -1,   118,   197,   211,
     119,    -1,    45,   188,    46,    -1,    45,    47,   188,    48,
      46,    -1,    45,    47,   183,    48,    46,    -1,    45,    46,
      -1,    49,    50,    -1,    49,   206,    50,    -1,   204,    -1,
     206,   204,    -1,    51,   201,    52,    -1,    51,   202,    52,
      -1,    51,   212,    52,    -1,    53,   209,    54,    -1,   207,
      -1,   209,   207,    -1,    45,    46,    -1,    55,    56,    -1,
      57,    58,    -1,    57,   213,    58,    -1,   214,    -1,   213,
     214,    -1,    59,    61,    62,    60,    -1,    59,   207,    60,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   540,   540,   546,   552,   558,   564,   567,   570,   573,
     576,   579,   582,   585,   588,   591,   594,   597,   600,   603,
     606,   609,   612,   615,   618,   621,   624,   627,   630,   641,
     653,   664,   677,   692,   698,   705,   709,   719,   731,   736,
     747,   760,   765,   777,   787,   802,   808,   823,   828,   833,
     842,   855,   860,   871,   883,   893,   905,   915,   930,   944,
     955,   968,   984,   995,  1009,  1017,  1024,  1028,  1035,  1039,
    1046,  1050,  1057,  1069,  1079,  1092,  1097,  1105,  1110,  1115,
    1121,  1133,  1143,  1156,  1162,  1171,  1175,  1180,  1184,  1190,
    1194,  1200,  1204,  1210,  1214,  1227,  1240,  1257,  1268,  1282,
    1290,  1297,  1301,  1308,  1312,  1319,  1323,  1330,  1343,  1353,
    1367,  1380,  1389,  1403,  1415,  1427,  1443,  1454,  1469,  1477,
    1484,  1488,  1495,  1507,  1516,  1530,  1542,  1551,  1565,  1578,
    1596,  1608,  1623,  1633,  1646,  1653,  1661,  1665,  1673,  1677,
    1685,  1689,  1697,  1701,  1708,  1722,  1734,  1755,  1772,  1793,
    1810,  1830,  1839,  1847,  1851,  1859,  1863,  1871,  1875,  1883,
    1887,  1896,  1900,  1909,  1913,  1920,  1935,  1950,  1969,  1984,
    2002,  2011,  2019,  2023,  2031,  2035,  2044,  2048,  2057,  2061,
    2068,  2081,  2095,  2112,  2123,  2137,  2143,  2149,  2153,  2159,
    2163,  2169,  2173,  2179,  2193,  2205,  2221,  2230,  2242,  2248,
    2254,  2258,  2264,  2276,  2289,  2304,  2305,  2310,  2314,  2321,
    2336,  2337,  2344,  2358,  2359,  2374,  2388,  2394,  2398,  2409,
    2410,  2419,  2422,  2427,  2438,  2439,  2449,  2461,  2466,  2482,
    2485,  2494,  2499,  2503,  2507,  2520,  2524,  2538,  2545,  2549,
    2562,  2570,  2576,  2595,  2604,  2617,  2623,  2629,  2633,  2641,
    2646,  2656,  2663,  2671,  2676,  2681,  2689,  2696,  2703,  2711,
    2719,  2731,  2737,  2745,  2755,  2766,  2772
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "XTOK_XML", "ZTOK_XML", "XTOK_CIM",
  "ZTOK_CIM", "XTOK_MESSAGE", "ZTOK_MESSAGE", "XTOK_SIMPLEREQ",
  "ZTOK_SIMPLEREQ", "XTOK_GETCLASS", "XTOK_ENUMCLASSNAMES",
  "XTOK_ENUMCLASSES", "XTOK_CREATECLASS", "XTOK_CREATEINSTANCE",
  "XTOK_DELETECLASS", "XTOK_DELETEINSTANCE", "XTOK_MODIFYINSTANCE",
  "XTOK_GETINSTANCE", "XTOK_ENUMINSTANCENAMES", "XTOK_ENUMINSTANCES",
  "XTOK_EXECQUERY", "XTOK_ASSOCIATORS", "XTOK_REFERENCES",
  "XTOK_ASSOCIATORNAMES", "XTOK_REFERENCENAMES", "XTOK_SETQUALIFIER",
  "XTOK_SETPROPERTY", "XTOK_ENUMQUALIFIERS", "XTOK_GETQUALIFIER",
  "XTOK_DELETEQUALIFIER", "XTOK_GETPROPERTY", "ZTOK_IMETHODCALL",
  "XTOK_METHODCALL", "ZTOK_METHODCALL", "XTOK_NAMESPACEPATH",
  "ZTOK_NAMESPACEPATH", "XTOK_LOCALNAMESPACEPATH",
  "ZTOK_LOCALNAMESPACEPATH", "XTOK_NAMESPACE", "ZTOK_NAMESPACE",
  "ZTOK_IPARAMVALUE", "XTOK_HOST", "ZTOK_HOST", "XTOK_VALUE", "ZTOK_VALUE",
  "XTOK_CDATA", "ZTOK_CDATA", "XTOK_VALUEARRAY", "ZTOK_VALUEARRAY",
  "XTOK_VALUEREFERENCE", "ZTOK_VALUEREFERENCE", "XTOK_VALUEREFARRAY",
  "ZTOK_VALUEREFARRAY", "XTOK_CLASSNAME", "ZTOK_CLASSNAME",
  "XTOK_INSTANCENAME", "ZTOK_INSTANCENAME", "XTOK_KEYBINDING",
  "ZTOK_KEYBINDING", "XTOK_KEYVALUE", "ZTOK_KEYVALUE", "XTOK_IP_LOCALONLY",
  "XTOK_IP_INCLUDEQUALIFIERS", "XTOK_IP_INCLUDECLASSORIGIN",
  "XTOK_IP_DEEPINHERITANCE", "XTOK_IP_CLASSNAME", "XTOK_IP_INSTANCE",
  "XTOK_IP_MODIFIEDINSTANCE", "XTOK_IP_INSTANCENAME", "XTOK_IP_OBJECTNAME",
  "XTOK_IP_ASSOCCLASS", "XTOK_IP_RESULTCLASS", "XTOK_IP_ROLE",
  "XTOK_IP_RESULTROLE", "XTOK_IP_QUERY", "XTOK_IP_QUERYLANG",
  "XTOK_IP_CLASS", "XTOK_IP_QUALIFIERDECLARATION", "XTOK_IP_QUALIFIERNAME",
  "XTOK_IP_PROPERTYNAME", "XTOK_IP_NEWVALUE", "XTOK_IP_PROPERTYLIST",
  "XTOK_VALUENAMEDINSTANCE", "ZTOK_VALUENAMEDINSTANCE", "XTOK_QUALIFIER",
  "ZTOK_QUALIFIER", "XTOK_QUALIFIERDECLARATION",
  "ZTOK_QUALIFIERDECLARATION", "XTOK_SCOPE", "ZTOK_SCOPE", "XTOK_PROPERTY",
  "ZTOK_PROPERTY", "XTOK_PROPERTYARRAY", "ZTOK_PROPERTYARRAY",
  "XTOK_PROPERTYREFERENCE", "ZTOK_PROPERTYREFERENCE", "XTOK_PARAM",
  "ZTOK_PARAM", "XTOK_PARAMARRAY", "ZTOK_PARAMARRAY", "XTOK_PARAMREF",
  "ZTOK_PARAMREF", "XTOK_PARAMREFARRAY", "ZTOK_PARAMREFARRAY",
  "XTOK_METHOD", "ZTOK_METHOD", "XTOK_CLASS", "ZTOK_CLASS",
  "XTOK_INSTANCE", "ZTOK_INSTANCE", "XTOK_PARAMVALUE", "ZTOK_PARAMVALUE",
  "XTOK_INSTANCEPATH", "ZTOK_INSTANCEPATH", "XTOK_LOCALINSTANCEPATH",
  "ZTOK_LOCALINSTANCEPATH", "XTOK_LOCALCLASSPATH", "ZTOK_LOCALCLASSPATH",
  "$accept", "start", "cimOperation", "message", "simpleReq",
  "iMethodCall", "methodCall", "paramValues", "paramValue", "getProperty",
  "getPropertyParm", "setProperty", "setPropertyParmsList",
  "setPropertyParms", "newValue", "getQualifier", "getQualifierParm",
  "deleteQualifier", "deleteQualifierParm", "enumQualifiers",
  "setQualifier", "setQualifierParm", "getClass", "getClassParmsList",
  "getClassParms", "enumClassNames", "enumClassNamesParmsList",
  "enumClassNamesParms", "enumClasses", "enumClassesParmsList",
  "enumClassesParms", "getInstance", "getInstanceParmsList",
  "getInstanceParms", "createClass", "createClassParm", "createInstance",
  "createInstanceParm", "modifyInstance", "modifyInstanceParmsList",
  "modifyInstanceParms", "deleteClass", "deleteClassParm",
  "deleteInstance", "deleteInstanceParm", "enumInstanceNames",
  "enumInstances", "enumInstancesParmsList", "enumInstancesParms",
  "execQuery", "associators", "associatorsParmsList", "associatorsParms",
  "references", "referencesParmsList", "referencesParms",
  "associatorNames", "associatorNamesParmsList", "associatorNamesParms",
  "referenceNames", "referenceNamesParmsList", "referenceNamesParms",
  "namedInstance", "class", "classData", "method", "methodData",
  "parameter", "instance", "instanceData", "qualifierDeclaration",
  "qualifierDeclarationData", "scope", "property", "qualifierList",
  "propertyData", "qualifier", "localNameSpacePath", "namespaces",
  "nameSpacePath", "host", "instancePath", "localInstancePath",
  "localClassPath", "value", "valueArray", "valueList", "valueReference",
  "valueRefArray", "valueRefList", "boolValue", "className",
  "instanceName", "keyBindings", "keyBinding", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   120,   121,   122,   123,   124,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   126,
     126,   126,   126,   127,   127,   128,   128,   128,   128,   128,
     129,   130,   130,   131,   131,   132,   132,   133,   133,   133,
     134,   134,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   142,   143,   143,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   145,   145,   146,   146,   147,   147,   147,
     147,   148,   148,   149,   149,   150,   150,   150,   150,   150,
     150,   150,   150,   150,   150,   151,   151,   152,   152,   153,
     153,   153,   153,   153,   153,   153,   153,   153,   154,   154,
     155,   156,   156,   157,   158,   158,   159,   159,   160,   160,
     160,   160,   160,   161,   161,   162,   163,   163,   164,   165,
     166,   166,   167,   167,   168,   168,   168,   168,   168,   168,
     168,   168,   168,   168,   168,   169,   169,   170,   170,   171,
     171,   172,   172,   172,   172,   172,   172,   172,   172,   172,
     172,   172,   172,   172,   172,   172,   173,   173,   174,   174,
     175,   175,   175,   175,   175,   175,   175,   175,   175,   175,
     175,   176,   176,   177,   177,   178,   178,   178,   178,   178,
     178,   178,   178,   178,   179,   179,   180,   180,   181,   181,
     181,   181,   181,   182,   183,   184,   184,   184,   184,   185,
     186,   186,   186,   187,   187,   188,   189,   189,   189,   190,
     190,   191,   191,   191,   192,   192,   193,   193,   193,   194,
     194,   195,   195,   195,   195,   196,   196,   197,   198,   198,
     199,   200,   201,   202,   203,   204,   204,   204,   204,   205,
     205,   206,   206,   207,   207,   207,   208,   209,   209,   210,
     211,   212,   212,   213,   213,   214,   214
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
       2,     1,     2,     1,     2,     2,     3,     3,     3,     3,
       2,     6,     6,     1,     2,     1,     2,     3,     3,     3,
       1,     1,     1,     2,     3,     2,     3,     1,     2,     3,
       1,     2,     1,     2,     3,     3,     2,     3,     2,     3,
       2,     3,     2,     1,     2,     1,     2,     2,     3,     3,
       2,     1,     2,     1,     2,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     1,     2,     1,     2,     3,
       3,     2,     3,     2,     3,     2,     3,     2,     1,     2,
       3,     1,     2,     3,     1,     2,     1,     2,     3,     3,
       2,     3,     2,     1,     2,     3,     1,     2,     3,     4,
       1,     2,     1,     2,     3,     3,     2,     3,     2,     3,
       2,     3,     2,     3,     2,     7,     7,     1,     2,     1,
       2,     3,     3,     2,     3,     2,     3,     2,     3,     2,
       3,     2,     3,     2,     3,     2,     1,     2,     1,     2,
       3,     3,     2,     3,     2,     3,     2,     3,     2,     3,
       2,     1,     2,     1,     2,     3,     3,     2,     3,     2,
       3,     2,     3,     2,     1,     2,     1,     2,     3,     3,
       2,     3,     2,     4,     3,     0,     2,     2,     2,     3,
       0,     2,     4,     0,     2,     3,     0,     2,     2,     0,
       4,     0,     1,     1,     0,     2,     4,     4,     4,     0,
       2,     0,     1,     1,     1,     3,     3,     3,     2,     3,
       4,     2,     4,     4,     4,     3,     5,     5,     2,     2,
       3,     1,     2,     3,     3,     3,     3,     1,     2,     2,
       2,     2,     3,     1,     2,     4,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,     0,     0,     0,     1,     0,     2,     0,     0,     0,
       0,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     4,     0,     0,    60,
       0,    73,     0,    81,     0,   108,     0,   111,     0,   123,
       0,   126,     0,   114,     0,    95,     0,     0,     0,   130,
       0,     0,     0,   147,     0,   166,     0,   181,     0,   194,
       0,     0,     0,    43,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    31,    29,     5,     0,     0,
       6,     0,     0,     0,     0,     0,    61,    62,     7,     0,
       0,    74,    75,     8,     0,     0,     0,     0,     0,    82,
      83,    22,     0,   109,    12,     0,   112,    21,     0,   124,
       9,     0,   127,    11,     0,     0,     0,   115,   116,    10,
       0,     0,     0,     0,     0,    96,    97,    13,     0,    14,
       0,     0,     0,     0,     0,     0,   131,   132,    19,     0,
       0,    15,     0,     0,     0,     0,     0,     0,     0,     0,
     148,   149,    17,     0,     0,     0,     0,     0,     0,   167,
     168,    16,     0,     0,     0,     0,     0,   182,   183,    18,
       0,     0,     0,   195,   196,    24,   219,    58,    28,     0,
       0,     0,    44,    45,    23,    25,     0,    53,    26,     0,
      55,    27,     0,     0,    40,     0,     0,    20,     0,    32,
      33,    30,   238,   237,     0,    66,     0,     0,    68,     0,
      70,     0,     0,     0,    72,     0,     0,    63,    80,     0,
      77,     0,    76,    89,     0,    91,     0,    93,     0,    87,
       0,    85,     0,    84,   205,     0,   216,     0,     0,     0,
       0,   120,     0,     0,     0,   122,     0,   117,   101,     0,
     103,     0,   105,     0,     0,   107,     0,    98,     0,   136,
       0,   138,     0,   142,     0,   140,     0,     0,   144,     0,
     133,     0,     0,     0,   161,     0,   163,     0,     0,   153,
       0,   155,     0,   157,     0,   159,     0,   165,     0,   150,
     176,     0,   178,     0,     0,   172,     0,   174,     0,   180,
       0,   169,     0,   187,     0,   189,     0,   191,     0,   193,
       0,   184,     0,   200,     0,   202,     0,   197,   224,     0,
       0,     0,     0,     0,    50,    51,    52,    46,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,     0,     0,
      34,   239,   259,    65,    67,    69,   260,    64,   249,   251,
       0,    71,    79,    78,    90,    92,    94,    88,    86,     0,
     110,     0,   113,   125,   261,     0,     0,   263,   128,   119,
       0,   118,   121,   100,   102,   104,    99,   106,   129,   135,
     137,   141,   139,   134,   143,   248,     0,     0,     0,     0,
     160,   162,   151,   152,   154,   156,   158,   164,   175,   177,
     170,   171,   173,   179,   185,   186,   188,   190,   192,   198,
     199,   201,     0,   221,    59,    47,    48,     0,     0,     0,
       0,    49,    54,    56,     0,     0,   243,   244,   257,     0,
      36,    37,    38,    39,   250,   252,     0,   229,   229,   229,
     210,   204,   208,   207,   206,   215,   218,   217,     0,     0,
     262,   264,     0,     0,     0,   245,     0,     0,   225,     0,
     222,   223,     0,     0,   253,   254,   255,     0,     0,   256,
     258,     0,     0,   231,   231,   231,     0,     0,   266,   203,
       0,     0,     0,     0,   220,     0,     0,     0,     0,     0,
     235,   236,     0,   230,   232,   234,   233,     0,     0,   213,
     209,   211,   265,   247,   246,   145,   146,   241,     0,   242,
      42,    41,   226,   228,   227,     0,   240,   212,   214
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,     2,     6,     8,    10,    35,    84,   209,   210,    80,
     204,    72,   192,   193,   333,    76,   197,    78,   200,    74,
      70,   187,    38,    96,    97,    40,   101,   102,    42,   109,
     110,    54,   135,   136,    44,   113,    46,   116,    52,   127,
     128,    48,   119,    50,   122,    56,    58,   146,   147,    60,
      62,   160,   161,    64,   169,   170,    66,   177,   178,    68,
     183,   184,   254,   245,   369,   452,   486,   525,   247,   371,
     329,   469,   423,   453,   483,   502,   503,    39,    89,   473,
     496,   428,    85,    86,   504,   505,   360,   506,   349,   439,
     217,   223,   250,   376,   377
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -347
static const short int yypact[] =
{
      53,    34,    62,    82,  -347,    65,  -347,   101,   132,   215,
     139,  -347,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   147,   147,   147,   147,   147,   147,
     147,   147,   147,   147,   106,   160,  -347,   154,   182,    89,
     190,   257,   241,   187,   282,   170,   287,   137,   289,   270,
     305,   269,   308,   -34,   309,    94,   310,   277,   312,    14,
     313,   251,   314,   118,   315,    41,   317,   184,   318,   189,
     319,   274,   321,   130,   322,  -347,   323,   278,   324,   279,
     327,    21,   147,   147,   326,   252,   252,  -347,   328,   286,
    -347,    91,   162,   172,   307,    86,    89,  -347,  -347,   222,
      -5,   257,  -347,  -347,   224,   226,   230,   231,    48,   187,
    -347,  -347,   259,  -347,  -347,   258,  -347,  -347,   307,  -347,
    -347,   316,  -347,  -347,   236,   281,   124,   -34,  -347,  -347,
     237,   238,   242,   316,   125,    94,  -347,  -347,   307,  -347,
     243,   244,   248,   249,   307,   127,    14,  -347,  -347,   325,
     325,  -347,   250,   254,   316,    67,    88,   255,   256,   153,
     118,  -347,  -347,   260,   261,   316,    90,   262,   161,    41,
    -347,  -347,   316,    95,   123,   266,   267,   184,  -347,  -347,
     316,   126,   268,   189,  -347,  -347,   288,  -347,  -347,   316,
     325,    -3,   130,  -347,  -347,  -347,   325,  -347,  -347,   325,
    -347,  -347,   316,   325,  -347,   316,   307,  -347,   -17,   252,
    -347,   252,  -347,  -347,   333,  -347,   329,   335,  -347,   336,
    -347,   337,   330,   338,  -347,   168,   339,  -347,  -347,   340,
    -347,   343,  -347,  -347,   345,  -347,   346,  -347,   347,  -347,
     348,  -347,   349,  -347,  -347,   350,  -347,   351,   353,   271,
     354,  -347,   355,   316,   356,  -347,   357,  -347,  -347,   358,
    -347,   359,  -347,   360,   361,  -347,   362,  -347,   363,  -347,
     364,  -347,   365,  -347,   366,  -347,   367,   368,  -347,   369,
    -347,    -6,   370,   371,  -347,   372,  -347,   373,   374,  -347,
     375,  -347,   376,  -347,   377,  -347,   378,  -347,   379,  -347,
    -347,   380,  -347,   381,   382,  -347,   383,  -347,   384,  -347,
     385,  -347,   386,  -347,   387,  -347,   388,  -347,   389,  -347,
     390,  -347,   391,  -347,   392,  -347,   393,  -347,   352,   394,
     395,   396,   -28,   397,  -347,  -347,  -347,  -347,   398,   399,
     401,   402,   331,   332,   403,  -347,   334,   342,   344,   400,
    -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,
     171,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,    35,
    -347,   -35,  -347,  -347,  -347,    -7,   273,  -347,  -347,  -347,
     258,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,
    -347,  -347,  -347,  -347,  -347,  -347,   208,   404,   405,   408,
    -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,
    -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,
    -347,  -347,   406,   216,  -347,  -347,  -347,   409,   407,   410,
     411,  -347,  -347,  -347,   412,   415,  -347,  -347,  -347,   263,
    -347,  -347,  -347,  -347,  -347,  -347,   216,  -347,  -347,  -347,
    -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,   413,   414,
    -347,  -347,   416,   417,   418,  -347,   325,   325,  -347,   419,
    -347,  -347,   421,   316,  -347,  -347,  -347,   325,   316,  -347,
    -347,   420,   422,   -18,   -18,   -18,   -43,   426,  -347,  -347,
     423,   424,   425,   429,  -347,   428,   147,   341,   431,   434,
    -347,  -347,   427,  -347,  -347,  -347,  -347,   430,   432,  -347,
    -347,  -347,  -347,  -347,  -347,  -347,  -347,  -347,   440,  -347,
    -347,  -347,  -347,  -347,  -347,    85,  -347,  -347,  -347
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -347,  -347,  -347,  -347,  -347,  -347,  -347,   433,   110,  -347,
    -347,  -347,  -347,   276,  -347,  -347,  -347,  -347,  -347,  -347,
    -347,  -347,  -347,  -347,   435,  -347,  -347,   436,  -347,  -347,
     437,  -347,  -347,   311,  -347,  -347,  -347,  -347,  -347,  -347,
     438,  -347,  -347,  -347,  -347,  -347,  -347,  -347,   303,  -347,
    -347,  -347,   292,  -347,  -347,   284,  -347,  -347,   283,  -347,
    -347,   275,  -347,    83,  -347,  -347,  -347,  -347,  -257,  -347,
    -347,  -347,  -347,   107,  -115,  -149,  -346,   -13,  -347,  -347,
    -347,  -347,   129,  -347,   -83,   -74,  -347,  -169,  -347,  -347,
      56,   -55,  -107,  -347,   104
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned short int yytable[] =
{
      41,    43,    45,    47,    49,    51,    53,    55,    57,    59,
      61,    63,    65,    67,    69,    71,    73,    75,    77,    79,
      81,   226,   336,   454,   397,   457,   264,   281,   281,   249,
     124,   225,   225,   332,   332,   125,   344,   230,     3,   348,
     395,   396,   281,   446,   332,   231,   225,   288,   332,   126,
     222,   446,   256,   242,   458,   509,     1,   447,   304,   448,
     266,   449,     4,   248,   510,   312,   282,   283,   446,   205,
     206,   279,     7,   322,   294,   296,   455,   140,   141,   142,
     143,   144,   330,   268,   308,   298,   427,     5,    82,   277,
     241,   202,   318,   320,   310,   340,   345,   145,   342,   326,
     290,   292,   203,   222,   246,   163,   164,   331,   334,   289,
       9,   306,   165,   338,   166,   167,   339,   335,   314,   316,
     341,   446,   222,   462,   168,   346,   324,   447,   224,   448,
     291,   449,   305,   215,   347,   225,   216,   313,    11,   464,
     511,   450,   359,   222,   451,   222,   380,    36,   219,   221,
     222,   343,    91,    92,    93,   229,    94,   130,   131,   132,
     234,   236,   238,   240,   133,   315,   255,   265,   323,   278,
      87,   446,    95,   225,   225,   438,   225,   134,   222,   528,
     252,   222,   152,   153,   527,    37,   259,   261,   263,   154,
     155,   156,   157,   158,    88,   297,   270,   272,   274,   276,
     189,   159,   225,   309,   218,   115,   459,   216,   285,   287,
     225,   190,   191,   281,   220,    90,   281,   216,   358,   301,
     303,   444,    82,    98,    83,   430,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,   112,    34,
     104,   105,   106,   107,   108,   172,   173,   174,   175,   176,
     180,   281,   181,   182,   228,   225,   233,   216,   235,   216,
     480,   216,   237,   239,   103,   216,   216,   445,   251,   258,
     260,   216,   216,   216,   262,   269,   271,   216,   216,   216,
     273,   275,   284,   216,   216,   216,   286,   293,   295,   216,
     281,   281,   300,   302,   307,   216,   216,   281,   317,   319,
     325,   281,   281,   281,   332,   111,   244,   479,   246,   350,
     114,   350,   117,    99,   100,   213,   214,   149,   150,   374,
     375,   460,   375,   484,   485,   507,   508,   118,   120,   121,
     470,   123,   129,   137,   138,   139,   148,   151,   162,   471,
     171,   179,   185,   186,   188,   194,   195,   198,   196,   199,
     201,   207,   222,   481,   208,   253,   497,   244,   246,   212,
     281,   499,   482,   249,   351,   352,   328,   353,   354,   355,
     357,   361,   362,   492,   493,   363,   356,   364,   365,   366,
     367,   368,   370,   372,   498,   373,   378,   379,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   398,   399,   400,   401,   402,   403,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   424,   425,   426,   431,
     432,   433,   422,   434,   435,   472,   267,   440,   436,   280,
     465,   437,   299,   311,   332,   441,   519,   442,   327,   474,
     321,   429,   475,   476,   495,   490,   491,   515,   337,   513,
     514,   516,   517,   520,   488,   487,   521,   526,   456,   463,
     461,     0,   466,   518,   467,   478,   512,     0,     0,     0,
       0,     0,     0,   477,     0,     0,     0,   468,     0,     0,
       0,   489,     0,     0,     0,     0,     0,   500,   494,   501,
       0,     0,     0,   443,     0,     0,     0,     0,     0,   211,
     522,     0,     0,     0,     0,   523,     0,     0,     0,   524,
       0,   227,     0,     0,     0,     0,     0,   232,     0,     0,
       0,     0,     0,     0,     0,     0,   243,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   257
};

static const short int yycheck[] =
{
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    95,   191,   369,   281,   371,   133,    45,    45,    57,
      64,    49,    49,    51,    51,    69,    53,    42,     4,   208,
      46,    47,    45,    86,    51,   100,    49,   154,    51,    83,
      55,    86,   126,   108,    61,    98,     3,    92,   165,    94,
     134,    96,     0,   118,   107,   172,   149,   150,    86,    82,
      83,   145,     7,   180,   157,   158,   111,    63,    64,    65,
      66,    67,   189,   138,   167,   159,   114,     5,   116,   144,
      42,    70,   175,   176,   168,   202,   113,    83,   205,   182,
     155,   156,    81,    55,   110,    64,    65,   190,   191,    42,
       9,   166,    71,   196,    73,    74,   199,   191,   173,   174,
     203,    86,    55,   380,    83,   208,   181,    92,    42,    94,
      42,    96,    42,    42,   208,    49,    45,    42,     6,   396,
     486,   106,   225,    55,   109,    55,   253,     8,    92,    93,
      55,   206,    63,    64,    65,    99,    67,    63,    64,    65,
     104,   105,   106,   107,    70,    42,    42,    42,    42,    42,
      10,    86,    83,    49,    49,   344,    49,    83,    55,   525,
     124,    55,    64,    65,    99,    38,   130,   131,   132,    71,
      72,    73,    74,    75,    40,    42,   140,   141,   142,   143,
      70,    83,    49,    42,    42,    68,   375,    45,   152,   153,
      49,    81,    82,    45,    42,    33,    45,    45,    50,   163,
     164,    50,   116,    33,   118,   332,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    78,    34,
      63,    64,    65,    66,    67,    71,    72,    73,    74,    75,
      71,    45,    73,    74,    42,    49,    42,    45,    42,    45,
     439,    45,    42,    42,    33,    45,    45,   360,    42,    42,
      42,    45,    45,    45,    42,    42,    42,    45,    45,    45,
      42,    42,    42,    45,    45,    45,    42,    42,    42,    45,
      45,    45,    42,    42,    42,    45,    45,    45,    42,    42,
      42,    45,    45,    45,    51,    33,   108,    54,   110,   209,
      33,   211,    33,    66,    67,    39,    40,    76,    77,    58,
      59,    58,    59,   448,   449,   484,   485,    67,    33,    70,
     423,    33,    33,    33,    67,    33,    33,    33,    33,   423,
      33,    33,    33,    79,    33,    33,    33,    33,    80,    80,
      33,    35,    55,   446,   112,    84,   473,   108,   110,    41,
      45,   478,   446,    57,    41,    46,    88,    42,    42,    42,
      42,    42,    42,   466,   467,    42,    56,    42,    42,    42,
      42,    42,    42,    42,   477,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    90,    42,    42,    36,   135,   113,   117,   146,
      46,   119,   160,   169,    51,   113,   115,   113,   183,    52,
     177,   332,    52,    52,    43,    48,    48,    42,   192,    46,
      46,    42,    44,    42,    60,    62,    42,    37,   371,   396,
     376,    -1,    77,   496,    76,    70,    60,    -1,    -1,    -1,
      -1,    -1,    -1,    81,    -1,    -1,    -1,    91,    -1,    -1,
      -1,    85,    -1,    -1,    -1,    -1,    -1,    87,    89,    87,
      -1,    -1,    -1,   113,    -1,    -1,    -1,    -1,    -1,    86,
      93,    -1,    -1,    -1,    -1,    95,    -1,    -1,    -1,    97,
      -1,    96,    -1,    -1,    -1,    -1,    -1,   101,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   109,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   127
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     3,   121,     4,     0,     5,   122,     7,   123,     9,
     124,     6,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    34,   125,     8,    38,   142,   197,
     145,   197,   148,   197,   154,   197,   156,   197,   161,   197,
     163,   197,   158,   197,   151,   197,   165,   197,   166,   197,
     169,   197,   170,   197,   173,   197,   176,   197,   179,   197,
     140,   197,   131,   197,   139,   197,   135,   197,   137,   197,
     129,   197,   116,   118,   126,   202,   203,    10,    40,   198,
      33,    63,    64,    65,    67,    83,   143,   144,    33,    66,
      67,   146,   147,    33,    63,    64,    65,    66,    67,   149,
     150,    33,    78,   155,    33,    68,   157,    33,    67,   162,
      33,    70,   164,    33,    64,    69,    83,   159,   160,    33,
      63,    64,    65,    70,    83,   152,   153,    33,    67,    33,
      63,    64,    65,    66,    67,    83,   167,   168,    33,    76,
      77,    33,    64,    65,    71,    72,    73,    74,    75,    83,
     171,   172,    33,    64,    65,    71,    73,    74,    83,   174,
     175,    33,    71,    72,    73,    74,    75,   177,   178,    33,
      71,    73,    74,   180,   181,    33,    79,   141,    33,    70,
      81,    82,   132,   133,    33,    33,    80,   136,    33,    80,
     138,    33,    70,    81,   130,   197,   197,    35,   112,   127,
     128,   127,    41,    39,    40,    42,    45,   210,    42,   210,
      42,   210,    55,   211,    42,    49,   205,   144,    42,   210,
      42,   211,   147,    42,   210,    42,   210,    42,   210,    42,
     210,    42,   211,   150,   108,   183,   110,   188,   211,    57,
     212,    42,   210,    84,   182,    42,   205,   160,    42,   210,
      42,   210,    42,   210,   212,    42,   205,   153,   211,    42,
     210,    42,   210,    42,   210,    42,   210,   211,    42,   205,
     168,    45,   204,   204,    42,   210,    42,   210,   212,    42,
     211,    42,   211,    42,   204,    42,   204,    42,   205,   172,
      42,   210,    42,   210,   212,    42,   211,    42,   204,    42,
     205,   175,   212,    42,   211,    42,   211,    42,   204,    42,
     204,   178,   212,    42,   211,    42,   204,   181,    88,   190,
     212,   204,    51,   134,   204,   205,   207,   133,   204,   204,
     212,   204,   212,   211,    53,   113,   204,   205,   207,   208,
     128,    41,    46,    42,    42,    42,    56,    42,    50,   204,
     206,    42,    42,    42,    42,    42,    42,    42,    42,   184,
      42,   189,    42,    42,    58,    59,   213,   214,    42,    42,
     212,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    46,    47,   188,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    42,
      42,    42,    90,   192,    42,    42,    42,   114,   201,   202,
     212,    42,    42,    42,    42,    42,   117,   119,   207,   209,
     113,   113,   113,   113,    50,   204,    86,    92,    94,    96,
     106,   109,   185,   193,   196,   111,   193,   196,    61,   207,
      58,   214,   188,   183,   188,    46,    77,    76,    91,   191,
     204,   205,    36,   199,    52,    52,    52,    81,    70,    54,
     207,   204,   205,   194,   194,   194,   186,    62,    60,    85,
      48,    48,   204,   204,    89,    43,   200,   212,   204,   212,
      87,    87,   195,   196,   204,   205,   207,   195,   195,    98,
     107,   196,    60,    46,    46,    42,    42,    44,   197,   115,
      42,    42,    93,    95,    97,   187,    37,    99,   196
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 541 "cimXmlOps.y"
    {
    }
    break;

  case 3:
#line 547 "cimXmlOps.y"
    {
    }
    break;

  case 4:
#line 553 "cimXmlOps.y"
    {
    }
    break;

  case 5:
#line 559 "cimXmlOps.y"
    {
    }
    break;

  case 6:
#line 565 "cimXmlOps.y"
    {
    }
    break;

  case 7:
#line 568 "cimXmlOps.y"
    {
    }
    break;

  case 8:
#line 571 "cimXmlOps.y"
    {
    }
    break;

  case 9:
#line 574 "cimXmlOps.y"
    {
    }
    break;

  case 10:
#line 577 "cimXmlOps.y"
    {
    }
    break;

  case 11:
#line 580 "cimXmlOps.y"
    {
    }
    break;

  case 12:
#line 583 "cimXmlOps.y"
    {
    }
    break;

  case 13:
#line 586 "cimXmlOps.y"
    {
    }
    break;

  case 14:
#line 589 "cimXmlOps.y"
    {
    }
    break;

  case 15:
#line 592 "cimXmlOps.y"
    {
    }
    break;

  case 16:
#line 595 "cimXmlOps.y"
    {
    }
    break;

  case 17:
#line 598 "cimXmlOps.y"
    {
    }
    break;

  case 18:
#line 601 "cimXmlOps.y"
    {
    }
    break;

  case 19:
#line 604 "cimXmlOps.y"
    {
    }
    break;

  case 20:
#line 607 "cimXmlOps.y"
    {
    }
    break;

  case 21:
#line 610 "cimXmlOps.y"
    {
    }
    break;

  case 22:
#line 613 "cimXmlOps.y"
    {
    }
    break;

  case 23:
#line 616 "cimXmlOps.y"
    {
    }
    break;

  case 24:
#line 619 "cimXmlOps.y"
    {
    }
    break;

  case 25:
#line 622 "cimXmlOps.y"
    {
    }
    break;

  case 26:
#line 625 "cimXmlOps.y"
    {
    }
    break;

  case 27:
#line 628 "cimXmlOps.y"
    {
    }
    break;

  case 28:
#line 631 "cimXmlOps.y"
    {
    }
    break;

  case 29:
#line 642 "cimXmlOps.y"
    {
       (yyval.xtokMethodCall).op.count = 2;
       (yyval.xtokMethodCall).op.type = OPS_InvokeMethod;
       (yyval.xtokMethodCall).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalClassPath).path);
       (yyval.xtokMethodCall).op.className=setCharsMsgSegment((yyvsp[0].xtokLocalClassPath).className);
       (yyval.xtokMethodCall).instName=0;
       (yyval.xtokMethodCall).paramValues.first=NULL;
       (yyval.xtokMethodCall).paramValues.last=NULL;
       
       setRequest(parm,&(yyval.xtokMethodCall),sizeof(XtokMethodCall),OPS_InvokeMethod);
    }
    break;

  case 30:
#line 654 "cimXmlOps.y"
    {
       (yyval.xtokMethodCall).op.count = 2;
       (yyval.xtokMethodCall).op.type = OPS_InvokeMethod;
       (yyval.xtokMethodCall).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalClassPath).path);
       (yyval.xtokMethodCall).op.className=setCharsMsgSegment((yyvsp[-1].xtokLocalClassPath).className);
       (yyval.xtokMethodCall).instName=0;
       (yyval.xtokMethodCall).paramValues=(yyvsp[0].xtokParamValues);
       
       setRequest(parm,&(yyval.xtokMethodCall),sizeof(XtokMethodCall),OPS_InvokeMethod);
    }
    break;

  case 31:
#line 665 "cimXmlOps.y"
    {
       (yyval.xtokMethodCall).op.count = 2;
       (yyval.xtokMethodCall).op.type = OPS_InvokeMethod;
       (yyval.xtokMethodCall).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalInstancePath).path);
       (yyval.xtokMethodCall).op.className=setCharsMsgSegment((yyvsp[0].xtokLocalInstancePath).instanceName.className);
       (yyval.xtokMethodCall).instanceName=(yyvsp[0].xtokLocalInstancePath).instanceName;
       (yyval.xtokMethodCall).instName=1;
       (yyval.xtokMethodCall).paramValues.first=NULL;
       (yyval.xtokMethodCall).paramValues.last=NULL;
       
       setRequest(parm,&(yyval.xtokMethodCall),sizeof(XtokMethodCall),OPS_InvokeMethod);
    }
    break;

  case 32:
#line 678 "cimXmlOps.y"
    {
       (yyval.xtokMethodCall).op.count = 2;
       (yyval.xtokMethodCall).op.type = OPS_InvokeMethod;
       (yyval.xtokMethodCall).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalInstancePath).path);
       (yyval.xtokMethodCall).op.className=setCharsMsgSegment((yyvsp[-1].xtokLocalInstancePath).instanceName.className);
       (yyval.xtokMethodCall).instanceName=(yyvsp[-1].xtokLocalInstancePath).instanceName;
       (yyval.xtokMethodCall).instName=1;
       (yyval.xtokMethodCall).paramValues=(yyvsp[0].xtokParamValues);
              
       setRequest(parm,&(yyval.xtokMethodCall),sizeof(XtokMethodCall),OPS_InvokeMethod);
    }
    break;

  case 33:
#line 693 "cimXmlOps.y"
    {
      (yyval.xtokParamValues).first = NULL;
      (yyval.xtokParamValues).last = NULL;
      addParamValue(&(yyval.xtokParamValues),&(yyvsp[0].xtokParamValue));
    }
    break;

  case 34:
#line 699 "cimXmlOps.y"
    {
      addParamValue(&(yyval.xtokParamValues),&(yyvsp[0].xtokParamValue));
    }
    break;

  case 35:
#line 706 "cimXmlOps.y"
    {
       (yyvsp[-1].xtokParamValue).value.value=NULL;
    }
    break;

  case 36:
#line 710 "cimXmlOps.y"
    {
       (yyval.xtokParamValue).value=(yyvsp[-1].xtokValue);
       if((yyval.xtokParamValue).value.type == typeValue_Instance) {
          (yyval.xtokParamValue).type = CMPI_instance;
       } else 
       if((yyval.xtokParamValue).value.type == typeValue_Class) {
          (yyval.xtokParamValue).type = CMPI_class;
       }
    }
    break;

  case 37:
#line 720 "cimXmlOps.y"
    {
       (yyval.xtokParamValue).valueArray=(yyvsp[-1].xtokValueArray);
       (yyval.xtokParamValue).type|=CMPI_ARRAY;
       
       if((yyval.xtokParamValue).valueArray.values) {
          if((yyval.xtokParamValue).valueArray.values[0].type == typeValue_Instance)
          	(yyval.xtokParamValue).type = CMPI_instance | CMPI_ARRAY;
          else if((yyval.xtokParamValue).valueArray.values[0].type == typeValue_Class)
          	(yyval.xtokParamValue).type = CMPI_class | CMPI_ARRAY;          	
       }
    }
    break;

  case 38:
#line 732 "cimXmlOps.y"
    {
       (yyval.xtokParamValue).valueRef=(yyvsp[-1].xtokValueReference);
       (yyval.xtokParamValue).type=CMPI_ref;
    }
    break;

  case 39:
#line 737 "cimXmlOps.y"
    {
       (yyval.xtokParamValue).valueRefArray=(yyvsp[-1].xtokValueRefArray);
       (yyval.xtokParamValue).type=CMPI_ARRAY | CMPI_ref;
    }
    break;

  case 40:
#line 748 "cimXmlOps.y"
    {
       (yyval.xtokGetProperty).op.count = 3;
       (yyval.xtokGetProperty).op.type = OPS_GetProperty;
       (yyval.xtokGetProperty).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokGetProperty).op.className=setCharsMsgSegment((yyvsp[0].xtokGetPropertyParm).instanceName.className);
       (yyval.xtokGetProperty).name = (yyvsp[0].xtokGetPropertyParm).name;
       (yyval.xtokGetProperty).instanceName = (yyvsp[0].xtokGetPropertyParm).instanceName;
       setRequest(parm,&(yyval.xtokGetProperty),sizeof(XtokGetProperty),OPS_GetProperty);
	}
    break;

  case 41:
#line 761 "cimXmlOps.y"
    {
		(yyval.xtokGetPropertyParm).name = (yyvsp[-4].xtokValue).value;
		(yyval.xtokGetPropertyParm).instanceName = (yyvsp[-1].xtokInstanceName);
	}
    break;

  case 42:
#line 766 "cimXmlOps.y"
    {
		(yyval.xtokGetPropertyParm).name = (yyvsp[-1].xtokValue).value;
		(yyval.xtokGetPropertyParm).instanceName = (yyvsp[-4].xtokInstanceName);
	}
    break;

  case 43:
#line 778 "cimXmlOps.y"
    {
       (yyval.xtokSetProperty).op.count = 3;
       (yyval.xtokSetProperty).op.type = OPS_SetProperty;
       (yyval.xtokSetProperty).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokSetProperty).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokSetProperty).propertyName = NULL;

       setRequest(parm,&(yyval.xtokSetProperty),sizeof(XtokSetProperty),OPS_SetProperty);
	}
    break;

  case 44:
#line 788 "cimXmlOps.y"
    {
       (yyval.xtokSetProperty).op.count = 3;
       (yyval.xtokSetProperty).op.type = OPS_SetProperty;
       (yyval.xtokSetProperty).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokSetProperty).op.className=setCharsMsgSegment((yyvsp[0].xtokSetPropertyParmsList).instanceName.className);
       (yyval.xtokSetProperty).newVal = (yyvsp[0].xtokSetPropertyParmsList).newVal;
       (yyval.xtokSetProperty).instanceName = (yyvsp[0].xtokSetPropertyParmsList).instanceName;
       (yyval.xtokSetProperty).propertyName = (yyvsp[0].xtokSetPropertyParmsList).propertyName;
       
       setRequest(parm,&(yyval.xtokSetProperty),sizeof(XtokSetProperty),OPS_SetProperty);
	}
    break;

  case 45:
#line 803 "cimXmlOps.y"
    {
       (yyval.xtokSetPropertyParmsList).newVal = (yyvsp[0].xtokSetPropertyParms).newVal;
       (yyval.xtokSetPropertyParmsList).instanceName = (yyvsp[0].xtokSetPropertyParms).instanceName;
       (yyval.xtokSetPropertyParmsList).propertyName = (yyvsp[0].xtokSetPropertyParms).propertyName;
	}
    break;

  case 46:
#line 809 "cimXmlOps.y"
    {
		if((yyvsp[0].xtokSetPropertyParms).propertyName) {
			(yyval.xtokSetPropertyParmsList).propertyName = (yyvsp[0].xtokSetPropertyParms).propertyName;
		}
		else if((yyvsp[0].xtokSetPropertyParms).instanceName.className) {
			(yyval.xtokSetPropertyParmsList).instanceName = (yyvsp[0].xtokSetPropertyParms).instanceName;
		}
		else {
			(yyval.xtokSetPropertyParmsList).newVal = (yyvsp[0].xtokSetPropertyParms).newVal;
		}
	}
    break;

  case 47:
#line 824 "cimXmlOps.y"
    {
		(yyval.xtokSetPropertyParms).instanceName = (yyvsp[-1].xtokInstanceName);
		(yyval.xtokSetPropertyParms).propertyName = NULL;
	}
    break;

  case 48:
#line 829 "cimXmlOps.y"
    {
		(yyval.xtokSetPropertyParms).propertyName = (yyvsp[-1].xtokValue).value;
		(yyval.xtokSetPropertyParms).instanceName.className = NULL;
	}
    break;

  case 49:
#line 834 "cimXmlOps.y"
    {
		(yyval.xtokSetPropertyParms).newVal = (yyvsp[-1].xtokNewValue);
		(yyval.xtokSetPropertyParms).propertyName = NULL;
		(yyval.xtokSetPropertyParms).instanceName.className = NULL;
	}
    break;

  case 50:
#line 843 "cimXmlOps.y"
    {
		if((yyvsp[0].xtokValue).type == typeValue_Instance) {
			(yyval.xtokNewValue).type = CMPI_instance;
		}
		else if((yyvsp[0].xtokValue).type == typeValue_Class) {
			(yyval.xtokNewValue).type = CMPI_class;
		}
		else {
			(yyval.xtokNewValue).type = 0;
		}
		(yyval.xtokNewValue).val = (yyvsp[0].xtokValue);
	}
    break;

  case 51:
#line 856 "cimXmlOps.y"
    {
		(yyval.xtokNewValue).arr = (yyvsp[0].xtokValueArray);
		(yyval.xtokNewValue).type = CMPI_ARRAY;
	}
    break;

  case 52:
#line 861 "cimXmlOps.y"
    {
		(yyval.xtokNewValue).ref = (yyvsp[0].xtokValueReference);
		(yyval.xtokNewValue).type = CMPI_ref;
	}
    break;

  case 53:
#line 872 "cimXmlOps.y"
    {
       (yyval.xtokGetQualifier).op.count = 2;
       (yyval.xtokGetQualifier).op.type = OPS_GetQualifier;
       (yyval.xtokGetQualifier).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokGetQualifier).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokGetQualifier).name = (yyvsp[0].xtokGetQualifierParm).name;
       setRequest(parm,&(yyval.xtokGetQualifier),sizeof(XtokGetQualifier),OPS_GetQualifier);
    }
    break;

  case 54:
#line 884 "cimXmlOps.y"
    {
       (yyval.xtokGetQualifierParm).name = (yyvsp[-1].xtokValue).value;
    }
    break;

  case 55:
#line 894 "cimXmlOps.y"
    {
       (yyval.xtokDeleteQualifier).op.count = 2;
       (yyval.xtokDeleteQualifier).op.type = OPS_DeleteQualifier;
       (yyval.xtokDeleteQualifier).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokDeleteQualifier).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokDeleteQualifier).name = (yyvsp[0].xtokDeleteQualifierParm).name;
       setRequest(parm,&(yyval.xtokDeleteQualifier),sizeof(XtokDeleteQualifier),OPS_DeleteQualifier);
    }
    break;

  case 56:
#line 906 "cimXmlOps.y"
    {
       (yyval.xtokDeleteQualifierParm).name = (yyvsp[-1].xtokValue).value;
    }
    break;

  case 57:
#line 916 "cimXmlOps.y"
    {
       (yyval.xtokEnumQualifiers).op.count = 2;
       (yyval.xtokEnumQualifiers).op.type = OPS_EnumerateQualifiers;
       (yyval.xtokEnumQualifiers).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokEnumQualifiers).op.className=setCharsMsgSegment(NULL);
       setRequest(parm,&(yyval.xtokEnumQualifiers),sizeof(XtokEnumQualifiers),OPS_EnumerateQualifiers);
    }
    break;

  case 58:
#line 931 "cimXmlOps.y"
    {
       (yyval.xtokSetQualifier).op.count = 3;
       (yyval.xtokSetQualifier).op.type = OPS_SetQualifier;
       (yyval.xtokSetQualifier).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokSetQualifier).op.className=setCharsMsgSegment(NULL);       
       (yyval.xtokSetQualifier).qualifierdeclaration = (yyvsp[0].xtokSetQualifierParm).qualifierdeclaration;

       setRequest(parm,&(yyval.xtokSetQualifier),sizeof(XtokSetQualifier),OPS_SetQualifier);
    }
    break;

  case 59:
#line 945 "cimXmlOps.y"
    {
       (yyval.xtokSetQualifierParm).qualifierdeclaration = (yyvsp[-1].xtokQualifierDeclaration);
    }
    break;

  case 60:
#line 956 "cimXmlOps.y"
    {
       (yyval.xtokGetClass).op.count = 2;
       (yyval.xtokGetClass).op.type = OPS_GetClass;
       (yyval.xtokGetClass).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokGetClass).op.className=setCharsMsgSegment(NULL);
//     $$.flags = FL_localOnly;
       (yyval.xtokGetClass).flags = FL_localOnly|FL_includeQualifiers;
       (yyval.xtokGetClass).propertyList.values = NULL;
       (yyval.xtokGetClass).properties=0;

       setRequest(parm,&(yyval.xtokGetClass),sizeof(XtokGetClass),OPS_GetClass);
    }
    break;

  case 61:
#line 969 "cimXmlOps.y"
    {
       (yyval.xtokGetClass).op.count = 2;
       (yyval.xtokGetClass).op.type = OPS_GetClass;
       (yyval.xtokGetClass).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokGetClass).op.className=setCharsMsgSegment((yyvsp[0].xtokGetClassParmsList).className);
//     $$.flags = ($2.flags &  $2.flagsSet) | ((~$2.flagsSet) & (FL_localOnly));
       (yyval.xtokGetClass).flags = ((yyvsp[0].xtokGetClassParmsList).flags &  (yyvsp[0].xtokGetClassParmsList).flagsSet) | ((~(yyvsp[0].xtokGetClassParmsList).flagsSet) & (FL_localOnly|FL_includeQualifiers));
       (yyval.xtokGetClass).propertyList = (yyvsp[0].xtokGetClassParmsList).propertyList;
       (yyval.xtokGetClass).properties=(yyvsp[0].xtokGetClassParmsList).properties;

       setRequest(parm,&(yyval.xtokGetClass),sizeof(XtokGetClass),OPS_GetClass);
    }
    break;

  case 62:
#line 985 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParmsList).flags=(yyvsp[0].xtokGetClassParms).flags;
       (yyval.xtokGetClassParmsList).flagsSet=(yyvsp[0].xtokGetClassParms).flagsSet;
       if ((yyvsp[0].xtokGetClassParms).clsNameSet) (yyval.xtokGetClassParmsList).className=(yyvsp[0].xtokGetClassParms).className;
       (yyval.xtokGetClassParmsList).clsNameSet = (yyvsp[0].xtokGetClassParms).clsNameSet;
       if ((yyvsp[0].xtokGetClassParms).propertyList.values) {
          (yyval.xtokGetClassParmsList).propertyList=(yyvsp[0].xtokGetClassParms).propertyList;
          (yyval.xtokGetClassParmsList).properties=(yyvsp[0].xtokGetClassParms).properties;
       }
    }
    break;

  case 63:
#line 996 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParmsList).flags=(yyvsp[-1].xtokGetClassParmsList).flags|(yyvsp[0].xtokGetClassParms).flags;
       (yyval.xtokGetClassParmsList).flagsSet=(yyvsp[-1].xtokGetClassParmsList).flagsSet|(yyvsp[0].xtokGetClassParms).flagsSet;
       if ((yyvsp[0].xtokGetClassParms).clsNameSet) (yyval.xtokGetClassParmsList).className=(yyvsp[0].xtokGetClassParms).className;
       (yyval.xtokGetClassParmsList).clsNameSet |= (yyvsp[0].xtokGetClassParms).clsNameSet;
       if ((yyvsp[0].xtokGetClassParms).propertyList.values) {
          (yyval.xtokGetClassParmsList).propertyList=(yyvsp[0].xtokGetClassParms).propertyList;
          (yyval.xtokGetClassParmsList).properties=(yyvsp[0].xtokGetClassParms).properties;
       }
    }
    break;

  case 64:
#line 1010 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParms).className = (yyvsp[-1].className);
       (yyval.xtokGetClassParms).flags = (yyval.xtokGetClassParms).flagsSet = 0 ;
       (yyval.xtokGetClassParms).clsNameSet = 1;
       (yyval.xtokGetClassParms).propertyList.values=0;
       (yyval.xtokGetClassParms).properties=0;
    }
    break;

  case 65:
#line 1018 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParms).flags = (yyvsp[-1].boolValue) ? FL_localOnly : 0 ;
       (yyval.xtokGetClassParms).flagsSet = FL_localOnly;
       (yyval.xtokGetClassParms).properties=(yyval.xtokGetClassParms).clsNameSet=0;
       (yyval.xtokGetClassParms).propertyList.values=0;
    }
    break;

  case 66:
#line 1025 "cimXmlOps.y"
    {
       memset(&(yyval.xtokGetClassParms), 0, sizeof((yyval.xtokGetClassParms)));
    }
    break;

  case 67:
#line 1029 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokGetClassParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokGetClassParms).properties=(yyval.xtokGetClassParms).clsNameSet=0;
       (yyval.xtokGetClassParms).propertyList.values=0;
    }
    break;

  case 68:
#line 1036 "cimXmlOps.y"
    {
       memset(&(yyval.xtokGetClassParms), 0, sizeof((yyval.xtokGetClassParms)));
    }
    break;

  case 69:
#line 1040 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParms).flags = (yyvsp[-1].boolValue) ? FL_includeClassOrigin : 0 ;
       (yyval.xtokGetClassParms).flagsSet = FL_includeClassOrigin;
       (yyval.xtokGetClassParms).properties=(yyval.xtokGetClassParms).clsNameSet=0;
       (yyval.xtokGetClassParms).propertyList.values=0;
    }
    break;

  case 70:
#line 1047 "cimXmlOps.y"
    {
       memset(&(yyval.xtokGetClassParms), 0, sizeof((yyval.xtokGetClassParms)));
    }
    break;

  case 71:
#line 1051 "cimXmlOps.y"
    {
       (yyval.xtokGetClassParms).propertyList=(yyvsp[-1].xtokValueArray);
       (yyval.xtokGetClassParms).properties=(yyvsp[-1].xtokValueArray).next;
       (yyval.xtokGetClassParms).clsNameSet=0;
       (yyval.xtokGetClassParms).flags = (yyval.xtokGetClassParms).flagsSet = 0 ;
    }
    break;

  case 72:
#line 1058 "cimXmlOps.y"
    {
       memset(&(yyval.xtokGetClassParms), 0, sizeof((yyval.xtokGetClassParms)));
    }
    break;

  case 73:
#line 1070 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassNames).op.count = 2;
       (yyval.xtokEnumClassNames).op.type = OPS_EnumerateClassNames;
       (yyval.xtokEnumClassNames).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokEnumClassNames).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokEnumClassNames).flags = 0;

       setRequest(parm,&(yyval.xtokEnumClassNames),sizeof(XtokEnumClassNames),OPS_EnumerateClassNames);
    }
    break;

  case 74:
#line 1080 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassNames).op.count = 2;
       (yyval.xtokEnumClassNames).op.type = OPS_EnumerateClassNames;
       (yyval.xtokEnumClassNames).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokEnumClassNames).op.className=setCharsMsgSegment((yyvsp[0].xtokEnumClassNamesParmsList).className);
       (yyval.xtokEnumClassNames).flags = (yyvsp[0].xtokEnumClassNamesParmsList).flags;

       setRequest(parm,&(yyval.xtokEnumClassNames),sizeof(XtokEnumClassNames),OPS_EnumerateClassNames);
    }
    break;

  case 75:
#line 1093 "cimXmlOps.y"
    {
       if ((yyvsp[0].xtokEnumClassNamesParms).className) (yyval.xtokEnumClassNamesParmsList).className=(yyvsp[0].xtokEnumClassNamesParms).className;
       (yyval.xtokEnumClassNamesParmsList).flags=(yyvsp[0].xtokEnumClassNamesParms).flags;
    }
    break;

  case 76:
#line 1098 "cimXmlOps.y"
    {
       if ((yyvsp[0].xtokEnumClassNamesParms).className) (yyval.xtokEnumClassNamesParmsList).className=(yyvsp[0].xtokEnumClassNamesParms).className;
       (yyval.xtokEnumClassNamesParmsList).flags = ((yyvsp[0].xtokEnumClassNamesParms).flags & (yyvsp[0].xtokEnumClassNamesParms).flagsSet) | ((~(yyvsp[0].xtokEnumClassNamesParms).flagsSet) & FL_deepInheritance);
    }
    break;

  case 77:
#line 1106 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassNamesParms).className = NULL;
       (yyval.xtokEnumClassNamesParms).flags = (yyval.xtokEnumClassNamesParms).flagsSet = 0 ;
    }
    break;

  case 78:
#line 1111 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassNamesParms).className = (yyvsp[-1].className);
       (yyval.xtokEnumClassNamesParms).flags = (yyval.xtokEnumClassNamesParms).flagsSet = 0 ;
    }
    break;

  case 79:
#line 1116 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassNamesParms).flags = (yyvsp[-1].boolValue) ? FL_deepInheritance : 0 ;
       (yyval.xtokEnumClassNamesParms).flagsSet = FL_deepInheritance;
       (yyval.xtokEnumClassNamesParms).className=0;
    }
    break;

  case 80:
#line 1122 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassNamesParms).className = NULL;
       (yyval.xtokEnumClassNamesParms).flags = (yyval.xtokEnumClassNamesParms).flagsSet = 0 ;
    }
    break;

  case 81:
#line 1134 "cimXmlOps.y"
    {
       (yyval.xtokEnumClasses).op.count = 2;
       (yyval.xtokEnumClasses).op.type = OPS_EnumerateClasses;
       (yyval.xtokEnumClasses).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokEnumClasses).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokEnumClasses).flags = FL_localOnly;

       setRequest(parm,&(yyval.xtokEnumClasses),sizeof(XtokEnumClasses),OPS_EnumerateClasses);
    }
    break;

  case 82:
#line 1144 "cimXmlOps.y"
    {
       (yyval.xtokEnumClasses).op.count = 2;
       (yyval.xtokEnumClasses).op.type = OPS_EnumerateClasses;
       (yyval.xtokEnumClasses).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokEnumClasses).op.className=setCharsMsgSegment((yyvsp[0].xtokEnumClassesParmsList).className);
       (yyval.xtokEnumClasses).flags = ((yyvsp[0].xtokEnumClassesParmsList).flags & (yyvsp[0].xtokEnumClassesParmsList).flagsSet) | ((~(yyvsp[0].xtokEnumClassesParmsList).flagsSet) & FL_localOnly);

       setRequest(parm,&(yyval.xtokEnumClasses),sizeof(XtokEnumClasses),OPS_EnumerateClasses);
    }
    break;

  case 83:
#line 1157 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParmsList).flags=(yyvsp[0].xtokEnumClassesParms).flags;
       (yyval.xtokEnumClassesParmsList).flagsSet=(yyvsp[0].xtokEnumClassesParms).flagsSet;
       if ((yyvsp[0].xtokEnumClassesParms).className) (yyval.xtokEnumClassesParmsList).className=(yyvsp[0].xtokEnumClassesParms).className;
    }
    break;

  case 84:
#line 1163 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParmsList).flags=(yyvsp[-1].xtokEnumClassesParmsList).flags|(yyvsp[0].xtokEnumClassesParms).flags;
       (yyval.xtokEnumClassesParmsList).flagsSet=(yyvsp[-1].xtokEnumClassesParmsList).flagsSet|(yyvsp[0].xtokEnumClassesParms).flagsSet;
       if ((yyvsp[0].xtokEnumClassesParms).className) (yyval.xtokEnumClassesParmsList).className=(yyvsp[0].xtokEnumClassesParms).className;
    }
    break;

  case 85:
#line 1172 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumClassesParms), 0, sizeof((yyval.xtokEnumClassesParms)));
    }
    break;

  case 86:
#line 1176 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParms).className = (yyvsp[-1].className);
       (yyval.xtokEnumClassesParms).flags = (yyval.xtokEnumClassesParms).flagsSet = 0 ;
    }
    break;

  case 87:
#line 1181 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumClassesParms), 0, sizeof((yyval.xtokEnumClassesParms)));
    }
    break;

  case 88:
#line 1185 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParms).flags = (yyvsp[-1].boolValue) ? FL_deepInheritance : 0 ;
       (yyval.xtokEnumClassesParms).flagsSet = FL_deepInheritance;
       (yyval.xtokEnumClassesParms).className=0;
    }
    break;

  case 89:
#line 1191 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumClassesParms), 0, sizeof((yyval.xtokEnumClassesParms)));
    }
    break;

  case 90:
#line 1195 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParms).flags = (yyvsp[-1].boolValue) ? FL_localOnly : 0 ;
       (yyval.xtokEnumClassesParms).flagsSet = FL_localOnly;
       (yyval.xtokEnumClassesParms).className=0;
    }
    break;

  case 91:
#line 1201 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumClassesParms), 0, sizeof((yyval.xtokEnumClassesParms)));
    }
    break;

  case 92:
#line 1205 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokEnumClassesParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokEnumClassesParms).className=0;
    }
    break;

  case 93:
#line 1211 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumClassesParms), 0, sizeof((yyval.xtokEnumClassesParms)));
    }
    break;

  case 94:
#line 1215 "cimXmlOps.y"
    {
       (yyval.xtokEnumClassesParms).flags = (yyvsp[-1].boolValue) ? FL_includeClassOrigin : 0 ;
       (yyval.xtokEnumClassesParms).flagsSet = FL_includeClassOrigin;
       (yyval.xtokEnumClassesParms).className=0;
    }
    break;

  case 95:
#line 1228 "cimXmlOps.y"
    {
       (yyval.xtokGetInstance).op.count = 2;
       (yyval.xtokGetInstance).op.type = OPS_GetInstance;
       (yyval.xtokGetInstance).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokGetInstance).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokGetInstance).flags = FL_localOnly;
       (yyval.xtokGetInstance).propertyList.values = NULL;
       (yyval.xtokGetInstance).properties=0;
       (yyval.xtokGetInstance).instNameSet = 0;

       setRequest(parm,&(yyval.xtokGetInstance),sizeof(XtokGetInstance),OPS_GetInstance);
    }
    break;

  case 96:
#line 1241 "cimXmlOps.y"
    {
       (yyval.xtokGetInstance).op.count = 2;
       (yyval.xtokGetInstance).op.type = OPS_GetInstance;
       (yyval.xtokGetInstance).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokGetInstance).op.className=setCharsMsgSegment((yyvsp[0].xtokGetInstanceParmsList).instanceName.className);
       (yyval.xtokGetInstance).flags = ((yyvsp[0].xtokGetInstanceParmsList).flags & (yyvsp[0].xtokGetInstanceParmsList).flagsSet) | ((~(yyvsp[0].xtokGetInstanceParmsList).flagsSet) & (FL_localOnly));
       (yyval.xtokGetInstance).instanceName = (yyvsp[0].xtokGetInstanceParmsList).instanceName;
       (yyval.xtokGetInstance).instNameSet = (yyvsp[0].xtokGetInstanceParmsList).instNameSet;
       (yyval.xtokGetInstance).propertyList = (yyvsp[0].xtokGetInstanceParmsList).propertyList;
       (yyval.xtokGetInstance).properties=(yyvsp[0].xtokGetInstanceParmsList).properties;

       setRequest(parm,&(yyval.xtokGetInstance),sizeof(XtokGetInstance),OPS_GetInstance);
    }
    break;

  case 97:
#line 1258 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParmsList).flags=(yyvsp[0].xtokGetInstanceParms).flags;
       (yyval.xtokGetInstanceParmsList).flagsSet=(yyvsp[0].xtokGetInstanceParms).flagsSet;
       if ((yyvsp[0].xtokGetInstanceParms).instNameSet) (yyval.xtokGetInstanceParmsList).instanceName=(yyvsp[0].xtokGetInstanceParms).instanceName;
       (yyval.xtokGetInstanceParmsList).instNameSet = (yyvsp[0].xtokGetInstanceParms).instNameSet;
       if ((yyvsp[0].xtokGetInstanceParms).propertyList.values) {
          (yyval.xtokGetInstanceParmsList).propertyList=(yyvsp[0].xtokGetInstanceParms).propertyList;
          (yyval.xtokGetInstanceParmsList).properties=(yyvsp[0].xtokGetInstanceParms).properties;
       }
    }
    break;

  case 98:
#line 1269 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParmsList).flags=(yyvsp[-1].xtokGetInstanceParmsList).flags|(yyvsp[0].xtokGetInstanceParms).flags;
       (yyval.xtokGetInstanceParmsList).flagsSet=(yyvsp[-1].xtokGetInstanceParmsList).flagsSet|(yyvsp[0].xtokGetInstanceParms).flagsSet;
       if ((yyvsp[0].xtokGetInstanceParms).instNameSet) (yyval.xtokGetInstanceParmsList).instanceName=(yyvsp[0].xtokGetInstanceParms).instanceName;
       (yyval.xtokGetInstanceParmsList).instNameSet = (yyvsp[0].xtokGetInstanceParms).instNameSet;
       if ((yyvsp[0].xtokGetInstanceParms).propertyList.values) {
          (yyval.xtokGetInstanceParmsList).propertyList=(yyvsp[0].xtokGetInstanceParms).propertyList;
          (yyval.xtokGetInstanceParmsList).properties=(yyvsp[0].xtokGetInstanceParms).properties;
       }
    }
    break;

  case 99:
#line 1283 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParms).instanceName = (yyvsp[-1].xtokInstanceName);
       (yyval.xtokGetInstanceParms).flags = (yyval.xtokGetInstanceParms).flagsSet = 0 ;
       (yyval.xtokGetInstanceParms).propertyList.values=0;
       (yyval.xtokGetInstanceParms).instNameSet = 1;
       (yyval.xtokGetInstanceParms).properties=0;
    }
    break;

  case 100:
#line 1291 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParms).flags = (yyvsp[-1].boolValue) ? FL_localOnly : 0 ;
       (yyval.xtokGetInstanceParms).flagsSet = FL_localOnly;
       (yyval.xtokGetInstanceParms).propertyList.values=0;
       (yyval.xtokGetInstanceParms).properties=(yyval.xtokGetInstanceParms).instNameSet=0;
    }
    break;

  case 101:
#line 1298 "cimXmlOps.y"
    {
       memset(&(yyval.xtokGetInstanceParms), 0, sizeof((yyval.xtokGetInstanceParms)));
    }
    break;

  case 102:
#line 1302 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokGetInstanceParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokGetInstanceParms).propertyList.values=0;
       (yyval.xtokGetInstanceParms).properties=(yyval.xtokGetInstanceParms).instNameSet=0;
    }
    break;

  case 103:
#line 1309 "cimXmlOps.y"
    {
      memset(&(yyval.xtokGetInstanceParms), 0, sizeof((yyval.xtokGetInstanceParms)));
    }
    break;

  case 104:
#line 1313 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParms).flags = (yyvsp[-1].boolValue) ? FL_includeClassOrigin : 0 ;
       (yyval.xtokGetInstanceParms).flagsSet = FL_includeClassOrigin;
       (yyval.xtokGetInstanceParms).propertyList.values=0;
       (yyval.xtokGetInstanceParms).properties=(yyval.xtokGetInstanceParms).instNameSet=0;
    }
    break;

  case 105:
#line 1320 "cimXmlOps.y"
    {
      memset(&(yyval.xtokGetInstanceParms), 0, sizeof((yyval.xtokGetInstanceParms)));
    }
    break;

  case 106:
#line 1324 "cimXmlOps.y"
    {
       (yyval.xtokGetInstanceParms).propertyList=(yyvsp[-1].xtokValueArray);
       (yyval.xtokGetInstanceParms).properties=(yyvsp[-1].xtokValueArray).next;
       (yyval.xtokGetInstanceParms).instNameSet=0;
       (yyval.xtokGetInstanceParms).flags = (yyval.xtokGetInstanceParms).flagsSet = 0 ;
    }
    break;

  case 107:
#line 1331 "cimXmlOps.y"
    {
      memset(&(yyval.xtokGetInstanceParms), 0, sizeof((yyval.xtokGetInstanceParms)));
    }
    break;

  case 108:
#line 1344 "cimXmlOps.y"
    {
       (yyval.xtokCreateClass).op.count = 3;
       (yyval.xtokCreateClass).op.type = OPS_CreateClass;
       (yyval.xtokCreateClass).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokCreateClass).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokCreateClass).superClass=NULL;

       setRequest(parm,&(yyval.xtokCreateClass),sizeof(XtokCreateClass),OPS_CreateClass);
    }
    break;

  case 109:
#line 1354 "cimXmlOps.y"
    {
       (yyval.xtokCreateClass).op.count = 3;
       (yyval.xtokCreateClass).op.type = OPS_CreateClass;
       (yyval.xtokCreateClass).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokCreateClass).op.className=setCharsMsgSegment((yyvsp[0].xtokCreateClassParm).cls.className);
       (yyval.xtokCreateClass).superClass=(yyvsp[0].xtokCreateClassParm).cls.superClass;
       (yyval.xtokCreateClass).cls = (yyvsp[0].xtokCreateClassParm).cls;

       setRequest(parm,&(yyval.xtokCreateClass),sizeof(XtokCreateClass),OPS_CreateClass);
    }
    break;

  case 110:
#line 1368 "cimXmlOps.y"
    {
       (yyval.xtokCreateClassParm).cls = (yyvsp[-1].xtokClass);
    }
    break;

  case 111:
#line 1381 "cimXmlOps.y"
    {
       (yyval.xtokCreateInstance).op.count = 2;
       (yyval.xtokCreateInstance).op.type = OPS_CreateInstance;
       (yyval.xtokCreateInstance).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokCreateInstance).op.className=setCharsMsgSegment(NULL);

       setRequest(parm,&(yyval.xtokCreateInstance),sizeof(XtokCreateInstance),OPS_CreateInstance);
    }
    break;

  case 112:
#line 1390 "cimXmlOps.y"
    {
       (yyval.xtokCreateInstance).op.count = 2;
       (yyval.xtokCreateInstance).op.type = OPS_CreateInstance;
       (yyval.xtokCreateInstance).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokCreateInstance).op.className=setCharsMsgSegment((yyvsp[0].xtokCreateInstanceParm).instance.className);
       (yyval.xtokCreateInstance).instance = (yyvsp[0].xtokCreateInstanceParm).instance;

       setRequest(parm,&(yyval.xtokCreateInstance),sizeof(XtokCreateInstance),OPS_CreateInstance);
    }
    break;

  case 113:
#line 1404 "cimXmlOps.y"
    {
       (yyval.xtokCreateInstanceParm).instance = (yyvsp[-1].xtokInstance);
    }
    break;

  case 114:
#line 1416 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstance).op.count = 2;
       (yyval.xtokModifyInstance).op.type = OPS_ModifyInstance;
       (yyval.xtokModifyInstance).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokModifyInstance).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokModifyInstance).flags = FL_includeQualifiers;
       (yyval.xtokModifyInstance).propertyList.values = 0;
       (yyval.xtokModifyInstance).properties=0;

       setRequest(parm,&(yyval.xtokModifyInstance),sizeof(XtokModifyInstance),OPS_ModifyInstance);
    }
    break;

  case 115:
#line 1428 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstance).op.count = 2;
       (yyval.xtokModifyInstance).op.type = OPS_ModifyInstance;
       (yyval.xtokModifyInstance).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokModifyInstance).op.className=setCharsMsgSegment((yyvsp[0].xtokModifyInstanceParmsList).namedInstance.path.className);
       (yyval.xtokModifyInstance).namedInstance = (yyvsp[0].xtokModifyInstanceParmsList).namedInstance;
       (yyval.xtokModifyInstance).flags = (yyvsp[0].xtokModifyInstanceParmsList).flags | ((~(yyvsp[0].xtokModifyInstanceParmsList).flagsSet) & (FL_includeQualifiers));
       (yyval.xtokModifyInstance).propertyList = (yyvsp[0].xtokModifyInstanceParmsList).propertyList;
       (yyval.xtokModifyInstance).properties=(yyvsp[0].xtokModifyInstanceParmsList).properties;

       setRequest(parm,&(yyval.xtokModifyInstance),sizeof(XtokModifyInstance),OPS_ModifyInstance);
    }
    break;

  case 116:
#line 1444 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstanceParmsList).flags=(yyvsp[0].xtokModifyInstanceParms).flags;
       (yyval.xtokModifyInstanceParmsList).flagsSet=(yyvsp[0].xtokModifyInstanceParms).flagsSet;
       if ((yyvsp[0].xtokModifyInstanceParms).namedInstSet) (yyval.xtokModifyInstanceParmsList).namedInstance=(yyvsp[0].xtokModifyInstanceParms).namedInstance;
       (yyval.xtokModifyInstanceParmsList).namedInstSet = (yyvsp[0].xtokModifyInstanceParms).namedInstSet;
       if ((yyvsp[0].xtokModifyInstanceParms).propertyList.values) {
          (yyval.xtokModifyInstanceParmsList).propertyList=(yyvsp[0].xtokModifyInstanceParms).propertyList;
          (yyval.xtokModifyInstanceParmsList).properties=(yyvsp[0].xtokModifyInstanceParms).properties;
       }
    }
    break;

  case 117:
#line 1455 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstanceParmsList).flags=(yyvsp[-1].xtokModifyInstanceParmsList).flags|(yyvsp[0].xtokModifyInstanceParms).flags;
       (yyval.xtokModifyInstanceParmsList).flagsSet=(yyvsp[-1].xtokModifyInstanceParmsList).flagsSet|(yyvsp[0].xtokModifyInstanceParms).flagsSet;
       if ((yyvsp[0].xtokModifyInstanceParms).namedInstSet) (yyval.xtokModifyInstanceParmsList).namedInstance=(yyvsp[0].xtokModifyInstanceParms).namedInstance;
       (yyval.xtokModifyInstanceParmsList).namedInstSet = (yyvsp[0].xtokModifyInstanceParms).namedInstSet;
       if ((yyvsp[0].xtokModifyInstanceParms).propertyList.values) {
          (yyval.xtokModifyInstanceParmsList).propertyList=(yyvsp[0].xtokModifyInstanceParms).propertyList;
          (yyval.xtokModifyInstanceParmsList).properties=(yyvsp[0].xtokModifyInstanceParms).properties;
       }
    }
    break;

  case 118:
#line 1470 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstanceParms).namedInstance=(yyvsp[-1].xtokNamedInstance);
       (yyval.xtokModifyInstanceParms).namedInstSet=1;
       (yyval.xtokModifyInstanceParms).propertyList.values=NULL;
       (yyval.xtokModifyInstanceParms).properties=0;
       (yyval.xtokModifyInstanceParms).flags = (yyval.xtokModifyInstanceParms).flagsSet = 0 ;
    }
    break;

  case 119:
#line 1478 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstanceParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokModifyInstanceParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokModifyInstanceParms).propertyList.values=0;
       (yyval.xtokModifyInstanceParms).properties=(yyval.xtokModifyInstanceParms).namedInstSet=0;
    }
    break;

  case 120:
#line 1485 "cimXmlOps.y"
    {
       memset(&(yyval.xtokModifyInstanceParms), 0, sizeof((yyval.xtokModifyInstanceParms)));
    }
    break;

  case 121:
#line 1489 "cimXmlOps.y"
    {
       (yyval.xtokModifyInstanceParms).propertyList=(yyvsp[-1].xtokValueArray);
       (yyval.xtokModifyInstanceParms).properties=(yyvsp[-1].xtokValueArray).next;
       (yyval.xtokModifyInstanceParms).namedInstSet=0;
       (yyval.xtokModifyInstanceParms).flags = (yyval.xtokModifyInstanceParms).flagsSet = 0 ;
    }
    break;

  case 122:
#line 1496 "cimXmlOps.y"
    {
       memset(&(yyval.xtokModifyInstanceParms), 0, sizeof((yyval.xtokModifyInstanceParms)));
    }
    break;

  case 123:
#line 1508 "cimXmlOps.y"
    {
       (yyval.xtokDeleteClass).op.count = 2;
       (yyval.xtokDeleteClass).op.type = OPS_DeleteClass;
       (yyval.xtokDeleteClass).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokDeleteClass).op.className=setCharsMsgSegment(NULL);

       setRequest(parm,&(yyval.xtokDeleteClass),sizeof(XtokDeleteClass),OPS_DeleteClass);
    }
    break;

  case 124:
#line 1517 "cimXmlOps.y"
    {
       (yyval.xtokDeleteClass).op.count = 2;
       (yyval.xtokDeleteClass).op.type = OPS_DeleteClass;
       (yyval.xtokDeleteClass).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokDeleteClass).op.className=setCharsMsgSegment((yyvsp[0].xtokDeleteClassParm).className);
       (yyval.xtokDeleteClass).className = (yyvsp[0].xtokDeleteClassParm).className;

       setRequest(parm,&(yyval.xtokDeleteClass),sizeof(XtokDeleteClass),OPS_DeleteClass);
    }
    break;

  case 125:
#line 1531 "cimXmlOps.y"
    {
       (yyval.xtokDeleteClassParm).className = (yyvsp[-1].className);
    }
    break;

  case 126:
#line 1543 "cimXmlOps.y"
    {
       (yyval.xtokDeleteInstance).op.count = 2;
       (yyval.xtokDeleteInstance).op.type = OPS_DeleteInstance;
       (yyval.xtokDeleteInstance).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokDeleteInstance).op.className=setCharsMsgSegment(NULL);

       setRequest(parm,&(yyval.xtokDeleteInstance),sizeof(XtokDeleteInstance),OPS_DeleteInstance);
    }
    break;

  case 127:
#line 1552 "cimXmlOps.y"
    {
       (yyval.xtokDeleteInstance).op.count = 2;
       (yyval.xtokDeleteInstance).op.type = OPS_DeleteInstance;
       (yyval.xtokDeleteInstance).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokDeleteInstance).op.className=setCharsMsgSegment((yyvsp[0].xtokDeleteInstanceParm).instanceName.className);
       (yyval.xtokDeleteInstance).instanceName = (yyvsp[0].xtokDeleteInstanceParm).instanceName;

       setRequest(parm,&(yyval.xtokDeleteInstance),sizeof(XtokDeleteInstance),OPS_DeleteInstance);
    }
    break;

  case 128:
#line 1566 "cimXmlOps.y"
    {
       (yyval.xtokDeleteInstanceParm).instanceName = (yyvsp[-1].xtokInstanceName);
    }
    break;

  case 129:
#line 1579 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstanceNames).op.count = 2;
       (yyval.xtokEnumInstanceNames).op.type = OPS_EnumerateInstanceNames;
       (yyval.xtokEnumInstanceNames).op.nameSpace=setCharsMsgSegment((yyvsp[-3].xtokLocalNameSpacePath));
       (yyval.xtokEnumInstanceNames).op.className=setCharsMsgSegment((yyvsp[-1].className));

       setRequest(parm,&(yyval.xtokEnumInstanceNames),sizeof(XtokEnumInstanceNames),OPS_EnumerateInstanceNames);
    }
    break;

  case 130:
#line 1597 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstances).op.count = 2;
       (yyval.xtokEnumInstances).op.type = OPS_EnumerateInstances;
       (yyval.xtokEnumInstances).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokEnumInstances).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokEnumInstances).flags = FL_localOnly | FL_deepInheritance;
       (yyval.xtokEnumInstances).propertyList.values = NULL;
       (yyval.xtokEnumInstances).properties=0;

       setRequest(parm,&(yyval.xtokEnumInstances),sizeof(XtokEnumInstances),OPS_EnumerateInstances);
    }
    break;

  case 131:
#line 1609 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstances).op.count = 2;
       (yyval.xtokEnumInstances).op.type = OPS_EnumerateInstances;
       (yyval.xtokEnumInstances).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokEnumInstances).op.className=setCharsMsgSegment((yyvsp[0].xtokEnumInstancesParmsList).className);
       (yyval.xtokEnumInstances).flags = ((yyvsp[0].xtokEnumInstancesParmsList).flags & (yyvsp[0].xtokEnumInstancesParmsList).flagsSet) | ((~(yyvsp[0].xtokEnumInstancesParmsList).flagsSet) & (FL_localOnly));
       (yyval.xtokEnumInstances).propertyList = (yyvsp[0].xtokEnumInstancesParmsList).propertyList;
       (yyval.xtokEnumInstances).properties=(yyvsp[0].xtokEnumInstancesParmsList).properties;

       setRequest(parm,&(yyval.xtokEnumInstances),sizeof(XtokEnumInstances),OPS_EnumerateInstances);
    }
    break;

  case 132:
#line 1624 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParmsList).flags=(yyvsp[0].xtokEnumInstancesParms).flags;
       (yyval.xtokEnumInstancesParmsList).flagsSet=(yyvsp[0].xtokEnumInstancesParms).flagsSet;
       if ((yyvsp[0].xtokEnumInstancesParms).className) (yyval.xtokEnumInstancesParmsList).className=(yyvsp[0].xtokEnumInstancesParms).className;
       if ((yyvsp[0].xtokEnumInstancesParms).propertyList.values) {
          (yyval.xtokEnumInstancesParmsList).propertyList=(yyvsp[0].xtokEnumInstancesParms).propertyList;
          (yyval.xtokEnumInstancesParmsList).properties=(yyvsp[0].xtokEnumInstancesParms).properties;
       }
    }
    break;

  case 133:
#line 1634 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParmsList).flags=(yyvsp[-1].xtokEnumInstancesParmsList).flags|(yyvsp[0].xtokEnumInstancesParms).flags;
       (yyval.xtokEnumInstancesParmsList).flagsSet=(yyvsp[-1].xtokEnumInstancesParmsList).flagsSet|(yyvsp[0].xtokEnumInstancesParms).flagsSet;
       if ((yyvsp[0].xtokEnumInstancesParms).className) (yyval.xtokEnumInstancesParmsList).className=(yyvsp[0].xtokEnumInstancesParms).className;
       if ((yyvsp[0].xtokEnumInstancesParms).propertyList.values) {
          (yyval.xtokEnumInstancesParmsList).propertyList=(yyvsp[0].xtokEnumInstancesParms).propertyList;
          (yyval.xtokEnumInstancesParmsList).properties=(yyvsp[0].xtokEnumInstancesParms).properties;
       }
    }
    break;

  case 134:
#line 1647 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParms).className = (yyvsp[-1].className);
       (yyval.xtokEnumInstancesParms).flags = (yyval.xtokEnumInstancesParms).flagsSet = 0 ;
       (yyval.xtokEnumInstancesParms).properties=0;
       (yyval.xtokEnumInstancesParms).propertyList.values=0;
    }
    break;

  case 135:
#line 1654 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParms).flags = (yyvsp[-1].boolValue) ? FL_localOnly : 0 ;
       (yyval.xtokEnumInstancesParms).flagsSet = FL_localOnly;
       (yyval.xtokEnumInstancesParms).className=0;
       (yyval.xtokEnumInstancesParms).properties=0;
       (yyval.xtokEnumInstancesParms).propertyList.values=0;
    }
    break;

  case 136:
#line 1662 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumInstancesParms), 0, sizeof((yyval.xtokEnumInstancesParms)));
    }
    break;

  case 137:
#line 1666 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokEnumInstancesParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokEnumInstancesParms).className=0;
       (yyval.xtokEnumInstancesParms).properties=0;
       (yyval.xtokEnumInstancesParms).propertyList.values=0;
    }
    break;

  case 138:
#line 1674 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumInstancesParms), 0, sizeof((yyval.xtokEnumInstancesParms)));
    }
    break;

  case 139:
#line 1678 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParms).flags = (yyvsp[-1].boolValue) ? FL_deepInheritance : 0 ;
       (yyval.xtokEnumInstancesParms).flagsSet = FL_deepInheritance;
       (yyval.xtokEnumInstancesParms).className=0;
       (yyval.xtokEnumInstancesParms).properties=0;
       (yyval.xtokEnumInstancesParms).propertyList.values=0;
    }
    break;

  case 140:
#line 1686 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumInstancesParms), 0, sizeof((yyval.xtokEnumInstancesParms)));
    }
    break;

  case 141:
#line 1690 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParms).flags = (yyvsp[-1].boolValue) ? FL_includeClassOrigin : 0 ;
       (yyval.xtokEnumInstancesParms).flagsSet = FL_includeClassOrigin;
       (yyval.xtokEnumInstancesParms).className=0;
       (yyval.xtokEnumInstancesParms).properties=0;
       (yyval.xtokEnumInstancesParms).propertyList.values=0;
    }
    break;

  case 142:
#line 1698 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumInstancesParms), 0, sizeof((yyval.xtokEnumInstancesParms)));
    }
    break;

  case 143:
#line 1702 "cimXmlOps.y"
    {
       (yyval.xtokEnumInstancesParms).propertyList=(yyvsp[-1].xtokValueArray);
       (yyval.xtokEnumInstancesParms).properties=(yyvsp[-1].xtokValueArray).next;
       (yyval.xtokEnumInstancesParms).className=0;
       (yyval.xtokEnumInstancesParms).flags = (yyval.xtokEnumInstancesParms).flagsSet = 0 ;
    }
    break;

  case 144:
#line 1709 "cimXmlOps.y"
    {
       memset(&(yyval.xtokEnumInstancesParms), 0, sizeof((yyval.xtokEnumInstancesParms)));
    }
    break;

  case 145:
#line 1725 "cimXmlOps.y"
    {
       (yyval.xtokExecQuery).op.count = 3;
       (yyval.xtokExecQuery).op.type = OPS_ExecQuery;
       (yyval.xtokExecQuery).op.nameSpace=setCharsMsgSegment((yyvsp[-6].xtokLocalNameSpacePath));
       (yyval.xtokExecQuery).op.query=setCharsMsgSegment((yyvsp[-4].xtokValue).value);
       (yyval.xtokExecQuery).op.queryLang=setCharsMsgSegment((yyvsp[-1].xtokValue).value);

       setRequest(parm,&(yyval.xtokExecQuery),sizeof(XtokExecQuery),OPS_ExecQuery);
    }
    break;

  case 146:
#line 1737 "cimXmlOps.y"
    {
       (yyval.xtokExecQuery).op.count = 3;
       (yyval.xtokExecQuery).op.type = OPS_ExecQuery;
       (yyval.xtokExecQuery).op.nameSpace=setCharsMsgSegment((yyvsp[-6].xtokLocalNameSpacePath));
       (yyval.xtokExecQuery).op.query=setCharsMsgSegment((yyvsp[-1].xtokValue).value);
       (yyval.xtokExecQuery).op.queryLang=setCharsMsgSegment((yyvsp[-4].xtokValue).value);

       setRequest(parm,&(yyval.xtokExecQuery),sizeof(XtokExecQuery),OPS_ExecQuery);
    }
    break;

  case 147:
#line 1756 "cimXmlOps.y"
    {
       (yyval.xtokAssociators).op.count = 6;
       (yyval.xtokAssociators).op.type = OPS_Associators;
       (yyval.xtokAssociators).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokAssociators).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokAssociators).op.assocClass=setCharsMsgSegment(NULL);
       (yyval.xtokAssociators).op.resultClass=setCharsMsgSegment(NULL);
       (yyval.xtokAssociators).op.role=setCharsMsgSegment(NULL);
       (yyval.xtokAssociators).op.resultRole=setCharsMsgSegment(NULL);
       (yyval.xtokAssociators).flags = 0;
       (yyval.xtokAssociators).objNameSet = 0;
       (yyval.xtokAssociators).propertyList.values = 0;
       (yyval.xtokAssociators).properties=0;

       setRequest(parm,&(yyval.xtokAssociators),sizeof(XtokAssociators),OPS_Associators);
    }
    break;

  case 148:
#line 1773 "cimXmlOps.y"
    {
       (yyval.xtokAssociators).op.count = 6;
       (yyval.xtokAssociators).op.type = OPS_Associators;
       (yyval.xtokAssociators).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokAssociators).op.className=setCharsMsgSegment((yyvsp[0].xtokAssociatorsParmsList).objectName.className);
       (yyval.xtokAssociators).op.assocClass=setCharsMsgSegment((yyvsp[0].xtokAssociatorsParmsList).assocClass);
       (yyval.xtokAssociators).op.resultClass=setCharsMsgSegment((yyvsp[0].xtokAssociatorsParmsList).resultClass);
       (yyval.xtokAssociators).op.role=setCharsMsgSegment((yyvsp[0].xtokAssociatorsParmsList).role);
       (yyval.xtokAssociators).op.resultRole=setCharsMsgSegment((yyvsp[0].xtokAssociatorsParmsList).resultRole);
       (yyval.xtokAssociators).flags = ((yyvsp[0].xtokAssociatorsParmsList).flags & (yyvsp[0].xtokAssociatorsParmsList).flagsSet) | (~(yyvsp[0].xtokAssociatorsParmsList).flagsSet & 0);
       (yyval.xtokAssociators).objectName = (yyvsp[0].xtokAssociatorsParmsList).objectName;
       (yyval.xtokAssociators).objNameSet = (yyvsp[0].xtokAssociatorsParmsList).objNameSet;
       (yyval.xtokAssociators).propertyList = (yyvsp[0].xtokAssociatorsParmsList).propertyList;
       (yyval.xtokAssociators).properties=(yyvsp[0].xtokAssociatorsParmsList).properties;

       setRequest(parm,&(yyval.xtokAssociators),sizeof(XtokAssociators),OPS_Associators);
    }
    break;

  case 149:
#line 1794 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParmsList).flags=(yyvsp[0].xtokAssociatorsParms).flags;
       (yyval.xtokAssociatorsParmsList).flagsSet=(yyvsp[0].xtokAssociatorsParms).flagsSet;
       if ((yyvsp[0].xtokAssociatorsParms).objNameSet)  {
          (yyval.xtokAssociatorsParmsList).objectName=(yyvsp[0].xtokAssociatorsParms).objectName;
          (yyval.xtokAssociatorsParmsList).objNameSet = (yyvsp[0].xtokAssociatorsParms).objNameSet;
       }
       (yyval.xtokAssociatorsParmsList).assocClass=(yyvsp[0].xtokAssociatorsParms).assocClass;
       (yyval.xtokAssociatorsParmsList).resultClass=(yyvsp[0].xtokAssociatorsParms).resultClass;
       (yyval.xtokAssociatorsParmsList).role=(yyvsp[0].xtokAssociatorsParms).role;
       (yyval.xtokAssociatorsParmsList).resultRole=(yyvsp[0].xtokAssociatorsParms).resultRole;
       if ((yyvsp[0].xtokAssociatorsParms).propertyList.values) {
          (yyval.xtokAssociatorsParmsList).propertyList=(yyvsp[0].xtokAssociatorsParms).propertyList;
          (yyval.xtokAssociatorsParmsList).properties=(yyvsp[0].xtokAssociatorsParms).properties;
       }
    }
    break;

  case 150:
#line 1811 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParmsList).flags=(yyvsp[-1].xtokAssociatorsParmsList).flags|(yyvsp[0].xtokAssociatorsParms).flags;
       (yyval.xtokAssociatorsParmsList).flagsSet=(yyvsp[-1].xtokAssociatorsParmsList).flagsSet|(yyvsp[0].xtokAssociatorsParms).flagsSet;
       if ((yyvsp[0].xtokAssociatorsParms).assocClass) (yyval.xtokAssociatorsParmsList).assocClass=(yyvsp[0].xtokAssociatorsParms).assocClass;
       else if ((yyvsp[0].xtokAssociatorsParms).resultClass) (yyval.xtokAssociatorsParmsList).resultClass=(yyvsp[0].xtokAssociatorsParms).resultClass;
       else if ((yyvsp[0].xtokAssociatorsParms).role) (yyval.xtokAssociatorsParmsList).role=(yyvsp[0].xtokAssociatorsParms).role;
       else if ((yyvsp[0].xtokAssociatorsParms).resultRole) (yyval.xtokAssociatorsParmsList).resultRole=(yyvsp[0].xtokAssociatorsParms).resultRole;
       else if ((yyvsp[0].xtokAssociatorsParms).objNameSet) {
          (yyval.xtokAssociatorsParmsList).objectName=(yyvsp[0].xtokAssociatorsParms).objectName;
          (yyval.xtokAssociatorsParmsList).objNameSet = (yyvsp[0].xtokAssociatorsParms).objNameSet;
       }
       else if ((yyvsp[0].xtokAssociatorsParms).propertyList.values) {
          (yyval.xtokAssociatorsParmsList).propertyList=(yyvsp[0].xtokAssociatorsParms).propertyList;
          (yyval.xtokAssociatorsParmsList).properties=(yyvsp[0].xtokAssociatorsParms).properties;
       }
    }
    break;

  case 151:
#line 1831 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).objectName = (yyvsp[-1].xtokInstanceName);
       (yyval.xtokAssociatorsParms).objNameSet = 1;
       (yyval.xtokAssociatorsParms).flags = (yyval.xtokAssociatorsParms).flagsSet = 0 ;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).role=(yyval.xtokAssociatorsParms).resultRole=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 152:
#line 1840 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).assocClass = (yyvsp[-1].className);
       (yyval.xtokAssociatorsParms).objNameSet=(yyval.xtokAssociatorsParms).flags = (yyval.xtokAssociatorsParms).flagsSet = 0 ;
       (yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).role=(yyval.xtokAssociatorsParms).resultRole=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 153:
#line 1848 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 154:
#line 1852 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).resultClass = (yyvsp[-1].className);
       (yyval.xtokAssociatorsParms).objNameSet=(yyval.xtokAssociatorsParms).flags = (yyval.xtokAssociatorsParms).flagsSet = 0 ;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).role=(yyval.xtokAssociatorsParms).resultRole=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 155:
#line 1860 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 156:
#line 1864 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).role = (yyvsp[-1].xtokValue).value;
       (yyval.xtokAssociatorsParms).objNameSet=(yyval.xtokAssociatorsParms).flags = (yyval.xtokAssociatorsParms).flagsSet = 0 ;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).resultRole=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 157:
#line 1872 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 158:
#line 1876 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).resultRole = (yyvsp[-1].xtokValue).value;
       (yyval.xtokAssociatorsParms).objNameSet=(yyval.xtokAssociatorsParms).flags = (yyval.xtokAssociatorsParms).flagsSet = 0 ;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).role=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 159:
#line 1884 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 160:
#line 1888 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokAssociatorsParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokAssociatorsParms).objNameSet=0;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).role=(yyval.xtokAssociatorsParms).resultRole=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 161:
#line 1897 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 162:
#line 1901 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).flags = (yyvsp[-1].boolValue) ? FL_includeClassOrigin : 0 ;
       (yyval.xtokAssociatorsParms).flagsSet = FL_includeClassOrigin;
       (yyval.xtokAssociatorsParms).objNameSet=0;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).role=(yyval.xtokAssociatorsParms).resultRole=0;
       (yyval.xtokAssociatorsParms).properties=0;
       (yyval.xtokAssociatorsParms).propertyList.values=0;
    }
    break;

  case 163:
#line 1910 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 164:
#line 1914 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorsParms).propertyList=(yyvsp[-1].xtokValueArray);
       (yyval.xtokAssociatorsParms).properties=(yyvsp[-1].xtokValueArray).next;
       (yyval.xtokAssociatorsParms).objNameSet=(yyval.xtokAssociatorsParms).flags = (yyval.xtokAssociatorsParms).flagsSet = 0 ;
       (yyval.xtokAssociatorsParms).assocClass=(yyval.xtokAssociatorsParms).resultClass=(yyval.xtokAssociatorsParms).role=(yyval.xtokAssociatorsParms).resultRole=0;
    }
    break;

  case 165:
#line 1921 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorsParms), 0, sizeof((yyval.xtokAssociatorsParms)));
    }
    break;

  case 166:
#line 1936 "cimXmlOps.y"
    {
       (yyval.xtokReferences).op.count = 4;
       (yyval.xtokReferences).op.type = OPS_References;
       (yyval.xtokReferences).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokReferences).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokReferences).op.resultClass=setCharsMsgSegment(NULL);
       (yyval.xtokReferences).op.role=setCharsMsgSegment(NULL);
       (yyval.xtokReferences).flags = 0;
       (yyval.xtokReferences).objNameSet = 0;
       (yyval.xtokReferences).propertyList.values = 0;
       (yyval.xtokReferences).properties=0;

       setRequest(parm,&(yyval.xtokReferences),sizeof(XtokReferences),OPS_References);
    }
    break;

  case 167:
#line 1951 "cimXmlOps.y"
    {
       (yyval.xtokReferences).op.count = 4;
       (yyval.xtokReferences).op.type = OPS_References;
       (yyval.xtokReferences).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokReferences).op.className=setCharsMsgSegment((yyvsp[0].xtokReferencesParmsList).objectName.className);
       (yyval.xtokReferences).op.resultClass=setCharsMsgSegment((yyvsp[0].xtokReferencesParmsList).resultClass);
       (yyval.xtokReferences).op.role=setCharsMsgSegment((yyvsp[0].xtokReferencesParmsList).role);
       (yyval.xtokReferences).flags = ((yyvsp[0].xtokReferencesParmsList).flags & (yyvsp[0].xtokReferencesParmsList).flagsSet) | (~(yyvsp[0].xtokReferencesParmsList).flagsSet & 0);
       (yyval.xtokReferences).objectName = (yyvsp[0].xtokReferencesParmsList).objectName;
       (yyval.xtokReferences).objNameSet = (yyvsp[0].xtokReferencesParmsList).objNameSet;
       (yyval.xtokReferences).propertyList = (yyvsp[0].xtokReferencesParmsList).propertyList;
       (yyval.xtokReferences).properties=(yyvsp[0].xtokReferencesParmsList).properties;

       setRequest(parm,&(yyval.xtokReferences),sizeof(XtokReferences),OPS_References);
    }
    break;

  case 168:
#line 1970 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParmsList).flags=(yyvsp[0].xtokReferencesParms).flags;
       (yyval.xtokReferencesParmsList).flagsSet=(yyvsp[0].xtokReferencesParms).flagsSet;
       if ((yyvsp[0].xtokReferencesParms).objNameSet)  {
          (yyval.xtokReferencesParmsList).objectName=(yyvsp[0].xtokReferencesParms).objectName;
          (yyval.xtokReferencesParmsList).objNameSet = (yyvsp[0].xtokReferencesParms).objNameSet;
       }
       (yyval.xtokReferencesParmsList).resultClass=(yyvsp[0].xtokReferencesParms).resultClass;
       (yyval.xtokReferencesParmsList).role=(yyvsp[0].xtokReferencesParms).role;
       if ((yyvsp[0].xtokReferencesParms).propertyList.values) {
          (yyval.xtokReferencesParmsList).propertyList=(yyvsp[0].xtokReferencesParms).propertyList;
          (yyval.xtokReferencesParmsList).properties=(yyvsp[0].xtokReferencesParms).properties;
       }
    }
    break;

  case 169:
#line 1985 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParmsList).flags=(yyvsp[-1].xtokReferencesParmsList).flags|(yyvsp[0].xtokReferencesParms).flags;
       (yyval.xtokReferencesParmsList).flagsSet=(yyvsp[-1].xtokReferencesParmsList).flagsSet|(yyvsp[0].xtokReferencesParms).flagsSet;
       if ((yyvsp[0].xtokReferencesParms).resultClass) (yyval.xtokReferencesParmsList).resultClass=(yyvsp[0].xtokReferencesParms).resultClass;
       else if ((yyvsp[0].xtokReferencesParms).role) (yyval.xtokReferencesParmsList).role=(yyvsp[0].xtokReferencesParms).role;
       else if ((yyvsp[0].xtokReferencesParms).objNameSet) {
          (yyval.xtokReferencesParmsList).objectName=(yyvsp[0].xtokReferencesParms).objectName;
          (yyval.xtokReferencesParmsList).objNameSet = (yyvsp[0].xtokReferencesParms).objNameSet;
       }
       else if ((yyvsp[0].xtokReferencesParms).propertyList.values) {
          (yyval.xtokReferencesParmsList).propertyList=(yyvsp[0].xtokReferencesParms).propertyList;
          (yyval.xtokReferencesParmsList).properties=(yyvsp[0].xtokReferencesParms).properties;
       }
    }
    break;

  case 170:
#line 2003 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParms).objectName = (yyvsp[-1].xtokInstanceName);
       (yyval.xtokReferencesParms).objNameSet = 1;
       (yyval.xtokReferencesParms).flags = (yyval.xtokReferencesParms).flagsSet = 0 ;
       (yyval.xtokReferencesParms).resultClass=(yyval.xtokReferencesParms).role=0;
       (yyval.xtokReferencesParms).properties=0;
       (yyval.xtokReferencesParms).propertyList.values=0;
    }
    break;

  case 171:
#line 2012 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParms).resultClass = (yyvsp[-1].className);
       (yyval.xtokReferencesParms).objNameSet=(yyval.xtokReferencesParms).flags = (yyval.xtokReferencesParms).flagsSet = 0 ;
       (yyval.xtokReferencesParms).role=0;
       (yyval.xtokReferencesParms).properties=0;
       (yyval.xtokReferencesParms).propertyList.values=0;
    }
    break;

  case 172:
#line 2020 "cimXmlOps.y"
    {
       memset(&(yyval.xtokReferencesParms), 0, sizeof((yyval.xtokReferencesParms)));
    }
    break;

  case 173:
#line 2024 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParms).role = (yyvsp[-1].xtokValue).value;
       (yyval.xtokReferencesParms).objNameSet=(yyval.xtokReferencesParms).flags = (yyval.xtokReferencesParms).flagsSet = 0 ;
       (yyval.xtokReferencesParms).resultClass=0;
       (yyval.xtokReferencesParms).properties=0;
       (yyval.xtokReferencesParms).propertyList.values=0;
    }
    break;

  case 174:
#line 2032 "cimXmlOps.y"
    {
       memset(&(yyval.xtokReferencesParms), 0, sizeof((yyval.xtokReferencesParms)));
    }
    break;

  case 175:
#line 2036 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParms).flags = (yyvsp[-1].boolValue) ? FL_includeQualifiers : 0 ;
       (yyval.xtokReferencesParms).flagsSet = FL_includeQualifiers;
       (yyval.xtokReferencesParms).objNameSet=0;
       (yyval.xtokReferencesParms).resultClass=(yyval.xtokReferencesParms).role=0;
       (yyval.xtokReferencesParms).properties=0;
       (yyval.xtokReferencesParms).propertyList.values=0;
    }
    break;

  case 176:
#line 2045 "cimXmlOps.y"
    {
       memset(&(yyval.xtokReferencesParms), 0, sizeof((yyval.xtokReferencesParms)));
    }
    break;

  case 177:
#line 2049 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParms).flags = (yyvsp[-1].boolValue) ? FL_includeClassOrigin : 0 ;
       (yyval.xtokReferencesParms).flagsSet = FL_includeClassOrigin;
       (yyval.xtokReferencesParms).objNameSet=0;
       (yyval.xtokReferencesParms).resultClass=(yyval.xtokReferencesParms).role=0;
       (yyval.xtokReferencesParms).properties=0;
       (yyval.xtokReferencesParms).propertyList.values=0;
    }
    break;

  case 178:
#line 2058 "cimXmlOps.y"
    {
       memset(&(yyval.xtokReferencesParms), 0, sizeof((yyval.xtokReferencesParms)));
    }
    break;

  case 179:
#line 2062 "cimXmlOps.y"
    {
       (yyval.xtokReferencesParms).propertyList=(yyvsp[-1].xtokValueArray);
       (yyval.xtokReferencesParms).properties=(yyvsp[-1].xtokValueArray).next;
       (yyval.xtokReferencesParms).objNameSet=(yyval.xtokReferencesParms).flags = (yyval.xtokReferencesParms).flagsSet = 0 ;
       (yyval.xtokReferencesParms).resultClass=(yyval.xtokReferencesParms).role=0;
    }
    break;

  case 180:
#line 2069 "cimXmlOps.y"
    {
       memset(&(yyval.xtokReferencesParms), 0, sizeof((yyval.xtokReferencesParms)));
    }
    break;

  case 181:
#line 2082 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNames).op.count = 6;
       (yyval.xtokAssociatorNames).op.type = OPS_AssociatorNames;
       (yyval.xtokAssociatorNames).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokAssociatorNames).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokAssociatorNames).op.assocClass=setCharsMsgSegment(NULL);
       (yyval.xtokAssociatorNames).op.resultClass=setCharsMsgSegment(NULL);
       (yyval.xtokAssociatorNames).op.role=setCharsMsgSegment(NULL);
       (yyval.xtokAssociatorNames).op.resultRole=setCharsMsgSegment(NULL);
       (yyval.xtokAssociatorNames).objNameSet = 0;

       setRequest(parm,&(yyval.xtokAssociatorNames),sizeof(XtokAssociatorNames),OPS_AssociatorNames);
    }
    break;

  case 182:
#line 2096 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNames).op.count = 6;
       (yyval.xtokAssociatorNames).op.type = OPS_AssociatorNames;
       (yyval.xtokAssociatorNames).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokAssociatorNames).op.className=setCharsMsgSegment((yyvsp[0].xtokAssociatorNamesParmsList).objectName.className);
       (yyval.xtokAssociatorNames).op.assocClass=setCharsMsgSegment((yyvsp[0].xtokAssociatorNamesParmsList).assocClass);
       (yyval.xtokAssociatorNames).op.resultClass=setCharsMsgSegment((yyvsp[0].xtokAssociatorNamesParmsList).resultClass);
       (yyval.xtokAssociatorNames).op.role=setCharsMsgSegment((yyvsp[0].xtokAssociatorNamesParmsList).role);
       (yyval.xtokAssociatorNames).op.resultRole=setCharsMsgSegment((yyvsp[0].xtokAssociatorNamesParmsList).resultRole);
       (yyval.xtokAssociatorNames).objectName = (yyvsp[0].xtokAssociatorNamesParmsList).objectName;
       (yyval.xtokAssociatorNames).objNameSet = (yyvsp[0].xtokAssociatorNamesParmsList).objNameSet;
       setRequest(parm,&(yyval.xtokAssociatorNames),sizeof(XtokAssociatorNames),OPS_AssociatorNames);
    }
    break;

  case 183:
#line 2113 "cimXmlOps.y"
    {
       if ((yyvsp[0].xtokAssociatorNamesParms).objNameSet)  {
          (yyval.xtokAssociatorNamesParmsList).objectName=(yyvsp[0].xtokAssociatorNamesParms).objectName;
          (yyval.xtokAssociatorNamesParmsList).objNameSet = (yyvsp[0].xtokAssociatorNamesParms).objNameSet;
       }
      (yyval.xtokAssociatorNamesParmsList).assocClass=(yyvsp[0].xtokAssociatorNamesParms).assocClass;
      (yyval.xtokAssociatorNamesParmsList).resultClass=(yyvsp[0].xtokAssociatorNamesParms).resultClass;
      (yyval.xtokAssociatorNamesParmsList).role=(yyvsp[0].xtokAssociatorNamesParms).role;
      (yyval.xtokAssociatorNamesParmsList).resultRole=(yyvsp[0].xtokAssociatorNamesParms).resultRole;
    }
    break;

  case 184:
#line 2124 "cimXmlOps.y"
    {
       if ((yyvsp[0].xtokAssociatorNamesParms).assocClass) (yyval.xtokAssociatorNamesParmsList).assocClass=(yyvsp[0].xtokAssociatorNamesParms).assocClass;
       else if ((yyvsp[0].xtokAssociatorNamesParms).resultClass) (yyval.xtokAssociatorNamesParmsList).resultClass=(yyvsp[0].xtokAssociatorNamesParms).resultClass;
       else if ((yyvsp[0].xtokAssociatorNamesParms).role) (yyval.xtokAssociatorNamesParmsList).role=(yyvsp[0].xtokAssociatorNamesParms).role;
       else if ((yyvsp[0].xtokAssociatorNamesParms).resultRole) (yyval.xtokAssociatorNamesParmsList).resultRole=(yyvsp[0].xtokAssociatorNamesParms).resultRole;
       else if ((yyvsp[0].xtokAssociatorNamesParms).objNameSet) {
          (yyval.xtokAssociatorNamesParmsList).objectName=(yyvsp[0].xtokAssociatorNamesParms).objectName;
          (yyval.xtokAssociatorNamesParmsList).objNameSet = (yyvsp[0].xtokAssociatorNamesParms).objNameSet;
       }
    }
    break;

  case 185:
#line 2138 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNamesParms).objectName = (yyvsp[-1].xtokInstanceName);
       (yyval.xtokAssociatorNamesParms).objNameSet = 1;
       (yyval.xtokAssociatorNamesParms).assocClass=(yyval.xtokAssociatorNamesParms).resultClass=(yyval.xtokAssociatorNamesParms).role=(yyval.xtokAssociatorNamesParms).resultRole=0;
    }
    break;

  case 186:
#line 2144 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNamesParms).assocClass = (yyvsp[-1].className);
       (yyval.xtokAssociatorNamesParms).objNameSet = 0 ;
       (yyval.xtokAssociatorNamesParms).resultClass=(yyval.xtokAssociatorNamesParms).role=(yyval.xtokAssociatorNamesParms).resultRole=0;
    }
    break;

  case 187:
#line 2150 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorNamesParms), 0, sizeof((yyval.xtokAssociatorNamesParms)));
    }
    break;

  case 188:
#line 2154 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNamesParms).resultClass = (yyvsp[-1].className);
       (yyval.xtokAssociatorNamesParms).objNameSet = 0 ;
       (yyval.xtokAssociatorNamesParms).assocClass=(yyval.xtokAssociatorNamesParms).role=(yyval.xtokAssociatorNamesParms).resultRole=0;
    }
    break;

  case 189:
#line 2160 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorNamesParms), 0, sizeof((yyval.xtokAssociatorNamesParms)));
    }
    break;

  case 190:
#line 2164 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNamesParms).role = (yyvsp[-1].xtokValue).value;
       (yyval.xtokAssociatorNamesParms).objNameSet = 0 ;
       (yyval.xtokAssociatorNamesParms).assocClass=(yyval.xtokAssociatorNamesParms).resultClass=(yyval.xtokAssociatorNamesParms).resultRole=0;
    }
    break;

  case 191:
#line 2170 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorNamesParms), 0, sizeof((yyval.xtokAssociatorNamesParms)));
    }
    break;

  case 192:
#line 2174 "cimXmlOps.y"
    {
       (yyval.xtokAssociatorNamesParms).resultRole = (yyvsp[-1].xtokValue).value;
       (yyval.xtokAssociatorNamesParms).objNameSet= 0 ;
       (yyval.xtokAssociatorNamesParms).assocClass=(yyval.xtokAssociatorNamesParms).resultClass=(yyval.xtokAssociatorNamesParms).role=0;
    }
    break;

  case 193:
#line 2180 "cimXmlOps.y"
    {
       memset(&(yyval.xtokAssociatorNamesParms), 0, sizeof((yyval.xtokAssociatorNamesParms)));
    }
    break;

  case 194:
#line 2194 "cimXmlOps.y"
    {
       (yyval.xtokReferenceNames).op.count = 4;
       (yyval.xtokReferenceNames).op.type = OPS_ReferenceNames;
       (yyval.xtokReferenceNames).op.nameSpace=setCharsMsgSegment((yyvsp[0].xtokLocalNameSpacePath));
       (yyval.xtokReferenceNames).op.className=setCharsMsgSegment(NULL);
       (yyval.xtokReferenceNames).op.resultClass=setCharsMsgSegment(NULL);
       (yyval.xtokReferenceNames).op.role=setCharsMsgSegment(NULL);
       (yyval.xtokReferenceNames).objNameSet = 0;

       setRequest(parm,&(yyval.xtokReferenceNames),sizeof(XtokReferenceNames),OPS_ReferenceNames);
    }
    break;

  case 195:
#line 2206 "cimXmlOps.y"
    {
       (yyval.xtokReferenceNames).op.count = 4;
       (yyval.xtokReferenceNames).op.type = OPS_ReferenceNames;
       (yyval.xtokReferenceNames).op.nameSpace=setCharsMsgSegment((yyvsp[-1].xtokLocalNameSpacePath));
       (yyval.xtokReferenceNames).op.className=setCharsMsgSegment((yyvsp[0].xtokReferenceNamesParmsList).objectName.className);
       (yyval.xtokReferenceNames).op.resultClass=setCharsMsgSegment((yyvsp[0].xtokReferenceNamesParmsList).resultClass);
       (yyval.xtokReferenceNames).op.role=setCharsMsgSegment((yyvsp[0].xtokReferenceNamesParmsList).role);
       (yyval.xtokReferenceNames).objectName = (yyvsp[0].xtokReferenceNamesParmsList).objectName;
       (yyval.xtokReferenceNames).objNameSet = (yyvsp[0].xtokReferenceNamesParmsList).objNameSet;

       setRequest(parm,&(yyval.xtokReferenceNames),sizeof(XtokReferenceNames),OPS_ReferenceNames);
    }
    break;

  case 196:
#line 2222 "cimXmlOps.y"
    {
      if ((yyvsp[0].xtokReferenceNamesParms).objNameSet)  {
          (yyval.xtokReferenceNamesParmsList).objectName=(yyvsp[0].xtokReferenceNamesParms).objectName;
          (yyval.xtokReferenceNamesParmsList).objNameSet = (yyvsp[0].xtokReferenceNamesParms).objNameSet;
      }
      (yyval.xtokReferenceNamesParmsList).resultClass = (yyvsp[0].xtokReferenceNamesParms).resultClass;
      (yyval.xtokReferenceNamesParmsList).role = (yyvsp[0].xtokReferenceNamesParms).role;
    }
    break;

  case 197:
#line 2231 "cimXmlOps.y"
    {
       if((yyvsp[0].xtokReferenceNamesParms).objNameSet) {
          (yyval.xtokReferenceNamesParmsList).objectName=(yyvsp[0].xtokReferenceNamesParms).objectName;
          (yyval.xtokReferenceNamesParmsList).objNameSet=(yyvsp[0].xtokReferenceNamesParms).objNameSet;
       }
       else if((yyvsp[0].xtokReferenceNamesParms).resultClass) (yyval.xtokReferenceNamesParmsList).resultClass=(yyvsp[0].xtokReferenceNamesParms).resultClass;
       else if((yyvsp[0].xtokReferenceNamesParms).role) (yyval.xtokReferenceNamesParmsList).role=(yyvsp[0].xtokReferenceNamesParms).role;              
    }
    break;

  case 198:
#line 2243 "cimXmlOps.y"
    {
       (yyval.xtokReferenceNamesParms).objectName = (yyvsp[-1].xtokInstanceName);
       (yyval.xtokReferenceNamesParms).objNameSet = 1;
       (yyval.xtokReferenceNamesParms).resultClass=(yyval.xtokReferenceNamesParms).role=0;
    }
    break;

  case 199:
#line 2249 "cimXmlOps.y"
    {
       (yyval.xtokReferenceNamesParms).resultClass = (yyvsp[-1].className);
       (yyval.xtokReferenceNamesParms).objNameSet = 0;
       (yyval.xtokReferenceNamesParms).role=0;
    }
    break;

  case 200:
#line 2255 "cimXmlOps.y"
    {
      memset(&(yyval.xtokReferenceNamesParms), 0, sizeof((yyval.xtokReferenceNamesParms)));
    }
    break;

  case 201:
#line 2259 "cimXmlOps.y"
    {
       (yyval.xtokReferenceNamesParms).role = (yyvsp[-1].xtokValue).value;
       (yyval.xtokReferenceNamesParms).objNameSet = 0 ;
       (yyval.xtokReferenceNamesParms).resultClass=0;
    }
    break;

  case 202:
#line 2265 "cimXmlOps.y"
    {
      memset(&(yyval.xtokReferenceNamesParms), 0, sizeof((yyval.xtokReferenceNamesParms)));
    }
    break;

  case 203:
#line 2277 "cimXmlOps.y"
    {
        (yyval.xtokNamedInstance).path=(yyvsp[-2].xtokInstanceName);
	(yyval.xtokNamedInstance).instance=(yyvsp[-1].xtokInstance);
    }
    break;

  case 204:
#line 2290 "cimXmlOps.y"
    {
       if (((ParserControl*)parm)->Qs) 
          (yyval.xtokClass).qualifiers=((ParserControl*)parm)->qualifiers;
       else memset(&(yyval.xtokClass).qualifiers,0,sizeof((yyval.xtokClass).qualifiers));
       if (((ParserControl*)parm)->Ps) 
          (yyval.xtokClass).properties=((ParserControl*)parm)->properties;
       else memset(&(yyval.xtokClass).properties,0,sizeof((yyval.xtokClass).properties));
       if (((ParserControl*)parm)->Ms) 
          (yyval.xtokClass).methods=((ParserControl*)parm)->methods;
       else memset(&(yyval.xtokClass).methods,0,sizeof((yyval.xtokClass).methods));
    }
    break;

  case 205:
#line 2304 "cimXmlOps.y"
    {;}
    break;

  case 206:
#line 2306 "cimXmlOps.y"
    {
       ((ParserControl*)parm)->Qs++;
       addQualifier(&(((ParserControl*)parm)->qualifiers),&(yyvsp[0].xtokQualifier));
    }
    break;

  case 207:
#line 2310 "cimXmlOps.y"
    {
       ((ParserControl*)parm)->Ps++;
       addProperty(&(((ParserControl*)parm)->properties),&(yyvsp[0].xtokProperty));
    }
    break;

  case 208:
#line 2314 "cimXmlOps.y"
    {
        ((ParserControl*)parm)->Ms++;
        addMethod(&(((ParserControl*)parm)->methods),&(yyvsp[0].xtokMethod));
    }
    break;

  case 209:
#line 2322 "cimXmlOps.y"
    {
       if (((ParserControl*)parm)->MQs) 
          (yyval.xtokMethod).qualifiers=(yyvsp[-1].xtokMethodData).qualifiers;
       else memset(&(yyval.xtokMethod).qualifiers,0,sizeof((yyval.xtokMethod).qualifiers));
       if (((ParserControl*)parm)->MPs) 
          (yyval.xtokMethod).params=(yyvsp[-1].xtokMethodData).params;
       else memset(&(yyval.xtokMethod).params,0,sizeof((yyval.xtokMethod).params));
       ((ParserControl*)parm)->MQs=0; 
       ((ParserControl*)parm)->MPs=0; 
       ((ParserControl*)parm)->MPQs=0; 
    }
    break;

  case 210:
#line 2336 "cimXmlOps.y"
    {;}
    break;

  case 211:
#line 2338 "cimXmlOps.y"
    {
       if (((ParserControl*)parm)->MQs==0) 
          memset(&(yyval.xtokMethodData).qualifiers,0,sizeof((yyval.xtokMethodData).qualifiers));
       ((ParserControl*)parm)->MQs++;
       addQualifier(&((yyval.xtokMethodData).qualifiers),&(yyvsp[0].xtokQualifier));
    }
    break;

  case 212:
#line 2345 "cimXmlOps.y"
    {
       if (((ParserControl*)parm)->MPs==0) 
          memset(&(yyval.xtokMethodData).params,0,sizeof((yyval.xtokMethodData).params));
       ((ParserControl*)parm)->MPs++;
       if (((ParserControl*)parm)->MPQs) 
          (yyvsp[-2].xtokParam).qualifiers=(yyvsp[-1].xtokParam).qualifiers;
       else memset(&(yyvsp[-2].xtokParam).qualifiers,0,sizeof((yyvsp[-2].xtokParam).qualifiers));
       addParam(&((yyval.xtokMethodData).params),&(yyvsp[-2].xtokParam));
       ((ParserControl*)parm)->MPQs=0; 
    }
    break;

  case 213:
#line 2358 "cimXmlOps.y"
    {;}
    break;

  case 214:
#line 2360 "cimXmlOps.y"
    {
       if (((ParserControl*)parm)->MPQs==0) 
          memset(&(yyval.xtokParam).qualifiers,0,sizeof((yyval.xtokParam).qualifiers));
       ((ParserControl*)parm)->MPQs++; 
       addQualifier(&((yyval.xtokParam).qualifiers),&(yyvsp[0].xtokQualifier));
    }
    break;

  case 215:
#line 2375 "cimXmlOps.y"
    {
       if((yyvsp[-1].xtokInstanceData).qualifiers.first)
          (yyval.xtokInstance).qualifiers=(yyvsp[-1].xtokInstanceData).qualifiers;
       else memset(&(yyval.xtokInstance).qualifiers,0,sizeof((yyval.xtokInstance).qualifiers));
       
       if((yyvsp[-1].xtokInstanceData).properties.first)
          (yyval.xtokInstance).properties=(yyvsp[-1].xtokInstanceData).properties;
       else memset(&(yyval.xtokInstance).properties,0,sizeof((yyval.xtokInstance).properties)); 
    }
    break;

  case 216:
#line 2388 "cimXmlOps.y"
    {
       (yyval.xtokInstanceData).properties.last=0;
       (yyval.xtokInstanceData).properties.first=0;
       (yyval.xtokInstanceData).qualifiers.last=0;
       (yyval.xtokInstanceData).qualifiers.first=0;       
    }
    break;

  case 217:
#line 2395 "cimXmlOps.y"
    {
       addQualifier(&((yyval.xtokInstanceData).qualifiers),&(yyvsp[0].xtokQualifier));
    }
    break;

  case 218:
#line 2399 "cimXmlOps.y"
    {
       addProperty(&((yyval.xtokInstanceData).properties),&(yyvsp[0].xtokProperty));
    }
    break;

  case 219:
#line 2409 "cimXmlOps.y"
    {;}
    break;

  case 220:
#line 2411 "cimXmlOps.y"
    {
    	(yyval.xtokQualifierDeclaration).scope = (yyvsp[-2].xtokScope);
    	(yyval.xtokQualifierDeclaration).data = (yyvsp[-1].xtokQualifierDeclarationData);
    }
    break;

  case 221:
#line 2419 "cimXmlOps.y"
    {
    	(yyval.xtokQualifierDeclarationData).value.value = NULL;
    }
    break;

  case 222:
#line 2423 "cimXmlOps.y"
    {
    	(yyval.xtokQualifierDeclarationData).value = (yyvsp[0].xtokValue);
    	(yyval.xtokQualifierDeclarationData).type = 0;
    }
    break;

  case 223:
#line 2428 "cimXmlOps.y"
    {
    	(yyval.xtokQualifierDeclarationData).valueArray=(yyvsp[0].xtokValueArray);
    	(yyval.xtokQualifierDeclarationData).type=CMPI_ARRAY;
    }
    break;

  case 224:
#line 2438 "cimXmlOps.y"
    {;}
    break;

  case 225:
#line 2440 "cimXmlOps.y"
    {
	}
    break;

  case 226:
#line 2450 "cimXmlOps.y"
    {
       (yyvsp[-1].xtokPropertyData).qualifiers=(yyvsp[-2].xtokQualifiers);
       (yyval.xtokProperty).val=(yyvsp[-1].xtokPropertyData);
       
       if((yyval.xtokProperty).val.val.value) {
          if((yyval.xtokProperty).val.val.type == typeValue_Instance)
             (yyval.xtokProperty).valueType = CMPI_instance;
          else if((yyval.xtokProperty).val.val.type == typeValue_Class)
             (yyval.xtokProperty).valueType = CMPI_class;
       }
    }
    break;

  case 227:
#line 2462 "cimXmlOps.y"
    {
       (yyvsp[-1].xtokPropertyData).qualifiers=(yyvsp[-2].xtokQualifiers);
       (yyval.xtokProperty).val=(yyvsp[-1].xtokPropertyData);
    }
    break;

  case 228:
#line 2467 "cimXmlOps.y"
    {
       (yyvsp[-1].xtokPropertyData).qualifiers=(yyvsp[-2].xtokQualifiers);
       (yyval.xtokProperty).val=(yyvsp[-1].xtokPropertyData);
       
       if((yyval.xtokProperty).val.list.values) {
          if((yyval.xtokProperty).val.list.values[0].type == typeValue_Instance)
          	(yyval.xtokProperty).valueType = CMPI_instance | CMPI_ARRAY;
          if((yyval.xtokProperty).val.list.values[0].type == typeValue_Class)
          	(yyval.xtokProperty).valueType = CMPI_class | CMPI_ARRAY;          	
       }     
    }
    break;

  case 229:
#line 2482 "cimXmlOps.y"
    {
      (yyval.xtokQualifiers).first = (yyval.xtokQualifiers).last = NULL;
    }
    break;

  case 230:
#line 2486 "cimXmlOps.y"
    {
       addQualifier(&(yyvsp[-1].xtokQualifiers),&(yyvsp[0].xtokQualifier));
       (yyval.xtokQualifiers) = (yyvsp[-1].xtokQualifiers);
    }
    break;

  case 231:
#line 2494 "cimXmlOps.y"
    {
       (yyval.xtokPropertyData).val.value = NULL;
       (yyval.xtokPropertyData).list.values = NULL;
       (yyval.xtokPropertyData).val.type = 0;
    }
    break;

  case 232:
#line 2500 "cimXmlOps.y"
    {
       (yyval.xtokPropertyData).val=(yyvsp[0].xtokValue);
    }
    break;

  case 233:
#line 2504 "cimXmlOps.y"
    {
       (yyval.xtokPropertyData).ref=(yyvsp[0].xtokValueReference);
    }
    break;

  case 234:
#line 2508 "cimXmlOps.y"
    {
       (yyval.xtokPropertyData).list=(yyvsp[0].xtokValueArray);
    }
    break;

  case 235:
#line 2521 "cimXmlOps.y"
    {
       (yyval.xtokQualifier).value=(yyvsp[-1].xtokValue);
    }
    break;

  case 236:
#line 2525 "cimXmlOps.y"
    {
       (yyval.xtokQualifier).valueArray=(yyvsp[-1].xtokValueArray);
       (yyval.xtokQualifier).type |= CMPI_ARRAY;
    }
    break;

  case 237:
#line 2539 "cimXmlOps.y"
    {
       (yyval.xtokLocalNameSpacePath)=(yyvsp[-1].xtokNameSpace).cns;
    }
    break;

  case 238:
#line 2546 "cimXmlOps.y"
    {
       (yyval.xtokNameSpace).cns=strdup((yyvsp[-1].xtokNameSpace).ns);
    }
    break;

  case 239:
#line 2550 "cimXmlOps.y"
    {
       int l=strlen((yyvsp[-2].xtokNameSpace).cns)+strlen((yyvsp[-1].xtokNameSpace).ns)+2;
       (yyval.xtokNameSpace).cns=(char*)malloc(l);
       strcpy((yyval.xtokNameSpace).cns,(yyvsp[-2].xtokNameSpace).cns);
       strcat((yyval.xtokNameSpace).cns,"/");
       strcat((yyval.xtokNameSpace).cns,(yyvsp[-1].xtokNameSpace).ns);
       free((yyvsp[-2].xtokNameSpace).cns);
    }
    break;

  case 240:
#line 2563 "cimXmlOps.y"
    {
       (yyval.xtokNameSpacePath).host=(yyvsp[-2].xtokHost);
       (yyval.xtokNameSpacePath).nameSpacePath=(yyvsp[-1].xtokLocalNameSpacePath);
    }
    break;

  case 241:
#line 2571 "cimXmlOps.y"
    {
    }
    break;

  case 242:
#line 2577 "cimXmlOps.y"
    {
       (yyval.xtokInstancePath).path=(yyvsp[-2].xtokNameSpacePath);
       (yyval.xtokInstancePath).instanceName=(yyvsp[-1].xtokInstanceName);
       (yyval.xtokInstancePath).type=1;
    }
    break;

  case 243:
#line 2596 "cimXmlOps.y"
    {
       (yyval.xtokLocalInstancePath).path=(yyvsp[-2].xtokLocalNameSpacePath);
       (yyval.xtokLocalInstancePath).instanceName=(yyvsp[-1].xtokInstanceName);
       (yyval.xtokLocalInstancePath).type=1;
    }
    break;

  case 244:
#line 2605 "cimXmlOps.y"
    {
       (yyval.xtokLocalClassPath).path=(yyvsp[-2].xtokLocalNameSpacePath);
       (yyval.xtokLocalClassPath).className=(yyvsp[-1].className);
       (yyval.xtokLocalClassPath).type=1;
    }
    break;

  case 245:
#line 2618 "cimXmlOps.y"
    {
       (yyval.xtokValue).instance = malloc(sizeof(XtokInstance));
       (yyval.xtokValue).instance = memcpy((yyval.xtokValue).instance, &(yyvsp[-1].xtokInstance), sizeof(XtokInstance));
       (yyval.xtokValue).type=typeValue_Instance;
    }
    break;

  case 246:
#line 2624 "cimXmlOps.y"
    {
       (yyval.xtokValue).instance = malloc(sizeof(XtokInstance));
       (yyval.xtokValue).instance = memcpy((yyval.xtokValue).instance, &(yyvsp[-2].xtokInstance), sizeof(XtokInstance));
       (yyval.xtokValue).type=typeValue_Instance;
    }
    break;

  case 247:
#line 2630 "cimXmlOps.y"
    {
       (yyval.xtokValue).type=typeValue_Class;
    }
    break;

  case 248:
#line 2634 "cimXmlOps.y"
    {
       (yyval.xtokValue).value=(yyvsp[-1].xtokValue).value;
       (yyval.xtokValue).type=typeValue_charP;
    }
    break;

  case 249:
#line 2642 "cimXmlOps.y"
    {
	  (yyval.xtokValueArray).values=(XtokValue*)malloc(sizeof(XtokValue));
	  (yyval.xtokValueArray).next=0;
	}
    break;

  case 250:
#line 2647 "cimXmlOps.y"
    {
	  (yyval.xtokValueArray) = (yyvsp[-1].xtokValueArray);
	  (yyval.xtokValueArray).values[(yyval.xtokValueArray).next].value = NULL;
	  if((yyval.xtokValueArray).next == 0) (yyval.xtokValueArray).next = 1;
	}
    break;

  case 251:
#line 2657 "cimXmlOps.y"
    {
          (yyval.xtokValueArray).next=1;
          (yyval.xtokValueArray).max=64;
          (yyval.xtokValueArray).values=(XtokValue*)malloc(sizeof(XtokValue)*64);
          (yyval.xtokValueArray).values[0]=(yyvsp[0].xtokValue);
        }
    break;

  case 252:
#line 2664 "cimXmlOps.y"
    {
          (yyval.xtokValueArray).values[(yyval.xtokValueArray).next]=(yyvsp[0].xtokValue);
          (yyval.xtokValueArray).next++;
        }
    break;

  case 253:
#line 2672 "cimXmlOps.y"
    {
       (yyval.xtokValueReference).instancePath=(yyvsp[-1].xtokInstancePath);
       (yyval.xtokValueReference).type=typeValRef_InstancePath;
    }
    break;

  case 254:
#line 2677 "cimXmlOps.y"
    {
       (yyval.xtokValueReference).localInstancePath=(yyvsp[-1].xtokLocalInstancePath);
       (yyval.xtokValueReference).type=typeValRef_LocalInstancePath;
    }
    break;

  case 255:
#line 2682 "cimXmlOps.y"
    {
       (yyval.xtokValueReference).instanceName=(yyvsp[-1].xtokInstanceName);
       (yyval.xtokValueReference).type=typeValRef_InstanceName;
    }
    break;

  case 256:
#line 2690 "cimXmlOps.y"
    {
       (yyval.xtokValueRefArray)=(yyvsp[-1].xtokValueRefArray);
    }
    break;

  case 257:
#line 2697 "cimXmlOps.y"
    {
       (yyval.xtokValueRefArray).next=1;
       (yyval.xtokValueRefArray).max=64;
       (yyval.xtokValueRefArray).values=(XtokValueReference*)malloc(sizeof(XtokValueReference)*64);
       (yyval.xtokValueRefArray).values[0]=(yyvsp[0].xtokValueReference);
    }
    break;

  case 258:
#line 2704 "cimXmlOps.y"
    {
       (yyval.xtokValueRefArray).values[(yyval.xtokValueRefArray).next]=(yyvsp[0].xtokValueReference);
       (yyval.xtokValueRefArray).next++;
    }
    break;

  case 259:
#line 2712 "cimXmlOps.y"
    {
    if (strcasecmp((yyvsp[-1].xtokValue).value,"true")==0) (yyval.boolValue)=1;
    if (strcasecmp((yyvsp[-1].xtokValue).value,"false")==0) (yyval.boolValue)=0;
    }
    break;

  case 260:
#line 2720 "cimXmlOps.y"
    {
    }
    break;

  case 261:
#line 2732 "cimXmlOps.y"
    {
       (yyval.xtokInstanceName).className=(yyvsp[-1].xtokInstanceName).className;
       (yyval.xtokInstanceName).bindings.next=0;
       (yyval.xtokInstanceName).bindings.keyBindings=NULL;
    }
    break;

  case 262:
#line 2738 "cimXmlOps.y"
    {
       (yyval.xtokInstanceName).className=(yyvsp[-2].xtokInstanceName).className;
       (yyval.xtokInstanceName).bindings=(yyvsp[-1].xtokKeyBindings);
    }
    break;

  case 263:
#line 2746 "cimXmlOps.y"
    {
       (yyval.xtokKeyBindings).next=1;
       (yyval.xtokKeyBindings).max=16;
       (yyval.xtokKeyBindings).keyBindings=(XtokKeyBinding*)calloc(16,sizeof(XtokKeyBinding));
       (yyval.xtokKeyBindings).keyBindings[0].name=(yyvsp[0].xtokKeyBinding).name;
       (yyval.xtokKeyBindings).keyBindings[0].value=(yyvsp[0].xtokKeyBinding).value;
       (yyval.xtokKeyBindings).keyBindings[0].type=(yyvsp[0].xtokKeyBinding).type;
       (yyval.xtokKeyBindings).keyBindings[0].ref=(yyvsp[0].xtokKeyBinding).ref;
    }
    break;

  case 264:
#line 2756 "cimXmlOps.y"
    {
       (yyval.xtokKeyBindings).keyBindings[(yyval.xtokKeyBindings).next].name=(yyvsp[0].xtokKeyBinding).name;
       (yyval.xtokKeyBindings).keyBindings[(yyval.xtokKeyBindings).next].value=(yyvsp[0].xtokKeyBinding).value;
       (yyval.xtokKeyBindings).keyBindings[(yyval.xtokKeyBindings).next].type=(yyvsp[0].xtokKeyBinding).type;
       (yyval.xtokKeyBindings).keyBindings[(yyval.xtokKeyBindings).next].ref=(yyvsp[0].xtokKeyBinding).ref;
       (yyval.xtokKeyBindings).next++;
    }
    break;

  case 265:
#line 2767 "cimXmlOps.y"
    {
       (yyval.xtokKeyBinding).name=(yyvsp[-3].xtokKeyBinding).name;
       (yyval.xtokKeyBinding).value=(yyvsp[-2].xtokKeyValue).value;
       (yyval.xtokKeyBinding).type=(yyvsp[-2].xtokKeyValue).valueType;
    }
    break;

  case 266:
#line 2773 "cimXmlOps.y"
    {
       (yyval.xtokKeyBinding).name=(yyvsp[-2].xtokKeyBinding).name;
       (yyval.xtokKeyBinding).value=NULL;
       (yyval.xtokKeyBinding).type="ref";
       (yyval.xtokKeyBinding).ref=(yyvsp[-1].xtokValueReference);
    }
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 4706 "cimXmlOps.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 2781 "cimXmlOps.y"


