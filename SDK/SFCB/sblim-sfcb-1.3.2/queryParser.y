/*
**==============================================================================
**
** Includes
**
**==============================================================================
*/


%{

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

%}

/*
**==============================================================================
**
** Union used to pass tokens from Lexer to this Parser.
**
**==============================================================================
*/

%union
{
   long intValue;
   double doubleValue;
   char* strValue;
   void* nodeValue;
   
   QLOperand* fullPropertyName;
   QLOperand* propertyName;
   QLOperand* comparisonTerm;
   QLOperation* searchCondition;
   QLOperation* predicate;
}

/*
**==============================================================================
**
** Tokens, types, and associative rules.
**
**==============================================================================
*/

%token <intValue> TOK_INTEGER
%token <doubleValue> TOK_DOUBLE

%token <strValue> TOK_STRING
%token <intValue> TOK_TRUE
%token <intValue> TOK_FALSE
%token <intValue> TOK_NULL

%token <intValue> TOK_EQ
%token <intValue> TOK_NE
%token <intValue> TOK_LT
%token <intValue> TOK_LE
%token <intValue> TOK_GT
%token <intValue> TOK_GE

%token <intValue> TOK_NOT
%token <intValue> TOK_OR
%token <intValue> TOK_AND
%token <intValue> TOK_IS
%token <intValue> TOK_ISA
%token <intValue> TOK_SCOPE

%token <strValue> TOK_IDENTIFIER
%token <intValue> TOK_SELECT
%token <intValue> TOK_WHERE
%token <intValue> TOK_FROM

%token <intValue> TOK_UNEXPECTED_CHAR

%type <propertyName>  classPropertyName
%type <fullPropertyName>  fullPropertyName
//%type <strValue>  propertyIdentifier
%type <strValue> classAlias

%type <comparisonTerm>  classId
%type <comparisonTerm>  comparisonTerm
%type <searchCondition> searchCondition
%type <whereClause> whereClause

%type <predicate> predicate
%type <predicate> comparisonPredicate
%type <predicate> nullPredicate

%type <strValue> propertyName
%type <nodeValue> propertyList
%type <nodeValue> fromClause
%type <nodeValue> selectStatement
%type <nodeValue> selectList
%type <nodeValue> selectExpression
%type <strValue> className
%type <intValue> truthValue

%left TOK_OR
%left TOK_AND
%nonassoc TOK_NOT

%%

/*
**==============================================================================
**
** The grammar itself.
**
**==============================================================================
*/

start
    : selectStatement
    {
    }

selectStatement
    : TOK_SELECT selectList selectExpression
    {

    }

selectList
    : '*'
    {
       QS->ft->setAllProperties(QS,1);
    }
    | propertyList
    {

    }

propertyList
    : propertyName
    {
       QS->ft->appendSelectPropertyName(QS,qsStrDup(QS,$1));
    }
    | propertyList ',' propertyName
    {
       QS->ft->appendSelectPropertyName(QS,qsStrDup(QS,$3));
    }

selectExpression
    : fromClause whereClause
    {

    }
    | fromClause
    {
    }

fromClause
    : TOK_FROM classList
    {
    }

classList
    : classListEntry
    {
    }
    | classList ',' classListEntry
    {
    }
   
classListEntry 
    : className classAlias
    {
       QS->ft->addFromClass(QS,qsStrDup(QS,$1),$2);
    }
    | className
    {
        QS->ft->addFromClass(QS,qsStrDup(QS,$1),NULL);
    }
    
whereClause
    : TOK_WHERE searchCondition
    {
       QS->ft->setWhereCondition(QS,(QLOperation*)$2);
    }

searchCondition
    : searchCondition TOK_OR searchCondition
    {
       $$=newOrOperation(QS,$1,$3);
       QL_TRACE(fprintf(stderr,"- - searchCondition: %p (%p-%p)\n",$$,$1,$3));
    }
    | searchCondition TOK_AND searchCondition
    {
       $$=newAndOperation(QS,$1,$3);

    }
    | TOK_NOT searchCondition
    {
       $$=newNotOperation(QS,$2);
    }
    | '(' searchCondition ')'
    {
       $$=$2;
    }
    | predicate
    {
       $$=newBinaryOperation(QS,$1);
    }
    | predicate TOK_IS truthValue
    {
       if (QS->lang==QL_WQL) {
          if ($3);
          else $1->ft->eliminateNots($1,1);
          $$=newBinaryOperation(QS,$1);
      }
       else {
          yyErr("Three state logic not supported: IS ",$3?"TRUE":"FALSE","");
          YYERROR;
       }
    }
    | predicate TOK_IS TOK_NOT truthValue
    {
       if (QS->lang==QL_WQL) {
          if ($4) $1->ft->eliminateNots($1,1);
           $$=newBinaryOperation(QS,$1);
      }
       else {
          yyErr("Three state logic not supported: IS NOT ",$4?"TRUE":"FALSE","");
          YYERROR;
       }
    }

truthValue
    : TOK_TRUE
    {
       $$ = 1;
    }
    | TOK_FALSE
    {
       $$ = 0;
    }


/******************************************************************************/

predicate
    : comparisonPredicate
    {
       $$=$1;
    }
    | nullPredicate
    {
       $$=$1;
    }

comparisonPredicate
    : comparisonTerm TOK_LT comparisonTerm
    {
       $$=newLtOperation(QS,$1,$3);
    }
    | comparisonTerm TOK_GT comparisonTerm
    {
       $$=newGtOperation(QS,$1,$3);
    }
    | comparisonTerm TOK_LE comparisonTerm
    {
       $$=newLeOperation(QS,$1,$3);
    }
    | comparisonTerm TOK_GE comparisonTerm
    {
       $$=newGeOperation(QS,$1,$3);
    }
    | comparisonTerm TOK_EQ comparisonTerm
    {
       $$=newEqOperation(QS,$1,$3);
       QL_TRACE(fprintf(stderr,"- - comparisonPredicate: %p (%p-%p)\n",$$,$1,$3));
    }
    | comparisonTerm TOK_NE comparisonTerm
    {
       $$=newNeOperation(QS,$1,$3);
    }
    | comparisonTerm TOK_ISA classId
    {
       $$=newIsaOperation(QS,$1,$3);
    }

nullPredicate
    : comparisonTerm TOK_IS TOK_NULL
    {
       $$=newIsNullOperation(QS,$1);
    }
    | comparisonTerm TOK_IS TOK_NOT TOK_NULL
    {
       $$=newIsNotNullOperation(QS,$1);
    }

initPropName
    : /*emtpty */
    {
       QL_TRACE(fprintf(stderr,"### initPropName\n"));
       QC->propName=NULL;
    }
    
classPropertyName 
    : classPropertyName '.' fullPropertyName
    {
       QL_TRACE(fprintf(stderr,"-#- classPropertyName: classPropertyName '.' fullPropertyName\n"));
    }
    | fullPropertyName
    {
       QL_TRACE(fprintf(stderr,"-#- classPropertyName: fullPropertyName\n"));
    }
    | builtInFunction
    {
    }
    
    
fullPropertyName
    : TOK_IDENTIFIER
    {
       QL_TRACE(fprintf(stderr,"-#- fullPropertyName: propertyIdentifier  %s\n",$1));
       QC->addPropIdentifier(QC,QS,NULL,qsStrDup(QS,$1),-99999);
    }
    | TOK_IDENTIFIER TOK_SCOPE TOK_IDENTIFIER  
    {
       QL_TRACE(fprintf(stderr,"-#- fullPropertyName: propertyClassIdentifier TOK_SCOPE propertyIdentifier %s::%s\n",$1,$3));
       if (QS->lang==QL_CQL)
          QC->addPropIdentifier(QC,QS,qsStrDup(QS,$1),qsStrDup(QS,$3),-99999);
       else {
          yyErr("CQL style property not supported with language=wql","","");
          YYERROR;
       }
    }
    
propertyName
    : TOK_IDENTIFIER
    {
       $$=$1;
    }

className : TOK_IDENTIFIER
    {
       $$ = $1;
    }
    
classAlias : TOK_IDENTIFIER
    {
       $$ = $1;
    }


classId
    : className
    {
       $$=newNameQueryOperand(QS,qsStrDup(QS,$1));
    }

comparisonTerm
    : initPropName classPropertyName
    {
       $$=newPropQueryOperand(QS,QC->propName);
       QC->propName=NULL;
    }
    | TOK_INTEGER
    {
       $$=newIntQueryOperand(QS,$1);
    }
    | TOK_DOUBLE
    {
       $$=newDoubleQueryOperand(QS,$1);
    }
    | TOK_STRING
    {
       $$=newCharsQueryOperand(QS,qsStrDup(QS,$1));
    }
    | truthValue
    {
       $$=newBooleanQueryOperand(QS,$1);
    }    

functionArg
    :  '(' classPropertyName ')'
    
    |  '(' TOK_STRING ')'
    
    |  '(' ')'
        
builtInFunction
    :  TOK_IDENTIFIER functionArg    
    {
       int fnc=QL_FNC_NoFunction;
       if (QS->lang==QL_CQL) {
          if (strcasecmp($1,"classname")==0) fnc=QL_FNC_Classname;
          else if (strcasecmp($1,"namespacename")==0) fnc=QL_FNC_Namespacename;
          else if (strcasecmp($1,"namespacetype")==0) fnc=QL_FNC_Namespacetype;
          else if (strcasecmp($1,"hostport")==0) fnc=QL_FNC_Hostport;
          else if (strcasecmp($1,"modelpath")==0) fnc=QL_FNC_Modelpath;
          else if (strcasecmp($1,"classpath")==0) fnc=QL_FNC_Classpath;
          else if (strcasecmp($1,"objectpath")==0) fnc=QL_FNC_Objectpath;
          else if (strcasecmp($1,"instancetoreference")==0) fnc=QL_FNC_InstanceToReference;
          else if (strcasecmp($1,"currentdatetime")==0) fnc=QL_FNC_CurrentDateTime;
          else if (strcasecmp($1,"datetime")==0) fnc=QL_FNC_DateTime;
          else if (strcasecmp($1,"microsecondtotimestamp")==0) fnc=QL_FNC_MicrosecondsToTimestamp;
          else if (strcasecmp($1,"microsecondtointerval")==0) fnc=QL_FNC_MicrosecondsToInterval;
          else  {
             yyErr("Function ",$1," not supported");
             YYERROR;
          }
       }
       else {
          yyErr("Functions not supported when language != cql","","");
          YYERROR;
       }
    }
        
%%
