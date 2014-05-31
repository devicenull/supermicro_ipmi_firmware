
/*
 * $Id: subcond.c,v 1.4 2007/03/05 18:12:14 mihajlov Exp $
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
 * CMPISubCond implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mlog.h"
#include "native.h"
#include "queryOperation.h"

extern CMPIPredicate *TrackedCMPIPredicate(CMPIValuePtr,CMPIStatus*);

typedef struct native_subcond {
   CMPISubCond sc;
   int mem_state;
   CMPIValuePtr cond;
} NativeSubCond;

static NativeSubCond *__new_subcond(int mode, CMPIValuePtr ptr, CMPIStatus * rc);


/*****************************************************************************/

static CMPIStatus __eft_release(CMPISubCond * cond)
{
   NativeSubCond *c = (NativeSubCond *) cond;

   if (c->mem_state && c->mem_state != MEM_RELEASED) {
      memUnlinkEncObj(c->mem_state);
      c->mem_state = MEM_RELEASED;
      free(c);
      CMReturn(CMPI_RC_OK);
   }
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPISubCond *__eft_clone(const CMPISubCond * cond, CMPIStatus * rc)
{
   NativeSubCond *c = (NativeSubCond *) cond;

   return (CMPISubCond *) __new_subcond(MEM_NOT_TRACKED,c->cond,rc);
}


static CMPICount __eft_getCount(const CMPISubCond* cond, CMPIStatus* rc)
{
   NativeSubCond *c = (NativeSubCond *) cond;
   CMPIArray *conds=(CMPIArray*)c->cond.ptr;
   CMPICount count=0;
   
   if (conds) count=CMGetArrayCount(conds,NULL); 
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return count;
}
              
static CMPIPredicate* __eft_getPredicateAt
              (const CMPISubCond* cond, unsigned int index, CMPIStatus* rc)
{
   NativeSubCond *c = (NativeSubCond *)cond;
   CMPIValuePtr pvp={NULL,0};
   CMPIPredicate *pr=NULL;
   CMPICount count=0;
   CMPIStatus irc={CMPI_RC_ERR_NOT_FOUND,NULL};
   CMPIArray *conds=(CMPIArray*)c->cond.ptr;
   
   if (conds) {
      count=CMGetArrayCount(conds,NULL);
      if (index<count) pvp=CMGetArrayElementAt(conds,index,&irc).value.dataPtr;
   }
   else {
      mlogf(M_ERROR,M_SHOW,"### getPredicateAt, no conds\n");
   }
   if (rc) *rc=irc;
   
   if (pvp.ptr) pr=TrackedCMPIPredicate(pvp,&irc);
   
   return pr;
}              
              
static CMPIPredicate* __eft_getPredicate
              (const CMPISubCond* cond, const char *id, CMPIStatus* rc)
{
    if (rc) CMSetStatus(rc, CMPI_RC_ERR_NOT_SUPPORTED);
    return NULL;   
}

static NativeSubCond *__new_subcond(int mode, CMPIValuePtr ptr, CMPIStatus * rc)
{
   static CMPISubCondFT eft = {
      NATIVE_FT_VERSION,
      __eft_release,
      __eft_clone,
      __eft_getCount,
      __eft_getPredicateAt,
      __eft_getPredicate
   };
   
   static CMPISubCond c = {
      "CMPISubCond",
      &eft
   };
   int state;


   NativeSubCond cond,*tCond;
   memset(&cond, 0, sizeof(cond));
   
   cond.sc = c;
   cond.cond=ptr;
   
   tCond=memAddEncObj(mode, &cond, sizeof(cond),&state);
   tCond->mem_state=state;
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return tCond;
}


CMPISubCond *TrackedCMPISubCond(CMPIValuePtr ptr, CMPIStatus * rc)
{
   return (CMPISubCond*) __new_subcond(MEM_TRACKED, ptr, rc);
}

CMPISubCond *NewCMPISubCond(CMPIValuePtr ptr, CMPIStatus * rc)
{
   return (CMPISubCond*) __new_subcond(MEM_NOT_TRACKED, ptr, rc);
}

