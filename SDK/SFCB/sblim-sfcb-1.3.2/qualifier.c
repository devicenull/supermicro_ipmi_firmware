
/*
 * $Id: qualifier.c,v 1.5 2007/10/02 09:02:11 mihajlov Exp $
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
 * Description:
 *
 * Internal qualifier support for sfcb.
 *
*/

#include "objectImpl.h"
#include "qualifier.h"

extern CMPIArray *native_make_CMPIArray(CMPIData * av, CMPIStatus * rc, ClObjectHdr * hdr);

static CMPIQualifierDecl *qual_clone(CMPIQualifierDecl * q, CMPIStatus * rc);

static CMPIStatus release(CMPIQualifierDecl* q)
{
   CMPIStatus rc = { 0, NULL };

   if (q->hdl)  {
     ClQualifierFree(q->hdl);
   }
   return rc;
}

static const char* getCharQualifierName(CMPIQualifierDecl * cq)
{
   ClQualifierDeclaration *q = (ClQualifierDeclaration *) cq->hdl;
   if (q->qualifierName.id)
      return ClObjectGetClString(&q->hdr, &q->qualifierName);
   return NULL;
}

static CMPIData getQualifierDeclData(CMPIQualifierDecl * cq, CMPIStatus * rc)
{
   ClQualifierDeclaration *q = (ClQualifierDeclaration *) cq->hdl;

   CMPIData rv = { 0, CMPI_notFound, {0} };
   if (ClQualifierDeclarationGetQualifierData(q,&rv)) {
      if (rc) CMSetStatus(rc, CMPI_RC_ERR_NOT_FOUND);
      return rv;
   }
   if (rv.type == CMPI_chars) {
      rv.value.string = sfcb_native_new_CMPIString(ClObjectGetClString
						   (&q->hdr, (ClString *) & rv.value.chars), NULL, 0);
      rv.type = CMPI_string;
   }
   if (rv.type & CMPI_ARRAY && rv.value.array) {
      rv.value.array = native_make_CMPIArray((CMPIData *) rv.value.array, 
         NULL, &q->hdr);
   }
   
   if (rc) CMSetStatus(rc, CMPI_RC_OK);
   return rv;
}

static struct _CMPIQualifierDecl_FT ift = {
   1,
   release,
   qual_clone,
   getCharQualifierName,
   getQualifierDeclData
};

CMPIQualifierDecl_FT *CMPIQualifierDeclFT = &ift;

static CMPIQualifierDecl *qual_clone(CMPIQualifierDecl * cq, CMPIStatus * rc)
{
   CMPIQualifierDecl *q = (CMPIQualifierDecl *) malloc(getQualifierSerializedSize(cq));
   q->hdl = q + 1;
   q->ft = &ift;
   ClQualifierRebuildQualifier((ClQualifierDeclaration *) cq->hdl, q->hdl);
   if (rc)
      rc->rc = 0;
         
   return q;
}

unsigned long getQualifierSerializedSize(CMPIQualifierDecl * q)
{
   ClQualifierDeclaration *clq = (ClQualifierDeclaration *) q->hdl;
   return ClSizeQualifierDeclaration(clq) + sizeof(CMPIQualifierDecl);
}

MsgSegment setQualifierMsgSegment(CMPIQualifierDecl * q)
{
   MsgSegment s;
   s.data = q;
   s.type = MSG_SEG_QUALIFIER;
   s.length = getQualifierSerializedSize(q);
   return s;
}

void getSerializedQualifier(CMPIQualifierDecl * q, void *area)
{
   memcpy(area, q, sizeof(CMPIQualifierDecl));
   ClQualifierRebuildQualifier((ClQualifierDeclaration *) q->hdl,
                       (void *) ((char *) area + sizeof(CMPIQualifierDecl)));
   ((CMPIQualifierDecl *)(area))->hdl =
   			(ClQualifierDeclaration *) ((char *) area + sizeof(CMPIQualifierDecl));                       
}

CMPIQualifierDecl initQualifier(ClQualifierDeclaration *qual)
{
   CMPIQualifierDecl q;
   q.hdl=qual;
   q.ft=&ift;
   return q;
}

CMPIQualifierDecl *relocateSerializedQualifier(void *area)
{
   CMPIQualifierDecl *q = (CMPIQualifierDecl *) area;
   q->hdl = q + 1;
   q->ft = &ift;
   ClQualifierRelocateQualifier((ClQualifierDeclaration *) q->hdl);
   return (CMPIQualifierDecl *) q;
}
