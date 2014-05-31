
/*
 * selectcond.c
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
 * Author:        Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPISelectCond implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "native.h"
#include "queryOperation.h"

extern CMPISubCond *TrackedCMPISubCond(CMPIValuePtr,CMPIStatus*);

typedef struct native_selectcond {
   CMPISelectCond cond;
   int mem_state;
   CMPIArray *conds;
   int type;
} NativeSelectCond;

static NativeSelectCond *__new_cond();


/*****************************************************************************/

static CMPIStatus __eft_release(CMPISelectCond * cond)
{
   NativeSelectCond *c = (NativeSelectCond *) cond;

   if (c->mem_state && c->mem_state != MEM_RELEASED) {
      memUnlinkEncObj(c->mem_state);
      c->mem_state = MEM_RELEASED;
      free(c);
      CMReturn(CMPI_RC_OK);
   }
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPISelectCond *__eft_clone(const CMPISelectCond * cond, CMPIStatus * rc)
{
   NativeSelectCond *c = (NativeSelectCond *) cond;

   return (CMPISelectCond *) __new_cond(MEM_NOT_TRACKED,c->conds,c->type,rc);
}


static CMPICount __eft_getCountAndType
              (const CMPISelectCond* cond, int* type, CMPIStatus* rc)
{
   NativeSelectCond *c = (NativeSelectCond *) cond;
   CMPICount count=0;
   
   if (type) *type=c->type;
   if (c->conds) count=CMGetArrayCount(c->conds,NULL); 
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return count;
}
              
static CMPISubCond* __eft_getSubCondAt
              (const CMPISelectCond* cond, unsigned int index, CMPIStatus* rc)
{
   NativeSelectCond *c = (NativeSelectCond *) cond;
   CMPIValuePtr vp={NULL,0};
   CMPISubCond *sc=NULL;
   CMPICount count=0;
   CMPIStatus irc={CMPI_RC_ERR_NOT_FOUND,NULL};
   
   if (c->conds) {
      count=CMGetArrayCount(c->conds,NULL);
      if (index<count) vp=CMGetArrayElementAt(c->conds,index,&irc).value.dataPtr;
   }
   if (rc) *rc=irc;
   
   if (vp.ptr) sc=TrackedCMPISubCond(vp,&irc);
   
   return sc;
}              

static NativeSelectCond *__new_cond(int mode, CMPIArray *conds, int type, CMPIStatus * rc)
{
   static CMPISelectCondFT eft = {
      NATIVE_FT_VERSION,
      __eft_release,
      __eft_clone,
      __eft_getCountAndType,
      __eft_getSubCondAt
   };
   
   static CMPISelectCond c = {
      "CMPISelectCond",
      &eft
   };
   int state;


   NativeSelectCond cond,*tCond;
   memset(&cond, 0, sizeof(cond));
   
   cond.cond = c;
   cond.conds=conds;
   cond.type=type;
   
   tCond=memAddEncObj(mode, &cond, sizeof(cond),&state);
   tCond->mem_state=state;
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return tCond;
}


CMPISelectCond *TrackedCMPISelectCond(CMPIArray *conds, int type, CMPIStatus * rc)
{
   return (CMPISelectCond*) __new_cond(MEM_TRACKED, conds, type, rc);
}

CMPISelectCond *NewCMPISelectCond(CMPIArray *conds, int type, CMPIStatus * rc)
{
   return (CMPISelectCond*) __new_cond(MEM_NOT_TRACKED, conds, type, rc);
}

