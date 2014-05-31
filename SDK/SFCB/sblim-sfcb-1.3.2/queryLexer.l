%option never-interactive
%{

/*
 * queryLexer.l
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 * Based on concepts from pegasus project 
 *
 * Description:
 *
 * WQL/CQL query lexer for sfcb.
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


extern int queryInput(char* buffer, int* numRead, int numRequested);
extern int yyErr(const char*);
extern char * yyStrcpy(char *txt, int len);

#define YY_INPUT(BUF, NREAD, NREQUESTED) queryInput(BUF, &NREAD, NREQUESTED)

#include "queryOperation.h"
#include "queryParser.h"
#include <stdio.h>
#include <string.h>

%} 

POSITIVE_DECIMAL_DIGIT [1-9]
DECIMAL_DIGIT [0-9]
BLANK [ \t\n]
IDENT_CHAR [A-Za-z_]

%option nounput

%%

[Ss][Ee][Ll][Ee][Cc][Tt] {

    return TOK_SELECT;
}

[Ff][Rr][Oo][Mm] {

    return TOK_FROM;
}

[Ww][Hh][Ee][Rr][Ee] {

    return TOK_WHERE;
}

[Tt][Rr][Uu][Ee] {

    return TOK_TRUE;
}

[Ff][Aa][Ll][Ss][Ee] {

    return TOK_FALSE;
}

[Nn][Uu][Ll][Ll] {

    return TOK_NULL;
}

[Nn][Oo][Tt] {

    return TOK_NOT;
}

[Aa][Nn][Dd] {

    return TOK_AND;
}

[Oo][Rr] {

    return TOK_OR;
}

[Ii][Ss][Aa] {

    return TOK_ISA;
}

[Ii][Ss] {

    return TOK_IS;
}

[-+]?{POSITIVE_DECIMAL_DIGIT}{DECIMAL_DIGIT}* {

    yylval.intValue = strtol(yytext, (char**)0, 10);
    return TOK_INTEGER;
}

[+-]?0 {

    yylval.intValue = 0;
    return TOK_INTEGER;
}

[-+]?{DECIMAL_DIGIT}*\.{DECIMAL_DIGIT}+([eE][+-]?{DECIMAL_DIGIT}+)? {

    yylval.doubleValue = strtod((char*)yytext, (char**)0);
    return TOK_DOUBLE;
}

\'[^\'\n]*\' {

    {
       size_t n = strlen(yytext) - 2;
       yylval.strValue=yyStrcpy(yytext+1,n);
    }

    return TOK_STRING;
}

\'[^\'\n]*$ {

    yyErr("Unterminated string");
}

\"[^\"\n]*\" {

    {
       size_t n = strlen(yytext) - 2;
       yylval.strValue=yyStrcpy(yytext+1,n);
    }

    return TOK_STRING;
}

\"[^\"\n]*$ {

    yyErr("Unterminated string");
}

[\*(),.] { 

    return yytext[0];
}

"=" { 
    return TOK_EQ; 
}

"!=" { 

    return TOK_NE; 
}


"<>" { 
    return TOK_NE; 
}

"<=" { 
    return TOK_LE; 
}

"<" { 
    return TOK_LT; 
}

">=" { 
    return TOK_GE; 
}

">" { 
    return TOK_GT; 
}

"::" { 
    return TOK_SCOPE; 
}

{IDENT_CHAR}({IDENT_CHAR}|{DECIMAL_DIGIT})*  {

    {
	size_t n = strlen(yytext);
	yylval.strValue=yyStrcpy(yytext,n);
    }

    return TOK_IDENTIFIER;
}

{BLANK}+ {
    /* Ignore blanks */
}

. {
    yylval.intValue = 0;
    return TOK_UNEXPECTED_CHAR;
}

%%

int yywrap()
{
    return 1;
}
