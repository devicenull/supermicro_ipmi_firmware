
/*
 * $Id: queryOperation.c,v 1.14 2007/04/03 12:24:19 sschuetz Exp $
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


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "queryOperation.h"
#include "mlog.h"
#include "instance.h"

extern CMPIArray *TrackedCMPIArray(CMPICount size, CMPIType type, CMPIStatus * rc);
extern void sfcb_native_array_increase_size(CMPIArray * array, CMPICount increment);

static char *types[]={
  "-inv-",
  "integer",
  "double",
  "boolean",
  "chars",
  "name"
};  

char * qsStrDup(QLStatement *qs, char *str) {
   char *s=(char*)qsAlloc(qs,strlen(str)+1);
   strcpy(s,str);
   return s;
}   
   
static CMPIValue getPropValue(QLOperand* self, QLPropertySource* src, QLOpd *type)
{
   CMPIValue v;
   QLPropertyNameData *pd=self->propertyName;
   QLPropertySource nsrc=*src;

   /* resolves properties of the form "instance.propName" where
    * propName would be nextPart */
   while(pd->nextPart) {
      v=nsrc.getValue(&nsrc,pd->propName,type);
      if (*type!=QL_Inst) {
         *type=QL_Invalid;
        break;
      }
      nsrc.data=v.inst;
      pd = pd->nextPart;
   }
   
   v=nsrc.getValue(&nsrc,pd->propName,type);
 
   return v;
}

QLPropertyNameData* newPropertyNameData(QLStatement *qs) {
   QLPropertyNameData *pd=qsAllocNew(qs,QLPropertyNameData);
   pd->nextPart=NULL;
   return pd;
}

static char* intToString(QLOperand* op) 
{
   char* str=(char*)qsAlloc(NULL,64);
   sprintf(str,"%lld",op->integerVal);
   return str;
}

static char* doubleToString(QLOperand* op) 
{
   char* str=(char*)qsAlloc(NULL,64);
   sprintf(str,"%g",op->doubleVal);
   return str;
}

static char* booleanToString(QLOperand* op) 
{
   return qsStrDup(NULL,op->booleanVal ? "true" : "false");
}

static char* charsToString(QLOperand* op) 
{
   return qsStrDup(NULL,op->charsVal);
}
   
static char* propToString(QLOperand* op) 
{
   QLPropertyNameData *pd=op->propertyName;
   int s=0;
   char *str;
   while (pd) {
      if (pd->className) s+=2+strlen(pd->className);
      if (pd->propName) s+=strlen(pd->propName);
      if (pd->nextPart) s++;
      pd=pd->nextPart;
   }   
   str=(char*)malloc(s+8);
   str[0]=0;
   pd=op->propertyName;
   while (pd) {
      if (pd->className) {
         strcat(str,pd->className);
         strcat(str,"::");
      }   
      if (pd->propName) strcat(str,pd->propName);
      if (pd->nextPart) strcat(str,".");
      pd=pd->nextPart;
   }   
   return str;   //str must be freed by caller.
}
   
static char* typeToString(QLOperand* op) 
{
   return types[op->type];
}

static int intCompare(QLOperand* self, QLOperand* op, QLPropertySource* src)
{
   long long sov,ov;
   QLOpd type=op->type;
   
   sov=self->integerVal;
   if (type==QL_PropertyName) 
      ov=getPropValue(op, src, &type).sint64;
   else ov=op->integerVal;
   
   if (type==QL_Integer) 
      return sov-ov;
   if (type==QL_UInteger) 
      return (unsigned long long)sov-(unsigned long long)ov;
  
   return -(2+((int)type));
}

static int  doubleCompare(QLOperand* self, QLOperand* op, QLPropertySource* src)
{
   double sov,ov,d;
   QLOpd type=op->type;
   
   sov=self->doubleVal;
   if (type==QL_PropertyName) 
      ov=getPropValue(op, src, &type).real64;
   else ov=op->doubleVal;
   
   ov=op->doubleVal;
   if (type==QL_Double) {
      d=sov-ov;
      if (d<0.0) return -1;
      if (d>0.0) return 1;
      return 0;
   }   
   return -2;
}

static int  booleanCompare(QLOperand* self, QLOperand* op, QLPropertySource* src)
{
   unsigned char sov,ov;
   QLOpd type=op->type;
   
   sov=self->booleanVal;
   if (type==QL_PropertyName) 
      ov=getPropValue(op, src, &type).boolean;
   else ov=op->booleanVal;
   
   if (type==QL_Boolean) 
      return (sov!=0)-(ov!=0);
   
   return -(2+((int)type));
}

static int  charsCompare(QLOperand* self, QLOperand* op, QLPropertySource* src)
{
   char *sov,*ov;
   QLOpd type=op->type;
  
   sov=self->charsVal;
   if (type==QL_PropertyName) 
      ov=getPropValue(op, src, &type).chars;
   else ov=op->charsVal;
   
   if (type==QL_Chars) return strcmp(sov,ov);
   return -2;
}

static int propCompare(QLOperand* self, QLOperand* op, 
   QLPropertySource* src)
{
   QLOperand *nop=NULL;
   QLOpd type;
   int rc;
   CMPIValue v=getPropValue(self, src, &type);
   char *str;

   switch (type) {
   case QL_Integer: 
      nop=newIntQueryOperand(NULL,v.sint64);
      break;
   case QL_UInteger: 
      nop=newIntQueryOperand(NULL,v.sint64);
      nop->type=QL_UInteger;
      break;
   case QL_Double:
      nop=newDoubleQueryOperand(NULL,v.real64);
      break;
   case QL_Boolean:
      nop=newBooleanQueryOperand(NULL,v.boolean);
      break;
   case QL_Chars:
      nop=newCharsQueryOperand(NULL,v.chars);
      break;
   case QL_Char:
      nop=newCharsQueryOperand(NULL,v.chars);
      nop->type=QL_Char;
      nop->value.char16=v.char16;
      break;
    case QL_Inst:
      nop=newInstQueryOperand(NULL,v.inst);
      nop->type=QL_Char;
      nop->value.char16=v.char16;
      break;
   case QL_PropertyName:
   case QL_Name:
      mlogf(M_ERROR,M_SHOW,"### propCompare(): (QL_PropertyName QL_Name) got a problem\n");
      return -2;
      /*      abort();*/
      break;
   case QL_Invalid:
      mlogf(M_ERROR,M_SHOW,"### propCompare(): got a problem\n");
      return -2;
      /* abort();*/
      break;
   case QL_NotFound:
      str=propToString(self);
      mlogf(M_ERROR,M_SHOW,"### propCompare(): %s not found\n",str);
      free(str);
      return -2;
      /*abort();*/
   default:
      ; // what should we do here ?  
   }
   
   rc=nop->ft->compare(nop,op,src);
   QL_TRACE(fprintf(stderr,"propCompare(%s) %d\n",self->propertyName->propName,rc));
   return rc;
}

extern int isChild(const char *ns, const char *parent, const char* child);

static int  instCompare(QLOperand* self, QLOperand* op, QLPropertySource* src)
{
   CMPIInstance *ov=NULL;
   char *sov;
   QLOpd type=op->type;
   
   sov=(char*)instGetClassName(self->inst);
   if (type==QL_PropertyName) {
      ov=getPropValue(op, src, &type).inst;
   }
   
   if (type==QL_Name) {
      if (strcasecmp(sov,op->charsVal)==0) return 0;
      return isChild(src->sns,op->charsVal,sov)==0;
   }
   if (type==QL_Inst) {
      return instanceCompare(self->inst, ov);
   }
   return -2;
}

static char* instToString(QLOperand* op) 
{
   return qsStrDup(NULL,"*** instance ***");
}

static int  nameCompare(QLOperand* self, QLOperand* op, QLPropertySource* src)
{
   mlogf(M_ERROR,M_SHOW,"--- nameCompare\n");
   /*abort();*/
   return -2;
}

static char* nameToString(QLOperand* op) 
{
   return qsStrDup(NULL,op->charsVal);
}


static QLOperandFt qLintQueryOperandFt={
   intToString,
   typeToString,
   intCompare,
};
 
static QLOperandFt qLdoubleQueryOperandFt={
   doubleToString,
   typeToString,
   doubleCompare,
};

static QLOperandFt qLbooleanQueryOperandFt={
   booleanToString,
   typeToString,
   booleanCompare,
};

static QLOperandFt qLcharsQueryOperandFt={
   charsToString,
   typeToString,
   charsCompare,
};

static QLOperandFt qLpropQueryOperandFt={
   propToString,
   typeToString,
   propCompare,
};

static QLOperandFt qLinstQueryOperandFt={
   instToString,
   typeToString,
   instCompare,
};

static QLOperandFt qLnameQueryOperandFt={
   nameToString,
   typeToString,
   nameCompare,
};




QLOperand* newIntQueryOperand(QLStatement *qs, long long val) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newIntQueryOperand %lld\n",val));
   op->integerVal=val;
   op->type=QL_Integer;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLintQueryOperandFt;
   return op;
}

QLOperand* newDoubleQueryOperand(QLStatement *qs, double val) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newDoubleQueryOperand %g\n",val));
   op->doubleVal=val;
   op->type=QL_Double;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLdoubleQueryOperandFt;
   return op;
}

QLOperand* newBooleanQueryOperand(QLStatement *qs, unsigned char val) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   op->booleanVal=val;
   op->type=QL_Boolean;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLbooleanQueryOperandFt;
   QL_TRACE(fprintf(stderr,"--- newBooleanQueryOperand %d %p\n",val,op));
   return op;
}

QLOperand* newCharsQueryOperand(QLStatement *qs, char* val) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newCharsQueryOperand %s\n",val));
   op->charsVal=val;
   op->type=QL_Chars;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLcharsQueryOperandFt;
   return op;
}

QLOperand* newNameQueryOperand(QLStatement *qs, char* val) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newNameQueryOperand %s\n",val));
   op->charsVal=val;
   op->type=QL_Name;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLnameQueryOperandFt;
   return op;
}

QLOperand* newPropQueryOperand(QLStatement *qs, QLPropertyNameData* val) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newPropQueryOperand %p\n",val));
   op->propertyName=val;
   op->type=QL_PropertyName;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLpropQueryOperandFt;
   return op;
}

QLOperand* newInstQueryOperand(QLStatement *qs, CMPIInstance* ci) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newInstQueryOperand %p\n",ci));
   op->inst=ci;
   op->type=QL_Inst;
   op->fnc=QL_FNC_NoFunction;
   op->ft=&qLinstQueryOperandFt;
   return op;
}

QLOperand* newFncQueryOperand(QLStatement *qs, QLFnc fnc, QLOpd argType, void* argVal) 
{
   QLOperand *op=qsAllocNew(qs,QLOperand);
   QL_TRACE(fprintf(stderr,"--- newFncQueryOperand %p\n",argVal));
//   op->propertyName=val;
   op->type=QL_PropertyName;
   op->fnc=fnc;
   op->fncArgType=argType;
   switch (fnc) {
      case QL_FNC_NoFunction:
      case QL_FNC_BadFunction:
         break;
      case QL_FNC_Classname:
      case QL_FNC_Namespacename:
      case QL_FNC_Namespacetype:
      case QL_FNC_Hostport:
      case QL_FNC_Modelpath:
         op->type=QL_Chars;
         if (argType!=QL_Chars && argType!=QL_PropertyName) return NULL; 
         break;
      case QL_FNC_Classpath:
      case QL_FNC_Objectpath:
         op->type=QL_Ref;
         if (argType!=QL_Chars && argType!=QL_PropertyName) return NULL; 
         break;         
      case QL_FNC_InstanceToReference:
         op->type=QL_Ref;
         if (argType!=QL_Inst) return NULL; 
         break;
      case QL_FNC_CurrentDateTime:
      case QL_FNC_DateTime:
      case QL_FNC_MicrosecondsToTimestamp:
      case QL_FNC_MicrosecondsToInterval:
         break;
      default:
         ;   
   }
   op->ft=&qLpropQueryOperandFt;
   return op;
}




static char* opnToChars(QLOperation *op, char* opr, char *rh)
{
   char *ln,*rn,*str;
   
   if (op->lhon) ln=op->lhon->ft->toString(op->lhon);
   else ln="?l?";
   if (rh==NULL) {
      if (op->rhon) rn=op->rhon->ft->toString(op->rhon);
      else rn="?r?";
   }   
   else rn=rh;
   
   str=(char*)malloc(32+strlen(ln)+strlen(rn));
   
   strcpy(str,"[");
   strcat(str,ln);
   strcat(str,"] ");
   strcat(str,opr);
   strcat(str," [");
   strcat(str,rn);
   strcat(str,"]");

   return str;
}


static int evaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=op->ft->_evaluate(op,source); 
   QL_TRACE(fprintf(stderr,"evaluate(): %d\n",rc));
   return rc;
}

static void relTraverse(QLOperation *op) 
{
   char *str;
   
   QL_TRACE(fprintf(stderr,"- - relTraverse: %d\n",op->opr));
   if (op->lhon) op->lhon->ft->traverse(op->lhon);
   if (op->rhon) op->rhon->ft->traverse(op->rhon);
   str=op->ft->toString(op);
   fprintf(stderr,"%s\n",str);
   free(str);
}

static void relEliminateNots(QLOperation *op, int invert) 
{
   op->flag.invert=invert;
   if (op->lhon) op->lhon->ft->eliminateNots(op->lhon,invert);
   if (op->rhon) op->rhon->ft->eliminateNots(op->lhon,invert);
}

static int isBinaryOperation(QLOperation *op)
{
   return 0;
}





static int _andEvaluate(QLOperation *op, QLPropertySource* source) 
{
   return op->flag.invert ? 
      (op->lhon->ft->evaluate(op->lhon,source) || 
         (op->rhon ? op->rhon->ft->evaluate(op->rhon,source) : 1)) :
      (op->lhon->ft->evaluate(op->lhon,source) && 
         (op->rhon ? op->rhon->ft->evaluate(op->rhon,source) : 1));
}

static char *andToString(QLOperation *op) 
{
   return opnToChars(op,(op->flag.invert ? "QL_OR" : "QL_AND"),NULL);
}

static QLOp andOperation(QLOperation *op)
{
   return op->flag.invert ? QL_OR : QL_AND;
}

static CMPIArray *appendArray(CMPIArray *t, CMPIArray *f)
{
   int tn=CMGetArrayCount(t,NULL),fm=CMGetArrayCount(f,NULL),fn;
   
   for (fn=0; fn<fm; tn++,fn++) {
      CMPIData d=CMGetArrayElementAt(f,fn,NULL);
      sfcb_native_array_increase_size(t,1);
      CMSetArrayElementAt(t,tn,&d.value,d.type); 
   }   
   return t;   
}

static PredicateDisjunction* andGetPredicateDisjunction(QLOperation* op) 
{
   int rdn=0,i,m,ii,mi;
   PredicateDisjunction *lc=op->lhon->ft->getPredicateDisjunction(op->lhon);
   if (!op->rhon) return lc;
   
   PredicateDisjunction *rc=op->rhon->ft->getPredicateDisjunction(op->rhon);
   PredicateDisjunction *rd=TrackedCMPIArray(
      CMGetArrayCount(lc,NULL)*CMGetArrayCount(rc,NULL), CMPI_ptr, NULL);
      
   for (i=0, m=CMGetArrayCount(lc,NULL); i<m; i++) {
      PredicateDisjunction *lci=CMGetArrayElementAt(lc,i,NULL).value.dataPtr.ptr;
      for (ii=0, mi=CMGetArrayCount(rc,NULL); ii<mi; ii++) {
         PredicateDisjunction *rcii=CMGetArrayElementAt(rc,ii,NULL).value.dataPtr.ptr;
         Predicates *tc=TrackedCMPIArray(0, CMPI_ptr, NULL);
         CMPIValuePtr rdv={tc,1};               
         appendArray(tc,lci);
         appendArray(tc,rcii);
         CMSetArrayElementAt(rd,rdn,&rdv,CMPI_ptr); 
         rdn++;
      }
   }
   
   return rd;
} 

static PredicateConjunction* andGetPredicateConjunction(QLOperation* op) 
{
   PredicateConjunction *lc=op->lhon->ft->getPredicateConjunction(op->lhon);
   if (!op->rhon) return lc;
   PredicateConjunction *rc=op->rhon->ft->getPredicateConjunction(op->rhon);
   appendArray(lc,rc);
   return lc;
}

QLOperationFt qlAndOperationFt={
   _andEvaluate,
   evaluate,
   andToString,
   andOperation,
   relTraverse,
   relEliminateNots,
   andGetPredicateDisjunction,
   andGetPredicateConjunction,
   isBinaryOperation
}; 




static int _orEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) {
      rc=(op->lhon->ft->evaluate(op->lhon,source) && 
         (op->rhon ? op->rhon->ft->evaluate(op->rhon,source) : 1));
   }
   else {      
      rc=(op->lhon->ft->evaluate(op->lhon,source) || 
         (op->rhon ? op->rhon->ft->evaluate(op->rhon,source) : 1));
   }      
//   QL_TRACE(fprintf(stderr,"_orEvaluate: %d==%d: %d\n",rcl,rcr,rc));
   return rc;      
}

static char *orToString(QLOperation *op) 
{
   QL_TRACE(fprintf(stderr,"--- orToString %p\n",op));
   return opnToChars(op,(op->flag.invert ? "QL_AND" : "QL_OR"),NULL);
}

static QLOp orOperation(QLOperation *op)
{
   return op->flag.invert ? QL_AND : QL_OR;
}

PredicateDisjunction *orGetPredicatesDisjunction(QLOperation *op) 
{
   PredicateDisjunction *lc=op->lhon->ft->getPredicateDisjunction(op->lhon);
   if (!op->rhon) return lc;
   PredicateDisjunction *rc=op->rhon->ft->getPredicateDisjunction(op->rhon);
   appendArray(lc,rc);
   
   return lc;
}

PredicateConjunction *orGetPredicateConjunction(QLOperation *op) 
{
   int rdn=0,i,m,ii,mi;
   PredicateConjunction *lc=op->lhon->ft->getPredicateConjunction(op->lhon);
   if (!op->rhon) return lc;
   
   PredicateConjunction *rc=op->rhon->ft->getPredicateConjunction(op->rhon);
   PredicateConjunction *rd=TrackedCMPIArray(
      CMGetArrayCount(lc,NULL)*CMGetArrayCount(rc,NULL), CMPI_ptr, NULL);
      
   for (i=0, m=CMGetArrayCount(lc,NULL); i<m; i++) {
      PredicateConjunction *lci=CMGetArrayElementAt(lc,i,NULL).value.dataPtr.ptr;
      for (ii=0, mi=CMGetArrayCount(rc,NULL); ii<mi; ii++) {
         PredicateConjunction *rcii=CMGetArrayElementAt(rc,ii,NULL).value.dataPtr.ptr;
         Predicates *tc=TrackedCMPIArray(0, CMPI_ptr, NULL);
         CMPIValuePtr rdv={tc,1};               
         appendArray(tc,lci);
         appendArray(tc,rcii);
         CMSetArrayElementAt(rd,rdn,&rdv,CMPI_ptr); 
         rdn++;
      }
   }
   
   return rd;
}

QLOperationFt qlOrOperationFt={
   _orEvaluate,
   evaluate,
   orToString,
   orOperation,
   relTraverse,
   relEliminateNots,
   orGetPredicatesDisjunction,
   orGetPredicateConjunction,
   isBinaryOperation
}; 




static int _notEvaluate(QLOperation *op, QLPropertySource* source) 
{
   return op->lhon->ft->evaluate(op->lhon,source);
}

static char *notToString(QLOperation *op) 
{
   return opnToChars(op,(op->flag.invert ? "QL_nop" : "QL_NOT"),NULL);
}

static QLOp notOperation(QLOperation *op)
{
   return op->flag.invert ? QL_nop : QL_NOT;
}

PredicateDisjunction *notGetPredicateDisjunction(QLOperation *op) 
{
   return op->lhon->ft->getPredicateDisjunction(op->lhon);
}

PredicateConjunction *notGetPredicateConjunction(QLOperation *op) 
{
   return op->lhon->ft->getPredicateConjunction(op->lhon);
}

QLOperationFt qlNotOperationFt={
   _notEvaluate,
   evaluate,
   notToString,
   notOperation,
   relTraverse,
   relEliminateNots,
   notGetPredicateDisjunction,
   notGetPredicateConjunction,
   isBinaryOperation
}; 




int _isNullEvaluate(QLOperation *op, QLPropertySource* source) 
{
   QL_TRACE(fprintf(stderr,"--- unexpected invocation of _isNullEvaluate\n"));
   exit(16);
}

int isNullEvaluate(QLOperation *op, QLPropertySource* source) 
{
   QLOperand *opd=NULL;// op->lhod->ft->resolveProperty(op->lhod,source);
   if (opd==NULL) return 1;
   return 0;
}

char* isNullToString(QLOperation *op) 
{
   return opnToChars(op,(op->flag.invert ? "QL_IS_NOT_NULL" : "QL_IS_NULL"),"---");
}

QLOp isNullOperation(QLOperation *op) 
{ 
   return op->flag.invert ? QL_IS_NOT_NULL : QL_IS_NULL;
}

QLOperationFt qlIsNullOperationFt={
   _isNullEvaluate,
   isNullEvaluate,
   isNullToString,
   isNullOperation,
   relTraverse,
   relEliminateNots,
   NULL,
   NULL,
   isBinaryOperation 
}; 





int _isNotNullEvaluate(QLOperation *op, QLPropertySource* source) 
{
   QL_TRACE(fprintf(stderr,"--- unexpected invocation of _isNotNullEvaluate\n"));
   exit(16);
}

int isNotNullEvaluate(QLOperation *op, QLPropertySource* source) 
{
   QLOperand *opd=NULL;// op->lhod->ft->resolveProperty(op->lhod,source);
   if (opd==NULL) return 0;
   return 1;
}

char* isNotNullToString(QLOperation *op) 
{
   return opnToChars(op,(op->flag.invert ? "QL_IS_NULL" : "QL_IS_NOT_NULL"),"---");
}

QLOp isNotNullOperation(QLOperation *op) 
{ 
   return op->flag.invert ? QL_IS_NULL : QL_IS_NOT_NULL;
}

QLOperationFt qlIsNotNullOperationFt={
   _isNotNullEvaluate,
   isNotNullEvaluate,
   isNotNullToString,
   isNotNullOperation,
   relTraverse,
   relEliminateNots,
   NULL,
   NULL,
   isBinaryOperation
}; 








int _binEvaluate(QLOperation *op, QLPropertySource* source) 
{
   QL_TRACE(fprintf(stderr,"--- unexpected invocation of _isNotNullEvaluate\n"));
   exit(16);
}

int binEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=op->lhon->ft->evaluate(op->lhon,source);
   QL_TRACE(fprintf(stderr,"binEvaluate(): %d\n",rc));
   return rc;
}

char* binToString(QLOperation *op) 
{
   return opnToChars(op,(op->flag.invert ? "QL_IS_NOT_TRUE" : "QL_IS_TRUE"),"---");
}

QLOp binOperation(QLOperation *op) 
{ 
   return op->flag.invert ? QL_IS_NULL : QL_IS_NOT_NULL;
}

int binIsBinaryOperation(QLOperation *op){
   return 1;
} 
  
static void binTraverse(QLOperation *op) {
   char *str=NULL;
   QL_TRACE(fprintf(stderr,"--- binTraverse: %d\n",op->lhon->opr));
   str=op->lhon->ft->toString(op->lhon);
   fprintf(stderr,"%s\n",str);
   free(str);
}

static PredicateDisjunction* binGetPredicateDisjunction(QLOperation* op) 
{
   Predicates *c=TrackedCMPIArray(1, CMPI_ptr, NULL);
   PredicateDisjunction *dc=TrackedCMPIArray(1, CMPI_ptr, NULL);
   CMPIValuePtr cv={op,0};
   CMPIValuePtr dcv={c,1};
  
   CMSetArrayElementAt(c,0,&cv,CMPI_ptr);
   CMSetArrayElementAt(dc,0,&dcv,CMPI_ptr);

   return dc;
} 

static PredicateConjunction* binGetPredicateConjunction(QLOperation* op) 
{
   Predicates *d=TrackedCMPIArray(1, CMPI_ptr, NULL);
   PredicateConjunction *cd=TrackedCMPIArray(1, CMPI_ptr, NULL);
   CMPIValuePtr dv={op,0};
   CMPIValuePtr cdv={d,1};
  
   CMSetArrayElementAt(d,0,&dv,CMPI_ptr);
   CMSetArrayElementAt(cd,0,&cdv,CMPI_ptr);

   return cd;
} 

QLOperationFt qlBinOperationFt={
   _binEvaluate,
   binEvaluate,
   binToString,
   binOperation,
   binTraverse,
   relEliminateNots,
   binGetPredicateDisjunction,
   binGetPredicateConjunction,
   binIsBinaryOperation
}; 






QLOperation* newAndOperation(QLStatement *qs, QLOperation* lo, QLOperation* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhon=lo;
   op->rhon=ro;
   op->opr=QL_AND;
   op->ft=&qlAndOperationFt;
   QL_TRACE(fprintf(stderr,"--- newAndOperation %p\n",op));
   return op;
}

QLOperation* newOrOperation(QLStatement *qs, QLOperation* lo, QLOperation* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhon=lo;
   op->rhon=ro;
   op->opr=QL_OR;
   op->ft=&qlOrOperationFt;
   QL_TRACE(fprintf(stderr,"--- newOrOperation %p (%p-%p)\n",op,lo,ro));
   return op;
}

QLOperation* newNotOperation(QLStatement *qs, QLOperation* lo)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhon=lo;
   op->rhon=NULL;
   op->opr=QL_NOT;
   op->ft=&qlNotOperationFt;
   return op;
}

QLOperation* newBinaryOperation(QLStatement *qs, QLOperation* lo)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhon=lo;
   op->rhon=NULL;
   op->opr=QL_bin;
   op->ft=&qlBinOperationFt;
   QL_TRACE(fprintf(stderr,"--- newBinaryOperation %p (%p)\n",op,lo));
   return op;
}

QLOperation* newIsNullOperation(QLStatement *qs, QLOperand* lo)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=NULL;
   op->opr=QL_IS_NULL;
   op->ft=&qlIsNullOperationFt;
   return op;
}

QLOperation* newIsNotNullOperation(QLStatement *qs, QLOperand* lo)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=NULL;
   op->opr=QL_IS_NOT_NULL;
   op->ft=&qlIsNotNullOperationFt;
   return op;
}


 



static int _ltEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)>=0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)<0;
   QL_TRACE(fprintf(stderr,"_ltEvaluate() rc: %d\n",rc));
   return rc;
}

char * ltToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_GE " : " QL_LT "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp ltOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_GE : QL_LT;
}

QLOperationFt qlLtOperationFt={
   _ltEvaluate,
   evaluate,
   ltToString,
   ltOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 




static int _gtEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)<=0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)>0;
   QL_TRACE(fprintf(stderr,"_gtEvaluate() rc: %d\n",rc));
   return rc;
}

char * gtToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_LE " : " QL_GT "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp gtOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_LE : QL_GT;
}

QLOperationFt qlGtOperationFt={
   _gtEvaluate,
   evaluate,
   gtToString,
   gtOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 




static int _leEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)>0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)<=0;
   QL_TRACE(fprintf(stderr,"_leEvaluate() rc: %d\n",rc));
   return rc;
}

char * leToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_GT " : " QL_LE "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp leOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_GT : QL_LE;
}

QLOperationFt qlLeOperationFt={
   _leEvaluate,
   evaluate,
   leToString,
   leOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 




static int _geEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)<0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)>=0;
   QL_TRACE(fprintf(stderr,"_geEvaluate() rc: %d\n",rc));
   return rc;
}

char * geToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_LT " : " QL_GE "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp geOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_LT : QL_GE;
}

QLOperationFt qlGeOperationFt={
   _geEvaluate,
   evaluate,
   geToString,
   geOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 




static int _eqEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)!=0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)==0;
   QL_TRACE(fprintf(stderr,"_eqEvaluate() rc: %d\n",rc));
   return rc;
}

char * eqToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_NE " : " QL_EQ "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp eqOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_NE : QL_EQ;
}

QLOperationFt qlEqOperationFt={
   _eqEvaluate,
   evaluate,
   eqToString,
   eqOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 





static int _neEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)==0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)!=0;
   QL_TRACE(fprintf(stderr,"_neEvaluate() rc: %d\n",rc));
   return rc;
}

char * neToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_EQ " : " QL_NE "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp neOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_EQ : QL_NE;
}

QLOperationFt qlNeOperationFt={
   _neEvaluate,
   evaluate,
   neToString,
   neOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 




static int _isaEvaluate(QLOperation *op, QLPropertySource* source) 
{
   int rc=0;
   if (op->flag.invert) rc=op->lhod->ft->compare(op->lhod,op->rhod,source)!=0;
   else rc=op->lhod->ft->compare(op->lhod,op->rhod,source)==0;
   QL_TRACE(fprintf(stderr,"_isaEvaluate() rc: %d\n",rc));
   return rc;
}

char * isaToString(QLOperation *op) 
{
   char str[512];
   strcpy(str,op->lhod->ft->toString(op->lhod));
   strcat(str,(op->flag.invert ? " QL_NOT_ISA " : " QL_ISA "));
   strcat(str,(op->rhod ? op->rhod->ft->toString(op->rhod) : "--"));
   return strdup(str); 
}

QLOp isaOperation(QLOperation *op) 
{
   return op->flag.invert ? QL_NOT_ISA : QL_ISA;
}

QLOperationFt qlIsaOperationFt={
   _isaEvaluate,
   evaluate,
   isaToString,
   isaOperation,
   binTraverse,
   relEliminateNots,
   NULL,
   NULL,
   binIsBinaryOperation
}; 




QLOperation* newLtOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_LT;
   op->ft=&qlLtOperationFt;
   return op;
}

QLOperation* newGtOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_GT;
   op->ft=&qlGtOperationFt;
   return op;
}

QLOperation* newLeOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_LE;
   op->ft=&qlLeOperationFt;
   return op;
}

QLOperation* newGeOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_GE;
   op->ft=&qlGeOperationFt;
   return op;
}

QLOperation* newEqOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_EQ;
   op->ft=&qlEqOperationFt;
   QL_TRACE(fprintf(stderr,"--- newEqOperation %p\n",op));
   return op;
}

QLOperation* newNeOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_NE;
   op->ft=&qlNeOperationFt;
   return op;
}

QLOperation* newIsaOperation(QLStatement *qs, QLOperand* lo, QLOperand* ro)
{
   QLOperation *op=qsAllocNew(qs,QLOperation);
   op->lhod=lo;
   op->rhod=ro;
   op->opr=QL_ISA;
   op->ft=&qlIsaOperationFt;
   return op;
}


