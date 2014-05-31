
/*
 * $Id: objectImplSwapI32toP32.c,v 1.16 2008/01/29 22:38:26 buccella Exp $
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
 * Converts a classSchemas file from 32 bit i386 format to 32 bit power format.
 * This code uses CLP32:* structures that anticipate alignemtn rules for the 32 bit power arch.
 *
*/



#include <byteswap.h>
#include <stdio.h>
#include <stdlib.h>
#include "utilft.h"
#include <sys/utsname.h>

#ifdef SETCLPFX
#undef SETCLPFX
#endif

#include "objectImpl.h"

#define CLP32
#define SETCLPFX CLP32_

#define CLP32_CMPIType CMPIType
#define CLP32_CMPIValueState CMPIValueState
#define CLP32_CMPIValue CMPIValue

typedef struct _CLP32_CMPIData {
   CLP32_CMPIType type;
   CLP32_CMPIValueState state;
   int fillP32;
   CLP32_CMPIValue value;
} CLP32_CMPIData;

#include "objectImpl.h"

typedef struct {
   unsigned short iUsed,iMax;
   int indexOffset;
   int *indexPtr;
   unsigned int bUsed, bMax;
   int fillP32;
   CLP32_CMPIData buf[1];
} CLP32_ClArrayBuf;

typedef struct {
   CLP32_ClString id;
   int fillP32;
   CLP32_CMPIData data;
} CLP32_ClQualifier;


typedef struct {
   CLP32_CMPIData data;
   CLP32_ClString id;
   CLP32_ClString refName;
   unsigned short flags;
   #ifndef SETCLPFX
    #define ClProperty_EmbeddedObjectAsString 1
    #define ClProperty_Deleted 2
   #endif 
   unsigned char quals;
   #ifndef SETCLPFX
    #define ClProperty_Q_Key 1
    #define ClProperty_Q_Propagated 2
    #define ClProperty_Q_EmbeddedObject 8
   #endif 
   unsigned char originId;
   CLP32_ClSection qualifiers;
   long            padP32;
} CLP32_ClProperty;

static CLP32_CMPIData copyI32toP32Data(ClObjectHdr * hdr, CMPIData *fd)
{
   CLP32_CMPIData td;
   
   td.value.sint64=0;
   td.fillP32=0;
   
   if (fd->state & 8) {
      td.value.sint32=bswap_32(fd->value.sint32);
      fd->state &= ~8;
   }

   else switch (fd->type) {
   case CMPI_boolean:
   case CMPI_uint8:
   case CMPI_sint8:
      td.value.sint8=fd->value.sint8;
      break;
   case CMPI_char16:
   case CMPI_uint16:
   case CMPI_sint16:
      td.value.sint16=bswap_16(fd->value.sint16);
      break;
   case CMPI_real32:
   case CMPI_uint32:
   case CMPI_sint32:
      td.value.sint32=bswap_32(fd->value.sint32);
      break;
   case CMPI_real64:
   case CMPI_uint64:
   case CMPI_sint64:
      td.value.sint64=bswap_64(fd->value.sint64);
      break;
   default:
      /*if ((fd->type & (CMPI_ENC | CMPI_ARRAY))==CMPI_ENC) {
         td.value.sint32=bswap_32(fd->value.sint32);
      }   
      else if ((fd->type & (CMPI_ENC | CMPI_ARRAY))==(CMPI_ENC | CMPI_ARRAY)) {
        if (fd->value.array) {
            CMPIData *av=(CMPIData*)ClObjectGetClArray(hdr, (ClArray*)&fd->value.array);
            av->state |= 8;
            td.value.sint32=bswap_32(fd->value.sint32);
         }
      }*/
      if((fd->type & CMPI_ARRAY) || (fd->type & CMPI_ENC)) {
      	td.value.sint32=bswap_32(fd->value.sint32);
      }
   }   

   td.type=bswap_16(fd->type);
   td.state=bswap_16(fd->state);
   return td;
}

static int p32SizeQualifiers(ClObjectHdr * hdr, ClSection * s)
{
   long sz;

   sz = s->used * sizeof(CLP32_ClQualifier);
   return ALIGN(sz,CLALIGN);
}

static int copyI32toP32Qualifiers(int ofs, char *to, CLP32_ClSection * ts,
                          ClObjectHdr * from, ClSection * fs)
{
   ClQualifier *fq = (ClQualifier *) ClObjectGetClSection(from, fs);
   CLP32_ClQualifier *tq = (CLP32_ClQualifier*)(to + ofs);
   int i, l = fs->used * sizeof(CLP32_ClQualifier);
   
   ts->max = bswap_16(fs->max);
   ts->used = bswap_16(fs->used);
   ts->sectionOffset = 0;
   
   if (l == 0) return 0;
   
   tq->fillP32=0;

   for (i = 0; i<fs->used; i++, tq++, fq++) {
      tq->id.id = bswap_32(fq->id.id);
      tq->data = copyI32toP32Data(from, &fq->data);
   }
   
   ts->sectionOffset = bswap_32(ofs);
   
   return ALIGN(l,CLALIGN);
}

static int p32SizeProperties(ClObjectHdr * hdr, ClSection * s)
{
   int l;
   long sz = s->used * sizeof(CLP32_ClProperty);
   ClProperty *p = (ClProperty *) ClObjectGetClSection(hdr, s);

   if (s->used == 0) return 0;
   
   for (l = s->used; l > 0; l--, p++) {
     if (p->qualifiers.used)
         sz += p32SizeQualifiers(hdr, &p->qualifiers);
   }      
   return ALIGN(sz,CLALIGN);
}

static int copyI32toP32Properties(int ofs, char *to, CLP32_ClSection * ts,
                           ClObjectHdr * from, ClSection * fs)
{
   ClProperty *fp = (ClProperty *) ClObjectGetClSection(from, fs);
   CLP32_ClProperty *tp = (CLP32_ClProperty *) (to + ofs);   
   int i, l = fs->used * sizeof(CLP32_ClProperty);
   
   if (l == 0) return 0;
   
   ts->max = bswap_16(fs->max);
   ts->used = bswap_16(fs->used);
   ts->sectionOffset = 0;

   if (l == 0) return 0;
   
   for (i = fs->used; i > 0; i--, fp++, tp++) {
      tp->padP32 = 0L;
      tp->id.id = bswap_32(fp->id.id);
      tp->refName.id = bswap_32(fp->refName.id);
      tp->data = copyI32toP32Data(from, &fp->data);
      tp->flags = bswap_16(fp->flags);     
      tp->quals = fp->quals;     
      tp->originId = fp->originId;
      if (fp->qualifiers.used) {
         l += copyI32toP32Qualifiers(ofs + l, to, &tp->qualifiers, from,
                             &fp->qualifiers);
      }
      else {
      	tp->qualifiers.used=0;
      	tp->qualifiers.max=0;
      	tp->qualifiers.sectionOffset=0;
      }
   }                          

   ts->sectionOffset = bswap_32(ofs);
   return ALIGN(l,CLALIGN);
}

static int p32SizeParameters(ClObjectHdr * hdr, ClSection * s)
{
   int l;
   long sz = s->used * sizeof(CLP32_ClParameter);
   ClParameter *p = (ClParameter *) ClObjectGetClSection(hdr, s);

   if (s->used ==0) return 0;
   
   for (l = s->used; l > 0; l--, p++) {
      if (p->qualifiers.used)
         sz += p32SizeQualifiers(hdr, &p->qualifiers);
   }      
   return ALIGN(sz,CLALIGN);
}

static long copyI32toP32Parameters(int ofs, char *to, CLP32_ClSection * ts,
                           ClObjectHdr * from, ClSection * fs)
{
   ClParameter *fp = (ClParameter *) ClObjectGetClSection(from, fs);
   CLP32_ClParameter *tp = (CLP32_ClParameter *) (to + ofs);
   int i,l = fs->used * sizeof(CLP32_ClParameter);
   
   ts->max = bswap_16(fs->max);
   ts->used = bswap_16(fs->used);
   ts->sectionOffset = 0;

   if (l == 0) return 0;
   
   tp->quals = fp->quals;
   
   for (i = fs->used; i > 0; i--, fp++, tp++) {
      tp->id.id = bswap_32(fp->id.id);
      tp->quals = bswap_16(fp->quals);
      tp->parameter.type=bswap_16(fp->parameter.type);
      tp->parameter.arraySize=bswap_32(fp->parameter.arraySize);
      tp->parameter.refName=(void*)bswap_32((int)(fp->parameter.refName));
      if (fp->qualifiers.used)
         l += copyI32toP32Qualifiers(ofs + l, to, &tp->qualifiers, from, &fp->qualifiers);
   }                          

   ts->sectionOffset = bswap_32(ofs);
   return ALIGN(l,CLALIGN);
}

static int p32SizeMethods(ClObjectHdr * hdr, ClSection * s)
{
   int l;
   long sz = s->used * sizeof(CLP32_ClMethod);
   ClMethod *m = (ClMethod *) ClObjectGetClSection(hdr, s);

   if (s->used == 0) return 0;
   
   for (l = s->used; l > 0; l--, m++) {
      if (m->qualifiers.used) 
         sz += p32SizeQualifiers(hdr, &m->qualifiers);
      if (m->parameters.used)
         sz += p32SizeParameters(hdr, &m->parameters);
   }      
   return ALIGN(sz,CLALIGN);
}

static int copyI32toP32Methods(int ofs, char *to, CLP32_ClSection * ts,
                           ClObjectHdr * from, ClSection * fs)
{
   ClMethod *fm = (ClMethod *) ClObjectGetClSection(from, fs);
   CLP32_ClMethod *tm = (CLP32_ClMethod *) (to + ofs);
   int i, l = fs->used * sizeof(CLP32_ClMethod);
   
   ts->max = bswap_16(fs->max);
   ts->used = bswap_16(fs->used);
   ts->sectionOffset = 0;

   if (l == 0) return 0;
   
   tm->quals = fm->quals;
   tm->originId = fm->originId;
   
   for (i = fs->used; i > 0; i--, fm++, tm++) {
      tm->id.id = bswap_32(fm->id.id);
      tm->type = bswap_16(fm->type);
      tm->flags = bswap_16(fm->flags);
      if (fm->qualifiers.used)
         l += copyI32toP32Qualifiers(ofs + l, to, &tm->qualifiers, from,
                             &fm->qualifiers);
      if (fm->parameters.used)
         l += copyI32toP32Parameters(ofs + l, to, &tm->parameters, from,
                             &fm->parameters);
   }                          

   ts->sectionOffset = bswap_32(ofs);
   return ALIGN(l,CLALIGN);
}

static long p32SizeStringBuf(ClObjectHdr * hdr)
{
   ClStrBuf *buf;
   long sz = 0;
   
   if (hdr->strBufOffset == 0) return 0;

   buf = getStrBufPtr(hdr);   

   sz = sizeof(CLP32_ClStrBuf) + ALIGN(buf->bUsed,4) + (buf->iUsed * sizeof(*buf->indexPtr));

   return ALIGN(sz,CLALIGN);
}

static int copyI32toP32StringBuf(int ofs, CLP32_ClObjectHdr * th, ClObjectHdr * fh)
{
   ClStrBuf *fb = getStrBufPtr(fh);
   CLP32_ClStrBuf *tb = (CLP32_ClStrBuf *) (((char *) th) + ofs);
   int i,  l, il;
   unsigned short flags;
   
   if (fh->strBufOffset == 0) {
      th->strBufOffset = 0;
      return 0;
   }

   l = sizeof(CLP32_ClStrBuf) + ALIGN(fb->bUsed,4);
   il = fb->iUsed * sizeof(*fb->indexPtr);

   tb->bMax=bswap_32(fb->bUsed);
   tb->bUsed=bswap_32(fb->bUsed);  
    
   flags = fh->flags &= ~HDR_StrBufferMalloced; 
   th->flags = bswap_16(flags);
   
   th->strBufOffset=bswap_32(ofs);      
   memcpy(tb->buf, fb->buf, l - (sizeof(*fb)-1));
   
   tb->iMax=bswap_16(fb->iUsed);
   tb->iUsed=bswap_16(fb->iUsed);
    
   tb->indexPtr=(int*)(((char*)th) + ofs+l);  
   tb->indexOffset=bswap_32(ofs+l);
   
   for (i=0; i<fb->iUsed; i++)
      tb->indexPtr[i]=bswap_32(fb->indexPtr[i]);
     
   return ALIGN(l + il,CLALIGN);
}



static long p32SizeArrayBuf(ClObjectHdr * hdr)
{
   ClArrayBuf *buf;
   long sz = 0;
   
   if (hdr->arrayBufOffset == 0) return 0;

   buf = getArrayBufPtr(hdr);   

   sz = sizeof(CLP32_ClArrayBuf) + (buf->bUsed * sizeof(CLP32_CMPIData)) +
       (buf->iUsed * sizeof(*buf->indexPtr));

   return ALIGN(sz,CLALIGN);
}

static int copyI32toP32ArrayBuf(int ofs, CLP32_ClObjectHdr * th, ClObjectHdr * fh)
{
   ClArrayBuf *fb = getArrayBufPtr(fh);
   CLP32_ClArrayBuf *tb = (CLP32_ClArrayBuf *) (((char *) th) + ofs);
   int i,  l, il;
   unsigned short flags;
   
   if (fh->arrayBufOffset == 0) {
      th->arrayBufOffset = 0;
      return 0;
   }

   l = sizeof(CLP32_ClArrayBuf) + (fb->bUsed * sizeof(CLP32_CMPIData));
   il = fb->iUsed * sizeof(*fb->indexPtr);

   tb->bMax=bswap_32(fb->bUsed);
   tb->bUsed=bswap_32(fb->bUsed); 
   tb->fillP32=0; 
     
   flags = fh->flags &= ~HDR_ArrayBufferMalloced;
   th->flags = bswap_16(flags);

   th->arrayBufOffset=bswap_32(ofs);
   for (i=0; i<fb->bUsed; i++)
      tb->buf[i]=copyI32toP32Data(fh, &fb->buf[i]);
   
   tb->iMax=bswap_16(fb->iUsed);
   tb->iUsed=bswap_16(fb->iUsed); 
   
   tb->indexPtr=(int*)(((char*)th) + ofs + l);  
   tb->indexOffset=bswap_32(ofs + l);
   
   for (i=0; i<fb->iUsed; i++)
      tb->indexPtr[i]=bswap_32(fb->indexPtr[i]);
     
   return ALIGN(l + il,CLALIGN);
}

static long p32SizeClassH(ClObjectHdr * hdr, ClClass * cls)
{
   long sz = sizeof(CLP32_ClClass);

   sz += p32SizeQualifiers(hdr, &cls->qualifiers);
   sz += p32SizeProperties(hdr, &cls->properties);
   sz += p32SizeMethods(hdr, &cls->methods);
   sz += p32SizeStringBuf(hdr);
   sz += p32SizeArrayBuf(hdr);

   return ALIGN(sz,CLALIGN);
}

static long p32SizeInstanceH(ClObjectHdr * hdr, ClInstance * inst)
{
   long sz = sizeof(CLP32_ClInstance);

   sz += p32SizeQualifiers(hdr, &inst->qualifiers);
   sz += p32SizeProperties(hdr, &inst->properties);
   sz += p32SizeStringBuf(hdr);
   sz += p32SizeArrayBuf(hdr);

   return ALIGN(sz,CLALIGN);
}

static long p32SizeQualifierH(ClObjectHdr * hdr, ClQualifierDeclaration * qual)
{
   long sz = sizeof(CLP32_ClQualifierDeclaration);

   sz += p32SizeQualifiers(hdr, &qual->qualifierData);
   sz += p32SizeStringBuf(hdr);
   sz += p32SizeArrayBuf(hdr);

   return ALIGN(sz,CLALIGN);
}

void *swapI32toP32Class(ClClass * cls, int *size)
{
   ClObjectHdr * hdr = &cls->hdr;
   int ofs = sizeof(CLP32_ClClass);
   int sz=p32SizeClassH(hdr,cls) + CLEXTRA;
   struct utsname uName;
   static int first=1;
   // char *cn;
   
   if (first) {
      uname(&uName);
   
      if (uName.machine[0]!='i' || strcmp(uName.machine+2,"86")!=0) {
         fprintf(stderr,"--- swapI32toP32Class can only execute on ix86 machines\n");
         exit(16);
      }
      first=0;
   }
   
   // cn=(char*)ClObjectGetClString(&cls->hdr, &cls->name);
   
   CLP32_ClClass *nc = (CLP32_ClClass *) calloc(1,sz);

   nc->hdr.size=bswap_32(sz);
   nc->hdr.flags=bswap_16(hdr->flags);
   nc->hdr.type=bswap_16(hdr->type);
   
   nc->quals=cls->quals;
   nc->parents=cls->parents;
   nc->reserved=bswap_16(cls->reserved);
   nc->name.id=bswap_32(cls->name.id);
   nc->parent.id=bswap_32(cls->parent.id);
   
   ofs += copyI32toP32Qualifiers(ofs, (char *) nc, &nc->qualifiers, hdr,
                         &cls->qualifiers);
   ofs += copyI32toP32Properties(ofs, (char *) nc, &nc->properties, hdr,
                         &cls->properties);
   ofs += copyI32toP32Methods(ofs,(char*)nc, &nc->methods, hdr,
                         &cls->methods);
                         
   ofs += copyI32toP32StringBuf(ofs, &nc->hdr, hdr);
   ofs += copyI32toP32ArrayBuf(ofs, &nc->hdr, hdr);
   
   *size = sz;
   if (CLEXTRA) memcpy(((char*)nc)+sz-4,"%%%%",4);
   
   return nc;
}

void *swapI32toP32Instance(ClInstance * inst, int *size)
{
   ClObjectHdr * hdr = &inst->hdr;
   int ofs = sizeof(CLP32_ClInstance);
   int sz=p32SizeInstanceH(hdr,inst) + CLEXTRA;
   struct utsname uName;
   static int first=1;
   
   if (first) {
      uname(&uName);
   
      if (uName.machine[0]!='i' || strcmp(uName.machine+2,"86")!=0) {
         fprintf(stderr,"--- swapI32toP32Instance can only execute on ix86 machines\n");
         exit(16);
      }
      first=0;
   }
   
   CLP32_ClInstance *ni = (CLP32_ClInstance *) calloc(1,sz);

   ni->hdr.size=bswap_32(sz);
   ni->hdr.flags=bswap_16(hdr->flags);
   ni->hdr.type=bswap_16(hdr->type);
   
   ni->quals=inst->quals;
   ni->parents=inst->parents;
   ni->reserved=bswap_16(inst->reserved);
   ni->className.id=bswap_32(inst->className.id);
   ni->nameSpace.id=bswap_32(inst->nameSpace.id);
   ofs += copyI32toP32Qualifiers(ofs, (char *) ni, &ni->qualifiers, hdr,
                         &inst->qualifiers);
   ofs += copyI32toP32Properties(ofs, (char *) ni, &ni->properties, hdr,
                         &inst->properties);
                         
   ofs += copyI32toP32StringBuf(ofs, &ni->hdr, hdr);
   ofs += copyI32toP32ArrayBuf(ofs, &ni->hdr, hdr);
   
   *size = sz;
   if (CLEXTRA) memcpy(((char*)ni)+sz-4,"%%%%",4);
   
   return ni;
}

void *swapI32toP32QualifierDeclaration(ClQualifierDeclaration * qual, int *size)
{
   ClObjectHdr * hdr = &qual->hdr;
   int ofs = sizeof(CLP32_ClQualifierDeclaration);
   int sz=p32SizeQualifierH(hdr,qual) + CLEXTRA;
   struct utsname uName;
   static int first=1;
   
   if (first) {
      uname(&uName);
   
      if (uName.machine[0]!='i' || strcmp(uName.machine+2,"86")!=0) {
         fprintf(stderr,"--- swapI32toP32QualifierDeclaration can only execute on ix86 machines\n");
         exit(16);
      }
      first=0;
   }
   
   CLP32_ClQualifierDeclaration *nq = (CLP32_ClQualifierDeclaration *) calloc(1,sz);

   nq->hdr.size=bswap_32(sz);
   nq->hdr.flags=bswap_16(hdr->flags);
   nq->hdr.type=bswap_16(hdr->type);
   
   nq->flavor=qual->flavor;
   nq->scope=qual->scope;
   nq->type=bswap_16(qual->type);
   nq->arraySize=bswap_32(qual->arraySize);
   nq->qualifierName.id=bswap_32(qual->qualifierName.id);
   nq->nameSpace.id=bswap_32(qual->nameSpace.id);  
   
   ofs += copyI32toP32Qualifiers(ofs, (char *) nq, &nq->qualifierData, hdr,
                         &qual->qualifierData);
   ofs += copyI32toP32StringBuf(ofs, &nq->hdr, hdr);
   ofs += copyI32toP32ArrayBuf(ofs, &nq->hdr, hdr);
   
   *size = sz;
   if (CLEXTRA) memcpy(((char*)nq)+sz-4,"%%%%",4);
   
   return nq;
}
