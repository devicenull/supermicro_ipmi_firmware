
/*
 * $Id: predicate.c,v 1.7 2007/10/02 09:02:11 mihajlov Exp $
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
 * CMPIPredicate implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native.h"
#include "queryOperation.h"

typedef struct native_predicate {
   CMPIPredicate pred;
   int mem_state;
   QLOperation* op;
} NativePredicate;

static NativePredicate *__new_predicate(int mode, QLOperation* ptr, CMPIStatus * rc);


/*****************************************************************************/

static CMPIStatus __eft_release(CMPIPredicate * pred)
{
   NativePredicate *p = (NativePredicate *) pred;

   if (p->mem_state && p->mem_state != MEM_RELEASED) {
      memUnlinkEncObj(p->mem_state);
      p->mem_state = MEM_RELEASED;
      free(p);
      CMReturn(CMPI_RC_OK);
   }
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIPredicate *__eft_clone(const CMPIPredicate * pred, CMPIStatus * rc)
{
   NativePredicate *p = (NativePredicate *) pred;

   return (CMPIPredicate *) __new_predicate(MEM_NOT_TRACKED,p->op,rc);
}

static CMPIStatus __eft_getData(const CMPIPredicate* pred, CMPIType* type,
               CMPIPredOp* opc, CMPIString** lhs, CMPIString** rhs)
{
   NativePredicate *p = (NativePredicate *) pred;
   QLOperation *op=p->op,*o;
   CMPIStatus irc={CMPI_RC_OK,NULL};
   
   if (op) {
      if (op->opr==QL_bin) {
         QLOpd type=QL_Invalid;
         if (op->lhon) o=op->lhon;
         else o=op->rhon;
         if (o->lhod && o->lhod->type!=QL_PropertyName) 
            type=o->lhod->type;
         else  if (o->rhod && o->rhod->type!=QL_PropertyName) 
            type=o->rhod->type;     
         if (opc) *opc=o->opr;
         if (lhs) *lhs= sfcb_native_new_CMPIString(o->lhod->ft->toString(o->lhod),NULL, 0);  
         if (rhs) *rhs= sfcb_native_new_CMPIString(o->rhod->ft->toString(o->rhod),NULL, 0);  
      }
      else {
         printf("--- NOT QL_bin\n");
         CMSetStatusWithString(&irc,CMPI_RC_ERR_FAILED,
			       sfcb_native_new_CMPIString("Predicate has no a binary operator.", NULL, 0));
      }
   }   
   return irc;
}

static CMPIBoolean __eft_evaluate (const CMPIPredicate* pred,  
				   CMPIAccessor * acc, void *v, CMPIStatus *rc)
{
    if (rc) CMSetStatus(rc, CMPI_RC_ERR_NOT_SUPPORTED);
    return 0;   
}



static NativePredicate *__new_predicate(int mode, QLOperation *op, CMPIStatus * rc)
{
   static CMPIPredicateFT eft = {
      NATIVE_FT_VERSION,
      __eft_release,
      __eft_clone,
      __eft_getData,
      __eft_evaluate
   };
   
   static CMPIPredicate p = {
      "CMPIPredicate",
      &eft
   };
   int state;

   NativePredicate pred,*tPred;
   memset(&pred, 0, sizeof(pred));
   
   pred.pred = p;
   pred.op=op;
   
   tPred=memAddEncObj(mode, &pred, sizeof(pred),&state);
   tPred->mem_state=state;
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return tPred;
}


CMPIPredicate *TrackedCMPIPredicate(QLOperation *op, CMPIStatus * rc)
{
   return (CMPIPredicate*) __new_predicate(MEM_TRACKED, op, rc);
}

CMPIPredicate *NewCMPIPredicate(QLOperation *op, CMPIStatus * rc)
{
   return (CMPIPredicate*) __new_predicate(MEM_NOT_TRACKED, op, rc);
}

