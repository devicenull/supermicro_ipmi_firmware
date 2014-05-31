
/*
 * $Id: queryStatement.c,v 1.14 2007/06/22 10:34:43 sschuetz Exp $
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
 * query statement encapsulation support.
 *
*/


#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "queryOperation.h"
#include "mlog.h"

extern int sfcQueryparse(QLControl*);
extern CMPIBroker *Broker;
extern void *qsAlloc(QLStatement *qs, unsigned int size);

static char* q=NULL;
static QLStatementFt stmtFt;
static int ofs=0;

int queryInput(char* buffer, int* done, int requested)
{
   int left=strlen(q) - ofs;

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

char * sfcQueryStrcpy(char *txt, int n)
{
   int dummy;
   char* str=(char*)malloc(n+1);
   memAdd(str,&dummy);
   memcpy(str,txt,n);
   str[n]=0;
   return str;
}

void sfcQueryErr(char* s1, char* s2, char* s3)
{
   mlogf(M_ERROR,M_SHOW,"--- %s %s %s\n",s1,s2,s3);
}

void sfcQueryError(char* s1)
{
   mlogf(M_ERROR,M_SHOW,"--- %s\n",s1);
}

static char** ensureCharsListSpace(QLStatement *qs, char ***in, int *max, int nxt)
{
   char** list=*in;
   if (*max-1<=nxt) {
      *max=*max*2;
      list=*in=(char**)qsAlloc(qs,*max*sizeof(char*));
   }
   return list;
}   


QLStatement* newQLStatement(int fcm, int mode)
{
   QLStatement *qs;
   int x;
   
   qs=(QLStatement*)calloc(1,sizeof(QLStatement));
   if (mode==MEM_TRACKED) {
      qs->allocList=NULL;
      memAdd(qs,&x);
   }
   else {
      qs->allocList=(void**)malloc(16*sizeof(void*));
      qs->allocMax=16;
      qs->allocList[qs->allocNext++]=qs;
   }
   qs->allocMode=mode;
   qs->ft=&stmtFt; 
   
   qs->spMax=qs->fcMax=fcm;
   qs->fClasses=(char**)qsAlloc(qs,fcm*sizeof(char*));
   qs->fClasses[0]=NULL;
   qs->spNames=(char**)qsAlloc(qs,fcm*sizeof(char*));
   qs->spNames[0]=NULL;
   
   return qs;  
}

static void qsSetAllProperties(QLStatement *st, int all)
{
    st->allProps=all;
}

static void qsAppendSelectPropertyName(QLStatement *st, char* pn)
{
   char **names=ensureCharsListSpace(st,&st->spNames,&st->spMax,st->spNext);
   names[st->spNext++]=pn;
   names[st->spNext]=NULL;
   QL_TRACE(fprintf(stderr,"--- appendSelectPropertyName: %s\n",pn));  
}

static void qsAddFromClass(QLStatement *st, char *cn, char *ca) 
{
   char **cls=ensureCharsListSpace(st,&st->fClasses,&st->fcMax,st->fcNext);
   cls[st->fcNext++]=cn;
   cls[st->fcNext]=NULL;
   QL_TRACE(fprintf(stderr,"--- addFromClass: %s %s\n",cn,ca));  
}

static char **qsGetFromClassList(QLStatement *st) 
{
   return st->fClasses;  
}

static void qsSetWhereCondition(QLStatement *st, QLOperation *op)
{
   st->where=op;
   QL_TRACE(fprintf(stderr,"--- setWhereCondition: %p\n",op));  
}

static int qsTestPropertyClass(QLStatement *st, char *cl) 
{
   int i;
   QL_TRACE(fprintf(stderr,"--- testPropertyClass: %s\n",cl));  
   for (i=0; i<st->fcNext; i++)
     if ((strcasecmp(cl,st->fClasses[i]))==0) return 1;
   return 0;
}

static void qsRelease(QLStatement *st)
{
    if (st && st->allocMode != MEM_TRACKED) {
    	if(st->sns) {
    	    free(st->sns);
    	}
    	/* free everything but the first element of allocList, which is
    	 * the memory allocated for the QLStatement itself, see
    	 * newQLStatement(...). The struct is then freed after the loop */
        while(st->allocNext > 1) {
            free(st->allocList[--st->allocNext]);
        }
        free(st->allocList);
        free(st);
    }
}

static CMPIInstance* qsCloneAndFilter(QLStatement *st, CMPIInstance *ci, CMPIObjectPath *cop, 
         char **kNames)
{
   CMPIInstance *nic=CMNewInstance(Broker,cop,NULL);
   CMPICount i,c=CMGetPropertyCount(ci,NULL);
   CMSetPropertyFilter(nic,(const char**)st->spNames, (const char**)kNames);
   for (i=0; i<c; i++) {
      CMPIString *name;
      CMPIData d=CMGetPropertyAt(ci,i,&name,NULL);
      CMSetProperty(nic,(char*)name->hdl,&d.value,d.type);
      CMRelease(name);
   } 
   return nic;   
}


static void qcClear(QLCollector *qc) {
   QL_TRACE(fprintf(stderr,"qcClear\n"));
   qc->propName=NULL;
}

#ifdef NEEDS_CLEANUP
static QLOperand* newNameOperand(QLStatement *qs)
{    
   QLOperand* o=newPropQueryOperand(qs,newPropertyNameData(qs)); 
   QL_TRACE(fprintf(stderr,"newNameOperand %p\n",o));
   return o; 
}
#endif

static void qcAddPropIdentifier(QLCollector *qc, QLStatement *qs, char *cls, char *prop, int index)
{
   QLPropertyNameData **pi;
   for (pi=&qc->propName; 1; pi=&(*pi)->nextPart)
      if (*pi==NULL) {
         *pi=newPropertyNameData(qs);
         (*pi)->className=cls;
         (*pi)->propName=prop;
         (*pi)->index=index;
         (*pi)->nextPart=NULL;
         break;
      }
}

void *qsAlloc(QLStatement *qs, unsigned int size)
{
  void *ptr=calloc(1,size);
  if (qs && qs->allocMode!=MEM_TRACKED) {
    qs->allocList[qs->allocNext++]=ptr;
    if (qs->allocNext==qs->allocMax) {
      qs->allocMax*=2;
      qs->allocList=(void**)realloc(qs->allocList,qs->allocMax*sizeof(void*));
    }
  } else {
    int x;
    memAdd(ptr, &x);
  }
  return ptr;
}

static QLStatementFt stmtFt={
   qsRelease,
   qsCloneAndFilter,
   qsSetAllProperties,
   qsAppendSelectPropertyName,
   qsAddFromClass,
   qsSetWhereCondition,
   qsTestPropertyClass,
   qsGetFromClassList
};   
   

QLStatement *parseQuery(int mode, const char *query, const char *lang, const char *sns, int *rc)
{
   QLStatement *qs=NULL;
   QLCollector ctlFt={
      qcClear,  
      qcAddPropIdentifier,
      NULL,
   };
   
   QLControl ctl={
      qs,
      &ctlFt
   };
   
   q=(char*)query;
   ofs=0;
   ctl.statement=qs=newQLStatement(8,mode);
   if (strcasecmp(lang,"wql")==0) ctl.statement->lang=QL_WQL;
   else if (strcasecmp(lang,"cql")==0 || strcasecmp(lang,"cim:cql")==0) {
     ctl.statement->lang=QL_CQL;
   } else ctl.statement->lang=0;
   
   *rc=sfcQueryparse(&ctl);
   if (sns) qs->sns=strdup(sns);
   else sns=NULL;
   
   return qs;
}
