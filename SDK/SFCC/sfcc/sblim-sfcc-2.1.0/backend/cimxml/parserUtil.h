/*
 * $id$
 *
 *  © Copyright IBM Corp. 2007
 *
 * THIS FILE IS PROVIDED UNDER THE TERMS OF THE ECLIPSE PUBLIC LICENSE
 * ("AGREEMENT"). ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS FILE
 * CONSTITUTES RECIPIENTS ACCEPTANCE OF THE AGREEMENT.
 *
 * You can obtain a current copy of the Eclipse Public License from
 * http://www.opensource.org/licenses/eclipse-1.0.php
 *
 * Author:       Sven Schuetz <sven@de.ibm.com>
 *
 * Description:
 *
 * CIM XML parser helper functions header file
 *
*/

#include "cimXmlParser.h"


#define PARSER_MALLOC(s) parser_malloc(parm->heap,(s))


extern CMPIConstClass * native_new_CMPIConstClass ( char  *cn, CMPIStatus * rc );

static inline int isBoolean(CMPIData data);
void createPath(CMPIObjectPath **op, XtokInstanceName *p);
void setInstProperties(CMPIInstance *ci, XtokProperties *ps);
void setInstQualifiers(CMPIInstance *ci, XtokQualifiers *qs);
void setClassProperties(CMPIConstClass *cls, XtokProperties *ps);
void setClassQualifiers(CMPIConstClass *cls, XtokQualifiers *qs);
void addProperty(ParserControl *parm, XtokProperties *ps, XtokProperty *p);
void addParamValue(ParserControl *parm, XtokParamValues *vs, XtokParamValue *v);
void addKeyBinding(ParserControl *parm, XtokKeyBindings *ks, XtokKeyBinding *k);
void addQualifier(ParserControl *parm, XtokQualifiers *qs, XtokQualifier *q);
void addMethod(ParserControl *parm, XtokMethods *ms, XtokMethod *m);
void addParam(ParserControl *parm, XtokParams *ps, XtokParam *p);
void setError(ParserControl *parm, XtokErrorResp *e);
void setReturnArgs(ParserControl *parm, XtokParamValues *ps);

