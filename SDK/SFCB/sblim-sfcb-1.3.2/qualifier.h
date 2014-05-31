
/*
 * $Id: qualifier.h,v 1.1 2006/09/28 11:46:10 sschuetz Exp $
 *
 * (C) Copyright IBM Corp. 2006
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sven Schuetz <sven@de.ibm.com>
 *		based on constClass concept by Adrian Schuur.
 *
 * Description:
 *
 * Internal qualifier implementation.
 *
*/

#ifndef CMPI_qualifier_h
#define CMPI_qualifier_h

#include "cmpidt.h"
//#include "objectImpl.h"   -> causes compile error...circular dependency ?
#include "msgqueue.h"

struct _CMPIQualifierDecl_FT;
typedef struct _CMPIQualifierDecl_FT CMPIQualifierDecl_FT;

struct _CMPIQualifierDecl {
   void *hdl;
   CMPIQualifierDecl_FT *ft;
};
typedef struct _CMPIQualifierDecl CMPIQualifierDecl;

struct _CMPIQualifierDecl_FT { 
   int version;
   CMPIStatus(*release) (CMPIQualifierDecl * cc);
   CMPIQualifierDecl *(*clone) (CMPIQualifierDecl * cc, CMPIStatus * rc); 
   const char *(*getCharQualifierName)(CMPIQualifierDecl* cc);
   CMPIData (*getQualifierDeclData) (CMPIQualifierDecl * cq, CMPIStatus * rc);
};

extern CMPIQualifierDecl_FT *CMPIQualifierDeclFT;

MsgSegment setQualifierMsgSegment(CMPIQualifierDecl * q);
void getSerializedQualifier(CMPIQualifierDecl * q, void *area);
//CMPIQualifierDecl initQualifier(ClQualifierDeclaration *qual);
CMPIQualifierDecl *relocateSerializedQualifier(void *area);
unsigned long getQualifierSerializedSize(CMPIQualifierDecl * q);

#endif
