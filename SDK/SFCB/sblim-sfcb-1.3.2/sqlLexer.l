%option never-interactive
%{
/*
 * sqlLexer.l
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

extern int sqlInput(char* buffer, int* numRead, int numRequested);
extern int yyErr(const char*);
extern char * yyStrcpy(char *txt, int len);
extern void setSqlWarning(char* s, char *r);

#undef YY_INPUT
#define YY_INPUT(BUF, NREAD, NREQUESTED) sqlInput(BUF, &NREAD, NREQUESTED)



#include "queryOperation.h"
#include "sqlParser.h"
#include "sqlStatement.h"
#include <stdio.h>
#include <string.h>

#if 0
# define QL_TRACE(X) printf X
#else
# define QL_TRACE(X)
#endif

//extern SqlWarning* Sw;

#define MAXBUF 500

char linebuf[MAXBUF];
int lineno=0;
int tokenpos=0;

%} 

POSITIVE_DECIMAL_DIGIT [1-9]
DECIMAL_DIGIT [0-9]
BLANK [ \t]
IDENT_CHAR [A-Za-z_]

%%

"--".*	/* comment */


\n.* {
	strcpy(linebuf, yytext+1);//save next line	
	lineno++;
	tokenpos = 0;
	yyless(1);//alles außer dem \n zum erneuten Scannen zurückgeben
}

[Cc][Aa][Ll][Ll] {
	tokenpos+=yyleng;
	return TOK_CALL;
}


[Ss][Mm][Aa][Ll][Ll][Ii][Nn][Tt] {
	tokenpos+=yyleng;
	return TOK_SINTDT;
}

[Ii][Nn][Tt]([Ee][Gg][Ee][Rr])? {
	tokenpos+=yyleng;
	return TOK_INTDT;
}

[Bb][Ii][Gg][Ii][Nn][Tt] {
	tokenpos+=yyleng;
	return TOK_BINTDT;
}

[Rr][Ee][Aa][Ll] {
	tokenpos+=yyleng;
	return TOK_REALDT;
}

[Dd][Oo][Uu][Bb][Ll][Ee] {
	tokenpos+=yyleng;
	return TOK_DOUBLEDT;
}

(([Dd][Ee][Cc]([Ii][Mm][Aa][Ll])?)|([Nn][Uu][Mm]([Ee][Rr][Ii][Cc])?)) {
	tokenpos+=yyleng;
	return TOK_DECDT;
}

[Cc][Hh][Aa][Rr] {
	tokenpos+=yyleng;
	return TOK_CHARDT;
}

[Vv][Aa][Rr][Cc][Hh][Aa][Rr] {
	tokenpos+=yyleng;
	return TOK_VCHARDT;
}

[Dd][Aa][Tt][Ee] {
	tokenpos+=yyleng;
	return TOK_DATEDT;
} 

[Tt][Ii][Mm][Ee] {
	tokenpos+=yyleng;
	return TOK_TIMEDT;
}

[Tt][Ii][Mm][Ee][Ss][Tt][Aa][Mm][Pp] {
	tokenpos+=yyleng;
	return TOK_TSTAMPDT;
}

[Ss][Ee][Ll][Ee][Cc][Tt] {

    QL_TRACE(("LEX: %s [TOK_SELECT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_SELECT;
}

[Ff][Rr][Oo][Mm] {

    QL_TRACE(("LEX: %s [TOK_FROM]\n", yytext));
    tokenpos+=yyleng;
	return TOK_FROM;
}

[Ww][Hh][Ee][Rr][Ee] {

    QL_TRACE(("LEX: %s [TOK_WHERE]\n", yytext));
    tokenpos+=yyleng;
	return TOK_WHERE;
}

[Tt][Rr][Uu][Ee] {

    QL_TRACE(("LEX: %s [TOK_TRUE]\n", yytext));
    tokenpos+=yyleng;
	return TOK_TRUE;
}

[Ff][Aa][Ll][Ss][Ee] {

    QL_TRACE(("LEX: %s [TOK_FALSE]\n", yytext));
    tokenpos+=yyleng;
	return TOK_FALSE;
}

[Nn][Uu][Ll][Ll] {

    QL_TRACE(("LEX: %s [TOK_NULL]\n", yytext));
    tokenpos+=yyleng;
	return TOK_NULL;
}

[Cc][Oo][Nn][Cc][Aa][Tt] {

    QL_TRACE(("LEX: %s [TOK_CONCAT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_CONCAT;
}
"||" {

    QL_TRACE(("LEX: %s [TOK_CONCAT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_CONCAT;
}

[Nn][Oo][Tt] {

    QL_TRACE(("LEX: %s [TOK_NOT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_NOT;
}

[Aa][Nn][Dd] {

    QL_TRACE(("LEX: %s [TOK_AND]\n", yytext));
    tokenpos+=yyleng;
	return TOK_AND;
}

[Oo][Rr] {

   QL_TRACE(("LEX: %s [TOK_OR]\n", yytext));
    tokenpos+=yyleng;
	return TOK_OR;
}

[Ii][Ss] {

    QL_TRACE(("LEX: %s [TOK_IS]\n", yytext));
    tokenpos+=yyleng;
	return TOK_IS;
}

[Aa][Ll][Ll] {

    QL_TRACE(("LEX: %s [TOK_ALL]\n", yytext));
    tokenpos+=yyleng;
	return TOK_ALL;
}

[Aa][Ss] {

    QL_TRACE(("LEX: %s [TOK_AS]\n", yytext));
    tokenpos+=yyleng;
	return TOK_AS;
}

[Dd][Ii][Ss][Tt][Ii][Nn][Cc][Tt] {

    QL_TRACE(("LEX: %s [TOK_DISTINCT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_DISTINCT;
}

[Jj][Oo][Ii][Nn] {

	QL_TRACE(("LEX: %s [TOK_JOIN]\n", yytext));
	tokenpos+=yyleng;
	return TOK_JOIN;
}

[Oo][Nn] {

	QL_TRACE(("LEX: %s [TOK_ON]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ON;
}

[Ll][Ee][Ff][Tt]([ ][Oo][Uu][Tt][Ee][Rr])? {

	QL_TRACE(("LEX: %s [TOK_LEFT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_LEFT;
}

[Rr][Ii][Gg][Hh][Tt]([ ][Oo][Uu][Tt][Ee][Rr])? {

	QL_TRACE(("LEX: %s [TOK_RIGHT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_RIGHT;
}

[Ff][Uu][Ll][Ll]([ ][Oo][Uu][Tt][Ee][Rr])? {

	QL_TRACE(("LEX: %s [TOK_FULL]\n", yytext));
	tokenpos+=yyleng;
	return TOK_FULL;
}




[\*(),.+-,':] { 

QL_TRACE(("LEX: %s [???]\n", yytext));
   tokenpos+=yyleng;
	return yytext[0];
}

"="|"==" { 
	QL_TRACE(("LEX: %s [TOK_EQ]\n", yytext));   
    tokenpos+=yyleng;
	return TOK_EQ; 
}

"!="|"<>" { 

	QL_TRACE(("LEX: %s [TOK_NE]\n", yytext));    
    tokenpos+=yyleng;
	return TOK_NE; 
}


"<=" { 

    QL_TRACE(("LEX: %s [TOK_LE]\n", yytext));
    tokenpos+=yyleng;
	return TOK_LE; 
}

"<" { 

    QL_TRACE(("LEX: %s [TOK_LT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_LT; 
}

">=" { 

    QL_TRACE(("LEX: %s [TOK_GE]\n", yytext));
    tokenpos+=yyleng;
	return TOK_GE; 
}

">" { 

    QL_TRACE(("LEX: %s [TOK_GT]\n", yytext));
    tokenpos+=yyleng;
	return TOK_GT; 
}

[Dd][Rr][Oo][Pp] {

	QL_TRACE(("LEX: %s [TOK_DROP]\n", yytext));
	tokenpos+=yyleng;
	return TOK_DROP; 
}
 
[Tt][Aa][Bb][Ll][Ee] {

	QL_TRACE(("LEX: %s [TOK_TABLE]\n", yytext));
	tokenpos+=yyleng;
	return TOK_TABLE; 
}
	
[Aa][Ll][Tt][Ee][Rr] {

	QL_TRACE(("LEX: %s [TOK_ALTER]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ALTER; 
}
	
[Cc][Rr][Ee][Aa][Tt][Ee] {

	QL_TRACE(("LEX: %s [TOK_CREATE]\n", yytext));
	tokenpos+=yyleng;
	return TOK_CREATE; 
}
	

[Kk][Ee][Yy] {

	QL_TRACE(("LEX: %s [TOK_KEY]\n", yytext));
	tokenpos+=yyleng;
	return TOK_KEY; 
}

[Ff][Oo][Rr][Ee][Ii][Gg][Nn] {

	QL_TRACE(("LEX: %s [TOK_FOREIGN]\n", yytext));
	tokenpos+=yyleng;
	return TOK_FOREIGN; 
}

[Uu][Nn][Ii][Qq][Uu][Ee] {

	QL_TRACE(("LEX: %s [TOK_UNIQUE]\n", yytext));
	tokenpos+=yyleng;
	return TOK_UNIQUE; 
}

[Pp][Rr][Ii][Mm][Ee][Rr][Yy] {

	QL_TRACE(("LEX: %s [TOK_PRIMARY]\n", yytext));
	tokenpos+=yyleng;
	return TOK_PRIMARY; 
}

[Cc][Oo][Nn][Ss][Tt][Rr][Aa][Ii][Nn][Tt] {
	
	QL_TRACE(("LEX: %s [TOK_CONSTRAINT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_CONSTRAINT; 
}

[Rr][Ee][Ff][Ee][Rr][Ee][Nn][Cc][Ee][Ss] {

	QL_TRACE(("LEX: %s [TOK_REFERENCES]\n", yytext));
	tokenpos+=yyleng;
	return TOK_REFERENCES; 
}

[Cc][Hh][Ee][Cc][Kk] {
	
	QL_TRACE(("LEX: %s [TOK_CHECK]\n", yytext));
	tokenpos+=yyleng;
	return TOK_CHECK; 
}

[Pp][Rr][Ii][Mm][Aa][Rr][Yy] {

	QL_TRACE(("LEX: %s [TOK_PRIMARY]\n", yytext));
	tokenpos+=yyleng;
	return TOK_PRIMARY; 
}

[Ii][Nn][Ss][Ee][Rr][Tt] {

	QL_TRACE(("LEX: %s [TOK_INSERT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_INSERT; 
}

[Ss][Ee][Tt] {
	
	QL_TRACE(("LEX: %s [TOK_SET]\n", yytext));
	tokenpos+=yyleng;printf("set\n");
	return TOK_SET; 
}
	
[Uu][Pp][Dd][Aa][Tt][Ee] {

	QL_TRACE(("LEX: %s [TOK_UPDATE]\n", yytext));
	tokenpos+=yyleng;
	return TOK_UPDATE; 
}
	

[Dd][Ee][Ll][Ee][Tt][Ee] {

	QL_TRACE(("LEX: %s [TOK_DELETE]\n", yytext));
	tokenpos+=yyleng;
	return TOK_DELETE; 
}
	
[Ii][Nn][Tt][Oo] {

	QL_TRACE(("LEX: %s [TOK_INTO]\n", yytext));
	tokenpos+=yyleng;
	return TOK_INTO; 
}

[Vv][Aa][Ll][Uu][Ee][Ss] {

	QL_TRACE(("LEX: %s [TOK_VALUES]\n", yytext));
	tokenpos+=yyleng;
	return TOK_VALUES; 
}	
	
[Dd][Ee][Ff][Aa][Uu][Ll][Tt] {

	QL_TRACE(("LEX: %s [TOK_DEFAULT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_DEFAULT; 
}
	

[Ii][Nn] {

	QL_TRACE(("LEX: %s [TOK_IN]\n", yytext));
	tokenpos+=yyleng;
	return TOK_IN; 
}

[Ll][Ii][Kk][Ee] {
	
	QL_TRACE(("LEX: %s [TOK_LIKE]\n", yytext));
	tokenpos+=yyleng;
	return TOK_LIKE; 
}

[Ss][Oo][Mm][Ee] {

	QL_TRACE(("LEX: %s [TOK_SOME]\n", yytext));
	tokenpos+=yyleng;
	return TOK_SOME; 
}

[Aa][Nn][Yy] {

	QL_TRACE(("LEX: %s [TOK_ANY]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ANY; 
}

[Ee][Xx][Ii][Ss][Tt][Ss] {

	QL_TRACE(("LEX: %s [TOK_EXISTS]\n", yytext));
	tokenpos+=yyleng;
	return TOK_EXISTS; 
}

[Bb][Ee][Tt][Ww][Ee][Ee][Nn] {

	QL_TRACE(("LEX: %s [TOK_BETWEEN]\n", yytext));
	tokenpos+=yyleng;
	return TOK_BETWEEN; 
}

[Gg][Rr][Oo][Uu][Pp] {

	QL_TRACE(("LEX: %s [TOK_GROUP]\n", yytext));
	tokenpos+=yyleng;
	return TOK_GROUP; 
}
	
[Ff][Ee][Tt][Cc][Hh] {

	QL_TRACE(("LEX: %s [TOK_FETCH]\n", yytext));
	tokenpos+=yyleng;
	return TOK_FETCH; 
}
	
[Oo][Nn][Ll][Yy] {

	QL_TRACE(("LEX: %s [TOK_ONLY]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ONLY; 
}
	
[Aa][Ss][Cc] {

	QL_TRACE(("LEX: %s [TOK_ASC]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ASC; 
}
	
[Dd][Ee][Ss][Cc] {

	QL_TRACE(("LEX: %s [TOK_DESC]\n", yytext));
	tokenpos+=yyleng;
	return TOK_DESC; 
}
	
[Bb][Yy] {

	QL_TRACE(("LEX: %s [TOK_BY]\n", yytext));
	tokenpos+=yyleng;
	return TOK_BY; 
}
	
[Rr][Oo][Ww][Ss]? {

	QL_TRACE(("LEX: %s [TOK_ROWS]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ROW; 
}
	
[Oo][Rr][Dd][Ee][Rr] {

	QL_TRACE(("LEX: %s [TOK_ORDER]\n", yytext));
	tokenpos+=yyleng;
	return TOK_ORDER; 
}
	
[Ff][Ii][Rr][Ss][Tt] {

	QL_TRACE(("LEX: %s [TOK_FIRST]\n", yytext));
	tokenpos+=yyleng;
	return TOK_FIRST; 
}
	
[Hh][Aa][Vv][Ii][Nn][Gg] {

	QL_TRACE(("LEX: %s [TOK_HAVING]\n", yytext));
	tokenpos+=yyleng;
	return TOK_HAVING; 
}
	

[Uu][Nn][Ii][Oo][Nn] {

	QL_TRACE(("LEX: %s [TOK_UNION]\n", yytext));
	tokenpos+=yyleng;
	return TOK_UNION; 
}
[Uu][Nn][Ii][Oo][Nn][Aa][Ll][Al] {

	QL_TRACE(("LEX: %s [TOK_UNIONALL]\n", yytext));
	tokenpos+=yyleng;
	return TOK_UNIONALL; 
}
	
[Ee][Xx][Cc][Ee][Pp][Tt] {
	
	QL_TRACE(("LEX: %s [TOK_EXCEPT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_EXCEPT; 
}
[Ee][Xx][Cc][Ee][Pp][Tt][Aa][Ll][Ll] {
	
	QL_TRACE(("LEX: %s [TOK_EXCEPTALL]\n", yytext));
	tokenpos+=yyleng;
	return TOK_EXCEPTALL; 
}
	
[Ii][Nn][Tt][Ee][Rr][Ss][Ee][Cc][Tt] { 

	QL_TRACE(("LEX: %s [TOK_INTERSECT]\n", yytext));
	tokenpos+=yyleng;
	return TOK_INTERSECT; 
}

[Ii][Nn][Tt][Ee][Rr][Ss][Ee][Cc][Tt][Aa][Ll][Ll] { 

	QL_TRACE(("LEX: %s [TOK_INTERSECTALL]\n", yytext));
	tokenpos+=yyleng;
	return TOK_INTERSECTALL; 
}
	
[Ww][Ii][Tt][Hh] { 

	QL_TRACE(("LEX: %s [TOK_WITH]\n", yytext));
	tokenpos+=yyleng;
	return TOK_WITH; 
}




{BLANK}+ {
	tokenpos+=yyleng;
	
    /* Ignore blanks */
}



[-+]?{POSITIVE_DECIMAL_DIGIT}{DECIMAL_DIGIT}* {
	QL_TRACE(("LEX: %s [???1]\n", yytext));
    yylval.intValue = strtol(yytext, (char**)0, 10);
    tokenpos+=yyleng;
    return TOK_INTEGER;
}

[-+]?0 {
	QL_TRACE(("LEX: %s [???1]\n", yytext));
    yylval.intValue = 0;
    tokenpos+=yyleng;
    return TOK_INTEGER;
}


[-+]?{DECIMAL_DIGIT}*\.{DECIMAL_DIGIT}+([eE][+-]?{DECIMAL_DIGIT}+)? {

    
     yylval.doubleValue = strtod((char*)yytext, (char**)0);
     
    tokenpos+=yyleng;
	return TOK_DOUBLE;
}

\"[^\"\n]*\" {

    QL_TRACE(("LEX: %s [???2]\n", yytext));
    size_t n = strlen(yytext) - 2;
    char* str=(char*)malloc(n+1);
   memcpy(str,yytext+1,n);
   str[n]=0;
  
    yylval.strValue=str;
    tokenpos+=yyleng;
	return TOK_STRING;
}

\"[^\"\n]*$ {
  QL_TRACE(("LEX: %s [???3]\n", yytext));
 tokenpos+=yyleng;
	   
}

[$] {return '$';}

{IDENT_CHAR}({IDENT_CHAR}|{DECIMAL_DIGIT})*  {

 //   QL_TRACE(("LEX: %s [TOK_IDENTIFIER]\n", yytext));

    {
    tokenpos+=yyleng;
	size_t n = strlen(yytext);
	yylval.strValue=sfcQueryStrcpy(yytext,n);
    }

    return TOK_IDENTIFIER;
}



%%

/*/ergänzt die Nachricht  m um die Positionsangabe des zuletzt gelesenen Zeichens*/
void yyerror(char *m){
	//char* ss;
	
	char* r; 
	char z[5];
	char s[5];
	char p[MAXBUF];
	int zi, si, pi;
	tokenpos-=yyleng;
	
	zi = sprintf(z,"%d",lineno);
	si = sprintf(s,"%d",tokenpos);
	pi = sprintf(p,"%*s\n",1+tokenpos,"^");
	
	
	r = (char *) malloc(strlen(m)+21+strlen(linebuf)+zi+si+pi+1);
	strcpy(r,"Zeile ");
	strncat(r,z,zi);
	strcat(r,", Spalte ");
	strncat(r,s,si);
	strcat(r,": ");
	strcat(r,m);
	strcat(r,"\n\t");
	strncat(r,linebuf,strlen(linebuf));
	strcat(r,"\n\t");
	strncat(r,p,pi);
	
	//printf("Zeile %d, Spalte %d: %s: \n%s\n",lineno,tokenpos,s,linebuf);
	//printf("%*s\n---\n",1+tokenpos,"^");
	//printf("%s",r);
//	RS->meta="kk";
//printf("%p\n",yylval.stm);
	//if(yylval.stm!=NULL&&strcmp("!!!!!",stm->rs->sw->sqlstate)==0){
	//	stm->rs->sw->reason = r;
	//}
	//else{
		//printf("adad\n");
		setSqlWarning("11111",r);
	//}
	//printf("2\n");
	
}

int yywrap()
{
    return 1;
}

