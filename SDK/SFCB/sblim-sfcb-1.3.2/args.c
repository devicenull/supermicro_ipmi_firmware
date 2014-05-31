
/*
 * $Id: args.c,v 1.13 2007/10/31 15:09:45 sschuetz Exp $
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
 * Author:       Frank Scheffler 
 * Contributors: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 * Extension of the CMPIArgs data type.
 * This structure stores the information needed to represent arguments for
 * CMPI providers, i.e. within invokeMethod() calls.
 *
*/



#include <stdio.h>
#include <string.h>

#include "native.h"
#include "objectImpl.h"
#include "msgqueue.h"

struct native_args {
   CMPIArgs args;               /*!< the inheriting data structure  */
   int mem_state;               /*!< states, whether this object is
                                   registered within the memory mangagement or
                                   represents a cloned object */
};

extern int ClObjectLocateProperty(ClObjectHdr * hdr, ClSection * prps,
                                  const char *id);
extern void ClArgsFree(ClArgs * arg);
extern ClArgs *ClArgsRebuild(ClArgs * arg, void *area);
extern int ClArgsGetArgAt(ClArgs * arg, int id, CMPIData * data, char **name);
extern int ClArgsAddArg(ClArgs * arg, const char *id, CMPIData d);
extern int ClArgsGetArgCount(ClArgs * arg);
extern ClArgs *ClArgsNew();
extern unsigned long ClSizeArgs(ClArgs * arg);
extern void ClArgsRelocateArgs(ClArgs * arg);
extern CMPIArray *native_make_CMPIArray(CMPIData * av, CMPIStatus * rc,
                                        ClObjectHdr * hdr);
extern CMPIObjectPath *getObjectPath(char *path, char **msg);
extern const char *ClObjectGetClString(ClObjectHdr * hdr, ClString * id);
extern CMPIArray *internal_native_make_CMPIArray(CMPIData * av, CMPIStatus * rc,
                                 ClObjectHdr * hdr, int mode);
static struct native_args *__new_empty_args(int, CMPIStatus *);
MsgSegment setArgsMsgSegment(CMPIArgs * args);

/****************************************************************************/


static CMPIStatus __aft_release(CMPIArgs * args)
{
   struct native_args *a = (struct native_args *) args;

   if (a->mem_state && a->mem_state != MEM_RELEASED) {
      ClArgsFree((ClArgs *) a->args.hdl);
      memUnlinkEncObj(a->mem_state);
      a->mem_state = MEM_RELEASED;
      free(args);
      CMReturn(CMPI_RC_OK);
   }
   
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIArgs *__aft_clone(const CMPIArgs * args, CMPIStatus * rc)
{
   struct native_args *a = (struct native_args *) args;
   struct native_args *na = __new_empty_args(MEM_NOT_TRACKED, rc);

   //  if (rc->rc == CMPI_RC_OK) {
   na->args.hdl = ClArgsRebuild((ClArgs *) a->args.hdl, NULL);
   //  }

   return (CMPIArgs *) na;
}


static CMPIStatus __aft_addArg(const CMPIArgs * args, const char *name,
                               const CMPIValue * value, CMPIType type)
{
   ClArgs *ca = (ClArgs *) args->hdl;
   CMPIData data = { type, CMPI_goodValue, {0} };

   if (type == CMPI_chars) {
      /* VM: is this OK or do we need a __new copy */
      data.value.chars = (char *) value;
   } else if (type == CMPI_string) {
      data.type=CMPI_chars;
      if (value && value->string && value->string->hdl) {
	 /* VM: is this OK or do we need a __new copy */
         data.value.chars = (char *) value->string->hdl;
      } else {
	 data.value.chars=NULL;
      }
   } else if (value) {
	   sfcb_setAlignedValue(&data.value,value,type);
   }

   if (((type & CMPI_ENCA) && data.value.chars == NULL) || value == NULL) {
     data.state=CMPI_nullValue;
   }

   ClArgsAddArg(ca, name, data);

   CMReturn(CMPI_RC_OK);
}


static CMPIData __aft_getArgAt(const CMPIArgs * args,
                               unsigned int i,
                               CMPIString ** name, CMPIStatus * rc)
{
   ClArgs *ca = (ClArgs *) args->hdl;
   char *n;
   CMPIData rv = { 0, CMPI_notFound, {0} };

   if (ClArgsGetArgAt(ca, i, &rv, name ? &n : NULL)) {
      if (rc)
         CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
      return rv;
   }
   if (rv.type == CMPI_chars) {
      rv.value.string = sfcb_native_new_CMPIString(rv.value.chars, NULL, 0);
      rv.type = CMPI_string;
   }
   else if (rv.type == CMPI_ref) {
      char *msg;
      rv.value.ref = getObjectPath(
         (char*)ClObjectGetClString(&ca->hdr, (ClString *) & rv.value.chars), &msg);
   }
   else if (rv.type & CMPI_ARRAY && rv.value.array) {
      rv.value.array =
          internal_native_make_CMPIArray((CMPIData *) rv.value.array, NULL, &ca->hdr, MEM_TRACKED);
   }
   if (name) {
      *name = sfcb_native_new_CMPIString(n, NULL, 0);
   }
   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return rv;
}

static CMPIData __aft_getArg(const CMPIArgs * args, const char *name, CMPIStatus * rc)
{
   ClArgs *ca = (ClArgs *) args->hdl;
   ClSection *prps = &ca->properties;
   CMPIData rv = { 0, CMPI_notFound, {0} };
   int i;

   if ((i = ClObjectLocateProperty(&ca->hdr, prps, name)) != 0) {
      return __aft_getArgAt(args, i - 1, NULL, rc);
   }

   if (rc)
      CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
   return rv;
}


static unsigned int __aft_getArgCount(const CMPIArgs * args, CMPIStatus * rc)
{
   ClArgs *ca = (ClArgs *) args->hdl;
   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return (CMPICount) ClArgsGetArgCount(ca);
}


static CMPIArgsFT aft = {
   NATIVE_FT_VERSION, 
   __aft_release,
   __aft_clone,
   __aft_addArg,
   __aft_getArg,
   __aft_getArgAt,
   __aft_getArgCount
};

static struct native_args *__new_empty_args(int mm_add, CMPIStatus * rc)
{
   static CMPIArgs a = {
      "CMPIArgs",
      &aft
   };
   
   struct native_args args, *tArgs;
   int state;

   args.args = a;
   tArgs=memAddEncObj(mm_add, &args, sizeof(args),&state);
   tArgs->mem_state = state;
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return (struct native_args*) tArgs;
}


CMPIArgs *NewCMPIArgs(CMPIStatus * rc)
{
   struct native_args *args = __new_empty_args(MEM_NOT_TRACKED, rc);
   args->args.hdl = ClArgsNew();
   return (CMPIArgs *)args;
}

CMPIArgs *TrackedCMPIArgs(CMPIStatus * rc)
{
   struct native_args *args = __new_empty_args(MEM_TRACKED, rc);
   args->args.hdl = ClArgsNew();
   return (CMPIArgs *)args;
}

unsigned long getArgsSerializedSize(CMPIArgs * args)
{
   ClArgs *ca = (ClArgs *) args->hdl;
   return ClSizeArgs(ca) + sizeof(struct native_args);
}

void getSerializedArgs(CMPIArgs * args, void *area)
{
   if (args) {
      memcpy(area, args, sizeof(struct native_args));
      ClArgsRebuild((ClArgs *) args->hdl,
                    (void *) ((char *) area + sizeof(struct native_args)));
   }
   else {

   }
}

CMPIArgs *relocateSerializedArgs(void *area)
{
   struct native_args *args = (struct native_args *) area;
   args->args.hdl = args + 1;
   args->args.ft = &aft;
   ClArgsRelocateArgs((ClArgs *) args->args.hdl);
   return (CMPIArgs *) args;
}

MsgSegment setArgsMsgSegment(CMPIArgs * args)
{
   MsgSegment s;
   s.data = args;
   s.type = MSG_SEG_ARGS;
   s.length = args ? getArgsSerializedSize(args) : 0;
   return s;
}

CMPIString *args2String(CMPIArgs * arg, CMPIStatus * rc)
{
   char * argstr = ClArgsToString((ClArgs*)arg->hdl);
   return sfcb_native_new_CMPIString(argstr, NULL, 0);
}


/*****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 8 ***/
/*** End:              ***/
