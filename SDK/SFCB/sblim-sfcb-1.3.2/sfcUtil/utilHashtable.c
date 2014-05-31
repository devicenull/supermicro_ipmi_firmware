
/*
 * utilHashtable.c
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
 * Hashtable factory support.
 *
*/


#include "utilft.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

extern void *HashTableCreate(long numOfBuckets);
extern Util_HashTable_FT *UtilHashTableFT;

static unsigned long charHashFunction(const void *key)
{
   const unsigned char *str = (const unsigned char *) key;
   unsigned long hashValue = 0;
   int i;

   for (i = 0; str[i] != '\0'; i++)
      hashValue = hashValue * 37 + str[i];

   return hashValue;
}

static int charCmpFunction(const void *pointer1, const void *pointer2)
{
   return strcmp((char *) pointer1, (char *) pointer2);
}

static int ptrCmpFunction(const void *pointer1, const void *pointer2)
{
   return (pointer1 != pointer2);
}

/*
static unsigned long ptrHashFunction(const void *pointer) {
    return ((unsigned long) pointer) >> 4;
}
*/
typedef struct _CMPI_String {
   void *hdl;
   void *ft;
} CMPI_String;

static unsigned long charIcHashFunction(const void *key)
{
   const unsigned char *str = (const unsigned char *) key;
   unsigned long hashValue = 0;
   int i;

   for (i = 0; str[i] != '\0'; i++)
      hashValue = hashValue * 37 + toupper(str[i]);

   return hashValue;
}

static int charIcCmpFunction(const void *pointer1, const void *pointer2)
{
   return strcasecmp((char *) pointer1, (char *) pointer2);
}



static unsigned long cmpiStringIcHashFunction(const void *key)
{
   return charIcHashFunction((const void *) ((CMPI_String *) key)->hdl);
}

static int cmpiStringIcCmpFunction(const void *p1, const void *p2)
{
   return strcasecmp((char *) ((CMPI_String *) p1)->hdl,
                     (char *) ((CMPI_String *) p2)->hdl);
}



static unsigned long cmpiStringHashFunction(const void *key)
{
   return charHashFunction((const void *) ((CMPI_String *) key)->hdl);
}

static int cmpiStringCmpFunction(const void *p1, const void *p2)
{
   return strcmp((char *) ((CMPI_String *) p1)->hdl,
                 (char *) ((CMPI_String *) p2)->hdl);
}



UtilHashTable *newHashTableDefault(long buckets)
{
   UtilHashTable *ht = (UtilHashTable *) malloc(sizeof(UtilHashTable));
   void *t = HashTableCreate(buckets);

   ht->hdl = t;
   ht->ft = UtilHashTableFT;

   UtilHashTableFT->setHashFunction(ht, charHashFunction);
   UtilHashTableFT->setKeyCmpFunction(ht, charCmpFunction);
   UtilHashTableFT->setValueCmpFunction(ht, ptrCmpFunction);
   UtilHashTableFT->setReleaseFunctions(ht, free, free);

   return ht;
}



UtilHashTable *newHashTable(long buckets, long opt)
{
   UtilHashTable *ht = (UtilHashTable *) malloc(sizeof(UtilHashTable));
   void *t = HashTableCreate(buckets);
   void (*keyRelease) (void *key) = NULL;
   void (*valueRelease) (void *value) = NULL;

   ht->hdl = t;
   ht->ft = UtilHashTableFT;

   if (opt & UtilHashTable_charKey) {
      if (opt & UtilHashTable_ignoreKeyCase) {
         UtilHashTableFT->setHashFunction(ht, charIcHashFunction);
         UtilHashTableFT->setKeyCmpFunction(ht, charIcCmpFunction);
      }
      else {
         UtilHashTableFT->setHashFunction(ht, charHashFunction);
         UtilHashTableFT->setKeyCmpFunction(ht, charCmpFunction);
      }
   }

   else if (opt & UtilHashTable_CMPIStringKey) {
      if (opt & UtilHashTable_ignoreKeyCase) {
         UtilHashTableFT->setHashFunction(ht, cmpiStringIcHashFunction);
         UtilHashTableFT->setKeyCmpFunction(ht, cmpiStringIcCmpFunction);
      }
      else {
         UtilHashTableFT->setHashFunction(ht, cmpiStringHashFunction);
         UtilHashTableFT->setKeyCmpFunction(ht, cmpiStringCmpFunction);
      }
   }

   if (opt & UtilHashTable_charValue) {
      if (opt & UtilHashTable_ignoreValueCase)
         UtilHashTableFT->setValueCmpFunction(ht, charIcCmpFunction);
      else
         UtilHashTableFT->setValueCmpFunction(ht, charCmpFunction);
   }
   else
      UtilHashTableFT->setValueCmpFunction(ht, ptrCmpFunction);

   if (opt & UtilHashTable_managedKey) {
      if (opt & UtilHashTable_CMPIStringKey)
         keyRelease = NULL;
      else
         keyRelease = free;
   }

   if (opt & UtilHashTable_managedValue) {
      if (opt & UtilHashTable_CMPIStyleValue)
         valueRelease = NULL;
      else
         valueRelease = free;
   }
   UtilHashTableFT->setReleaseFunctions(ht, keyRelease, valueRelease);

   return ht;
}
