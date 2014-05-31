
/*
 * objectpath.c
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
 * CMPIObjectPath implementation.
 *
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "constClass.h"
#include "objectImpl.h"

#include "native.h"
#include "msgqueue.h"
#include "utilft.h"

extern UtilStringBuffer *newStringBuffer(int s);

extern CMPIArray *native_make_CMPIArray(CMPIData * av, CMPIStatus * rc,ClObjectHdr * hdr);
extern CMPIObjectPath *interal_new_CMPIObjectPath(int mode, const char *,
                                                  const char *, CMPIStatus *);
extern CMPIBroker *Broker;

CMPIObjectPath *getObjectPath(char *path, char **msg);

struct native_cop {
   CMPIObjectPath cop;
   int refCount;
   int mem_state;
};


static struct native_cop *__new_empty_cop(int, CMPIStatus *);

void memLinkObjectPath(CMPIObjectPath *cop)
{
   struct native_cop *o = (struct native_cop *) cop;
   memLinkEncObj(o,&o->mem_state);
}

/****************************************************************************/


static CMPIStatus __oft_release(CMPIObjectPath * cop)
{
   struct native_cop *o = (struct native_cop *) cop;

//   printf("__oft_release %d %d %p\n",getpid(),o->mem_state,cop);
   if (o->mem_state && o->mem_state != MEM_RELEASED) {
      ClObjectPathFree((ClObjectPath *) cop->hdl);
      memUnlinkEncObj(o->mem_state);
      o->mem_state = MEM_RELEASED;
      free(cop);
      CMReturn(CMPI_RC_OK);
   }

   CMReturn(CMPI_RC_ERR_FAILED);
}


static CMPIObjectPath *__oft_clone(const CMPIObjectPath * op, CMPIStatus * rc)
{
   CMPIStatus tmp;
   struct native_cop *ncop = __new_empty_cop(MEM_NOT_TRACKED, &tmp);
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   ncop->cop.hdl = ClObjectPathRebuild(cop, NULL);

   return (CMPIObjectPath *) ncop;
}


static CMPIStatus __oft_setNameSpace(CMPIObjectPath * op, const char *nameSpace)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   ClObjectPathSetNameSpace(cop, nameSpace);
   CMReturn(CMPI_RC_OK);
}

CMPIString *__oft_getNameSpace(const CMPIObjectPath * op, CMPIStatus * rc)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   return sfcb_native_new_CMPIString(ClObjectPathGetNameSpace(cop), rc, 0);
}


static CMPIStatus __oft_setHostName(CMPIObjectPath * op, const char *hostName)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   ClObjectPathSetHostName(cop, hostName);
   CMReturn(CMPI_RC_OK);
}

static CMPIString *__oft_getHostName(const CMPIObjectPath * op, CMPIStatus * rc)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   return sfcb_native_new_CMPIString(ClObjectPathGetHostName(cop), rc, 0);
}



static CMPIStatus __oft_setClassName(CMPIObjectPath * op, const char *className)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   ClObjectPathSetClassName(cop, className);
   CMReturn(CMPI_RC_OK);
}

static CMPIString *__oft_getClassName(const CMPIObjectPath * op, CMPIStatus * rc)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   return sfcb_native_new_CMPIString(ClObjectPathGetClassName(cop), rc, 0);
}

const char *opGetClassNameChars(const CMPIObjectPath * op)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   return ClObjectPathGetClassName(cop);
}

const char *opGetNameSpaceChars(const CMPIObjectPath * op)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   return ClObjectPathGetNameSpace(cop);
}


CMPIData opGetKeyCharsAt(const CMPIObjectPath * op,
                         unsigned int i, char **name, CMPIStatus * rc)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   CMPIData rv = { 0, CMPI_notFound, {0} };

   if (ClObjectPathGetKeyAt(cop, i, &rv, name ? name : NULL)) {
      if (rc)
         CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
      return rv;
   }
 
   if (rv.type == CMPI_chars) {
     rv.value.string = sfcb_native_new_CMPIString(rv.value.chars, NULL, 0);
      rv.type = CMPI_string;
   }
   else if (rv.type == CMPI_ref) {
      char *msg="";
      rv.value.ref = getObjectPath(
         (char*)ClObjectGetClString(&cop->hdr, (ClString *) & rv.value.chars), &msg);
   }
   else if (rv.type & CMPI_ARRAY && rv.value.array) {     // should nor occcur
      rv.value.array =
          native_make_CMPIArray((CMPIData *) rv.value.array, NULL,&cop->hdr);
   }

   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return rv;
}


static CMPIData __oft_getKeyAt(const CMPIObjectPath * op, CMPICount i, CMPIString ** name,
                        CMPIStatus * rc)
{
   char *n;
   CMPIData rv = opGetKeyCharsAt(op, i, &n, rc);

   if (name)
     *name = sfcb_native_new_CMPIString(n, NULL, 0);

   return rv; 
}

static CMPIData __oft_getKey(const CMPIObjectPath * op, const char *id, CMPIStatus * rc)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   ClSection *prps = &cop->properties;
   CMPIData rv = { 0, CMPI_notFound, {0} };
   int i;

   if ((i = ClObjectLocateProperty(&cop->hdr, prps, id)) != 0) {
      return __oft_getKeyAt(op, i - 1, NULL, rc);
   }

   if (rc)
      CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
   return rv;
}

static CMPICount __oft_getKeyCount(const CMPIObjectPath * op, CMPIStatus * rc)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   if (rc)
      CMSetStatus(rc, CMPI_RC_OK);
   return (CMPICount) ClObjectPathGetKeyCount(cop);
}

static CMPIStatus __oft_addKey(CMPIObjectPath * op,
                               const char *name,
                               const CMPIValue * value, CMPIType type)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   CMPIData data = { type, CMPI_goodValue, {0LL} };

   if (type == CMPI_chars) {
      data.value.chars = (char*) value;
   } else if (type == CMPI_string) {
     if (value && value->string) {
       data.value.chars = (char *) value->string->hdl;
     } else {
       data.value.chars = NULL;
     }
     data.type=CMPI_chars;
   } else if (value) {
     sfcb_setAlignedValue(&data.value,value,type);
   }

   if (((type & CMPI_ENCA) && data.value.chars == NULL) || value == NULL) {
     data.state=CMPI_nullValue;
   }

   ClObjectPathAddKey(cop, name, data);

   CMReturn(CMPI_RC_OK);
}

static CMPIStatus __oft_setHostAndNameSpaceFromObjectPath(CMPIObjectPath * op,
							  const CMPIObjectPath * src)
{
   ClObjectPath *s = (ClObjectPath *) src->hdl;
   CMPIStatus   st = {CMPI_RC_OK, NULL};
   st = __oft_setHostName(op, ClObjectPathGetHostName(s));
   if (st.rc == CMPI_RC_OK) {
     st = __oft_setNameSpace(op, ClObjectPathGetNameSpace(s));
   }
   return st;
}

static CMPIStatus __oft_setNameSpaceFromObjectPath(CMPIObjectPath * op,
                                                   const CMPIObjectPath * src)
{
   ClObjectPath *s = (ClObjectPath *) src->hdl;
   return __oft_setNameSpace(op, ClObjectPathGetNameSpace(s));
}

extern char *sfcb_value2Chars(CMPIType type, CMPIValue * value);

char *sfcb_pathToChars(const CMPIObjectPath * cop, CMPIStatus * rc, char *str)
{
//            "//atp:9999/root/cimv25:TennisPlayer.first="Patrick",last="Rafter";

   CMPIString *ns;
   CMPIString *cn;
   CMPIString *hn;
   CMPIString *name;
   CMPIData data;
   unsigned int i, m;
   char *v;
   *str = 0;

   hn = cop->ft->getHostname(cop, rc);
   ns = cop->ft->getNameSpace(cop, rc);
   cn = cop->ft->getClassName(cop, rc);
   if (ns && ns->hdl && *(char*)ns->hdl) {
      strcat(str,(char*)ns->hdl);
      strcat(str,":");
   }   
   strcat(str, (char *) cn->hdl);
   for (i = 0, m = cop->ft->getKeyCount(cop, rc); i < m; i++) {
      data = cop->ft->getKeyAt(cop, i, &name, rc);
      if (i)
         strcat(str, ",");
      else
         strcat(str, ".");
      strcat(str, (char *) name->hdl);
      if (name) CMRelease(name);
      strcat(str, "=");
      v = sfcb_value2Chars(data.type, &data.value);
      if (data.type==CMPI_string) CMRelease(data.value.string);
      strcat(str, v);
      free(v);
   };
   if (ns) CMRelease(ns);
   if (cn) CMRelease(cn);
   if (hn) CMRelease(hn);
   return str;
}

static CMPIString *__oft_toString(const CMPIObjectPath * cop, CMPIStatus * rc)
{
   char str[4096] = { 0 };
   sfcb_pathToChars(cop, rc, str);
   return sfcb_native_new_CMPIString(str, rc, 0);
}

char *oft_toCharsNormalized(const CMPIObjectPath * cop, CMPIConstClass * cls,
                            int full, CMPIStatus * rc)
{
   char str[2048] = { 0 };
   CMPIString *cn;
   CMPIString *name;
   CMPIData data;
   CMPIStatus irc;
   unsigned long quals;
   unsigned int i, n, m;
   char *v;

   cn = __oft_getClassName(cop, rc);
   strcat(str, (char *) cn->hdl);

   for (n = 0, i = 0, m = cls->ft->getPropertyCount(cls, rc); i < m; i++) {
      getPropertyQualsAt(cls, i, &name, &quals, NULL, NULL);
      if (quals & 1) {
         data = __oft_getKey(cop, (const char *) name->hdl, &irc);
         if (irc.rc == CMPI_RC_OK) {
            if (n) strcat(str, ",");
            else strcat(str, ".");
            strcat(str, (char *) name->hdl);
            strcat(str, "=");
            v = sfcb_value2Chars(data.type, &data.value);
            strcat(str, v);
            free(v);
         }
      }
   }
   return strdup(str);
}

static CMPIObjectPathFT oft = {
   NATIVE_FT_VERSION,
   __oft_release,
   __oft_clone,
   __oft_setNameSpace,
   __oft_getNameSpace,
   __oft_setHostName,
   __oft_getHostName,
   __oft_setClassName,
   __oft_getClassName,
   __oft_addKey,
   __oft_getKey,
   __oft_getKeyAt,
   __oft_getKeyCount,
   __oft_setNameSpaceFromObjectPath,
   __oft_setHostAndNameSpaceFromObjectPath,
   NULL,
   NULL,
   NULL,
   NULL,
    __oft_toString
};

CMPIObjectPathFT *CMPI_ObjectPath_FT = &oft;

unsigned long getObjectPathSerializedSize(const CMPIObjectPath * op)
{
   ClObjectPath *cop = (ClObjectPath *) op->hdl;
   return ClSizeObjectPath(cop) + sizeof(struct native_cop);
}

void getSerializedObjectPath(const CMPIObjectPath * op, void *area)
{
   memcpy(area, op, sizeof(struct native_cop));
   ClObjectPathRebuild((ClObjectPath *) op->hdl,
                       (void *) ((char *) area + sizeof(struct native_cop)));
}

CMPIObjectPath *relocateSerializedObjectPath(void *area)
{
   struct native_cop *cop = (struct native_cop *) area;
   cop->cop.hdl = cop + 1;
   cop->mem_state=MEM_RELEASED;
   cop->cop.ft = &oft;
   ClObjectPathRelocateObjectPath((ClObjectPath *) cop->cop.hdl);
   return (CMPIObjectPath *) cop;
}

MsgSegment setObjectPathMsgSegment(const CMPIObjectPath * op)
{
   MsgSegment s;
   s.data = (CMPIObjectPath*)op;
   s.type = MSG_SEG_OBJECTPATH;
   s.length = getObjectPathSerializedSize(op);
   return s;
}

static struct native_cop *__new_empty_cop(int mm_add, CMPIStatus * rc)
{
   static CMPIObjectPath o = {
      "CMPIObjectPath",
      &oft
   };
   struct native_cop cop,*tCop;
   int state;

   cop.cop = o;
   tCop=memAddEncObj(mm_add, &cop, sizeof(cop),&state);
   tCop->mem_state = state;
   tCop->refCount=0;
   if (rc) CMSetStatus(rc, CMPI_RC_OK);

   return (struct native_cop*)tCop;
}



CMPIObjectPath *internal_new_CMPIObjectPath(int mode, const char *nameSpace,
                                            const char *className,
                                            CMPIStatus * rc)
{
   struct native_cop *cop = __new_empty_cop(mode, rc);
   cop->cop.hdl = ClObjectPathNew(nameSpace, className);
   return (CMPIObjectPath *) cop;
}

CMPIObjectPath *TrackedCMPIObjectPath(const char *nameSpace,
                                      const char *className, CMPIStatus * rc)
{
   return internal_new_CMPIObjectPath(MEM_TRACKED, nameSpace, className, rc);
}

CMPIObjectPath *NewCMPIObjectPath(const char *nameSpace, const char *className,
                                  CMPIStatus * rc)
{
   return internal_new_CMPIObjectPath(MEM_NOT_TRACKED, nameSpace, className, rc);
}

static int refLookAhead(char *u, char **nu)
{
   int state = 0, i;
   char *pu = NULL;
   for (i = 0; u[i] != 0; i++) {
      switch (state) {
      case 0: /* start */
         if (isalnum(u[i]))
            state = 1;
         break;
      case 1: /* lhs: key property name */
         if (u[i] == '=')
            state = 2;
         break;
      case 2: /* rhs: value */
         if (isalnum(u[i]))
            state = 3;
         else
            return 0;
         break;
      case 3: /* rhs: some identifier or value */
         if (u[i] == ',')
            return 0;
         if (u[i] == '.') {
            state = 4;
         }
         break;
      case 4: /* rhs: start of key property of an ref value */
         if (!isalnum(u[i]))
            return 0;
	 state = 5;
	 break;
      case 5: /* rhs: key property of an ref value */
	 if (u[i] == '=') {
  	   state = 6;
	   if (u[i+1]==0) {
	      *nu = u + i;
	      return 1;
	   }
	 }
	 break;
      case 6: /* rhs: key property of a ref value's key property */
	 if (u[i] == ',') {
	    if (refLookAhead(u+i,&pu)) {
  	       *nu = u + i;
	       return 1;
	    }
	 }
	 break;
      }
   }
   if (state>4) {
      *nu = u + i;
      return 1;
   } else {
      return 0;
   }
}


static char *strnDup(const char *n, int l)
{
   char *tmp = (char *) malloc(l + 2);
   strncpy(tmp, n, l);
   tmp[l] = 0;
   return tmp;
}

static void addKey(CMPIObjectPath * op, char *kd, int ref)
{
   char *val = strchr(kd, '=');
   *val = 0;
   val++;
   if (ref) {
     char * msg;
     CMPIObjectPath *keyOp = getObjectPath(val,&msg);
     op->ft->addKey(op, kd, (CMPIValue*)&keyOp, CMPI_ref);
   } else if (*val == '"') {
      val++;
      val[strlen(val) - 1] = 0;
      op->ft->addKey(op, kd, (CMPIValue *) val, CMPI_chars);
   }
   else if (isdigit(*val)) {
   }
   else {
   }
}

CMPIObjectPath *getObjectPath(char *path, char **msg)
{
   char *p, *pp, *last, *un, *cname, *nname=NULL;
   char *origu, *u;
   int ref = 0;
   CMPIObjectPath *op;

   if (path == NULL) {
     return NULL;
   }
   u = origu = strdup(path);
   last = u + strlen(u);
   *msg = NULL;

   p = strchr(u, '.');
   if (!p) {
      if (u) { 
         if ((pp=strchr(u,':'))!=NULL) {
            nname=strnDup(u,pp-u);
            u=pp+1;
         }      
         cname = strdup(u);
//         op = Broker->eft->newObjectPath(Broker, nname ? nname : "root/cimv2", cname, NULL);
         op = Broker->eft->newObjectPath(Broker, nname , cname, NULL);
         free(cname);
         free(origu);
         if (nname) free(nname);
         return op;
      }
      *msg = "No className found";
      free(origu);
      if (nname) free(nname);
      return NULL;
   }
   
   if ((pp=strchr(u,':'))!=NULL) {
       nname=strnDup(u,pp-u);
       u=pp+1;
   }  
     
   cname = strnDup(u, p - u);
//   op = Broker->eft->newObjectPath(Broker, nname ? nname : "root/cimv2", cname, NULL);
   op = Broker->eft->newObjectPath(Broker, nname , cname, NULL);
   free(cname);
   if (nname) free(nname);

   for (u = p + 1; ; u = p + 1) {
      if ((ref = refLookAhead(u, &un))) {
	char *t; 
        p = un;
	if (*p == 0) {
	  break;
	}
	t = strnDup(u, un - u);
	addKey(op, t, ref);
	free(t);
	continue;
      }
      if ((p = strpbrk(u, ",\"")) == NULL)
         break;
      if (*p == '"') {
         if (*(p - 1) != '=') {
            *msg = "Incorrectly quoted string 1";
            free(origu);
            return NULL;
         }
         p++;
         if ((p = strchr(p, '"')) == NULL) {
            *msg = "Unbalanced quoted string";
            free(origu);
            return NULL;
         }
         p++;
         if (*p != ',' && *p != 0) {
            *msg = "Incorrectly quoted string 2";
            free(origu);
            return NULL;
         }
         if (*p == 0)
            break;
      }
      char *t = strnDup(u, p - u);
      addKey(op, t, ref);
      free(t);
   }
   if (last > u) {
      char *t = strnDup(u, last - u);
      addKey(op, t, ref);
      free(t);
   }
   free(origu);
   return op;
}

typedef struct keyIds {
   CMPIString *key;
   CMPIData data;
} KeyIds;

static int qCompare(const void *arg1, const void *arg2)
{
   return strcasecmp((char *) ((KeyIds *) arg1)->key->hdl,
                     (char *) ((KeyIds *) arg2)->key->hdl);
}

static char copKey[8192];

UtilStringBuffer *normalizeObjectPathStrBuf(const CMPIObjectPath * cop)
{
   int c = CMGetKeyCount(cop, NULL);
   int i;
   char pc = 0,*cp;
   UtilStringBuffer *sb=newStringBuffer(512);

   KeyIds *ids = (KeyIds *) malloc(sizeof(KeyIds) * c);
   
   for (i = 0; i < c; i++) {
      ids[i].data = CMGetKeyAt(cop, i, &ids[i].key, NULL);
      cp=ids[i].key->hdl;
      while (*cp) {
         *cp=tolower(*cp);
         cp++; 
      }
   }
   qsort(ids, c, sizeof(KeyIds), qCompare);

   for (i = 0; i < c; i++) {
      if (pc) SFCB_APPENDCHARS_BLOCK(sb,",");
      sb->ft->appendChars(sb,(char*)ids[i].key->hdl);
      SFCB_APPENDCHARS_BLOCK(sb,"=");
      if (ids[i].data.type==CMPI_ref) {
         CMPIString *cn=CMGetClassName(ids[i].data.value.ref,NULL);
	 CMPIString *ns=CMGetNameSpace(ids[i].data.value.ref,NULL);
         UtilStringBuffer *sbt= normalizeObjectPathStrBuf(ids[i].data.value.ref);
	 char *nss;
         cp=(char*)cn->hdl;
         while (*cp) {
            *cp=tolower(*cp);
            cp++; 
         }
	 if (ns==NULL) {
	   nss = CMGetCharPtr(CMGetNameSpace(cop,NULL));
	 } else {
	   nss = CMGetCharPtr(ns);
	 }
	 if (nss) {
	   sb->ft->appendChars(sb,nss);
	   SFCB_APPENDCHARS_BLOCK(sb,":");
	 }
         sb->ft->appendChars(sb,(char*)cn->hdl);
         SFCB_APPENDCHARS_BLOCK(sb,".");
         sb->ft->appendChars(sb,sbt->ft->getCharPtr(sbt));
         sbt->ft->release(sbt);
      }
      else {
         char *v = sfcb_value2Chars(ids[i].data.type, &ids[i].data.value);
         sb->ft->appendChars(sb,v);
         free(v);
      }   
      pc = ',';
   }
   free(ids);
   

   return(sb);
}

char *normalizeObjectPathChars(const CMPIObjectPath *cop)
{
   UtilStringBuffer *sb=normalizeObjectPathStrBuf(cop);  
   strcpy(copKey,sb->ft->getCharPtr(sb));
   sb->ft->release(sb);
   return copKey;
}

char *normalizeObjectPathCharsDup(const CMPIObjectPath *cop)
{
   char *n;
   UtilStringBuffer *sb=normalizeObjectPathStrBuf(cop);  
   n=strdup(sb->ft->getCharPtr(sb));
   sb->ft->release(sb);
   return n;
}

int objectpathCompare(const CMPIObjectPath *cop1, const CMPIObjectPath *cop2)
{
   UtilStringBuffer *sb1, *sb2;
   int result;
   
   sb1=normalizeObjectPathStrBuf(cop1);
   sb2=normalizeObjectPathStrBuf(cop2);

   result = strcmp(sb1->ft->getCharPtr(sb1), sb2->ft->getCharPtr(sb2));
   
   sb1->ft->release(sb1);
   sb2->ft->release(sb2);
   
   return result;
}
