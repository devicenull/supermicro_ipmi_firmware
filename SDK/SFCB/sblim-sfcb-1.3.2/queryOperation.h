
/*
 * queryOperation.h
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
 *
 * Description:
 *
 * query executor.
 *
*/


#ifndef qlOperation_h
#define qlOperation_h

#include "cmpidt.h"
#include "cmpift.h"
#include "cmpimacs.h"
#include "support.h"

#define qsAllocNew(qs,strct) (strct*)qsAlloc(qs,sizeof(strct))

//#define QL_TRACE(X) X  
#define QL_TRACE(X)   

struct qlPropertySource;
typedef struct qlPropertySource QLPropertySource;
struct qlPropertyNameData; 
typedef struct qlPropertyNameData QLPropertyNameData; 
struct qlOperandFt;
typedef struct qlOperandFt QLOperandFt;
struct qLqueryOperand;
typedef struct qLqueryOperand QLOperand;

struct qlStatementFt;
typedef struct qlStatementFt QLStatementFt;
struct qlStatement;
typedef struct qlStatement QLStatement;
struct qlControl;
typedef struct qlControl QLControl;
struct qlCollector;
typedef struct qlCollector QLCollector;

extern void *qsAlloc(QLStatement *qs, unsigned int size);

typedef enum qlOpd {
    QL_Invalid,
    QL_NotFound,
    QL_Null,
    QL_Integer,
    QL_UInteger,
    QL_Double,
    QL_Boolean,
    QL_Chars,
    QL_Char,
    QL_PropertyName, 
    QL_Name, 
    QL_Inst, 
    QL_Ref, 
    QL_DateTime, 
} QLOpd;

typedef enum qlFnc {
   QL_FNC_NoFunction=0,
   QL_FNC_BadFunction=1,
   QL_FNC_Classname=2,
   QL_FNC_Namespacename=3,
   QL_FNC_Namespacetype=4,
   QL_FNC_Hostport=5,
   QL_FNC_Modelpath=6,
   QL_FNC_Classpath=7,
   QL_FNC_Objectpath=8,
   QL_FNC_InstanceToReference=9,
   QL_FNC_CurrentDateTime=10,
   QL_FNC_DateTime=11,
   QL_FNC_MicrosecondsToTimestamp=12,
   QL_FNC_MicrosecondsToInterval=13
} QLFnc;

struct qlPropertyNameData {
   QLPropertyNameData *nextPart;
   char* className;
   char* propName;
   int index;
};

struct qlOperandFt {
   char *(*toString)(QLOperand*);
   char *(*type)(QLOperand*);
   int (*compare)(QLOperand*,QLOperand*,QLPropertySource*);
};

struct qLqueryOperand {
   QLOperandFt *ft;
   QLOpd type;
   QLOpd fncArgType;
   QLFnc fnc;
   union {
     CMPIValue value;
     long long  integerVal;
     double  doubleVal;
     unsigned char booleanVal;
     char* charsVal;
     CMPIInstance *inst;
     CMPIObjectPath *ref;
     CMPIDateTime *dateTime;
     QLPropertyNameData *propertyName;
   };
};

QLOperand* newIntQueryOperand(QLStatement*, long long val);
QLOperand* newDoubleQueryOperand(QLStatement*, double val);
QLOperand* newBooleanQueryOperand(QLStatement*, unsigned char val);
QLOperand* newCharsQueryOperand(QLStatement*, char* val);
QLOperand* newInstQueryOperand(QLStatement*, CMPIInstance* val);
QLOperand* newPropQueryOperand(QLStatement*, QLPropertyNameData* val);
QLPropertyNameData* newPropertyNameData(QLStatement*);

typedef enum qlOp {
    QL_OR,
    QL_AND,
    QL_NOT,
    QL_EQ=CMPI_PredOp_Equals,
    QL_NE=CMPI_PredOp_NotEquals,
    QL_LT=CMPI_PredOp_LessThan,
    QL_LE=CMPI_PredOp_LessThanOrEquals,
    QL_GT=CMPI_PredOp_GreaterThan,
    QL_GE=CMPI_PredOp_GreaterThanOrEquals,
    QL_ISA=CMPI_PredOp_Isa,
    QL_NOT_ISA=CMPI_PredOp_NotIsa,
    QL_LIKE=CMPI_PredOp_Like,
    QL_NOT_LIKE=CMPI_PredOp_NotLike,
    QL_IS_NULL,
    QL_IS_NOT_NULL,
    QL_IS_TRUE,
    QL_IS_NOT_TRUE,
    QL_IS_FALSE,
    QL_IS_NOT_FALSE,
    QL_nop,
    QL_bin
} QLOp;


struct qlOperationFt;
typedef struct qlOperationFt QLOperationFt;
struct qlOperation;
typedef struct qlOperation QLOperation;
//struct predicateDisjunction; 
//typedef struct predicateDisjunction PredicateDisjunction; 
//struct predicateConjunction; 
//typedef struct predicateConjunction PredicateConjunction; 

#define Predicates CMPIArray
#define PredicateDisjunction CMPIArray
#define PredicateConjunction CMPIArray

struct qlOperationFt {
   int (*_evaluate)(QLOperation*,QLPropertySource* source);
   int (*evaluate)(QLOperation*,QLPropertySource* source);
   char* (*toString)(QLOperation*);
   QLOp (*operation)(QLOperation*);
   void (*traverse)(QLOperation*);
   void (*eliminateNots)(QLOperation*, int inv);
   PredicateDisjunction* (*getPredicateDisjunction)(QLOperation*); 
   PredicateConjunction* (*getPredicateConjunction)(QLOperation*); 
   int (*isBinaryOperation)(QLOperation*);
};

struct qlOperation {
   QLOperationFt *ft;
   QLOperation  *lhon,*rhon;
   QLOperand *lhod,*rhod;
   QLOp opr;
   union {
     unsigned int noOp : 1;
     unsigned int invert : 1;
   } flag;
};

QLOperation* newLtOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
QLOperation* newGtOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
QLOperation* newLeOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
QLOperation* newGeOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
QLOperation* newEqOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
QLOperation* newNeOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
QLOperation* newIsaOperation(QLStatement*,QLOperand* lo, QLOperand* ro);
 
QLOperation* newAndOperation(QLStatement*,QLOperation* lo, QLOperation* ro); 
QLOperation* newOrOperation(QLStatement*,QLOperation* lo, QLOperation* ro); 
QLOperation* newNotOperation(QLStatement*,QLOperation* lo);
QLOperation* newBinaryOperation(QLStatement*,QLOperation* lo);

QLOperation* newIsNullOperation(QLStatement*,QLOperand* lo);
QLOperation* newIsNotNullOperation(QLStatement*,QLOperand* lo);



struct qlPropertySource {
   void* data;
   char *sns;
   CMPIValue (*getValue)(QLPropertySource*, char* name, QLOpd *type);
};





struct qlStatementFt {
   void (*release)(QLStatement*);  
   CMPIInstance *(*cloneAndFilter)(QLStatement*,CMPIInstance*,CMPIObjectPath*,char**);
   void (*setAllProperties)(QLStatement*,int allProperties);  
   void (*appendSelectPropertyName)(QLStatement*,char *name);
   void (*addFromClass)(QLStatement*,char *cn, char *ca);
   void (*setWhereCondition)(QLStatement*,QLOperation *op);
   int  (*testPropertyClass)(QLStatement*,char* cl);
   char **(*getFromClassList)(QLStatement*);
};

#define QL_WQL 1
#define QL_CQL 2
#define QL_FILTER 1
#define QL_QUERY 2

struct qlStatement {
   QLStatementFt *ft;
//   QLStatement *next;
   void *filterId;
   unsigned int useCount;
   int lang,allProps,mode;
   int fcMax,fcNext;
   char **fClasses;
   int spMax,spNext;
   char **spNames;
   QLOperation *where;
   CMPIObjectPath *cop;
   char **keys;
   QLPropertySource propSrc;
   unsigned int allocMode,allocNext,allocMax;
   void **allocList;
   char *sns;
};

struct qlCollector {
   void (*clear)(QLCollector *qc);
   void (*addPropIdentifier)(QLCollector *qc, QLStatement *qs, char *cls, char *prop, int index);
   QLPropertyNameData *propName;
};   

struct qlControl {
   QLStatement *statement;
   QLCollector *collector;
};

extern QLStatement *parseQuery(int mode, const char *query, const char *lang, const char *sns, int *rc);

#endif
