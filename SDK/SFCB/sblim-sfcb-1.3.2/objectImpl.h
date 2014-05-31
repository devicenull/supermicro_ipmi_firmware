
/*
 * objectimpl.h
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
 * Author:     Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * Internal implementation support for cim objects.
 *
*/

#ifdef SETCLPFX
 #ifdef CLOBJECTS_H
 #undef CLOBJECTS_H
 #endif
#endif

#ifndef CLOBJECTS_H
#define CLOBJECTS_H

#ifndef SETCLPFX

 #include "cmpidt.h"
 #include "cmpift.h"
 #include "cmpimacs.h"

 #include "native.h"

 #define SFCB_LOCAL_ENDIAN       0
 #define SFCB_LITTLE_ENDIAN      1
 #define SFCB_BIG_ENDIAN         2

 #define ClCurrentVersion 1
 #define ClCurrentLevel 0
 #define ClTypeClassRep 1

 #define ClCurrentObjImplLevel 3

 #define GetLo15b(x) (x&0x7fff)
 #define GetHi1b(x)  (x&0x8000)

 #define GetMax(f)   (GetLo15b((f)))
 #define IsMallocedMax(x) (GetHi1b((x)))

 #define CLALIGN 4
 #define CLEXTRA 0
 
 #define ALIGN(x,y) (x == 0 ? 0 : ((((x-1)/y)+1)*y))

#endif


// The PFX macro enables one to add a prefix to the Cl structures in this header file
// This will be used to have multiple HW platform specific alignments in one compile.
// An example of its usage is in ObjectImplSwapI32toP32.c
// PFX prepends the string indicated by the the CLPFX preprocesor #define to the Cl structure names


#ifdef SETCLPFX
 #ifdef CLPFX
   #undef CLPFX
 #endif
 #define CLPFX SETCLPFX
#else
 #define CLPFX
#endif

#define _XPFX(p,x) p ## x
#define PFX(p,x) _XPFX(p,x) 

typedef struct {
   union {
      struct {
         union {
            unsigned int size;  // used to determine endianes - byter order in designated host order
            unsigned char sByte[4];
         };
         unsigned short zeros;   // all remaining integer fields in network order 
         unsigned short type;
         char id[10];            // "sfcb-rep\0" used to determine asci/ebcdic char encoding  
         unsigned short version;
         unsigned short level;
         unsigned short objImplLevel;
         unsigned short options;
         unsigned short flags;
         char creationDate[32];
      };
      struct {                   // used to force 96 bytes record length
         unsigned int fixedSize[23];  
         unsigned int lastInt;
      };
   };   
}  PFX(CLPFX,ClVersionRecord);   


typedef struct {
   char *str;
   unsigned int used, max;
} PFX(CLPFX,stringControl);

typedef struct {
   union {
      long sectionOffset;
      void *sectionPtr;
   };
   unsigned short used, max;
} PFX(CLPFX,ClSection);

typedef struct {
   unsigned short iUsed,iMax;
   int indexOffset;
   int *indexPtr;
   unsigned int bUsed, bMax;
   char buf[1];
} PFX(CLPFX,ClStrBuf);

#ifndef CLP32   // different layout for power 32
typedef struct {
   unsigned short iUsed,iMax;
   int indexOffset;
   int *indexPtr;
   unsigned int bUsed, bMax;
   PFX(CLPFX,CMPIData) buf[1];
} PFX(CLPFX,ClArrayBuf);
#endif

typedef struct {
   unsigned int size;
   unsigned short flags;
   #ifndef SETCLPFX
    #define HDR_Rebuild 1
    #define HDR_RebuildStrings 2
    #define HDR_ContainsEmbeddedObject 4
    #define HDR_StrBufferMalloced 16
    #define HDR_ArrayBufferMalloced 32 
   #endif
   unsigned short type;
   #ifndef SETCLPFX
    #define HDR_Class 1
    #define HDR_Instance 2
    #define HDR_ObjectPath 3
    #define HDR_Args 4
    #define HDR_Qualifier 5    
    #define HDR_Version 0x1010
   #endif
   union {
      long strBufOffset;
      ClStrBuf *strBuffer;
   };
   union {
      long arrayBufOffset;
      ClArrayBuf *arrayBuffer;
   };   
} PFX(CLPFX,ClObjectHdr);

typedef struct {
   long id;
} PFX(CLPFX,ClString);

typedef struct {
   long id;
} PFX(CLPFX,ClArray);

typedef struct {
   PFX(CLPFX,ClObjectHdr) hdr;
   unsigned char quals;
   #ifndef SETCLPFX
    #define ClClass_Q_Abstract 1
    #define ClClass_Q_Association 2
    #define ClClass_Q_Indication 4
   #endif
   unsigned char parents;
   unsigned short reserved;
   PFX(CLPFX,ClString) name;
   PFX(CLPFX,ClString) parent;
   PFX(CLPFX,ClSection) qualifiers;
   PFX(CLPFX,ClSection) properties;
   PFX(CLPFX,ClSection) methods;
} PFX(CLPFX,ClClass);

typedef struct {
   ClObjectHdr hdr;
   PFX(CLPFX,ClString) hostName;
   PFX(CLPFX,ClString) nameSpace;
   PFX(CLPFX,ClString) className;
   PFX(CLPFX,ClSection) properties;
} PFX(CLPFX,ClObjectPath);

typedef struct {
   PFX(CLPFX,ClObjectHdr) hdr;
   PFX(CLPFX,ClSection) properties;
} PFX(CLPFX,ClArgs);

typedef struct {
   PFX(CLPFX,ClObjectHdr) hdr;
   unsigned char quals;
   #ifndef SETCLPFX
    #define ClInst_Q_Association 2
    #define ClInst_Q_Indication 4
   #endif 
   unsigned char parents;
   unsigned short reserved;
   PFX(CLPFX,ClString) className;
   PFX(CLPFX,ClString) nameSpace;
   PFX(CLPFX,ClSection) qualifiers;
   PFX(CLPFX,ClSection) properties;
   PFX(CLPFX,ClObjectPath) *path;
} PFX(CLPFX,ClInstance);

typedef struct {
   int type;
   long data;
} PFX(CLPFX,ClData);

#ifndef CLP32   // different layout for power 32
typedef struct {
   PFX(CLPFX,ClString) id;
   PFX(CLPFX,CMPIData) data;
} PFX(CLPFX,ClQualifier);
#endif

typedef struct {
   PFX(CLPFX,ClObjectHdr) hdr;
   unsigned char flavor;
   #ifndef SETCLPFX
    #define ClQual_F_Overridable 1
    #define ClQual_F_ToSubclass 2
    #define ClQual_F_ToInstance 4
    #define ClQual_F_Translatable 8        
   #endif 
   unsigned char scope;
   #ifndef SETCLPFX
   	#define ClQual_S_Class 1
   	#define ClQual_S_Association 2
   	#define ClQual_S_Reference 4
   	#define ClQual_S_Property 8
   	#define ClQual_S_Method 16
   	#define ClQual_S_Parameter 32
   	#define ClQual_S_Indication 64   	   	   
   #endif
   PFX(CLPFX,CMPIType) type;
   unsigned int arraySize;
   PFX(CLPFX,ClString) qualifierName;
   PFX(CLPFX,ClString) nameSpace;
   PFX(CLPFX,ClSection) qualifierData;
} PFX(CLPFX,ClQualifierDeclaration);

#ifndef CLP32   // different layout for power 32
typedef struct {
   PFX(CLPFX,CMPIData) data;
   PFX(CLPFX,ClString) id;
   PFX(CLPFX,ClString) refName;
   unsigned short flags;
   #ifndef SETCLPFX
    #define ClProperty_EmbeddedObjectAsString 1
    #define ClProperty_Deleted 2
    #define ClProperty_Filtered 4
   #endif 
   unsigned char quals;
   #ifndef SETCLPFX
    #define ClProperty_Q_Key 1
    #define ClProperty_Q_EmbeddedObject 8
   #endif 
   unsigned char originId;
   PFX(CLPFX,ClSection) qualifiers;
} PFX(CLPFX,ClProperty);
#endif

typedef struct {
   PFX(CLPFX,ClString) id;
   PFX(CLPFX,CMPIType) type;
   unsigned short flags;
   unsigned char quals;
   unsigned char originId;
   PFX(CLPFX,ClSection) qualifiers;
   PFX(CLPFX,ClSection) parameters;
} PFX(CLPFX,ClMethod);

typedef struct {
   PFX(CLPFX,CMPIType) type;
   unsigned int arraySize;
   char *refName;  
} PFX(CLPFX,CMPIParameter);

typedef struct {
   PFX(CLPFX,ClString) id;
   PFX(CLPFX,CMPIParameter) parameter;
   unsigned short quals;
   PFX(CLPFX,ClSection) qualifiers;
} PFX(CLPFX,ClParameter);


#ifndef SETCLPFX

inline static void *getSectionPtr(ClObjectHdr *hdr, ClSection *s)
{
   if (IsMallocedMax(s->max)) return s->sectionPtr;
   return (void*) ((char*) hdr + s->sectionOffset);
}

inline static int isMallocedSection(ClSection *s)
{
   return IsMallocedMax(s->max);
}

inline static void setSectionOffset(ClSection *s, long offs)
{
   s->sectionOffset=offs;
   s->max &= 0x7fff;
}

inline static void* setSectionPtr(ClSection *s, void *ptr)
{
   s->max |= 0x8000;
   return s->sectionPtr=ptr;
}



inline static ClStrBuf *getStrBufPtr(ClObjectHdr *hdr)
{
   if (hdr->flags & HDR_StrBufferMalloced) return hdr->strBuffer;
   return  (ClStrBuf *) ((char *) hdr + hdr->strBufOffset);
}

inline static ClStrBuf *setStrBufPtr(ClObjectHdr *hdr, void *buf)
{
   hdr->flags |= HDR_StrBufferMalloced;
   return  hdr->strBuffer=(ClStrBuf*)buf;
}

inline static void setStrBufOffset(ClObjectHdr *hdr, long offs)
{
   hdr->flags &= ~HDR_StrBufferMalloced;
   hdr->strBufOffset=offs;
}

inline static int *setStrIndexPtr(ClStrBuf *buf, void *idx)
{
   buf->iMax |= 0x8000;
   return buf->indexPtr=(int*)idx;
}

inline static void setStrIndexOffset(ClObjectHdr *hdr, ClStrBuf *buf, long offs)
{
   buf->iMax &= 0x7fff;
   buf->indexPtr=(int*)(((char*)hdr) + offs);
   buf->indexOffset=offs;
}



inline static ClArrayBuf *getArrayBufPtr(ClObjectHdr *hdr)
{
   if (hdr->flags & HDR_ArrayBufferMalloced) return hdr->arrayBuffer;
   else return (ClArrayBuf *) ((char *) hdr + hdr->arrayBufOffset);
}

inline static ClArrayBuf *setArrayBufPtr(ClObjectHdr *hdr, void *buf)
{
   hdr->flags |= HDR_ArrayBufferMalloced;
   return  hdr->arrayBuffer=(ClArrayBuf*)buf;
}

inline static void setArrayBufOffset(ClObjectHdr *hdr, long offs)
{
   hdr->flags &= ~HDR_ArrayBufferMalloced;
   hdr->arrayBufOffset=offs;
}

inline static int *setArrayIndexPtr(ClArrayBuf *buf, void *idx)
{
   buf->iMax |= 0x8000;
   return buf->indexPtr=(int*)idx;
}

inline static void setArrayIndexOffset(ClObjectHdr *hdr, ClArrayBuf *buf, long offs)
{
   buf->iMax &= 0x7fff;
   buf->indexPtr=(int*)(((char*)hdr) + offs);
   buf->indexOffset=offs;
}




inline static int isMallocedStrBuf(ClObjectHdr *hdr)
{
   return hdr->flags & HDR_StrBufferMalloced;
}

inline static int isMallocedStrIndex(ClStrBuf *buf)
{
   return IsMallocedMax(buf->iMax);
}   
   
inline static int isMallocedArrayBuf(ClObjectHdr *hdr)
{
   return hdr->flags & HDR_ArrayBufferMalloced;
}

inline static int isMallocedArrayIndex(ClArrayBuf *buf)
{
   return IsMallocedMax(buf->iMax);
}   
   
/* objectImpl.c */

extern ClVersionRecord ClBuildVersionRecord(unsigned short opt, int endianMmode, long *size);
extern int ClVerifyObjImplLevel(ClVersionRecord* vr);
extern const char *ClObjectGetClString(ClObjectHdr *hdr, ClString *id);
extern const CMPIData *ClObjectGetClArray(ClObjectHdr *hdr, ClArray *id);
extern void *ClObjectGetClSection(ClObjectHdr *hdr, ClSection *s);
extern int ClClassAddQualifierSpecial(ClObjectHdr *hdr, ClSection *qlfs, const char *id, CMPIData d, ClObjectHdr *arrHdr);
extern int ClClassAddQualifier(ClObjectHdr *hdr, ClSection *qlfs, const char *id, CMPIData d);
extern int ClClassAddPropertyQualifierSpecial(ClObjectHdr *hdr, ClProperty *p, const char *id, CMPIData d, ClObjectHdr *arrHdr);
extern int ClClassAddPropertyQualifier(ClObjectHdr *hdr, ClProperty *p, const char *id, CMPIData d);
extern int ClClassAddMethodQualifier(ClObjectHdr *hdr, ClMethod *m, const char *id, CMPIData d);
extern int ClClassAddMethParamQualifier(ClObjectHdr *hdr, ClParameter *p,const char *id, CMPIData d);
extern int ClClassGetQualifierAt(ClClass *cls, int id, CMPIData *data, char **name);
extern int ClClassGetQualifierCount(ClClass *cls);
extern int ClClassGetMethParameterCount(ClClass * cls, int id);
extern int ClClassAddMethParameter(ClObjectHdr *hdr, ClMethod *m, const char *id, CMPIParameter cp);
extern int ClClassLocateMethod(ClObjectHdr *hdr, ClSection *mths, const char *id);
extern int ClClassGetMethQualifierCount(ClClass * cls, int id);
extern int ClClassGetMethParamQualifierCount(ClClass * cls, ClParameter *p);
extern int ClObjectLocateProperty(ClObjectHdr *hdr, ClSection *prps, const char *id);
extern void showClHdr(void *ihdr);
extern unsigned char ClClassAddGrandParent(ClClass *cls, char *gp);
extern ClClass *ClClassNew(const char *cn, const char *pa);
extern unsigned long ClSizeClass(ClClass *cls);
extern ClClass *ClClassRebuildClass(ClClass *cls, void *area);
extern void ClClassRelocateClass(ClClass *cls);
extern void ClClassFreeClass(ClClass *cls);
extern char *ClClassToString(ClClass *cls);
extern int ClClassAddProperty(ClClass *cls, const char *id, CMPIData d, char *refName);
extern int ClClassGetPropertyCount(ClClass *cls);
extern int ClClassGetPropertyAt(ClClass *cls, int id, CMPIData *data, char **name, unsigned long *quals, char **refName);
extern int ClClassGetPropQualifierCount(ClClass *cls, int id);
extern int ClClassGetPropQualifierAt(ClClass *cls, int id, int qid, CMPIData *data, char **name);
extern int ClClassAddMethod(ClClass *cls, const char *id, CMPIType t);
extern int ClClassGetMethodCount(ClClass *cls);
extern int ClClassGetMethodAt(ClClass *cls, int id, CMPIType *data, char **name, unsigned long *quals);
extern int ClClassGetMethQualifierAt(ClClass *cls, ClMethod *m, int qid, CMPIData *data, char **name);
extern int ClClassGetMethParameterAt(ClClass *cls, ClMethod *m, int pid, CMPIParameter *parm, char **name);
extern int ClClassGetMethParamQualifierAt(ClClass * cls, ClParameter *parm, int id, CMPIData *d, char **name);
extern int isInstance(const CMPIInstance *ci);
extern ClInstance *ClInstanceNew(const char *ns, const char *cn);
extern unsigned long ClSizeInstance(ClInstance *inst);
extern ClInstance *ClInstanceRebuild(ClInstance *inst, void *area);
extern void ClInstanceRelocateInstance(ClInstance *inst);
extern void ClInstanceFree(ClInstance *inst);
extern char *ClInstanceToString(ClInstance *inst);
extern int ClInstanceGetPropertyCount(ClInstance *inst);
extern int ClInstanceGetPropertyAt(ClInstance *inst, int id, CMPIData *data, char **name, unsigned long *quals);
extern int ClInstanceAddProperty(ClInstance *inst, const char *id, CMPIData d);
extern void ClInstanceFilterFlagProperty(ClInstance * inst, int id);
extern int ClInstanceIsPropertyAtFiltered(ClInstance * inst, int id);
extern const char *ClInstanceGetClassName(ClInstance *inst);
extern const char *ClInstanceGetNameSpace(ClInstance *inst);
extern const char *ClGetStringData(CMPIInstance *ci, int id);
extern ClObjectPath *ClObjectPathNew(const char *ns, const char *cn);
extern unsigned long ClSizeObjectPath(ClObjectPath *op);
extern ClObjectPath *ClObjectPathRebuild(ClObjectPath *op, void *area);
extern void ClObjectPathRelocateObjectPath(ClObjectPath *op);
extern void ClObjectPathFree(ClObjectPath *op);
extern char *ClObjectPathToString(ClObjectPath *op);
extern int ClObjectPathGetKeyCount(ClObjectPath *op);
extern int ClObjectPathGetKeyAt(ClObjectPath *op, int id, CMPIData *data, char **name);
extern int ClObjectPathAddKey(ClObjectPath *op, const char *id, CMPIData d);
extern void ClObjectPathSetHostName(ClObjectPath *op, const char *hn);
extern const char *ClObjectPathGetHostName(ClObjectPath *op);
extern void ClObjectPathSetNameSpace(ClObjectPath *op, const char *ns);
extern const char *ClObjectPathGetNameSpace(ClObjectPath *op);
extern void ClObjectPathSetClassName(ClObjectPath *op, const char *cn);
extern const char *ClObjectPathGetClassName(ClObjectPath *op);
extern ClArgs *ClArgsNew(void);
extern unsigned long ClSizeArgs(ClArgs *arg);
extern ClArgs *ClArgsRebuild(ClArgs *arg, void *area);
extern void ClArgsRelocateArgs(ClArgs *arg);
extern void ClArgsFree(ClArgs *arg);
extern char *ClArgsToString(ClArgs *arg);
extern int ClArgsGetArgCount(ClArgs *arg);
extern int ClArgsGetArgAt(ClArgs *arg, int id, CMPIData *data, char **name);
extern int ClArgsAddArg(ClArgs *arg, const char *id, CMPIData d);
extern ClQualifierDeclaration *ClQualifierDeclarationNew(const char *ns, const char *name);
extern unsigned long ClSizeQualifierDeclaration(ClQualifierDeclaration * q);
extern ClQualifierDeclaration *ClQualifierRebuildQualifier(ClQualifierDeclaration * q, void *area);
extern void ClQualifierRelocateQualifier(ClQualifierDeclaration * q);
extern int ClQualifierAddQualifier(ClObjectHdr * hdr, ClSection * qlfs, const char *id, CMPIData d);
extern int ClQualifierDeclarationGetQualifierData(ClQualifierDeclaration * q, CMPIData * data);
extern void ClQualifierFree(ClQualifierDeclaration * q);
const char *ClObjectGetClObject(ClObjectHdr * hdr, ClString * id);

#endif // SETCLPFX
   
#endif
