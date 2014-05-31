/*
 * $Id: instance.c,v 1.44 2008/09/10 18:58:43 buccella Exp $
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
 * Author:        Frank Scheffler
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * CMPIInstance implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilft.h"
#include "native.h"
#include "instance.h"

#include "objectImpl.h"
#include "providerMgr.h"
#include "config.h"
#include "sfcUtil/hashtable.h"
#include "array.h"

#ifdef SFCB_IX86
#define SFCB_ASM(x) asm(x)
#else
#define SFCB_ASM(x)
#endif

extern int ClInstanceGetPropertyAt(ClInstance * inst, int id, CMPIData * data,
                                   char **name, unsigned long *quals);
extern int ClObjectLocateProperty(ClObjectHdr * hdr, ClSection * prps,
                                  const char *id);
extern int ClInstanceGetPropertyCount(ClInstance * inst);
extern int ClInstanceAddProperty(ClInstance * inst, const char *id, CMPIData d);
extern ClInstance *ClInstanceNew(const char *ns, const char *cn);
extern void ClInstanceFree(ClInstance * inst);
extern const char *ClInstanceGetClassName(ClInstance * inst);
extern const char *ClInstanceGetNameSpace(ClInstance * inst);
extern unsigned long ClSizeInstance(ClInstance * inst);
extern ClInstance *ClInstanceRebuild(ClInstance * inst, void *area);
extern void ClInstanceRelocateInstance(ClInstance * inst);
extern CMPIArray *native_make_CMPIArray(CMPIData * av, CMPIStatus * rc,
                                        ClObjectHdr * hdr);
extern CMPIObjectPath *getObjectPath(char *path, char **msg);
CMPIObjectPath *internal_new_CMPIObjectPath(int mode, const char *nameSpace,
                                            const char *className,
                                            CMPIStatus * rc);
extern const char *ClObjectGetClString(ClObjectHdr * hdr, ClString * id);
extern int sfcb_comp_CMPIValue(CMPIValue *val1, CMPIValue *val2, CMPIType type);
extern long addClString(ClObjectHdr * hdr, const char *str);

extern CMPIBroker *Broker;

struct native_instance {
   CMPIInstance instance;
   int refCount;
   int mem_state;
   int filtered;
   char **property_list;
   char **key_list;
};

#ifdef HAVE_DEFAULT_PROPERTIES 
static void instFillDefaultProperties(struct native_instance *inst, 
				      const char * ns, const char * cn);
#endif

/****************************************************************************/

static void __release_list(char **list)
{
   if (list) {
      char **tmp = list;
      while (*tmp) free(*tmp++);
      free(list);
   }
}


static char **__duplicate_list(const char **list)
{
   char **result = NULL;

   if (list) {
      size_t size = 1;
      char **tmp = (char**)list;

      while (*tmp++) ++size;
      result = calloc(1,size*sizeof(char *));
      for (tmp = result; *list; tmp++) {
         *tmp = strdup(*list++);
      }
   }
   return result;
}


void memLinkInstance(CMPIInstance *ci)
{
   struct native_instance *i = (struct native_instance *) ci;
   memLinkEncObj(i,&i->mem_state);
}

static int __contained_list(char **list, const char *name)
{
   if (list) {
      while (*list) {
         if (strcasecmp(*list++, name) == 0) return 1;
      }   
   }
   return 0;
}


/****************************************************************************/

void Zfree_value(void *value)
{
	struct native_array *tmp = (struct native_array *) value;
	if (tmp != NULL)
	{
		tmp->array.ft->release(tmp);
	}
	tmp = NULL;
}

/* b_control, 0: for free() 1: for alloc() */
UtilHashTable *control_hashtable(unsigned char b_control)
{
	static UtilHashTable *klt = NULL;
	
	if (b_control)	// 1: for alloc()
	{
		if (klt == NULL)
			klt = UtilFactory->newHashTable(61, UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);
	}
	else
	{
		Util_HashTable_FT *ft;
		HashTable *hdl;
		if (klt != NULL)
		{
			ft = (Util_HashTable_FT *) (klt->ft);
			hdl = (HashTable *) (klt->hdl);
			if (hdl->keyDeallocator == NULL)
				hdl->keyDeallocator = free;
			if (hdl->valueDeallocator == NULL)
				hdl->valueDeallocator = Zfree_value;
			ft->release(klt);
			klt = NULL;
		}
	}
	return klt;
}

/* b_control, 0: for free() 1: for alloc() */
CMPI_MUTEX_TYPE *control_mtx(unsigned char b_control)
{
	static CMPI_MUTEX_TYPE * mtx = NULL;
	
	if (b_control)	// 1: for alloc()
	{
		if (mtx == NULL)
		{
			int dummy = 0;
			mtx = memAlloc(MEM_TRACKED, sizeof(CMPI_MUTEX_TYPE), &dummy);
			*mtx = Broker->xft->newMutex(0);
		}
	}
	else			// 0: for free()
	{
		if (mtx != NULL)
		{
			if (*mtx != NULL)
				Broker->xft->destroyMutex(*mtx);
			free(mtx);
			mtx = NULL;
		}
	}
	return mtx;
}

static CMPIStatus __ift_release(CMPIInstance * instance)
{
   struct native_instance *i = (struct native_instance *) instance;

   if (i->mem_state && i->mem_state != MEM_RELEASED) {
      __release_list(i->property_list);
      __release_list(i->key_list);
      ClInstanceFree((ClInstance *) instance->hdl);
      memUnlinkEncObj(i->mem_state);
      i->mem_state = MEM_RELEASED;
      free(i);
      control_hashtable(0);
      control_mtx(0);
      CMReturn(CMPI_RC_OK);
   }
   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIInstance *__ift_clone(const CMPIInstance * instance, CMPIStatus * rc)
{
   struct native_instance *i = (struct native_instance*) instance;
   struct native_instance *new = (struct native_instance*)
       malloc(sizeof(struct native_instance));

   new->mem_state=MEM_NOT_TRACKED;
   new->property_list = __duplicate_list((const char**)i->property_list);
   new->key_list = __duplicate_list((const char**)i->key_list);
   new->filtered = i->filtered;

   ((CMPIInstance*)new)->hdl =
       ClInstanceRebuild((ClInstance *) instance->hdl, NULL);
   ((CMPIInstance*)new)->ft = instance->ft;

   return (CMPIInstance *) new;
}

CMPIData __ift_internal_getPropertyAt(const CMPIInstance * ci, CMPICount i, 
				      char ** name,
				      CMPIStatus * rc, int readonly)
{
   ClInstance *inst = (ClInstance *) ci->hdl;
   CMPIData rv = { 0, CMPI_notFound, {0} };
   if (ClInstanceGetPropertyAt(inst, i, &rv, name, NULL)) {
      if (rc)
         CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
      return rv;
   }

   
   if (rv.type == CMPI_chars) {
      rv.value.string = 
	 sfcb_native_new_CMPIString(rv.value.chars, NULL, readonly ? 2 : 0);
      rv.type = CMPI_string;
   } else if (readonly == 0 && rv.type == CMPI_string) {
      /* Not read-only -  must make a managed copy */
      rv.value.string = 
	 sfcb_native_new_CMPIString(rv.value.string->hdl, NULL,0);
   }  else if (rv.type == CMPI_ref) {
      char *msg;
      rv.value.ref = getObjectPath(
         (char*)ClObjectGetClString(&inst->hdr, (ClString*)&rv.value.chars), &msg);
   }
   else if (rv.type & CMPI_ARRAY && rv.value.array) {
      rv.value.array =
          native_make_CMPIArray((CMPIData *) rv.value.array, NULL, &inst->hdr);
   }

   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return rv;
}

CMPIData __ift_getPropertyAt(const CMPIInstance * ci, CMPICount i, CMPIString ** name,
                             CMPIStatus * rc)
{
   char * sname;
   CMPIData rv = __ift_internal_getPropertyAt(ci, i, &sname, rc, 0);
   if (name) {
      *name = sfcb_native_new_CMPIString(sname, NULL, 0);
   }
   return rv;
}

CMPIData __ift_getProperty(const CMPIInstance * ci, const char *id, CMPIStatus * rc)
{
   ClInstance *inst = (ClInstance *) ci->hdl;
   ClSection *prps = &inst->properties;
   CMPIData rv = { 0, CMPI_notFound, {0} };
   int i;

   if ((i = ClObjectLocateProperty(&inst->hdr, prps, id)) != 0) {
      return __ift_getPropertyAt(ci, i - 1, NULL, rc);
   }

   if (rc)
      CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
   return rv;
}

static CMPICount __ift_getPropertyCount(const CMPIInstance * ci, CMPIStatus * rc)
{
   ClInstance *inst = (ClInstance *) ci->hdl;
   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return (CMPICount) ClInstanceGetPropertyCount(inst);
}


static CMPIStatus __ift_setProperty(const CMPIInstance * instance,
                                    const char *name,
                                    const CMPIValue * value, CMPIType type)
{
   struct native_instance *i = (struct native_instance *) instance;
   ClInstance *inst = (ClInstance *) instance->hdl;
   CMPIData data = { type, CMPI_goodValue, {0LL} };
   int rc;

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

   if (i->filtered == 0 ||
       i->property_list == NULL ||
       __contained_list(i->property_list, name) ||
       __contained_list(i->key_list, name)) {

      rc=ClInstanceAddProperty(inst, name, data);
      
      if(i->filtered && !__contained_list(i->property_list, name)
         && __contained_list(i->key_list, name)) {
          /* rc is the number of properties used, so we have to substract one here */
          ClInstanceFilterFlagProperty(inst, rc - 1);
      }
      if (rc<0) CMReturn(-rc);      
   }
   CMReturn(CMPI_RC_OK);
}

static CMPIStatus __ift_setObjectPath(CMPIInstance * inst,
                                      const CMPIObjectPath * cop)
{
   CMPIStatus tmp1, tmp2, tmp3;
   CMPIString *str;
   const char *ns, *cn;
   int j;
   CMPIStatus rc = { CMPI_RC_OK, NULL };

   /* get information out of objectpath */
   if (cop) {
      j = CMGetKeyCount(cop, &tmp1);
      str = CMGetClassName(cop, &tmp2);
      cn = CMGetCharsPtr(str, NULL);
      str = CMGetNameSpace(cop, &tmp3);
      ns = CMGetCharsPtr(str, NULL);
   }

   /* there SHOULD be an op passed in, otherwise this call is useless.... */
   else {
      j=0;
      //SFCB_ASM("int $3");
      ns = "*NoNameSpace*";
      cn = "*NoClassName*";
      tmp1.rc=tmp2.rc=tmp3.rc=CMPI_RC_OK;
   }

   if (tmp1.rc != CMPI_RC_OK || tmp2.rc != CMPI_RC_OK || tmp3.rc != CMPI_RC_OK) {
      rc.rc = CMPI_RC_ERR_FAILED;
      return rc;
   }
   else {

      /* set cn and ns in inst */
      ClInstance* instance = (ClInstance*)inst->hdl;
      if (ns) instance->nameSpace.id = addClString(&instance->hdr, ns);
      if (cn) instance->className.id = addClString(&instance->hdr, cn);

      /* loop over keys, set them in the inst */
      while (j-- && (tmp1.rc == CMPI_RC_OK)) {
         CMPIString *keyName;
         CMPIData tmp = CMGetKeyAt(cop, j, &keyName, &tmp1);
         __ift_setProperty(inst, CMGetCharsPtr(keyName, NULL),
                           &tmp.value, tmp.type);
      }
      return tmp1;
   }

   return rc;

}

static CMPIObjectPath *__ift_getObjectPath(const CMPIInstance * instance,
                                           CMPIStatus * rc)
{
   CMPI_MUTEX_TYPE *mtx;
   UtilHashTable *klt;
   int j, f = 0;
   CMPIStatus tmp;
   const char *cn = ClInstanceGetClassName((ClInstance *) instance->hdl);
   const char *ns = ClInstanceGetNameSpace((ClInstance *) instance->hdl);

   CMPIObjectPath *cop;
   cop = TrackedCMPIObjectPath(ns, cn, rc);

   if (rc && rc->rc != CMPI_RC_OK)
      return NULL;

   j = __ift_getPropertyCount(instance, NULL);

   while (j--) {
      char *keyName;
      CMPIData d = __ift_internal_getPropertyAt(instance, j, &keyName, &tmp, 1);
      if (d.state & CMPI_keyValue) {
         CMAddKey(cop, keyName, &d.value, d.type);
         f++;
      }
      if (d.type & CMPI_ARRAY && (d.state & CMPI_nullValue) == 0) {
         d.value.array->ft->release(d.value.array);
      }   
   }

   if (f == 0) {
      CMPIArray *kl;
      CMPIData d;
      unsigned int e, m;

	  mtx = control_mtx(1);
//      if (mtx == NULL) {
//         int dummy = 0;
//	 mtx = memAlloc(MEM_TRACKED, sizeof(CMPI_MUTEX_TYPE), &dummy);
//	 *mtx = Broker->xft->newMutex(0); 
//      }
      Broker->xft->lockMutex(*mtx);
      klt = control_hashtable(1);
//      if (klt == NULL) klt = UtilFactory->newHashTable(61,
//                 UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);

      if ((kl = klt->ft->get(klt, cn)) == NULL) {	 
	 CMPIConstClass * cc = getConstClass(ns,cn);
	 if(cc) {
	    kl = cc->ft->getKeyList(cc);
            klt->ft->put(klt, strdup(cn), kl);
	 } else {
	    if(rc) {
	       CMSetStatus(rc, CMPI_RC_ERR_INVALID_CLASS);
	    }
	    Broker->xft->unlockMutex(*mtx);
	    return NULL;
	 }
      }
      Broker->xft->unlockMutex(*mtx);
      m = kl->ft->getSize(kl, NULL);

      for (e = 0; e < m; e++) {
         CMPIString *n = kl->ft->getElementAt(kl, e, NULL).value.string;
         d = __ift_getProperty(instance, CMGetCharPtr(n), &tmp);
         if (tmp.rc == CMPI_RC_OK) {
            CMAddKey(cop, CMGetCharPtr(n), &d.value, d.type);
         }
      }
   }
   return cop;
}


static CMPIStatus __ift_setPropertyFilter(CMPIInstance * instance,
                                          const char **propertyList, 
					  const char **keys)
{
    int j,m;
    CMPIObjectPath * cop;
    CMPIInstance * newInstance;
    CMPIStatus st;
    CMPIData data;
    char * name;
    struct native_instance *i = (struct native_instance *) instance;
    struct native_instance *iNew,iTemp;
    
    if (propertyList == NULL) {
        /* NULL property list, no need to set filter */
        CMReturn(CMPI_RC_OK);
    }

    cop = TrackedCMPIObjectPath(instGetNameSpace(instance), instGetClassName(instance), NULL);
    if(cop) {
        if(i->mem_state == MEM_RELEASED || i->mem_state > 0) {
            newInstance = internal_new_CMPIInstance(MEM_TRACKED, cop, &st, 1);
        } else {
            newInstance = internal_new_CMPIInstance(MEM_NOT_TRACKED, cop, &st, 1);
        }
        iNew = (struct native_instance *) newInstance;
        iNew->filtered = 1;
        iNew->property_list = __duplicate_list(propertyList);
        iNew->key_list = __duplicate_list(keys);
        for (j = 0, m = __ift_getPropertyCount(instance, &st); j < m; j++) {
	    data = __ift_internal_getPropertyAt(instance, j, &name, &st, 1);
            if(__contained_list((char**)propertyList, name) || __contained_list((char**)keys, name)) {
               if ( (data.state & ~CMPI_keyValue) != 0) {
                  newInstance->ft->setProperty(
                     newInstance, name, NULL, data.type);
               } else {
                  newInstance->ft->setProperty(
                     newInstance, name, &data.value, data.type);
               }
            }
        }
        
        if(i->mem_state == MEM_RELEASED) {
            memcpy(i, iNew, sizeof(struct native_instance));
        }
        else {
            memcpy(&iTemp, i, sizeof(struct native_instance));
            memcpy(i, iNew, sizeof(struct native_instance));
            i->refCount = iTemp.refCount;
            memcpy(iNew, &iTemp, sizeof(struct native_instance));
            if(iNew->mem_state > 0) {
                iNew->mem_state = i->mem_state;
                i->mem_state = iTemp.mem_state;
            } else {
                newInstance->ft->release(newInstance);
            }
        }
    }
    
    CMReturn(CMPI_RC_OK);
}

static CMPIData 
__ift_getQualifier(CMPIInstance* inst, const char *name, CMPIStatus* rc)
{
   CMPIData data = { 0, CMPI_notFound, {0} };
   if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_FOUND);
   return data;
}

static CMPIData 
__ift_getQualifierAt(CMPIInstance* inst, unsigned int index, CMPIString** name,CMPIStatus* rc)
{
   CMPIData data = { 0, CMPI_notFound, {0} };
   if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_FOUND);
   return data;
}

static unsigned int 
__ift_getQualifierCount(CMPIInstance* inst, CMPIStatus* rc)
{
   if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_FOUND);
   return 0;
}
              
              
static CMPIData 
__ift_getPropertyQualifier(CMPIInstance* inst, const char *pname, const char *qname, CMPIStatus* rc)
{
   CMPIData data = { 0, CMPI_notFound, {0} };
   if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_FOUND);
   return data;
}

static CMPIData 
__ift_getPropertyQualifierAt(CMPIInstance* inst, const char *pname, unsigned int index, CMPIString** name, CMPIStatus* rc)
{
   CMPIData data = { 0, CMPI_notFound, {0} };
   if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_FOUND);
   return data;
}

static unsigned int 
__ift_getPropertyQualifierCount(CMPIInstance* inst, const char *pname, CMPIStatus* rc)
{
   if (rc) CMSetStatus(rc,CMPI_RC_ERR_NOT_FOUND);
   return 0;
}
 
void add(char **buf, unsigned int *p, unsigned int *m, char *data)
{
   unsigned int ds = strlen(data) + 1;

   if (*buf == NULL) {
      *buf = (char *) malloc(1024);
      *p = 0;
      *m = 1024;
   }
   if ((ds + (*p)) >= *m) {
      unsigned nm = *m;
      char *nb;
      while ((ds + (*p)) >= nm)
         nm *= 2;
      nb = (char *) malloc(nm);
      memcpy(nb, *buf, *p);
      free(*buf);
      *buf = nb;
      *m = nm;
   }
   memcpy(*buf + (*p), data, ds);
   *p += ds - 1;
}

extern char *sfcb_value2Chars(CMPIType type, CMPIValue * value);

CMPIString *instance2String(CMPIInstance * inst, CMPIStatus * rc)
{
   CMPIObjectPath *path;
   CMPIData data;
   CMPIString *name, *ps, *rv;
   unsigned int i, m;
   char *buf = NULL, *v, *pname;
   unsigned int bp, bm;

   add(&buf, &bp, &bm, "Instance of ");
   path = __ift_getObjectPath(inst, NULL);
   name =  path->ft->toString(path, rc);
   add(&buf, &bp, &bm, (char *) name->hdl);
   add(&buf, &bp, &bm, " {\n");
   ps = path->ft->toString(path, rc);
   add(&buf, &bp, &bm, " PATH: ");
   add(&buf, &bp, &bm, (char *) ps->hdl);
   add(&buf, &bp, &bm, "\n");

   for (i = 0, m = __ift_getPropertyCount(inst, rc); i < m; i++) {
      data = __ift_internal_getPropertyAt(inst, i, &pname, rc, 1);
      add(&buf, &bp, &bm, " ");
      add(&buf, &bp, &bm, pname);
      add(&buf, &bp, &bm, " = ");
      v = sfcb_value2Chars(data.type, &data.value);
      add(&buf, &bp, &bm, v);
      free(v);
      add(&buf, &bp, &bm, " ;\n");
   }
   add(&buf, &bp, &bm, "}\n");
   rv = sfcb_native_new_CMPIString(buf, rc, 1);
   return rv;
}

static CMPIInstanceFT ift = {
   NATIVE_FT_VERSION,
   __ift_release,
   __ift_clone,
   __ift_getProperty,
   __ift_getPropertyAt,
   __ift_getPropertyCount,
   __ift_setProperty,
   __ift_getObjectPath,
   __ift_setPropertyFilter,
   __ift_setObjectPath
};

static struct {
   unsigned int ftVersion;
   CMPIStatus (*release)
      (CMPIInstance* inst);
   CMPIInstance* (*clone)
      (const CMPIInstance* inst, CMPIStatus* rc);
   CMPIData (*getProperty)
      (const CMPIInstance* inst, const char *name, CMPIStatus* rc);
   CMPIData (*getPropertyAt)
      (const CMPIInstance* inst, unsigned int index, CMPIString** name,
       CMPIStatus* rc);
   unsigned int (*getPropertyCount)
      (const CMPIInstance* inst, CMPIStatus* rc);
   CMPIStatus (*setProperty)
      (const CMPIInstance* inst, const char *name,
       const CMPIValue* value, CMPIType type);
   CMPIObjectPath* (*getObjectPath)
      (const CMPIInstance* inst, CMPIStatus* rc);
   CMPIStatus (*setPropertyFilter)
      ( CMPIInstance* inst, const char **propertyList, const char **keys);
   

   CMPIData (*getQualifier)
      (CMPIInstance* inst, const char *name, CMPIStatus* rc);
   CMPIData (*getQualifierAt)
      (CMPIInstance* inst, unsigned int index, CMPIString** name,
       CMPIStatus* rc);
   unsigned int (*getQualifierCount)
      (CMPIInstance* inst, CMPIStatus* rc);
   
   
   CMPIData (*getPropertyQualifier)
      (CMPIInstance* inst, const char *pname, const char *qname, CMPIStatus* rc);
   CMPIData (*getPropertyQualifierAt)
      (CMPIInstance* inst, const char *pname, unsigned int index, CMPIString** name,
       CMPIStatus* rc);
   unsigned int (*getPropertyQualifierCount)
      (CMPIInstance* inst, const char *pname, CMPIStatus* rc);
   
} iftLocal = { 
   NATIVE_FT_VERSION,
   __ift_release,
   __ift_clone,
   __ift_getProperty,
   __ift_getPropertyAt,
   __ift_getPropertyCount,
   __ift_setProperty,
   __ift_getObjectPath,
   __ift_setPropertyFilter,
   __ift_getQualifier,
   __ift_getQualifierAt,
   __ift_getQualifierCount,    
   __ift_getPropertyQualifier,
   __ift_getPropertyQualifierAt,
   __ift_getPropertyQualifierCount
};

CMPIInstanceFT *CMPI_Instance_FT = &ift;

unsigned long getInstanceSerializedSize(const CMPIInstance * ci)
{
   ClInstance *cli = (ClInstance *) ci->hdl;
   return ClSizeInstance(cli) + sizeof(struct native_instance);
}

void getSerializedInstance(const CMPIInstance * ci, void *area)
{
   memcpy(area, ci, sizeof(struct native_instance));
   ClInstanceRebuild((ClInstance *) ci->hdl,
                     (void *) ((char *) area + sizeof(struct native_instance)));
   ((CMPIInstance *)(area))->hdl = (ClInstance *) ((char *) area + sizeof(struct native_instance));
}

CMPIInstance *relocateSerializedInstance(void *area)
{
   struct native_instance *ci = (struct native_instance *) area;
   ci->instance.hdl = ci + 1;
   ci->instance.ft = CMPI_Instance_FT;
   ci->mem_state=MEM_RELEASED;
   ci->property_list = NULL;
   ci->key_list = NULL;
   ClInstanceRelocateInstance((ClInstance *) ci->instance.hdl);
   return (CMPIInstance *) ci;
}

MsgSegment setInstanceMsgSegment(CMPIInstance * ci)
{
   MsgSegment s;
   s.data = ci;
   s.type = MSG_SEG_INSTANCE;
   s.length = getInstanceSerializedSize(ci);
   return s;
}

CMPIInstance *internal_new_CMPIInstance(int mode, const CMPIObjectPath * cop,
                                        CMPIStatus * rc, int override)
{
   CMPIInstance i = {
      "CMPIInstance",
      CMPI_Instance_FT
   };

   struct native_instance instance,*tInst;
   memset(&instance, 0, sizeof(instance));

   CMPIStatus tmp1, tmp2, tmp3;
   CMPIString *str;
   const char *ns, *cn;
   int j,state;

   instance.instance = i;

   if (cop) {
      j = CMGetKeyCount(cop, &tmp1);
   str = CMGetClassName(cop, &tmp2);
   cn = CMGetCharsPtr(str, NULL);
   str = CMGetNameSpace(cop, &tmp3);
   ns = CMGetCharsPtr(str, NULL);
   }
   
   else {
      j=0;
      //SFCB_ASM("int $3");
      ns = "*NoNameSpace*";
      cn = "*NoClassName*";
      tmp1.rc=tmp2.rc=tmp3.rc=CMPI_RC_OK;
   }   

   if (tmp1.rc != CMPI_RC_OK || tmp2.rc != CMPI_RC_OK || tmp3.rc != CMPI_RC_OK) {
      if (rc) CMSetStatus(rc, CMPI_RC_ERR_FAILED);
   }
   else {
      instance.instance.hdl = ClInstanceNew(ns, cn);

      while (j-- && (tmp1.rc == CMPI_RC_OK)) {
         CMPIString *keyName;
         CMPIData tmp = CMGetKeyAt(cop, j, &keyName, &tmp1);
         __ift_setProperty(&instance.instance, CMGetCharsPtr(keyName, NULL),
                           &tmp.value, tmp.type);
      }
      if (rc) CMSetStatus(rc, tmp1.rc);
   }

   tInst=memAddEncObj(mode, &instance, sizeof(instance),&state);
   tInst->mem_state=state;
   tInst->refCount=0;
   
#ifdef HAVE_DEFAULT_PROPERTIES
   if(!override) {
      instFillDefaultProperties(tInst,ns,cn);
   }
#endif

   return (CMPIInstance*)tInst;
}

CMPIInstance *TrackedCMPIInstance(const CMPIObjectPath * cop, CMPIStatus * rc)
{
   return internal_new_CMPIInstance(MEM_TRACKED, cop, rc, 0);
}

CMPIInstance *NewCMPIInstance(CMPIObjectPath * cop, CMPIStatus * rc)
{
   return internal_new_CMPIInstance(MEM_NOT_TRACKED, cop, rc, 0);
}


static void dataToString(CMPIData d, UtilStringBuffer * sb)
{
   char str[256];
   char *sp = str;

   if (d.type & CMPI_ARRAY) {
      sb->ft->appendChars(sb, "[]");
      return;
   }
   if (d.type & CMPI_UINT) {
      unsigned long long ul = 0LL;
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
      sprintf(str, "%llu", ul);
   }
   else if (d.type & CMPI_SINT) {
      long long sl = 0LL;
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
      sprintf(str, "%lld", sl);
   }
   else if (d.type == CMPI_string) {
      sp = (char *) d.value.string->hdl;
   }
   sb->ft->appendChars(sb, sp);
}

UtilStringBuffer *instanceToString(CMPIInstance * ci, char **props)
{
   unsigned int i, m;
   CMPIData data;
   CMPIString *name;
   UtilStringBuffer *sb = UtilFactory->newStrinBuffer(64);

   for (i = 0, m = CMGetPropertyCount(ci, NULL); i < m; i++) {
      data = CMGetPropertyAt(ci, i, &name, NULL);
      sb->ft->appendChars(sb, (char*)name->hdl);
      SFCB_APPENDCHARS_BLOCK(sb, "=");
      dataToString(data, sb);
      SFCB_APPENDCHARS_BLOCK(sb, "\n");
   }
   return sb;
}

const char *instGetClassName(CMPIInstance * ci)
{
   ClInstance *inst = (ClInstance *) ci->hdl;
   return ClInstanceGetClassName(inst);
}

const char *instGetNameSpace(CMPIInstance * ci)
{
   ClInstance *inst = (ClInstance *) ci->hdl;
   return ClInstanceGetNameSpace(inst);
}

int instanceCompare(CMPIInstance *inst1, CMPIInstance *inst2)
{
   int c,i;
   CMPIStatus st = { CMPI_RC_OK, NULL };
   CMPIData d1, d2;
   CMPIString *propName;

   /* check if we have null pointers for our instances */
   if(inst1 == NULL && inst2 == NULL) return 0; /*identical*/
   if(inst1 == NULL) return -1; /* inst1 is less than inst2 */
   if(inst2 == NULL) return 1;  /* inst2 is less than inst1 */
   
   c = inst1->ft->getPropertyCount(inst1, NULL);
   
   if(c != inst2->ft->getPropertyCount(inst2, NULL)) {
      /* property count not equal, instances cannot be identical */
      return 1;
   }
   
   for(i = 0; i < c; i++) {
      /* get property at position i, store the name in propName */
      d1 = inst1->ft->getPropertyAt(inst1, i, &propName, NULL);
      /* try to find the property in instance2 via the name just retrieved */
      d2 = inst2->ft->getProperty(inst2, propName->ft->getCharPtr(propName, NULL), &st);
      
      if(st.rc ||     /* property could not be retrieved, probably non existent*/
         d1.type != d2.type ||                                 /* types differ */
         sfcb_comp_CMPIValue(&d1.value, &d2.value, d1.type)) { /* values differ*/
         
         /* instances not equal */
         return 1;
      }
   }
   /* passed all tests - instances seem to be identical */
   return 0;
}

/* needed for local client support as function tables differ */
void setInstanceLocalMode(int mode)
{
   if (mode) {
      CMPI_Instance_FT = (CMPIInstanceFT*)&iftLocal;
   } else {
      CMPI_Instance_FT = &ift;
   }
}

#ifdef HAVE_DEFAULT_PROPERTIES 
static void instFillDefaultProperties(struct native_instance *inst, 
				      const char * ns, const char * cn)
{
   static CMPI_MUTEX_TYPE * mtx = NULL;
   static UtilHashTable *clt = NULL;
   CMPIConstClass *cc;
   CMPICount       pc;
   CMPIData        pd;
   CMPIStatus      ps;
   CMPIString     *pn = NULL;
   CMPIValue      *vp;
   
   if (mtx == NULL) {
      mtx = malloc(sizeof(CMPI_MUTEX_TYPE));
      *mtx = Broker->xft->newMutex(0); 
   }
   Broker->xft->lockMutex(*mtx);
   if (clt == NULL) clt = 
      UtilFactory->newHashTable(61,
				UtilHashTable_charKey | UtilHashTable_ignoreKeyCase);
   
   if ((cc = clt->ft->get(clt, cn)) == NULL) {	 
      cc = getConstClass(ns,cn);
      if (cc) {
	 clt->ft->put(clt, strdup(cn), cc->ft->clone(cc,NULL));
      }
   }
   Broker->xft->unlockMutex(*mtx);
   if (cc) {
      pc = cc->ft->getPropertyCount(cc,NULL);
      while (pc > 0) {
	 pc -= 1;
	 pd = cc->ft->getPropertyAt(cc,pc,&pn,&ps);
	 if (ps.rc == CMPI_RC_OK && pn ) {
	    vp = &pd.value;
	    if (pd.state & CMPI_nullValue) {
	       /* must set  null value indication: 
		  CMPI doesn't allow to do that properly */
	       pd.value.chars = NULL;
	       if ((pd.type & (CMPI_SIMPLE|CMPI_REAL|CMPI_INTEGER)) && 
		   (pd.type & CMPI_ARRAY) == 0 ) {
		  vp = NULL;
	       }
	    }
	    __ift_setProperty(&inst->instance,CMGetCharsPtr(pn,NULL),
			      vp,pd.type);
	 }
      }
   }
}
#endif

/****************************************************************************/

/*** Local Variables:  ***/
/*** mode: C           ***/
/*** c-basic-offset: 3 ***/
/*** End:              ***/
