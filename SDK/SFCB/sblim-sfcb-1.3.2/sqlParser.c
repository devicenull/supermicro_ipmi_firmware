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
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_INTEGER = 258,
     TOK_DOUBLE = 259,
     TOK_STRING = 260,
     TOK_CONCAT = 261,
     TOK_TRUE = 262,
     TOK_FALSE = 263,
     TOK_NULL = 264,
     TOK_TSTAMPDT = 265,
     TOK_TIMEDT = 266,
     TOK_DATEDT = 267,
     TOK_VCHARDT = 268,
     TOK_CHARDT = 269,
     TOK_DECDT = 270,
     TOK_DOUBLEDT = 271,
     TOK_REALDT = 272,
     TOK_BINDT = 273,
     TOK_INTDT = 274,
     TOK_SINTDT = 275,
     TOK_BINTDT = 276,
     TOK_EQ = 277,
     TOK_NE = 278,
     TOK_LT = 279,
     TOK_LE = 280,
     TOK_GT = 281,
     TOK_GE = 282,
     TOK_NOT = 283,
     TOK_OR = 284,
     TOK_AND = 285,
     TOK_IS = 286,
     TOK_ALL = 287,
     TOK_CALL = 288,
     TOK_DISTINCT = 289,
     TOK_AS = 290,
     TOK_IN = 291,
     TOK_LIKE = 292,
     TOK_SOME = 293,
     TOK_ANY = 294,
     TOK_EXISTS = 295,
     TOK_BETWEEN = 296,
     TOK_GROUP = 297,
     TOK_FETCH = 298,
     TOK_ONLY = 299,
     TOK_ASC = 300,
     TOK_DESC = 301,
     TOK_BY = 302,
     TOK_ROW = 303,
     TOK_ORDER = 304,
     TOK_FIRST = 305,
     TOK_HAVING = 306,
     TOK_UNION = 307,
     TOK_EXCEPT = 308,
     TOK_INTERSECT = 309,
     TOK_UNIONALL = 310,
     TOK_EXCEPTALL = 311,
     TOK_INTERSECTALL = 312,
     TOK_JOIN = 313,
     TOK_ON = 314,
     TOK_INNER = 315,
     TOK_LEFT = 316,
     TOK_RIGHT = 317,
     TOK_FULL = 318,
     TOK_WITH = 319,
     TOK_DROP = 320,
     TOK_TABLE = 321,
     TOK_ALTER = 322,
     TOK_CREATE = 323,
     TOK_INSERT = 324,
     TOK_INTO = 325,
     TOK_VALUES = 326,
     TOK_DEFAULT = 327,
     TOK_DELETE = 328,
     TOK_UPDATE = 329,
     TOK_SET = 330,
     TOK_KEY = 331,
     TOK_FOREIGN = 332,
     TOK_UNIQUE = 333,
     TOK_PRIMARY = 334,
     TOK_CONSTRAINT = 335,
     TOK_REFERENCES = 336,
     TOK_CHECK = 337,
     TOK_IDENTIFIER = 338,
     TOK_IDENTIFIER2 = 339,
     TOK_SELECT = 340,
     TOK_WHERE = 341,
     TOK_FROM = 342,
     TOK_UNEXPECTED_CHAR = 343
   };
#endif
/* Tokens.  */
#define TOK_INTEGER 258
#define TOK_DOUBLE 259
#define TOK_STRING 260
#define TOK_CONCAT 261
#define TOK_TRUE 262
#define TOK_FALSE 263
#define TOK_NULL 264
#define TOK_TSTAMPDT 265
#define TOK_TIMEDT 266
#define TOK_DATEDT 267
#define TOK_VCHARDT 268
#define TOK_CHARDT 269
#define TOK_DECDT 270
#define TOK_DOUBLEDT 271
#define TOK_REALDT 272
#define TOK_BINDT 273
#define TOK_INTDT 274
#define TOK_SINTDT 275
#define TOK_BINTDT 276
#define TOK_EQ 277
#define TOK_NE 278
#define TOK_LT 279
#define TOK_LE 280
#define TOK_GT 281
#define TOK_GE 282
#define TOK_NOT 283
#define TOK_OR 284
#define TOK_AND 285
#define TOK_IS 286
#define TOK_ALL 287
#define TOK_CALL 288
#define TOK_DISTINCT 289
#define TOK_AS 290
#define TOK_IN 291
#define TOK_LIKE 292
#define TOK_SOME 293
#define TOK_ANY 294
#define TOK_EXISTS 295
#define TOK_BETWEEN 296
#define TOK_GROUP 297
#define TOK_FETCH 298
#define TOK_ONLY 299
#define TOK_ASC 300
#define TOK_DESC 301
#define TOK_BY 302
#define TOK_ROW 303
#define TOK_ORDER 304
#define TOK_FIRST 305
#define TOK_HAVING 306
#define TOK_UNION 307
#define TOK_EXCEPT 308
#define TOK_INTERSECT 309
#define TOK_UNIONALL 310
#define TOK_EXCEPTALL 311
#define TOK_INTERSECTALL 312
#define TOK_JOIN 313
#define TOK_ON 314
#define TOK_INNER 315
#define TOK_LEFT 316
#define TOK_RIGHT 317
#define TOK_FULL 318
#define TOK_WITH 319
#define TOK_DROP 320
#define TOK_TABLE 321
#define TOK_ALTER 322
#define TOK_CREATE 323
#define TOK_INSERT 324
#define TOK_INTO 325
#define TOK_VALUES 326
#define TOK_DEFAULT 327
#define TOK_DELETE 328
#define TOK_UPDATE 329
#define TOK_SET 330
#define TOK_KEY 331
#define TOK_FOREIGN 332
#define TOK_UNIQUE 333
#define TOK_PRIMARY 334
#define TOK_CONSTRAINT 335
#define TOK_REFERENCES 336
#define TOK_CHECK 337
#define TOK_IDENTIFIER 338
#define TOK_IDENTIFIER2 339
#define TOK_SELECT 340
#define TOK_WHERE 341
#define TOK_FROM 342
#define TOK_UNEXPECTED_CHAR 343




/* Copy the first part of user declarations.  */
#line 10 "sqlParser.y"

	
/*
 * sqlParser.y
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
 * Author:       Sebastian Seyrich <seyrich@de.ibm.com>
 *
 * Description:
 * 
 *
 * 
 *
*/

#include <stdio.h>
#include <errno.h>
#include "queryOperation.h"
#include "sqlStatement.h"
#include "utilft.h"

#define YYPARSE_PARAM parm 
#define YYLEX_PARAM parm
#define YYERROR_VERBOSE 1
 
extern int yylex();
extern void sfcQueryErr(char*,char*,char*);
extern void yyError(char*);
extern void yyerror(char*);
extern UtilList *newList();

//extern void setMeta(char *meta);//durch Memberfunction ersetzen

#define sfcQueryerror sfcQueryError
#define yyErr sfcQueryErr

#define RS ((SqlStatement*)parm)->rs 
#define STM ((SqlStatement*)parm)

static UtilList* columnList = NULL;
static UtilList* allColumnsList = NULL;
static UtilList* allColumnsListIns = NULL;
static UtilList* obList = NULL;
int clempty = 1;
UtilList* aliasList = NULL;
UtilList* tableReferenceList = NULL;
UtilList* eList = NULL;
UtilList* whereList = NULL;
UtilList* assignmentList = NULL;
static char* semanticError(char* s1, char* s2, char* s3);
static UtilList* cloneAL();



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 81 "sqlParser.y"
typedef union YYSTYPE {
   int intValue;
   double doubleValue;
   char* strValue;
   void* nodeValue;
   void* stm;
   void* exl;
	char* status;
	void* sigma;
	void* column;
	void* classDef;
	void* list;
	void* fullSelect;
	void* subSelect;
	void* crossJoin;
	void* projection;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 341 "sqlParser.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 353 "sqlParser.c"

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
#define YYFINAL  28
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   251

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  83
/* YYNRULES -- Number of rules. */
#define YYNRULES  175
/* YYNRULES -- Number of states. */
#define YYNSTATES  300

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   343

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    93,     2,     2,    98,
      95,    96,    91,    89,    97,    90,    94,    92,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    99,     2,
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
      85,    86,    87,    88
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    26,    27,    36,    38,    40,    44,    46,
      50,    56,    58,    62,    70,    71,    72,    80,    82,    86,
      90,    92,    96,   100,   104,   108,   111,   113,   115,   116,
     122,   123,   126,   127,   134,   135,   139,   141,   145,   147,
     149,   152,   158,   162,   164,   168,   169,   171,   173,   175,
     179,   186,   193,   195,   199,   203,   204,   205,   207,   209,
     211,   213,   218,   223,   230,   232,   234,   236,   238,   240,
     242,   244,   246,   247,   253,   254,   257,   258,   263,   264,
     266,   270,   273,   275,   277,   279,   281,   283,   285,   293,
     297,   298,   300,   302,   304,   306,   312,   316,   321,   324,
     325,   327,   330,   332,   334,   335,   339,   341,   345,   348,
     355,   356,   359,   361,   362,   364,   366,   368,   370,   371,
     374,   375,   376,   381,   383,   387,   388,   389,   393,   394,
     395,   400,   402,   406,   409,   412,   413,   415,   417,   418,
     424,   428,   429,   434,   439,   440,   444,   448,   452,   454,
     458,   462,   466,   473,   478,   480,   484,   486,   488,   490,
     492,   494,   496,   498,   500,   501
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     101,     0,    -1,    -1,   102,   138,    -1,   137,    -1,   129,
      -1,   130,    -1,   136,    -1,   120,    -1,   117,    -1,   111,
      -1,   103,    -1,     1,    93,    -1,    -1,    33,   159,    94,
     105,   104,    95,   106,    96,    -1,    83,    -1,   107,    -1,
     107,    97,   109,    -1,   108,    -1,   107,    97,   108,    -1,
      98,    83,    22,     5,    98,    -1,   110,    -1,   109,    97,
     110,    -1,    98,    83,    99,     5,    99,     3,    98,    -1,
      -1,    -1,    74,   159,   112,    75,   113,   114,   119,    -1,
     115,    -1,   114,    97,   115,    -1,    83,    22,   116,    -1,
       9,    -1,    83,    89,   180,    -1,    83,    90,   180,    -1,
      83,    91,   180,    -1,    83,    92,   180,    -1,     6,     5,
      -1,     5,    -1,   180,    -1,    -1,    73,    87,   159,   118,
     119,    -1,    -1,    86,   173,    -1,    -1,    69,    70,   159,
     121,   122,   125,    -1,    -1,    95,   123,    96,    -1,   124,
      -1,   123,    97,   124,    -1,    83,    -1,   138,    -1,    71,
     126,    -1,   126,    97,    95,   127,    96,    -1,    95,   127,
      96,    -1,   128,    -1,   127,    97,   128,    -1,    -1,     3,
      -1,     4,    -1,     5,    -1,    65,    66,   159,    -1,    68,
      66,    83,    95,   131,    96,    -1,    68,    66,    83,    37,
     159,   133,    -1,   132,    -1,   131,    97,   132,    -1,    83,
     135,   134,    -1,    -1,    -1,    76,    -1,    10,    -1,    11,
      -1,    12,    -1,    13,    95,     3,    96,    -1,    14,    95,
       3,    96,    -1,    15,    95,     3,    97,     3,    96,    -1,
      16,    -1,    17,    -1,    18,    -1,    19,    -1,    20,    -1,
      21,    -1,    67,    -1,    64,    -1,    -1,   139,   140,   143,
     167,   172,    -1,    -1,   141,   146,    -1,    -1,    95,   142,
     138,    96,    -1,    -1,   144,    -1,   144,   145,   140,    -1,
     145,   140,    -1,    52,    -1,    55,    -1,    53,    -1,    56,
      -1,    54,    -1,    57,    -1,   147,   154,   161,   162,   165,
     167,   172,    -1,    85,   148,   149,    -1,    -1,    32,    -1,
      34,    -1,    91,    -1,   150,    -1,   150,    97,    83,    94,
      83,    -1,    83,    94,    83,    -1,   150,    97,   152,   151,
      -1,   152,   151,    -1,    -1,    83,    -1,    35,    83,    -1,
     153,    -1,    83,    -1,    -1,    87,   155,   156,    -1,   157,
      -1,   156,    97,   157,    -1,   159,   158,    -1,   157,   160,
      58,   157,    59,   176,    -1,    -1,    35,    83,    -1,    83,
      -1,    -1,    60,    -1,    61,    -1,    62,    -1,    63,    -1,
      -1,    86,   173,    -1,    -1,    -1,    42,   163,    47,   164,
      -1,    83,    -1,   164,    97,    83,    -1,    -1,    -1,    51,
     166,   173,    -1,    -1,    -1,    49,    47,   168,   169,    -1,
     170,    -1,   169,    97,   170,    -1,     3,   171,    -1,    83,
     171,    -1,    -1,    45,    -1,    46,    -1,    -1,    43,    50,
       3,    48,    44,    -1,   182,   177,   174,    -1,    -1,    30,
     182,   177,   174,    -1,    29,   182,   177,   174,    -1,    -1,
      30,   182,   178,    -1,    29,   182,   178,    -1,   182,   178,
     175,    -1,   178,    -1,   179,   181,   179,    -1,   179,   181,
     180,    -1,   179,   181,     5,    -1,   179,   182,    41,   180,
      30,   180,    -1,   179,    31,   182,     9,    -1,    83,    -1,
      83,    94,    83,    -1,     3,    -1,     4,    -1,    22,    -1,
      23,    -1,    26,    -1,    24,    -1,    27,    -1,    25,    -1,
      -1,    28,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   259,   259,   259,   266,   267,   272,   273,   274,   275,
     276,   277,   278,   282,   282,   314,   321,   322,   325,   326,
     336,   359,   360,   364,   373,   388,   373,   398,   399,   402,
     478,   479,   480,   481,   482,   483,   484,   485,   489,   489,
     513,   515,   519,   519,   549,   556,   582,   583,   586,   610,
     719,   723,   728,   736,   737,   740,   745,   795,   837,   884,
     891,   901,   913,   914,   917,   937,   941,   942,   946,   947,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
    1011,  1016,  1020,  1020,  1034,  1034,  1066,  1066,  1076,  1080,
    1087,  1088,  1092,  1096,  1100,  1104,  1108,  1112,  1119,  1142,
    1153,  1157,  1161,  1167,  1174,  1182,  1193,  1204,  1213,  1226,
    1226,  1226,  1234,  1239,  1249,  1249,  1295,  1301,  1309,  1377,
    1383,  1384,  1390,  1412,  1413,  1414,  1415,  1416,  1419,  1420,
    1423,  1424,  1424,  1431,  1432,  1435,  1436,  1436,  1438,  1439,
    1439,  1442,  1443,  1446,  1470,  1489,  1490,  1491,  1494,  1495,
    1506,  1513,  1515,  1522,  1530,  1532,  1540,  1549,  1558,  1563,
    1576,  1598,  1630,  1658,  1669,  1685,  1714,  1721,  1737,  1738,
    1739,  1740,  1741,  1742,  1759,  1760
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_INTEGER", "TOK_DOUBLE",
  "TOK_STRING", "TOK_CONCAT", "TOK_TRUE", "TOK_FALSE", "TOK_NULL",
  "TOK_TSTAMPDT", "TOK_TIMEDT", "TOK_DATEDT", "TOK_VCHARDT", "TOK_CHARDT",
  "TOK_DECDT", "TOK_DOUBLEDT", "TOK_REALDT", "TOK_BINDT", "TOK_INTDT",
  "TOK_SINTDT", "TOK_BINTDT", "TOK_EQ", "TOK_NE", "TOK_LT", "TOK_LE",
  "TOK_GT", "TOK_GE", "TOK_NOT", "TOK_OR", "TOK_AND", "TOK_IS", "TOK_ALL",
  "TOK_CALL", "TOK_DISTINCT", "TOK_AS", "TOK_IN", "TOK_LIKE", "TOK_SOME",
  "TOK_ANY", "TOK_EXISTS", "TOK_BETWEEN", "TOK_GROUP", "TOK_FETCH",
  "TOK_ONLY", "TOK_ASC", "TOK_DESC", "TOK_BY", "TOK_ROW", "TOK_ORDER",
  "TOK_FIRST", "TOK_HAVING", "TOK_UNION", "TOK_EXCEPT", "TOK_INTERSECT",
  "TOK_UNIONALL", "TOK_EXCEPTALL", "TOK_INTERSECTALL", "TOK_JOIN",
  "TOK_ON", "TOK_INNER", "TOK_LEFT", "TOK_RIGHT", "TOK_FULL", "TOK_WITH",
  "TOK_DROP", "TOK_TABLE", "TOK_ALTER", "TOK_CREATE", "TOK_INSERT",
  "TOK_INTO", "TOK_VALUES", "TOK_DEFAULT", "TOK_DELETE", "TOK_UPDATE",
  "TOK_SET", "TOK_KEY", "TOK_FOREIGN", "TOK_UNIQUE", "TOK_PRIMARY",
  "TOK_CONSTRAINT", "TOK_REFERENCES", "TOK_CHECK", "TOK_IDENTIFIER",
  "TOK_IDENTIFIER2", "TOK_SELECT", "TOK_WHERE", "TOK_FROM",
  "TOK_UNEXPECTED_CHAR", "'+'", "'-'", "'*'", "'/'", "'$'", "'.'", "'('",
  "')'", "','", "'''", "':'", "$accept", "start", "@1", "call", "@2",
  "procedureName", "argList", "kList", "kpair", "pList", "ptripel",
  "update", "@3", "@4", "assignmentClause", "assignment",
  "expressionlight", "delete", "@5", "optDelWhere", "insert", "@6",
  "optColName", "colList", "colListIDENTIFIER", "valuesOrSelect",
  "tupelList", "tupel", "val", "dropTable", "createTable", "elementList",
  "element", "copyOptions", "isKey", "dataType", "alterTable", "select",
  "fullSelect", "@7", "fullSelectClause", "@8", "@9",
  "fullSelectOperationList", "fullSelectOperation", "setOperation",
  "subselect", "selectClause", "OPT_ALL_DIST", "rowList", "rowListElement",
  "OPT_AS", "expression", "expressionElement", "fromClause", "@10",
  "tableReferenceList", "tableReference", "correlationClause", "tableName",
  "joinType", "whereClause", "groupByClause", "@11", "groupingExpression",
  "havingClause", "@12", "orderByClause", "@13", "sortKeyList", "sortKey",
  "OPT_ASC_DESC", "fetchFirstClause", "searchCondition", "optCondition",
  "optLimCondition", "joinCondition", "predicate", "limitedPredicate",
  "colIdentifier", "number", "comparison", "optNot", 0
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
     335,   336,   337,   338,   339,   340,   341,   342,   343,    43,
      45,    42,    47,    36,    46,    40,    41,    44,    39,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,   100,   102,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   104,   103,   105,   106,   106,   107,   107,
     108,   109,   109,   110,   112,   113,   111,   114,   114,   115,
     116,   116,   116,   116,   116,   116,   116,   116,   118,   117,
     119,   119,   121,   120,   122,   122,   123,   123,   124,   125,
     125,   126,   126,   127,   127,   128,   128,   128,   128,   129,
     130,   130,   131,   131,   132,   133,   134,   134,   135,   135,
     135,   135,   135,   135,   135,   135,   135,   135,   135,   135,
     136,   137,   139,   138,   141,   140,   142,   140,   143,   143,
     144,   144,   145,   145,   145,   145,   145,   145,   146,   147,
     148,   148,   148,   149,   149,   150,   150,   150,   150,   151,
     151,   151,   152,   153,   155,   154,   156,   156,   157,   157,
     158,   158,   159,   160,   160,   160,   160,   160,   161,   161,
     162,   163,   162,   164,   164,   165,   166,   165,   167,   168,
     167,   169,   169,   170,   170,   171,   171,   171,   172,   172,
     173,   174,   174,   174,   175,   175,   175,   176,   177,   178,
     178,   178,   178,   178,   179,   179,   180,   180,   181,   181,
     181,   181,   181,   181,   182,   182
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     0,     8,     1,     1,     3,     1,     3,
       5,     1,     3,     7,     0,     0,     7,     1,     3,     3,
       1,     3,     3,     3,     3,     2,     1,     1,     0,     5,
       0,     2,     0,     6,     0,     3,     1,     3,     1,     1,
       2,     5,     3,     1,     3,     0,     1,     1,     1,     3,
       6,     6,     1,     3,     3,     0,     0,     1,     1,     1,
       1,     4,     4,     6,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     5,     0,     2,     0,     4,     0,     1,
       3,     2,     1,     1,     1,     1,     1,     1,     7,     3,
       0,     1,     1,     1,     1,     5,     3,     4,     2,     0,
       1,     2,     1,     1,     0,     3,     1,     3,     2,     6,
       0,     2,     1,     0,     1,     1,     1,     1,     0,     2,
       0,     0,     4,     1,     3,     0,     0,     3,     0,     0,
       4,     1,     3,     2,     2,     0,     1,     1,     0,     5,
       3,     0,     4,     4,     0,     3,     3,     3,     1,     3,
       3,     3,     6,     4,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,    81,     0,    80,     0,     0,     0,     0,
       0,    82,    11,    10,     9,     8,     5,     6,     7,     4,
      12,   122,     0,     0,     0,     0,     0,    24,     1,     3,
      84,     0,    59,     0,    42,    38,     0,    86,    88,     0,
      15,    13,     0,     0,    44,    40,    25,    82,    92,    94,
      96,    93,    95,    97,   138,    89,    84,   100,    85,     0,
       0,    65,     0,     0,    62,     0,    82,   174,    39,     0,
       0,     0,   148,    84,    91,   101,   102,     0,   114,   128,
       0,    61,    68,    69,    70,     0,     0,     0,    74,    75,
      76,    77,    78,    79,    66,    60,     0,    48,     0,    46,
       0,    43,    49,   175,    41,     0,     0,    40,    27,    87,
     139,     0,    83,    90,   113,   103,    99,   104,   109,   112,
       0,   174,   130,     0,     0,    16,    18,     0,     0,     0,
      67,    64,    63,    45,     0,    55,    50,   164,   151,   158,
     174,     0,     0,    26,     0,     0,     0,     0,     0,   110,
     108,   115,   116,   120,   129,   131,   135,     0,    14,     0,
       0,     0,     0,    47,    56,    57,    58,     0,    53,     0,
       0,   174,   174,   150,   168,   169,   171,   173,   170,   172,
     174,     0,     0,   166,   167,    36,     0,    30,     0,    29,
      37,    28,   145,   145,   140,   141,     0,   106,   113,   109,
     111,     0,   124,   125,   126,   127,     0,     0,   118,     0,
     136,   138,     0,     0,    19,    17,    21,    71,    72,     0,
      52,    55,    55,   165,     0,     0,     0,   161,   159,   160,
       0,    35,     0,     0,     0,     0,   146,   147,   143,   144,
       0,     0,     0,   107,   117,     0,   121,     0,   174,   148,
       0,     0,     0,     0,    54,     0,   151,   151,   163,     0,
      31,    32,    33,    34,   142,   149,   105,   123,   133,   132,
     137,    98,    20,     0,     0,    22,    73,    51,   153,   152,
       0,   174,     0,     0,     0,   162,   119,     0,   134,     0,
     154,     0,   174,   174,   157,    23,     0,     0,   156,   155
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    10,    11,    12,    60,    41,   124,   125,   126,   215,
     216,    13,    36,    69,   107,   108,   189,    14,    45,    68,
      15,    44,    66,    98,    99,   101,   136,   167,   168,    16,
      17,    63,    64,    81,   131,    94,    18,    19,    29,    30,
      38,    39,    47,    54,    55,    56,    58,    59,    77,   116,
     117,   150,   118,   119,    79,   120,   151,   152,   208,   153,
     206,   122,   156,   209,   269,   211,   248,    72,   144,   194,
     195,   238,   112,   104,   173,   294,   286,   138,   139,   140,
     190,   181,   105
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -264
static const short int yypact[] =
{
       2,   -55,   -33,  -264,    -9,  -264,    -4,    -2,     9,   -33,
      83,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
    -264,  -264,    -5,   -33,    15,   -33,   -33,  -264,  -264,  -264,
     -10,    54,  -264,   -23,  -264,  -264,    52,  -264,    25,    53,
    -264,  -264,   -33,    56,    45,    57,  -264,  -264,  -264,  -264,
    -264,  -264,  -264,  -264,    93,    25,   -10,    27,  -264,    58,
      49,  -264,    92,   -74,  -264,    63,    76,   120,  -264,    66,
      59,   103,   108,   -10,  -264,  -264,  -264,   -62,  -264,    68,
      60,  -264,  -264,  -264,  -264,    61,    62,    64,  -264,  -264,
    -264,  -264,  -264,  -264,    84,  -264,    56,  -264,   -32,  -264,
      67,  -264,  -264,  -264,  -264,    80,   139,   -67,  -264,  -264,
    -264,   114,  -264,  -264,    71,  -264,  -264,    69,   -20,  -264,
     -33,   120,   125,    85,    73,    74,  -264,   167,   169,   170,
    -264,  -264,  -264,  -264,    63,    23,    77,    81,    44,  -264,
      21,     1,    66,  -264,     5,   173,    94,    95,    96,  -264,
    -264,    86,    32,   145,  -264,  -264,   130,   160,  -264,    87,
      88,    90,    91,  -264,  -264,  -264,  -264,     3,  -264,    97,
     104,   120,   120,  -264,  -264,  -264,  -264,  -264,  -264,  -264,
     120,     8,   148,  -264,  -264,  -264,   185,  -264,    30,  -264,
    -264,  -264,    78,    78,    98,  -264,   143,  -264,    99,   -20,
    -264,   -33,  -264,  -264,  -264,  -264,   136,   113,  -264,   150,
    -264,    93,   193,   116,  -264,   105,  -264,  -264,  -264,   197,
    -264,    23,    23,  -264,    80,    80,   192,  -264,  -264,  -264,
     122,  -264,   122,   122,   122,   122,  -264,  -264,  -264,  -264,
       5,   159,   121,  -264,    32,   -33,  -264,   123,   120,   108,
     107,   -13,   109,   112,  -264,    33,    44,    44,  -264,   179,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,    55,  -264,   115,
    -264,  -264,  -264,   205,   128,  -264,  -264,  -264,  -264,  -264,
     122,   120,   131,   117,   118,  -264,  -264,    80,  -264,   210,
     102,   124,   120,   120,  -264,  -264,    80,    80,  -264,  -264
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -264,  -264,  -264,  -264,  -264,  -264,  -264,  -264,    65,  -264,
     -37,  -264,  -264,  -264,  -264,    79,  -264,  -264,  -264,   111,
    -264,  -264,  -264,  -264,    89,  -264,  -264,    -3,    -1,  -264,
    -264,  -264,   129,  -264,  -264,  -264,  -264,  -264,   -30,  -264,
     -36,  -264,  -264,  -264,  -264,   171,  -264,  -264,  -264,  -264,
    -264,    28,    82,  -264,  -264,  -264,  -264,  -185,  -264,    16,
    -264,  -264,  -264,  -264,  -264,  -264,  -264,    17,  -264,  -264,
      -8,    37,   -18,  -120,  -123,  -264,  -264,   -89,  -263,    70,
    -179,  -264,  -140
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -124
static const short int yytable[] =
{
     182,   154,   229,     1,   183,   184,   185,   186,   192,   212,
     187,   183,   184,   227,    42,   148,   244,    70,    22,    67,
      74,   114,    95,    96,   290,    27,   164,   165,   166,   115,
     142,   224,   225,   298,   299,     2,   102,   113,    20,    32,
     226,    34,    35,   174,   175,   176,   177,   178,   179,   103,
      21,   259,   180,   260,   261,   262,   263,    23,    61,    75,
     267,    76,    24,   149,   133,   134,     3,     4,    25,     5,
       6,     7,    43,   171,   172,     8,     9,    48,    49,    50,
      51,    52,    53,    28,   188,    37,   273,    -2,   193,    31,
    -123,   137,   202,   203,   204,   205,    26,    -2,    33,   220,
     221,   285,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    93,   281,   202,   203,   204,   205,   232,
     233,   234,   235,   236,   237,   183,   184,    46,   270,   277,
     221,   292,   293,   278,   279,   256,   257,    40,    57,    62,
      65,   287,    71,    67,    80,    78,    97,   100,   103,   106,
     110,   111,   296,   297,   121,   109,   127,   128,   123,   129,
     130,   141,   135,   137,   145,   146,   147,   155,   157,   158,
     160,   159,   161,   162,   169,   170,   196,   197,   198,   200,
     207,   210,   212,   201,   217,   213,   218,   223,   219,   230,
     231,   241,   222,   242,   245,   240,   246,   247,   250,   251,
     253,   258,   252,   265,   266,   272,   268,   274,   276,   280,
     283,   284,   282,   291,   288,   275,   289,   273,   143,   255,
     254,   191,   295,   163,   214,   132,    73,   243,   249,   199,
     239,   271,   264,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   228
};

static const short int yycheck[] =
{
     140,   121,   181,     1,     3,     4,     5,     6,     3,    22,
       9,     3,     4,     5,    37,    35,   201,    47,     2,    86,
      56,    83,    96,    97,   287,     9,     3,     4,     5,    91,
      97,   171,   172,   296,   297,    33,    66,    73,    93,    23,
     180,    25,    26,    22,    23,    24,    25,    26,    27,    28,
      83,   230,    31,   232,   233,   234,   235,    66,    42,    32,
     245,    34,    66,    83,    96,    97,    64,    65,    70,    67,
      68,    69,    95,    29,    30,    73,    74,    52,    53,    54,
      55,    56,    57,     0,    83,    95,    99,    85,    83,    94,
      58,    83,    60,    61,    62,    63,    87,    95,    83,    96,
      97,   280,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    59,    60,    61,    62,    63,    89,
      90,    91,    92,    45,    46,     3,     4,    75,   248,    96,
      97,    29,    30,   256,   257,   224,   225,    83,    85,    83,
      95,   281,    49,    86,    95,    87,    83,    71,    28,    83,
      47,    43,   292,   293,    86,    96,    95,    95,    98,    95,
      76,    22,    95,    83,    50,    94,    97,    42,    83,    96,
       3,    97,     3,     3,    97,    94,     3,    83,    83,    83,
      35,    51,    22,    97,    96,    98,    96,    83,    97,    41,
       5,    48,    95,    94,    58,    97,    83,    47,     5,    83,
       3,     9,    97,    44,    83,    98,    83,    98,    96,    30,
       5,    83,    97,     3,    83,   252,    99,    99,   107,   222,
     221,   142,    98,   134,   159,    96,    55,   199,   211,   147,
     193,   249,   240,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   181
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,    33,    64,    65,    67,    68,    69,    73,    74,
     101,   102,   103,   111,   117,   120,   129,   130,   136,   137,
      93,    83,   159,    66,    66,    70,    87,   159,     0,   138,
     139,    94,   159,    83,   159,   159,   112,    95,   140,   141,
      83,   105,    37,    95,   121,   118,    75,   142,    52,    53,
      54,    55,    56,    57,   143,   144,   145,    85,   146,   147,
     104,   159,    83,   131,   132,    95,   122,    86,   119,   113,
     138,    49,   167,   145,   140,    32,    34,   148,    87,   154,
      95,   133,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   135,    96,    97,    83,   123,   124,
      71,   125,   138,    28,   173,   182,    83,   114,   115,    96,
      47,    43,   172,   140,    83,    91,   149,   150,   152,   153,
     155,    86,   161,    98,   106,   107,   108,    95,    95,    95,
      76,   134,   132,    96,    97,    95,   126,    83,   177,   178,
     179,    22,    97,   119,   168,    50,    94,    97,    35,    83,
     151,   156,   157,   159,   173,    42,   162,    83,    96,    97,
       3,     3,     3,   124,     3,     4,     5,   127,   128,    97,
      94,    29,    30,   174,    22,    23,    24,    25,    26,    27,
      31,   181,   182,     3,     4,     5,     6,     9,    83,   116,
     180,   115,     3,    83,   169,   170,     3,    83,    83,   152,
      83,    97,    60,    61,    62,    63,   160,    35,   158,   163,
      51,   165,    22,    98,   108,   109,   110,    96,    96,    97,
      96,    97,    95,    83,   182,   182,   182,     5,   179,   180,
      41,     5,    89,    90,    91,    92,    45,    46,   171,   171,
      97,    48,    94,   151,   157,    58,    83,    47,   166,   167,
       5,    83,    97,     3,   128,   127,   177,   177,     9,   180,
     180,   180,   180,   180,   170,    44,    83,   157,    83,   164,
     173,   172,    98,    99,    98,   110,    96,    96,   174,   174,
      30,    59,    97,     5,    83,   180,   176,   182,    83,    99,
     178,     3,    29,    30,   175,    98,   182,   182,   178,   178
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
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
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



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



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
#line 259 "sqlParser.y"
    {	//printf("parsen...\n");
	 }
    break;

  case 3:
#line 261 "sqlParser.y"
    {
  //printf("fullSelect abgearbeitet\n");
		RS->sw->setWarning(RS->sw,"00000","Successful Completion");
		return 0;
	  }
    break;

  case 4:
#line 266 "sqlParser.y"
    {printf("select abgearbeitet\n");return 0;}
    break;

  case 5:
#line 267 "sqlParser.y"
    {
	  //printf("drop abgearbeitet\n");
		RS->sw->setWarning(RS->sw,"00000","Successful Completion");
		return 0;
	}
    break;

  case 6:
#line 272 "sqlParser.y"
    {printf("create abgearbeitet\n");return 0;}
    break;

  case 7:
#line 273 "sqlParser.y"
    {printf("alter abgearbeitet\n");return 0;}
    break;

  case 8:
#line 274 "sqlParser.y"
    {printf("insert abgearbeitet\n");return 0;}
    break;

  case 9:
#line 275 "sqlParser.y"
    {printf("delete abgearbeitet\n");return 0;}
    break;

  case 10:
#line 276 "sqlParser.y"
    {printf("update abgearbeitet\n");return 0;}
    break;

  case 11:
#line 277 "sqlParser.y"
    {printf("call abgearbeitet\n");return 0;}
    break;

  case 12:
#line 278 "sqlParser.y"
    {return 1;}
    break;

  case 13:
#line 282 "sqlParser.y"
    {
	  ClassDef * cd = (ClassDef*)(yyvsp[-2].classDef); 
	  //printf("call abgearbeitet\n");
	  whereList = newList();assignmentList = newList();
	  columnList = cd->fieldNameList;
	  /*	  ExpressionLight * el;
	  el = newExpressionLight("cmd",UNDEF,"ls");
	  el->sqltype = CMPI_string;
	  whereList->ft->append(whereList,el);
	  el = newExpressionLight("out",UNDEF," ");
	  el->sqltype = CMPI_string;
	  whereList->ft->append(whereList,el);
	  assignmentList = newList();
	  el = newExpressionLight("Name",UNDEF,"localhost.localdomain");
	  el->sqltype = CMPI_string;
	  assignmentList->ft->append(assignmentList,el);
	  el = newExpressionLight("CreationClassName",UNDEF,"Linux_OperatingSystem");
	  el->sqltype = CMPI_string;
	  assignmentList->ft->append(assignmentList,el);
	  el = newExpressionLight("CSName",UNDEF,"localhost.localdomain");
	  el->sqltype = CMPI_string;
	  assignmentList->ft->append(assignmentList,el);
	  el = newExpressionLight("CSCreationClassName",UNDEF,"Linux_ComputerSystem");
	  el->sqltype = CMPI_string;
	  assignmentList->ft->append(assignmentList,el);
	  */
	  Call * call = newCall(cd->className,(yyvsp[0].strValue),assignmentList,whereList);
	  STM->addNode(STM,CALL,call);	
	}
    break;

  case 15:
#line 314 "sqlParser.y"
    {
	  //muss validiert werden, sobald die Funktionalität im sfcb implementiert ist
	  (yyval.strValue)=(yyvsp[0].strValue);
	}
    break;

  case 20:
#line 336 "sqlParser.y"
    {
	  ExpressionLight * el;
	  Column* col;
	  int i, found;
	  found = 0;
	  //printf("und los %d\n", CMPI_string);
	  //printf(columnList->ft->);
	  for(i=0,col = (Column*)columnList->ft->getFirst(columnList);col;col = (Column*)columnList->ft->getNext(columnList),i++){
	    if(strcasecmp(col->colName,(yyvsp[-3].strValue))==0){
	      found = 1;
	      //printf("::: %s %s\n",$2,$4);
	      el = newExpressionLight((yyvsp[-3].strValue),UNDEF,(yyvsp[-1].strValue));
	      el->sqltype = col->colSQLType;
	      whereList->ft->append(whereList,el);
	      break;
	    }
	  }
	  if(found==0){
	    yyerror(semanticError("UNDIFINED PARAMETER:\nColumn \"",(yyvsp[-3].strValue),"\" is not part of the Key"));STM->rs->sw->sqlstate = "42P02";YYERROR;
	  }
	 
	}
    break;

  case 23:
#line 364 "sqlParser.y"
    {
	  ExpressionLight * el;
	  el = newExpressionLight((yyvsp[-5].strValue),UNDEF,(yyvsp[-3].strValue));
	  el->sqltype = (yyvsp[-1].intValue);
	  //printf("::: %s %s\n",$2,$4);
	  assignmentList->ft->append(assignmentList,el);
	}
    break;

  case 24:
#line 373 "sqlParser.y"
    {
		whereList = newList(); aliasList = newList();//die bleibt leer, muss aber initialisiert werden, damit searchcondition funktioniert
		allColumnsList = newList();
		assignmentList = newList();
		ClassDef* cd = (ClassDef*)(yyvsp[0].classDef);
		UtilList* ul = cd->fieldNameList;
		int i;
		for(i=0; i<cd->fieldCount; i++){
			Column * col = (Column*)ul->ft->getNext(ul);
			col->colAlias = col->colName;
			col->tableAlias = cd->className;
			allColumnsList->ft->append(allColumnsList,col);	
		}
		//printf("gefunden\n");
		
	}
    break;

  case 25:
#line 388 "sqlParser.y"
    {
			//printf("set fertig...\n");
		}
    break;

  case 26:
#line 390 "sqlParser.y"
    {
		//printf("fertig...\n");
		ClassDef* cd = (ClassDef*)(yyvsp[-5].classDef);
		STM->addNode(STM,UPDATE,newUpdIns(cd->className,allColumnsList,assignmentList,whereList));	
	}
    break;

  case 29:
#line 402 "sqlParser.y"
    {
		//printf("assignment\n");
		ExpressionLight * exl = (ExpressionLight*)(yyvsp[0].exl);
		if(exl->name==NULL)
			exl->name = (yyvsp[-2].strValue);
		Column* col;
		Column* rcol = NULL;
		//printf("assignment2\n");
		for(col = (Column*)allColumnsList->ft->getFirst(allColumnsList);col;col = (Column*)allColumnsList->ft->getNext(allColumnsList)){
			if(strcasecmp(col->colName,(yyvsp[-2].strValue))==0){
				rcol = col;
				//printf("gefunden!!!\n");
			}
			
			if(rcol!=NULL)
				break;
		}

		//printf("assignment3b %p\n",exl);
		ExpressionLight* exl2;
		for(exl2 = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);exl2;exl2 = (ExpressionLight*)assignmentList->ft->getNext(assignmentList)){
			if(strcasecmp(exl->name,exl2->name)==0){
				yyerror(semanticError("DUPLICATE COLUMN:\nCannot change column \'",(yyvsp[-2].strValue),"\' more then once in one statement"));STM->rs->sw->sqlstate = "42701";YYERROR;
			}
			
		}
		//printf("assignment3 %p\n",exl);
		
		if(rcol==NULL){
	  		
			yyerror(semanticError("UNDIFINED COLUMN:\nColumn \'",(yyvsp[-2].strValue),"\' in assigmnet-clause does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}	
		
		if(exl->name!=NULL&&strcasecmp(rcol->colName,exl->name)!=0){
			
			yyerror(semanticError("ERROR IN ASSIGNMENT:\nOperation can only be applied to same column: \"",(yyvsp[0].exl),"\" "));STM->rs->sw->sqlstate = "22005";YYERROR;
		}
		//printf("assignment4\n");
		
		int ni = 0;
	  	switch (rcol->colSQLType) {
		   case CMPI_chars:
		   case CMPI_string:
		   case CMPI_char16:
		   if(exl->op!=CONCAT&&exl->op!=EQ&&exl->op!=ISNULL&&exl->op!=ASS)
		      ni = 1;
		   break;
		   case CMPI_sint64:
		   case CMPI_uint64:
		   case CMPI_sint32:
		   case CMPI_uint32:
		   case CMPI_sint16:
		   case CMPI_uint16:
		   case CMPI_uint8:
		   case CMPI_sint8:
		   case CMPI_real32:
		   case CMPI_real64:
		   if(exl->op!=PLUS&&exl->op!=SUB&&exl->op!=DIV&&exl->op!=MUL&&exl->op!=ISNULL&&exl->op!=ASN)
		      ni = 1;
		   break;
		   default: ni=1; break;
		   
	  	}
	  	//printf("assignment5\n");
		
	  	if(ni==1){
			yyerror(semanticError("ERROR IN ASSIGNMENT:\nWrong type in assigemnment of column \'",col->colName,"\' "));STM->rs->sw->sqlstate = "22005";YYERROR;
	  	}
	  	exl->sqltype = rcol->colSQLType; 
	  	
		assignmentList->ft->append(assignmentList,exl);
		//printf("assignment6\n");
		
	}
    break;

  case 30:
#line 478 "sqlParser.y"
    {(yyval.exl) = newExpressionLight(NULL,ISNULL,NULL);}
    break;

  case 31:
#line 479 "sqlParser.y"
    {(yyval.exl) = newExpressionLight((yyvsp[-2].strValue),PLUS,(yyvsp[0].strValue));}
    break;

  case 32:
#line 480 "sqlParser.y"
    {(yyval.exl) = newExpressionLight((yyvsp[-2].strValue),SUB,(yyvsp[0].strValue));}
    break;

  case 33:
#line 481 "sqlParser.y"
    {(yyval.exl) = newExpressionLight((yyvsp[-2].strValue),MUL,(yyvsp[0].strValue));}
    break;

  case 34:
#line 482 "sqlParser.y"
    {(yyval.exl) = newExpressionLight((yyvsp[-2].strValue),DIV,(yyvsp[0].strValue));}
    break;

  case 35:
#line 483 "sqlParser.y"
    {(yyval.exl) = newExpressionLight((yyvsp[-1].intValue),CONCAT,(yyvsp[0].strValue));}
    break;

  case 36:
#line 484 "sqlParser.y"
    {(yyval.exl) = newExpressionLight(NULL,ASS,(yyvsp[0].strValue));}
    break;

  case 37:
#line 485 "sqlParser.y"
    {(yyval.exl) = newExpressionLight(NULL,ASN,(yyvsp[0].strValue));}
    break;

  case 38:
#line 489 "sqlParser.y"
    {
		whereList = newList(); aliasList = newList();//die bleibt leer, muss aber initialisiert werden, damit searchcondition funktioniert
		allColumnsList = newList();
		assignmentList = newList();
		ClassDef* cd = (ClassDef*)(yyvsp[0].classDef);
		UtilList* ul = cd->fieldNameList;
		int i;
		for(i=0; i<cd->fieldCount; i++){
			Column * col = (Column*)ul->ft->getNext(ul);
			col->colAlias = col->colName;
			col->tableAlias = cd->className;
			allColumnsList->ft->append(allColumnsList,col);	
		}
		//printf("gefunden\n");
		
	}
    break;

  case 39:
#line 504 "sqlParser.y"
    {
		//printf("fertig... \n");
		ClassDef* cd = (ClassDef*)(yyvsp[-2].classDef);
		//printf("fertig... %s\n",cd->className);
		
		STM->addNode(STM,DELETE,newUpdIns(cd->className,allColumnsList,NULL,whereList));	
	}
    break;

  case 42:
#line 519 "sqlParser.y"
    {
		//printf("### 1\n");
		allColumnsListIns = newList();
		assignmentList = newList();
		ClassDef* cd = (ClassDef*)(yyvsp[0].classDef);
		int i;
		Column *colB;
		UtilList* ul = cd->fieldNameList;
		//printf("### 2\n");
		for(i=0; i<cd->fieldCount; i++){
			//printf("### 2a %d\n",i);
			colB = (Column*)ul->ft->getNext(ul);
			//printf("### 2b %d\n",i);
			Column *col = newColumn(cd->className,cd->className,colB->colName,colB->colName,colB->colSQLType,colB->isKey);
			//printf("### 2c %d\n",i);
			allColumnsListIns->ft->append(allColumnsListIns,col);
			//printf("### 2d %d\n",i);
			
		}	
		//printf("### 3\n");
		STM->addNode(STM,INSERT,newInsert(cd->className));
		
	}
    break;

  case 43:
#line 542 "sqlParser.y"
    {
				
	}
    break;

  case 44:
#line 549 "sqlParser.y"
    {
		Column* col;
		for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
			assignmentList->ft->append(assignmentList,newExpressionLight(col->colName,UNDEF,NULL));		
			
		}
	}
    break;

  case 45:
#line 556 "sqlParser.y"
    {
		//teste, ob alle Keyattribute vorkommen!!!
		//int i;
		Column* col;
		for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
		//	printf("### %s %d\n",col->colName,col->isKey);
			if(col->isKey==KEY){
				//suche korrespondierende Spalte in AssigenmentList	
				ExpressionLight * el;
				int found = 0;
				for(el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);el;el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList)){
					if(strcasecmp(el->name,col->colName)==0){
						found = 1;
						break;	
					}		
				}
				if(found==0){
					yyerror(semanticError("NOT NULL VIOLATION:\nColumn \'",col->colName,"\' is part of the key of the table. You have to enter values for all key-properties in Column-Name-clause"));STM->rs->sw->sqlstate = "23502";YYERROR;
				}
			//	printf(" %d %s \n ",col->isKey,col->colName);
			}
		}
	}
    break;

  case 48:
#line 586 "sqlParser.y"
    {
		Column* rcol = NULL;
		Column* col;
		for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
			if(strcasecmp(col->colName,(yyvsp[0].strValue))==0){
				rcol = col;
			}
			if(rcol!=NULL)
				break;
		}
		
		if(rcol==NULL){
			yyerror(semanticError("UNDEFINED COLUMN:\nColumn \'",(yyvsp[0].strValue),"\' in colList-clause of INSERT-Statement does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}
	  	ExpressionLight * el;
		for(el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);el;el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList)){
			if(strcasecmp(el->name,(yyvsp[0].strValue))==0){
				yyerror(semanticError("DUPLICATED COLUMN:\nColumn \'",(yyvsp[0].strValue),"\' indicated twice"));STM->rs->sw->sqlstate = "42701";YYERROR;
			}
		}
		assignmentList->ft->append(assignmentList,newExpressionLight((yyvsp[0].strValue),UNDEF,NULL));	
	}
    break;

  case 49:
#line 610 "sqlParser.y"
    {
		FullSelect * fs = ((Insert*)(STM->stmt))->fs;
		if(fs->pi->col->ft->size(fs->pi->col)>assignmentList->ft->size(assignmentList)){
			yyerror(semanticError("SYNTAX ERROR:\nNumber of columns in Fullselect-clause ist greater than Number of columns entered in columnList-clause"," "," "));STM->rs->sw->sqlstate = "42601";YYERROR;	
		}
		//types der columns vergleichen
		ExpressionLight * el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);
		Column* col = (Column*)fs->pi->col->ft->getFirst(fs->pi->col);
		int i;
		for(i=0;i<assignmentList->ft->size(assignmentList);i++){//geht über zwei Listen. ist so übersichtlicher
			Column* colA;//MUSS DAS NICHT fs->pi->col sein????
			for(colA = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;colA = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
				if(strcasecmp(colA->colName,el->name)==0){
					break;
				}	
			}
			
			int ni = 0;
		  	switch (colA->colSQLType) {
			   case CMPI_chars:
			   case CMPI_string:
			   case CMPI_char16:
			   if(col->colSQLType!=CMPI_chars&&col->colSQLType!=CMPI_string&&col->colSQLType!=CMPI_char16)
			      ni = 1;
			   break;
			   case CMPI_sint64:
			   case CMPI_uint64:
			   case CMPI_sint32:
			   case CMPI_uint32:
			   case CMPI_sint16:
			   case CMPI_uint16:
			   case CMPI_uint8:
			   case CMPI_sint8:
			   case CMPI_real32:
			   case CMPI_real64:
			   if(col->colSQLType!=CMPI_sint64
			   &&col->colSQLType!=CMPI_uint64
			   &&col->colSQLType!=CMPI_sint32
			   &&col->colSQLType!=CMPI_uint32
			   &&col->colSQLType!=CMPI_sint16
			   &&col->colSQLType!=CMPI_uint16
			   &&col->colSQLType!=CMPI_uint8
			   &&col->colSQLType!=CMPI_sint8
			   &&col->colSQLType!=CMPI_real32
			   &&col->colSQLType!=CMPI_real64)
			    	  ni = 1;
		   	   	break;
		   	  	default: ni=1; break;
		   	
	  		}
	  		if(ni==1){
	  		
				yyerror(semanticError("DATATYPE MISMATCH:\nWrong type in assigemnment of column \'",col->colName,"\' "));STM->rs->sw->sqlstate = "42804";
	  		}
			
			col = (Column*)fs->pi->col->ft->getNext(fs->pi->col);
			el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList);
			//jetzt calculate
			
			//jedes Tupel  in Assigmentlist in  packen
		}
		//das macht hier schon Sinn, denn dann ist es für das calculate des insert unerheblich, woher die Assigenments stammen. Das heißt, dass insert-fehler (Tupel mit gleichen Key-Values bereits vorhanden) erst im calculate gefunden und behandelt werden
		AvlTree* t = fs->calculate(fs);
		if(t==NULL){
			yyerror(semanticError("INTERNAL ERROR."," "," "));STM->rs->sw->sqlstate ="xx000";YYERROR;	
		}

    	int tos;
    	AvlNode *p, **stack;
		//iterativ durch den Baum
    	if(t->root) {
        	stack = t->stack;
			stack[0] = t->root;
        	tos = 1;
        
			while(tos) {
		
			    p = stack[--tos];
			    if(p->left) {
					stack[tos++] = p->left;
		    	}
	    		if(p->right) {
					stack[tos++] = p->right;
	    		}
	    		Row * r = (Row*) p->item;	
	    		if(r->doublette>1){
	    			yyerror(semanticError("UNIQUE VIOLATION:\nThere are doublicates in the resultset of the FULSELECT-clause. Use \"DISTINCT\" to avoid this."," "," "));STM->rs->sw->sqlstate = "23505";YYERROR;
	    		}
	    		int k;
	    		Column* col = (Column*)fs->pi->col->ft->getFirst(fs->pi->col);
	    		//über alle Spalten des Fullselects
	    		for(k=0;k<r->size;k++){
	    			//finde assignment:
	    			ExpressionLight * el;
	    			for(el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);el;col = (Column*)fs->pi->col->ft->getNext(fs->pi->col)){
	    				if(strcasecmp(el->name,col->colName)==0){
	    					el->value = r->row[k];
	    					el->sqltype = col->colSQLType; 
	    					break;	
	    				}
	    				el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList);	
	    			}
	    			STM->addNode(STM,INSTUPEL,newUpdIns(NULL,allColumnsListIns,assignmentList,NULL));
					assignmentList = cloneAL();	
	    			
	    		}
			}
    	}//iterativ ends		 
	}
    break;

  case 51:
#line 723 "sqlParser.y"
    {
		STM->addNode(STM,INSTUPEL,newUpdIns(NULL,allColumnsListIns,assignmentList,NULL));
		//printf("%p\n",assignmentList);
		assignmentList = cloneAL();	
	}
    break;

  case 52:
#line 728 "sqlParser.y"
    {
		STM->addNode(STM,INSTUPEL,newUpdIns(NULL,allColumnsListIns,assignmentList,NULL));
		//printf("%p\n",assignmentList);
		assignmentList = cloneAL();	
	}
    break;

  case 56:
#line 745 "sqlParser.y"
    {
		ExpressionLight * el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);
		Column* col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);
		int i;
		for(i=0;i<assignmentList->ft->size(assignmentList);i++){
			if(el->value==NULL){
				break;
			}
			col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns);
			el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList);
		}
		if(el==NULL){
			yyerror(semanticError("SYNTAX ERROR:\nTried to assign more columns than entered in the columnList-clause"," "," "));STM->rs->sw->sqlstate = "42601";YYERROR;	
		}
		if(assignmentList->ft->size(assignmentList)!=allColumnsListIns->ft->size(allColumnsListIns)){//d.h. columnList war nicht leer
			//suche richtige Column
			
			for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
				if(strcasecmp(col->colName,el->name)==0){
					break;
				}	
			}
		}
		int ni = 0;
		switch (col->colSQLType) {
		   case CMPI_sint64:
		   case CMPI_uint64:
		   case CMPI_sint32:
		   case CMPI_uint32:
		   case CMPI_sint16:
		   case CMPI_uint16:
		   case CMPI_uint8:
		   case CMPI_sint8:
		   case CMPI_real32:
		   case CMPI_real64:
		   		ni = 0;
		   		break;
		   default: ni=1; break;
	  	}
	  	if(ni==1){
			yyerror(semanticError("DATATYPE MISMATCH:\nWrong type in assigemnment of column \'",col->colName,"\' "));STM->rs->sw->sqlstate = "42804";YYERROR;
	  	}
	  	char buf[100];
		int ii = sprintf(buf,"%d",(yyvsp[0].intValue));
		char *tmp = (char*) calloc(1,ii+1);
		strncpy(tmp,buf,ii);
	  	el->value = tmp;
	  	el->sqltype = col->colSQLType; 
	  	//printf("--2- %s\n",el->value);
	}
    break;

  case 57:
#line 795 "sqlParser.y"
    {
		ExpressionLight * el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);
		Column* col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);
		int i;
		for(i=0;i<assignmentList->ft->size(assignmentList);i++){
			if(el->value==NULL){
				break;
			}
			col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns);
			el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList);
		}
		if(el==NULL){
			yyerror(semanticError("SYNTAX ERROR:\n Tried to assign more columns than entered in the columnList-clause"," "," "));STM->rs->sw->sqlstate = "42601";YYERROR;	
		}
		if(assignmentList->ft->size(assignmentList)!=allColumnsListIns->ft->size(allColumnsListIns)){//d.h. columnList war nicht leer
			//suche richtige Column
			
			for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
				if(strcasecmp(col->colName,el->name)==0){
					break;
				}
			}
		}
		int ni = 0;
		switch (col->colSQLType) {
		   case CMPI_real32:
		   case CMPI_real64:   
		      ni = 0;
		   break;
		   default: ni=1; break;
		   
	  	}
	  	if(ni==1){
			yyerror(semanticError("DATATYPE MISMATCH:\nWrong type in assigemnment of column \'",col->colName,"\' "));STM->rs->sw->sqlstate = "42804";YYERROR;
	  	}
	  	char buf[100];
		int ii = sprintf(buf,"%f",(yyvsp[0].doubleValue));
		char *tmp = (char*) calloc(1,ii+1);
		strncpy(tmp,buf,ii);
	  	el->sqltype = col->colSQLType; 
	  	el->value = tmp; 
	}
    break;

  case 58:
#line 837 "sqlParser.y"
    {
		//printf(">>> %s\n",$1);
		ExpressionLight * el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);
		Column* col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);
		int i;
		
		for(i=0;i<assignmentList->ft->size(assignmentList);i++){
			if(el->value==NULL){
				break;
			}
			col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns);
			el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList);
		}
		if(el==NULL){
			yyerror(semanticError("SYNTAX ERROR:\nTried to assign more columns than entered in the columnList-clause"," "," "));STM->rs->sw->sqlstate = "42601";YYERROR;	
		}
		if(assignmentList->ft->size(assignmentList)!=allColumnsListIns->ft->size(allColumnsListIns)){//d.h. columnList war nicht leer
			//suche richtige Column
			
			for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
				if(strcasecmp(col->colName,el->name)==0){
					break;
				}	
			}
		}
		
		int ni = 1;
		switch (col->colSQLType) {
		   	case CMPI_chars:
		   	case CMPI_string:
			case CMPI_char16:
		   		ni = 0;
		   		break;
		  	default: ni=1; break;
		   
	  	}
	  	
	  	//printf("%d %d\n",col->colSQLType,CMPI_string);
	  	if(ni==1){
			yyerror(semanticError("DATATYPE MISMATCH:\nWrong type in assigemnment of column \'",col->colName,"\' "));STM->rs->sw->sqlstate = "42804";YYERROR;
	  	}
	  	el->sqltype = col->colSQLType; 
	  	el->value = (yyvsp[0].strValue); 
	}
    break;

  case 59:
#line 884 "sqlParser.y"
    {
		ClassDef* cd = (ClassDef*)(yyvsp[0].classDef);
		STM->addNode(STM,DROP,cd->className);	
	}
    break;

  case 60:
#line 891 "sqlParser.y"
    {
		//printf("suche: %s\n",$3);
		ClassDef * cd = getClassDef(STM->db,(yyvsp[-3].strValue));
		if(cd!=NULL){	
			yyerror(semanticError("DUPLICATE TABLE:\nTable \"",(yyvsp[-3].strValue),"\" does alrady exist"));STM->rs->sw->sqlstate = "42P07";YYERROR;
		}
		
		STM->addNode(STM,CREATE,newClassDef(0,(yyvsp[-3].strValue),eList,0,NULL));
			  	
	  }
    break;

  case 61:
#line 901 "sqlParser.y"
    {
		//printf("suche: %s\n",$3);
		ClassDef* cd1 = (ClassDef*)(yyvsp[-1].classDef);
		ClassDef * cd = getClassDef(STM->db,(yyvsp[-3].strValue));
		if(cd!=NULL){	
			yyerror(semanticError("DUPLICATE TABLE:\nTable \"",(yyvsp[-3].strValue),"\" does alrady exist"));STM->rs->sw->sqlstate = "42P07";YYERROR;
		}
		STM->addNode(STM,CREATE,newClassDef(0,(yyvsp[-3].strValue),NULL,0,cd1->className));
	  }
    break;

  case 64:
#line 917 "sqlParser.y"
    {//columnOptions {
		if(eList==NULL)
			eList=newList();
			//printf("DT: %d %d\n",$2,CMPI_real32);
		eList->ft->append(eList,newColumn(NULL,NULL, (yyvsp[-2].strValue), NULL, (yyvsp[-1].intValue),(yyvsp[0].intValue)));//CMPI_real32
		//BUG!!! es tut offenbar nur CMPI_real32!!!
	}
    break;

  case 66:
#line 941 "sqlParser.y"
    {(yyval.intValue)=NKEY;}
    break;

  case 67:
#line 942 "sqlParser.y"
    {(yyval.intValue)=KEY;}
    break;

  case 68:
#line 946 "sqlParser.y"
    {(yyval.intValue) = CMPI_string;}
    break;

  case 69:
#line 947 "sqlParser.y"
    {(yyval.intValue) = CMPI_string;}
    break;

  case 70:
#line 948 "sqlParser.y"
    {(yyval.intValue) = CMPI_dateTime;}
    break;

  case 71:
#line 949 "sqlParser.y"
    {(yyval.intValue) = CMPI_chars;}
    break;

  case 72:
#line 950 "sqlParser.y"
    {(yyval.intValue) = CMPI_chars;}
    break;

  case 73:
#line 951 "sqlParser.y"
    {(yyval.intValue) = CMPI_real64;}
    break;

  case 74:
#line 952 "sqlParser.y"
    {(yyval.intValue) = CMPI_real32;}
    break;

  case 75:
#line 953 "sqlParser.y"
    {(yyval.intValue) = CMPI_real32;}
    break;

  case 76:
#line 954 "sqlParser.y"
    {(yyval.intValue) = CMPI_string;}
    break;

  case 77:
#line 955 "sqlParser.y"
    {(yyval.intValue) = CMPI_sint32;}
    break;

  case 78:
#line 956 "sqlParser.y"
    {(yyval.intValue) = CMPI_sint16;}
    break;

  case 79:
#line 957 "sqlParser.y"
    {(yyval.intValue) = CMPI_sint64;}
    break;

  case 80:
#line 1011 "sqlParser.y"
    {yyerror("FEATURE NOT SUPPORTED: ALTER");STM->rs->sw->sqlstate = "0A000";YYERROR;}
    break;

  case 81:
#line 1016 "sqlParser.y"
    {yyerror("FEATURE NOT SUPPORTED: WITH");STM->rs->sw->sqlstate = "0A000";YYERROR;}
    break;

  case 82:
#line 1020 "sqlParser.y"
    {
		FullSelect* fs = newFullSelect();
		STM->addNode(STM,INITFUL,fs);
	}
    break;

  case 83:
#line 1027 "sqlParser.y"
    {
		STM->addNode(STM,SIGMA,newSelection(obList,(int)(yyvsp[0].intValue)));//$3 statt NULL
		//printf("fetchfirs:  %d\n",(int)$5);
		obList=NULL;	
	}
    break;

  case 84:
#line 1034 "sqlParser.y"
    {
	columnList = newList();
	if(allColumnsList==NULL)
		allColumnsList = newList();
	aliasList = newList();
	whereList = newList();}
    break;

  case 85:
#line 1040 "sqlParser.y"
    {
		//printf("subselect im fullSelect\n");
		
		//fs->setA = (SubSelect*)$1;
		//fs->typeA = SUB;
		
		int ret = STM->addNode(STM,SUB,(SubSelect*)(yyvsp[0].subSelect));
		
		if(ret!=OK){
			int sw = (ret/1000)*1000;
			
			switch(sw){
			case CCOUNTERR:
				
				yyerror("SYNTAX ERROR: The number of elements of two relations is different");STM->rs->sw->sqlstate = "42601";YYERROR;
				break;
			case CDEFERR:
				
				yyerror("DATATYPE MISMATCH");STM->rs->sw->sqlstate = "42804";YYERROR;
				break;
			default:;	
			}
		}
		
		
	}
    break;

  case 86:
#line 1066 "sqlParser.y"
    {
		//printf("(fullSelect) im fullSelect\n");
		FullSelect* fs = newFullSelect();
		//STM->addNode(STM,INITFUL,fs);
		STM->addNode(STM,FUL,fs);
	}
    break;

  case 88:
#line 1076 "sqlParser.y"
    {
		//printf("EMPTY: fullSelectoperation\n");
		STM->addNode(STM,EMPTY,NULL);
	}
    break;

  case 89:
#line 1080 "sqlParser.y"
    {
		//printf("fullSelectoperation\n");
		//nothing to do	
	}
    break;

  case 92:
#line 1092 "sqlParser.y"
    {
		//printf("UNION\n");
		STM->addNode(STM,UNION,NULL);	
	}
    break;

  case 93:
#line 1096 "sqlParser.y"
    {
		//printf("UNION_ALL\n");
		STM->addNode(STM,UNIONALL,NULL);	
	}
    break;

  case 94:
#line 1100 "sqlParser.y"
    {
		//printf("EXCEPT\n");
		STM->addNode(STM,EXCEPT,NULL);	
	}
    break;

  case 95:
#line 1104 "sqlParser.y"
    {
		//printf("EXCEPTALL\n");
		STM->addNode(STM,EXCEPTALL,NULL);	
	}
    break;

  case 96:
#line 1108 "sqlParser.y"
    {
		//printf("INTERSECT\n");
		STM->addNode(STM,INTERSECT,NULL);	
	}
    break;

  case 97:
#line 1112 "sqlParser.y"
    {
		//printf("INTERSECTALL\n");
		STM->addNode(STM,INTERSECTALL,NULL);	
	}
    break;

  case 98:
#line 1124 "sqlParser.y"
    {
		//printf("subselect erfolgreich fertig!!!\n");
		
		Projection* prj = (Projection*)(yyvsp[-6].projection);
		
		Selection* sigma = newSelection(obList,(yyvsp[0].intValue));
		SubSelect* sbs = (SubSelect*)(yyvsp[-5].subSelect);
		sbs->sigma = sigma;
		sbs->pi = prj;
		
		sbs->addWhereList(sbs,whereList);
		(yyval.subSelect) = sbs;
		obList=NULL;	
		
	}
    break;

  case 99:
#line 1147 "sqlParser.y"
    {
		(yyval.projection) = newProjection((yyvsp[-1].intValue),(UtilList*)(yyvsp[0].list));
		//printf("rowList abgearbeitet\n");
	}
    break;

  case 100:
#line 1153 "sqlParser.y"
    {
		//printf("ALL_DIST: empty\n");
		(yyval.intValue) = ALL;	
	}
    break;

  case 101:
#line 1157 "sqlParser.y"
    {
		//printf("ALL_DIST: ALL\n");
		(yyval.intValue) = ALL;	
	}
    break;

  case 102:
#line 1161 "sqlParser.y"
    {
		//printf("ALL_DIST: DISTINCT\n");
		(yyval.intValue) = DIST;
	}
    break;

  case 103:
#line 1168 "sqlParser.y"
    {
		(yyval.list)  = columnList = newList();
		clempty = 1;
		aliasList = newList();
		//printf("STERN %s\n",columnList);
	}
    break;

  case 104:
#line 1174 "sqlParser.y"
    {
		(yyval.list) = columnList;
		clempty = 0;
	}
    break;

  case 105:
#line 1182 "sqlParser.y"
    {
		  	 
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn((yyvsp[-2].strValue),(yyvsp[-2].strValue),(yyvsp[0].strValue),NULL,UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
		}
    break;

  case 106:
#line 1193 "sqlParser.y"
    {
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn((yyvsp[-2].strValue),(yyvsp[-2].strValue),(yyvsp[0].strValue),NULL,UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
			if((yyvsp[-2].strValue)!=NULL)
				aliasList->ft->append(aliasList,(yyvsp[-2].strValue));	
		}
    break;

  case 107:
#line 1204 "sqlParser.y"
    {
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn(NULL,NULL,(yyvsp[-1].strValue),(yyvsp[0].strValue),UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
		}
    break;

  case 108:
#line 1213 "sqlParser.y"
    {
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn(NULL,NULL,(yyvsp[-1].strValue),(yyvsp[0].strValue),UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
		}
    break;

  case 109:
#line 1226 "sqlParser.y"
    {(yyval.strValue)=" ";}
    break;

  case 110:
#line 1226 "sqlParser.y"
    {(yyval.strValue)=(yyvsp[0].strValue);}
    break;

  case 111:
#line 1226 "sqlParser.y"
    {(yyval.strValue)=(yyvsp[0].strValue);}
    break;

  case 114:
#line 1249 "sqlParser.y"
    {
		//printf("FROM gefunden\n");
		}
    break;

  case 115:
#line 1251 "sqlParser.y"
    {
		Column *col;
		
		if(!(aliasList->ft->isEmpty(aliasList))){
			yyerror(semanticError("AMBIGUOUS ALIAS:\nTable alias name \"",(char*)aliasList->ft->getFirst(aliasList),"\" is undifined"));STM->rs->sw->sqlstate = "42P09";YYERROR;
		}
		
		for(col = (Column*)columnList->ft->getFirst(columnList); col; col = (Column*)columnList->ft->getNext(columnList)){
			if(col->colSQLType == UNDEF){		
				yyerror(semanticError("UNDIFINED COLUMN:\nColumn \"",col->colName,"\" does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
			}
			
		}
		SubSelect* s = (SubSelect*)tableReferenceList->ft->getFirst(tableReferenceList);
		//printf("sbuSelect: %p\n",s);
		int i;
		if(tableReferenceList->ft->size(tableReferenceList)>1){
		  SubSelect* sbs = newSubSelect();//printf("newSubselect() from: (mehr als ein Tablename) %p\n",sbs);
			SubSelect* rsbs = sbs;
			for(i=0; i<tableReferenceList->ft->size(tableReferenceList); i++){//das ergibt einen maximal unbalancierten Baum...
				if(sbs->cross->setA == NULL){
					sbs->cross->setA = s;
					sbs->cross->type = INNER;
					s->sigma = newSelection(NULL,UNDEF);
				}
				else{
					sbs->cross->setB = s;
					s->sigma = newSelection(NULL,UNDEF);
					SubSelect* stmp = sbs;//printf("newSubselect() from: (in for-schleife: cross) %p\n",stmp);
					sbs = newSubSelect();
					sbs->cross->setA = stmp;
					stmp->sigma = newSelection(NULL,UNDEF);
				}
				s = (SubSelect*)tableReferenceList->ft->getNext(tableReferenceList);
			}
			
			(yyval.subSelect) = rsbs;
		}
		else 
			(yyval.subSelect) = s; 
		tableReferenceList->ft->clear(tableReferenceList);
	}
    break;

  case 116:
#line 1295 "sqlParser.y"
    {
		if(tableReferenceList==NULL)
			tableReferenceList = newList();
		tableReferenceList->ft->append(tableReferenceList,(yyvsp[0].subSelect));
	}
    break;

  case 117:
#line 1301 "sqlParser.y"
    {
		if(tableReferenceList==NULL)
			tableReferenceList = newList();
		tableReferenceList->ft->append(tableReferenceList,(yyvsp[0].subSelect));	
	}
    break;

  case 118:
#line 1309 "sqlParser.y"
    {
		ClassDef* cd = (ClassDef*)(yyvsp[-1].classDef);
		Column *colA, *colB;
		UtilList* ul = cd->fieldNameList;
		SubSelect* sbs = newSubSelect();//printf("new SubSelect() tableReference: %p\n",sbs);
		//$2 ist Table-Alias-Name
		Projection* prj = newProjection(ALL,newList());
		sbs->pi = prj;
		
		for(colB = (Column*)ul->ft->getFirst(ul); colB; colB = (Column*)ul->ft->getNext(ul)){
			//jedes Subselect braucht die komplette Classendefinition für calculate()
			Column *col = newColumn(cd->className,cd->className,colB->colName,colB->colName,colB->colSQLType,colB->isKey);
			sbs->pi->col->ft->append(sbs->pi->col,col);//die zwei listen scheinen identisch zu sein!!!
			allColumnsList->ft->append(allColumnsList,col);//die zwei listen scheinen identisch zu sein!!!	
			if(clempty){	
				//printf("%s %u %d\n",colB->colName,colB->colSQLType,colB->colSQLType);
				columnList->ft->append(columnList,col);			
			}
			//setMeta("lalala");
			//printf("* wurde eingegeben. %d Spalten wurden eingefügt\n",cd->fNameLength);	
		}
		if(!clempty){
			for(colA = (Column*)columnList->ft->getFirst(columnList); colA; colA = (Column*)columnList->ft->getNext(columnList)){//gefundene Spalten mit classdef vergleichen. 
				//nach abbarbeiten der FROM-Clause miss die Liste leer sein.
				for(colB = (Column*)ul->ft->getFirst(ul); colB; colB = (Column*)ul->ft->getNext(ul)){			
					if(strcasecmp(colB->colName,colA->colName)==0){
						//??? hier stimmts nicht!!!!!!
						//welches verhalten bei gleichem spaltennamen mehrmals????
						
						if(colA->tableName==NULL || (strcasecmp(colA->tableName,(yyvsp[0].strValue))==0 || strcasecmp(colA->tableName,cd->className)==0)){
							//printf("%s %s %s\n",colA->tableName,$2,cd->className);
							
//							if(colA->colSQLType != UNDEF ){ //das tut so noch nicht...
//								yylval.stm = STM;STM->rs->sw->sqlstate = "77777";
//								yyerror(semanticError("Column \"",colA->colName,"\" is part of more than one table "));YYERROR;
//							}		
//								
							colA->tableName = cd->className;
							
							colA->colSQLType = colB->colSQLType;
							
							char *alias;
							for(alias = (char*)aliasList->ft->getFirst(aliasList); alias; alias = (char*)aliasList->ft->getNext(aliasList)){
								if(strcasecmp(alias,(yyvsp[0].strValue))==0)
									aliasList->ft->removeCurrent(aliasList);
								
							}
							if(strcmp(" ",(yyvsp[0].strValue))==0)
								colA->tableAlias = cd->className;
							else
								colA->tableAlias = (yyvsp[0].strValue);
							//printf("spalte gefunden: %s.%s mit typ: %d\n",colA->tableName,colA->colName,colA->colSQLType);
							break;
						}	
					}	
				}
			}
			//printf("§§§!!! %s\n",columnList);	
		}
		
		sbs->setName = (char*)calloc(1,strlen(cd->className)+1);
		sbs->aliasName = (char*)calloc(1,strlen((yyvsp[0].strValue))+1);
		strcpy(sbs->aliasName,(yyvsp[0].strValue));
		strcpy(sbs->setName,cd->className);
		sbs->isSet = 1;
		(yyval.subSelect) = sbs;

	}
    break;

  case 119:
#line 1377 "sqlParser.y"
    {
		(yyval.subSelect) = newSubSelectC((yyvsp[-5].subSelect),(yyvsp[-2].subSelect),(yyvsp[-4].intValue));
		//printf("new SubSelect join %p\n",$$);
	}
    break;

  case 120:
#line 1383 "sqlParser.y"
    {(yyval.strValue) = " ";}
    break;

  case 121:
#line 1385 "sqlParser.y"
    {
  	(yyval.strValue) = (yyvsp[0].strValue);
  }
    break;

  case 122:
#line 1390 "sqlParser.y"
    {
		//printf("suche: %s\n",$1);
		ClassDef * cd = getClassDef(STM->db,(yyvsp[0].strValue));
		if(cd==NULL){	
			yyerror(semanticError("UNDIFINED TABLE:\nTable \"",(yyvsp[0].strValue),"\" does not exist"));STM->rs->sw->sqlstate = "42P01";YYERROR;
		}
		else{
			//printf("Die Klasse gibt es!!!\n");
			(yyval.classDef) = cd;
		}
	}
    break;

  case 123:
#line 1412 "sqlParser.y"
    {(yyval.intValue) = INNER;}
    break;

  case 124:
#line 1413 "sqlParser.y"
    {(yyval.intValue) = INNER;}
    break;

  case 125:
#line 1414 "sqlParser.y"
    {(yyval.intValue) = LEFT;}
    break;

  case 126:
#line 1415 "sqlParser.y"
    {(yyval.intValue) = RIGHT;}
    break;

  case 127:
#line 1416 "sqlParser.y"
    {(yyval.intValue) = FULL;}
    break;

  case 131:
#line 1424 "sqlParser.y"
    {
	  yyerror("FEATURE NOT SUPPORTED: GROUP BY");STM->rs->sw->sqlstate = "0A000";YYERROR;
	}
    break;

  case 136:
#line 1436 "sqlParser.y"
    {yyerror("FEATURE NOT SUPPORTED: HAVING");STM->rs->sw->sqlstate = "0A000";YYERROR;}
    break;

  case 139:
#line 1439 "sqlParser.y"
    {obList = newList();}
    break;

  case 143:
#line 1446 "sqlParser.y"
    {//orderby nur auf columns, die in pi angegeben wurden!!!
		//columnList muss zu diesem Zeitpunkt gefüllt sein!!!
		//int found = 0;
		int colnr = (yyvsp[-1].intValue);
		char num[4];
		
		if(colnr>columnList->ft->size(columnList)||colnr<1){			
		  sprintf(num,"%d",(yyvsp[-1].intValue));
		  yyerror(semanticError("UNDIFINED COLUMN:\nColumn number \"",num,"\" in order-by-clause does not exist in the result relation"));STM->rs->sw->sqlstate = "42703";YYERROR;
		}
		else{
			Column* col;
			int i,t = 0;
			for(i=0,col = (Column*)columnList->ft->getFirst(columnList);col;col = (Column*)columnList->ft->getNext(columnList),i++){
				if(i==(colnr-1)){
					t = col->colSQLType;
					break;
				}		
			}
			
			obList->ft->append(obList,newOrder(colnr,t,(yyvsp[0].intValue)));
			//printf("int key found \n");
		}
	}
    break;

  case 144:
#line 1470 "sqlParser.y"
    {
	
		Column* col;
		int i, found;
		for(i=0,col = (Column*)columnList->ft->getFirst(columnList);col;col = (Column*)columnList->ft->getNext(columnList),i++){
			if(strcasecmp(col->colName,(yyvsp[-1].strValue))==0||strcasecmp(col->colAlias,(yyvsp[-1].strValue))==0){
				found = 1;
				obList->ft->append(obList,newOrder(i,col->colSQLType,(yyvsp[0].intValue)));
				//printf("key found\n");
				break;
			}
		}
		if(found==0){
			yyerror(semanticError("UNDEFINED COLUMN:\nColumn \"",(yyvsp[-1].strValue),"\" in order-by-clause does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
		}
	}
    break;

  case 145:
#line 1489 "sqlParser.y"
    {(yyval.intValue) = ASC;}
    break;

  case 146:
#line 1490 "sqlParser.y"
    {(yyval.intValue) = ASC;}
    break;

  case 147:
#line 1491 "sqlParser.y"
    {(yyval.intValue) = DSC;}
    break;

  case 148:
#line 1494 "sqlParser.y"
    {(yyval.intValue) = UNDEF;}
    break;

  case 149:
#line 1495 "sqlParser.y"
    {
		if((yyvsp[-2].intValue)<1){
			char num[10];
			//sprintf(num,"%d",$3);
			yyerror(semanticError("INVALIDE VALUEN:\non positive value x=",num," in \"FETCH FIRST x ROWS ONLY\"-Clause \""));STM->rs->sw->sqlstate = "22020";YYERROR;
		}
		else
			(yyval.intValue)=(yyvsp[-2].intValue);	
	}
    break;

  case 150:
#line 1506 "sqlParser.y"
    {
		Sigma *sigma = (Sigma*) (yyvsp[-1].sigma);
		if((yyvsp[-2].intValue)==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
    break;

  case 152:
#line 1515 "sqlParser.y"
    {//optnot dreht die Einträge, die man aus predicate erhält, um
		Sigma *sigma = (Sigma*) (yyvsp[-1].sigma);
		sigma->link = AND;
		if((yyvsp[-2].intValue)==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
    break;

  case 153:
#line 1522 "sqlParser.y"
    {
		Sigma *sigma = (Sigma*) (yyvsp[-1].sigma);
		sigma->link = OR;
		if((yyvsp[-2].intValue)==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
    break;

  case 155:
#line 1532 "sqlParser.y"
    {
		Sigma *sigma = (Sigma*) (yyvsp[0].sigma);
		sigma->link = AND;
		if((yyvsp[-1].intValue)==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
    break;

  case 156:
#line 1540 "sqlParser.y"
    {
	        Sigma *sigma = (Sigma*) (yyvsp[0].sigma);
		sigma->link = OR;
		if((yyvsp[-1].intValue)==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
    break;

  case 157:
#line 1549 "sqlParser.y"
    {
		Sigma *sigma = (Sigma*) (yyvsp[-1].sigma);
		if((yyvsp[-1].sigma)==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);
		
	}
    break;

  case 158:
#line 1558 "sqlParser.y"
    {(yyval.sigma) = (yyvsp[0].sigma);}
    break;

  case 159:
#line 1563 "sqlParser.y"
    {//ursprünglich stand hier eine expression. bis auf weiteres zurückgestellt
	  	//printf("join pred\n");
	  	Column* colA = (Column*)(yyvsp[-2].column);
		Column* colB = (Column*)(yyvsp[0].column);
		int typeA = colA->colSQLType;
		int typeB = colB->colSQLType;
		
	  	if(typeA!=typeB||typeA==UNDEF||typeB==UNDEF){  		
			yyerror(semanticError("DATATYPE MISMATCH:\nColumn-Types of the two columns do not match: ",colA->colName,colB->colName));STM->rs->sw->sqlstate = "42804";YYERROR;
	  	}
	  		
	  	(yyval.sigma) = newSigma(colA,(yyvsp[-1].intValue),NULL,colB,NULL);//INNER oder FULL????
	}
    break;

  case 160:
#line 1576 "sqlParser.y"
    {
		
		Column* col = (Column*)(yyvsp[-2].column);
		int type = col->colSQLType;
		//printf("%s %s %s %s %d\n",col->colName,col->colAlias,col->tableName,col->tableAlias,type);
	  	int ni = 0;
	  	switch (type) {
		   case CMPI_chars:
		   case CMPI_string:
		   case CMPI_char16:
		   case CMPI_dateTime: // wie werden die verglichen?????? extra machen!!!
		   case CMPI_ref:
		   case CMPI_instance:
		      ni = 1; 
	  	}
	  	if(ni==1){
			yyerror(semanticError("DATA EXCEPTION:\nType of Column\'",col->colName,"\' cannot be casted to a number"));STM->rs->sw->sqlstate = "22000";YYERROR;
	  	}
		     
	  	(yyval.sigma) = newSigma(col,(yyvsp[-1].intValue),(yyvsp[0].strValue),NULL,NULL);
	  	//printf("%s %p %p\n",colnameA,colnameA,$$);
	}
    break;

  case 161:
#line 1598 "sqlParser.y"
    {
		//printf("::: string pred\n");
		Column* col = (Column*)(yyvsp[-2].column);
		
		int type = col->colSQLType;
		
	  	int ni = 0;
	  	switch (type) {  
		   case CMPI_sint64:
		   case CMPI_uint64:
		   case CMPI_sint32:
		   case CMPI_uint32:
		   case CMPI_sint16:
		   case CMPI_uint16:
		   case CMPI_uint8:
		   case CMPI_sint8:
		   case CMPI_boolean:
		   case CMPI_real32:
		   case CMPI_real64:
		   case CMPI_dateTime: // wie werden die verglichen?????? extra machen!!!
		   case CMPI_ref:
		   case CMPI_instance:
		    	ni = 1; 
  		}
	  	
	  	if(ni==1){
			yyerror(semanticError("DATATYPE MISMATCH:\nType of Column \'",col->colName,"\' cannot be casted to a [VAR]CHAR"));STM->rs->sw->sqlstate = "42804";YYERROR;
	  	}
		      
  		(yyval.sigma) = newSigma(col,(yyvsp[-1].intValue),(yyvsp[0].strValue),NULL,NULL);
	  	//printf("%s %p %p\n",colnameA,colnameA,$$);
	}
    break;

  case 162:
#line 1630 "sqlParser.y"
    {//zwei Einträge ex >a AND ex<b == ex between a and b
														// kann eine Stufe weiter oben gemacht werden, wenn AND, NOT bekannt ist!
		//printf("::: between\n");
	  	Column* col = (Column*)(yyvsp[-5].column);
		
		int typeA = col->colSQLType;
		int ni = 0;
	  	switch (typeA) {
		   case CMPI_chars:
		   case CMPI_string:
		   case CMPI_char16:
		   case CMPI_dateTime: // wie werden die verglichen?????? extra machen!!!
		   case CMPI_ref:
		   case CMPI_instance:
		      ni = 1; 
	  	}
	  	if(ni==1){
			yyerror(semanticError("DATA EXCEPTION:\nType of Column\'",col->colName,"\' cannot be casted to a number"));STM->rs->sw->sqlstate = "22000";YYERROR;
	  	}
	  	int between = ((yyvsp[-4].intValue)==NOT)? NOTBETWEEN : BETWEEN;
	  	double a = strtod((yyvsp[-2].strValue),(char**)NULL);
	  	double b = strtod((yyvsp[0].strValue),(char**)NULL);
	  	if(a<b)
	  		(yyval.sigma) = newSigma(col,between,(yyvsp[-2].strValue),NULL,(yyvsp[0].strValue));
	  	 else
	  		(yyval.sigma) = newSigma(col,between,(yyvsp[0].strValue),NULL,(yyvsp[-2].strValue));
	  	//printf("BETWEEN %s and % s\n",$4,$6)		
	}
    break;

  case 163:
#line 1658 "sqlParser.y"
    {
		Column* col = (Column*)(yyvsp[-3].column);
		
		int not = ((yyvsp[-1].intValue)==NOT)? NOTNULL : ISNULL;      
  		(yyval.sigma) = newSigma(col,not,NULL,NULL,NULL);
	}
    break;

  case 164:
#line 1669 "sqlParser.y"
    {
		
		Column* rcol = NULL;
		Column* col;
		for(col = (Column*)allColumnsList->ft->getFirst(allColumnsList);col;col = (Column*)allColumnsList->ft->getNext(allColumnsList)){
			if(strcasecmp(col->colName,(yyvsp[0].strValue))==0||strcasecmp(col->colAlias,(yyvsp[0].strValue))==0){
				rcol = col;//printf("gefunden!!!\n");
			}
			if(rcol!=NULL)
				break;
		}
	  	if(rcol==NULL){
			yyerror(semanticError("UNDIFINED COLUMN:\nColumn \'",(yyvsp[0].strValue),"\' in SEARCH-CONDITION does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}	
	  	(yyval.column) = rcol;
	}
    break;

  case 165:
#line 1685 "sqlParser.y"
    {
	       
		Column* col;
		Column* rcol = NULL;
		char *alias = NULL;
		char *aliasc;
		for(aliasc = (char*)aliasList->ft->getFirst(aliasList);aliasc;aliasc = (char*)aliasList->ft->getNext(aliasList)){
			if(strcasecmp(aliasc,(yyvsp[-2].strValue))==0||strcasecmp(aliasc,(yyvsp[-2].strValue))==0){
				alias = aliasc;
			}
			
			if(alias!=NULL)
				break;
		}
		for(col = (Column*)allColumnsList->ft->getFirst(allColumnsList);col;col = (Column*)allColumnsList->ft->getNext(allColumnsList)){
			if((strcasecmp(col->colName,(yyvsp[0].strValue))==0||strcasecmp(col->colAlias,(yyvsp[0].strValue))==0)&&(alias!=NULL||strcasecmp(col->tableName,(yyvsp[-2].strValue)))){
				rcol = col;
			}
			
			if(rcol!=NULL)
				break;
		}
	  	if(rcol==NULL){
			yyerror(semanticError("UNDIFINED COLUMN:\nColumn \'",(yyvsp[0].strValue),"\' in SEARCH-CONDITION does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}	
	  	(yyval.column) = rcol;
	}
    break;

  case 166:
#line 1714 "sqlParser.y"
    {//printf("::: number1\n");
		char buf[100];
		int i = sprintf(buf,"%d",(yyvsp[0].intValue));
		//printf("::: unumber2 %d %s %d %d\n",i,buf,$1,$1);
		(yyval.strValue) = (char*) calloc(1,i+1);
		strncpy((yyval.strValue),buf,i);
	}
    break;

  case 167:
#line 1721 "sqlParser.y"
    {
		char buf[100];
		int i = sprintf(buf,"%f",(yyvsp[0].doubleValue));
		//printf("::: unumber2 %d %s %f\n",i,buf,$1);
		(yyval.strValue) = (char*) calloc(1,i+1);
		strncpy((yyval.strValue),buf,i);
	}
    break;

  case 168:
#line 1737 "sqlParser.y"
    {(yyval.intValue) = EQ;}
    break;

  case 169:
#line 1738 "sqlParser.y"
    {(yyval.intValue) = NE;}
    break;

  case 170:
#line 1739 "sqlParser.y"
    {(yyval.intValue) = GT;}
    break;

  case 171:
#line 1740 "sqlParser.y"
    {(yyval.intValue) = LT;}
    break;

  case 172:
#line 1741 "sqlParser.y"
    {(yyval.intValue) = GE;}
    break;

  case 173:
#line 1742 "sqlParser.y"
    {(yyval.intValue) = LE;}
    break;

  case 174:
#line 1759 "sqlParser.y"
    {(yyval.intValue) = UNDEF;}
    break;

  case 175:
#line 1760 "sqlParser.y"
    {(yyval.intValue) = NOT;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 3353 "sqlParser.c"

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


#line 1774 "sqlParser.y"

static UtilList* cloneAL(){
	UtilList * res = newList();
	
	ExpressionLight* exl;
	for(exl = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);exl;exl = (ExpressionLight*)assignmentList->ft->getNext(assignmentList)){
		res->ft->append(res,newExpressionLight(exl->name,UNDEF,NULL));
	}
	
	return res;
}

static char* semanticError(char* s1, char* s2, char* s3){
	char *message = (char *) malloc(strlen(s1)+strlen(s2)+strlen(s3)+1);
	strcpy(message,s1);
	strcat(message,s2);
	strcat(message,s3);
	return message;
}
//int main() {printf("hm\n");return yyparse(); }

