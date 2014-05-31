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
     Identifier = 258,
     BaseTypeUINT8 = 259,
     BaseTypeSINT8 = 260,
     BaseTypeUINT16 = 261,
     BaseTypeSINT16 = 262,
     BaseTypeUINT32 = 263,
     BaseTypeSINT32 = 264,
     BaseTypeUINT64 = 265,
     BaseTypeSINT64 = 266,
     BaseTypeREAL32 = 267,
     BaseTypeREAL64 = 268,
     BaseTypeSTRING = 269,
     BaseTypeCHAR16 = 270,
     BaseTypeDATETIME = 271,
     BaseTypeBOOLEAN = 272,
     IntLiteral = 273,
     FloatLiteral = 274,
     StringLiteral = 275,
     CharLiteral = 276,
     BoolLiteral = 277,
     NullLiteral = 278,
     AS = 279,
     CLASS = 280,
     INSTANCE = 281,
     OF = 282,
     QUALIFIER = 283,
     REF = 284,
     FLAVOR = 285,
     SCOPE = 286
   };
#endif
/* Tokens.  */
#define Identifier 258
#define BaseTypeUINT8 259
#define BaseTypeSINT8 260
#define BaseTypeUINT16 261
#define BaseTypeSINT16 262
#define BaseTypeUINT32 263
#define BaseTypeSINT32 264
#define BaseTypeUINT64 265
#define BaseTypeSINT64 266
#define BaseTypeREAL32 267
#define BaseTypeREAL64 268
#define BaseTypeSTRING 269
#define BaseTypeCHAR16 270
#define BaseTypeDATETIME 271
#define BaseTypeBOOLEAN 272
#define IntLiteral 273
#define FloatLiteral 274
#define StringLiteral 275
#define CharLiteral 276
#define BoolLiteral 277
#define NullLiteral 278
#define AS 279
#define CLASS 280
#define INSTANCE 281
#define OF 282
#define QUALIFIER 283
#define REF 284
#define FLAVOR 285
#define SCOPE 286




/* Copy the first part of user declarations.  */
#line 20 "mofc.y"


/*
 * passhthru
 */
# include <stdio.h>
# include <string.h>
# include <hash.h>
# include <mofdecl.h>
# include "symtab.h"

extern class_chain  * cls_chain_current;
extern class_chain  * inst_chain_current;
extern qual_chain  * qual_chain_current;



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
#line 42 "mofc.y"
typedef union YYSTYPE {
        char *                lval_id;
        type_type             lval_type;
        char *                lval_literal;
        class_entry         * lval_class;
        prop_or_method_list * lval_props;
        qual_chain          * lval_quals;
        qual_entry          * lval_qual;
        value_chain         * lval_vals;
        param_chain         * lval_params;
        int                   lval_int;
        qual_quals			  lval_qual_quals;
        } YYSTYPE;
/* Line 191 of yacc.c.  */
#line 178 "mofc.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 190 "mofc.c"

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   142

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  36
/* YYNRULES -- Number of rules. */
#define YYNRULES  86
/* YYNRULES -- Number of states. */
#define YYNSTATES  140

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    36,     2,     2,    34,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    32,    33,
       2,    41,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    39,     2,    40,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    37,     2,    38,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned char yyprhs[] =
{
       0,     0,     3,     4,     6,     8,    11,    13,    15,    17,
      26,    27,    33,    39,    50,    52,    56,    58,    62,    64,
      66,    75,    85,    86,    90,    92,    96,    99,   100,   104,
     108,   109,   111,   113,   117,   118,   121,   122,   124,   126,
     129,   137,   139,   142,   144,   146,   148,   150,   152,   154,
     156,   158,   160,   162,   164,   166,   168,   170,   171,   174,
     178,   179,   181,   182,   185,   189,   191,   195,   200,   201,
     204,   206,   210,   212,   214,   216,   218,   220,   222,   224,
     227,   228,   231,   232,   234,   236,   239
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      43,     0,    -1,    -1,    44,    -1,    45,    -1,    44,    45,
      -1,    46,    -1,    51,    -1,    52,    -1,    28,     3,    32,
      64,    65,    70,    47,    33,    -1,    -1,    34,    31,    35,
      48,    36,    -1,    34,    30,    35,    49,    36,    -1,    34,
      31,    35,    48,    36,    34,    30,    35,    49,    36,    -1,
      50,    -1,    48,    34,    50,    -1,    50,    -1,    49,    34,
      50,    -1,     3,    -1,    25,    -1,    53,    25,     3,    59,
      37,    60,    38,    33,    -1,    53,    26,    27,     3,    74,
      37,    75,    38,    33,    -1,    -1,    39,    54,    40,    -1,
      55,    -1,    54,    34,    55,    -1,     3,    56,    -1,    -1,
      35,    66,    36,    -1,    37,    57,    38,    -1,    -1,    58,
      -1,    72,    -1,    58,    34,    72,    -1,    -1,    32,     3,
      -1,    -1,    61,    -1,    62,    -1,    61,    62,    -1,    53,
      63,     3,    65,    67,    70,    33,    -1,    64,    -1,     3,
      29,    -1,     5,    -1,     4,    -1,     7,    -1,     6,    -1,
       9,    -1,     8,    -1,    11,    -1,    10,    -1,    12,    -1,
      13,    -1,    14,    -1,    15,    -1,    17,    -1,    16,    -1,
      -1,    39,    40,    -1,    39,    72,    40,    -1,    -1,    72,
      -1,    -1,    35,    36,    -1,    35,    68,    36,    -1,    69,
      -1,    68,    34,    69,    -1,    53,    63,     3,    65,    -1,
      -1,    41,    71,    -1,    72,    -1,    37,    57,    38,    -1,
      18,    -1,    19,    -1,    21,    -1,    73,    -1,    22,    -1,
      23,    -1,    20,    -1,    73,    20,    -1,    -1,    24,     3,
      -1,    -1,    76,    -1,    77,    -1,    76,    77,    -1,     3,
      70,    33,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   126,   126,   127,   135,   136,   139,   143,   147,   157,
     167,   171,   176,   181,   188,   192,   198,   202,   208,   209,
     216,   223,   233,   234,   237,   238,   241,   245,   246,   247,
     250,   251,   254,   255,   258,   259,   262,   263,   267,   268,
     273,   282,   283,   286,   287,   288,   289,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   302,   303,   304,
     307,   308,   311,   312,   313,   316,   317,   320,   324,   325,
     328,   329,   332,   333,   334,   335,   336,   337,   340,   341,
     344,   345,   348,   349,   355,   359,   365
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "Identifier", "BaseTypeUINT8",
  "BaseTypeSINT8", "BaseTypeUINT16", "BaseTypeSINT16", "BaseTypeUINT32",
  "BaseTypeSINT32", "BaseTypeUINT64", "BaseTypeSINT64", "BaseTypeREAL32",
  "BaseTypeREAL64", "BaseTypeSTRING", "BaseTypeCHAR16", "BaseTypeDATETIME",
  "BaseTypeBOOLEAN", "IntLiteral", "FloatLiteral", "StringLiteral",
  "CharLiteral", "BoolLiteral", "NullLiteral", "AS", "CLASS", "INSTANCE",
  "OF", "QUALIFIER", "REF", "FLAVOR", "SCOPE", "':'", "';'", "','", "'('",
  "')'", "'{'", "'}'", "'['", "']'", "'='", "$accept", "mof",
  "definition_list", "definition", "qualifier_definition",
  "opt_qualifier_extension_list", "scope_list", "flavor_list",
  "ext_identifier", "class_definition", "instance_definition",
  "opt_qualifier_list", "qualifier_list", "qualifier",
  "opt_qualifier_initializer", "opt_literal_list", "literal_list",
  "opt_superclass", "opt_property_or_method_definitionlist",
  "property_or_method_definitionlist", "property_or_method_definition",
  "full_type", "base_type", "opt_array_spec", "opt_base_literal",
  "opt_parameter_list", "parameter_list", "parameter", "opt_initializer",
  "initial_value", "base_literal", "string_literal_list", "opt_alias",
  "opt_property_initializer_list", "property_initializer_list",
  "property_initializer", 0
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
     285,   286,    58,    59,    44,    40,    41,   123,   125,    91,
      93,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    42,    43,    43,    44,    44,    45,    45,    45,    46,
      47,    47,    47,    47,    48,    48,    49,    49,    50,    50,
      51,    52,    53,    53,    54,    54,    55,    56,    56,    56,
      57,    57,    58,    58,    59,    59,    60,    60,    61,    61,
      62,    63,    63,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    65,    65,    65,
      66,    66,    67,    67,    67,    68,    68,    69,    70,    70,
      71,    71,    72,    72,    72,    72,    72,    72,    73,    73,
      74,    74,    75,    75,    76,    76,    77
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     1,     1,     8,
       0,     5,     5,    10,     1,     3,     1,     3,     1,     1,
       8,     9,     0,     3,     1,     3,     2,     0,     3,     3,
       0,     1,     1,     3,     0,     2,     0,     1,     1,     2,
       7,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     2,     3,
       0,     1,     0,     2,     3,     1,     3,     4,     0,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     2,
       0,     2,     0,     1,     1,     2,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned char yydefact[] =
{
      22,     0,     0,     0,    22,     4,     6,     7,     8,     0,
       0,    27,     0,    24,     1,     5,     0,     0,     0,    60,
      30,    26,     0,    23,    34,     0,    44,    43,    46,    45,
      48,    47,    50,    49,    51,    52,    53,    54,    56,    55,
      57,    72,    73,    78,    74,    76,    77,     0,    61,    75,
       0,    31,    32,    25,     0,     0,    80,     0,    68,    28,
      79,    29,     0,    35,    22,     0,     0,    58,     0,     0,
      10,    33,     0,     0,    22,    38,    81,    82,    59,    30,
      69,    70,     0,     0,     0,     0,    41,     0,    39,    68,
       0,    83,    84,     0,     0,     0,     9,    42,    57,    20,
       0,     0,    85,    71,     0,     0,    62,    86,    21,    18,
      19,     0,    16,     0,    14,    22,    68,     0,    12,     0,
      11,    63,     0,     0,    65,     0,    17,    15,     0,     0,
      22,    64,    40,     0,    57,    66,     0,    67,     0,    13
};

/* YYDEFGOTO[NTERM-NUM]. */
static const yysigned_char yydefgoto[] =
{
      -1,     3,     4,     5,     6,    83,   113,   111,   112,     7,
       8,     9,    12,    13,    21,    50,    51,    55,    73,    74,
      75,    85,    86,    58,    47,   116,   123,   124,    70,    80,
      52,    49,    66,    90,    91,    92
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -98
static const yysigned_char yypact[] =
{
       3,    15,    22,    29,     4,   -98,   -98,   -98,   -98,    34,
      33,    -9,    -4,   -98,   -98,   -98,    95,    72,    78,    35,
      35,   -98,    22,   -98,    68,    98,   -98,   -98,   -98,   -98,
     -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,   -98,
      63,   -98,   -98,   -98,   -98,   -98,   -98,    67,   -98,    84,
      69,    71,   -98,   -98,   103,    73,    85,    -6,    70,   -98,
     -98,   -98,    35,   -98,    23,   105,    75,   -98,    74,     1,
      79,   -98,    64,    77,    25,   -98,   -98,   113,   -98,    35,
     -98,   -98,    66,    86,    88,   115,   -98,    87,   -98,    70,
      83,   113,   -98,    89,    90,    91,   -98,   -98,    63,   -98,
      96,    97,   -98,   -98,     2,     2,    93,   -98,   -98,   -98,
     -98,   -27,   -98,     5,   -98,   -28,    70,     2,   -98,     2,
      99,   -98,    64,    10,   -98,   101,   -98,   -98,    92,   120,
     100,   -98,   -98,   102,    63,   -98,     2,   -98,    14,   -98
};

/* YYPGOTO[NTERM-NUM].  */
static const yysigned_char yypgoto[] =
{
     -98,   -98,   -98,   127,   -98,   -98,   -98,   -12,   -70,   -98,
     -98,   -64,   -98,   110,   -98,    56,   -98,   -98,   -98,   -98,
      62,    16,   122,   -97,   -98,   -98,   -98,    11,   -83,   -98,
     -17,   -98,   -98,   -98,   -98,    51
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -38
static const short int yytable[] =
{
      72,   106,    48,    -2,    -3,   109,   100,   117,   121,   118,
      72,     2,    41,    42,    43,    44,    45,    46,    10,    41,
      42,    43,    44,    45,    46,    11,    19,   110,    20,    14,
      22,     1,     1,   125,    67,   114,    23,   137,    79,   119,
      68,   120,     2,     2,   130,    71,   131,   126,   117,   127,
     139,   122,    81,    41,    42,    43,    44,    45,    46,    16,
      17,   -36,     2,   -37,     2,    18,   122,    84,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    94,    95,    24,    25,
      54,    56,    57,    59,    60,    62,    63,    61,    76,    65,
      64,    69,    77,    82,    78,    87,    89,    97,    98,    96,
      99,   101,   133,   134,   138,   104,   105,   103,   115,   107,
     108,    15,    53,   128,   132,    93,    88,   136,   129,     2,
      40,   135,   102
};

static const unsigned char yycheck[] =
{
      64,    98,    19,     0,     0,     3,    89,    34,    36,    36,
      74,    39,    18,    19,    20,    21,    22,    23,     3,    18,
      19,    20,    21,    22,    23,     3,    35,    25,    37,     0,
      34,    28,    28,   116,    40,   105,    40,   134,    37,    34,
      57,    36,    39,    39,    34,    62,    36,   117,    34,   119,
      36,   115,    69,    18,    19,    20,    21,    22,    23,    25,
      26,    38,    39,    38,    39,    32,   130,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    30,    31,     3,    27,
      32,     3,    39,    36,    20,    34,     3,    38,     3,    24,
      37,    41,    37,    34,    40,    38,     3,    29,     3,    33,
      33,    38,    30,     3,   136,    35,    35,    38,    35,    33,
      33,     4,    22,    34,    33,    79,    74,    35,   122,    39,
      18,   130,    91
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,    28,    39,    43,    44,    45,    46,    51,    52,    53,
       3,     3,    54,    55,     0,    45,    25,    26,    32,    35,
      37,    56,    34,    40,     3,    27,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      64,    18,    19,    20,    21,    22,    23,    66,    72,    73,
      57,    58,    72,    55,    32,    59,     3,    39,    65,    36,
      20,    38,    34,     3,    37,    24,    74,    40,    72,    41,
      70,    72,    53,    60,    61,    62,     3,    37,    40,    37,
      71,    72,    34,    47,     3,    63,    64,    38,    62,     3,
      75,    76,    77,    57,    30,    31,    33,    29,     3,    33,
      70,    38,    77,    38,    35,    35,    65,    33,    33,     3,
      25,    49,    50,    48,    50,    35,    67,    34,    36,    34,
      36,    36,    53,    68,    69,    70,    50,    50,    34,    63,
      34,    36,    33,    30,     3,    69,    35,    65,    49,    36
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
        case 3:
#line 128 "mofc.y"
    {
#ifndef ONEPASS
    fix_forward_decls(cls_chain_current);
#endif
  }
    break;

  case 6:
#line 140 "mofc.y"
    {
               add_qual_list(qual_chain_current,(yyvsp[0].lval_qual));
             }
    break;

  case 7:
#line 144 "mofc.y"
    {
               add_class_list(cls_chain_current,(yyvsp[0].lval_class));
             }
    break;

  case 8:
#line 148 "mofc.y"
    {
               add_class_list(inst_chain_current,(yyvsp[0].lval_class));
             }
    break;

  case 9:
#line 160 "mofc.y"
    {
                         (yyval.lval_qual) = make_qualifier_definition(current_qualtab, (yyvsp[-6].lval_id), (yyvsp[-4].lval_type),
                                                   (yyvsp[-3].lval_literal),(yyvsp[-2].lval_vals),(yyvsp[-1].lval_qual_quals));
                       }
    break;

  case 10:
#line 167 "mofc.y"
    {
                             	(yyval.lval_qual_quals).scope = 0;
                             	(yyval.lval_qual_quals).flavor = 0;                             	
							 }
    break;

  case 11:
#line 172 "mofc.y"
    {
                             	(yyval.lval_qual_quals).scope = (yyvsp[-1].lval_int);
                             	(yyval.lval_qual_quals).flavor = 0;
                             }
    break;

  case 12:
#line 177 "mofc.y"
    {
                             	(yyval.lval_qual_quals).scope = 0;
                             	(yyval.lval_qual_quals).flavor = (yyvsp[-1].lval_int);                             
                             }
    break;

  case 13:
#line 182 "mofc.y"
    {
                             	(yyval.lval_qual_quals).scope = (yyvsp[-6].lval_int);
                             	(yyval.lval_qual_quals).flavor = (yyvsp[-1].lval_int);                       	
                             }
    break;

  case 14:
#line 189 "mofc.y"
    {
				(yyval.lval_int)=make_scope((yyvsp[0].lval_id));
			}
    break;

  case 15:
#line 193 "mofc.y"
    {
				(yyval.lval_int)|=make_scope((yyvsp[0].lval_id));			
			}
    break;

  case 16:
#line 199 "mofc.y"
    {
				(yyval.lval_int)=make_flavor((yyvsp[0].lval_id));
			}
    break;

  case 17:
#line 203 "mofc.y"
    {
				(yyval.lval_int)|=make_flavor((yyvsp[0].lval_id));
			}
    break;

  case 20:
#line 220 "mofc.y"
    { (yyval.lval_class) = make_class(current_symtab,(yyvsp[-7].lval_quals),(yyvsp[-5].lval_id),(yyvsp[-4].lval_class),(yyvsp[-2].lval_props)); }
    break;

  case 21:
#line 227 "mofc.y"
    {
                   	 (yyval.lval_class) = make_instance(current_symtab,(yyvsp[-8].lval_quals),(yyvsp[-5].lval_id),(yyvsp[-2].lval_props));
                   	 add_class_list(cls_chain_current,get_class_def(current_symtab,(yyvsp[-5].lval_id)));
                   }
    break;

  case 22:
#line 233 "mofc.y"
    {(yyval.lval_quals)=NULL;}
    break;

  case 23:
#line 234 "mofc.y"
    {(yyval.lval_quals)=(yyvsp[-1].lval_quals);}
    break;

  case 24:
#line 237 "mofc.y"
    {(yyval.lval_quals)=(yyvsp[0].lval_quals);}
    break;

  case 25:
#line 238 "mofc.y"
    {qualifier_list_add((yyvsp[-2].lval_quals),(yyvsp[0].lval_quals));}
    break;

  case 26:
#line 242 "mofc.y"
    {(yyval.lval_quals)=make_qualifier(current_qualtab,(yyvsp[-1].lval_id),(yyvsp[0].lval_vals));}
    break;

  case 27:
#line 245 "mofc.y"
    {(yyval.lval_vals)=NULL;}
    break;

  case 28:
#line 246 "mofc.y"
    {(yyval.lval_vals)=make_value_list((yyvsp[-1].lval_literal));}
    break;

  case 29:
#line 247 "mofc.y"
    {(yyval.lval_vals)=(yyvsp[-1].lval_vals);}
    break;

  case 30:
#line 250 "mofc.y"
    {(yyval.lval_vals)=NULL;}
    break;

  case 31:
#line 251 "mofc.y"
    {(yyval.lval_vals)=(yyvsp[0].lval_vals);}
    break;

  case 32:
#line 254 "mofc.y"
    {(yyval.lval_vals)=make_value_list((yyvsp[0].lval_literal));}
    break;

  case 33:
#line 255 "mofc.y"
    {value_list_add((yyvsp[-2].lval_vals),(yyvsp[0].lval_literal));}
    break;

  case 34:
#line 258 "mofc.y"
    {(yyval.lval_class)=NULL;}
    break;

  case 35:
#line 259 "mofc.y"
    {(yyval.lval_class)=get_class_def(current_symtab,(yyvsp[0].lval_id));}
    break;

  case 36:
#line 262 "mofc.y"
    {(yyval.lval_props)=NULL;}
    break;

  case 37:
#line 264 "mofc.y"
    {(yyval.lval_props)=(yyvsp[0].lval_props);}
    break;

  case 38:
#line 267 "mofc.y"
    {(yyval.lval_props)=(yyvsp[0].lval_props);}
    break;

  case 39:
#line 270 "mofc.y"
    {pom_list_add((yyvsp[-1].lval_props),(yyvsp[0].lval_props));}
    break;

  case 40:
#line 279 "mofc.y"
    {(yyval.lval_props)=make_pom_list((yyvsp[-6].lval_quals),(yyvsp[-5].lval_type),(yyvsp[-4].lval_id),(yyvsp[-3].lval_literal),(yyvsp[-2].lval_params),(yyvsp[-1].lval_vals));}
    break;

  case 41:
#line 282 "mofc.y"
    {(yyval.lval_type)=(yyvsp[0].lval_type);}
    break;

  case 42:
#line 283 "mofc.y"
    {(yyval.lval_type)=make_ref_type(current_symtab,(yyvsp[-1].lval_id));}
    break;

  case 43:
#line 286 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeSINT8; /* must be a smarter way */}
    break;

  case 44:
#line 287 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeUINT8;}
    break;

  case 45:
#line 288 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeSINT16;}
    break;

  case 46:
#line 289 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeUINT16;}
    break;

  case 47:
#line 290 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeSINT32;}
    break;

  case 48:
#line 291 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeUINT32;}
    break;

  case 49:
#line 292 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeSINT64;}
    break;

  case 50:
#line 293 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeUINT64;}
    break;

  case 51:
#line 294 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeREAL32;}
    break;

  case 52:
#line 295 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeREAL64;}
    break;

  case 53:
#line 296 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeSTRING;}
    break;

  case 54:
#line 297 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeCHAR16;}
    break;

  case 55:
#line 298 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeBOOLEAN;}
    break;

  case 56:
#line 299 "mofc.y"
    {(yyval.lval_type).type_base = BaseTypeDATETIME;}
    break;

  case 57:
#line 302 "mofc.y"
    {(yyval.lval_literal)=NULL;}
    break;

  case 58:
#line 303 "mofc.y"
    {(yyval.lval_literal)="0"; /*unspecified length*/}
    break;

  case 59:
#line 304 "mofc.y"
    {(yyval.lval_literal)=(yyvsp[-1].lval_literal);}
    break;

  case 60:
#line 307 "mofc.y"
    {(yyval.lval_literal)=NULL;}
    break;

  case 61:
#line 308 "mofc.y"
    {(yyval.lval_literal)=(yyvsp[0].lval_literal);}
    break;

  case 62:
#line 311 "mofc.y"
    {(yyval.lval_params)=NULL;}
    break;

  case 63:
#line 312 "mofc.y"
    {(yyval.lval_params)=make_param_list(NULL,(type_type)0,NULL,NULL);}
    break;

  case 64:
#line 313 "mofc.y"
    {(yyval.lval_params)=(yyvsp[-1].lval_params);}
    break;

  case 65:
#line 316 "mofc.y"
    {(yyval.lval_params)=(yyvsp[0].lval_params);}
    break;

  case 66:
#line 317 "mofc.y"
    {param_list_add((yyvsp[-2].lval_params),(yyvsp[0].lval_params));}
    break;

  case 67:
#line 321 "mofc.y"
    {(yyval.lval_params)=make_param_list((yyvsp[-3].lval_quals),(yyvsp[-2].lval_type),(yyvsp[-1].lval_id),(yyvsp[0].lval_literal));}
    break;

  case 68:
#line 324 "mofc.y"
    {(yyval.lval_vals)=NULL;}
    break;

  case 69:
#line 325 "mofc.y"
    {(yyval.lval_vals) = (yyvsp[0].lval_vals);}
    break;

  case 70:
#line 328 "mofc.y"
    {(yyval.lval_vals)=make_value_list((yyvsp[0].lval_literal));}
    break;

  case 71:
#line 329 "mofc.y"
    {(yyval.lval_vals)=(yyvsp[-1].lval_vals);}
    break;

  case 74:
#line 334 "mofc.y"
    {(yyval.lval_literal)=make_char((yyvsp[0].lval_literal));}
    break;

  case 78:
#line 340 "mofc.y"
    {(yyval.lval_literal)=make_string((yyvsp[0].lval_literal));}
    break;

  case 79:
#line 341 "mofc.y"
    {(yyval.lval_literal)=append_string((yyvsp[-1].lval_literal),make_string((yyvsp[0].lval_literal)));}
    break;

  case 82:
#line 348 "mofc.y"
    {(yyval.lval_props)=NULL;}
    break;

  case 83:
#line 350 "mofc.y"
    {
								(yyval.lval_props)=(yyvsp[0].lval_props);
							}
    break;

  case 84:
#line 356 "mofc.y"
    {
							(yyval.lval_props)=(yyvsp[0].lval_props);
						}
    break;

  case 85:
#line 360 "mofc.y"
    {
							pom_list_add((yyvsp[-1].lval_props),(yyvsp[0].lval_props));
						}
    break;

  case 86:
#line 366 "mofc.y"
    {
							(yyval.lval_props)=make_pom_list(NULL,(type_type)0,(yyvsp[-2].lval_id),NULL,NULL,(yyvsp[-1].lval_vals));
						}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 1726 "mofc.c"

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



