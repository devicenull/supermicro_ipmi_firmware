
/*
 * utilFactory.c
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
 * Encapsulated utility factory implementation.
 *
*/



#include "utilft.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern UtilHashTable *newHashTable(long buckets, long opt);
extern UtilHashTable *newHashTableDefault(long buckets);
extern UtilList *newList();
extern UtilStringBuffer *newStringBuffer(int s);

static Util_Factory_FT ift = {
   1,
   newHashTableDefault,
   newHashTable,
   newList,
   newStringBuffer
};


Util_Factory_FT *UtilFactory = &ift;
