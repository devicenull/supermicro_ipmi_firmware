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
/* Line 1403 of yacc.c.  */
#line 232 "sqlParser.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



