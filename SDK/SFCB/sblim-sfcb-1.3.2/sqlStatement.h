/*
 * sqlStatement.h
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


#include "utilft.h"

#include "cmpimacs.h"
#include "avltree.h"

#define DEFAULTDB "root/cimv2"
/* SqlStatement.type: */
#define SELECT 0
#define INSERT 1
#define UPDATE 2
#define DELETE 3
#define CREATE 4
#define DROP 5
#define CALL 6

#define SUB 10
#define FUL 11
#define INITFUL 12
#define SIGMA 13
//#define FINISHFS 14
#define INSTUPEL 14

#define PRJ 20
#define ALL 21
#define DIST 22

#define ASC 30
#define DSC 31

#define UNION 50
#define UNIONALL 51
#define EXCEPT 52
#define EXCEPTALL 53
#define INTERSECT 54
#define INTERSECTALL 55

#define PLUS 60
//#define SUB 61
#define MUL 62
#define DIV 63
//assignment String
#define ASS 64 
//assignment Number
#define ASN 65 
#define CONCAT 66

#define EQ 71
#define NOT 70
#define NE 72
#define GT 73
#define LT 74
#define GE 75
#define LE 76

#define AND 80
#define OR 81
#define ISNULL 82
#define NOTNULL 83
#define BETWEEN 84
#define NOTBETWEEN 85

#define INNER 90
#define LEFT 91
#define RIGHT 92
#define FULL 93


#define EMPTY 100
#define KEY 101
#define NKEY 102 
#define UNDEF -1

#define OK 1
//falls die Anzahl der Spalten der verschiedenen Spalten ungleich ist
#define CCOUNTERR 2000 
//die erste Spalte, die nicht vereingbar ist. die mod 2000 999 Stellen codieren die Spaltennr 
#define CDEFERR 3000

struct sqlWarning;
typedef struct sqlWarning SqlWarning;

struct resultSet;
typedef struct resultSet ResultSet;

struct sqlStatement;
typedef struct sqlStatement SqlStatement;

struct fullSelect;
typedef struct fullSelect FullSelect;

struct subSelect;
typedef struct subSelect SubSelect;

struct projection;
typedef struct projection Projection;

struct column;
typedef struct column Column;

struct selection;
typedef struct selection Selection;

struct crossJoin;
typedef struct crossJoin CrossJoin;

struct row;
typedef struct row Row;

struct classDef;
typedef struct classDef ClassDef;

struct order;
typedef struct order Order;

struct sigma;
typedef struct sigma Sigma;

struct expressionLight;
typedef struct expressionLight ExpressionLight;

struct updIns;
typedef struct updIns UpdIns;

struct insert;
typedef struct insert Insert;

struct call;
typedef struct call Call;

struct call {
	char * tableName;
	char * procedureName; 
	UtilList * keyList;
	UtilList * parameterList;
	void (*free)();
	
};



struct updIns {
	char * tname;
	UtilList * colList; 
	UtilList * assignmentList;
	UtilList * where;
	void (*free)();
	
};


struct insert {
	char * tname;
	FullSelect* fs;
	UtilList * tupelList; 
	void (*free)();
	
};

struct expressionLight {
	int op;
	int sqltype;
	char * name;
	char * value; 
	void (*free)();
	
};

struct classDef {
	int fieldCount;
	char * className;
	UtilList * fieldNameList; //Columns stecken da drin
	int fNameLength;
	char * superclass;
	char * description;
};





struct sqlWarning {
	char* sqlstate;
	char* reason;
	void (*setWarning)();
	void (*free)();
};

struct resultSet {
	SqlWarning *sw;
	char* query;
	char* meta;
	char* tupel;
	void (*setWarning)(ResultSet *this, char* s, char *r);
	void (*addMeta)(ResultSet *this, Projection* prj);
	int (*addSet)(ResultSet *this, AvlTree* t);
	void (*free)();
};


struct sqlStatement {
	int type;//select, insert, update...
	ResultSet* rs;
	void* stmt;
	char* db;//namespace
	//int currentType;
	UtilList* cnode;
	FullSelect * lasttos, *lastFs;//um Sigma in fs einfügen zu können. wenn der parser das sigma erstellt hat ist aber setB schon gefüllt und das aktuelle fs schon vom stack genommen.
        //auch für Konstrukte der Form A u B u C
	void (*free)();
	int (*addNode)(SqlStatement* this, int type, void * value);
	int (*calculate)();
};

struct fullSelect {
	void* setA;
	int typeA;//fullselect oder subselect
	int type; //UNION, EXCEPT, ...
	Projection *pi;//hier wird eine Referenz auf eines der Subselects 
				   //gespeichert zum schnellen Vergleich, ob die weiteren
				   //Subselects konform dazu sind!
	void* setB;
	Selection *sigma;
	int typeB;
	void (*free)();
	AvlTree* (*calculate)();
};

struct subSelect {
	Projection* pi;
	Selection* sigma;
	CrossJoin* cross;
	int isSet;
	AvlTree* set;
	char* setName;
	char* aliasName;
	void (*free)();
	void (*addNode)(SubSelect* this,int type, void* value);
	void (*addWhereList)(SubSelect* this, UtilList *ul);	
	AvlTree* (*calculate)();
};

struct projection {
	int mode;//ALL, DISTINKT //Könnte auch in Selection passen??
	//int colCount;
	void (*free)();
	UtilList* col;
};

struct column {
	char* tableName;
	char* tableAlias;
	char* colName;
	char* colAlias;
	int colSQLType;
	int isKey;
	char *description;
	void (*free)();
};

struct selection {
	int fetchFirst; //oderBy und fetch first sind nicht wirklich eigenschaften 
					//dieses objekts, allerdings passen sie hier dazu.
	UtilList *orderBy;
	UtilList * where;
	void (*free)();
};

struct crossJoin {
	SubSelect* setA;
	SubSelect* setB;
	int type;
	void (*free)();
	AvlTree* (*calculate)();
	
};


struct order{
	int column;
	int order;
	int colSQLType;
	void (*free)();
		
};

struct row {
	char** row;
	int doublette;//hack. da avl-baum doubletten nicht unterstützt
	int size;
	Selection * sigma;
	void (*free)();
	
};

struct sigma{
	int link; //AND OR
	int op; //== != < ... 
	Column* colA;
	char* value; //mit was der wert verglichen werden soll 
	char* valueB;//falls joinbedingung, oder between (dann steht hier der zweite wert, daher auch double...)
	Column* colB;
	void (*negate)(Sigma* this);	
	void (*free)();
		
};


/*Konstruktoren, zu den oben definierten Datentypen*/
ResultSet* newResultSet(char* query);
//SqlStatement* newSqlStatement(int type, ResultSet* rs);
FullSelect* newFullSelect();
SubSelect* newSubSelect();
SubSelect* newSubSelectC(SubSelect* sbsA,SubSelect* sbsB, int type);
Projection* newProjection(int mode, UtilList* ul);
Column* newColumn(char* tableName,char* tableAlias, char* colName, char* colAlias, int colSQLType,int isKey);
Selection* newSelection(UtilList* orderBy, int fetchFirst);
CrossJoin* newCrossJoin(SubSelect* sbsA,SubSelect* sbsB, int type);
Row* newRow(int size);
Order *newOrder(int nr, int type, int order);
Sigma* newSigma(Column* colA, int op,char* value, Column* colB,char* valueB);
ExpressionLight* newExpressionLight(char* name, int op,char* value);
UpdIns* newUpdIns(char* tname, UtilList* colList,UtilList* assignmentList,UtilList* where);
Insert* newInsert(char* tname);
ClassDef* newClassDef(int fieldCount, char * className, UtilList * fieldNameList, int fNameLength, char * superclass);
Call* newCall(char * tname, char * pname, UtilList * klist, UtilList * pList);




/*Definition der Schnittstelle zum CIMOM, über die Zugfriff auf die Daten erlangt wird*/
CMPIEnumeration * enumInstances(char * ns, char * cn);
int createInstance(char * ns, char * cn, UtilList * ins);
int createClass(char * ns, char * cn, char * scn, UtilList * colDef);
int deleteClass(char * ns, char * cn);
CMPIValue * string2cmpival(char * value, int type);
int deleteInstance(char * ns, char * cn, UtilList *al);
int updateInstance(char * ns, char * cn, UtilList *al,UtilList *kl);
UtilList * getClassNames(char * ns, char *filter);
UtilList * invokeMethod(char * ns, char * cn, char * mn, UtilList *pl,UtilList *kl);
ClassDef * getClassDef(const char *ns, const char *cn);

/*Der Teil des SQL-Prozessors, der die Metaanfragen bearbeitet*/
/*geht über den Parser, das Ergebnis-Resultset muss noch als Antwort codiert werden*/
ResultSet *createRS(char *query, int *rc);
/*die Anfrage wird innerhalb der Funktion bearbeitet und gleich als Antworstring codiert*/
char * processMetaTables(char * filter, char* db);
char * processMetaColumns(char * filter,char * filtercol, char* db);
char * processSuperTables(char * filter, char* db);
char * processKeyTable(char * filter,char * db);
