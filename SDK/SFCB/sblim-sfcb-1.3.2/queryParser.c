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

/* Substitute the variable and function names.  */
#define yyparse sfcQueryparse
#define yylex   sfcQuerylex
#define yyerror sfcQueryerror
#define yylval  sfcQuerylval
#define yychar  sfcQuerychar
#define yydebug sfcQuerydebug
#define yynerrs sfcQuerynerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_INTEGER = 258,
     TOK_DOUBLE = 259,
     TOK_STRING = 260,
     TOK_TRUE = 261,
     TOK_FALSE = 262,
     TOK_NULL = 263,
     TOK_EQ = 264,
     TOK_NE = 265,
     TOK_LT = 266,
     TOK_LE = 267,
     TOK_GT = 268,
     TOK_GE = 269,
     TOK_NOT = 270,
     TOK_OR = 271,
     TOK_AND = 272,
     TOK_IS = 273,
     TOK_ISA = 274,
     TOK_SCOPE = 275,
     TOK_IDENTIFIER = 276,
     TOK_SELECT = 277,
     TOK_WHERE = 278,
     TOK_FROM = 279,
     TOK_UNEXPECTED_CHAR = 280
   };
#endif
/* Tokens.  */
#define TOK_INTEGER 258
#define TOK_DOUBLE 259
#define TOK_STRING 260
#define TOK_TRUE 261
#define TOK_FALSE 262
#define TOK_NULL 263
#define TOK_EQ 264
#define TOK_NE 265
#define TOK_LT 266
#define TOK_LE 267
#define TOK_GT 268
#define TOK_GE 269
#define TOK_NOT 270
#define TOK_OR 271
#define TOK_AND 272
#define TOK_IS 273
#define TOK_ISA 274
#define TOK_SCOPE 275
#define TOK_IDENTIFIER 276
#define TOK_SELECT 277
#define TOK_WHERE 278
#define TOK_FROM 279
#define TOK_UNEXPECTED_CHAR 280




/* Copy the first part of user declarations.  */
#line 10 "queryParser.y"


/*
 * $Id: queryParser.y,v 1.11 2007/03/20 09:36:49 sschuetz Exp $
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 * Based on concepts from pegasus project 
 *
 * Description:
 *
 * WQL/CQL query parser for sfcb.
 *
*/


//%2005////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queryOperation.h"
#include "mlog.h"

#define YYPARSE_PARAM parm
#define YYLEX_PARAM parm
#define YYERROR_VERBOSE 1 
 
extern int yylex();
extern void sfcQueryErr(char*,char*,char*);
extern void yyError(char*);
extern void yyerror(char*);
extern void sfcQueryError(char* s1);
extern char * qsStrDup(QLStatement *qs, char *str);
extern QLOperand* newNameQueryOperand(QLStatement *qs, char* val);


#define sfcQueryerror sfcQueryError
#define yyErr sfcQueryErr

#define QS ((QLControl*)parm)->statement
#define QC ((QLControl*)parm)->collector



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
#line 100 "queryParser.y"
typedef union YYSTYPE {
   long intValue;
   double doubleValue;
   char* strValue;
   void* nodeValue;
   
   QLOperand* fullPropertyName;
   QLOperand* propertyName;
   QLOperand* comparisonTerm;
   QLOperation* searchCondition;
   QLOperation* predicate;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 237 "queryParser.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 249 "queryParser.c"

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   75

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  25
/* YYNRULES -- Number of rules. */
#define YYNRULES  54
/* YYNRULES -- Number of states. */
#define YYNSTATES  85

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      28,    29,    26,     2,    27,     2,    30,     2,     2,     2,
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
      25
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     5,     9,    11,    13,    15,    19,    22,
      24,    27,    29,    33,    36,    38,    41,    45,    49,    52,
      56,    58,    62,    67,    69,    71,    73,    75,    79,    83,
      87,    91,    95,    99,   103,   107,   112,   113,   117,   119,
     121,   123,   127,   129,   131,   133,   135,   138,   140,   142,
     144,   146,   150,   154,   157
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      32,     0,    -1,    33,    -1,    22,    34,    36,    -1,    26,
      -1,    35,    -1,    49,    -1,    35,    27,    49,    -1,    37,
      40,    -1,    37,    -1,    24,    38,    -1,    39,    -1,    38,
      27,    39,    -1,    50,    51,    -1,    50,    -1,    23,    41,
      -1,    41,    16,    41,    -1,    41,    17,    41,    -1,    15,
      41,    -1,    28,    41,    29,    -1,    43,    -1,    43,    18,
      42,    -1,    43,    18,    15,    42,    -1,     6,    -1,     7,
      -1,    44,    -1,    45,    -1,    53,    11,    53,    -1,    53,
      13,    53,    -1,    53,    12,    53,    -1,    53,    14,    53,
      -1,    53,     9,    53,    -1,    53,    10,    53,    -1,    53,
      19,    52,    -1,    53,    18,     8,    -1,    53,    18,    15,
       8,    -1,    -1,    47,    30,    48,    -1,    48,    -1,    55,
      -1,    21,    -1,    21,    20,    21,    -1,    21,    -1,    21,
      -1,    21,    -1,    50,    -1,    46,    47,    -1,     3,    -1,
       4,    -1,     5,    -1,    42,    -1,    28,    47,    29,    -1,
      28,     5,    29,    -1,    28,    29,    -1,    21,    54,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   188,   188,   193,   199,   203,   209,   213,   219,   223,
     228,   233,   236,   241,   245,   251,   257,   262,   267,   271,
     275,   279,   291,   304,   308,   317,   321,   327,   331,   335,
     339,   343,   348,   352,   358,   362,   369,   375,   379,   383,
     389,   394,   406,   411,   416,   423,   429,   434,   438,   442,
     446,   452,   454,   456,   459
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_INTEGER", "TOK_DOUBLE",
  "TOK_STRING", "TOK_TRUE", "TOK_FALSE", "TOK_NULL", "TOK_EQ", "TOK_NE",
  "TOK_LT", "TOK_LE", "TOK_GT", "TOK_GE", "TOK_NOT", "TOK_OR", "TOK_AND",
  "TOK_IS", "TOK_ISA", "TOK_SCOPE", "TOK_IDENTIFIER", "TOK_SELECT",
  "TOK_WHERE", "TOK_FROM", "TOK_UNEXPECTED_CHAR", "'*'", "','", "'('",
  "')'", "'.'", "$accept", "start", "selectStatement", "selectList",
  "propertyList", "selectExpression", "fromClause", "classList",
  "classListEntry", "whereClause", "searchCondition", "truthValue",
  "predicate", "comparisonPredicate", "nullPredicate", "initPropName",
  "classPropertyName", "fullPropertyName", "propertyName", "className",
  "classAlias", "classId", "comparisonTerm", "functionArg",
  "builtInFunction", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    42,    44,    40,    41,
      46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    31,    32,    33,    34,    34,    35,    35,    36,    36,
      37,    38,    38,    39,    39,    40,    41,    41,    41,    41,
      41,    41,    41,    42,    42,    43,    43,    44,    44,    44,
      44,    44,    44,    44,    45,    45,    46,    47,    47,    47,
      48,    48,    49,    50,    51,    52,    53,    53,    53,    53,
      53,    54,    54,    54,    55
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     3,     1,     1,     1,     3,     2,     1,
       2,     1,     3,     2,     1,     2,     3,     3,     2,     3,
       1,     3,     4,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     3,     4,     0,     3,     1,     1,
       1,     3,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     3,     3,     2,     2
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
       0,     0,     0,     2,    42,     4,     0,     5,     6,     1,
       0,     3,     9,     0,    43,    10,    11,    14,    36,     8,
       7,     0,    44,    13,    47,    48,    49,    23,    24,    36,
      36,    15,    50,    20,    25,    26,     0,     0,    12,    18,
       0,    36,    36,     0,    40,    46,    38,    39,    36,    36,
      36,    36,    36,    36,     0,     0,    19,    16,    17,     0,
      21,     0,     0,    54,     0,    31,    32,    27,    29,    28,
      30,    34,     0,    45,    33,    22,    41,     0,    53,     0,
      40,    37,    35,    52,    51
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     2,     3,     6,     7,    11,    12,    15,    16,    19,
      31,    32,    33,    34,    35,    36,    45,    46,     8,    17,
      23,    74,    37,    63,    47
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -38
static const yysigned_char yypact[] =
{
       6,    -8,    14,   -38,   -38,   -38,    32,    30,   -38,   -38,
      34,   -38,    35,    38,   -38,    33,   -38,    40,    -3,   -38,
     -38,    34,   -38,   -38,   -38,   -38,   -38,   -38,   -38,    -3,
      -3,    10,   -38,    44,   -38,   -38,    42,    21,   -38,   -38,
      -6,    -3,    -3,     9,   -11,    36,   -38,   -38,    47,    47,
      47,    47,    47,    47,    28,    34,   -38,    48,   -38,    31,
     -38,    43,     0,   -38,    46,   -38,   -38,   -38,   -38,   -38,
     -38,   -38,    60,   -38,   -38,   -38,   -38,    41,   -38,    12,
      49,   -38,   -38,   -38,   -38
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -38,   -38,   -38,   -38,   -38,   -38,   -38,   -38,    50,   -38,
     -22,   -37,   -38,   -38,   -38,   -38,    11,     8,    61,    20,
     -38,   -38,    -4,   -38,   -38
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const unsigned char yytable[] =
{
      24,    25,    26,    27,    28,    77,    60,    39,    40,    61,
      41,    42,    29,     4,     9,    27,    28,    62,     5,    57,
      58,    44,    75,    56,    59,    30,    41,    42,     1,    78,
      48,    49,    50,    51,    52,    53,    71,    27,    28,    54,
      55,    84,    64,    72,    65,    66,    67,    68,    69,    70,
      24,    25,    26,    27,    28,    14,    10,    13,    18,     4,
      21,    22,    43,    44,    76,    42,    64,    80,    82,    61,
      83,    38,    81,    79,    20,    73
};

static const unsigned char yycheck[] =
{
       3,     4,     5,     6,     7,     5,    43,    29,    30,    20,
      16,    17,    15,    21,     0,     6,     7,    28,    26,    41,
      42,    21,    59,    29,    15,    28,    16,    17,    22,    29,
       9,    10,    11,    12,    13,    14,     8,     6,     7,    18,
      19,    29,    30,    15,    48,    49,    50,    51,    52,    53,
       3,     4,     5,     6,     7,    21,    24,    27,    23,    21,
      27,    21,    18,    21,    21,    17,    30,    21,     8,    20,
      29,    21,    64,    62,    13,    55
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    22,    32,    33,    21,    26,    34,    35,    49,     0,
      24,    36,    37,    27,    21,    38,    39,    50,    23,    40,
      49,    27,    21,    51,     3,     4,     5,     6,     7,    15,
      28,    41,    42,    43,    44,    45,    46,    53,    39,    41,
      41,    16,    17,    18,    21,    47,    48,    55,     9,    10,
      11,    12,    13,    14,    18,    19,    29,    41,    41,    15,
      42,    20,    28,    54,    30,    53,    53,    53,    53,    53,
      53,     8,    15,    50,    52,    42,    21,     5,    29,    47,
      21,    48,     8,    29,    29
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
#line 189 "queryParser.y"
    {
    }
    break;

  case 3:
#line 194 "queryParser.y"
    {

    }
    break;

  case 4:
#line 200 "queryParser.y"
    {
       QS->ft->setAllProperties(QS,1);
    }
    break;

  case 5:
#line 204 "queryParser.y"
    {

    }
    break;

  case 6:
#line 210 "queryParser.y"
    {
       QS->ft->appendSelectPropertyName(QS,qsStrDup(QS,(yyvsp[0].strValue)));
    }
    break;

  case 7:
#line 214 "queryParser.y"
    {
       QS->ft->appendSelectPropertyName(QS,qsStrDup(QS,(yyvsp[0].strValue)));
    }
    break;

  case 8:
#line 220 "queryParser.y"
    {

    }
    break;

  case 9:
#line 224 "queryParser.y"
    {
    }
    break;

  case 10:
#line 229 "queryParser.y"
    {
    }
    break;

  case 11:
#line 234 "queryParser.y"
    {
    }
    break;

  case 12:
#line 237 "queryParser.y"
    {
    }
    break;

  case 13:
#line 242 "queryParser.y"
    {
       QS->ft->addFromClass(QS,qsStrDup(QS,(yyvsp[-1].strValue)),(yyvsp[0].strValue));
    }
    break;

  case 14:
#line 246 "queryParser.y"
    {
        QS->ft->addFromClass(QS,qsStrDup(QS,(yyvsp[0].strValue)),NULL);
    }
    break;

  case 15:
#line 252 "queryParser.y"
    {
       QS->ft->setWhereCondition(QS,(QLOperation*)(yyvsp[0].searchCondition));
    }
    break;

  case 16:
#line 258 "queryParser.y"
    {
       (yyval.searchCondition)=newOrOperation(QS,(yyvsp[-2].searchCondition),(yyvsp[0].searchCondition));
       QL_TRACE(fprintf(stderr,"- - searchCondition: %p (%p-%p)\n",(yyval.searchCondition),(yyvsp[-2].searchCondition),(yyvsp[0].searchCondition)));
    }
    break;

  case 17:
#line 263 "queryParser.y"
    {
       (yyval.searchCondition)=newAndOperation(QS,(yyvsp[-2].searchCondition),(yyvsp[0].searchCondition));

    }
    break;

  case 18:
#line 268 "queryParser.y"
    {
       (yyval.searchCondition)=newNotOperation(QS,(yyvsp[0].searchCondition));
    }
    break;

  case 19:
#line 272 "queryParser.y"
    {
       (yyval.searchCondition)=(yyvsp[-1].searchCondition);
    }
    break;

  case 20:
#line 276 "queryParser.y"
    {
       (yyval.searchCondition)=newBinaryOperation(QS,(yyvsp[0].predicate));
    }
    break;

  case 21:
#line 280 "queryParser.y"
    {
       if (QS->lang==QL_WQL) {
          if ((yyvsp[0].intValue));
          else (yyvsp[-2].predicate)->ft->eliminateNots((yyvsp[-2].predicate),1);
          (yyval.searchCondition)=newBinaryOperation(QS,(yyvsp[-2].predicate));
      }
       else {
          yyErr("Three state logic not supported: IS ",(yyvsp[0].intValue)?"TRUE":"FALSE","");
          YYERROR;
       }
    }
    break;

  case 22:
#line 292 "queryParser.y"
    {
       if (QS->lang==QL_WQL) {
          if ((yyvsp[0].intValue)) (yyvsp[-3].predicate)->ft->eliminateNots((yyvsp[-3].predicate),1);
           (yyval.searchCondition)=newBinaryOperation(QS,(yyvsp[-3].predicate));
      }
       else {
          yyErr("Three state logic not supported: IS NOT ",(yyvsp[0].intValue)?"TRUE":"FALSE","");
          YYERROR;
       }
    }
    break;

  case 23:
#line 305 "queryParser.y"
    {
       (yyval.intValue) = 1;
    }
    break;

  case 24:
#line 309 "queryParser.y"
    {
       (yyval.intValue) = 0;
    }
    break;

  case 25:
#line 318 "queryParser.y"
    {
       (yyval.predicate)=(yyvsp[0].predicate);
    }
    break;

  case 26:
#line 322 "queryParser.y"
    {
       (yyval.predicate)=(yyvsp[0].predicate);
    }
    break;

  case 27:
#line 328 "queryParser.y"
    {
       (yyval.predicate)=newLtOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
    }
    break;

  case 28:
#line 332 "queryParser.y"
    {
       (yyval.predicate)=newGtOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
    }
    break;

  case 29:
#line 336 "queryParser.y"
    {
       (yyval.predicate)=newLeOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
    }
    break;

  case 30:
#line 340 "queryParser.y"
    {
       (yyval.predicate)=newGeOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
    }
    break;

  case 31:
#line 344 "queryParser.y"
    {
       (yyval.predicate)=newEqOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
       QL_TRACE(fprintf(stderr,"- - comparisonPredicate: %p (%p-%p)\n",(yyval.predicate),(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm)));
    }
    break;

  case 32:
#line 349 "queryParser.y"
    {
       (yyval.predicate)=newNeOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
    }
    break;

  case 33:
#line 353 "queryParser.y"
    {
       (yyval.predicate)=newIsaOperation(QS,(yyvsp[-2].comparisonTerm),(yyvsp[0].comparisonTerm));
    }
    break;

  case 34:
#line 359 "queryParser.y"
    {
       (yyval.predicate)=newIsNullOperation(QS,(yyvsp[-2].comparisonTerm));
    }
    break;

  case 35:
#line 363 "queryParser.y"
    {
       (yyval.predicate)=newIsNotNullOperation(QS,(yyvsp[-3].comparisonTerm));
    }
    break;

  case 36:
#line 369 "queryParser.y"
    {
       QL_TRACE(fprintf(stderr,"### initPropName\n"));
       QC->propName=NULL;
    }
    break;

  case 37:
#line 376 "queryParser.y"
    {
       QL_TRACE(fprintf(stderr,"-#- classPropertyName: classPropertyName '.' fullPropertyName\n"));
    }
    break;

  case 38:
#line 380 "queryParser.y"
    {
       QL_TRACE(fprintf(stderr,"-#- classPropertyName: fullPropertyName\n"));
    }
    break;

  case 39:
#line 384 "queryParser.y"
    {
    }
    break;

  case 40:
#line 390 "queryParser.y"
    {
       QL_TRACE(fprintf(stderr,"-#- fullPropertyName: propertyIdentifier  %s\n",(yyvsp[0].strValue)));
       QC->addPropIdentifier(QC,QS,NULL,qsStrDup(QS,(yyvsp[0].strValue)),-99999);
    }
    break;

  case 41:
#line 395 "queryParser.y"
    {
       QL_TRACE(fprintf(stderr,"-#- fullPropertyName: propertyClassIdentifier TOK_SCOPE propertyIdentifier %s::%s\n",(yyvsp[-2].strValue),(yyvsp[0].strValue)));
       if (QS->lang==QL_CQL)
          QC->addPropIdentifier(QC,QS,qsStrDup(QS,(yyvsp[-2].strValue)),qsStrDup(QS,(yyvsp[0].strValue)),-99999);
       else {
          yyErr("CQL style property not supported with language=wql","","");
          YYERROR;
       }
    }
    break;

  case 42:
#line 407 "queryParser.y"
    {
       (yyval.strValue)=(yyvsp[0].strValue);
    }
    break;

  case 43:
#line 412 "queryParser.y"
    {
       (yyval.strValue) = (yyvsp[0].strValue);
    }
    break;

  case 44:
#line 417 "queryParser.y"
    {
       (yyval.strValue) = (yyvsp[0].strValue);
    }
    break;

  case 45:
#line 424 "queryParser.y"
    {
       (yyval.comparisonTerm)=newNameQueryOperand(QS,qsStrDup(QS,(yyvsp[0].strValue)));
    }
    break;

  case 46:
#line 430 "queryParser.y"
    {
       (yyval.comparisonTerm)=newPropQueryOperand(QS,QC->propName);
       QC->propName=NULL;
    }
    break;

  case 47:
#line 435 "queryParser.y"
    {
       (yyval.comparisonTerm)=newIntQueryOperand(QS,(yyvsp[0].intValue));
    }
    break;

  case 48:
#line 439 "queryParser.y"
    {
       (yyval.comparisonTerm)=newDoubleQueryOperand(QS,(yyvsp[0].doubleValue));
    }
    break;

  case 49:
#line 443 "queryParser.y"
    {
       (yyval.comparisonTerm)=newCharsQueryOperand(QS,qsStrDup(QS,(yyvsp[0].strValue)));
    }
    break;

  case 50:
#line 447 "queryParser.y"
    {
       (yyval.comparisonTerm)=newBooleanQueryOperand(QS,(yyvsp[0].intValue));
    }
    break;

  case 54:
#line 460 "queryParser.y"
    {
       int fnc=QL_FNC_NoFunction;
       if (QS->lang==QL_CQL) {
          if (strcasecmp((yyvsp[-1].strValue),"classname")==0) fnc=QL_FNC_Classname;
          else if (strcasecmp((yyvsp[-1].strValue),"namespacename")==0) fnc=QL_FNC_Namespacename;
          else if (strcasecmp((yyvsp[-1].strValue),"namespacetype")==0) fnc=QL_FNC_Namespacetype;
          else if (strcasecmp((yyvsp[-1].strValue),"hostport")==0) fnc=QL_FNC_Hostport;
          else if (strcasecmp((yyvsp[-1].strValue),"modelpath")==0) fnc=QL_FNC_Modelpath;
          else if (strcasecmp((yyvsp[-1].strValue),"classpath")==0) fnc=QL_FNC_Classpath;
          else if (strcasecmp((yyvsp[-1].strValue),"objectpath")==0) fnc=QL_FNC_Objectpath;
          else if (strcasecmp((yyvsp[-1].strValue),"instancetoreference")==0) fnc=QL_FNC_InstanceToReference;
          else if (strcasecmp((yyvsp[-1].strValue),"currentdatetime")==0) fnc=QL_FNC_CurrentDateTime;
          else if (strcasecmp((yyvsp[-1].strValue),"datetime")==0) fnc=QL_FNC_DateTime;
          else if (strcasecmp((yyvsp[-1].strValue),"microsecondtotimestamp")==0) fnc=QL_FNC_MicrosecondsToTimestamp;
          else if (strcasecmp((yyvsp[-1].strValue),"microsecondtointerval")==0) fnc=QL_FNC_MicrosecondsToInterval;
          else  {
             yyErr("Function ",(yyvsp[-1].strValue)," not supported");
             YYERROR;
          }
       }
       else {
          yyErr("Functions not supported when language != cql","","");
          YYERROR;
       }
    }
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1708 "queryParser.c"

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


#line 486 "queryParser.y"


