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
/* Line 1403 of yacc.c.  */
#line 101 "queryParser.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE sfcQuerylval;



