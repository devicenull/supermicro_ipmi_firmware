/*
**==============================================================================
**
** Includes
**
**==============================================================================
*/


%{
	
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
}

/*
**==============================================================================
**
** Tokens, types, and associative rules.
**
**==============================================================================
*/

%error-verbose
//%pure-parser //wozu das??? dagegen spricht: yyval ist dann nicht mehr verwendbar. yacc statt bison zu verwenden

%token <intValue> TOK_INTEGER
%token <doubleValue> TOK_DOUBLE

%token <strValue> TOK_STRING
%token <intValue> TOK_CONCAT
%token <intValue> TOK_TRUE
%token <intValue> TOK_FALSE
%token <intValue> TOK_NULL

%token <strValue> TOK_TSTAMPDT
%token <strValue> TOK_TIMEDT
%token <strValue> TOK_DATEDT
%token <strValue> TOK_VCHARDT
%token <strValue> TOK_CHARDT
%token <strValue> TOK_DECDT
%token <strValue> TOK_DOUBLEDT
%token <strValue> TOK_REALDT
%token <strValue> TOK_BINDT
%token <strValue> TOK_INTDT
%token <strValue> TOK_SINTDT
%token <strValue> TOK_BINTDT

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

%token <intValue> TOK_ALL
%token <intValue> TOK_CALL
%token <intValue> TOK_DISTINCT
%token <intValue> TOK_AS

%token <intValue> TOK_IN
%token <intValue> TOK_LIKE
%token <intValue> TOK_SOME
%token <intValue> TOK_ANY
%token <intValue> TOK_EXISTS
%token <intValue> TOK_BETWEEN

%token <intValue> TOK_GROUP
%token <intValue> TOK_FETCH
%token <intValue> TOK_ONLY
%token <intValue> TOK_ASC
%token <intValue> TOK_DESC
%token <intValue> TOK_BY
%token <intValue> TOK_ROW
%token <intValue> TOK_ORDER
%token <intValue> TOK_FIRST
%token <intValue> TOK_HAVING

%token <intValue> TOK_UNION
%token <intValue> TOK_EXCEPT
%token <intValue> TOK_INTERSECT
%token <intValue> TOK_UNIONALL
%token <intValue> TOK_EXCEPTALL
%token <intValue> TOK_INTERSECTALL

%token <intValue> TOK_JOIN
%token <intValue> TOK_ON
%token <intValue> TOK_INNER
%token <intValue> TOK_LEFT
%token <intValue> TOK_RIGHT
%token <intValue> TOK_FULL

%token <intValue> TOK_WITH

%token <intValue> TOK_DROP 
%token <intValue> TOK_TABLE
%token <intValue> TOK_ALTER
%token <intValue> TOK_CREATE

%token <intValue> TOK_INSERT
%token <intValue> TOK_INTO
%token <intValue> TOK_VALUES
%token <intValue> TOK_DEFAULT

%token <intValue> TOK_DELETE

%token <intValue> TOK_UPDATE
%token <intValue> TOK_SET

%token <intValue> TOK_KEY
%token <intValue> TOK_FOREIGN
%token <intValue> TOK_UNIQUE
%token <intValue> TOK_PRIMARY
%token <intValue> TOK_CONSTRAINT
%token <intValue> TOK_REFERENCES
%token <intValue> TOK_CHECK


%token <strValue> TOK_IDENTIFIER
%token <strValue> TOK_IDENTIFIER2
%token <intValue> TOK_SELECT
%token <intValue> TOK_WHERE
%token <intValue> TOK_FROM

%token <intValue> TOK_UNEXPECTED_CHAR
%left TOK_OR
%left '+'
%left '-'
%left '*'
%left '/'
%left TOK_AND
%left TOK_CONCAT
%nonassoc TOK_NOT


%type <fullSelect> fullSelect
%type <subSelect> subselect
%type <projection> selectClause
%type <intValue> OPT_ALL_DIST
%type <list> rowList
%type <intValue> isKey
%type <intValue> dataType
%type <strValue> expression
%type <strValue> expressionElement
%type <strValue> OPT_AS
%type <classDef> tableName
%type <strValue> correlationClause
%type <subSelect> tableReference 
%type <subSelect> fromClause
%type <intValue> fetchFirstClause
%type <intValue> OPT_ASC_DESC
%type <strValue> number
%type <intValue> comparison
%type <intValue> optNot
%type <sigma> limitedPredicate
%type <sigma> predicate
//%type <sigma> enhPredicate
%type <sigma> optCondition
%type <column> colIdentifier
%type <intValue> joinType
%type <exl> expressionlight
%type <strValue> procedureName
%%



start:
	/*subselect  { printf("subselect abgearbeitet\n");return 0;}
	| */
	 
{	//printf("parsen...\n");
	 }
	  fullSelect  {
  //printf("fullSelect abgearbeitet\n");
		RS->sw->setWarning(RS->sw,"00000","Successful Completion");
		return 0;
	  }
	| select  {printf("select abgearbeitet\n");return 0;}
	| dropTable {
	  //printf("drop abgearbeitet\n");
		RS->sw->setWarning(RS->sw,"00000","Successful Completion");
		return 0;
	}
	| createTable {printf("create abgearbeitet\n");return 0;}
	| alterTable  {printf("alter abgearbeitet\n");return 0;}
	| insert {printf("insert abgearbeitet\n");return 0;}
	| delete {printf("delete abgearbeitet\n");return 0;}
	| update {printf("update abgearbeitet\n");return 0;}
	| call {printf("call abgearbeitet\n");return 0;}
	| error '$' {return 1;}/* WICHTIG: Löscht die komplette Eingabe, so dass beim nächsten Aufruf die neue Query bearbeitet werden kann!!!*/	
;	

call:
	TOK_CALL tableName '.' procedureName {
	  ClassDef * cd = (ClassDef*)$2; 
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
	  Call * call = newCall(cd->className,$4,assignmentList,whereList);
	  STM->addNode(STM,CALL,call);	
	} '(' argList ')'
;

procedureName:
	TOK_IDENTIFIER {
	  //muss validiert werden, sobald die Funktionalität im sfcb implementiert ist
	  $$=$1;
	}
;

argList:
	kList |
	kList ',' pList
	;
kList:	
	kpair 
	| kList ',' kpair
;

kpair:
	//select distinct Priority from linux_unixprocess fetch first 5 rows only;
	//select  Priority from linux_unixprocess where Priority=24;
	//select distinct Priority from linux_unixprocess where Priority<24 intersect
	//select  distinct Priority from linux_unixprocess where Priority=23;


	 '\'' TOK_IDENTIFIER TOK_EQ TOK_STRING '\''{
	  ExpressionLight * el;
	  Column* col;
	  int i, found;
	  found = 0;
	  //printf("und los %d\n", CMPI_string);
	  //printf(columnList->ft->);
	  for(i=0,col = (Column*)columnList->ft->getFirst(columnList);col;col = (Column*)columnList->ft->getNext(columnList),i++){
	    if(strcasecmp(col->colName,$2)==0){
	      found = 1;
	      //printf("::: %s %s\n",$2,$4);
	      el = newExpressionLight($2,UNDEF,$4);
	      el->sqltype = col->colSQLType;
	      whereList->ft->append(whereList,el);
	      break;
	    }
	  }
	  if(found==0){
	    yyerror(semanticError("UNDIFINED PARAMETER:\nColumn \"",$2,"\" is not part of the Key"));STM->rs->sw->sqlstate = "42P02";YYERROR;
	  }
	 
	}
pList:  
	ptripel
	| pList ',' ptripel 
;

ptripel: 
	 '\'' TOK_IDENTIFIER  ':' TOK_STRING ':' TOK_INTEGER '\''  {
	  ExpressionLight * el;
	  el = newExpressionLight($2,UNDEF,$4);
	  el->sqltype = $6;
	  //printf("::: %s %s\n",$2,$4);
	  assignmentList->ft->append(assignmentList,el);
	}

update:
	TOK_UPDATE tableName {
		whereList = newList(); aliasList = newList();//die bleibt leer, muss aber initialisiert werden, damit searchcondition funktioniert
		allColumnsList = newList();
		assignmentList = newList();
		ClassDef* cd = (ClassDef*)$2;
		UtilList* ul = cd->fieldNameList;
		int i;
		for(i=0; i<cd->fieldCount; i++){
			Column * col = (Column*)ul->ft->getNext(ul);
			col->colAlias = col->colName;
			col->tableAlias = cd->className;
			allColumnsList->ft->append(allColumnsList,col);	
		}
		//printf("gefunden\n");
		
	} TOK_SET {
			//printf("set fertig...\n");
		} assignmentClause optDelWhere {
		//printf("fertig...\n");
		ClassDef* cd = (ClassDef*)$2;
		STM->addNode(STM,UPDATE,newUpdIns(cd->className,allColumnsList,assignmentList,whereList));	
	}
;

assignmentClause:
	assignment 
	| assignmentClause ',' assignment;
;
assignment:
	TOK_IDENTIFIER TOK_EQ expressionlight {
		//printf("assignment\n");
		ExpressionLight * exl = (ExpressionLight*)$3;
		if(exl->name==NULL)
			exl->name = $1;
		Column* col;
		Column* rcol = NULL;
		//printf("assignment2\n");
		for(col = (Column*)allColumnsList->ft->getFirst(allColumnsList);col;col = (Column*)allColumnsList->ft->getNext(allColumnsList)){
			if(strcasecmp(col->colName,$1)==0){
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
				yyerror(semanticError("DUPLICATE COLUMN:\nCannot change column \'",$1,"\' more then once in one statement"));STM->rs->sw->sqlstate = "42701";YYERROR;
			}
			
		}
		//printf("assignment3 %p\n",exl);
		
		if(rcol==NULL){
	  		
			yyerror(semanticError("UNDIFINED COLUMN:\nColumn \'",$1,"\' in assigmnet-clause does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}	
		
		if(exl->name!=NULL&&strcasecmp(rcol->colName,exl->name)!=0){
			
			yyerror(semanticError("ERROR IN ASSIGNMENT:\nOperation can only be applied to same column: \"",$3,"\" "));STM->rs->sw->sqlstate = "22005";YYERROR;
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
;
expressionlight://erlaubt operationen der Art +=,...
	TOK_NULL {$$ = newExpressionLight(NULL,ISNULL,NULL);}
	| TOK_IDENTIFIER '+' number {$$ = newExpressionLight($1,PLUS,$3);}
	| TOK_IDENTIFIER '-' number {$$ = newExpressionLight($1,SUB,$3);}
	| TOK_IDENTIFIER '*' number {$$ = newExpressionLight($1,MUL,$3);}
	| TOK_IDENTIFIER '/' number {$$ = newExpressionLight($1,DIV,$3);}
	| TOK_CONCAT TOK_STRING {$$ = newExpressionLight($1,CONCAT,$2);}
	| TOK_STRING {$$ = newExpressionLight(NULL,ASS,$1);}
	| number {$$ = newExpressionLight(NULL,ASN,$1);}
;

delete:
	TOK_DELETE TOK_FROM tableName  {
		whereList = newList(); aliasList = newList();//die bleibt leer, muss aber initialisiert werden, damit searchcondition funktioniert
		allColumnsList = newList();
		assignmentList = newList();
		ClassDef* cd = (ClassDef*)$3;
		UtilList* ul = cd->fieldNameList;
		int i;
		for(i=0; i<cd->fieldCount; i++){
			Column * col = (Column*)ul->ft->getNext(ul);
			col->colAlias = col->colName;
			col->tableAlias = cd->className;
			allColumnsList->ft->append(allColumnsList,col);	
		}
		//printf("gefunden\n");
		
	} optDelWhere {
		//printf("fertig... \n");
		ClassDef* cd = (ClassDef*)$3;
		//printf("fertig... %s\n",cd->className);
		
		STM->addNode(STM,DELETE,newUpdIns(cd->className,allColumnsList,NULL,whereList));	
	}
;

optDelWhere:						
	/* empty */					/* delete or update all tupels */
	| TOK_WHERE searchCondition
;

insert:
	TOK_INSERT TOK_INTO tableName {
		//printf("### 1\n");
		allColumnsListIns = newList();
		assignmentList = newList();
		ClassDef* cd = (ClassDef*)$3;
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
	optColName valuesOrSelect {
				
	}
;

optColName:
	/* empty */ //die Werte werden entsprechend der Reihenfolge in der colList eingetragen. In der Hoffnung, dass diese deterministisch zurückgeliefert wird
	{
		Column* col;
		for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
			assignmentList->ft->append(assignmentList,newExpressionLight(col->colName,UNDEF,NULL));		
			
		}
	}
	| '(' colList ')' {
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
;

colList:
	colListIDENTIFIER
	| colList ',' colListIDENTIFIER
;
colListIDENTIFIER:
	TOK_IDENTIFIER {
		Column* rcol = NULL;
		Column* col;
		for(col = (Column*)allColumnsListIns->ft->getFirst(allColumnsListIns);col;col = (Column*)allColumnsListIns->ft->getNext(allColumnsListIns)){
			if(strcasecmp(col->colName,$1)==0){
				rcol = col;
			}
			if(rcol!=NULL)
				break;
		}
		
		if(rcol==NULL){
			yyerror(semanticError("UNDEFINED COLUMN:\nColumn \'",$1,"\' in colList-clause of INSERT-Statement does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}
	  	ExpressionLight * el;
		for(el = (ExpressionLight*)assignmentList->ft->getFirst(assignmentList);el;el = (ExpressionLight*)assignmentList->ft->getNext(assignmentList)){
			if(strcasecmp(el->name,$1)==0){
				yyerror(semanticError("DUPLICATED COLUMN:\nColumn \'",$1,"\' indicated twice"));STM->rs->sw->sqlstate = "42701";YYERROR;
			}
		}
		assignmentList->ft->append(assignmentList,newExpressionLight($1,UNDEF,NULL));	
	}
;
valuesOrSelect:
	fullSelect {
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
	| TOK_VALUES tupelList
;
//
tupelList:
	tupelList ',' '(' tupel ')' {
		STM->addNode(STM,INSTUPEL,newUpdIns(NULL,allColumnsListIns,assignmentList,NULL));
		//printf("%p\n",assignmentList);
		assignmentList = cloneAL();	
	}
	| '(' tupel ')' {
		STM->addNode(STM,INSTUPEL,newUpdIns(NULL,allColumnsListIns,assignmentList,NULL));
		//printf("%p\n",assignmentList);
		assignmentList = cloneAL();	
	}
;

tupel:
	val
	| tupel ',' val
;

val:
	/*expression
	| TOK_NULL
	| TOK_DEFAULT
	*/ //es gibt keine default und keine NULL-Werte für Properties in CIM. Und Expressions unterstütze ich nicht also:
	| TOK_INTEGER {
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
		int ii = sprintf(buf,"%d",$1);
		char *tmp = (char*) calloc(1,ii+1);
		strncpy(tmp,buf,ii);
	  	el->value = tmp;
	  	el->sqltype = col->colSQLType; 
	  	//printf("--2- %s\n",el->value);
	}
	| TOK_DOUBLE {
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
		int ii = sprintf(buf,"%f",$1);
		char *tmp = (char*) calloc(1,ii+1);
		strncpy(tmp,buf,ii);
	  	el->sqltype = col->colSQLType; 
	  	el->value = tmp; 
	}
	| TOK_STRING {
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
	  	el->value = $1; 
	}
;

dropTable: // was ist mit ref, vererbung??? cascade loesung? oder verbieten???
	TOK_DROP TOK_TABLE tableName {
		ClassDef* cd = (ClassDef*)$3;
		STM->addNode(STM,DROP,cd->className);	
	}
;

createTable:
	TOK_CREATE TOK_TABLE TOK_IDENTIFIER '(' elementList ')'{
		//printf("suche: %s\n",$3);
		ClassDef * cd = getClassDef(STM->db,$3);
		if(cd!=NULL){	
			yyerror(semanticError("DUPLICATE TABLE:\nTable \"",$3,"\" does alrady exist"));STM->rs->sw->sqlstate = "42P07";YYERROR;
		}
		
		STM->addNode(STM,CREATE,newClassDef(0,$3,eList,0,NULL));
			  	
	  }
	| TOK_CREATE TOK_TABLE TOK_IDENTIFIER TOK_LIKE tableName copyOptions {
		//printf("suche: %s\n",$3);
		ClassDef* cd1 = (ClassDef*)$5;
		ClassDef * cd = getClassDef(STM->db,$3);
		if(cd!=NULL){	
			yyerror(semanticError("DUPLICATE TABLE:\nTable \"",$3,"\" does alrady exist"));STM->rs->sw->sqlstate = "42P07";YYERROR;
		}
		STM->addNode(STM,CREATE,newClassDef(0,$3,NULL,0,cd1->className));
	  }
;

elementList:
	element
	| elementList ',' element
;
element: 
	TOK_IDENTIFIER dataType isKey {//columnOptions {
		if(eList==NULL)
			eList=newList();
			//printf("DT: %d %d\n",$2,CMPI_real32);
		eList->ft->append(eList,newColumn(NULL,NULL, $1, NULL, $2,$3));//CMPI_real32
		//BUG!!! es tut offenbar nur CMPI_real32!!!
	}
/*	| uniqueConstraint {
		if(eList=NULL)
			eList=newList();	
	}
	| referentialConstraint {
		if(eList=NULL)
			eList=newList();	
	} 
	| checkConstraint {
		if(eList=NULL)
			eList=newList();	
	}*/
;	
copyOptions:
	/* empty */
;
isKey: 
	/* empty */ {$$=NKEY;}
	| TOK_KEY	{$$=KEY;}
;

dataType://nicht ganz ideal...
	  TOK_TSTAMPDT {$$ = CMPI_string;}
	| TOK_TIMEDT {$$ = CMPI_string;}
	| TOK_DATEDT {$$ = CMPI_dateTime;}
	| TOK_VCHARDT '(' TOK_INTEGER ')' {$$ = CMPI_chars;}
	| TOK_CHARDT '(' TOK_INTEGER ')' {$$ = CMPI_chars;}
	| TOK_DECDT '(' TOK_INTEGER ',' TOK_INTEGER ')' {$$ = CMPI_real64;}
	| TOK_DOUBLEDT {$$ = CMPI_real32;}
	| TOK_REALDT {$$ = CMPI_real32;}
	| TOK_BINDT {$$ = CMPI_string;}
	| TOK_INTDT {$$ = CMPI_sint32;}
	| TOK_SINTDT {$$ = CMPI_sint16;}
	| TOK_BINTDT {$$ = CMPI_sint64;}
;
/*
uniqueConstraint:
	/* empty *
	| OPT_CONSTRAINT OPT_UNIQUE_KEY '(' columnNameList ')'	
;/*

/*OPT_UNIQUE_KEY:
	TOK_UNIQUE
	| TOK_PRIMARY TOK_KEY
	;*/

/*OPT_CONSTRAINT:
	/* empty *
	| TOK_CONSTRAINT TOK_IDENTIFIER
;*/

/*referentialConstraint:
	/* empty *
	| OPT_CONSTRAINT TOK_FOREIGN TOK_KEY '(' columnNameList ')' referencesClause // rest gespart 
	;*/

/*referencesClause:
	TOK_REFERENCES TOK_IDENTIFIER '(' columnNameList ')'
	;*/

/*checkConstraint:
	/* empty *
	| OPT_CONSTRAINT TOK_CHECK '(' checkCondition ')'  // rest gespart 
	;*/

/*checkCondition:
	TOK_IDENTIFIER comparison TOK_IDENTIFIER
	| TOK_IDENTIFIER optNot TOK_BETWEEN TOK_IDENTIFIER TOK_AND TOK_IDENTIFIER
	| matchExpression optNot TOK_LIKE patternExpression // escape gespart 
	;*/
/*columnOptions: 
/* empty *
	| columnOptionElement	
	;*/
/*columnOptionElement: 
	  columnOptions TOK_NOT TOK_NULL
	| columnOptions OPT_CONSTRAINT columnConstraint	
	;*/

/*columnConstraint:
	TOK_PRIMARY TOK_KEY
	| TOK_UNIQUE
	| referencesClause
	| TOK_CHECK '(' checkCondition ')'
	;*/
alterTable:								/* wird nicht unterstützt! */
	
	TOK_ALTER {yyerror("FEATURE NOT SUPPORTED: ALTER");STM->rs->sw->sqlstate = "0A000";YYERROR;}
;

select:	
								/* wird nicht unterstützt! */
	TOK_WITH {yyerror("FEATURE NOT SUPPORTED: WITH");STM->rs->sw->sqlstate = "0A000";YYERROR;}
;
fullSelect:							/* Achtung: orderby und fetch first nur, wenn fullSelectOperation nicht leer!!! */
	
	{
		FullSelect* fs = newFullSelect();
		STM->addNode(STM,INITFUL,fs);
	}
	fullSelectClause
	fullSelectOperationList
	orderByClause
	fetchFirstClause {
		STM->addNode(STM,SIGMA,newSelection(obList,(int)$5));//$3 statt NULL
		//printf("fetchfirs:  %d\n",(int)$5);
		obList=NULL;	
	}
;

fullSelectClause: {
	columnList = newList();
	if(allColumnsList==NULL)
		allColumnsList = newList();
	aliasList = newList();
	whereList = newList();}
	subselect {
		//printf("subselect im fullSelect\n");
		
		//fs->setA = (SubSelect*)$1;
		//fs->typeA = SUB;
		
		int ret = STM->addNode(STM,SUB,(SubSelect*)$2);
		
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
	| '('  {
		//printf("(fullSelect) im fullSelect\n");
		FullSelect* fs = newFullSelect();
		//STM->addNode(STM,INITFUL,fs);
		STM->addNode(STM,FUL,fs);
	} 
	fullSelect ')' //{STM->addNode(STM,FINISHFS,NULL);}
;

fullSelectOperationList:
	/* empty */{
		//printf("EMPTY: fullSelectoperation\n");
		STM->addNode(STM,EMPTY,NULL);
	}
	| fullSelectOperation {
		//printf("fullSelectoperation\n");
		//nothing to do	
	}	
;

fullSelectOperation:
	fullSelectOperation  setOperation fullSelectClause
	| setOperation fullSelectClause
;

setOperation:
	TOK_UNION {
		//printf("UNION\n");
		STM->addNode(STM,UNION,NULL);	
	}
	| TOK_UNIONALL{
		//printf("UNION_ALL\n");
		STM->addNode(STM,UNIONALL,NULL);	
	}
	| TOK_EXCEPT{
		//printf("EXCEPT\n");
		STM->addNode(STM,EXCEPT,NULL);	
	}
	| TOK_EXCEPTALL{
		//printf("EXCEPTALL\n");
		STM->addNode(STM,EXCEPTALL,NULL);	
	}
	| TOK_INTERSECT{
		//printf("INTERSECT\n");
		STM->addNode(STM,INTERSECT,NULL);	
	}
	| TOK_INTERSECTALL{
		//printf("INTERSECTALL\n");
		STM->addNode(STM,INTERSECTALL,NULL);	
	}
;

subselect:
	selectClause fromClause 
	whereClause 					/* optional */
	groupByClause 					/* optional */
	havingClause					/* optional */
	orderByClause					/* optional */
	fetchFirstClause 				/* optional */{
		//printf("subselect erfolgreich fertig!!!\n");
		
		Projection* prj = (Projection*)$1;
		
		Selection* sigma = newSelection(obList,$7);
		SubSelect* sbs = (SubSelect*)$2;
		sbs->sigma = sigma;
		sbs->pi = prj;
		
		sbs->addWhereList(sbs,whereList);
		$$ = sbs;
		obList=NULL;	
		
	}
;

selectClause:						/* not empty */
	TOK_SELECT/* {
		//printf("SELECT eingegeben...\n");
		
	}*/
	OPT_ALL_DIST					/* optional */
	rowList{
		$$ = newProjection($2,(UtilList*)$3);
		//printf("rowList abgearbeitet\n");
	}
;
OPT_ALL_DIST:							/* may be empty */	
	/*empty*/{
		//printf("ALL_DIST: empty\n");
		$$ = ALL;	
	}
	| TOK_ALL {
		//printf("ALL_DIST: ALL\n");
		$$ = ALL;	
	}
	| TOK_DISTINCT {
		//printf("ALL_DIST: DISTINCT\n");
		$$ = DIST;
	}
;
rowList:
	'*' 
	{
		$$  = columnList = newList();
		clempty = 1;
		aliasList = newList();
		//printf("STERN %s\n",columnList);
	}
	| rowListElement {
		$$ = columnList;
		clempty = 0;
	}
;
	rowListElement: /*****************************************
					**  ACHTUNG, eventuell umsortieren!!!!  **
					******************************************/
		  rowListElement ',' TOK_IDENTIFIER '.' TOK_IDENTIFIER{
		  	 
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn($3,$3,$5,NULL,UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
		}
		  
		| TOK_IDENTIFIER '.' TOK_IDENTIFIER{
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn($1,$1,$3,NULL,UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
			if($1!=NULL)
				aliasList->ft->append(aliasList,$1);	
		}
		| rowListElement ',' expression OPT_AS  {
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn(NULL,NULL,$3,$4,UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
		}
		| expression OPT_AS{
			if(columnList == NULL){
				 columnList = newList();
				 aliasList = newList();
			}
			Column *col = newColumn(NULL,NULL,$1,$2,UNDEF,UNDEF);
			columnList->ft->append(columnList,col);
			allColumnsList->ft->append(allColumnsList,col); 	
		}   
		
		  	
;
		OPT_AS:///!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			/* empty */ {$$=" ";} | TOK_IDENTIFIER {$$=$1;} | TOK_AS TOK_IDENTIFIER {$$=$2;}
			;



expression:							/* very limited */
/*	expression operator expression
	|  */   
	expressionElement
/*	| '+' expressionElement
	| '-' expressionElement */
;
	expressionElement:
		TOK_IDENTIFIER				/* sehr ausbaufähig */
	;
/*operator:
	TOK_CONCAT
	| '+'
	| '-'
	| '*'
	| '/'	
;*/	
fromClause:							/* not empty */
	TOK_FROM {
		//printf("FROM gefunden\n");
		} tableReferenceList{
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
			
			$$ = rsbs;
		}
		else 
			$$ = s; 
		tableReferenceList->ft->clear(tableReferenceList);
	}
	;
tableReferenceList:
	tableReference	{
		if(tableReferenceList==NULL)
			tableReferenceList = newList();
		tableReferenceList->ft->append(tableReferenceList,$1);
	}
		
	| tableReferenceList ',' tableReference {
		if(tableReferenceList==NULL)
			tableReferenceList = newList();
		tableReferenceList->ft->append(tableReferenceList,$3);	
	}	
;

tableReference: 					/* very limited (S.64) */
	tableName correlationClause {
		ClassDef* cd = (ClassDef*)$1;
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
						
						if(colA->tableName==NULL || (strcasecmp(colA->tableName,$2)==0 || strcasecmp(colA->tableName,cd->className)==0)){
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
								if(strcasecmp(alias,$2)==0)
									aliasList->ft->removeCurrent(aliasList);
								
							}
							if(strcmp(" ",$2)==0)
								colA->tableAlias = cd->className;
							else
								colA->tableAlias = $2;
							//printf("spalte gefunden: %s.%s mit typ: %d\n",colA->tableName,colA->colName,colA->colSQLType);
							break;
						}	
					}	
				}
			}
			//printf("§§§!!! %s\n",columnList);	
		}
		
		sbs->setName = (char*)calloc(1,strlen(cd->className)+1);
		sbs->aliasName = (char*)calloc(1,strlen($2)+1);
		strcpy(sbs->aliasName,$2);
		strcpy(sbs->setName,cd->className);
		sbs->isSet = 1;
		$$ = sbs;

	}			/* view or table name */
	| tableReference joinType TOK_JOIN tableReference TOK_ON joinCondition{
		$$ = newSubSelectC($1,$4,$2);
		//printf("new SubSelect join %p\n",$$);
	}
; 	
correlationClause:
	/* empty */ {$$ = " ";}
	| TOK_AS TOK_IDENTIFIER /*columnName */  //die ist erstmal überflüssig!!
  {
  	$$ = $2;
  }
;
tableName:
	TOK_IDENTIFIER {
		//printf("suche: %s\n",$1);
		ClassDef * cd = getClassDef(STM->db,$1);
		if(cd==NULL){	
			yyerror(semanticError("UNDIFINED TABLE:\nTable \"",$1,"\" does not exist"));STM->rs->sw->sqlstate = "42P01";YYERROR;
		}
		else{
			//printf("Die Klasse gibt es!!!\n");
			$$ = cd;
		}
	}
;
/*	columnName:				ist ersteinmal überflüssig, siehe oben
		/ * empty * /
		| '(' columnNameList ')'
	;*/ 
/*	columnNameList:
		TOK_IDENTIFIER
		| columnNameList ',' TOK_IDENTIFIER
		;*/

joinType:
	/* empty */ {$$ = INNER;}						/* äquvivalent zu INNER */
	| TOK_INNER {$$ = INNER;}
	| TOK_LEFT {$$ = LEFT;}
	| TOK_RIGHT {$$ = RIGHT;}
	| TOK_FULL {$$ = FULL;}
;

whereClause:						/* may be empty */
	/* empty */ | TOK_WHERE searchCondition
;

groupByClause:						/* may be empty */
	/* empty */ | TOK_GROUP  {
	  yyerror("FEATURE NOT SUPPORTED: GROUP BY");STM->rs->sw->sqlstate = "0A000";YYERROR;
	}
        TOK_BY groupingExpression
;

groupingExpression:
	TOK_IDENTIFIER								/* reicht das??? */
	| groupingExpression ',' TOK_IDENTIFIER
;

havingClause:						/* may be empty */
	/* empty */ | TOK_HAVING  {yyerror("FEATURE NOT SUPPORTED: HAVING");STM->rs->sw->sqlstate = "0A000";YYERROR;} searchCondition	
;
orderByClause:						/* may be empty */
	/* empty */ | TOK_ORDER TOK_BY {obList = newList();} sortKeyList
;
sortKeyList:
	sortKey
	| sortKeyList ',' sortKey
;
sortKey:
	TOK_INTEGER OPT_ASC_DESC {//orderby nur auf columns, die in pi angegeben wurden!!!
		//columnList muss zu diesem Zeitpunkt gefüllt sein!!!
		//int found = 0;
		int colnr = $1;
		char num[4];
		
		if(colnr>columnList->ft->size(columnList)||colnr<1){			
		  sprintf(num,"%d",$1);
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
			
			obList->ft->append(obList,newOrder(colnr,t,$2));
			//printf("int key found \n");
		}
	}
	| TOK_IDENTIFIER OPT_ASC_DESC {
	
		Column* col;
		int i, found;
		for(i=0,col = (Column*)columnList->ft->getFirst(columnList);col;col = (Column*)columnList->ft->getNext(columnList),i++){
			if(strcasecmp(col->colName,$1)==0||strcasecmp(col->colAlias,$1)==0){
				found = 1;
				obList->ft->append(obList,newOrder(i,col->colSQLType,$2));
				//printf("key found\n");
				break;
			}
		}
		if(found==0){
			yyerror(semanticError("UNDEFINED COLUMN:\nColumn \"",$1,"\" in order-by-clause does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
		}
	}
;

OPT_ASC_DESC:
	/* empty */{$$ = ASC;}
	| TOK_ASC {$$ = ASC;}
	| TOK_DESC {$$ = DSC;}
;	
fetchFirstClause:					/* may be empty */
	/* empty */ {$$ = UNDEF;}
	| TOK_FETCH TOK_FIRST TOK_INTEGER TOK_ROW TOK_ONLY {
		if($3<1){
			char num[10];
			//sprintf(num,"%d",$3);
			yyerror(semanticError("INVALIDE VALUEN:\non positive value x=",num," in \"FETCH FIRST x ROWS ONLY\"-Clause \""));STM->rs->sw->sqlstate = "22020";YYERROR;
		}
		else
			$$=$3;	
	}
;
searchCondition:					/* vereinfacht wegen ?? */
	optNot predicate optCondition {
		Sigma *sigma = (Sigma*) $2;
		if($1==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
;
optCondition:
	/* empty */
	| TOK_AND  optNot predicate optCondition {//optnot dreht die Einträge, die man aus predicate erhält, um
		Sigma *sigma = (Sigma*) $3;
		sigma->link = AND;
		if($2==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	} 
	| TOK_OR  optNot predicate optCondition {
		Sigma *sigma = (Sigma*) $3;
		sigma->link = OR;
		if($2==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
;
optLimCondition:
	/* empty */
	| TOK_AND optNot limitedPredicate {
		Sigma *sigma = (Sigma*) $3;
		sigma->link = AND;
		if($2==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
	
	| TOK_OR  optNot limitedPredicate {
	        Sigma *sigma = (Sigma*) $3;
		sigma->link = OR;
		if($2==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);	
	}
;
joinCondition:
	optNot limitedPredicate optLimCondition {
		Sigma *sigma = (Sigma*) $2;
		if($2==NOT)
			sigma->negate(sigma);
		whereList->ft->append(whereList,sigma);
		
	}
;
predicate:							/* Differenzierung notwendig wegen joinCondition ohne fullSelects ACHTUNG: Dadurch kein IN in JOIN! */
	limitedPredicate {$$ = $1;}
/*| enhPredicate*/
;

limitedPredicate:
	  colIdentifier comparison colIdentifier {//ursprünglich stand hier eine expression. bis auf weiteres zurückgestellt
	  	//printf("join pred\n");
	  	Column* colA = (Column*)$1;
		Column* colB = (Column*)$3;
		int typeA = colA->colSQLType;
		int typeB = colB->colSQLType;
		
	  	if(typeA!=typeB||typeA==UNDEF||typeB==UNDEF){  		
			yyerror(semanticError("DATATYPE MISMATCH:\nColumn-Types of the two columns do not match: ",colA->colName,colB->colName));STM->rs->sw->sqlstate = "42804";YYERROR;
	  	}
	  		
	  	$$ = newSigma(colA,$2,NULL,colB,NULL);//INNER oder FULL????
	}
	| colIdentifier comparison number {
		
		Column* col = (Column*)$1;
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
		     
	  	$$ = newSigma(col,$2,$3,NULL,NULL);
	  	//printf("%s %p %p\n",colnameA,colnameA,$$);
	}
	| colIdentifier comparison TOK_STRING  {
		//printf("::: string pred\n");
		Column* col = (Column*)$1;
		
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
		      
  		$$ = newSigma(col,$2,$3,NULL,NULL);
	  	//printf("%s %p %p\n",colnameA,colnameA,$$);
	} 
	| colIdentifier optNot TOK_BETWEEN number TOK_AND number {//zwei Einträge ex >a AND ex<b == ex between a and b
														// kann eine Stufe weiter oben gemacht werden, wenn AND, NOT bekannt ist!
		//printf("::: between\n");
	  	Column* col = (Column*)$1;
		
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
	  	int between = ($2==NOT)? NOTBETWEEN : BETWEEN;
	  	double a = strtod($4,(char**)NULL);
	  	double b = strtod($6,(char**)NULL);
	  	if(a<b)
	  		$$ = newSigma(col,between,$4,NULL,$6);
	  	 else
	  		$$ = newSigma(col,between,$6,NULL,$4);
	  	//printf("BETWEEN %s and % s\n",$4,$6)		
	}
	| colIdentifier TOK_IS optNot TOK_NULL {
		Column* col = (Column*)$1;
		
		int not = ($3==NOT)? NOTNULL : ISNULL;      
  		$$ = newSigma(col,not,NULL,NULL,NULL);
	}
	//| matchExpression optNot TOK_LIKE patternExpression /* escape gespart */
;
colIdentifier://entweder colname, colalias tabelname.colname tablealias.colname,
			  //tablename.colalias und tablealia.colalias machen keinen Sinn!
			  //zurueckgegeben wird tablename.colname 
	TOK_IDENTIFIER{
		
		Column* rcol = NULL;
		Column* col;
		for(col = (Column*)allColumnsList->ft->getFirst(allColumnsList);col;col = (Column*)allColumnsList->ft->getNext(allColumnsList)){
			if(strcasecmp(col->colName,$1)==0||strcasecmp(col->colAlias,$1)==0){
				rcol = col;//printf("gefunden!!!\n");
			}
			if(rcol!=NULL)
				break;
		}
	  	if(rcol==NULL){
			yyerror(semanticError("UNDIFINED COLUMN:\nColumn \'",$1,"\' in SEARCH-CONDITION does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}	
	  	$$ = rcol;
	}
	| TOK_IDENTIFIER '.' TOK_IDENTIFIER {
	       
		Column* col;
		Column* rcol = NULL;
		char *alias = NULL;
		char *aliasc;
		for(aliasc = (char*)aliasList->ft->getFirst(aliasList);aliasc;aliasc = (char*)aliasList->ft->getNext(aliasList)){
			if(strcasecmp(aliasc,$1)==0||strcasecmp(aliasc,$1)==0){
				alias = aliasc;
			}
			
			if(alias!=NULL)
				break;
		}
		for(col = (Column*)allColumnsList->ft->getFirst(allColumnsList);col;col = (Column*)allColumnsList->ft->getNext(allColumnsList)){
			if((strcasecmp(col->colName,$3)==0||strcasecmp(col->colAlias,$3)==0)&&(alias!=NULL||strcasecmp(col->tableName,$1))){
				rcol = col;
			}
			
			if(rcol!=NULL)
				break;
		}
	  	if(rcol==NULL){
			yyerror(semanticError("UNDIFINED COLUMN:\nColumn \'",$3,"\' in SEARCH-CONDITION does not exist"));STM->rs->sw->sqlstate = "42703";YYERROR;
	  	}	
	  	$$ = rcol;
	}
	
number:
	TOK_INTEGER {//printf("::: number1\n");
		char buf[100];
		int i = sprintf(buf,"%d",$1);
		//printf("::: unumber2 %d %s %d %d\n",i,buf,$1,$1);
		$$ = (char*) calloc(1,i+1);
		strncpy($$,buf,i);
	} 
	| TOK_DOUBLE {
		char buf[100];
		int i = sprintf(buf,"%f",$1);
		//printf("::: unumber2 %d %s %f\n",i,buf,$1);
		$$ = (char*) calloc(1,i+1);
		strncpy($$,buf,i);
	}
;
/*	
enhPredicate:
	| TOK_EXISTS '('fullSelect')'
	| inPredicate
	| expression comparison optSome '('fullSelect')'
	| '(' inExprB ')' comparison optSome '('fullSelect')'	
	;*/
comparison:
	  TOK_EQ {$$ = EQ;}
	| TOK_NE {$$ = NE;}
	| TOK_GT {$$ = GT;}
	| TOK_LT {$$ = LT;}
	| TOK_GE {$$ = GE;}
	| TOK_LE {$$ = LE;}
;
/*
inPredicate:
	  expression optNot TOK_IN inExprA
	| '(' inExprB ')' optNot TOK_IN '('fullSelect')'
;
inExprA:
	expression
	| '(' fullSelect ')'
	| '(' inExprB ')';						//Semantisch vielleicht andere BEdeutung als im anderen Fall 
;
inExprB:
	 expression 
	| inExprB ','  expression 
	;*/
optNot:
	/* empty */{$$ = UNDEF;}
	| TOK_NOT {$$ = NOT;}
;
/*matchExpression:
	expression 						// muss zu einem String evaluieren... 
;*/
/*patternExpression:
	'\'' TOK_IDENTIFIER '\''
	;*/
/*optSome:
	| TOK_SOME
	| TOK_ANY
	| TOK_ALL
;
*/
%%
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
