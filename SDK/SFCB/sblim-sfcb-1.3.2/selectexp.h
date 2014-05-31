
/*
 * selectexp.h
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
 * CMPISelectExp implementation.
 *
*/


#ifndef SELECTEXP_H
#define SELECTEXP_H 

#include "native.h"
#include "queryOperation.h"

typedef struct native_selectexp NativeSelectExp;

struct native_selectexp {
   CMPISelectExp exp;
   int mem_state;
   NativeSelectExp *next;
   char *queryString;
   char *language;
   char *sns;
   void *filterId;
   QLStatement *qs;
};

#endif
