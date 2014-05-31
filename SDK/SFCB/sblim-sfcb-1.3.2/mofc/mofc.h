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
/* Line 1403 of yacc.c.  */
#line 114 "mofc.h"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



