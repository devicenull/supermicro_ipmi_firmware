
/*
 * sqlStatement.c
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

#include "sqlStatement.h"
#include "utilft.h"

#include "cimXmlParser.h" //Provider operationen
#include "cmpidt.h"

#include "objectImpl.h"
#include "cmpimacs.h"
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#ifdef SFCB_IX86
#define SFCB_ASM(x) asm(x)
#else
#define SFCB_ASM(x)
#endif

#define SbS sbs = (SubSelect*)this->cnode; sbs
#define FS fs = (FullSelect*)this->cnode; fs

#define FREE(zgr){	\
	if(zgr!=NULL){	\
		free(zgr);	\
		zgr = NULL;	\
	}				\
}

/*benötigt für die Kommunikation mit den Providern, bzw. der Datenaufbereitung der Ergebnisse eines Providercalls*/
extern CMPIObjectPath *relocateSerializedObjectPath(void *area);
extern CMPIArgs *relocateSerializedArgs(void *area);
const char *opGetClassNameChars(CMPIObjectPath * cop);
extern CMPIConstClass *relocateSerializedConstClass(void *area);
extern CMPIConstClass *getConstClass(const char *ns, const char *cn);
extern CMPIConstClass initConstClass(ClClass *cl);
extern MsgSegment setConstClassMsgSegment(CMPIConstClass * cl);
extern CMPIValue str2CMPIValue(CMPIType type, char *val, XtokValueReference *ref);
extern MsgSegment setInstanceMsgSegment(CMPIInstance * ci);
extern CMPIStatus arraySetElementNotTrackedAt(CMPIArray * array, CMPICount index, CMPIValue * val, CMPIType type);
extern void closeProviderContext(BinRequestContext * ctx);
extern BinResponseHdr *invokeProvider(BinRequestContext * ctx);
extern int getProviderContext(BinRequestContext * ctx, OperationHdr * ohdr);
extern MsgSegment setCharsMsgSegment(const char *str);
extern MsgSegment setArgsMsgSegment(CMPIArgs * args);






/*die parse-Funktion, die in sqlParser.y definiert ist (im Makefile werden ihre Namen geändert, aus allen vorangestellten "yy" wird "sqfSlq")*/
extern int sfcSqlparse(void* stmt);

/*Datenstrukur-Konstruktoren*/
extern UtilList *newList();
extern UtilStringBuffer *newStringBuffer(int s);

static char *q = NULL;
static int ofs = 0;
static ResultSet* rs = NULL;

/*Definition der "Instanzmethoden, damit sie n den Konstruktoren benutzt werden können" */
static void freeWarn(SqlWarning** this);
static void freeCall(Call** this);
static void addMetaRset(ResultSet* this, Projection* prj);
static int addSetRset(ResultSet *this, AvlTree *t);
static void freeRSet(ResultSet** this);
static void freeStm(SqlStatement** this);
static int addNodeStm(SqlStatement* this, int type, void * value);
static int calculateStm(SqlStatement* this);
static void freeFSel(FullSelect** this);
static AvlTree* calculateFSel(FullSelect* this,char *db);
static void freeSSel(SubSelect** this);
static void addNodeSSel(SubSelect* this,int type, void* value);
static AvlTree* calculateSSel(SubSelect* this,char * db);
static void freePrj(Projection** this);
static void freeCol(Column** this);
static void freeCJoin(CrossJoin** this);
static void freeUpd(UpdIns** this);
static AvlTree* calculateCJoin(CrossJoin* this);
static void freeSel(Selection** this);
static void freeRow(Row** this);
static void freeOr(Order** this);
static void freeIns(Insert** this);
static void setWarningRs(ResultSet *this, char* s, char *r);
static void setWarning(SqlWarning *this, char* s, char *r);
static void negateSi(Sigma* this);
static void freeSi(Sigma** this);
static void freeExL(ExpressionLight** this);
static void addWhereListSbs(SubSelect* this, UtilList *ul);	



/*Konstruktoren*/
Call* newCall(char * tname, char * pname, UtilList * klist, UtilList * plist) {
	Call* this = (Call*) calloc(1, sizeof(Call));
	this->tableName = tname;
	this->procedureName = pname;
	this->keyList = klist;
	this->parameterList = plist;
	this->free = freeCall; 
	return this;
}

	
SqlWarning* newSqlWarning() {
	SqlWarning* this = (SqlWarning*) calloc(1, sizeof(SqlWarning));
	this->free = freeWarn;
	this->setWarning = setWarning; 
	return this;
}

Insert* newInsert(char* tname) {
	Insert* this = (Insert*) calloc(1, sizeof(Insert));
	this->tname = tname;
	this->tupelList = newList();
	this->free = freeIns;
	return this;
}

ResultSet* newResultSet(char* query) {
	ResultSet* this = (ResultSet*) calloc(1, sizeof(ResultSet));
	this->sw = newSqlWarning();
	this->sw->reason = "Parsing in Progress";
   	this->sw->sqlstate = "!!!!!";
	this->query = query;
	this->setWarning = setWarningRs; 
	this->addMeta = addMetaRset;
	this->addSet = addSetRset;
	this->free = freeRSet;
	return this;
}

SqlStatement* newSqlStatement(int type, ResultSet* rs) {
	SqlStatement* this = (SqlStatement*) calloc(1, sizeof(SqlStatement));
	this->type = type;
	this->rs = rs;
	this->db = DEFAULTDB;
	this->lastFs = NULL;
	this->cnode = newList();
	this->free = freeStm;
	this->addNode = addNodeStm;
	this->calculate = calculateStm;
	return this;
}

FullSelect* newFullSelect() {
	FullSelect* this = (FullSelect*) calloc(1, sizeof(FullSelect));
	this->typeA = UNDEF;
	this->typeB = UNDEF;
	this->free = freeFSel;
	this->calculate = calculateFSel;
	return this;
}

SubSelect* newSubSelect() {
	SubSelect* this = (SubSelect*) calloc(1, sizeof(SubSelect));
	this->cross = newCrossJoin(NULL,NULL,UNDEF);
	this->setName = "<empty>";
	this->free = freeSSel;
	this->addNode = addNodeSSel;
	this->calculate = calculateSSel;
	this->addWhereList = addWhereListSbs;
	return this;
}

SubSelect* newSubSelectC(SubSelect* sbsA,SubSelect* sbsB, int type) {
	SubSelect* this = newSubSelect();
	this->cross = newCrossJoin(sbsA,sbsB,type);
	this->addWhereList = addWhereListSbs;
	return this;
}

Projection* newProjection(int mode, UtilList* ul) {
	Projection* this = (Projection*) calloc(1, sizeof(Projection));
	this->mode = mode;
	this->col = ul;
	//ul->ft->clear
	this->free = freePrj;
	return this;
}

ExpressionLight* newExpressionLight(char* name, int op,char* value) {
	ExpressionLight* this = (ExpressionLight*) calloc(1, sizeof(ExpressionLight));
	this->op = op;
	this->name = name;
	this->value = value; 
	this->free = freeExL;
	return this;	
}

Column* newColumn(char* tableName,char* tableAlias, char* colName, char* colAlias, int colSQLType,int isKey) {
	Column* this = (Column*) calloc(1, sizeof(Column));
	if(tableName!=NULL){
//		this->tableName = (char*) malloc(sizeof(tableName));
//		strcpy(this->tableName,tableName);
		this->tableName = tableName;
	}
	if(colName!=NULL){
//		this->colName = (char*) malloc(sizeof(colName));
//		strcpy(this->colName,colName);
		this->colName=colName;
	}
	if(colAlias!=NULL){
//		this->colAlias = (char*) malloc(sizeof(colAlias));
//		strcpy(this->colAlias,colAlias);
		this->colAlias=colAlias;
	}
	this->tableAlias=tableAlias;
	this->colSQLType = colSQLType;
	this->isKey = isKey;
	this->free = freeCol;
	return this;
}

Selection* newSelection(UtilList* orderBy, int fetchFirst) {
	Selection* this = (Selection*) calloc(1, sizeof(Selection));
	this->fetchFirst = fetchFirst;
	this->orderBy = orderBy;
	this->free = freeSel;
	return this;	
}

CrossJoin* newCrossJoin(SubSelect* sbsA,SubSelect* sbsB, int type){
	CrossJoin* this = (CrossJoin*) calloc(1, sizeof(CrossJoin));
	this->setA = sbsA;
	this->setB = sbsB;
	this->type = type;
	this->free = freeCJoin;
	this->calculate = calculateCJoin;
	return this;
}

UpdIns* newUpdIns(char* tname, UtilList* colList,UtilList* assignmentList,UtilList* where){
	UpdIns* this = (UpdIns*) calloc(1, sizeof(UpdIns));	
	this->tname = tname;
	this->colList = colList;
	this->assignmentList = assignmentList;
	this->where = where;
	this->free = freeUpd;
	return this;
}

Row* newRow(int size){
	Row* this = (Row*) calloc(1, sizeof(Row));	
	this->doublette = 1;
	this->size=size;
	this->row = (char**) calloc(size, sizeof(char*));
	this->free = freeRow;
	return this;
}

ClassDef* newClassDef(int fieldCount, char * className, UtilList * fieldNameList, int fNameLength,char * superclass) {
	ClassDef* this = (ClassDef*) calloc(1, sizeof(ClassDef));	
	this->className = className;
	this->fieldCount = fieldCount;
	this->fieldNameList = fieldNameList;
	this->fNameLength = fNameLength;
	this->superclass = superclass;
	return this;
}
	

Order *newOrder(int nr, int type, int order){
	Order* this = (Order*) calloc(1, sizeof(Order));	
	this->column = nr;
	this->colSQLType = type;
	this->order = order;
	this->free = freeOr;
	return this;
}

Sigma* newSigma(Column* colA, int op,char* value, Column* colB,char* valueB){
	Sigma* this = (Sigma*) calloc(1, sizeof(Sigma));
	this->colA = colA;
	this->op = op;
	this->value = value;
	this->colB = colB;
	this->valueB = valueB;
	this->link = UNDEF;
	this->free = freeSi;
	this->negate = negateSi;
	this->free = freeSi;
	return this;
}

/*Hilfsfunktion, um die Gleichheit zweier Objekte vom Typ Column festzustellen*/
int coleql(Column *colA, Column *colB){
	//printf("%p %p\n",colA,colB);
	//printf("%p %p\n",colA->colName,colB->colName);
	//printf("%p %p\n",colA->tableAlias,colB->tableAlias);
	if(strcasecmp(colA->colName,colB->colName)==0&&strcasecmp(colA->tableAlias,colB->tableAlias)==0)
		return 0;
	return 1;
}

/*
kpiert den Teilbaum, den node repräsentiert in den Baum t ein. rekursiv.
 */
void copytree(AvlTree *t,AvlNode * node){
	if(node!=NULL){
		copytree(t,node->left);
		t->insert(t,node->item);
		copytree(t,node->right);
	}
}

/*wird aufgerufen, wenn ein Sigma-Objekt (Bedingung in einer WHERE-Klausel) einem Subselect zugeordnet wird
Verteilt die einzelnen Bedinugen auf die eventuell darunterliegenden Subselects (Baumstruktur, erzeugt durch Joins)
*/
static void addWhere(SubSelect *sbs, Sigma *sigma){
	//rekursiv durch den "Baum"
	//printf(">>> 1\n");
	UtilList *ul = sbs->pi->col;
	Column *col;
	//printf(">>> 2\n");
	
	for(col=(Column*)ul->ft->getFirst(ul);col;col=(Column*)ul->ft->getNext(ul)){
	
		//printf(">>> %d\n",i);
		//printf(">>> %p %p\n", sigma, col);
		
		if(sigma->colB!=NULL&&sigma->colA!=NULL){//JOIN!!!
			if(strcasecmp(col->colName,sigma->colA->colName)==0&&strcasecmp(col->colName,sigma->colB->colName)==0){
				if(sbs->sigma->where==NULL)
					sbs->sigma->where = newList();
				sbs->sigma->where->ft->append(sbs->sigma->where,sigma);
			return;
			}
		}
		//printf("3\n");
		if(coleql(col,sigma->colA)==0){
			//printf("4\n");
			if(sbs->sigma->where==NULL)
				sbs->sigma->where = newList();
			sbs->sigma->where->ft->append(sbs->sigma->where,sigma);
			
			//printf(">>> found\n");
	
			return;
		}
	}
	if(sbs->isSet!=1){
		addWhere(sbs->cross->setA,sigma);
		if(sbs->cross->setB!=NULL);
			addWhere(sbs->cross->setA,sigma);
	}
	
}

/*in ul befindet sich eine Liste von Bedingungen, die alle diesem Subselect zugeordnet werden sollen*/
static void addWhereListSbs(SubSelect* this, UtilList *ul){
       
	Sigma *sigma;
	//printf("%p/n",sigma);
	//printf("%d\n",ul->ft->size(ul));
	for(sigma=(Sigma*)ul->ft->getFirst(ul);sigma; sigma = (Sigma*)ul->ft->getNext(ul)){
		//printf("a\n");
		addWhere(this,sigma);	
	}
}

/*negiert die Bedigung*/
static void negateSi(Sigma* this){
	switch(this->link){
		case OR: this->link = AND; break;
		case AND: this->link = OR; break;
		case ISNULL: this->link = NOTNULL; return;
		case NOTNULL: this->link = ISNULL; return;
		case BETWEEN:  this->link = NOTBETWEEN; return;
		case NOTBETWEEN:  this->link = BETWEEN; return;
	}
	switch(this->op){
		case EQ: this->op = NE; break;
		case NE: this->op = NE; break;
		case GT: this->op = LE; break;
		case LT: this->op = GE; break;
		case GE: this->op = GT; break;
		case LE: this->op = LT; break;	
	}
}

/*Vergleicht zwei Zeichenketten abhängig von ihrem Typ (lexikographisch, numerisch, usw.) und der Orderby-Bedingung (asc, desc)*/
int row_cmp(char* r1,char *r2, int type, int order){
	int res = 0;
	long long int a1;
	long long int b1;
	unsigned long long int a2;
	unsigned long long int b2;
	long double a3;
	long double b3;
	if(r1==NULL||strcasecmp(r1,"NULL")==0)//||strcmp(r1,"Null")==0||strcmp(r1,"null")==0)
		res = -1;
	if(r2==NULL||strcasecmp(r2,"NULL")==0)//||strcmp(r2,"Null")==0||strcmp(r2,"null")==0)
		res = 1;
	else{
		switch (type & ~CMPI_ARRAY) {
			
			case CMPI_ARRAY:
				return 0;
			
			case CMPI_ref: //???
			case CMPI_instance: //???
			case CMPI_boolean:	//true-false
			case CMPI_dateTime: //jjjj-mm-tt ????
			case CMPI_chars:
			case CMPI_char16:
			case CMPI_string:
				res = strcasecmp(r1,r2);
				break;
			case CMPI_sint64:
			case CMPI_sint32:
			case CMPI_sint16:
			case CMPI_sint8:
				a1 = strtoll(r1,(char**)NULL,10);
				b1 = strtoll(r2,(char**)NULL,10);
				if(a1==b1)
					res = 0;
				else 
					res = (a1<b1) ? -1 : 1;
				break;
			case CMPI_uint32:
			case CMPI_uint16:
			case CMPI_uint8:
			case CMPI_uint64:
				a2 = strtoull(r1,(char**)NULL,10);
				b2 = strtoull(r2,(char**)NULL,10);
				if(a2==b2)
					res = 0;
				else 
					res = (a2<b2) ? -1 : 1;
				break;
			case CMPI_real32:
			case CMPI_real64:
				a3 = strtold(r1,(char**)NULL);
				b3 = strtold(r2,(char**)NULL);
				if(a3==b3)
					res = 0;
				else 
					res = (a3<b3) ? -1 : 1;
				break;
	
	   }
	}
	if(order==DSC)
		res *= -1;	
	char *rr = "==";
	if(res==-1)
		rr = "<";
	if(res==1)
		rr = ">";	
	//printf("--- %s %s %s\n",r1,rr,r2);
	return res;
}

/*wird im avlBaum benutzt, wenn Objekte vom Typ Row einsortiert werden. Row entält alle wichtigen Informationen (typ, order), um row_cmp aufrufen zu können*/
int item_cmp(const void *item1, const void *item2){
	Row *r1 = (Row*)item1;
	Row *r2 = (Row*)item2;
	int i;
	int res = 0;
//	printf("1\n");
	Selection *si = r1->sigma;
	//testet of Gleichheit
	for (i = 0; i < r1->size; ++i) {
		if(r1->row[i]==NULL||r2->row[i]==NULL)
			continue;
		if((res = strcmp(r1->row[i],r2->row[i]))!=0){
			//res = 1;
			break;
		}
	}
	//printf("2\n");
	if(res==0)
		return res;
	//kleiner oder größer?
	//i=0;printf("3 %p \n",r1);
	if(r1->sigma!=NULL&&r1->sigma->orderBy!=NULL){
		Order* o = si->orderBy->ft->getFirst(si->orderBy);
		do{
			res = row_cmp(r1->row[o->column],r2->row[o->column],o->colSQLType,o->order);	
			i++;
			o = si->orderBy->ft->getNext(si->orderBy);
		}while(res==0 && i < si->orderBy->ft->size(si->orderBy));	
		
	}
	//falls nun null rauskommt muss nach den keyattributen sortiert werden
	//man muss nur der row noch mitgeben ob sie schluessel ist oder nicht, 
	//dazu sollte mir jemand sagen, wie ich diese Info in getClassDef bekomme
	//UPDATE: Ich hab die Info eigebaut. Allerdngs steckt die in der Column, die von hier offenbar nicht accessible ist - also belasse ich es erstmal wie es ist
	//- oder eben nach allen und dies kann banal mit strcmp gemacht werden,
	//da die reihenfolge ja nicht definiert ist
	//printf("4\n");
	if(res==0){
		for(i=0;i<r1->size;i++){
			
			if(r1->row[i]!=NULL&&r2->row[i]!=NULL)res = strcmp(r1->row[i],r2->row[i]);
			//else printf("NULL aber warum???\n");
			if(res!=0)
				break;	
		}		
	}
	//printf("5\n");	
	return res;
}

/*Baumdurchlauf, bei dem alle items vom Typ Row freigegeben werden*/
void traverseDelRow(AvlNode* node){
	if(node!=NULL){
		traverseDelRow(node->left);
		Row * r = (Row*)node->item;
		r->free(&r);
		traverseDelRow(node->right);
	}
}

/*Destruktor*/
static void freeCall(Call** this) {
	Call * t = *this;
       
	FREE(t);
}
/*Destruktor*/
static void freeWarn(SqlWarning** this) {
	SqlWarning * t = *this;
	FREE(t->reason);
	FREE(t->sqlstate);
	FREE(t);
}
/*
 * siehe Tablle zur Datentypkonvertierung
 * Problem: Länge des VARCHAR.
 *  - einmal über Qualifier maxlength manchmal beschrieben. Kaum ein 
 *    Cimom kümmert das
 *  - da diese Info eigentlich nur für die Columnbreite (-->darstellung) 
 *    wichtig ist, könnte man hier einfach die Länge des längsten Strings der 
 *    Spalte zurückgeben. Problem nun: das ist für update eine semantische 
 *    Einschränkung an die Länge des einzufügenden Strings...
 */
static char *dataType(CMPIType type)
{
   switch (type & ~CMPI_ARRAY) {
   case CMPI_chars:
   case CMPI_string:
      return "VARCHAR(77)";
   case CMPI_sint64:
      return "BIGINT";
   case CMPI_uint64:
      return "BIGINT"; //was ist mit unsigned???
   case CMPI_sint32:
      return "INTEGER";
   case CMPI_uint32:
      return "BIGINT";
   case CMPI_sint16:
      return "SMALLINT";
   case CMPI_uint16:
      return "INTEGER";
   case CMPI_uint8:
      return "TINYINT"; //Tinyint??
   case CMPI_sint8:
      return "SMALLINT";
   case CMPI_boolean:
      return "BOOLEAN";
   case CMPI_char16:
      return "VARCHAR(17)";//Vielleicth CHAR(xy)
   case CMPI_real32:
      return "DECIMAL(7,4)";
   case CMPI_real64:
      return "DECIMAL(7,4)";
   case CMPI_dateTime:
      return "DATE"; 
   case CMPI_ref:
      return "REF";
   case CMPI_instance:
      return "OTHER";
   }
   printf("%s(%d): invalid data type %d %x\n", __FILE__, __LINE__, (int) type,
          (int) type);
   SFCB_ASM("int $3");
   abort();
   return "*??*";
}

char * int2String(int v){
	char buf[100];
	char * res;
	int i = sprintf(buf,"%d",v);
	res = (char*) calloc(1,i+1);
	strncpy(res,buf,i);
	return res;
}


char* type2sqlString(int type){
	switch (type & ~CMPI_ARRAY) {
	   case CMPI_chars:
	   case CMPI_string:
	      return "VARCHAR(1111)";
	   case CMPI_sint64:
	      return "BIGINT";
	   case CMPI_uint64:
	      return "BIGINT";
	   case CMPI_sint32:
	      return "INTEGER";
	   case CMPI_uint32:
	      return "INTEGER";
	   case CMPI_sint16:
	      return "SMALLINT";
	   case CMPI_uint16:
	      return "SMALLINT";
	   case CMPI_uint8:
	      return "SMALLINT";
	   case CMPI_sint8:
	      return "SMALLINT";
	   case CMPI_boolean:
	      return "BOOLEAN";
	   case CMPI_char16:
	      return "VARCHAR(222)";// vielleicht CHAR(12)
	   case CMPI_real32:
	      return "DECIMAL(7,4)";
	   case CMPI_real64:
	      return "DECIMAL(7,4)";
	   case CMPI_dateTime:
	      return "DATE"; 
	   case CMPI_ref:
	      return "REF";
	   case CMPI_instance:
	      return "OTHER";
   }
   printf("%s(%d): invalid data type %d %x\n", __FILE__, __LINE__, (int) type,
          (int) type);
   SFCB_ASM("int $3");
   abort();
   return "*??*";
}


/*Codiert die Metainfos des Ergebnis, das dieses Resultset repräsentiert in das Nachrichtenfpormat als Antwort an den Client*/
static void addMetaRset(ResultSet* this, Projection* prj){
	int i;
	UtilStringBuffer *sb=newStringBuffer(4096);
	//printf("%d MetaColsize\n",prj->col->ft->size(prj->col));
	//printf("pi: %p\n",prj);
	Column *col = (Column*)prj->col->ft->getFirst(prj->col);
	for (i = 0; i < prj->col->ft->size(prj->col); ++i) {
		sb->ft->appendChars(sb,col->tableName);
		sb->ft->appendChars(sb,";");
		sb->ft->appendChars(sb,col->colName);
		sb->ft->appendChars(sb,";");
		sb->ft->appendChars(sb,type2sqlString(col->colSQLType));
		sb->ft->appendChars(sb,";");
		sb->ft->appendChars(sb,"false");
		sb->ft->appendChars(sb,";");
		sb->ft->appendChars(sb,"0");
		sb->ft->appendChars(sb,";");
		sb->ft->appendChars(sb,"false");
		if(i<(prj->col->ft->size(prj->col)-1))
			sb->ft->appendChars(sb,"::");
		col->free(&col);
		col = prj->col->ft->getNext(prj->col);
	}
	this->meta = malloc(sb->ft->getSize(sb)+1);
	strcpy(this->meta, sb->ft->getCharPtr(sb));
	sb->ft->release(sb);
	
}


/*codiert die Tupel dem Baum gemäß der Protokollspezifikation. Rekursicer Baumdurhlauf*/
static void tree2sb(AvlNode *node,UtilStringBuffer *sb){
	int i,h;
	
	if(node){
		
		tree2sb(node->left,sb);
		
		Row *row = (Row*)node->item;
		for (h = 0; h < row->doublette; ++h) {//fügt doubletten ein
			for (i = 0; i < row->size; ++i) {
				if(row->row[i]!=NULL)
					sb->ft->appendChars(sb,row->row[i]);
				else
					sb->ft->appendChars(sb,"UNDEF");
				
				if(i<row->size-1)
					sb->ft->appendChars(sb,";");
			}
			
			sb->ft->appendChars(sb,"\n");
		}
		tree2sb(node->right,sb);
		
	}
	
}


/*Dubugging-Funktion, gibt den Baum auf der Standardausgabe aus*/
void treetoString(AvlNode * node){
	if(node){
		
		treetoString(node->left);
		int h;
		Row *row = (Row*)node->item;
		for (h = 0; h < row->doublette; ++h) {//fügt doubletten ein
				if(row->row[1]!=NULL)
					printf("\t%s ",row->row[1]);
				else
					printf("\t%s ","UNDEF");
				if(row->row[2]!=NULL)
					printf("\t%s \n",row->row[2]);
				else
					printf("\t%s \n","UNDEF");
				
			
		}
		treetoString(node->right);
		
	}
	
}

/*codiert die Ergebnismenge, falls eine vorliegt gemäß dem DB Protokoll*/
static int addSetRset(ResultSet *this, AvlTree *t){
	//printf("addSetRset()\n");
	
	//printf("afsf\n");
	if(t==NULL){
		this->sw->reason = "INTERNAL ERROR";
		this->sw->sqlstate ="XX000";
		return 1;	
	}
	else{
		//printf("afsf 1\n");	
		UtilStringBuffer *sb = newStringBuffer(80960000);//ist der groß genug??? Großes Problem!!! Stürzt sonst ab!!!
	//	printf("afsf 2\n");
		//treetoString(t->root);	
//	printf("afsf 3\n");
		tree2sb(t->root,sb);
		//sb->ft->appendChars(sb,"und schluss!!!");
		int size = sb->ft->getSize(sb);
		if(size < 1){
			this->tupel = malloc(strlen("**EMPTYSET**"));
			strcpy(this->tupel,"**EMPTYSET**");
		}
		else{
			this->tupel = calloc(1,size+1);
		//	printf("1  %d\n",sb->ft->getSize(sb));
			strncpy(this->tupel, sb->ft->getCharPtr(sb),size-1);//das letzte \n ist zu viel
	//printf("2\n");
		}	
		sb->ft->release(sb);
		//t->free(&t);
		//printf("3\n");
		
	//	printf("--%s--\n",this->tupel);
	return 0;
	}	
}

/*Destruktoren,...<
rufen sich im Prinzip rekursiv auf, so dass ein Aufruf von stmt->free(&stmt) alle im Zusammenhang erzeugten Strukturen wieder freigegeben werden.
  Noch ausbaufähig*/
static void freeRSet(ResultSet** this) {
	ResultSet* t = *this;
	FREE(t->meta);
	FREE(t->query);
	FREE(t->tupel);
	t->sw->free(&(t->sw));
	//FREE(t->sw);	 
}


static void freeStm(SqlStatement** this) {
 	//printf("freeStm 1\n");
 	SqlStatement *t = *this;
 	if(t->type == SELECT){
 		//printf("freeStm 2\n");
 
 		FullSelect* fs = (FullSelect*)t->stmt;
 		fs->free(&fs);
 		//printf("freeStm 3\n");
 		FREE(t->stmt);
 		//printf("freeStm 4\n");
 		//FREE(t->lasttos);	
 		//printf("freeStm 5\n");
 		t->cnode->ft->release(t->cnode);
 		//printf("freeStm 6\n");
 		//FREE(t->cnode);
 		//printf("freeStm 7\n");
 		FREE(t);
 		//printf("freeStm 1000\n");
 		
 	}
 
}

static void freeSel(Selection** this) {
	Selection* t = *this;
	//printf("freeSel 1\n");
	if(t->orderBy!=NULL){
		Order * ob;
		//printf("freeSel a\n");
		
		for(ob = (Order*)t->orderBy->ft->getFirst(t->orderBy);ob;ob = (Order*)t->orderBy->ft->getNext(t->orderBy)){//printf("freeSel c2\n");
			ob->free(&ob);
			//printf("freeSel c3\n");
		}
		//printf("freeSel d\n");
		t->orderBy->ft->release(t->orderBy);//printf("freeSel e\n");
	}
	//printf("freeSel 2\n");
	
	//printf("freeSel 3\n");
	//FREE(t->orderBy);
	//printf("freeSel 4\n");
	if(t->where!=NULL){
		Sigma * si;
		
		for(si = (Sigma*)t->where->ft->getFirst(t->where);si;si = (Sigma*)t->where->ft->getNext(t->where)){//printf("freeSel 4a %p\n",si);
			si->free(&si);
			//printf("freeSel 4b\n");
		}
		t->where->ft->release(t->where);
	}
	
	//FREE(t->where);
	FREE(t);
	 
}

/*eineee zentrale Funktion. Hier wird der Parsebaum aufebaut: dem statement wir ein value mitgegeben, das durch type eindeutig definiert ist. Abhängig vom Typ wird value auf den Baum angewendet.*/
static int addNodeStm(SqlStatement* this, int type, void * value) {
	FullSelect* fs=NULL;
  FullSelect *fsroot;//printf("--- 1 %d\n",type);
  if(type==SUB&&this->lastFs!=NULL){
    //printf("--- 2a\n");
    fs = this->lasttos;
  }
  else{
    //printf("--- 2\n");
	if(!this->cnode->ft->isEmpty(this->cnode))
		fs = (FullSelect*)this->cnode->ft->getLast(this->cnode);
    if(fs==NULL)
      fs = this->lasttos;
  }
  //printf("--- 3\n");
  //falls A u B u C geparst wurde muss ein neues Fullselect oben eingefügt werden
  if((type==UNION||type==UNIONALL||type==EXCEPT||type==EXCEPTALL||type==INTERSECT)&&fs!=NULL&&fs->typeB!=UNDEF){
    FullSelect * nfs = newFullSelect();//printf("--- 3a\n");
    nfs->setA = this->lasttos->setB;
    nfs->typeA = this->lasttos->typeB;
    nfs->type = type;
    //nfs->setB = value;
    //nfs->typeB = SUB;
    this->lasttos->setB = nfs;
    this->lasttos->typeB = FUL;
    this->lasttos = nfs;
    return OK;
  }//printf("--- 3 2\n");
	Insert *ins;
	UpdIns *upi;
	switch(type){
		case CALL:
			this->type = CALL;
			this->stmt = value;
			break;
		case CREATE:
			this->type = CREATE;
			this->stmt = value;
			break;
		case INSTUPEL:
			ins = (Insert*)this->stmt;
			upi = (UpdIns*)value;
			upi->tname = ins->tname;
			ins->tupelList->ft->append(ins->tupelList,upi);
			break;
		case INSERT:
			this->type = INSERT;
			this->stmt = value;
			break;
		case DELETE:
			this->type = DELETE;
			this->stmt = value;
			break;
		case UPDATE:
			this->type = UPDATE;
			this->stmt = value;
			break;
		case DROP:
			this->type = DROP;
			this->stmt = value;
			break;
		case INITFUL:
			//printf("**** %p\n",value);
			if(this->type==UNDEF){
				this->type = SELECT;
				this->stmt = value;
				//this->currentType = FUL;
				this->cnode->ft->append(this->cnode,value);
				//printf("### INITFUL %p %p\n",fs, value);
			}
			else if(this->type==INSERT){
				ins = (Insert*)this->stmt;
				if(ins->fs==NULL){
					ins->fs = (FullSelect*)value;
					this->cnode->ft->append(this->cnode,value);
				}
				else{
					fs = (FullSelect*)value;
					fs->free(&fs);
				}
			}
			else{
				fs = (FullSelect*)value;
				fs->free(&fs);
			}
    //this->lasttos = NULL;
			break;
		case FUL:	
			//printf("**** %p\n",value);
			if(fs->type!=EMPTY){
				if(fs->typeA==UNDEF){
					fs->setA = value;
					fs->typeA = FUL;
					//printf("### FUL A %p %p\n",fs, value);	
				}
				else{
					fs->setB = value;
					fs->typeB = FUL;
					//printf("### FUL %p %p\n",fs, value);
				}
			}
			else{//kann nicht passiere. sollte durch die grammatik abgefangen werden!!
      //printf("Schwerer Fehler in addNode(): \"cannot happen\"\n");
			}
			
			this->cnode->ft->append(this->cnode,value);
    //this->lasttos = NULL;
			break;
		case EMPTY:
			fs->type = EMPTY;
			this->lasttos = fs;
			this->cnode->ft->removeLast(this->cnode);
			break;
		
		case UNION:
			fs->type = UNION;
			break;
		case UNIONALL:
			fs->type = UNIONALL;
			break;
		case EXCEPT:
			fs->type = EXCEPT;
		break;
		case EXCEPTALL:
			fs->type = EXCEPTALL;
			break;
		case INTERSECT:
			fs->type = INTERSECT;
			break;
		case INTERSECTALL:
			fs->type = INTERSECTALL;
			break;
		case SUB:
			//if(this->currentType == FUL){
    //printf("--- SUB 1");
				fsroot = (FullSelect*)this->cnode->ft->getFirst(this->cnode);
    //printf("--- SUB 2");
    if(fsroot==NULL)
      fsroot = this->lasttos;
    // printf("--- SUB 3");
				if(fsroot->pi==NULL){	
					fsroot->pi = ((SubSelect*)value)->pi;
					//printf("fs-pi %p\n",fs->pi);
				}
				else{
					Projection* pi = ((SubSelect*)value)->pi;
					int i;
					if(pi->col->ft->size(pi->col)==fsroot->pi->col->ft->size(fsroot->pi->col)){
						Column* c1 = (Column*)pi->col->ft->getFirst(pi->col); 
						Column* c2 = (Column*)fsroot->pi->col->ft->getFirst(fsroot->pi->col); 
						for(i=0; i<pi->col->ft->size(pi->col);i++){//ist so übersichtlicher
							if(c1->colSQLType != c2->colSQLType)
								return CDEFERR+i+1;
							c1 = (Column*)pi->col->ft->getNext(pi->col); 
							c2 = (Column*)fsroot->pi->col->ft->getNext(fsroot->pi->col); 		
						}
					}
					else{
						return CCOUNTERR;	
					}
				}
    //printf("--- 4\n");
				if(fs->typeA==UNDEF){
					fs->typeA = SUB;
					fs->setA = value;
					//printf("### SUB A %p %p\n",fs, value);
				}
    else{
      // printf("--- 5\n");
      if(fs->typeB==UNDEF){//A union B
					fs->typeB = SUB;
					fs->setB = value;
					//printf("### SUB B %p %p\n",fs, value);
					this->lasttos = fs;
					this->cnode->ft->removeLast(this->cnode); 
	//this->lastFs = fs;
	
      }
      else{//A union B union C
	printf("### cannot happen\n");
	/* FullSelect * nfs = newFullSelect();
	    nfs->setA = fs->setB;
	    nfs->typeA = fs->typeB;
	    nfs->type = fs->typeB;
	    nfs->setB = value;
	    nfs->typeA = SUB;
	    fs->setB = nfs;
	    fs->typeA = FUL;
	    this->lastFs = nfs; printf("### 2\n");*/
	//this->cnode->ft->add(this->cnode,nfs);
      }
				}
				//this->cnode->ft->add(this->cnode,value);
				//this->currentType = SUB;
			//}
			//else{ //jetzt muss currentType==SEL gelten...
			//		printf("------hm\n"); 
			//}
			break;
		case SIGMA://funktioniert im Falle von () union () order by nicht!!!
			fs = this->cnode->ft->getLast(this->cnode);
    this->lastFs = NULL;
			//printf("SIGMA: %p %p\n",this->lasttos,this->lasttos->sigma);
			if(fs!=NULL&&fs->typeB!=UNDEF){
				//printf("lasttos\n");
				this->lasttos = fs;
				fs->sigma = (Selection*)value;
				//printf("****remove: %p\n",fs);
				//printf("****SIGMA %p\n",this->lasttos);
				this->cnode->ft->removeLast(this->cnode);//nun ist setB des bisherigen mutterknotens definiert. also wird dieses fs nicht mehr bearbeitet und kann vom stack genommen werden ==> Zeichnung fs-Stack
			}
			else{
				this->lasttos->sigma = (Selection*)value;
				//printf("****SIGMA %p\n",this->lasttos);
			}
			break;
//		case FINISHFS:
//			this->lasttos = fs;printf("hallo\n");
//			this->cnode->ft->removeLast(this->cnode); 
//			break;
		
		default:;	
		
	}
	//printf("OK\n");
	return OK;
}


/*die Anfrage, die durch den Baum, der in stmt repräsentiert wird, wird abhängig von der Art des Statements ausgwertet.*/
static int calculateStm(SqlStatement* this) {
	FullSelect* fs;
	Insert * ins;
	UpdIns * tupel; 
	int i,c,tos;
	AvlNode *p, **stack;
	ClassDef *cd;
	SubSelect * sb;
	Selection *si;
	AvlTree * t;
	Row *r;
	UtilList *al;
	Column * col;
	ExpressionLight *el;
	Call *call;
	UtilList * ul;
	UtilList * colList;

	switch(this->type){
		case CALL:
		  call = (Call*)this->stmt;
			
			ul = invokeMethod(this->db, call->tableName, call->procedureName, call->keyList,call->parameterList);
			
			if(ul!=NULL){
			  colList = newList();

			  r = newRow(ul->ft->size(ul));
			  for(el = (ExpressionLight*)ul->ft->getFirst(ul),i=0;el;el = (ExpressionLight*)ul->ft->getNext(ul),i++){
			    r->row[i] = el->value;
			    char * tmp = r->row[i];			    
			    while(*tmp!=0){
			      if(*tmp=='\n')
				*tmp=',';
			      *tmp++;
			    }// printf("--- ::: %s \n###%s\n",r->row[i], el->name);
			    colList->ft->append(colList,newColumn("CALL",NULL, el->name, NULL,el->sqltype,NKEY));
			  }	
			  t = newAvlTree(item_cmp);
			  t->insert(t,r);
			  Projection *pi = newProjection(ALL,colList);
			  this->rs->addSet(this->rs,t);
			  
			  this->rs->addMeta(this->rs,pi);
			  this->rs->sw->reason = "Successful Completion";
			  this->rs->sw->sqlstate = "00000";
			  return 0;
			  //this->rs->meta = "<EMPTY>";
			  //this->rs->tupel = "METHOD INVOKED";
			}else{
				this->rs->sw->reason = "SQL ROUTINE EXCEPTION";
				this->rs->sw->sqlstate ="2F000";	
				return 1;
			}
			break;
		case CREATE:
			cd = (ClassDef*)this->stmt;
			c = createClass(this->db,cd->className,cd->superclass,cd->fieldNameList);
			if(c==0){
				this->rs->meta = "<EMPTY>";
				this->rs->tupel	= "TABLE CREATED";
			}else{
				this->rs->sw->reason = "INTERNAL ERROR";
				this->rs->sw->sqlstate ="XX000";	
			}
			return c;
			break;
		case DELETE:
			tupel = (UpdIns*)this->stmt;
			//ein select basteln und ausfuehren mit where als sigma
			//dann darueber iterieren und jeweils das tupel loeschen
			sb = newSubSelect();
			sb->pi=newProjection(ALL,tupel->colList);
			//printf("--- 1\n");
			//printf("--- 2\n");
			sb->isSet = 1;
			sb->setName = tupel->tname; 
			si = newSelection(NULL,UNDEF);
				//si->where = tupel->where;
			sb->sigma = si;
				
			sb->addWhereList(sb,tupel->where);
			
			//printf("--- 3\n");
			
			t = sb->calculate(sb,this->db);		
			//iterativ durch den baum:
			//printf("--- 4\n");
			int size = 0;
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
					//Knoten vergleichen, falls ok neue row basteln und einfuegen
					//printf("3\n");
					al = newList();
					r = (Row*) p->item;
					col = (Column*)sb->pi->col->ft->getFirst(sb->pi->col);
					for(i=0;i<r->size;i++){
						if(r->row[i]==NULL||col->isKey!=KEY){
							//printf("Null ");
							col = (Column*)sb->pi->col->ft->getNext(sb->pi->col);
							continue;	
						}
						el = newExpressionLight(col->colName, UNDEF,r->row[i]);
						el->sqltype = col->colSQLType;
						al->ft->append(al, el);
						col = (Column*)sb->pi->col->ft->getNext(sb->pi->col);
						 
						
					}
					//printf("...\n");
					if((c=deleteInstance(this->db,tupel->tname,al))!=0){
						this->rs->sw->reason = "INTERNAL ERROR";
						this->rs->sw->sqlstate ="XX000";
						return 1;
					}
					al->ft->release(al);
					size++;
				}
	    	}
			//iterativ baum ends
			if(size==0){
				this->rs->meta = "<EMPTY>";
				this->rs->tupel	= "NO TUPEL DELETED";
			}
			else{
				if(c==0){
					this->rs->meta = "<EMPTY>";
					//this->rs->tupel	= "TUPEL DELETED";
					char * sizec = int2String(size);
					this->rs->tupel = (char *) malloc(strlen(" TUPEL DELETED")+strlen(sizec)+1);
					strcpy(this->rs->tupel,sizec);
					strcat(this->rs->tupel," TUPEL DELETED");
		
				}else{
					this->rs->sw->reason = "INTERNAL ERROR";
					this->rs->sw->sqlstate ="XX000";	
				}
			}
			
			
			return c;
			break;
		case INSERT:
			ins = (Insert*)this->stmt;
			
			for(tupel = ins->tupelList->ft->getFirst(ins->tupelList);tupel;tupel = ins->tupelList->ft->getNext(ins->tupelList)){
				//printf("insert... %d\n",ins->tupelList->ft->size(ins->tupelList));
				if((c=createInstance(this->db,ins->tname,tupel->assignmentList))!=0){
					if(c==CMPI_RC_ERR_ALREADY_EXISTS){
						this->rs->sw->reason = "WARNING:\nTupel already insered";
						this->rs->sw->sqlstate ="01000";
					}
					else{
						this->rs->sw->reason = "INTERNAL ERROR:\nINSERT into Table: error...";
						this->rs->sw->sqlstate ="XX000";
					}
					return 1;
				}
				
			}
			if(c==0){
				this->rs->meta = "<EMPTY>";
				this->rs->tupel	= "INSTANCE CREATED";
			}
			else{
				this->rs->sw->reason = "INTERNAL ERROR";
				this->rs->sw->sqlstate ="XX000";	
			}	
			return c;
			break;
		case UPDATE:
			tupel = (UpdIns*)this->stmt;
			//ein select basteln und ausfuehren mit where als sigma
			//dann darueber iterieren und jeweils das tupel loeschen
			sb = newSubSelect();
			sb->pi=newProjection(ALL,tupel->colList);
			si = newSelection(NULL,UNDEF);
			//si->where = tupel->where;
			sb->sigma = si;
			sb->isSet = 1;
			sb->setName = tupel->tname; 
			sb->addWhereList(sb,tupel->where);
			
			t = sb->calculate(sb,this->db);		
			//iterativ durch den baum:
			//printf("gelesen %s %p\n",tupel->tname,t->root);
	    	
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
					//Knoten vergleichen, falls ok neue row basteln und einfuegen
					//printf("3\n");
					al = newList();
					r = (Row*) p->item;
					col = (Column*)sb->pi->col->ft->getFirst(sb->pi->col);
					for(i=0;i<r->size;i++){
						if(r->row[i]==NULL||col->isKey!=KEY){
						//printf("Null ");
							col = (Column*)sb->pi->col->ft->getNext(sb->pi->col);
							continue;	
						}
						//printf("key: %s %s\n",col->colName, r->row[i]);
						el = newExpressionLight(col->colName, UNDEF,r->row[i]);
						el->sqltype = col->colSQLType;
						al->ft->append(al, el);
						col = (Column*)sb->pi->col->ft->getNext(sb->pi->col);

					}
					//printf("...\n");
					if((c=updateInstance(this->db,tupel->tname,tupel->assignmentList,al))!=0){
						this->rs->sw->reason = "INTERNAL ERROR:\nUPDATE WHERE: error...";
						this->rs->sw->sqlstate ="XX000";
						return 1;
					}
					al->ft->release(al);
				}
	    	}
			//iterativ baum ends
			if(c==0){
				this->rs->meta = "<EMPTY>";
				this->rs->tupel	= "TUPEL(S) UPDATED";
			}else{
				this->rs->sw->reason = "INTERNAL ERROR";
				this->rs->sw->sqlstate ="XX000";	
			}
		
			return c;
			break;
			break;
		case DROP:
			
			c = deleteClass(this->db,(char*)this->stmt);
			if(c==0){
				this->rs->meta = "<EMPTY>";
				this->rs->tupel	= "TABLE DROPED";
			}else{
				this->rs->sw->reason = "INTERNAL ERROR";
				this->rs->sw->sqlstate ="XX000";	
			}
			//printf("return DROP: %d\n",c);
			return c;
			break;
		case SELECT:
			fs = (FullSelect*) this->stmt;
			
			c = this->rs->addSet(this->rs,fs->calculate(fs,this->db));//mussvor addMeta ausgefuhert werden, da hier pi bei joins geandert wird
			this->rs->addMeta(this->rs,fs->pi);
			
			return c;
		default://Fehlermeldung, obwohl es nicht vorkommen sollte...
			this->rs->sw->reason = "FEATURE NOT SUPPORTED:\nNot supported";
				this->rs->sw->sqlstate ="0A000";
				return 1;	
			break;
	}
	
	
	
}

static void freeFSel(FullSelect** this) {
	FullSelect * t = *this;
	if(t->typeA==FUL){
		FullSelect * fs = (FullSelect*)t->setA;
		fs->free(&fs);	
	}
	if(t->typeA==SUB){
		SubSelect * sb = (SubSelect*)t->setA;
		sb->free(&sb);	
	}	
	if(t->typeB==FUL){
		FullSelect * fs = (FullSelect*)t->setB;
		fs->free(&fs);	
	}
	if(t->typeB==SUB){
		SubSelect * sb = (SubSelect*)t->setB;
		sb->free(&sb);	
	}
	//FREE(t->setA);
	//FREE(t->setB);
	//FREE(t->sigma);//das ist eine Kopie eins sigmas aus einem subselect
	//FREE(t->pi);
}
int min(int a, int b){
	if(a<b)
		return a;
	else return b;
}

/*nächsten drei Funktionen führen Fullselect-Operaionen auf dem Baum aus.*/
AvlTree* uniontree(AvlTree *t1, AvlNode * node,int type){
	
	if(node!=NULL){
		uniontree(t1,node->left,type);
		Row * r1 = (Row*)node->item;
		Row * r2 = (Row*) t1->insert(t1,node->item);
		if(r2!=NULL&&type==UNIONALL){
			//printf("doublette++\n");
			r2->doublette += r1->doublette;
		}	
		uniontree(t1,node->right,type);
	}
	return t1;
}

AvlTree* excepttree(AvlTree *t1, AvlNode * node,int type){
	
	if(node!=NULL){
		excepttree(t1,node->left,type);
		Row * r1 = (Row*)node->item;
		Row * r2 = (Row*) t1->delete(t1,node->item);
		if(r2!=NULL&&type == EXCEPTALL){
			r2->doublette-=r1->doublette;
			//printf("doublette-- \n");
			if(r2->doublette>0)
				t1->insert(t1,node->item);
		}	
		excepttree(t1,node->right,type);
	}
	return t1;
}

AvlTree* intersecttree(AvlTree *res, AvlTree *t1, AvlNode * node,int type){
	
	if(node!=NULL){
		intersecttree(res,t1,node->left,type);
		Row * r1 = (Row*)node->item;
		Row * r2 = (Row*) t1->delete(t1,node->item);
		if(r2!=NULL){
			res->insert(res,r2);
			if(type == INTERSECTALL)
				r2->doublette=min(r1->doublette,r2->doublette);
			else
				r2->doublette=1;
		}
		intersecttree(res,t1,node->right,type);
	}
	return res;
}



void changeSigma(AvlNode * node, Selection *sigma){
	if(node!=NULL){
		changeSigma(node->left,sigma);
		Row* r = (Row*)node->item;
		r->sigma = sigma;//man kann hier nicht einfach einen **xy umbigen auf neues Sigma für alle Rows auf einma, denn die einzelnen Rows können aus unterschiedlichen Relationen stammen
		changeSigma(node->right,sigma);
	}
}


/*das Subselect ist Teil eines anderen Subselects und das ist anders geornet, daher muss auch dieses umsortiert werden*/
AvlTree* reorder(AvlTree *t, Selection *sigma){
	//printf("+++ roerder 1\n");
	changeSigma(t->root,sigma);
	//printf("+++ roerder 2\n");
	AvlTree *res = newAvlTree(item_cmp);
	//printf("+++ roerder 3\n");
	copytree(res,t->root);
	//printf("+++ roerder 4\n");
	return res;
}

/*kürzt den Baum auf ff Elemente*/
static AvlTree * fetchfirst(AvlTree * t,int ff){
	AvlTree *ot = newAvlTree(item_cmp);
	int i;
	//printf("+++++ order\n");
	for(i=0; i<ff;){
		Row * r = (Row*)t->deleteMin(t);
		//printf(">>>a %d %d,\n",r->doublette,ff);
		i += r->doublette;
		if(i>ff)
			r->doublette = r->doublette - i + ff;
		if(r!=NULL)
			ot->insert(ot,r);
		//printf(">>>b %d %d,\n",r->doublette,ff);
	}
	//traverseDelRow(t->root);
	t->free(&t);
	return ot;
}

/*zwei Select-Statements werden über die Operation verknüpft zu einem. Fallunterschedungen, je nachdem, welchen Typ die einzenen Operanden haben*/
static AvlTree* calculateFSel(FullSelect* this,char *db) {
  //printf("calculateFSel() %p\n",this);
	AvlTree* avlt;
	 
	if(this->type==EMPTY){
		if(this->typeA==SUB){ 
		  //s			printf("calculateFSel()2\n");
			SubSelect *sbs = (SubSelect*)this->setA;
			//printf("is set: %d\n",sbs->isSet);
			//printf("calculateFSel()3\n");
			avlt = sbs->calculate(sbs,db);
			if(avlt==NULL)
				return NULL;
		}
		else{
			//printf("geklammertes select\n");
			FullSelect *fs = (FullSelect*)this->setA;
			avlt = fs->calculate(fs,db);
			if(avlt==NULL)
				return NULL;

		} 
	}
	if(this->type==UNION||this->type==UNIONALL){
		//printf("+++ 1\n");
		AvlTree *t1, *t2;
		if(this->typeA==SUB){ 
			//printf("+++ 2a\n");
		
			SubSelect *sbs = (SubSelect*)this->setA;
			//printf("+++ %p\n",sbs);
			t1 =  sbs->calculate(sbs,db);
			if(t1==NULL)
				return NULL;
		}
		else{
			//printf("+++ 2b\n");
		
			FullSelect *fs = (FullSelect*)this->setA;
			t1 = fs->calculate(fs,db);
			if(t1==NULL)
				return NULL;
		}
		if(this->typeB==SUB){
			//printf("+++ 3a\n");
		
			SubSelect *sbs = (SubSelect*)this->setB;
			//printf("+++ %p\n",sbs);
			t2 =  sbs->calculate(sbs,db);
			//printf("+++ 3a2 %p\n",t2);
			if(t2==NULL)
				return NULL;
		}
		else{
			//printf("+++ 3b\n");
		
			FullSelect *fs = (FullSelect*)this->setB;
			t2 = fs->calculate(fs,db);
			if(t2==NULL)
				return NULL;
		}
		//printf("+++ 4\n");
		
		avlt = uniontree(t1,t2->root,this->type);
	}
	if(this->type==EXCEPT||this->type==EXCEPTALL){//liefert eine Rel<tion zuviel !!!
		//printf("+++ 1\n");
		AvlTree *t1, *t2;
		if(this->typeA==SUB){ 
			//printf("+++ 2a\n");
		
			SubSelect *sbs = (SubSelect*)this->setA;
			//printf("+++ %p\n",sbs);
			t1 =  sbs->calculate(sbs,db);
			if(t1==NULL)
				return NULL;
		}
		else{
			//printf("+++ 2b\n");
		
			FullSelect *fs = (FullSelect*)this->setA;
			t1 = fs->calculate(fs,db);
			if(t1==NULL)
				return NULL;
		}
		if(this->typeB==SUB){
			//printf("+++ 3a\n");
		
			SubSelect *sbs = (SubSelect*)this->setB;
			//printf("+++ %p\n",sbs);
			t2 =  sbs->calculate(sbs,db);
			if(t2==NULL)
				return NULL;
		}
		else{
			//printf("+++ 3b\n");
		
			FullSelect *fs = (FullSelect*)this->setB;
			t2 = fs->calculate(fs,db);
			if(t2==NULL)
				return NULL;
		}
		//printf("+++ 4\n");
		
		avlt = excepttree(t1,t2->root,this->type);
	}
	if(this->type==INTERSECT||this->type==INTERSECTALL){
		//printf("+++ 1\n");
		AvlTree *t1, *t2;
		if(this->typeA==SUB){ 
			//printf("+++ 2a\n");
		
			SubSelect *sbs = (SubSelect*)this->setA;
			//printf("+++ %p\n",sbs);
			t1 =  sbs->calculate(sbs,db);
			if(t1==NULL)
				return NULL;
		}
		else{
			//printf("+++ 2b\n");
		
			FullSelect *fs = (FullSelect*)this->setA;
			t1 = fs->calculate(fs,db);
			if(t1==NULL)
				return NULL;
		}
		if(this->typeB==SUB){
			//printf("+++ 3a\n");
		
			SubSelect *sbs = (SubSelect*)this->setB;
			//printf("+++ %p\n",sbs);
			t2 =  sbs->calculate(sbs,db);
			if(t2==NULL)
				return NULL;
		}
		else{
			//printf("+++ 3b\n");
		
			FullSelect *fs = (FullSelect*)this->setB;
			t2 = fs->calculate(fs,db);
			if(t2==NULL)
				return NULL;
		}
		//printf("+++ 4\n");
		AvlTree *res = newAvlTree(item_cmp); 
		avlt = intersecttree(res,t1,t2->root,this->type);
	}
	//printf("+++ %p\n", this);
	if(this->sigma!=NULL){
		
		//if(this->sigma->orderBy==NULL)
			//printf("NULL2!\n");
		//falls nur select: reorder überflüssig, wird hier dann aber dennoch ausgeführt
		if(this->sigma->orderBy!=NULL){
			//printf("+++++ reorder\n");
			avlt = reorder(avlt,this->sigma);
		}
		if(this->sigma->fetchFirst!=UNDEF)
			avlt = fetchfirst(avlt,this->sigma->fetchFirst);
	}
	//printf("...\n");
	return avlt;
}

static void freeSSel(SubSelect** this) {
	SubSelect* t = *this;
	//printf("freeSSel 1\n");
	t->cross->free(&(t->cross));
	//printf("freeSSel 2\n");
	//FREE(t->aliasName);
	//printf("freeSSel 3\n");
	//FREE(t->setName);
	//printf("freeSSel 4\n");
	t->pi->free(&(t->pi));
	//printf("freeSSel 5\n");
	if(t->set!=NULL)
		t->set->free(&(t->set));
	//printf("freeSSel 6\n");
	//FREE(t->set);
	//printf("freeSSel 7\n");
	t->sigma->free(&(t->sigma));
	//printf("freeSSel 8\n");
	//FREE(t->sigma);
	//printf("freeSSel 9\n");
	FREE(t);
}




static void addNodeSSel(SubSelect* this,int type, void* value){
	switch(type){
		case PRJ:
			this->pi = (Projection*)value;
			break;
		
		default:;
	}
	
}

/*Für die Kodierung im DB Protokoll müssen alle Werte in eine Zeichenkette konvertiert werden*/
static char * value2string(CMPIData d){
  char str[128];
   	char *sp = str;
		
      if ((d.type & (CMPI_UINT|CMPI_SINT))==CMPI_UINT) {
         unsigned long long ul;
         switch (d.type) {
         case CMPI_uint8:
            ul = d.value.uint8;
            break;
         case CMPI_uint16:
            ul = d.value.uint16;
            break;
         case CMPI_uint32:
            ul = d.value.uint32;
            break;
         case CMPI_uint64:
            ul = d.value.uint64;
            break;
         }
         sprintf(sp,"%llu", ul);
      }
      
      else if (d.type & CMPI_SINT) {
         long long sl;
          switch (d.type) {
         case CMPI_sint8:
            sl = d.value.sint8;
            break;
         case CMPI_sint16:
            sl = d.value.sint16;
            break;
         case CMPI_sint32:
            sl = d.value.sint32;
            break;
         case CMPI_sint64:
            sl = d.value.sint64;
            break;
         }
         sprintf(sp,"%lld", sl);
      }
      
      else if (d.type==CMPI_real32) {
         sprintf(sp,"%.7e", d.value.real32);
      }
      
      else if (d.type==CMPI_real64) {
         sprintf(sp,"%.16e", d.value.real64);
      }

      else if (d.type == CMPI_boolean)
         sprintf(sp,"%s", d.value.boolean ? "TRUE" : "FALSE");
      else if (d.type == CMPI_char16)
         sprintf(sp,"%c", d.value.char16);
      else if (d.type == CMPI_chars) {
         sprintf(sp,"%s",d.value.chars);
        
      }
      else if (d.type == CMPI_string) {
    //sprintf(sp,"%s",d.value.string->hdl);
    return d.value.string->hdl;
      }
      else if (d.type == CMPI_dateTime) {
         if (d.value.dateTime) {
            CMPIString *sdf = CMGetStringFormat(d.value.dateTime, NULL);
             sprintf(sp,"%s",(char *) sdf->hdl);
         }
         
      }
  //printf("--- %s ---\n",sp);
  return sp;//strdup(sp);

}

static void extractValue(CMPIData d, UtilStringBuffer * sb){

 
  if (d.type & CMPI_ARRAY) {
    // sprintf(sp,"**[]**"); 
    CMPIArray *ar = d.value.array;
    CMPIData da;
    int j, ac = CMGetArrayCount(ar, NULL);
    
    for (j = 0; j < ac; j++) {
      da = CMGetArrayElementAt(ar, j, NULL);
      
      extractValue(da, sb);
      if(j<ac-1)
	sb->ft->appendChars(sb,",");
    }
    return; 
    
  }
  else {
    sb->ft->appendChars(sb,value2string(d));				      
    
   	}
}

/*
 * Problem: nicht sonderlich effizient!!!
 * Lösbar dadurch, dass man sich den extra Providercall in getClassDef
 * spart und die Spalteninfos über die gewonnenen Infos mit enumInst bekäme 
 * (sollte man machen, da dann nur ein Providercall notwendig), allerdings 
 * dauert ein enumInst länger und ist unnötig, wenn ein Syntaxfehler vorliegt)
 * Das kann man aber nicht machen, da enumInst nicht alle Properties zurückgibt!
 */
static int getColumnNr(char * name,UtilList * ul){
	Column *col;
	int i;
	for(i=0,col = (Column*) ul->ft->getFirst(ul);col;col = (Column*) ul->ft->getNext(ul),i++){
		if(strcasecmp(col->colName,name)==0){
			return i;	
		}
	}
	return -1;
}

static int getColNr(Column * colA,UtilList * ul){
	Column *col;
	int i;
	for(i=0,col = (Column*) ul->ft->getFirst(ul);col;col = (Column*) ul->ft->getNext(ul),i++){
		if(coleql(colA,col)==0){
			return i;
			
		}
			
	}
	return -1;
}

/*in eine Row müssen nicht nur die Spalten, die in der Projektion definiert werden gespeichert werden, sondern auch, die, die in einer Bedingung vorkommen
select a from X where b<n
*/
static int testSigma(Projection *pi,Row * row){
	int res = UNDEF;
	UtilList *ul = row->sigma->where;
	if(ul==NULL||ul->ft->size(ul)==0)//kein Sigma also alle
		return 1;
	Sigma *sigma;
	for(sigma = (Sigma*)ul->ft->getLast(ul); sigma; sigma = (Sigma*)ul->ft->getPrevious(ul)){
		//printf(">1>> %d %p\n",sigma->link,sigma);
		if(!(sigma->link==AND&&res==0)&&!(sigma->link==OR&&res==1)){
					
			int nr = getColNr(sigma->colA,pi->col);
			int cmp;
			
			if(sigma->op==ISNULL||sigma->op==NOTNULL)
				cmp = strcasecmp(row->row[nr],"NULL");
			else if(sigma->op==BETWEEN||sigma->op==NOTBETWEEN){
				double v = strtod(row->row[nr],(char**)NULL);
				double a = strtod(sigma->value,(char**)NULL);
				double b = strtod(sigma->valueB,(char**)NULL);
				//printf("%f %f %f\n",a,v,b);
				if(v>a && v<b)cmp=0; 	
			}
			else{
				cmp = row_cmp(row->row[nr],sigma->value,sigma->colA->colSQLType,ASC);
			}
			int cmp_op = 0;
			switch(sigma->op){
				case ISNULL:
				case BETWEEN: //printf("between\n");
				case EQ: if(cmp==0) cmp_op = 1; break;
				case NOTNULL:
				case NOTBETWEEN:
				case NE: if(cmp!=0) cmp_op = 1; break;
				case GT: if(cmp>0) cmp_op = 1; break;
				case LT: if(cmp<0) cmp_op = 1; break;
				case GE: if(cmp>=0) cmp_op = 1;	break;
				case LE: if(cmp<=0) cmp_op = 1; break;
				
			}
			if(res==UNDEF){
				res = cmp_op;	
			}
			else if(sigma->link==UNDEF){
				res = cmp_op;
			}
			else if(sigma->link==AND){
				//printf("AND %d    ",cmp_op);
				res = (res==1)? cmp_op:0;
				//printf("AND %d %d\n",res,cmp_op);
			}
			else if(sigma->link==OR){
			//	printf("OR %d %d\n",res, cmp_op);
				if(res!=1)
					res = cmp_op;
				//printf("\tOR %d %d\n",res, cmp_op);
					
			}
		}
		
		//printf(">2>> %p\n",sigma);
	}
	return res;
}


/*berechnet das Kreuzprodukt zweier Bäume*/
void traversecross(AvlTree* res,AvlNode * node, AvlTree* t,UtilList *ul, int type){
	if(node!=NULL){
		//printf("1\n");
		traversecross(res,node->left,t,ul,type);
		//printf("2\n");
		
		Row *r1 = (Row*) node->item;
		Row *r2, *rn;
		//iterativ durch den 2. baum:
		//katastrohales laufzeitverhalten: geht durch alle durch.
		//ein reorder s.u. und Abbruch waere effizienter.
		//aber: beachte NULL-Werte
		int tos;
    	AvlNode *p, **stack;
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
				//Knoten vergleichen, falls ok neue row basteln und einfuegen
				//printf("3\n");
		
				r2 = (Row*) p->item;
				//printf("=== %p %p\n",r1,r2);
				int cmp = 0;
				int *si;
				//int si[3] = *_si;
				for(si = (int*)ul->ft->getFirst(ul);si;si = (int*)ul->ft->getNext(ul)){
					int cmp_t = -1;
					//printf("3a %d %d %d\n",i, si[0],si[1]);
					//printf("3a %d %p\n",i, r1->row[si[0]]);//invalid read
		
					switch(type){
						case LEFT: 
							if(r1->row[si[0]]!=NULL)
								cmp_t = strcmp(r1->row[si[0]],"NULL"); 
							else
								cmp_t = 0;
							break;
						case RIGHT: 
							if(r2->row[si[0]]!=NULL)
								cmp_t = strcmp(r2->row[si[1]],"NULL"); 
							else
								cmp_t = 0;
							break;
						case FULL:
							if(r1->row[si[0]]!=NULL)
								cmp_t = strcmp(r1->row[si[0]],"NULL");
							else
								cmp_t = 0;
							if(cmp_t!=0)
								cmp_t = strcmp(r2->row[si[1]],"NULL");
							break;
					}
					//was, wenn beide NULL???
					//printf("3b %p %p\n",r1->row[si[0]],r2->row[si[1]]);
					//printf("3b1 %d %d %d %d\n",si[0],si[1],r1->size,r2->size);
					if(cmp_t!=0)
						if(r1->row[si[0]]!=NULL&&r2->row[si[1]]!=NULL){
							//printf("...\n");printf("3b1a %s %s\n",r1->row[si[0]],r2->row[si[1]]);
							//cmp_t= strcmp(r1->row[si[0]],r2->row[si[1]]);
						}
					//printf("3b2\n");
					if(cmp_t==0){
						switch(si[2]){
							case UNDEF: cmp = cmp_t; break;
							case OR: if(cmp_t==0) cmp = 0; break;
							case AND: if(cmp==1&&cmp_t==1) cmp = 1; else cmp = 0; break;	
						}
						
					}
					//printf("3c\n");
					
							   
				}
				//printf("4\n");
		
				if(cmp==0){//gilt auch, wenn keine Bedingung angegben wurde also wirkliches kreuzprodukt berechnet wird
					//printf("4a 1\n");
					rn = newRow(r1->size+r2->size-ul->ft->size(ul));
					//printf("Anzahl der cols: %d\n",rn->size);
					//printf("4a 1a\n");
					//rn->doublette = r1->doublette + r1->doublette;
					int j;
					//printf("4a 1b\n");
					//printf("4a 2 %d %d %d\n",r1->size,r2->size,rn->size);
					
					for(j=0;j<r1->size;j++){
						//printf("%d %p %p\n",j,rn->row[j],r1->row[j]);
						if(r1->row[j]==NULL)
							continue;
						rn->row[j] = (char*) malloc(strlen(r1->row[j])+1);
						strcpy(rn->row[j],r1->row[j]);
						//printf(" %d %s\n",j,r1->row[j]);	
					}
					int k = r1->size;
					//printf("4a 3\n");
					
					for(j=0;j<r2->size;j++){
						//printf("4a 3 %d\n",j);
						
						int ignore = 0;
						//printf("4a 3a %d\n",j);
						for(si = (int*)ul->ft->getFirst(ul);si; si = (int*)ul->ft->getNext(ul)){
							if(j==si[1])
								ignore = 1;
							
						}
						//printf("4a 3b %d\n",j);
						if(ignore==0){
							if(r2->row[j]!=NULL){
								rn->row[k] = (char*) malloc(strlen(r2->row[j])+1);
								strcpy(rn->row[k],r2->row[j]);
							}
							k++;	
						}
						else
							ignore = 0;
						//printf("4a 3c %d\n",j);	
					}
				//	printf("4a 4...\n");
					rn->sigma = r1->sigma;
					t->insert(res,rn);
			//		printf("4a 5\n");
					//r1->free(&r1);  
			//		printf("4a 6\n");
				}
		//		printf("5\n");
		
			}
    	}
		//iterativ baum ends
		
		//freeRow()
		traversecross(res,node->right,t,ul,type);
	}
	//printf("node is NULL\n");	
}

/*siehe traversecross*/
AvlTree * crossproduct(AvlTree* setA,AvlTree* setB,UtilList  *ul, int type){
	//ein reorder auf setB waere hier sinnvoll damit man die suche auch abbrechen kann. 
	AvlTree* res = newAvlTree(item_cmp);
	//printf("traverse los\n");
	traversecross(res,setA->root,setB,ul,type);
	//printf("cross ok\n");
	//traverseDelRow(setA->root);
	//traverseDelRow(setB->root);
	//setA->free(&setA);
	//setB->free(&setB);
	//fetch first gibts nicht , da das ja nochmals teil eines joins sein kann, der anders sortiert ist
	return res;
}

/*berechnet eine Ergebnismenge aus allen darunterliegenden Subselects. Rekursiv*/
static AvlTree* calculateSSel(SubSelect* this,char * db) {
	//printf("-- 0\n");
	if(this->isSet!=1){
		AvlTree *setA, *setB;
		if(this->cross->setA!=NULL){
			//printf("-- 1A\n");
			setA = this->cross->setA->calculate(this->cross->setA);
			if(setA==NULL)
				return NULL;
		}
		if(this->cross->setB!=NULL){
			//printf("-- 1B\n");
			setB = this->cross->setB->calculate(this->cross->setB);
			if(setB==NULL)
				return NULL;
		}
		
		//printf("-- cross-product\n");
		UtilList * ul = newList();
		Sigma *sigma;
		if(this->sigma->where!=NULL&&this->sigma->where->ft->size(this->sigma->where)>0){
			sigma = (Sigma*)this->sigma->where->ft->getFirst(this->sigma->where);
			//printf("1\n");
			for(sigma = (Sigma*)this->sigma->where->ft->getFirst(this->sigma->where);sigma;sigma = (Sigma*)this->sigma->where->ft->getNext(this->sigma->where)){
				if(sigma->colB!=NULL){//also join
					int si[3] = {getColumnNr(sigma->colA->colName,this->cross->setA->pi->col),getColumnNr(sigma->colB->colName,this->cross->setB->pi->col),sigma->link};
					ul->ft->append(ul,&si);
					//in this->pi muessen die doppelten geloescht werden 
			
					Column *tcol;
					//printf("1\n");
					for(tcol = (Column*)this->pi->col->ft->getFirst(this->pi->col);tcol;tcol= (Column*)this->pi->col->ft->getNext(this->pi->col)){
						if(strcasecmp(tcol->colAlias,sigma->colB->colAlias)==0){
							this->pi->col->ft->removeCurrent(this->pi->col);
							//printf("col %s aus pi loeschen\n",tcol->colAlias);
							break;
						}				
						 	
					}			
				}
			}
		}
		//printf("und los\n");
		AvlTree * cpt = crossproduct(setA,setB,ul,this->cross->type);
		//printf("und fertig %d\n",this->pi->col->ft->size(this->pi->col));
		//printf("fs-pi %p\n",this->pi);
		//es muss nun aus den eleminerten Spalten gebastelt werden.
		return cpt;
	}
	else{
		int rcount = 0;
		int i;
		//printf("-- 1\n");
		//sigma-operationen!!!
		if(this->set == NULL)
			this->set = newAvlTree(item_cmp);
		//printf("------ %p  %p\n",this, this->set);
		//CMPIObjectPath *cop;
		CMPIInstance *ci;
		
		CMPIEnumeration *enm;//noChunking = 1;
	//	enm = enumInst("root/cimv2", this->setName);
	//	enm = enumInstss("root/cimv2", this->setName);
		enm = enumInstances(db, this->setName);
		//printf("-- 2\n");
		int wcount = 0;
		
		if(enm == NULL){
			//printf("dumm\n");
			return NULL;	
		}
		else while (CMHasNext(enm, NULL)) {
			wcount++;
			//printf("-- 3 %d\n",wcount);
		
			ci = CMGetNext(enm, NULL).value.inst;
		//	cop = CMGetObjectPath(ci, NULL);
			ClInstance *inst = (ClInstance *) ci->hdl;
			int i, mm = this->pi->col->ft->size(this->pi->col), m = ClInstanceGetPropertyCount(inst);//offenbar gibt es Felder, die keine Werte haben (und nicht NULL sind); diese werden nicht im Couint berücksichtigt
			//char *trow[m];
			Row *row = newRow(mm);
			//printf("Anzahl der cols: %d\n",mm);
			UtilStringBuffer *sb = newStringBuffer(4096);
				
			for (i = 0; i < m; i++) {
				CMPIString *name;
				CMPIData d;
				   
				d = CMGetPropertyAt(ci, i, &name, NULL);
				sb->ft->reset(sb);
				extractValue(d,sb);
				int cnr = getColumnNr(name->ft->getCharPtr(name,NULL),this->pi->col);
				if(cnr!=-1){
					//printf("--- %d %d %d\n",wcount, i, cnr);
					//copy(row->row[cnr],(char*)sb->hdl);
					row->row[cnr] = malloc(strlen((char*)sb->hdl)+1);
					strcpy(row->row[cnr],(char*)sb->hdl);	 
				}
				CMRelease(name);
			}
			row->sigma = this->sigma;

			//printf("---------- %d %d\n", rcount,this->sigma->fetchFirst);
			//printf("%p\n",this->sigma);
			//if(this->sigma!=NULL){//==NULL bei Joins!?
				if(this->sigma->orderBy==NULL||this->sigma->orderBy->ft->size(this->sigma->orderBy)==0){
				//printf("a 0\n");
					if(this->sigma!=NULL&&rcount==this->sigma->fetchFirst)
						break;
				}
				//printf("a\n");
			
				if(!testSigma(this->pi,row))
					continue;
			//}printf("b %p %p\n",this->set,row);		
			//printf("b1\n");
			Row *r = (Row*)this->set->insert(this->set,row);
			
			//printf("b2\n");
			if(r!=NULL){
				if(this->pi->mode==ALL)
					r->doublette++;
			  else
			    rcount--;
			}
			//printf("%d ",row->size);
			rcount++;
			//printf("c\n");
		}
		//printf("forend\n");
		//printf("rcount: %d %d ",rcount, this->sigma->fetchFirst);
		if(this->sigma->fetchFirst>0)
			rcount = this->sigma->fetchFirst;
		
		//fetch-first auf geordneter ergebnismenge
		if(this->sigma->orderBy!=NULL&&this->sigma->orderBy->ft->size(this->sigma->orderBy)>0){
			AvlTree *ot = newAvlTree(item_cmp);
			//printf("+++++ order\n");
			for(i=0; i<rcount;){
				Row * r = (Row*)this->set->deleteMin(this->set);
				i += r->doublette;
				if(i>rcount)
					r->doublette = r->doublette - i + rcount;
				if(r!=NULL)
					ot->insert(ot,r);	
			}
			
			//printf("%d\n",i);
		//	traverseDelRow(this->set->root);
			this->set->free(&(this->set));
			this->set = ot;
		}	
		
		
	//	printf("calculateSSel() end %d\n", wcount);
	//	treetoString(this->set->root);
	//	printf("calculateSSel() end %d\n", wcount);
		
		return this->set;	
	}
}

static void freePrj(Projection** this) {
	Projection* t = *this;
	//int i;
	//werden bereits in den Selects gelöscht
//	if(t->col!=NULL){
//		Column * col;
//		col = (Column*)t->col->ft->getFirst(t->col);
//		for(i=0;i<t->col->ft->size(t->col);i++){
//			col->free(&col);
//			col = (Column*)t->col->ft->getNext(t->col);
//		}
//	}
	t->col->ft->release(t->col);
	//FREE(t->col);
	FREE(t);
}

static void freeExL(ExpressionLight** this) {
	ExpressionLight* t = *this;
	FREE(t);
}
static void freeOr(Order** this) {
	Order* t = *this;
	//prtintf("ob\n");
	FREE(t);
}

static void freeUpd(UpdIns** this) {
	UpdIns* t = *this;
	
	//TODO!!!!!
	
	//prtintf("ob\n");
	FREE(t);
}
static void freeIns(Insert** this) {
	Insert* t = *this;
	
	//TODO!!!!!
	
	//prtintf("ob\n");
	FREE(t);
}

static void freeRow(Row** this) {
	Row* t = *this;
	//printf("freeRow 1 %p\n",t);
	if(t->sigma!=NULL){
		//printf("freeRow 1 %p\n",t->sigma);
		t->sigma->free(&(t->sigma));
	}
	//printf("freeRow 2\n");
	
	int i;
	for(i=0;i<t->size;i++)
		FREE(t->row[i]);
	//printf("freeRow 3\n");
		
	FREE(t->row);
	//printf("freeRow 4\n");
	FREE(t);
}

static void freeCol(Column** this) {
	//printf("freeCol 1\n");
	Column* t = *this;//die sind vermutlich nicht allocated sondern einfach kopiert --> aendern!!!!
	//printf("freeCol 2\n");
	//FREE(t->colAlias);
	//FREE(t->tableName);
	//FREE(t->colName);
	//FREE(t->colSQLType);
	//printf("col 3\n");
	FREE(t);
	//printf("freeCol 4\n");
	
}

static void freeCJoin(CrossJoin** this) {
	CrossJoin* t = *this;
	if(t->setA!=NULL)
		t->setA->free(&(t->setA));
	//FREE(t->setA);
	if(t->setB!=NULL)
		t->setB->free(&(t->setB));
	//FREE(t->setB);
	FREE(t);
}
static void freeSi(Sigma** this) {
	//printf("ssffs %p\n",this);
	//auf colA und colB muss kein free aufgerufen werden, da die Cols alle in den Selects behandelt werden 
	Sigma* t = *this;
	//FREE(t->colA);
	printf("freeSi 2\n");
	//FREE(t->colB);
	//printf("freeSi 3\n");
	FREE(t->value);
	//printf("freeSi 4\n");
	FREE(t->valueB);
	//printf("freeSi 5\n");
	FREE(t);
}

/*anders realisiert, siehe oben...*/
static AvlTree* calculateCJoin(CrossJoin* this) {

	return NULL;
}


static void setWarning(SqlWarning *this, char* s, char *r){
	this->reason = r;
	this->sqlstate = s; 
}

static void setWarningRs(ResultSet *this, char* s, char *r){
	this->sw->reason = r;
	this->sw->sqlstate = s;
}

void setSqlWarning(char* s, char *r){
	rs->sw->reason = r;
	rs->sw->sqlstate = s;
}

//Sollte eine Object-Methode werden!!!
void setMeta(char *meta){
	rs->meta = meta;  
}


int sqlInput(char* buffer, int* done, int requested)
{	
    int left = strlen(q) - ofs;
    if (left == 0) {
      *done = 0;
      return 0;
    }
    if (left < requested) requested = left;
    memcpy(buffer,q+ofs,requested);
    ofs += requested;
    *done = requested;
    return *done;
    
}


/*wird vom SQL-Prozessor aufgerufen, wenn eine Anfage (Komanndo der Gruppe 2) bearbeitet werden soll*/
ResultSet *createRS(char *query, int *rc){//wird CommHndl noch benutzt?

	int p;
	
 	rs = newResultSet(query);
	SqlStatement * stm = newSqlStatement(UNDEF,rs);
	
	q = query;
   	ofs = 0;

    p = sfcSqlparse(stm);
  	//printf("%d %d und los\n",count,wcount);
	if(p==0)
		p = stm->calculate(stm);
	//printf("return aus statement %d\n",p);
	//printf("-%s-",stm.rs->tupel);
   	stm->free(&stm);
   	//printf("alles befreit\n");
   	*rc = p;
   	return rs;
}






char * type2cmpiString(int type){
	switch (type & ~CMPI_ARRAY) {
   		case CMPI_chars:
   		case CMPI_string:
      		return "string";
   		case CMPI_sint64:
      		return "sint64";
   		case CMPI_uint64:
      		return "uint64";
   		case CMPI_sint32:
      		return "sint32";
   		case CMPI_uint32:
      		return "uint32";
   		case CMPI_sint16:
      		return "sint16";
   		case CMPI_uint16:
      		return "uint16";
   		case CMPI_uint8:
      		return "uint8";
   		case CMPI_sint8:
      		return "sint8";
   		case CMPI_boolean:
      		return "boolean";
   		case CMPI_char16:
      		return "char16";
   		case CMPI_real32:
      		return "real32";
   		case CMPI_real64:
      		return "real64";
   		case CMPI_dateTime:
      		return "datetime"; 
   		case CMPI_ref:
      		return "*";
   		case CMPI_instance:
      		return "%";
   	}
   	return "*??*";
}


/*Hilfsfunktion für enumInstances*/
CMPIEnumeration * createEnum(BinRequestContext * binCtx, BinResponseHdr ** resp, int arrLen){
   	int i, c, j;
   	void *object;
   	CMPIArray *ar;
   	CMPIEnumeration *enm;
   	CMPIStatus rc;


   	ar = NewCMPIArray(arrLen, binCtx->type, NULL);
   	for (c = 0, i = 0; i < binCtx->rCount; i++) {
      	for (j = 0; j < resp[i]->count; c++, j++) {
         	object = relocateSerializedInstance(resp[i]->object[j].data);
         	rc=arraySetElementNotTrackedAt(ar,c, &object, binCtx->type);//object???.
      	}
   	}

    enm = sfcb_native_new_CMPIEnumeration(ar, NULL);
	
   	ar->ft->release(ar);
	return enm;
}


/*Es folgt die Implementation der Schnittstelle zu den Providern. Genauere Erklärung siehe Tabelle in der Arbeit*/

CMPIEnumeration * enumInstances(char * ns, char * cn) {

 	RequestHdr * hdr = NULL;
   	XtokEnumInstances reqq;
	reqq.op.count = 2;
    reqq.op.type = OPS_EnumerateInstances;
	reqq.op.nameSpace = setCharsMsgSegment(ns);
    reqq.op.className = setCharsMsgSegment(cn);
    reqq.flags = 0;//
	reqq.propertyList = NULL;
    reqq.properties = 0;

	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokEnumInstances);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_EnumerateInstances;
	
	hdr = &hdrq;
	
   	CMPIObjectPath *path;
   	EnumInstancesReq *sreq;
   	int irc, l = 0, err = 0,sreqSize=sizeof(EnumInstancesReq)-sizeof(MsgSegment);
   	BinResponseHdr **resp;
   	BinRequestContext binCtx;

   	memset(&binCtx,0,sizeof(BinRequestContext));

   	XtokEnumInstances *req = (XtokEnumInstances *) hdr->cimRequest;

   	sreq=calloc(1,sreqSize);
   	sreq->hdr.operation=OPS_EnumerateInstances;
   	sreq->hdr.count=req->properties+2;

   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
   	sreq->principal = setCharsMsgSegment(NULL);//ctx->principal
   	sreq->objectPath = setObjectPathMsgSegment(path);

   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq->hdr;
   	binCtx.bHdr->flags = req->flags;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sreqSize;
   	binCtx.type=CMPI_instance;
   	binCtx.xmlAs=binCtx.noResp=0;

    hdr->chunkedMode=binCtx.chunkedMode=0;
   	binCtx.pAs=NULL;

   	irc = getProviderContext(&binCtx, (OperationHdr *) req);
   	//printf("msgx %d\n",irc);
   	if (irc == MSG_X_PROVIDER) {

    	resp = invokeProviders(&binCtx, &err, &l);
      	closeProviderContext(&binCtx);
      	free(sreq);
      	//printf("%d\n",err);
     	if (err == 0) return createEnum(&binCtx, resp, l);
      	return NULL;
   	}
   	closeProviderContext(&binCtx);
   	free(sreq);
  	return NULL;
}


int  createInstance(char * ns, char * cn, UtilList * ins) {
	RequestHdr * hdr = NULL;
   	
   	XtokCreateInstance reqq;
	reqq.op.count = 2;
    reqq.op.type = OPS_CreateInstance;
    reqq.op.nameSpace=setCharsMsgSegment(ns);
    reqq.op.className=setCharsMsgSegment(cn);
    	
	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokCreateInstance);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_CreateInstance;
	
	hdr = &hdrq;
	

   	CMPIObjectPath *path;
   	CMPIInstance *inst;
   	CMPIValue val;
   	
   	int irc;
   	BinRequestContext binCtx;
   	BinResponseHdr *resp;
	CreateInstanceReq sreq = BINREQ(OPS_CreateInstance, 3);
	
	
	memset(&binCtx,0,sizeof(BinRequestContext));
	XtokCreateInstance *req = (XtokCreateInstance *) hdr->cimRequest;
	
	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
	inst = NewCMPIInstance(path, NULL);
	//printf("%s %s <<\n",req->op.nameSpace.data,req->op.className.data); 
	ExpressionLight * el;
	
	for(el = (ExpressionLight*)ins->ft->getFirst(ins);el;el = (ExpressionLight*)ins->ft->getNext(ins)){	
		//printf(" %s %s \n", el->value,el->name);
		val = str2CMPIValue(el->sqltype==CMPI_string?CMPI_chars:el->sqltype, el->value,NULL);//refernzen bis auf weiteres nicht unterstuetzt
		CMSetProperty(inst, el->name, &val, el->sqltype==CMPI_string?CMPI_chars:el->sqltype);
	}
	
	sreq.instance = setInstanceMsgSegment(inst);
	sreq.principal = setCharsMsgSegment(NULL);
	
	path = inst->ft->getObjectPath(inst,NULL);
	sreq.path = setObjectPathMsgSegment(path);
	
	binCtx.oHdr = (OperationHdr *) req;
	binCtx.bHdr = &sreq.hdr;
	binCtx.rHdr = hdr;
	binCtx.bHdrSize = sizeof(sreq);
	binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
	binCtx.pAs=NULL;
	//printf("3\n");
	irc = getProviderContext(&binCtx, (OperationHdr *) req);
	//printf("4 irc %d\n",irc);
	if (irc == MSG_X_PROVIDER) {
		resp = invokeProvider(&binCtx);
		closeProviderContext(&binCtx);
		resp->rc--;
		//printf("5 rc %d\n",resp->rc);
		if (resp->rc == CMPI_RC_OK) {
			return 0;
		}
		return resp->rc;
	}
	closeProviderContext(&binCtx);
	return 1;
}


int createClass(char * ns, char * cn, char * scn, UtilList * colDef) {
   	//printf("%s %s %s\n",ns,cn,scn);
	RequestHdr * hdr = NULL;
   	
    XtokCreateClass reqq;

	reqq.op.count = 3;
	reqq.op.type = OPS_CreateClass;
	reqq.op.nameSpace=setCharsMsgSegment(ns);
	reqq.op.className=setCharsMsgSegment(cn);
	reqq.superClass=scn;
	reqq.cls.className = cn;
	
	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokCreateClass);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_CreateClass;
	
	hdr = &hdrq;
	
	CMPIObjectPath *path;
   	CMPIConstClass cls;
   	ClClass *cl;
   	int irc;
   	BinRequestContext binCtx;
   	BinResponseHdr *resp;
   	CreateClassReq sreq = BINREQ(OPS_CreateClass, 3);
   
   	CMPIData d;
   	
   
   	memset(&binCtx,0,sizeof(BinRequestContext));
   	XtokCreateClass *req = (XtokCreateClass *) hdr->cimRequest;

   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
   
   	cl = ClClassNew(req->op.className.data, req->superClass ? req->superClass : NULL);

//hier koennte man eine Klassenbeschreibung mitgeben. Dazu den SQL-Standart erweitern   
//   	qs=&c->qualifiers;
//   	for (q=qs->first; q; q=q->next) {
//      	d.state=CMPI_goodValue;
//      	d.value=str2CMPIValue(q->type,q->value,NULL);
//      	d.type=q->type;
//      	ClClassAddQualifier(&cl->hdr, &cl->qualifiers, q->name, d);
//   	}
//                  
//   	ps=&c->properties;
	Column * col;

   	if(colDef!=NULL&&colDef->ft->size(colDef)>0){
	    for (col = (Column*)colDef->ft->getFirst(colDef); col; col = (Column*)colDef->ft->getNext(colDef)) {
	      	ClProperty *prop;
	      	int propId;
	      	d.state=CMPI_goodValue;
	      	//d.value=//str2CMPIValue(col->colSQLType,NULL,NULL);       
	      	d.type=col->colSQLType;
	      	propId=ClClassAddProperty(cl,col->colName,d);//(cl, p->name, d);
	//      	qs=&p->val.qualifiers;
	      	prop=((ClProperty*)ClObjectGetClSection(&cl->hdr,&cl->properties))+propId-1;
	//      	for (q=qs->first; q; q=q->next) {
	//Key oder Nicht-Key Qualifier einbauen
	//hier koennte man Min, Max und Columnbeschreibung einfuegen.
	         	if(col->isKey==KEY){//bug im sfcb: d.value wird ignoriert und immer true reingeschrieben
	         		d.state=CMPI_goodValue;
	         		//printf("::: %d %s\n",col->isKey,col->isKey==KEY?"true":"false");
	         		d.value=str2CMPIValue(CMPI_boolean,col->isKey==KEY?"true":"false",NULL);
	         		d.type=CMPI_boolean;
	         		ClClassAddPropertyQualifier(&cl->hdr, prop, "Key", d);
	         	}
	//      	}
	   	}
   	}   
    cl = ClClassRebuildClass(cl,NULL); 
   	cls=initConstClass(cl);

   	sreq.principal = setCharsMsgSegment(NULL);
   	sreq.path = setObjectPathMsgSegment(path);
   	sreq.cls = setConstClassMsgSegment(&cls);
	//printf("1\n");
   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq.hdr;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sizeof(sreq);
   	binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   	binCtx.pAs=NULL;

   	irc = getProviderContext(&binCtx, (OperationHdr *) req);
	//printf("2 %d %s %s\n",irc,ns,cn);

   	if (irc == MSG_X_PROVIDER) {
      	resp = invokeProvider(&binCtx);
      	closeProviderContext(&binCtx);
      	//printf("3\n");
      	
      	resp->rc--;
      	if (resp->rc == CMPI_RC_OK) {
         	//printf("4\n");
         	return 0;
      	}
      	//printf("5 %d  %d\n",resp->rc,CMPI_RC_OK);
      	return 1;
   	}
   	closeProviderContext(&binCtx);
   	//printf("6\n");
   	return 1;
}

int deleteClass(char * ns, char * cn) {
	
	RequestHdr * hdr = NULL;
   	
    XtokDeleteClass reqq;
	reqq.op.count = 2;
	reqq.op.type = OPS_DeleteClass;
	reqq.op.nameSpace=setCharsMsgSegment(ns);
	reqq.op.className=setCharsMsgSegment(cn);
	reqq.className = cn;
	
	
	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokDeleteClass);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_DeleteClass;
	
	hdr = &hdrq;
	
	
   	CMPIObjectPath *path;
   	int irc;
   	BinRequestContext binCtx;
   	BinResponseHdr *resp;
   	DeleteClassReq sreq;

   	memset(&binCtx,0,sizeof(BinRequestContext));
   	XtokDeleteClass *req = (XtokDeleteClass *) hdr->cimRequest;

   	memset(&sreq,0,sizeof(sreq));
   	sreq.hdr.operation=OPS_DeleteClass;
   	sreq.hdr.count=2;

   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
   	sreq.objectPath = setObjectPathMsgSegment(path);
   	sreq.principal = setCharsMsgSegment(NULL);

   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq.hdr;
   	binCtx.bHdr->flags = 0;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sizeof(sreq);
   	binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   	binCtx.pAs=NULL;

   	irc = getProviderContext(&binCtx, (OperationHdr *) req);

   	if (irc == MSG_X_PROVIDER) {
      	resp = invokeProvider(&binCtx);
      	closeProviderContext(&binCtx);
      	resp->rc--;
      	if (resp->rc == CMPI_RC_OK) {
  			return 0;
      	}
      	//printf(">>> %s %d\n",(char*)resp->object[0].data,resp->rc);
      	return 1;
   	}
   	closeProviderContext(&binCtx);
   	//printf(">>> Fehler\n");
	return 1;
}


/*Hilfssfunktion für die Erzeugung einer Instanz, da während der SQL-Verarbeitung alle Daten als Strings behandelt werden*/
CMPIValue * string2cmpival(char * value, int type){
	CMPIValue * val = (CMPIValue*) calloc(1, sizeof(CMPIValue));
	switch (type & ~CMPI_ARRAY) {
   		case CMPI_chars:
   			val->chars = value;
   			break; 
   		case CMPI_string:
      		val->string = sfcb_native_new_CMPIString(value, NULL);
   			//val->chars = value;
   			break; 
   		case CMPI_sint64:
    val->sint64 = strtol(value,(char**)NULL,10);
   			break;
   		case CMPI_uint64:
      		val->uint64 = strtoul(value,(char**)NULL,10);
   			break;
   		case CMPI_sint32:
      		val->sint32 = (int)strtol(value,(char**)NULL,10);
   			break;
   		case CMPI_uint32:
      		val->uint32 = (int)strtoul(value,(char**)NULL,10);
   			break;
   		case CMPI_sint16:
      		val->sint16 = (short)strtol(value,(char**)NULL,10);
   			break;
   		case CMPI_uint16:
      		val->uint16 = (short)strtoul(value,(char**)NULL,10);
   			break;
   		case CMPI_uint8:
      		val->uint8 = (char)strtol(value,(char**)NULL,10);
   			break;
   		case CMPI_sint8:
      		val->sint8 = (char)strtoul(value,(char**)NULL,10);
   			break;
   		case CMPI_boolean:
    val->boolean = strcasecmp(value, "true") == 0 ? 1 : 0;
   			break;
   		case CMPI_char16:
      		val->char16 = value[0];
   			break;
   		case CMPI_real32:
      		val->real32 = strtod(value,(char**)NULL);
   			break;
   		case CMPI_real64:
      		val->real64 = strtold(value,(char**)NULL);
   			break;
   		//case CMPI_dateTime:
      	//	return "datetime"; 
   		//case CMPI_ref:
      	//	return "*";
   		//case CMPI_instance:
      	//	return "%";
      	default: 
      		return NULL;
   	}
   	return val;	
}


int deleteInstance(char * ns, char * cn, UtilList *al) {
   	RequestHdr * hdr = NULL;
   	
    XtokDeleteInstance reqq;
	reqq.op.count = 2;
	reqq.op.type = OPS_DeleteInstance;
	reqq.op.nameSpace=setCharsMsgSegment(ns);
	reqq.op.className=setCharsMsgSegment(cn);
	//reqq.instanceName = $2.instanceName;
	
	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokDeleteInstance);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_DeleteInstance;
	
	hdr = &hdrq;
	
   	
   	CMPIObjectPath *path;
       
   	int irc;
   	BinRequestContext binCtx;
   	BinResponseHdr *resp;
   	DeleteInstanceReq sreq = BINREQ(OPS_DeleteInstance, 2);

   	memset(&binCtx,0,sizeof(BinRequestContext));
   	XtokDeleteInstance *req = (XtokDeleteInstance *) hdr->cimRequest;

	ExpressionLight * el;
   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
   	for (el = (ExpressionLight*)al->ft->getFirst(al);el;el=(ExpressionLight*)al->ft->getNext(al)) {
        //valp = getKeyValueTypePtr(type2cmpiString(el->sqltype),el->value,NULL,&val,&type); //type ist char* !!!!!
      	//hier muss ich den valp selber bauen. denn diese getKey... 
      	//arbeitet nur mit strings sauber siehe
      	 //val = str2CMPIValue(el->value,el->sqltype,NULL);
      	//printf("%s %s %d\n",el->name,el->value,el->sqltype);
      	CMAddKey(path, el->name, string2cmpival(el->value,el->sqltype),el->sqltype);
      	//printf("%s \n",el->value);
   	}
   	sreq.objectPath = setObjectPathMsgSegment(path);
   	sreq.principal = setCharsMsgSegment(NULL);

   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq.hdr;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sizeof(sreq);
   	binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   	binCtx.pAs=NULL;

   	irc = getProviderContext(&binCtx, (OperationHdr *) req);
	//printf("irc %d\n",irc);

   	if (irc == MSG_X_PROVIDER) {
      	resp = invokeProvider(&binCtx);
      	closeProviderContext(&binCtx);
      	resp->rc--;
      	//printf("rc %d\n",resp->rc);
      	if (resp->rc == CMPI_RC_OK) {
         	return 0;
      	}
      	return 1;
   	}
   	closeProviderContext(&binCtx);
   	return 1;
}

int updateInstance(char * ns, char * cn, UtilList *al,UtilList *kl) {
	RequestHdr * hdr = NULL;
   	
    XtokModifyInstance reqq;
	reqq.op.count = 2;
	reqq.op.type = OPS_ModifyInstance;
	reqq.op.nameSpace=setCharsMsgSegment(ns);
	reqq.op.className=setCharsMsgSegment(cn);
	reqq.propertyList = NULL;
	reqq.properties=0;
	//printf("1 %s %s\n",ns,cn);
	
	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokModifyInstance);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_ModifyInstance;
	
	hdr = &hdrq;
	
   	CMPIObjectPath *path;
   	CMPIInstance *inst;
   	
      
   	int irc, i, sreqSize=sizeof(ModifyInstanceReq)-sizeof(MsgSegment);
   	BinRequestContext binCtx;
   	BinResponseHdr *resp;
   	ModifyInstanceReq *sreq;
       
       
  

	//printf("2\n");

   	memset(&binCtx,0,sizeof(BinRequestContext));
 	XtokModifyInstance *req = (XtokModifyInstance *) hdr->cimRequest;

   	if (req->properties) sreqSize+=req->properties*sizeof(MsgSegment);
 	sreq=calloc(1,sreqSize);
 	sreq->hdr.operation=OPS_ModifyInstance;
   	sreq->hdr.count=req->properties+3;


   	ExpressionLight * el;
   	//printf("%s %s\n",req->op.nameSpace.data, req->op.className.data);
   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
   	for (el = (ExpressionLight*)kl->ft->getFirst(kl);el;el=(ExpressionLight*)kl->ft->getNext(kl)) {
      	//printf("a\n");
      	//printf("key: %s %s %d\n",el->name,el->value,el->sqltype);
      	//val = str2CMPIValue(el->value,el->sqltype,NULL);
      	//printf("b\n");
      	CMAddKey(path, el->name, string2cmpival(el->value,el->sqltype),el->sqltype);
   	}
   
 	//printf("4\n");  
 
   	inst = NewCMPIInstance(path, NULL);
   	//printf("5\n");
   	for (i=0,el = (ExpressionLight*)al->ft->getFirst(al);el;el=(ExpressionLight*)al->ft->getNext(al),i++) {
      	//printf("val: %s %s\n",el->name,el->value);
      	CMSetProperty(inst, el->name, string2cmpival(el->value,el->sqltype), el->sqltype);
   	}
   	
   	//printf("6\n");
   	sreq->instance = setInstanceMsgSegment(inst);
   	sreq->path = setObjectPathMsgSegment(path);
   	sreq->principal = setCharsMsgSegment(NULL);
   	
   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq->hdr;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sreqSize;
   	binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   	binCtx.pAs=NULL;
	printf("7\n");
   	irc = getProviderContext(&binCtx, (OperationHdr *) req);
	//printf("irc %d should be: %d\n",irc,MSG_X_PROVIDER);
   	if (irc == MSG_X_PROVIDER) {
      	resp = invokeProvider(&binCtx);
      	closeProviderContext(&binCtx);
      	free(sreq);
      	resp->rc--;
		//printf("irc %d\n",resp->rc); 
      	if (resp->rc == CMPI_RC_OK) {	
         	return 0;
      	}
    	return 1;
   	}
   	closeProviderContext(&binCtx);
   	free(sreq);
	return 1;
}

//filter: *<pattern>* mit strstr!=NULL
UtilList * getClassNames(char * ns, char *filter) {

 	UtilList *cn;
 	RequestHdr * hdr = NULL;
   	
 	XtokEnumClassNames reqq;
	reqq.op.count = 2;
	reqq.op.type = OPS_EnumerateClassNames;
	reqq.op.nameSpace=setCharsMsgSegment(ns);
	reqq.op.className=setCharsMsgSegment(NULL);
	reqq.flags = CMPI_FLAG_DeepInheritance;
	
	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokEnumClassNames);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_EnumerateClassNames;
	
	hdr = &hdrq;
 	
	
	CMPIObjectPath *path;
   	EnumClassNamesReq sreq = BINREQ(OPS_EnumerateClassNames, 2);
   	int irc, l = 0, err = 0;
   	BinResponseHdr **resp;
   	BinRequestContext binCtx;
   
  
  	memset(&binCtx,0,sizeof(BinRequestContext));
   	XtokEnumClassNames *req = (XtokEnumClassNames *) hdr->cimRequest;
   
   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
   	sreq.objectPath = setObjectPathMsgSegment(path);
   	sreq.principal = setCharsMsgSegment(NULL);
   	sreq.hdr.flags = req->flags;
 	
   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq.hdr;
   	binCtx.bHdr->flags = req->flags;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sizeof(sreq);
   	binCtx.type=CMPI_ref;
   	binCtx.xmlAs=binCtx.noResp=0;
   	binCtx.chunkedMode=0;
   	binCtx.pAs=NULL;
 	
   	irc = getProviderContext(&binCtx, (OperationHdr *) req);
	//printf("irc %d\n",irc);
   	if (irc == MSG_X_PROVIDER) {
      	resp = invokeProviders(&binCtx, &err, &l);
      
      	closeProviderContext(&binCtx);
		//printf("err %d\n",err);
      	if (err == 0) {
      		int c,i,j;
      		cn = newList();
      		CMPIArray *ar = NewCMPIArray(l, binCtx.type, NULL);
   			//printf("l: %d\n",l);
      		for (c = 0, i = 0; i < binCtx.rCount; i++) {
      			void *object;
      			for (j = 0; j < resp[i]->count; c++, j++) {
         			if (binCtx.type == CMPI_ref)
            			object = relocateSerializedObjectPath(resp[i]->object[j].data);
         			else if (binCtx.type == CMPI_instance)
            			object = relocateSerializedInstance(resp[i]->object[j].data);
         			else if (binCtx.type == CMPI_class) {
            			object=relocateSerializedConstClass(resp[i]->object[j].data);
         			}   
         			arraySetElementNotTrackedAt(ar,c, (CMPIValue*)&object, binCtx.type);
      			}
   			}
      		CMPIEnumeration *enm = sfcb_native_new_CMPIEnumeration(ar, NULL);
   			while (CMHasNext(enm, NULL)) {
   				CMPIObjectPath *cop = CMGetNext(enm, NULL).value.ref;
         		char *kk = opGetClassNameChars(cop);
         		if(filter==NULL||strstr(kk,filter)!=NULL){
         			cn->ft->append(cn,kk);
         			//printf("%s\n",kk);
         		}	
   			}     
			return cn;
      	}
		return NULL;      
    }
   	closeProviderContext(&binCtx);
   	return NULL;
}


UtilList * invokeMethod(char * ns, char * cn, char * mn, UtilList *pl,UtilList *kl) {
   	RequestHdr * hdr = NULL;
  //printf("%s %s %s\n",ns,cn,mn);
    XtokMethodCall reqq;
	reqq.op.count = 2;
	reqq.op.type = OPS_InvokeMethod;
	reqq.op.nameSpace=setCharsMsgSegment(ns);
	reqq.op.className=setCharsMsgSegment(cn);
    
   	RequestHdr  hdrq;	
	unsigned long size = sizeof(XtokMethodCall);
	hdrq.cimRequestLength = size;
	hdrq.cimRequest = malloc(size);
	memcpy(hdrq.cimRequest,&reqq,size);
	hdrq.opType = OPS_ModifyInstance;
	
	hdr = &hdrq;
   
   	CMPIObjectPath *path;
   	CMPIArgs *out;
  

  
  int irc, i;
   	BinRequestContext binCtx;
   	BinResponseHdr *resp;
  
   	InvokeMethodReq sreq = BINREQ(OPS_InvokeMethod, 5);
   	CMPIArgs *in=TrackedCMPIArgs(NULL);
  

   	memset(&binCtx,0,sizeof(BinRequestContext));
   	XtokMethodCall *req = (XtokMethodCall *) hdr->cimRequest;

   	path = NewCMPIObjectPath(req->op.nameSpace.data, req->op.className.data, NULL);
	ExpressionLight * el;
   	for (el = (ExpressionLight*)kl->ft->getFirst(kl);el;el=(ExpressionLight*)kl->ft->getNext(kl)) {
    //printf("key: %s %s %d\n",el->name,el->value,el->sqltype);
      	CMAddKey(path, el->name, string2cmpival(el->value,el->sqltype),el->sqltype);      	
   	}
   	sreq.objectPath = setObjectPathMsgSegment(path);
   	sreq.principal = setCharsMsgSegment(NULL);

   	for (i=0,el = (ExpressionLight*)pl->ft->getFirst(pl);el;el=(ExpressionLight*)pl->ft->getNext(pl),i++) {
    //printf("param: %s %s %d\n",el->name,el->value,el->sqltype);
      	CMAddArg(in, el->name, string2cmpival(el->value,el->sqltype), el->sqltype);
   	}
   	sreq.in = setArgsMsgSegment(in);
   	sreq.out = setArgsMsgSegment(NULL);
   	sreq.method = setCharsMsgSegment(mn);
   
   	binCtx.oHdr = (OperationHdr *) req;
   	binCtx.bHdr = &sreq.hdr;
   	binCtx.bHdr->flags = 0;
   	binCtx.rHdr = hdr;
   	binCtx.bHdrSize = sizeof(InvokeMethodReq);
   	binCtx.chunkedMode=binCtx.xmlAs=binCtx.noResp=0;
   	binCtx.pAs=NULL;

   	
   	irc = getProviderContext(&binCtx, (OperationHdr *) req);
  //printf("irc %d\n",irc);
   	if (irc == MSG_X_PROVIDER) {
     	resp = invokeProvider(&binCtx);
      	closeProviderContext(&binCtx);
      	resp->rc--;
    //printf("rc %d %d\n",resp->rc,CMPI_RC_OK);
      	if (resp->rc == CMPI_RC_OK) {
      out = relocateSerializedArgs(resp->object[0].data);
      UtilList * rl = newList();
      ExpressionLight * el;

      int i, m;
    
      
      el = newExpressionLight("RETURNVALUE",UNDEF,strdup(value2string(resp->rv)));
      el->sqltype = resp->rv.type;
      rl->ft->append(rl,el);
   
      if (out!=NULL){
   
	m = CMGetArgCount(out,NULL);
	for (i = 0; i < m; i++) {
	  CMPIString *name;
	  CMPIData data;
	  data = CMGetArgAt(out, i, &name, NULL);
	  
	  if (data.type & CMPI_ARRAY) {
	   
      	}
	  else {
      	      el = newExpressionLight(name->hdl,UNDEF,value2string(data));
	      el->sqltype = data.type;
	      rl->ft->append(rl,el);
	  }  
	  CMRelease(name);
	}
      }
      
     
      return rl;
    }
    return NULL;
   	}
   	closeProviderContext(&binCtx);

  return NULL;
}
 
ClassDef * getClassDef(const char *ns, const char *cn){
	
    CMPIConstClass * ccl = getConstClass(ns,cn);
        
    CMPIStatus rc;
  	CMPIString **name = NULL;
  	unsigned long quals = 0;
  	int count = 0;
  	if(ccl==NULL)
  		return NULL; 
    count = ccl->ft->getPropertyCount(ccl,&rc);
 
	int i;

	ClClass * cls = (ClClass *) ccl->hdl;
    
   	ClProperty * p = (ClProperty *) ClObjectGetClSection(&cls->hdr, &cls->properties);
   	int l;
   	
   	
   	
	char *dscr;
	UtilList *ul = newList();
	ClassDef *cld = newClassDef(count,ccl->ft->getCharClassName(ccl),ul,0,ccl->ft->getCharSuperClassName(ccl));
	
	int m,j;
	for (i = 0, m = ClClassGetQualifierCount(cls); i < m; i++) {
        CMPIString *name;
        CMPIData data = ccl->ft->getQualifierAt(ccl, i, &name, NULL);
        if(strcmp("Description",name->hdl)==0){
        	cld->description = (char*)data.value.string->hdl;
        }
    }
	for (i = 0, l = cls->properties.used; i < l; i++) {
  		CMPIData cd = ccl->ft->getPropertyAt(ccl, i, name, &rc);
   		for (j = 0, m = ClClassGetPropQualifierCount(cls,i); j < m; j++) {
	        CMPIString *name;
	        CMPIData data = ccl->ft->getPropQualifierAt(ccl, i,j, &name, NULL);
	        if(strcmp("Description",name->hdl)==0){
	        	dscr = (char*)data.value.string->hdl;
	        }
    	}		

   		int key = CMPI_keyValue;
   		if (quals & ClProperty_Q_Key){
  			key = KEY;
   		}
  		
		Column* col = newColumn(NULL, NULL, ClObjectGetClString(&cls->hdr, &(p+i)->id),NULL,(int)cd.type,key);
		col->description = dscr;
		ul->ft->append(ul,col);
	}
	return cld;
}



/*es folgen die weiteren Prozessor-Funktionen
*/
//leifert EMPTYSET zurueck, falls keine Tabelle zum Muster passt!
char * processMetaTables(char * filter, char* db){
	ResultSet * rs = newResultSet(NULL);
	UtilList *ul = getClassNames(db,filter);
	char *cn;
	ClassDef * cd;
	Row *r;
	AvlTree *t = newAvlTree(item_cmp);
	for(cn=(char*)ul->ft->getFirst(ul);cn;cn=(char*)ul->ft->getNext(ul)){
		cd = getClassDef(db,cn);
		r = newRow(10);
		r->row[0] = "NULL";
		r->row[1] = "NULL";
		r->row[2] = cd->className;
		r->row[3] = "TABLE";
		r->row[4] = cd->description;
		r->row[5] = "NULL";
		r->row[6] = "NULL";
		r->row[7] = "NULL";
		r->row[8] = "NULL";
		r->row[9] = "NULL";
		
		t->insert(t,r);

	}
	UtilList * colList = newList();
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TABLE_CAT", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TABLE_SCHEM", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TABLE_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TABLE_TYPE", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "REMARKS", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TYPE_CAT", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TYPE_SCHEM", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "TYPE_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "SELF_REFERENCING_COL_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLTABLES",NULL, "REF_GENERATION", NULL,CMPI_string,NKEY));	
	
	Projection *pi = newProjection(ALL,colList);
	
	rs->addSet(rs,t);//mussvor addMeta ausgefuhert werden, da hier pi bei joins geandert wird
	
	rs->addMeta(rs,pi);
	rs->sw->reason = "Successful Completion";
	rs->sw->sqlstate = "00000";
	char * res = (char *) malloc(strlen(rs->meta)+strlen(rs->tupel)+strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+3+3+1);
	strcpy(res,"3 2 1\n");
	strcat(res,rs->sw->sqlstate);
	strcat(res,";");
	strcat(res,rs->sw->reason);
	strcat(res,"$$\n");
	strcat(res,rs->meta);
	strcat(res,"$$\n");
	strcat(res,rs->tupel);
	strcat(res,"$$\n");
	//Free rs, sbs,...
	return res;			 
}


char * processMetaColumns(char * filter,char * filtercol, char* db){
	ResultSet * rs = newResultSet(NULL);
	UtilList *ul = getClassNames(db,filter);
	char *cn;
	ClassDef * cd;
	Row *r;
	Column * col;
	int i;
	AvlTree *t = newAvlTree(item_cmp);
	for(cn=(char*)ul->ft->getFirst(ul);cn;cn=(char*)ul->ft->getNext(ul)){
		cd = getClassDef(db,cn);
		for(i=0,col=(Column*)cd->fieldNameList->ft->getFirst(cd->fieldNameList);col;col=(Column*)cd->fieldNameList->ft->getNext(cd->fieldNameList),i++){
			if(filtercol==NULL||strstr(col->colName,filtercol)==NULL)
				continue;
			
			r = newRow(22);
			r->row[0] = "NULL";
			r->row[1] = "NULL";
			r->row[2] = cd->className;
			r->row[3] = col->colName;
			r->row[4] = int2String(col->colSQLType);
			r->row[5] = type2sqlString(col->colSQLType);
			r->row[6] = "0";//Column-size. siehe java
			r->row[7] = "0";//not used
			r->row[8] = "0";//??
			r->row[9] = "0";//??
			r->row[10] = "0";//unknown -> in java.api nachschlagen
			r->row[11] = col->description;
			r->row[12] = "NULL";
			r->row[13] = "0";//not used
			r->row[14] = "0";//not used
			r->row[15] = "0";//das hier zu berechnen ist zu aufwendig. im jdbc gibts glaub ich solch eine Methode	
			r->row[16] = int2String(i+1); 
			r->row[17] = "";
			r->row[18] = "NULL";
			r->row[19] = "NULL";
			r->row[20] = "NULL";
			r->row[21] = "0";
			t->insert(t,r);
			
		}			
	}
	UtilList * colList = newList();
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "TABLE_CAT", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "TABLE_SCHEM", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "TABLE_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "COLUMN_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "DATA_TYPE", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "TYPE_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "COLUMN_SIZE", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "BUFFER_LENGTH", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "DECIMAL_DIGITS", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "NUM_PREC_RADIX", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "NULLABLE", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "REMARKS", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "COLUMN_DEF", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "SQL_DATA_TYPE", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "SQL_DATETIME_SUB", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "CHAR_OCTET_LENGTH", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "ORDINAL_POSITION", NULL,CMPI_sint32,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "IS_NULLABLE", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "SCOPE_CATLOG", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "SCOPE_SCHEMA", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "SCOPE_TABLE", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("ALLCOLUMNS",NULL, "SOURCE_DATA_TYPE", NULL,CMPI_string,NKEY));	
	
	Projection *pi = newProjection(ALL,colList);
	rs->addSet(rs,t);
	
	rs->addMeta(rs,pi);
	
	rs->sw->reason = "Successful Completion";
	rs->sw->sqlstate = "00000";
	char * res = (char *) malloc(strlen(rs->meta)+strlen(rs->tupel)+strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+3+3+1);
	strcpy(res,"3 5 1\n");
	strcat(res,rs->sw->sqlstate);
	strcat(res,";");
	strcat(res,rs->sw->reason);
	strcat(res,"$$\n");
	strcat(res,rs->meta);
	strcat(res,"$$\n");
	strcat(res,rs->tupel);
	strcat(res,"$$\n");
	return res;			 
}

	
char * processSuperTables(char * filter, char* db){
	ResultSet * rs = newResultSet(NULL);
	UtilList *ul = getClassNames(db,filter);
	char *cn;
	ClassDef * cd;
	Row *r;
	AvlTree *t = newAvlTree(item_cmp);
	for(cn=(char*)ul->ft->getFirst(ul);cn;cn=(char*)ul->ft->getNext(ul)){
		cd = getClassDef(db,cn);
		r = newRow(4);
		r->row[0] = "NULL";
		r->row[1] = "NULL";
		r->row[2] = cd->className;
		r->row[3] = cd->superclass;
		
		t->insert(t,r);

	}
	UtilList * colList = newList();
	colList->ft->append(colList,newColumn("SUPERTABLES",NULL, "TABLE_CAT", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("SUPERTABLES",NULL, "TABLE_SCHEM", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("SUPERTABLES",NULL, "TABLE_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("SUPERTABLES",NULL, "SUPERTABLE_NAME", NULL,CMPI_string,NKEY));	
	
	Projection *pi = newProjection(ALL,colList);
	
	rs->addSet(rs,t);//mussvor addMeta ausgefuhert werden, da hier pi bei joins geandert wird
	
	rs->addMeta(rs,pi);
	rs->sw->reason = "Successful Completion";
	rs->sw->sqlstate = "00000";
	char * res = (char *) malloc(strlen(rs->meta)+strlen(rs->tupel)+strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+3+3+1);
	strcpy(res,"3 3 1\n");
	strcat(res,rs->sw->sqlstate);
	strcat(res,";");
	strcat(res,rs->sw->reason);
	strcat(res,"$$\n");
	strcat(res,rs->meta);
	strcat(res,"$$\n");
	strcat(res,rs->tupel);
	strcat(res,"$$\n");
	return res;			 
}	

	
char * processKeyTable(char * filter,char * db){
	ResultSet * rs = newResultSet(NULL);
	ClassDef * cd = getClassDef(db,filter);
	Column * col;
	Row *r;
	AvlTree *t = newAvlTree(item_cmp);
	int i;
	
	if(cd==NULL){
		//printf("Parse Error !!\n");
		//printf("###%s \n###%s!!\n",rs->sw->reason,rs->sw->sqlstate);
		
		rs->sw->reason = "UNDIFINED TABLE:\nTable does not exist";
		rs->sw->sqlstate = "42P01";
	
		char * res = (char *) malloc(strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+1);
		strcpy(res,"3 4 0\n");
		strcat(res,rs->sw->sqlstate);
		strcat(res,";");
		strcat(res,rs->sw->reason);
		strcat(res,"$$\n");
		return res;
	}
	for(i=0,col=(Column*)cd->fieldNameList->ft->getFirst(cd->fieldNameList);col;col=(Column*)cd->fieldNameList->ft->getNext(cd->fieldNameList)){
		if(col->isKey!=KEY)
			continue;
		i++;
	
		r = newRow(6);
		r->row[0] = "NULL";
		r->row[1] = "NULL";
		r->row[2] = cd->className;
		r->row[3] = col->colName;
		r->row[4] = "???";//ist das i?
		r->row[5] = "NULL";
		
		t->insert(t,r);

	}
	
	UtilList * colList = newList();
	colList->ft->append(colList,newColumn("KEYTABLE",NULL, "TABLE_CAT", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("KEYTABLE",NULL, "TABLE_SCHEM", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("KEYTABLE",NULL, "TABLE_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("KEYTABLE",NULL, "COLUMN_NAME", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("KEYTABLE",NULL, "KEY_SEQ", NULL,CMPI_string,NKEY));	
	colList->ft->append(colList,newColumn("KEYTABLE",NULL, "PK_NAME", NULL,CMPI_string,NKEY));	
	
	Projection *pi = newProjection(ALL,colList);
	
	rs->addSet(rs,t);//mussvor addMeta ausgefuhert werden, da hier pi bei joins geandert wird
	
	rs->addMeta(rs,pi);
	rs->sw->reason = "Successful Completion";
	rs->sw->sqlstate = "00000";
	char * res = (char *) malloc(strlen(rs->meta)+strlen(rs->tupel)+strlen(rs->sw->reason)+strlen(rs->sw->sqlstate)+4+1+3+3+3+1);
	strcpy(res,"3 4 1\n");
	strcat(res,rs->sw->sqlstate);
	strcat(res,";");
	strcat(res,rs->sw->reason);
	strcat(res,"$$\n");
	strcat(res,rs->meta);
	strcat(res,"$$\n");
	strcat(res,rs->tupel);
	strcat(res,"$$\n");
	return res;			 
}	


