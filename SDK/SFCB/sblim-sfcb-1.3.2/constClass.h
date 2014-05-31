
/*
 * constClass.h
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
 * Author:        Frank Scheffler
 * Contributions: Adrian Schuur <schuur@de.ibm.com>
 *
 * Description:
 *
 * Internal CMPIConstClass implementation.
 *
*/

#ifndef CMPI_constClass_h
#define CMPI_constClass_h

#include "cmpidt.h"

#define MALLOCED(a) (((a) & 0xff000000)!=0xff000000)

#define FL_assocsOnly 64

struct _CMPIConstClass_FT;
typedef struct _CMPIConstClass_FT CMPIConstClass_FT;

struct _CMPIConstClass {
   void *hdl;
   CMPIConstClass_FT *ft;
   int refCount;
};
typedef struct _CMPIConstClass CMPIConstClass;

struct _CMPIConstClass_FT { 
   int version;
  /* functions defined in cimcift.h */
   CMPIStatus(*release) (CMPIConstClass * cc);
   CMPIConstClass *(*clone) (CMPIConstClass * cc, CMPIStatus * rc); 
   CMPIString *(*getClassName)(CMPIConstClass* cc, CMPIStatus * rc);
   CMPIData (*getProperty) (CMPIConstClass * cc, const char *prop, CMPIStatus * rc);
   CMPIData (*getPropertyAt) (CMPIConstClass * cc, CMPICount i,
                             CMPIString ** name,  CMPIStatus * rc);
   CMPICount (*getPropertyCount) (CMPIConstClass * cc, CMPIStatus * rc);
   
   CMPIData (*getQualifier) (CMPIConstClass * cc, const char *qual, CMPIStatus * rc);
   CMPIData (*getQualifierAt) (CMPIConstClass * cc, CMPICount i,
                              CMPIString ** name, CMPIStatus * rc);
   CMPICount (*getQualifierCount) (CMPIConstClass * cc, CMPIStatus * rc);
   
   CMPIData (*getPropQualifier) (CMPIConstClass * cc, const char *prop,
                                   const char *qual, CMPIStatus * rc);
   CMPIData (*getPropQualifierAt) (CMPIConstClass * cc, const char *prop,
                                   CMPICount i, CMPIString ** name,
                                   CMPIStatus * rc);
   CMPICount (*getPropQualifierCount) (CMPIConstClass * cc, const char * prop, CMPIStatus * rc);
   
  /* local functions for sfcb */
   CMPIString *(*getSuperClassName)(CMPIConstClass* cc, CMPIStatus * rc);   
   CMPIArray *(*getKeyList) (CMPIConstClass * cc);
   char *(*toString) (CMPIConstClass * cc);     
   void (*relocate) (CMPIConstClass * cc);
   const char *(*getCharClassName) (CMPIConstClass * br);
   const char *(*getCharSuperClassName) (CMPIConstClass * br);
   CMPIBoolean(*isAssociation) (CMPIConstClass * cc);
   CMPIBoolean(*isAbstract) (CMPIConstClass * cc);
   CMPIBoolean(*isIndication) (CMPIConstClass * cc);
   CMPIData(*getPropQualAt) (CMPIConstClass * cc, CMPICount p, CMPICount i, CMPIString ** name,
                                   CMPIStatus * rc);
};

extern CMPIData getPropertyQuals (CMPIConstClass * cc, const char *prop,
                            unsigned long *quals, CMPIStatus * rc);
extern CMPIData getPropertyQualsAt (CMPIConstClass * cc, CMPICount i,
                              CMPIString ** name, unsigned long *quals, CMPIString ** refName,
                              CMPIStatus * rc);
extern CMPIData internalGetPropQualAt (CMPIConstClass * cc, CMPICount p,
                              CMPICount i, CMPIString ** name, CMPIStatus * rc);
extern CMPIConstClass_FT *CMPIConstClassFT;

//   extern CMPIConstClass* newCMPIConstClass(const char *cn, const char *pn);

#endif
